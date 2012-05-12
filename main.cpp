/* 
 * File:   main.cpp
 * Author: lubos
 *
 * Created on September 23, 2011, 3:56 PM
 */
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>


//#define DEBUG
#define COUNT_OF_TILES 21
#define MSG_INPUT_DATA     1


#include "Exception.h"
#include "Stack.h"
#include "TileTypes.h"
#include "Tile.h"
#include "Playground.h"
#include "SequenceAlgorithm.h"
#include "ParallelAlgorithm.h"



Tile** 	initTiles();
void 		eraseTiles();
double 	getTime(clock_t clock1,clock_t clock2);
int* 		readFromFile(char* path, int count);
int* 		readFromStdInput(int count);
void 		printUsage();


int* readFromFile(char* path, int count)
{
	int a;
	int* data = new int[count];
	std::ifstream input;	

	input.open(path);

	if(!input)
		std::cerr << "Can't open file." << std::endl;

	for(int i = 0; !input.eof() && i < count; i++)
	{
		input >> a;	
		data[i] = a;
	}

	input.close();	
	return data;
}


int* readFromStdInput(int count)
{
	int* holeIndexes = new int[count];
	int a;

	for(int i = 0; i < count; i++)
	{
		std::cout << "Insert position of hole " << i+1 << std::endl;
		std::cin >> a;
	    	std::cin.get();
		holeIndexes[i] = a;
	}
	return holeIndexes;
}


void printUsage()
{
	std::cout << "USAGE:" << std::endl;
	std::cout << "terminal:\t ./main -l <height> <width> <countFfHoles>" << std::endl;
	std::cout << "file:\t\t ./main -f <height> <width> <countOfHoles> <inputFilePath>" << std::endl;
}


double getTime(time_t end, time_t begin)
{
	double diffms = end - begin;
	diffms = (diffms * 1000) / CLOCKS_PER_SEC;
	return diffms;
}


Tile** initTiles() 
{
	Tile** tiles = new Tile*[COUNT_OF_TILES];
    	tiles[0] = new Tile(hole, 1, 1, -6); 

    	tiles[1] = new Tile(L1a, 2, 2, 1);
    	tiles[2] = new Tile(L1b, 2, 2, 1, true);
    	tiles[3] = new Tile(L1c, 2, 2, 1);
    	tiles[4] = new Tile(L1d, 2, 2, 1);
    	tiles[5] = new Tile(L2b, 3, 2, 2); 
    	tiles[6] = new Tile(L2d, 3, 2, 2, true);
    	tiles[7] = new Tile(L2f, 3, 2, 2);
    	tiles[8] = new Tile(L2h, 3, 2, 2);
    	tiles[9] = new Tile(L3b, 4, 2, 3);
    	tiles[10] = new Tile(L3d, 4, 2, 3, true);
    	tiles[11] = new Tile(L3f, 4, 2, 3);
    	tiles[12] = new Tile(L3h, 4, 2, 3);
    	tiles[13] = new Tile(L2a, 2, 3, 2);
    	tiles[14] = new Tile(L2c, 2, 3, 2);
    	tiles[15] = new Tile(L2e, 2, 3, 2);
    	tiles[16] = new Tile(L2g, 2, 3, 2, true);
	tiles[17] = new Tile(L3a, 2, 4, 3);
	tiles[18] = new Tile(L3c, 2, 4, 3);
	tiles[19] = new Tile(L3g, 2, 4, 3, true);
	tiles[20] = new Tile(L3e, 2, 4, 3);

	return tiles;
}


void eraseTiles(Tile** tiles)
{
	for(int i = 0; i < COUNT_OF_TILES; i++)
		delete tiles[i];

	delete [] tiles;
}


int main(int argc, char** argv) 
{
	MPI_Init(&argc, &argv);
	time_t begin, end;

	int  width;
	int  height;
	int  countOfHoles;
	int* holes;
	int  myRank;
	int  countOfProcesses;
	MPI_Status status;

	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    	MPI_Comm_size(MPI_COMM_WORLD, &countOfProcesses);
	
	if(argc >= 6)
	{
		width = atoi(argv[3]);
		height = atoi(argv[4]);
		countOfHoles = atoi(argv[5]);
		
		
		if(countOfHoles > 0)
		{
			if(myRank == 0)
			{	
				std::cout << "myRank == " << myRank << std::endl;

				if(strncmp(argv[2], "-l", 2) == 0) //pozice nutnych der se zadavaji z radky
					holes = readFromStdInput(countOfHoles);
				else if(strncmp(argv[2], "-f", 2) == 0) //ze souboru se nacitaji pozice nutnych der
					holes = readFromFile(argv[6], countOfHoles);

				for(int i = 1; i < countOfProcesses; i++)
				MPI_Send(holes, countOfHoles, MPI_INT, i, MSG_INPUT_DATA, MPI_COMM_WORLD);
			}
			else
			{	
				int flag = 0;
				int source = 0;
				while(true)
				{
					MPI_Iprobe(source, MSG_INPUT_DATA, MPI_COMM_WORLD, &flag, &status);
		
					if(flag)
					{
						holes = new int[countOfHoles];
						MPI_Recv(holes, countOfHoles, MPI_INT, source, MSG_INPUT_DATA, MPI_COMM_WORLD, &status);
						std::cout << "Process #" << myRank << " recieved input data from " << source << "." << std::endl;

						for(int i = 0; i < countOfHoles; i++)
							std::cout << holes[i] << " ";
						std::cout << std::endl;

						break;
					}		
				}
			}
		}

    		Tile** tiles = initTiles();

      	Playground playground(width, height, tiles, countOfHoles, holes); //v konstruktoru pak vytvori 2D pole intů o velikosti n,m	

		if(strncmp(argv[1], "-s", 2) == 0)
		{
			SequenceAlgorithm sequence(&playground);
			begin = clock();
    			sequence.run();
			end = clock();
			playground.printSolution();
			std::cout << "Sequence simulation takes: " << static_cast<double>(getTime(end,begin)) << " ms"<< std::endl;
      	}
		else if(strncmp(argv[1], "-p", 2) == 0)
		{
			ParallelAlgorithm parallel(&playground, argc, argv);
			parallel.mainLoop();
//			std::cout << "Paralel simulation takes: " << static_cast<double>(getTime(end,begin)) << " ms"<< std::endl;
		}
		else
		{
			if(myRank == 0)
				printUsage();
		}
		
		if(countOfHoles > 0)
			delete [] holes;
		eraseTiles(tiles);
	}
	else
	{
		if(myRank == 0)
			printUsage();
	}
	MPI_Finalize();
    	return 0;
}

