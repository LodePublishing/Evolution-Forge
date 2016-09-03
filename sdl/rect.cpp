#include "rect.hpp"
#include "../core/configuration.hpp"

Rect::Rect(const signed int x, const signed int y, const unsigned int w, const unsigned int h) :
	topLeftCorner(x,y),
	bottomRightCorner(x+w, y+h),
	rectSize(w, h)
{
	if((w > 1280)||(h> 1024))
		toLog("ERROR, width/height too large");
}

void Rect::SetRight(const signed int right) 
{
#ifdef _SCC_DEBUG
	if(right < GetLeft()) {
		toLog("ERROR: SetRight was put left of GetLeft!");return;
	}				
#endif
	SetWidth(right - GetLeft());
	bottomRightCorner.x = right;
}

void Rect::move(const Rect startRect, const Rect targetRect)
{
	if(!configuration.isSmoothMovements())
		*this = targetRect;
	else
	{
		topLeftCorner.move(startRect.GetTopLeft(), targetRect.GetTopLeft());
		rectSize.move(startRect.GetSize(), targetRect.GetSize());
		bottomRightCorner = topLeftCorner + rectSize;
	}
}

