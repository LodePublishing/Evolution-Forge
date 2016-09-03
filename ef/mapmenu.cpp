#include "mapmenu.hpp"

MapMenu::MapMenu(const MapMenu& object) :
    UI_Menu((UI_Menu)object),
    anarace(object.anarace)
{ }

MapMenu& MapMenu::operator=(const MapMenu& object)
{
	((UI_Menu)(*this)) = ((UI_Menu)object);
	anarace = object.anarace;
	return(*this);
}

MapMenu::MapMenu(UI_Object* map_parent, const Rect map_rect, const Size distance_bottom_right, const ePositionMode position_mode):
	UI_Menu(map_parent, map_rect, distance_bottom_right, position_mode, true),
	anarace(NULL)
{ 
	resetData();
}

void MapMenu::resetData()
{
	unsigned int i=0;
	height=1;
	
	
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
		Rect edge = Rect(Point(0, i*(FONT_SIZE+6) + 25), Size(UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), FONT_SIZE+3));
		(*m)->adjustRelativeRect(edge);
		++height;
		++i;
	}
	for(;i<database.getMapCount();++i)
	{
		Rect edge = Rect(Point(0, i*(FONT_SIZE+6) + 25), Size(UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), FONT_SIZE+3));
		UI_MenuEntry* entry = new UI_MenuEntry(this, edge, database.getMap(i)->getName());
		entry->setButtonColorsType(eButtonColorsType(UNIT_TYPE_4_BUTTON));
		menuEntries.push_back(entry);
		++height;
	}
	++height; // TODO
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

