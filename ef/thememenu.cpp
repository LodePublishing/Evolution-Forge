#include "thememenu.hpp"

ThemeMenu::ThemeMenu(UI_Object* thememenu_parent, const Rect rect, const Size distance_bottom_right, const ePositionMode position_mode) :
	UI_Menu(thememenu_parent, rect, distance_bottom_right, position_mode, true)
{
	for(unsigned int i=1;i<MAX_COLOR_THEMES;++i)
	{
		UI_MenuEntry* entry = new UI_MenuEntry(this, Rect(), (eString)(SETTING_ZERO_THEME_STRING+i));
		entry->setButtonColorsType(eButtonColorsType(UNIT_TYPE_4_BUTTON));
   		menuEntries.push_back(entry);
	}
	reloadOriginalSize();
}

ThemeMenu::~ThemeMenu() 
{ }

void ThemeMenu::reloadOriginalSize()
{
	updateItemSizes(UI_Object::theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH));
	UI_Object::reloadOriginalSize();
	updateItemPositions(TWO_COLOUMN_MENU);
}

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

