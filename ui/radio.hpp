#ifndef _UI_RADIO_HPP
#define _UI_RADIO_HPP

#include "button.hpp"
#include "group.hpp"

class UI_Radio : public UI_Group
{
	public:
		UI_Radio& operator=(const UI_Radio& object);
		UI_Radio(const UI_Radio& object);
		UI_Radio(UI_Object* radio_parent=NULL, Point position=Point(0,0), const bool always_on=false, const eString radio_title = NULL_STRING);
		~UI_Radio();
		void addButton(UI_Button* button);

		const unsigned int getMarked();
		void forcePress(const unsigned int button);

		void leftButtonPressed(UI_Button* button);
		void leftButtonReleased(UI_Button* button); // allow release?		
		void rightButtonPressed(UI_Button* button);
		void rightButtonReleased(UI_Button* button); // allow release?		

		void process();
		void draw(DC* dc) const;
		
		const bool hasChanged() const;
		void forceUnpressAll();

	private:
		unsigned int markedItem;
		bool changed;
		bool alwaysOn;
};

inline void UI_Radio::rightButtonPressed(UI_Button* button) {
	leftButtonPressed(button);
}

inline void UI_Radio::rightButtonReleased(UI_Button* button) { // allow release?
	leftButtonReleased(button);
}

#endif

