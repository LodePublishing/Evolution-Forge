#include "rect.hpp"

const bool Rect::moveSmooth(const Rect& startRect, const Rect& targetRect)
{
	if((startRect == targetRect)||(*this == targetRect))
		return(false);
	Rect oldRect = *this;
	topLeftCorner.move(startRect.GetTopLeft(), targetRect.GetTopLeft());
	rectSize.move(startRect.GetSize(), targetRect.GetSize());
	bottomRightCorner = topLeftCorner + rectSize;
	if(oldRect!=*this)
		return(true);
	else return(false);
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
	return(Inside(rect.GetTopLeft()) || Inside(rect.GetBottomLeft()) || Inside(rect.GetBottomRight()) || Inside(rect.GetTopRight()));	
}
