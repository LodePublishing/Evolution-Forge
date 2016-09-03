#ifndef _UI_RADIO_HPP
#define _UI_RADIO_HPP

#include "button.hpp"
#include "group.hpp"

class UI_Radio : public UI_Group
{
	public:
		UI_Radio& operator=(const UI_Radio& object);
		UI_Radio(const UI_Radio& object);
		UI_Radio(UI_Object* radio_parent=NULL, Rect initial_rect=Rect(0,0,0,0), Size distance_bottom_right=Size(0,0), const ePositionMode position_mode = DO_NOT_ADJUST, const eString radio_title = NULL_STRING);
		~UI_Radio();
		void addButton(UI_Button* button, const unsigned int button_id);
		void removeButton(const unsigned int button_id);

		const unsigned int getMarked();
		void forcePress(const unsigned int button_id);

		void leftButtonPressed(UI_Button* button);
		void leftButtonReleased(/*UI_Button* button*/); // allow release?		
		void rightButtonPressed(UI_Button* button);
		void rightButtonReleased(/*UI_Button* button*/); // allow release?		

		void process();
		void draw(DC* dc) const;
		
		const bool buttonHasChanged() const;
		void setButtonHasChanged(const bool button_changed = true);
		
		void forceUnpressAll();

		void updateIds();
	private:
		unsigned int markedItem;
		bool changed;
		bool reorder;
		UI_Button* buttonId[50];
		
};

inline void UI_Radio::rightButtonPressed(UI_Button* button) {
	leftButtonPressed(button);
}

inline void UI_Radio::rightButtonReleased(/*UI_Button* button*/) { // allow release?
	leftButtonReleased(/*button*/);
}

inline const bool UI_Radio::buttonHasChanged() const {
	return(changed);
}

inline void UI_Radio::setButtonHasChanged(const bool has_changed) {
	changed = has_changed;
}


#endif

