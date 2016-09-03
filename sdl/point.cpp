#include "point.hpp"

void Point::mv(signed int& x, const signed int sx, const signed int tx)
{
	if(x > tx)
		--x;
	else if(x < tx)
		++x;
	if(x == tx)
		return;
//	signed int z = (signed int)(((x>sx)?(2*(x-sx)<(tx-sx)?(double)(x-sx)/2:(double)(tx-x)/2):(2*(x-sx)>(tx-sx)?(double)(x-sx)/2:(double)(tx-x)/2)));
	
	signed int mx = (tx + sx)/2;
	signed int z = 0;
	if((tx >= mx)&&(mx >= sx)&&(sx >=  x)) z = (tx-x)/2; else
	if((tx >= mx)&&(mx >=  x)&&(x >= sx)) z = (x-sx)/2; else
	if((tx >=  x)&&(x >= mx)&&(mx >= sx)) z = (tx-x)/2; else
	if(( x >= tx)&&(tx >= mx)&&(mx >= sx)) z = (tx-x)/2; else

	if((tx <= mx)&&(mx <= sx)&&(sx <=  x)) z = (tx-x)/2; else
	if((tx <= mx)&&(mx <=  x)&&(x <= sx)) z = (x-sx)/2; else
	if((tx <=  x)&&(x <= mx)&&(mx <= sx)) z = (tx-x)/2; else
	if(( x <= tx)&&(tx <= mx)&&(mx <= sx)) z = (tx-x)/2;

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

