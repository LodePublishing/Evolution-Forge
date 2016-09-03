#include "racemenu.hpp"
#include <list>

RaceMenu::RaceMenu(UI_Object* race_parent, const Rect race_rect, const Size distance_bottom_right, const ePositionMode position_mode) :
	UI_Menu(race_parent, race_rect, distance_bottom_right, position_mode, true, HORIZONTAL_MENU, TINY_BUTTON_WIDTH, 3, TERRA_STRING, UNIT_TYPE_5_BUTTON)
{
	std::list<UI_MenuEntry*>::iterator m = menuEntries.begin();
	(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_5_BUTTON));++m;
	(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_7_BUTTON));++m;
	(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_3_BUTTON));
}
	
RaceMenu::~RaceMenu()
{ }

const bool RaceMenu::addKey(unsigned int key, unsigned int mod)
{
	if(((key != SDLK_t) && (key != SDLK_p) && (key != SDLK_z))||(!isOpen()))
		return(false);
	eRace race;
	switch(key)
	{
		case SDLK_t:race = TERRA;break;
		case SDLK_p:race = PROTOSS;break;
		case SDLK_z:race = ZERG;break;
		default:return(false);
	}
	forcePressItem(race);	
	return(true);
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

