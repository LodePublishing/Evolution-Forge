#include "thememenu.hpp"

ThemeMenu::ThemeMenu(UI_Object* thememenu_parent, Rect rect) :
	Menu(thememenu_parent, rect, true)
{
    for(int i=0;i<MAX_COLOR_THEMES-1;i++)
    {
		Rect edge = Rect(Point(10 + (i%2) * 90, (i/2)*20 + height * (FONT_SIZE+9)), Size(75, FONT_SIZE+3));
        MenuEntry* entry = new MenuEntry(this, edge, (eString)(SETTING_DARK_BLUE_THEME_STRING+i));
		entry->setButton(eButton(UNIT_TYPE_4_BUTTON));
   		menuEntries.push_back(entry);
    }
}

ThemeMenu::~ThemeMenu() 
{ }

void ThemeMenu::process()
{
	Menu::process();
	if(!isShown())
		return;
}
	
void ThemeMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	Menu::draw(dc);
}	

