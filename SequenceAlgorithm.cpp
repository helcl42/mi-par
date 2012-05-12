#include "SequenceAlgorithm.h"

SequenceAlgorithm::SequenceAlgorithm(Playground* playground) 
:	playground(playground)
{}


SequenceAlgorithm::~SequenceAlgorithm()  
{
}


void SequenceAlgorithm::run() 
{
    	int position = 0;
    	long price = 0;
    	int x = 0, y = 0;
    	int tileType = 0;
    	int lastInsertedTile = 0;
    	bool backtrack = false;
    	int tmpValue = 0;
	int threshold = playground->getThreshold();
	int plHeight  = playground->getHeight();
	int plWidth   = playground->getWidth();

//	std::cout << "StackSize: " << stck.getSize() << std::endl;
//	std::cout << "MAX: " << threshold << std::endl;
//	std::cout << "MIN: " << playground->getMinThreshold() << std::endl;
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

            	position = stck.pop(); 		// vyjimam o jednu dlazdici -> navrat zpet o jedno pole
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
                		}

                		if (tmpValue == -5) 
                    		break;
                 		else 
                    		y = 0; // pokracovani na dalsim radku od zacatku
            	}
	
            	//pokus o polozeni nasledujici dlazdici na dalsi pozici od te posledne vlozene dlazdice (na zasobniku)
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
			playground->setPrice(price);
			playground->saveSolution(stck);

                	std::cout << *playground;
                	std::cout << "?? ";
			playground->printSolution();	
            }

            if (playground->getPrice() == threshold) 
		{
          		std::cout << "Maximal price found." << std::endl;
                	return;
		}
	}
}

