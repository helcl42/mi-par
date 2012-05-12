#include "ParallelAlgorithm.h"


ParallelAlgorithm::ParallelAlgorithm(Playground* playground, int argc, char** argv)
:	playground(playground), startFlag(true)
{	    	
    	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    	MPI_Comm_size(MPI_COMM_WORLD, &countOfProcesses);

	ahdRank = (myRank == 0) ? countOfProcesses - 1 : myRank - 1;
	token = (myRank == 0) ? 1 : 0;

	tokenColor = WHITE;
	processColor = WHITE;

	previousOutcomingWorkRequestRank = -1;
	previousIncomingWorkRequestRank = -1;
}


ParallelAlgorithm::~ParallelAlgorithm()  
{
}



void ParallelAlgorithm::mainLoop(void)
{
	double t1, t2;
	int response;

	MPI_Barrier (MPI_COMM_WORLD);
	t1 = MPI_Wtime();		
	
	if(countOfProcesses > 1)
	{
		if(myRank != 0)
			startAcceptData();

		while(true)
		{
			response = run();		
			if(response)
				break;

			requestMoreWork();
			communicationLoop();
			response = checkWorkEnd();
			if(response)
				break;			
		}
	}
	else
	{
		run();
	}

	t2 = MPI_Wtime();
	MPI_Barrier (MPI_COMM_WORLD);

	if (myRank == 0)
	{
		std::cout << "Takes: " << (t2 - t1) << std::endl;
		playground->printSolution();		
	}	
}



void ParallelAlgorithm::start(void)
{
	int* data;
	int lengthOfData = 2;

	if(myRank == 0)
	{
		std::cout << "Original Stack: " << std::endl;
		std::cout << stck << std::endl;
		int pos = 2;

		for(int dest = 1; dest < countOfProcesses; dest++)
		{
			data = new int[lengthOfData];
			for(int i = 0; i < lengthOfData; i++, pos++)
				data[i] = stck[pos];
			
			MPI_Send(data, lengthOfData, MPI_INT, dest, MSG_WORK_TRANSMIT, MPI_COMM_WORLD);

		//	for(int i = 0; i < lengthOfData; i++ )
		//		std::cout << data[i] << " ";
		//	std::cout << std::endl;
			delete [] data;
		}

		while(stck.getSize() > 2)
			stck.pop();
	}
}


void ParallelAlgorithm::startAcceptData()
{
	int* data;
	int size = 0;
	int flag = 0;

	while(true)
	{
		MPI_Iprobe(MPI_ANY_SOURCE, MSG_WORK_TRANSMIT, MPI_COMM_WORLD, &flag, &status);
		
		if(flag)
		{
			MPI_Get_count(&status, MPI_INT, &size);

			if(size > 1)
			{
				data = new int[size];
				MPI_Recv(data, size, MPI_INT, MPI_ANY_SOURCE, MSG_WORK_TRANSMIT, MPI_COMM_WORLD, &status);								

				std::cout << "Process #" << myRank << " recieves start data from process #" << status.MPI_SOURCE << std::endl;
				while(!stck.isEmpty())
					stck.pop();
	
				for(int i = 0 ; i < size; i++)
					stck.push(data[i]);

		//		std::cout << stck << std::endl;		
	
//				playground->initSquares();	
//				playground->insertHoles();

				for(unsigned int i = 0; i < stck.getSize() - 1; i += 2)
				{
					int position = stck[i + 1];			
					int x = position / playground->getWidth();
					int y = position % playground->getWidth();					

					playground->insertTile(x, y, stck[i]);
				}

				delete [] data;
			}
			break;
		}
	}
}


int ParallelAlgorithm::getDonorProcess()
{
	ahdRank++;

	//modulo poctem procesu
	ahdRank %= countOfProcesses;

	//abychom neposilali request na praci sami sobe
	if(ahdRank == myRank)
	{
		ahdRank++;
		ahdRank %= countOfProcesses;
	}
		
	return ahdRank;			
}


