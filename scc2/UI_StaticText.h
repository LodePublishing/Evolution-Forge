#ifndef __UI_STATICTEXT__H_
#define __UI_STATICTEXT__H_

#include "UI_Object.h"

enum eTextMode
{
	NO_TEXT_MODE,
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
        UI_StaticText(UI_Object* parent, Rect pos, eTextMode mode = NO_TEXT_MODE, eFont font=NULL_FONT, eColor color=NULL_COLOUR);

		UI_StaticText(UI_Object* parent, eString text, Rect pos, eTextMode mode = NO_TEXT_MODE, eFont font=NULL_FONT, eColor color=NULL_COLOUR);
		
		UI_StaticText(UI_Object* parent, string text, Rect pos, eTextMode mode = NO_TEXT_MODE, eFont font=NULL_FONT, eColor color=NULL_COLOUR);
		~UI_StaticText();
		
		void setMode(eTextMode mode);
		void setColor(Color color);
		void setColor(eColor color);
		void setFont(eFont font);
		void updateText(string text);
		void updateText(eString text);
		
        void draw(DC* dc);
        void process();

		Size getSize();
	protected:
        eTextMode  mode; // left-bound=0, right-bound=1, vertically centered=2, horizontally centered=4, block text = 8, FORMATTED = 16
    private:
		string text;
        eFont font;
        Color color;
};

#endif

