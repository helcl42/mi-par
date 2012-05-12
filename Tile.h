/* 
 * File:   Tile.h
 * Author: lubos
 *
 * Created on October 18, 2011, 2:39 AM
 */

#ifndef TILE_H
#define	TILE_H
#include <iostream>

class Tile
{
private:
	void initTile(int tileArr[][4]);

	Tile(const Tile& tl);
	Tile& operator=(const Tile& tl);

protected:
    	bool 	m_fromRight;
    	int 	m_width;
    	int 	m_height;
    	int**	m_pTileArr;
    	int 	m_price;

public:
    	Tile(int tile[][4], int width, int height, int price, bool _priznakZprava = false);
    	virtual ~Tile();

	friend std::ostream& operator<< (std::ostream& out, const Tile& tl);

	//gettry settry
	int  getHeight() const 	 		 	   { return m_height; }
	int  getWidth() const	 			   { return m_width; }
	int  getPrice() const	 			   { return m_price; }
	bool isFromRight() const 	   		   { return m_fromRight; }
	int  getTileValueOnIndex(int x, int y) const { return m_pTileArr[x][y]; }
};


#endif	/* TILE_H */