int ParallelAlgorithm::communicationLoop(void)
{
	for(int processId = 0; processId < countOfProcesses; processId++)
	{
//		if(processId == myRank) 
//			continue;

		int flag = 0;
    		MPI_Iprobe(processId, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);

    		if (flag)
    		{
      		//v promenne status je tag (status.MPI_TAG), cislo odesilatele (status.MPI_SOURCE)
      		//a pripadne cislo chyby (status.MPI_ERROR)
      		switch (status.MPI_TAG)
      		{
         			case MSG_WORK_REQUEST : // zadost o praci, prijmout a dopovedet
					checkWorkRequest();
					break;

         			case MSG_WORK_TRANSMIT : // prisel rozdeleny zasobnik, prijmout
					checkIncommingWork();
                              break;

         			case MSG_WORK_NOWORK : // odmitnuti zadosti o praci
					std::cout << "Process #" << myRank << " recieved MSG_NO_WORK from #" << status.MPI_SOURCE << std::endl;
                              // zkusit jiny proces
//					requestMoreWork();
					break;

         			case MSG_TOKEN : //ukoncovaci token, prijmout a nasledne preposlat
					checkToken();					
                          	break;

				case MSG_SOLUTION_TRANSMIT: //prijmeme reseni od jineho procesu
					checkSolution();
					break;

         			case MSG_END_WORK : //konec vypoctu - proces 0 pomoci tokenu zjistil, ze jiz nikdo nema praci
					std::cout << "Process #" << myRank << " recieved MSG_END_WORK from " << status.MPI_SOURCE << std::endl;
					checkWorkEnd();
					return 2;
                           	break;

				default :
					throw new Exception("ERROR: Unknown message.");
      		}
    		}
	}
	return 0;
}


void ParallelAlgorithm::requestMoreWork(void)
{	
	if(token)
	{ 
		std::cout << "REQUEST WORK!" << std::endl;	
		int nextEmployedProcess = (myRank == countOfProcesses - 1) ? 0 : myRank + 1;

		if(myRank == 0)
			tokenColor = WHITE;

		std::cout << "Process " << myRank << " sends TOKEN with color " << (tokenColor == WHITE ? "WHITE" : "BLACK") << " to process " << nextEmployedProcess << std::endl; 
		MPI_Send(&tokenColor, 1, MPI_INT, nextEmployedProcess, MSG_TOKEN, MPI_COMM_WORLD);
		token = 0;

		//dokonci se predchozi zadost o praci
		if(previousOutcomingWorkRequestRank >= 0)
			MPI_Wait(&outcomingWorkRequest, &status);
	}

	processColor = WHITE;

	if(previousOutcomingWorkRequestRank >= 0){
		MPI_Wait(&outcomingWorkRequest, &status);
	}

    	printf("Process #%d sending MSG_WORK_REQUEST to process #%d.\n", myRank, ahdRank);

	//zasleme zadost o praci
	MPI_Isend(&tokenColor, 1, MPI_INT, ahdRank, MSG_WORK_REQUEST, MPI_COMM_WORLD, &outcomingWorkRequest);
}
	


void ParallelAlgorithm::checkWorkRequest(void)
{
	for(int i = 0; i < countOfProcesses; i++)
	{
		if(i == myRank) 
			continue;

		int flag = 0;
		MPI_Iprobe(i, MSG_WORK_REQUEST, MPI_COMM_WORLD, &flag, &status);

		if(flag)
		{
			//dokonceni predchoziho sendu
			if(previousIncomingWorkRequestRank >= 0) {
				MPI_Wait(&incomingWorkRequest, &status);
			}

			//prijme se zadost o praci
			int message;
			MPI_Recv(&message, 1, MPI_INT, i, MSG_WORK_REQUEST, MPI_COMM_WORLD, &status);
			printf("Process #%d recieved MSG_WORK_REQUEST %d from process #%d.\n", myRank, message , i);
			
			if(stck.getSize() > MIN_STACK_SIZE_FOR_DIVIDE)				
			{	
//				std::cout << "Original Stack:" << std::endl; 
//				std::cout << stck << std::endl;
				unsigned int ratio = stck.getSizeOfHalfStack();
				unsigned int size = stck.getSize() - ratio;
				int* data = stck / STACK_DIV;

				while(stck.getSize() > ratio)
					stck.pop();

				printf("Process #%d sends data with size %d to process #%d.\n", myRank, size, i);

				MPI_Isend(data, size, MPI_INT, i, MSG_WORK_TRANSMIT, MPI_COMM_WORLD, &incomingWorkRequest);
				previousIncomingWorkRequestRank = i;

				//pokud je zadatel niz nez my, nastaci se barva pska na BLACK
				if(myRank > i)
					processColor = BLACK;			
							
				delete [] data;
			}
			else
			{	
				printf("Process #%d sends MSG_WORK_NOWORK to process #%d.\n", myRank, i);	
				MPI_Send(&tokenColor, 1, MPI_INT, i, MSG_WORK_NOWORK, MPI_COMM_WORLD);
			}
		}
	}
}


