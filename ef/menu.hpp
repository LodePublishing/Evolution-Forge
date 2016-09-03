#ifndef _GUI_MENU_HPP
#define _GUI_MENU_HPP

#include "menuentry.hpp"
#include "../core/anarace.hpp"

class Menu : public UI_Object
{
	public:
		Menu(UI_Object* parent, ANARACE* anarace, Rect rect);
		~Menu();
		void process();
		void draw(DC* dc) const;
		const int getHeight() const;
		const int getPressedItem() const;
		void close();
		void open();		
		const bool isOpen() const;
	protected:
		MenuEntry* menuEntry[UNIT_TYPE_COUNT]; // TODO Vector draus machen
		int menuLevel;
		int pressedItem;
		int height;
		ANARACE* anarace;
};

#endif // _GUI_MENU_HPP

