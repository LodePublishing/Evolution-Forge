#include "thememenu.hpp"

ThemeMenu::ThemeMenu(UI_Object* thememenu_parent, const Rect rect, const Size distance_bottom_right, const ePositionMode position_mode) :
	UI_Menu(thememenu_parent, rect, distance_bottom_right, position_mode, true, ONE_COLOUMN_MENU, SMALL_BUTTON_WIDTH, MAX_COLOR_THEMES-1, SETTING_DARK_RED_THEME_STRING, UNIT_TYPE_4_BUTTON)
{}

ThemeMenu::~ThemeMenu() 
{ }

void ThemeMenu::process()
{
	UI_Menu::process();
	if(!isShown())
		return;
}
	
void ThemeMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Menu::draw(dc);
}	

