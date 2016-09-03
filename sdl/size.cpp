#include "size.hpp"

void Size::move(const Size& startSize, const Size& targetSize)
{
	mv(width, startSize.width, targetSize.width);
	mv(height, startSize.height, targetSize.height);
}	

const bool Size::mv(unsigned int& x, const unsigned int sx, const unsigned int tx)
{
// TODO
	if(x == tx)
		return(true);
	
	signed int z = (signed int)(((x>sx)?(2*(((signed int)(x))-((signed int)(sx)))<(((signed int)(tx))-((signed int)(sx)))?(double)(((signed int)(x))-((signed int)(sx)))/2:(double)(((signed int)(tx))-((signed int)(x)))/2):(2*(((signed int)(x))-((signed int)(sx)))>(((signed int)(tx))-((signed int)(sx)))?(double)(((signed int)(x))-((signed int)(sx)))/2:(double)(((signed int)(tx))-((signed int)(x)))/2)));// TODO?
	if(z == 0)
		x+=((x>tx)?-1:0)+((x<tx)?1:0);
	else
		x= unsigned (((signed int)(x)) + z);
	return(false);
}	

const bool Size::mv2(unsigned int& x, const unsigned int sx, const unsigned int tx)
{
	if(x == tx)
		return(false);
	if(x > tx)
	{
		--x;
		x -= (x - tx)/2;
	}
	else if(x < tx)
	{
		++x;
		x += (tx - x)/2;
	}
	return(true);
//	signed int z = (signed int)(((x>sx)?(2*(x-sx)<(tx-sx)?(double)(x-sx)/2:(double)(tx-x)/2):(2*(x-sx)>(tx-sx)?(double)(x-sx)/2:(double)(tx-x)/2)));
	
/*	signed int mx = (tx + sx)/2;
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
	return(true);*/
}
