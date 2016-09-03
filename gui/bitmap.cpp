#include "bitmap.hpp"

UI_Bitmap::UI_Bitmap(UI_Object* bitmap_parent, 
				const Rect& bitmap_rect,
				const Size distance_bottom_right, 
				const eBitmap my_bitmap, 
				const ePositionMode bitmap_position_mode) :
	UI_Object(bitmap_parent, Rect(bitmap_rect.getTopLeft(), Size(UI_Object::theme.lookUpBitmap(my_bitmap)->w, UI_Object::theme.lookUpBitmap(my_bitmap)->h)),  distance_bottom_right, bitmap_position_mode, NO_AUTO_SIZE),
	bitmap(my_bitmap),
	checked(false),
	brightness(100)
{
	isClipped = true;
	setDrawType(TRANSPARENT_OBJECT);
}

UI_Bitmap::~UI_Bitmap()
{}

void UI_Bitmap::reloadOriginalSize()
{
	setOriginalSize(getBitmapSize());
	UI_Object::reloadOriginalSize();
	adjustSize(CHILD_WAS_CHANGED);
}

void UI_Bitmap::setBrightness(const unsigned int bitmap_brightness) 
{
	if(brightness != bitmap_brightness)
	{
		brightness = bitmap_brightness;
		makePufferInvalid();
//		setNeedRedrawAllThatOverlaps(getRelativeRect());
//		toErrorLog(brightness);
	}
}

void UI_Bitmap::object_info()
{
	toErrorLog("ui_bitmap");
}
	

// Render button.  How it draws exactly depends on it's current state.
void UI_Bitmap::draw() const
{
	if(clipRect != Rect())
		dc->DrawBrightenedBitmap(theme.lookUpBitmap(checked?(eBitmap)(bitmap+1):bitmap), Point(), clipRect, brightness);
	else
		dc->DrawBrightenedBitmap(theme.lookUpBitmap(checked?(eBitmap)(bitmap+1):bitmap), Point(), brightness);
	UI_Object::draw();
}

void UI_Bitmap::setBitmap(const eBitmap new_bitmap)
{
#ifdef _SCC_DEBUG
	if((new_bitmap<0)||(new_bitmap>=MAX_BITMAPS)) {
		toErrorLog("WARNING (UI_Bitmap::setBitmap()): Value new_bitmap out of range.");return;
	}
#endif
	bitmap = new_bitmap;
	reloadOriginalSize();
	//redraw?
}

UI_Object* UI_Bitmap::checkToolTip() {
	if( (!isShown()) || (!getAbsoluteRect().isTopLeftCornerInside(mouse)))
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

