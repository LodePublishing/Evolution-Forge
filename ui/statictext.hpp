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
	BOTTOM_TEXT_MODE,
	TOP_TEXT_MODE,	
	FORMATTED_TEXT_MODE,
	FORMATTED_NON_BLOCK_TEXT_MODE
};

class UI_StaticText:public UI_Object
{
	public:
		UI_StaticText& operator=(const UI_StaticText& object);
		UI_StaticText(const UI_StaticText& object);
		
		UI_StaticText(UI_Object* st_parent, const Rect st_pos, const eColor st_color, const eFont st_font, const eTextMode st_mode = NO_TEXT_MODE);

		UI_StaticText(UI_Object* st_parent, const eString st_text, const Rect st_pos, const eColor st_color, const eFont st_font, const eTextMode st_mode = NO_TEXT_MODE);
		
		UI_StaticText(UI_Object* st_parent, const string& st_text, const Rect st_pos, const eColor st_color, const eFont st_font, const eTextMode st_mode = NO_TEXT_MODE);

		~UI_StaticText();

		
		void setMode(const eTextMode st_mode);
		void setColor(const Color st_color);
		void setColor(const eColor st_color);
		void setFont(const eFont st_font);
		const std::string& getString() const;
		void updateText(const string& st_text);

		void addChar(const unsigned int position, const char key);
		void removeCharBackspace(const unsigned int position);
		void removeCharDelete(const unsigned int position);

// for formatted Text:
		const bool doneWriting() const; 
			
		UI_Object* checkTooltip();
		UI_Object* checkHighlight();
	
		void updateText(const eString st_text);
		void reloadStrings();
		void calculatePosition();
		
		void draw(DC* dc) const;
		void process();

		const Size getTextSize() const;
		const Rect& getTextBox() const;
		const Size getTextPosSize(const unsigned int pos) const;
		const Size& getBoxSize() const;

		void setPressed(const bool press=false);

	protected:
		eTextMode  mode; 
	private:

		string text;
		
		eFont font;
		unsigned int position;
		Color color;
		eString eText;
		bool textMode;
		bool pressed; // for buttons only, draw the text +(1,1)	
		Rect textBox;
};

#endif // _UI_STATICTEXT_HPP

