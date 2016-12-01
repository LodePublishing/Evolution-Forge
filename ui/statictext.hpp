#ifndef _UI_STATICTEXT_HPP
#define _UI_STATICTEXT_HPP

#include "object.hpp"

class UI_StaticText : public UI_Object
{
	public:
		UI_StaticText(UI_Object* st_parent, 
				const unsigned int string_id, 
				const Rect st_pos, 
				const Size distance_bottom_right, 
				const eColor st_color, 
				const eFont st_font, 
				const ePositionMode position_mode = HORIZONTALLY_CENTERED);
		UI_StaticText(UI_Object* st_parent, 
				const std::string& st_text,  // static text like numbers or names
				const Rect st_pos, 
				const Size distance_bottom_right, 
				const eColor st_color, 
				const eFont st_font, 
				const ePositionMode position_mode = HORIZONTALLY_CENTERED);
		~UI_StaticText();

		void setTemporaryColor(const Color& st_color);
		void setColor(const eColor st_color);
		void setFont(const eFont st_font);
		const std::string& getString() const;
		void updateText(const std::string& st_text, const bool etext_change = false);
		void reloadText(const std::string& st_text);
		void reloadOriginalSize();

		void addChar(const unsigned int position, const char key);
		void removeCharBackspace(const unsigned int position);
		void removeCharDelete(const unsigned int position);

		UI_Object* checkToolTip();
		UI_Object* checkHighlight();
	
		void updateText(const unsigned int string_id);
		void reloadText(const unsigned int string_id);
		void reloadStrings();
		void setTextWasChanged();
		
		void draw() const;
		void object_info();
		void process();

		const Size getTextSize() const;
		const Rect& getTextBox() const;
		const Size getTextPosSize(const unsigned int pos) const;

		void doHighlight(const bool high_light=true);
	private:
		std::string text;
		unsigned int stringID;
		bool textWasChanged;
		eFont font;
		eColor color;
		Color tempColor;
		bool tempColorIsSet;
		bool highlight;
	
		UI_StaticText& operator=(const UI_StaticText& object);
		UI_StaticText(const UI_StaticText& object);
};

inline UI_Object* UI_StaticText::checkHighlight() {
	return(UI_Object::checkHighlight());
}

inline const Size UI_StaticText::getTextSize() const {
	return(theme.lookUpFont(font)->getTextExtent(text));
}

inline const Size UI_StaticText::getTextPosSize(const unsigned pos) const {
	return(theme.lookUpFont(font)->getTextExtent(text.substr(0, pos)));
}

inline void UI_StaticText::setColor(const eColor st_color) {
	color = st_color;
}

inline void UI_StaticText::setTemporaryColor(const Color& st_color) {
	tempColorIsSet = true;
	tempColor = st_color;
}

inline const std::string& UI_StaticText::getString() const {
	return(text);
}

inline void UI_StaticText::setFont(const eFont st_font) {
	font = st_font;
}

#endif // _UI_STATICTEXT_HPP

