/* 
 * File:   Playground.h
 * Author: lubos
 *
 * Created on October 18, 2011, 2:43 AM
 */

#ifndef PLAYGROUND_H
#define	PLAYGROUND_H

#include "Stack.h"
#include "Tile.h"



#define COUNT_OF_TILES 21
#define MIN_PRICE -100000


class Playground 
{

private:	
//	Playground(const Playground& plag);
//	Playground& operator=(const Playground& plg);

protected:
	int 		m_height;
	int 		m_width; 
	int		m_size;
	int** 	m_pSquares;

	int		m_countOfHoles;	

	int*		m_pResult;
	int 		m_resultSize;

    	int 	 	m_price;
	Tile** 	m_tiles;
	int 		m_solverRank;
	int* 		m_pHoles;

public:
    	Playground(int n, int m, Tile** tiles, int countOfHoles, int* holes);
    	virtual ~Playground();	
	
	void 	initSquares();
    	void 	printSolution();

    	int 	insertTile(int x, int y, int position);
    	int 	eraseTile(int x, int y, int position);
    	int 	computePrice(Stack<int> &stck) ;
    	void 	saveSolution(Stack<int> &stck);
  
    	int 	getThreshold() const;
	int   getMinThreshold() const;
    	int 	insertHoles();
	
	Playground(const Playground& plag);
	Playground& operator=(const Playground& plg);
	friend std::ostream & operator<<(std::ostream& x, const Playground &a);

	// gettry settry
	int getHeight() const 				{ return m_height; }
	int getWidth() const				{ return m_width; } 
	int geSize() const				{ return m_size; }
	int getValueOnSquare(int x, int y) const  { return m_pSquares[x][y]; }
	int getPrice() const 				{ return m_price; }
	void setPrice(int pr) 				{ this->m_price = pr; }
	void setSolver(int solver)			{ this->m_solverRank = solver; }
	int getSolverRank() const			{ return m_solverRank; }
};


#endif	/* PLAYGROUND_H */

