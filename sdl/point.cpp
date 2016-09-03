#include "point.hpp"
#include "../core/defs.hpp"

void Point::mv(signed int& x, const signed int sx, const signed int tx)
{
	if(x == tx)
		return;
	signed int z = (signed int)(((x>sx)?(2*(x-sx)<(tx-sx)?(double)(x-sx)/2:(double)(tx-x)/2):(2*(x-sx)>(tx-sx)?(double)(x-sx)/2:(double)(tx-x)/2)));
	if(z==0)
		z=((x>tx)?-1:0)+((x<tx)?1:0);
	x+=z;
}	

void Point::mv2(signed int& x, const signed int sx, const signed int tx)
{
	if(x == tx)
		return;
	signed z = 0;
	if(x > sx)
	{
		if((x - sx) < (tx - sx))
			z = (x - sx) / 2;
		else
			z = (tx - x) / 2;
	}
	else
	{
		if((x - sx) > (tx - sx))
			z = (x - sx) / 2;
		else
			z = (tx - x) / 2;
	}
//	signed int z = (signed int)(((x>sx)?(2*(x-sx)<(tx-sx)?(double)(x-sx)/2:(double)(tx-x)/2):(2*(x-sx)>(tx-sx)?(double)(x-sx)/2:(double)(tx-x)/2)));
	if(z==0)
		z=((x>tx)?-1:0)+((x<tx)?1:0);
	x+=z;
}	

