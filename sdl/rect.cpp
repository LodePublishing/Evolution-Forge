#include "rect.hpp"

const bool Rect::moveSmooth(const Rect& startRect, const Rect& targetRect)
{
	if((startRect == targetRect)||(*this == targetRect))
		return(false);
	Rect oldRect = *this;
	topLeftCorner.move(startRect.getTopLeft(), targetRect.getTopLeft());
	rectSize.move(startRect.getSize(), targetRect.getSize());
	bottomRightCorner = topLeftCorner + rectSize;
	if(oldRect==*this)
		return(false);
	else return(true);
}

const bool Rect::move(const Rect& startRect, const Rect& targetRect)
{
	if((startRect == targetRect)||(*this == targetRect))
		return(false);
	*this = targetRect;
	return(true);
}

const bool Rect::overlaps(const Rect& rect) const
{
	return(Inside(rect.getTopLeft()) || Inside(rect.getBottomLeft()) || Inside(rect.getBottomRight()) || Inside(rect.getTopRight()));	
}

const bool Rect::Inside(const Rect& rect) const 
{
	if( ( rect.getTopLeft() >= topLeftCorner ) && ( rect.getBottomRight() <= bottomRightCorner ) )
		return(true);
	else return(false);
}

