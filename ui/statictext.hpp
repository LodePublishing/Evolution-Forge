#ifndef _UI_STATICTEXT_HPP
#define _UI_STATICTEXT_HPP

#include "object.hpp"

class UI_StaticText : public UI_Object
{
	public:
		UI_StaticText& operator=(const UI_StaticText& object);
		UI_StaticText(const UI_StaticText& object);
		
		UI_StaticText(UI_Object* st_parent, const Rect st_pos, const Size distance_bottom_right, const eColor st_color, const eFont st_font, const ePositionMode position_mode = HORIZONTALLY_CENTERED, const eAutoSize auto_size = NO_AUTO_SIZE);
		UI_StaticText(UI_Object* st_parent, const eString st_text, const Rect st_pos, const Size distance_bottom_right, const eColor st_color, const eFont st_font, const ePositionMode position_mode = HORIZONTALLY_CENTERED, const eAutoSize auto_size = NO_AUTO_SIZE);
		UI_StaticText(UI_Object* st_parent, const std::string& st_text, const Rect st_pos, const Size distance_bottom_right, const eColor st_color, const eFont st_font, const ePositionMode position_mode = HORIZONTALLY_CENTERED, const eAutoSize auto_size = NO_AUTO_SIZE);
		~UI_StaticText();

		void setColor(const Color st_color);
		void setColor(const eColor st_color);
		void setFont(const eFont st_font);
		const std::string& getString() const;
		void updateText(const std::string& st_text);
		void reloadText(const std::string& st_text);
		void reloadOriginalSize();

		void addChar(const unsigned int position, const char key);
		void removeCharBackspace(const unsigned int position);
		void removeCharDelete(const unsigned int position);

		UI_Object* checkToolTip();
		UI_Object* checkHighlight();
	
		void updateText(const eString st_text);
		void reloadText(const eString st_text);
		void reloadStrings();
		
		void draw(DC* dc) const;
		void process();

		const Size getTextSize() const;
		const Rect& getTextBox() const;
		const Size getTextPosSize(const unsigned int pos) const;

		void setPressed(const bool press=false);
		void doHighlight(const bool high_light=true);
	private:
		std::string text;
		bool textWasChanged;
		eFont font;
		Color color;
		eString eText;
		bool pressed; // for buttons only, draw the text +(1,1)	
		bool highlight;
};



inline void UI_StaticText::setPressed(const bool press) {
	pressed = press;
}

inline UI_Object* UI_StaticText::checkHighlight() {
	return(UI_Object::checkHighlight());
}

inline const Size UI_StaticText::getTextSize() const {
	return(theme.lookUpFont(font)->GetTextExtent(text));
}

inline const Size UI_StaticText::getTextPosSize(const unsigned pos) const {
	return(theme.lookUpFont(font)->GetTextExtent(text.substr(0, pos)));
}

inline void UI_StaticText::setColor(const eColor st_color) {
	color = *theme.lookUpColor(st_color);
}

inline void UI_StaticText::setColor(const Color st_color) {
	color = st_color;
}

inline const std::string& UI_StaticText::getString() const {
	return(text);
}

inline void UI_StaticText::setFont(const eFont st_font) {
	font = st_font;
}

#endif // _UI_STATICTEXT_HPP

