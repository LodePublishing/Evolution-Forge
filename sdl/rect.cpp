#include "rect.hpp"
#include "../core/configuration.hpp"

const bool Rect::move(const Rect startRect, const Rect targetRect)
{
	if((startRect == targetRect)||(*this == targetRect))
		return(false);
	Rect oldRect = *this;
	if(!configuration.isSmoothMovements())
		*this = targetRect;
	else
	{
		topLeftCorner.move(startRect.GetTopLeft(), targetRect.GetTopLeft());
		rectSize.move(startRect.GetSize(), targetRect.GetSize());
		bottomRightCorner = topLeftCorner + rectSize;
	}
	
	return(oldRect!=*this);
}

const bool Rect::overlaps(const Rect& rect) const
{
	return(Inside(rect.GetTopLeft()) || Inside(rect.GetBottomLeft()) || Inside(rect.GetBottomRight()) || Inside(rect.GetTopRight()));	
}
