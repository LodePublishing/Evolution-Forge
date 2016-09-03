#ifndef _UI_MENU_HPP
#define _UI_MENU_HPP

#include "menuentry.hpp"
#include <list>

enum eMenuType
{
	CUSTOM_MENU,
	ONE_COLOUMN_MENU,
	TWO_COLOUMNS_MENU,
	HORIZONTAL_MENU
};

class UI_Menu : public UI_Object
{
	public:
// Anzahl, Position, coloumns, height / width, first string, Button
// TODO Button noch rein evtl
		UI_Menu(UI_Object* menu_parent, const Rect& rect, const Size distance_bottom_right, const ePositionMode position_mode, const bool choose_menu);
		UI_Menu(UI_Object* menu_parent, const Rect& rect, const Size distance_bottom_right, const ePositionMode position_mode, const unsigned int entryNumber, const unsigned int coloumns, const Size& s, const eString firstString, const eButtonColorsType button_colors_type, const bool choose_menu);
		
		UI_Menu(const UI_Menu& object);
		~UI_Menu();
		UI_Menu& operator=(const UI_Menu& object);
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
		void updateItemSizes(const unsigned int width);
		void updateItemPositions(const eMenuType menu_type);
	protected:
		void setMenuLevel(const unsigned int menu_level);
		std::list<UI_MenuEntry*> menuEntries;
		unsigned int menuLevel;
		signed int pressedItem;
		signed int markedItem;
		unsigned int height;
		bool chooseMenu;
	private:
		Point p1, p2;
		bool menuChanged;
};

#endif // _UI_MENU_HPP