bool ParallelAlgorithm::checkIncommingWork()
{
	int* data;
	int  size = 0;

	MPI_Get_count(&status, MPI_INT, &size);
	
	if(size > 0)
	{	
		data = new int[size];
		MPI_Recv(data, size, MPI_INT, ahdRank, MSG_WORK_TRANSMIT, MPI_COMM_WORLD, &status);
		printf("Process #%d received data with size %d from process #%d.\n", myRank, size, ahdRank);

		//ahdRank = getDonorProcess();

		//inkrementujeme citac darce
		ahdRank++;

		if(ahdRank == myRank){
			ahdRank++;
		}

		if(ahdRank >= countOfProcesses){
			ahdRank = (myRank == 0) ? 1 : 0;
		}

		while(!stck.isEmpty())
			stck.pop();

		for(int i = 0 ; i < size; i++)
			stck.push(data[i]);

//		std::cout << stck << std::endl;		
	
		playground->initSquares();	
		playground->insertHoles();
	
		for(unsigned int i = 0; i < stck.getSize() - 1; i += 2)
		{
			int position = stck[i + 1];			
			int x = position / playground->getWidth();
			int y = position % playground->getWidth();					

			playground->insertTile(x, y, stck[i]);
		}

		delete [] data;
		return true;
	}
	else
		return false;
}


void ParallelAlgorithm::checkToken(void) 
{
	std::cout << "TOKEN" << std::endl;
	//spočítáme si, od koho by mohl pešek přijít
	int process = (myRank == 0) ? countOfProcesses - 1 : myRank - 1;

	//přišel nám pešek
	MPI_Recv(&tokenColor, 1, MPI_INT, process, MSG_TOKEN, MPI_COMM_WORLD, &status);

	printf("Process %d received TOKEN with color %s from #%d.\n", myRank, tokenColor == WHITE ? "WHITE" : "BLACK", process);

	token = 1;

	//poskuz prisel zpet bily pesek, konec vypoctu
	if(myRank == 0 && tokenColor == WHITE)
		transmitWorkEnd();

	if(processColor == BLACK)
		tokenColor = BLACK;	
}


int ParallelAlgorithm::checkWorkEnd()
{
	int flag;

	MPI_Iprobe(MPI_ANY_SOURCE, MSG_END_WORK, MPI_COMM_WORLD, &flag, &status);
		
	if(flag)
	{
		int val;
		MPI_Recv(&val, 1, MPI_INT, MPI_ANY_SOURCE, MSG_END_WORK, MPI_COMM_WORLD, &status);
		std::cout << "Process " << myRank << " recieved MSG_END_WORK from " << status.MPI_SOURCE << std::endl;
		return 1;
	}

	return 0;
}


void ParallelAlgorithm::checkSolution(void) 
{
	int* data;
	int price;
	int size = 0;
	int solver = 0;
	int flag = 0;

//	while(true)
//	{
		MPI_Iprobe(MPI_ANY_SOURCE, MSG_SOLUTION_TRANSMIT, MPI_COMM_WORLD, &flag, &status);
		
		if(flag)
		{
			MPI_Get_count(&status, MPI_INT, &size);

			if(size > 1)
			{
				data = new int[size];
				MPI_Recv(data, size, MPI_INT, MPI_ANY_SOURCE, MSG_SOLUTION_TRANSMIT, MPI_COMM_WORLD, &status);			
		
				price = data[0];
				solver = data[1];

				Stack<int> stack;
				for(int i = 2; i < size; i++)
					stack.push(data[i]);		

//				std::cout << stack << std::endl;
//				std::cout << "Process #" << myRank << " recieves solution from process #" << status.MPI_SOURCE << ", recieved price " << price << std::endl;

				if(price > playground->getPrice())
				{
					playground->saveSolution(stack);
					playground->setPrice(price);
					playground->setSolver(solver);
				}

				delete [] data;
			}
//			break;
		}
//	}
}


void ParallelAlgorithm::sendSolution()
{
	int size = stck.getSize() + 2;
	int* data = new int[size];
		
	data[0] = playground->getPrice();
	data[1] = myRank;

	for(int i = 2; i < size; i++)
		data[i] = stck[i - 2];

	for(int i = 0; i < countOfProcesses; i++)
	{
		MPI_Send(data, size, MPI_INT, i, MSG_SOLUTION_TRANSMIT, MPI_COMM_WORLD);
//		std::cout << "Process #" << myRank << " send solution with price " << playground->getPrice() << " to process #" << i << "." << std::endl;
	}
	delete [] data;
}



void ParallelAlgorithm::transmitWorkEnd(void) 
{
	printf("Process #%d is transmitting MSG_WORK_END!\n", myRank);

	for(int i = 0; i < countOfProcesses; i++)
		MPI_Send(&i, 1, MPI_INT, i, MSG_END_WORK, MPI_COMM_WORLD);
}



