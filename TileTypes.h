/* 
 * File:   TileTypes.h
 * Author: lubos
 *
 * Created on October 18, 2011, 2:52 AM
 */

#ifndef TILETYPES_H
#define	TILETYPES_H


// height 1
int hole[4][4] =
{
	{ 1, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 }
};


// height 2
int L1a[4][4] =
{
	{ 1, 1, 0, 0 },
	{ 1, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 }
};


int L1b[4][4] =
{
	{ 0, 1, 0, 0 },
	{ 1, 1, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 }
};


int L1c[4][4] =
{
	{ 1, 1, 0, 0 },
	{ 0, 1, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 }
};


int L1d[4][4] =
{
	{ 1, 0, 0, 0 },
	{ 1, 1, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 }
};


//L2
int L2b[4][4] =
{
	{ 1, 1, 1, 0 },
    	{ 1, 0, 0, 0 },
    	{ 0, 0, 0, 0 },
    	{ 0, 0, 0, 0 }
};


int L2d[4][4] =
{
	{ 0, 0, 1, 0 },
	{ 1, 1, 1, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 }
};


int L2f[4][4] =
{
	{ 1, 1, 1, 0 },
	{ 0, 0, 1, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 }
};


int L2h[4][4] =
{
	{ 1, 0, 0, 0 },
	{ 1, 1, 1, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 }
};


int L3b[4][4] =
{
	{ 1, 1, 1, 1 },
	{ 1, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 }
};


int L3d[4][4] =
{
	{ 0, 0, 0, 1 },
	{ 1, 1, 1, 1 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 }
};


int L3f[4][4] =
{
	{ 1, 1, 1, 1 },
	{ 0, 0, 0, 1 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 }
};


int L3h[4][4] =
{
	{ 1, 0, 0, 0 },
	{ 1, 1, 1, 1 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 }
};



// height 3
int L2a[4][4] =
{
	{ 1, 0, 0, 0 },
	{ 1, 0, 0, 0 },
	{ 1, 1, 0, 0 },
	{ 0, 0, 0, 0 }
};


int L2c[4][4] =
{
	{ 1, 1, 0, 0 },
	{ 0, 1, 0, 0 },
	{ 0, 1, 0, 0 },
	{ 0, 0, 0, 0 }
};


int L2e[4][4] =
{
	{ 1, 1, 0, 0 },
	{ 1, 0, 0, 0 },
	{ 1, 0, 0, 0 },
	{ 0, 0, 0, 0 }
};


int L2g[4][4] =
{
	{ 0, 1, 0, 0 },
	{ 0, 1, 0, 0 },
	{ 1, 1, 0, 0 },
	{ 0, 0, 0, 0 }
};


//height 4
int L3a[4][4] =
{
	{ 1, 0, 0, 0 },
	{ 1, 0, 0, 0 },
	{ 1, 0, 0, 0 },
	{ 1, 1, 0, 0 }
};


int L3c[4][4] =
{
	{ 1, 1, 0, 0 },
	{ 0, 1, 0, 0 },
	{ 0, 1, 0, 0 },
	{ 0, 1, 0, 0 }
};


int L3e[4][4] =
{
	{ 1, 1, 0, 0 },
	{ 1, 0, 0, 0 },
	{ 1, 0, 0, 0 },
	{ 1, 0, 0, 0 }
};


int L3g[4][4] =
{
	{ 0, 1, 0, 0 },
	{ 0, 1, 0, 0 },
	{ 0, 1, 0, 0 },
	{ 1, 1, 0, 0 }
};


#endif	/* TILETYPES_H */
