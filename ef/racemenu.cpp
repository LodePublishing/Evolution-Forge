#include "racemenu.hpp"
#include <list>

RaceMenu& RaceMenu::operator=(const RaceMenu& object)
{
	((Menu)(*this)) = ((Menu)object);
	return(*this);
}

RaceMenu::RaceMenu(const RaceMenu& object) :
	Menu((Menu)object)
{ }

RaceMenu::RaceMenu(UI_Object* race_parent, const Rect race_rect, const Size distance_bottom_right, const ePositionMode position_mode) :
	Menu(race_parent, race_rect, distance_bottom_right, position_mode, true)
{
//	height = 3; => ForceMenu
	height = 1;
	for(int i=0;i<MAX_RACES;++i)
	{
//		MenuEntry* entry = new MenuEntry(this, Rect(Point(10 + i * 90, height * (FONT_SIZE+9)), Size(75, FONT_SIZE+3)), (eString)(TERRA_STRING+i)); => ForceMenu
		MenuEntry* entry = new MenuEntry(this, Rect(Point(0, 3+ 14 * i + height * (FONT_SIZE+9)), Size(75, FONT_SIZE+3)), (eString)(TERRA_STRING+i));
		menuEntries.push_back(entry);
	}
	std::list<MenuEntry*>::iterator m = menuEntries.begin();
	(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_5_BUTTON));++m;
	(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_7_BUTTON));++m;
	(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_3_BUTTON));
//	height+=1; => ForceMenu
	height+=3;
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

