#include "bitdepthmenu.hpp"

BitDepthMenu::BitDepthMenu(UI_Object* bit_depth_menu_parent, const Rect rect, const Size distance_bottom_right, const ePositionMode position_mode) :
	UI_Menu(bit_depth_menu_parent, rect, distance_bottom_right, position_mode, true, ONE_COLOUMN_MENU, SMALL_BUTTON_WIDTH, 1+DEPTH_32BIT-DEPTH_8BIT, SETTING_DEPTH_8BIT_STRING, UNIT_TYPE_7_BUTTON)
{ }

void BitDepthMenu::reloadOriginalSize()
{
	UI_Menu::reloadOriginalSize();
}

BitDepthMenu::~BitDepthMenu() 
{ }

void BitDepthMenu::process()
{
	UI_Menu::process();
	if(!isShown())
		return;
}
	
void BitDepthMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Menu::draw(dc);
}	

