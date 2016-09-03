#ifndef _GUI_SAVEBOX_HPP
#define _GUI_SAVEBOX_HPP

#include "../ui/window.hpp"
#include "../ui/editfield.hpp"

// simplified window class, maybe inheritate UI_Window from SaveBox...
class SaveBox : public UI_Window
{
	public:
		SaveBox(UI_Object* savebox_parent, const eString savebox_text=NULL_STRING, const eString description_text=NULL_STRING, const eString ok_string=SAVE_BOX_OK_STRING, const eString cancel_string=SAVE_BOX_CANCEL_STRING, const std::string& name_proposal="");
		~SaveBox();
		SaveBox(const SaveBox& object);
		SaveBox& operator=(const SaveBox& object);

		void draw(DC* dc) const;
		void process();
		
		UI_Object* checkToolTip();
		UI_Object* checkHighlight();
		
		void reloadOriginalSize();
		
		const bool isCanceled() const;
		const bool isDone() const;
		void forceCancel();
		void forceDone();
		const std::string& getString() const;
	private:
		UI_Button* OK_Button;
		UI_Button* Cancel_Button;
		UI_EditField* editField;
};



inline void SaveBox::forceCancel() {
	Cancel_Button->forcePress();
}

inline void SaveBox::forceDone() {
	OK_Button->forcePress();
}

inline const std::string& SaveBox::getString() const {
	return(editField->getText()->getString());
}

#endif