int ParallelAlgorithm::run()
{
    	int position = 0;
    	long price = 0;
    	int x = 0, y = 0;
    	int tileType = 0;
    	int lastInsertedTile = 0;
    	bool backtrack = false;
    	int tmpValue = 0;
	int plHeight  = playground->getHeight();
	int plWidth   = playground->getWidth();
	int counter   = 0;
	
    	//zasobnik bude prazdny pouze v pripade, ze nevlozim ani dlazdici
	//ani volnou diru kontrola maximalni hloubky stromu, kdy se muzu vratit
    	while(tileType < COUNT_OF_TILES || !stck.isEmpty()) 
	{ 		
        	if(!stck.isEmpty()) 
		{ 	//zasobnik: pozice, typDlazdice
            	x = stck.getTop() / playground->getWidth(); //radek z pozice na zasobniku
            	y = stck.getTop() % playground->getWidth(); //sloupec z pozice na zasobniku

			#ifdef DEBUG
	            	std::cout << "Top Of Stack: " << stck.getTop() << " [" << x << "," << y << "]. ";
			#endif

            	position = stck.pop(); 			// vyjimam o jednu dlazdici -> navrat zpet o jedno pole
            	lastInsertedTile = stck.pop(); 	//typ dlazdice vkladany naposledy
			
			#ifdef DEBUG
            		std::cout << "Last Inserted Tile : " << lastInsertedTile << std::endl;
			#endif

            	tileType = lastInsertedTile + 1;
            	playground->eraseTile(x, y, lastInsertedTile);

			#ifdef DEBUD
            		std::cout << "ERASED on position: " << position << ", tileType: " << lastInsertedTile << std::endl;
			#endif

            	backtrack = false;
			//std::cout << "BACKTRACK FALSE HIGH" << std::endl;
        	} 
		else if (backtrack) 
		{ 	//zasobnik je prazdny a dalsi dlazdice se nevejde kvuli vysce
            	break;
        	}

        	while (!backtrack && tileType < COUNT_OF_TILES) 
		{			
            	for (; x < plHeight; ++x) 
			{
                		for (; y < plWidth; ++y) 
				{
					if(startFlag && counter >= countOfProcesses)
					{
						start();
						startFlag = false;			
						counter = 0;
					}
					else if(!startFlag && counter >= CHECK_MSG_AMOUNT)
					{
						int result = communicationLoop();
						if(result == 2)
							return 2;

						counter = 0;
					}				
					
                    		position = x * plWidth + y; // radek * pocet sloupcu + pocet sloupec ==  pozice

                    		if ((tmpValue = playground->insertTile(x, y, tileType)) == 0) 
					{ 	//podarilo se vlozit dlazdici
						#ifdef DEBUG
                        			std::cout << "INSERTED on position: " << position << ", tileType: " << tileType << std::endl;
						#endif

                        		stck.push(tileType);
                        		stck.push(position);

                        		backtrack = false;
						//std::cout << "BACKTRACK FALSE" << std::endl;
						tileType = 0; 	//kdyz vlozim dlazdici, tak jde do hloubky 
									//a snazim se vkladat opet vsechny dalsi dlazdice
                    		}
					else if (tmpValue == -4) 
					{ 	//dlazdice se nevejde na vysku
                        		backtrack = true;
						//std::cout << "BACKTRACK TRUE" << std::endl;
                        		tmpValue = -5;
                        		break; 
                    		}
					else if (playground->getValueOnSquare(x, y) == -1) 
					{ 	//na pozici neni dira ani jina dlazdice
                        		tmpValue = -5;
                        		break;
                    		}
			
					counter++;  	

                		}

                		if (tmpValue == -5) 
                    		break;
                 		else 
                    		y = 0; // pokracovani na dalsim radku od zacatku
            	}

            	++tileType;

            	if (!stck.isEmpty()) 
			{
                		x = stck.getTop() / plHeight; 
                		y = (stck.getTop() % plHeight) + 1; //dalsi pozice

                		position = stck.getTop() + 1; 

				// osetreni na novy radek
               		if (y == plWidth) 
				{ 
                    		y = 0;
                    		++x;
                		}
            	}
			else 
			{
                		x = 0;
                		y = 0;
            	}
        	}
        
        	if ((price = playground->computePrice(stck)) > playground->getPrice()) 
		{
			std::cout << "ProcessID: " << myRank << " price: " << price << std::endl;

			playground->setPrice(price);
			playground->saveSolution(stck);

			if(myRank != 0)
				sendSolution();

			playground->printSolution();	
            }
	}
	return 0;
}

