#ifndef _GUI_MENU_HPP
#define _GUI_MENU_HPP

#include "menuentry.hpp"
#include <list>

class Menu : public UI_Object
{
	public:
// Anzahl, Position, coloumns, height / width, first string, Button
// TODO Button noch rein evtl
		Menu(UI_Object* menu_parent, Rect rect, const bool choose_menu);
		Menu(UI_Object* menu_parent, Rect rect, const unsigned int entryNumber, const unsigned int coloumns, const Size& s, const eString firstString, const eButton button, const bool choose_menu);
		
		Menu(const Menu& object);
		~Menu();
		Menu& operator=(const Menu& object);
		void process();
		void draw(DC* dc) const;
		const unsigned int getHeight() const;
		const signed int getPressedItem() const;
		const signed int getMarkedItem() const;
		void close();
		void open();		
		const bool isOpen() const;
		const bool menuHasChanged() const;
		void setMenuHasChanged(const bool has_changed = true);
	protected:
		void setMenuLevel(const unsigned int menu_level);
		std::list<MenuEntry*> menuEntries;
		unsigned int menuLevel;
		signed int pressedItem;
		signed int markedItem;
		unsigned int height;
		bool chooseMenu;
	private:
		Point p1, p2;
		bool menuChanged;
};

#endif // _GUI_MENU_HPP

