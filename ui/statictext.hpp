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
	UPPER_CENTERED_TEXT_MODE
};

class UI_StaticText:public UI_Object
{
    public:
        UI_StaticText(UI_Object* parent, const Rect pos, const eTextMode mode = NO_TEXT_MODE, const eColor color=NULL_COLOUR, const eFont font=NULL_FONT);

		UI_StaticText(UI_Object* parent, const eString text, const Rect pos, const eTextMode mode = NO_TEXT_MODE, const eColor color=NULL_COLOUR, const eFont font=NULL_FONT);
		
		UI_StaticText(UI_Object* parent, const string text, const Rect pos, const eTextMode mode = NO_TEXT_MODE, const eColor color=NULL_COLOUR, const eFont font=NULL_FONT);
		~UI_StaticText();
		
		void setMode(const eTextMode mode);
		void setColor(const Color color);
		void setColor(const eColor color);
		void setFont(const eFont font);
		void updateText(const string text);
		void updateText(const eString text);
		
        void draw(DC* dc) const;
        void process();

		const Size getTextSize() const;
	protected:
        eTextMode  mode; // left-bound=0, right-bound=1, vertically centered=2, horizontally centered=4, block text = 8, FORMATTED = 16
    private:
		string text;
        eFont font;
        Color color;
};

#endif // _UI_STATICTEXT_HPP

