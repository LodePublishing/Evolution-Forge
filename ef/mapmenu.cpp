#include "mapmenu.hpp"

MapMenu::MapMenu(UI_Object* map_parent, const Rect map_rect, const Size distance_bottom_right, const ePositionMode position_mode):
	UI_Menu(map_parent, map_rect, distance_bottom_right, position_mode, true, ONE_COLOUMN_MENU, STANDARD_BUTTON_WIDTH)
{ 
	resetData();
	reloadOriginalSize();
}

void MapMenu::reloadOriginalSize()
{
	UI_Menu::reloadOriginalSize();
}

void MapMenu::resetData()
{
	unsigned int i=0;
	
	for(std::list<UI_MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
	{
		if(i >= database.getMapCount())
		{
			(*m)->Hide();
			continue;
		}
		(*m)->Show();
		(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_4_BUTTON));
		(*m)->updateText(database.getMap(i)->getName());
		++i;
	}
	for(;i<database.getMapCount();++i)
	{
		UI_MenuEntry* entry = new UI_MenuEntry(this, Rect(), database.getMap(i)->getName());
		entry->setButtonColorsType(eButtonColorsType(UNIT_TYPE_4_BUTTON));
		menuEntries.push_back(entry);
	}
	reloadOriginalSize();
}


MapMenu::~MapMenu()
{ }

void MapMenu::process()
{
	UI_Menu::process();
	if(!isShown())
		return;
}

void MapMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Menu::draw(dc);
}

