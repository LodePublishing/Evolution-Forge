#ifndef _UI_EDITFIELD_HPP
#define _UI_EDITFIELD_HPP

#include "window.hpp"

// simplified window class, maybe inheritate UI_Window from UI_EditField...
class UI_EditField : public UI_Window
{
	public:
		UI_EditField(UI_Object* edit_parent, UI_Object* edit_caller, const eString edit_text=NULL_STRING, const eString description_text=NULL_STRING, const eString ok_string=EDIT_FIELD_OK_STRING, const eString cancel_string=EDIT_FIELD_CANCEL_STRING, const std::string& name_proposal="");
		~UI_EditField();
		UI_EditField(const UI_EditField& object);
		UI_EditField& operator=(const UI_EditField& object);

		void draw(DC* dc) const;
		void process();
		
		UI_Object* checkToolTip();
		UI_Object* checkHighlight();
		
		void reloadOriginalSize();
		
		void addChar(char a);
		void removeCharBackspace();
		void removeCharDelete();

		void moveLeft();
		void moveRight();

		const std::string& getString() const;
		const UI_Object* getCaller() const;
		
		const bool isCanceled() const;
		const bool isDone() const;
		void forceCancel();
		void forceDone();
		
	private:
		UI_Button* OK_Button;
		UI_Button* Cancel_Button;
		UI_StaticText* editText;
		UI_StaticText* userText;
		int position;
		UI_Object* caller;
		int ani;
};

inline const std::string& UI_EditField::getString() const {
	return(userText->getString());
}

inline const bool UI_EditField::isCanceled() const {
	return(Cancel_Button->isLeftClicked());
}

inline const bool UI_EditField::isDone() const {
	return(OK_Button->isLeftClicked());
}

inline void UI_EditField::forceCancel() {
	Cancel_Button->forcePress();
}

inline void UI_EditField::forceDone() {
	OK_Button->forcePress();
}

inline const UI_Object* UI_EditField::getCaller() const {
	return(caller);
}

#endif

