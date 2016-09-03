#include "size.hpp"

Size::Size(const Size& size) :
	width(size.width),
	height(size.height)
{ 
/*#ifdef _SCC_DEBUG
	if((width > 1280) || (height > 1024))
	{
		toLog("SIZE ERROR: width/height out of range");
	}
#endif*/
}

Size::Size(const unsigned int w, const unsigned int h) :
	width(w),
	height(h)
{
/*#ifdef _SCC_DEBUG
	if((width > 1280) || (height > 1024))
	{
		toLog("SIZE ERROR: width/height out of range");
	}
#endif*/
}

const Size Size::operator-(const Size& size) const 
{
#ifdef _SCC_DEBUG
	if((size.width > width) || (size.height > height))
	{
		toLog("RESIZE ERROR: Negative size is not allowed");
		return(Size(0,0));
	}
#endif
	return Size(width - size.width, height - size.height);
}

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

