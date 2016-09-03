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
		
		UI_StaticText(UI_Object* st_parent, const std::string& st_text, const Rect st_pos, const eColor st_color, const eFont st_font, const eTextMode st_mode = NO_TEXT_MODE);

		~UI_StaticText();

		
		void setMode(const eTextMode st_mode);
		void setColor(const Color st_color);
		void setColor(const eColor st_color);
		void setFont(const eFont st_font);
		const std::string& getString() const;
		void updateText(const std::string& st_text);

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

		std::string text;
		
		eFont font;
		unsigned int position;
		Color color;
		eString eText;
		bool textMode;
		bool pressed; // for buttons only, draw the text +(1,1)	
		Rect textBox;
};

inline const Size& UI_StaticText::getBoxSize() const {
	return(textBox.GetSize());
}

inline void UI_StaticText::setPressed(const bool press) {
	pressed = press;
}

inline UI_Object* UI_StaticText::checkHighlight() {
	return(UI_Object::checkHighlight());
}

inline const bool UI_StaticText::doneWriting() const {
	return(position >= text.size());
}

inline const Rect& UI_StaticText::getTextBox() const {
	return(textBox);
}

inline const Size UI_StaticText::getTextSize() const {
	//return(textBox.GetSize());
	return(theme.lookUpFont(font)->GetTextExtent(text));
}

inline const Size UI_StaticText::getTextPosSize(const unsigned pos) const {
	return(theme.lookUpFont(font)->GetTextExtent(text.substr(0, pos)));
}

inline void UI_StaticText::setColor(const eColor st_color) {
	color=*theme.lookUpColor(st_color);
}

inline void UI_StaticText::setColor(const Color st_color) {
	color=st_color;
}

inline void UI_StaticText::setMode(const eTextMode st_mode) {
	mode=st_mode;
}

inline void UI_StaticText::setFont(const eFont st_font) {
	font=st_font;
}

#endif // _UI_STATICTEXT_HPP

