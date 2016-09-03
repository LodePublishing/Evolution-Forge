#include "forcemenu.hpp"

ForceMenu::ForceMenu(UI_Object* force_parent, ANARACE* force_anarace, Rect force_rect):
	Menu(force_parent, force_rect, true)
{
	height = 3;
    for(int i=0;i<settings.getStartconditionCount( force_anarace->getPlayerNum() );i++) // TODO
    {
		MenuEntry* entry = new MenuEntry(this, Rect(Point(10, height*(FONT_SIZE+9)), Size(160, FONT_SIZE)), settings.getStartcondition(force_anarace->getPlayerNum(), i)->getName());
        entry->setButton(eButton(UNIT_TYPE_2_BUTTON));
		menuEntries.push_back(entry);
        height++;
    }
}

ForceMenu& ForceMenu::operator=(const ForceMenu& object)
{
	((Menu)(*this)) = ((Menu)object);
	return(*this);
}

ForceMenu::ForceMenu(const ForceMenu& object) : 
    Menu((Menu)object)
{ }
	
ForceMenu::~ForceMenu()
{ }

void ForceMenu::process()
{
	Menu::process();
	if(!isShown())
		return;
}

void ForceMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	Menu::draw(dc);
}

