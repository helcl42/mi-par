#include "Playground.h"


Playground::Playground(int m, int n, Tile** tiles, int countOfHoles, int* holes) 
:    	m_height(m),
    	m_width(n),	
	m_countOfHoles(countOfHoles)
{
	m_tiles = tiles;
	m_pResult = NULL;
	m_pHoles = holes;
	m_size = m * n;
	m_tiles = tiles;
	m_price = getMinThreshold();
	initSquares();

	if(m_countOfHoles > 0)
	  	insertHoles();
}


Playground::~Playground()
{
	for(int i = 0; i < m_height; i++)
		delete [] m_pSquares[i];

	delete [] m_pSquares;	
	delete [] m_pResult;	
}


void Playground::initSquares()
{
	m_pSquares = new int*[m_height];

    	for (int i = 0; i < m_height; ++i) 
	{
		m_pSquares[i] = new int[m_width];
        	for (int j = 0; j < m_width; ++j) 
            	m_pSquares[i][j] = -1; //-1 == volne misto
	}
}


int Playground::insertHoles() 
{
    	for (int i = 0; i < m_countOfHoles; ++i) 
	{
        	if (m_pHoles[i] > ((m_height - 1) * (m_width) + (m_width - 1)) || m_pHoles[i] < 0) 
	            return (-1);
		//mapovani do dvourozmerneho pole
		int x = m_pHoles[i] / m_width; 
		int y = m_pHoles[i] % m_width;

		m_pSquares[x][y] = -2; 	// -2 == nutna díra
    	}			 		// -1 == volne misto
    	return 0;				//  0 - 20 typ dlazdice
}


void Playground::printSolution()
{
    	std::cout << std::endl << "Solution by process " << m_solverRank << std::endl;

	initSquares();
	insertHoles();

    	for (int i = 0; i < m_resultSize; i += 2) 
	{
        	std::cout << "position " << m_pResult[i] << ": [" << (m_pResult[i] / (m_width));
        	std::cout << "," << (m_pResult[i] % (m_width)) << "] tileType: " << (m_pResult[i + 1]) << std::endl;

        	insertTile((m_pResult[i] / (m_width)), (m_pResult[i] % (m_width)), m_pResult[i + 1]);
    	}

    	std::cout << *this;
    	std::cout << "Price: " << m_price << std::endl << std::endl;
}


void Playground::saveSolution(Stack<int> & stck) 
{
    	int stackSize = 0;
/*
	//kotrola jestli se povedlo vyplnit cele pole
	for (int i = 0; i < m_height; ++i)
		for (int j = 0; j < m_width; ++j)
            	if (m_pSquares[i][j] == -1) 
                		return;
*/
	stackSize = stck.getSize();

	if (stackSize > 0 && stackSize % 2 != 0) 
		throw new Exception("Stack has not even size.");

    	if (m_pResult != NULL) 
        	delete [] m_pResult; 
    
	m_pResult = new int[stackSize];
	m_resultSize = stackSize;

	// vysledek se uzlozi v prevracenem poradi(ze zasobniku)
	//na sudych pozicich jsou typy dlazdic, na lichych je pozice
    	for (int i = 0; i < stackSize; ++i) 
        	m_pResult[i] = stck[stackSize - 1 - i]; 
}


// cena=3*pocet L3 + 2*pocet L2 + pocet  L1 - 6 * a(vznikla dira)
int Playground::computePrice(Stack<int> & stck)
{
	int price = 0;
    	int stackSize = 0;
	int tmpStck;

	//kontrola, jestli je vyplnena cela sachovnice
    	for (int i = 0; i < m_height; ++i)
        	for (int j = 0; j < m_width; ++j)
            	if (m_pSquares[i][j] == -1)
                		return MIN_PRICE;

	stackSize = stck.getSize();

	if (stackSize >= 0 && stackSize % 2 != 0)
            throw new Exception("Chyba. Zasobnik nema sudou velikost nebo je prazdny.");

	//pro vypocitani ceny je potreba secist hodnoty jednotlivych dlazdic podle typu,
	//ktere jsou v zasobniku na sudych patrech
    	for(int i = 0; i < stackSize; i+=2)
	{
		tmpStck = stck[i];
		if (tmpStck <= COUNT_OF_TILES || tmpStck > 0)
			price += m_tiles[tmpStck]->getPrice();
		else
			throw new Exception("Chyba. V poli je zapsana hodnota dlazdice, ktera neni znama.");
    	}
    	return price;
}



