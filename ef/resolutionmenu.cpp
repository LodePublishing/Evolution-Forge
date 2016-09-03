#include "resolutionmenu.hpp"

ResolutionMenu::ResolutionMenu(UI_Object* resolution_menu_parent, const Rect rect, const Size distance_bottom_right, const ePositionMode position_mode) :
	UI_Menu(resolution_menu_parent, rect, distance_bottom_right, position_mode, true, ONE_COLOUMN_MENU, SMALL_BUTTON_WIDTH, MAX_RESOLUTIONS-1, SETTING_RESOLUTION_640x480_STRING, UNIT_TYPE_6_BUTTON)
{}

ResolutionMenu::~ResolutionMenu() 
{ }

void ResolutionMenu::process()
{
	UI_Menu::process();
	if(!isShown())
		return;
}
	
void ResolutionMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Menu::draw(dc);
}	

