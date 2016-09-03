#include "resolutionmenu.hpp"

ResolutionMenu::ResolutionMenu(UI_Object* resmenu_parent, Rect rect) :
	Menu(resmenu_parent, rect, true)
{
	for(int i=1;i<MAX_RESOLUTIONS;i++)
    {
		Rect edge = Rect(Point(10, i*20 + height * (FONT_SIZE+9)), Size(75, FONT_SIZE+3));
 		MenuEntry* entry = new MenuEntry(this, edge, (eString)(SETTING_RESOLUTION_ZERO_STRING+i));
		entry->setButton(eButton(UNIT_TYPE_6_BUTTON));
   		menuEntries.push_back(entry);
    }
}

ResolutionMenu::~ResolutionMenu() 
{ }

void ResolutionMenu::process()
{
	Menu::process();
	if(!isShown())
		return;
}
	
void ResolutionMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	Menu::draw(dc);
}	