int Playground::getThreshold() const
{
	return 3 * (m_size - m_countOfHoles) / 5;
}


int Playground::getMinThreshold() const
{
	if(m_countOfHoles >= m_size / 2)
		return (m_size - m_countOfHoles) * (-6);
	else
		return MIN_PRICE;
}


std::ostream & operator<<(std::ostream& out, const Playground & pl) 
{
    	out << std::endl;
    	for (int i = 0; i < pl.m_height; ++i) 
	{
		out << "| ";

		for (int j = 0; j < pl.m_width; ++j) 
		{
			if(pl.m_pSquares[i][j] >= 0 && pl.m_pSquares[i][j] < 10)
				out << " "; 
			out << pl.m_pSquares[i][j];
			if (j != pl.m_width - 1) 
				out << ", ";	
		}
		out << " |" << std::endl;
    	}
    	out << std::endl;
    	return out;
}



int Playground::insertTile(int x, int y,  int tileType) 
{
    	int tempY;
	int tileWidth = m_tiles[tileType]->getWidth();
	int tileHeight = m_tiles[tileType]->getHeight();

	#ifdef DEBUG 
		std::cout << "Pokus o vlozeni dlazdice na pozici: [ " << x << ", " << y << " ] typ:" <<  tileType << std::endl;
	#endif

	if ((x >= m_height) || (y >= m_width)) 
		throw new Exception("Pozice jsou mimo pole, pri vkladani dlazdice.");

    	// musim zjistovat jestli nepresahnu rozmery plochy !
    	if (x + tileHeight > m_height) 
	{ 
		#ifdef DEBUG
            	std::cout << "Dlazdice se nevejde na vysku, proto nezkousi dalsi." << std::endl;
		#endif
        	return -4;
    	}

    	if (m_tiles[tileType]->isFromRight()) 
	{ 
		//zde vkladam dlazdice z praveho horniho rohu (pac v levem maji nulu)
        	//Y je pozice od ktery zacinam vkladat, v pripade, ze vkladam zprava je Y pozice posunuta
        	tempY = y - ((tileWidth) - 1);

        	//tohle asi nemusi byt, Y pozice je posunuta do leva
        	if (tempY + tileWidth > m_width) 
		{ //sloupec, sirka od jednicky
			#ifdef DEBUG
                		std::cout << "Dlazdice se nevejde doprava.\n" << std::endl; //muze se tam ale jeste vejit dlazdice vkladana zprava
			#endif
            	return -3; //dlazdice se nevejde do pole (jeste netestuji, zdali jsou tam diry nebo jina dlazdice
        	}

        	if (tempY < 0) 
		{
			#ifdef DEBUG
                		std::cout << "Dlazdice se nevejde doleva.\n" << std::endl; //muze se tam ale jeste vejit dlazdice vkladana zprava
			#endif
            	return -3;
        	}
	}
	else //vklada se zleva
	{
		tempY = y;

        	if (tempY + tileWidth > m_width) 
		{
			#ifdef DEBUG
                		std::cout << "Dlazdice se nevejde doprava." << std::endl; 
			#endif
            	return -2;
        	}
	}

    	// podivam se jestli je na dane pozici volno a jestli je mozne tam dlazdici vlozit
	for (int i = 0; i < tileHeight; ++i) 
        	for (int j = 0; j < tileWidth; ++j) 
            	if(m_tiles[tileType]->getTileValueOnIndex(i, j) == 1 && m_pSquares[x + i][tempY + j] != -1) 
			{	// pokud je na miste pro dlazdici neco jineho nez -1(volne, tak vypisu chybu
				#ifdef DEBUG
                    		std::cout << "Neni mozne vlozit dlazdici na pozici: [ " << (x + i) << ", " << (tempY + j) << " ]" << std::endl << std::endl;
				#endif
                		return -1;
            	}

    	// pole lze vlozit -> vlozim jej
    	for (int i = 0; i < tileHeight; ++i) 
        	for (int j = 0; j < tileWidth; ++j) 
            	if (m_tiles[tileType]->getTileValueOnIndex(i, j) == 1) 
                		m_pSquares[x + i][tempY + j] = tileType; //position obsahuje vlastne typ dlazdice
            
	#ifdef DEBUG 
        	std::cout << *this;
	#endif
    	return 0;
}


