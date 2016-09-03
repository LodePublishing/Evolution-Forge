#include "bitmap.hpp"

UI_Bitmap::UI_Bitmap(UI_Object* bitmap_parent, 
				const Rect& bitmap_rect,
				const Size distance_bottom_right, 
				const eBitmap my_bitmap, 
				const ePositionMode bitmap_position_mode) :
	UI_Object(bitmap_parent, Rect(bitmap_rect.getTopLeft(), Size(UI_Object::theme.lookUpBitmap(my_bitmap)->w, UI_Object::theme.lookUpBitmap(my_bitmap)->h)),  distance_bottom_right, bitmap_position_mode, NO_AUTO_SIZE),
	bitmap(my_bitmap)
{
	isClipped = true;
}



UI_Bitmap::~UI_Bitmap()
{}

void UI_Bitmap::reloadOriginalSize()
{
	setOriginalSize(getBitmapSize());
	UI_Object::reloadOriginalSize();
	adjustSize(CHILD_WAS_CHANGED);
}

// Render button.  How it draws exactly depends on it's current state.
void UI_Bitmap::draw(DC* dc) const
{
	if(!isShown())
		return;
	if(checkForNeedRedraw())
	{
		if(clipRect!=Rect())
			dc->DrawBitmap(theme.lookUpBitmap(bitmap), getAbsolutePosition(), clipRect);
		else
			dc->DrawBitmap(theme.lookUpBitmap(bitmap), getAbsolutePosition());
	}
	UI_Object::draw(dc);
}



UI_Object* UI_Bitmap::checkToolTip() {
	if( (!isShown()) || (!getAbsoluteRect().Inside(mouse)))
		return(0);
	return((UI_Object*)this);
}

UI_Object* UI_Bitmap::checkHighlight()
{
	if(!isShown())
		return(NULL);
	return(UI_Object::checkHighlight());
//	return((UI_Object*)this); lol nein!
	
}

void UI_Bitmap::process()
{
	UI_Object::process();
}

