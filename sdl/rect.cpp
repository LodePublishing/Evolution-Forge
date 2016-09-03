#include "rect.hpp"
#include "../core/configuration.hpp"

void Rect::move(const Rect startRect, const Rect targetRect)
{
	if(startRect == targetRect)
		return;
	if(!configuration.isSmoothMovements())
		*this = targetRect;
	else
	{
		topLeftCorner.move(startRect.GetTopLeft(), targetRect.GetTopLeft());
		rectSize.move(startRect.GetSize(), targetRect.GetSize());
		bottomRightCorner = topLeftCorner + rectSize;
	}
}

