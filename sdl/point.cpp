#include "point.hpp"
#include "../core/defs.hpp"

const Size Point::operator-(const Point& point) const
{
#ifdef _SCC_DEBUG
	if(((x > point.x)&&(y < point.y))||((x < point.x)&&(y > point.y)))
		toLog("POINT: wrong subtraction!");
#endif
	return Size(x > point.x?x - point.x:point.x - x, y > point.y?y - point.y:point.y - y);
	// TODO ERROR wenn nicht beide > bzw beide <
}

void Point::mv(signed int& x, const signed int sx, const signed int tx)
{
	signed int z = (signed int)(((x>sx)?(2*(x-sx)<(tx-sx)?(double)(x-sx)/2:(double)(tx-x)/2):(2*(x-sx)>(tx-sx)?(double)(x-sx)/2:(double)(tx-x)/2)));
	if(z==0)
		z=((x>tx)?-2:0)+((x<tx)?1:0);
	x+=z;
}	
	
