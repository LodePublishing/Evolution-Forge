#ifndef _UI_BITMAP_HPP
#define _UI_BITMAP_HPP

#include "object.hpp"

class UI_Bitmap : public UI_Object
{
	public:
// TODO Beschreibung der Konstruktoren
		UI_Bitmap(UI_Object* bitmap_parent, 
				const Rect& bitmap_rect, 
				const Size distance_bottom_right, 
				const eBitmap my_bitmap, 
				const ePositionMode bitmap_position_mode = DO_NOT_ADJUST);
		~UI_Bitmap();
		
		void reloadOriginalSize();
		void process(); // process messages, continue animation etc.
		UI_Object* checkToolTip();
		UI_Object* checkHighlight();
		void draw(DC* dc) const;
		const unsigned int getBitmapWidth() const;
		const unsigned int getBitmapHeight() const;
		const Size getBitmapSize() const;
	private:
		eBitmap bitmap;
};

inline const unsigned int UI_Bitmap::getBitmapWidth() const {
	return(UI_Object::theme.lookUpBitmap(bitmap)->w);
}

inline const unsigned int UI_Bitmap::getBitmapHeight() const {
	return(UI_Object::theme.lookUpBitmap(bitmap)->h);
}

inline const Size UI_Bitmap::getBitmapSize() const {
	return(Size(UI_Object::theme.lookUpBitmap(bitmap)->w, UI_Object::theme.lookUpBitmap(bitmap)->h));
}

#endif // _UI_BITMAP_HPP

