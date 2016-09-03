#ifndef _UI_LONGTEXT_HPP
#define _UI_LONGTEXT_HPP

#include "button.hpp"
#include "bitmap.hpp"

class UI_LongText : public UI_Object
{
	public:
		UI_LongText& operator=(const UI_LongText& object);
		UI_LongText(const UI_LongText& object);
		UI_LongText(UI_Object* lt_parent, const Rect lt_pos, const Size distance_bottom_right, const std::string& lt_text, const eColor lt_color1, const eColor lt_color2, const eFont lt_font, const eButtonColorsType button, UI_Object* scroll_bar=NULL, const ePositionMode lt_mode = DO_NOT_ADJUST, const eAutoSize auto_size=NO_AUTO_SIZE);

		~UI_LongText();
		void updateText(const std::string& lt_text);

		void reloadOriginalSize();
		void reloadStrings();
		
		void draw(DC* dc) const;
		void process();

		const signed int getPressed() const;
		const unsigned int getTextHeight() const;
		const unsigned int getTextWidth() const;
		
//	protected:
//		eTextMode  mode; 
	private:
		UI_Object* scrollBar;
		std::list<UI_StaticText*> longText; // parsed text
		std::list<UI_Button*> longButton; // parsed buttons
		std::list<signed int> longNumber; // numbers of parsed buttons
		std::list<UI_Bitmap*> longBitmap; // parsed Bitmaps
		std::string text;
		eColor color1;
		eColor color2;
		eFont font;
		eButtonColorsType button;
		bool textWasChanged;
};

#endif // _UI_LONGTEXT_HPP

