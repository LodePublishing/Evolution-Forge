#include "rect.hpp"

const enum eRectMovement Rect::moveSmooth(const Rect& start_rect, const Rect& target_rect)
{
	if(*this == target_rect)
		return(NO_CHANGE); // no action
	if(start_rect == target_rect)
		return(move(target_rect));
	
	bool got_smaller = isInside(target_rect);
	bool was_moved = (target_rect.getTopLeft() != getTopLeft());
			
	topLeftCorner.move(start_rect.getTopLeft(), target_rect.getTopLeft());
	rectSize.move(start_rect.getSize(), target_rect.getSize());
	bottomRightCorner = topLeftCorner + rectSize;

	if((got_smaller) || (was_moved))
		return(GOT_SMALLER_OR_MOVED); // smaller  or moved => moved
	else
		return(GOT_BIGGER); // => bigger => not moved
}

const enum eRectMovement Rect::move(const Rect& target_rect)
{
	if(*this == target_rect)
		return(NO_CHANGE);

	bool got_smaller = isInside(target_rect);
	bool was_moved = (target_rect.getTopLeft() != getTopLeft());
	
	*this = target_rect;
	
	if((got_smaller) || (was_moved))
		return(GOT_SMALLER_OR_MOVED); // smaller or moved
	else 
		return(GOT_BIGGER); // bigger
}


