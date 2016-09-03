#ifndef _GUI_MENU_HPP
#define _GUI_MENU_HPP

#include "menuentry.hpp"
#include "../core/anarace.hpp"
#include <list>

class Menu : public UI_Object
{
	public:
		Menu(UI_Object* parent, Rect rect/*, const unsigned int width, const unsigned int number*/);
		Menu(const Menu& object);
		~Menu();
		Menu& operator=(const Menu& object);
		void process();
		void draw(DC* dc) const;
		const unsigned int getHeight() const;
		const signed int getPressedItem() const;
		void close();
		void open();		
		const bool isOpen() const;
	protected:
		list<MenuEntry*> menuEntries;
		unsigned int menuLevel;
		signed int pressedItem;
		unsigned int height;
	private:
		Point p1, p2;
};

#endif // _GUI_MENU_HPP

