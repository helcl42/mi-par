#include "Tile.h"


Tile::Tile(int tile[][4], int width, int height, int price, bool right)
:	m_fromRight(right),
	m_width(width),
	m_height(height),
	m_price(price)
{
	initTile(tile);
}


Tile::~Tile()
{
	for(int i = 0; i < m_height; i++)
		delete [] m_pTileArr[i];

	delete [] m_pTileArr;
}

void Tile::initTile(int tile[][4])
{
	m_pTileArr = new int*[m_height];

    	for(int i = 0; i < m_height; i++)
	{
        	m_pTileArr[i] = new int[m_width];
        	for (int j = 0; j < m_width; j++)
            	this->m_pTileArr[i][j] = tile[i][j];
	}
}


std::ostream& operator<< (std::ostream& out, const Tile& tl)
{
	out << "Tuto dlazdici vkladam ";

	if (tl.m_fromRight)
		out << "od praveho horniho rohu" << std::endl;
      else
            out << "od leveho horniho rohu" << std::endl;

	for (int i = 0; i < tl.m_height; ++i)
	{
		out << "| ";
            for (int j = 0; j < tl.m_width; ++j)
	    {
                if(tl.m_pTileArr[i][j] >= 0 && tl.m_pTileArr[i][j] < 10)
			out << " "; 
                out << tl.m_pTileArr[i][j];
                if (j != tl.m_width - 1)
			out << ", ";
            }
            out << " |";

            if (i != tl.m_height - 1)
			out << ",";
		out << std::endl;
	}
      out << std::endl << std::endl;
	return out;
}


