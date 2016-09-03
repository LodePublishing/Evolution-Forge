#include "racemenu.hpp"
#include "../core/game.hpp"

RaceMenu::RaceMenu(UI_Object* race_parent, const Rect race_rect, const Size distance_bottom_right, const ePositionMode position_mode) :
	UI_Menu(race_parent, race_rect, distance_bottom_right, position_mode, true, HORIZONTAL_MENU, TINY_BUTTON_WIDTH)
{
	for(unsigned int i=0; i<GAME::MAX_RACES; ++i)
	{
		UI_MenuEntry* entry = new UI_MenuEntry(this, Rect(), GAME::lookUpGameString(GAME::FIRST_RACE_STRING + i));
		entry->setButtonColorsType(UNIT_TYPE_5_BUTTON);
		menuEntries.push_back(entry);
	}
	updateItemSizes(UI_Object::theme.lookUpButtonWidth(TINY_BUTTON_WIDTH));
	updateItemPositions();

//	std::list<UI_MenuEntry*>::iterator m = menuEntries.begin();
//	(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_5_BUTTON));++m; // TODO
//	(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_7_BUTTON));++m;
//	(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_3_BUTTON));
}

RaceMenu::~RaceMenu()
{ }

void RaceMenu::reloadStrings()
{
	unsigned int i = 0;
	for(std::list<UI_MenuEntry*>::iterator m = menuEntries.begin(); m != menuEntries.end(); ++m, ++i)
		(*m)->updateText(GAME::lookUpGameString(GAME::FIRST_RACE_STRING + i));
}

const bool RaceMenu::addKey(unsigned int key, unsigned int mod)
{
	if(((key != SDLK_t) && (key != SDLK_p) && (key != SDLK_z))||(!isOpen()))
		return(false);
	
	for(unsigned int i = GAME::MAX_RACES; i--;)
		if(key == SDLK_a + (GAME::race[i].hotkey - 'a'))
		{
			forcePressItem(i);
			return(true);
		}
	
	return(false);
}
	
void RaceMenu::process()
{
	UI_Menu::process();
	if(!isShown())
		return;
}

void RaceMenu::draw() const
{
	UI_Menu::draw();
}

