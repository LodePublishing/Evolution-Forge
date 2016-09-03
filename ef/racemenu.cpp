#include "racemenu.hpp"
#include <list>

RaceMenu::RaceMenu(UI_Object* race_parent, const Rect race_rect, const Size distance_bottom_right, const ePositionMode position_mode) :
	UI_Menu(race_parent, race_rect, distance_bottom_right, position_mode, true)
{
//	height = 0;
	for(unsigned int i=0;i<MAX_RACES;++i)
	{
//		UI_MenuEntry* entry = new UI_MenuEntry(this, Rect(Point(10 + i * 90, height * (FONT_SIZE+9)), Size(75, FONT_SIZE+3)), (eString)(TERRA_STRING+i)); => ForceMenu
//		UI_MenuEntry* entry = new UI_MenuEntry(this, Rect(Point(0, 3+ height * (FONT_SIZE+9)), Size(75, FONT_SIZE+3)), (eString)(TERRA_STRING+i));
		UI_MenuEntry* entry = new UI_MenuEntry(this, Rect(), (eString)(TERRA_STRING+i));
		menuEntries.push_back(entry);
//		++height;
	}
	std::list<UI_MenuEntry*>::iterator m = menuEntries.begin();
	(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_5_BUTTON));++m;
	(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_7_BUTTON));++m;
	(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_3_BUTTON));
        
	reloadOriginalSize();
}
	
RaceMenu::~RaceMenu()
{ }

void RaceMenu::reloadOriginalSize()
{
	updateItemSizes(UI_Object::theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH)/2);
	UI_Object::reloadOriginalSize();
	updateItemPositions(HORIZONTAL_MENU);
}


void RaceMenu::process()
{
	UI_Menu::process();
	if(!isShown())
		return;
}

void RaceMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Menu::draw(dc);
}

