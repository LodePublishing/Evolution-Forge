#include "rect.hpp"

const enum eRectMovement Rect::moveSmooth(const Rect& startRect, const Rect& targetRect)
{
	if((startRect == targetRect)||(*this == targetRect))
		return(NO_CHANGE); // no action
	Rect oldRect = *this;
	topLeftCorner.move(startRect.getTopLeft(), targetRect.getTopLeft());
	rectSize.move(startRect.getSize(), targetRect.getSize());
	bottomRightCorner = topLeftCorner + rectSize;

	if(oldRect.Inside(*this))
		return(GOT_BIGGER); // => bigger => not moved
	else
		return(GOT_SMALLER_OR_MOVED); // smaller  or moved => moved
}

const enum eRectMovement Rect::move(const Rect& startRect, const Rect& targetRect)
{
	if((startRect == targetRect)||(*this == targetRect))
		return(NO_CHANGE);

	bool got_bigger = Inside(targetRect);
	*this = targetRect;
	if(got_bigger)
		return(GOT_BIGGER); // bigger
	else return(GOT_SMALLER_OR_MOVED); // smaller or moved
}