int Playground::eraseTile(int x, int y, int tileType) 
{
	int tempY;
	int tileHeight = m_tiles[tileType]->getHeight();
	int tileWidth = m_tiles[tileType]->getWidth();

	#ifdef DEBUG
		std::cout << "Pokus o smazani dlazdice: " << tileType << " na pozici: [" << x << "," << y << "]" << std::endl;
	#endif

    	if (m_tiles[tileType]->isFromRight()) 
		tempY = y - (tileWidth - 1);
    	else 
        	tempY = y;
    
    	// musim zjistovat jestli nepresahnu rozmery plochy
    	if ((x + tileHeight > m_height) || (tempY + tileWidth > m_width)) 
        	throw new Exception("Pokousim se spatne smazat dlazdici mimo pole.");

    	// podivam se jestli je na dane pozici volno a jestli je mozne dlazdici smazat
	for(int i = 0; i < tileHeight; ++i)
		for (int j = 0; j < tileWidth; ++j)
            	if ((m_tiles[tileType]->getTileValueOnIndex(i, j) == 1) && (m_pSquares[x + i][tempY + j] != tileType)) 
			{
				std::cout << "ERROR: Snazime se smazat jinou dlazdici. TileType = " << tileType << " value on square = " <<  m_pSquares[x + i][tempY + j] << std::endl;
			//	throw new Exception("Snazime se smazat jinou dlazdici.");
			}

    	// vymazu nasi dlazdici
    	for (int i = 0; i < tileHeight; ++i) 
        	for (int j = 0; j < tileWidth; ++j) 
            	if ((m_tiles[tileType]->getTileValueOnIndex(i, j) == 1)) 
                		m_pSquares[x + i][tempY + j] = -1;
    
	#ifdef DEBUG
        	std::cout << *this;
	#endif
    	return 0;
}


Playground::Playground(const Playground& plg)
{
	for(int i = 0; i < m_height; i++)
		delete [] m_pSquares[i];

	delete [] m_pSquares;	
	delete [] m_pResult;	

	m_height = plg.m_height;
	m_width = plg.m_width;
	m_size = plg.m_size;
	m_countOfHoles = plg.m_countOfHoles;	
	m_pResult = NULL;
	m_resultSize = 0;
	m_price = MIN_PRICE;
	m_tiles = plg.m_tiles;
	
	m_pSquares = new int*[m_height];

   	for (int i = 0; i < m_height; ++i) 
	{
		m_pSquares[i] = new int[m_width];
       	for (int j = 0; j < m_width; ++j) 
			m_pSquares[i][j] = plg.m_pSquares[i][j]; //-1 == volne misto
	}		
}


Playground& Playground::operator=(const Playground& plg)
{
	if(this != &plg)
	{
		for(int i = 0; i < m_height; i++)
			delete [] m_pSquares[i];

		delete [] m_pSquares;	
		delete [] m_pResult;	

		m_height = plg.m_height;
		m_width = plg.m_width;
		m_size = plg.m_size;
		m_countOfHoles = plg.m_countOfHoles;	
		m_pResult = NULL;
		m_resultSize = 0;
		m_price = MIN_PRICE;
		m_tiles = plg.m_tiles;
	
		m_pSquares = new int*[m_height];

    		for (int i = 0; i < m_height; ++i) 
		{
			m_pSquares[i] = new int[m_width];
        		for (int j = 0; j < m_width; ++j) 
            		m_pSquares[i][j] = plg.m_pSquares[i][j]; //-1 == volne misto
		}		
	}
	return(*this);
}
