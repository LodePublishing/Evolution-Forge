#include "mapmenu.hpp"

MapMenu::MapMenu(const MapMenu& object) :
    Menu((Menu)object)
{ }

MapMenu& MapMenu::operator=(const MapMenu& object)
{
	((Menu)(*this)) = ((Menu)object);
	return(*this);
}

MapMenu::MapMenu(UI_Object* map_parent, Rect map_rect):
	Menu(map_parent, map_rect, true)
{ 
	resetData();
}

void MapMenu::resetData()
{
	unsigned int i=0;
	height=2;
	for(std::list<MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
	{
		if(i >= database.getMapCount())
		{
			(*m)->Hide();
			continue;
		}
		(*m)->Show();
		(*m)->setButton(eButton(UNIT_TYPE_4_BUTTON));
		(*m)->updateText(database.getMap(i)->getName());
		Rect edge = Rect(Point(10, height * (FONT_SIZE + 9)), Size(160,0));
		(*m)->adjustRelativeRect(edge);
		height++;
		i++;
	}
	for(;i<database.getMapCount();i++)
	{
		MenuEntry* entry = new MenuEntry(this, Rect(Point(10, height * (FONT_SIZE + 9)), Size(160, FONT_SIZE)), database.getMap(i)->getName());
		entry->setButton(eButton(UNIT_TYPE_4_BUTTON));
		menuEntries.push_back(entry);
		height++;
	}
	height++; // TODO
}


MapMenu::~MapMenu()
{ }

void MapMenu::process()
{
	Menu::process();
	if(!isShown())
		return;
}

void MapMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	Menu::draw(dc);
}

