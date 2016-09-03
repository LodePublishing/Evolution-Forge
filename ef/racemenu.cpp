#include "racemenu.hpp"

RaceMenu& RaceMenu::operator=(const RaceMenu& object)
{
	((Menu)(*this)) = ((Menu)object);
	return(*this);
}

RaceMenu::RaceMenu(const RaceMenu& object) :
	Menu((Menu)object)
{ }

RaceMenu::RaceMenu(UI_Object* race_parent, Rect race_rect) :
	Menu(race_parent, race_rect, true)
{
	height = 3;
	for(int i=0;i<MAX_RACES;i++)
	{
		MenuEntry* entry = new MenuEntry(this, Rect(Point(10 + i * 90, height * (FONT_SIZE+9)), Size(75, FONT_SIZE+3)), (eString)(TERRA_STRING+i));
		menuEntries.push_back(entry);
	}
	list<MenuEntry*>::iterator m = menuEntries.begin();
	(*m)->setButton(eButton(UNIT_TYPE_5_BUTTON));m++;
	(*m)->setButton(eButton(UNIT_TYPE_7_BUTTON));m++;
	(*m)->setButton(eButton(UNIT_TYPE_3_BUTTON));
	height+=1;
}
	
RaceMenu::~RaceMenu()
{ }

void RaceMenu::process()
{
	Menu::process();
	if(!isShown())
		return;
}

void RaceMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	Menu::draw(dc);
}

