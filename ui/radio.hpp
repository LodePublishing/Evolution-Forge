#ifndef _UI_RADIO_HPP
#define _UI_RADIO_HPP

#include "button.hpp"

class UI_Radio : public UI_Object
{
	public:
		UI_Radio& operator=(const UI_Radio& object);
		UI_Radio(const UI_Radio& object);
		UI_Radio(UI_Object* radio_parent, const bool alwaysOn=false);
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

#endif

