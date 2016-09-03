#ifndef _UI_STATICTEXT_HPP
#define _UI_STATICTEXT_HPP

#include "object.hpp"

enum eTextMode
{
	NO_TEXT_MODE = 0,
	RIGHT_BOUNDED_TEXT_MODE,
	VERTICALLY_CENTERED_TEXT_MODE,
	HORIZONTALLY_CENTERED_TEXT_MODE,
	TOTAL_CENTERED_TEXT_MODE,
	LOWER_CENTERED_TEXT_MODE,
	UPPER_CENTERED_TEXT_MODE,
	FORMATTED_TEXT_MODE
};

class UI_StaticText:public UI_Object
{
    public:
		UI_StaticText& operator=(const UI_StaticText& object);
		UI_StaticText(const UI_StaticText& object);
		
        UI_StaticText(UI_Object* st_parent, const Rect st_pos, const eTextMode st_mode = NO_TEXT_MODE, const eColor st_color = NULL_COLOR, const eFont st_font = NULL_FONT);

		UI_StaticText(UI_Object* st_parent, const eString st_text, const Rect st_pos, const eTextMode st_mode = NO_TEXT_MODE, const eColor st_color = NULL_COLOR, const eFont st_font = NULL_FONT);
		
		UI_StaticText(UI_Object* st_parent, const string st_text, const Rect st_pos, const eTextMode st_mode = NO_TEXT_MODE, const eColor st_color = NULL_COLOR, const eFont st_font = NULL_FONT);

		~UI_StaticText();
		
		void setMode(const eTextMode st_mode);
		void setColor(const Color st_color);
		void setColor(const eColor st_color);
		void setFont(const eFont st_font);
		void updateText(const string& st_text);
		UI_Object* checkHighlight();
		void updateText(const eString st_text);
		
        void draw(DC* dc) const;
        void process();

		const Size getTextSize() const;
	protected:
        eTextMode  mode; 
    private:
		string text;
        eFont font;
        Color color;
};

#endif // _UI_STATICTEXT_HPP

