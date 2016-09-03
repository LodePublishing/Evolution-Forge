#ifndef _UI_EDITFIELD_HPP
#define _UI_EDITFIELD_HPP

#include "button.hpp"


// TODO: Editfield aus Button und Text bestehen lassen... wegen UI_Group (die Box) und wegen besserer Uebersicht (damits net immer ein extra STaticText braucht)
// 
class UI_EditField : public UI_Object
{
	public:
		UI_EditField(UI_Object* edit_parent, const Rect& edit_rect, const Size& edit_size, const eFont st_font, const ePositionMode position_mode, const eString description_text, const std::string& name_proposal);
		UI_EditField(UI_Object* edit_parent, const Rect& edit_rect, const Size& edit_size, const eFont st_font, const ePositionMode position_mode, const std::string& description_text, const std::string& name_proposal);
		~UI_EditField();
		UI_EditField(const UI_EditField& object);
		UI_EditField& operator=(const UI_EditField& object);

		void draw(DC* dc) const;
		void process();
		
		UI_Object* checkToolTip();
		UI_Object* checkHighlight();
	
		const bool addKey(unsigned int key, unsigned int mod);

		const UI_StaticText* getText() const;
		
		void addChar(char a);
		void removeCharBackspace();
		void removeCharDelete();
		void enterWasPressed();
		void escapeWasPressed();
		const bool done() const;
		const bool canceled() const;

		void resetData();

		void moveLeft();
		void moveRight();

	private:
		int position;
		int ani;
		bool pressedEnter;
		bool pressedEscape;

		UI_Button* editField;
		UI_StaticText* descriptionText;
};

inline const UI_StaticText* UI_EditField::getText() const {
	return(editField->getText());
}

inline void UI_EditField::enterWasPressed() {
	pressedEnter = true;
}

inline void UI_EditField::escapeWasPressed() {
	pressedEscape = true;
}

inline const bool UI_EditField::done() const {
	return(pressedEnter);
}

inline const bool UI_EditField::canceled() const {
	return(pressedEscape);
}


#endif

