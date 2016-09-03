#ifndef _UI_MENU_HPP
#define _UI_MENU_HPP

#include "menuentry.hpp"

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
		// if all items to 'choose_menu' are used then it is a custom menu and reloadOriginalSize, updateItemPositions etc will not be called
		UI_Menu(UI_Object* menu_parent, const Rect& rect, const Size distance_bottom_right, const ePositionMode position_mode, const bool choose_menu, eMenuType menu_type = CUSTOM_MENU, const eButtonWidthType button_width_type = STANDARD_BUTTON_WIDTH, const unsigned int entryNumber = 0, const eString firstString = NULL_STRING, const eButtonColorsType button_colors_type = STANDARD_BUTTON);
		~UI_Menu();
		void process();
		void draw() const;
		const unsigned int getHeight() const;
		const signed int getPressedItem();
		const signed int getMarkedItem() const;

		void forcePressItem(const unsigned int number);
		void close();
		void open();		
		const bool isOpen() const;
		const bool menuHasChanged() const;
		void setMenuHasChanged(const bool has_changed = true);
		void updateItemSizes(const unsigned int width);
		void updateItemPositions();
		void reloadOriginalSize();
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
		eButtonWidthType buttonWidthType;
		eMenuType menuType;
	
		UI_Menu& operator=(const UI_Menu& object);
		UI_Menu(const UI_Menu& object);
};

inline const bool UI_Menu::menuHasChanged() const {
	return(menuChanged);
}
	
inline const signed int UI_Menu::getMarkedItem() const {
	return(markedItem);
}


#endif // _UI_MENU_HPP

