#include "size.hpp"

void Size::move(const Size& startSize, const Size& targetSize)
{
	mv(width, startSize.width, targetSize.width);
	mv(height, startSize.height, targetSize.height);
}	

void Size::mv(unsigned int& x, const unsigned int sx, const unsigned int tx)
{
// TODO
	if(x == tx)
		return;
	
	signed int z = (signed int)(((x>sx)?(2*(((signed int)(x))-((signed int)(sx)))<(((signed int)(tx))-((signed int)(sx)))?(double)(((signed int)(x))-((signed int)(sx)))/2:(double)(((signed int)(tx))-((signed int)(x)))/2):(2*(((signed int)(x))-((signed int)(sx)))>(((signed int)(tx))-((signed int)(sx)))?(double)(((signed int)(x))-((signed int)(sx)))/2:(double)(((signed int)(tx))-((signed int)(x)))/2)));// TODO?
	if(z == 0)
		x+=((x>tx)?-1:0)+((x<tx)?1:0);
	else
		x= unsigned (((signed int)(x)) + z);
}	

