#include "resolutionmenu.hpp"

ResolutionMenu::ResolutionMenu(UI_Object* resolution_menu_parent, const Rect rect, const Size distance_bottom_right, const ePositionMode position_mode) :
	UI_Menu(resolution_menu_parent, rect, distance_bottom_right, position_mode, true)
{
	for(unsigned int i=1;i<MAX_RESOLUTIONS;i++)
	{
 		UI_MenuEntry* entry = new UI_MenuEntry(this, Rect(), (eString)(SETTING_RESOLUTION_ZERO_STRING+i));
		entry->setButtonColorsType(eButtonColorsType(UNIT_TYPE_6_BUTTON));
   		menuEntries.push_back(entry);
	}
	reloadOriginalSize();
}

void ResolutionMenu::reloadOriginalSize()
{
	updateItemSizes(UI_Object::theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH));
	UI_Object::reloadOriginalSize();
	updateItemPositions(TWO_COLOUMN_MENU);
}

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

