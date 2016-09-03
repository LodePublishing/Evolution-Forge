#include "locationmenu.hpp"

LocationMenu::LocationMenu(const LocationMenu& object) :
	UI_Menu((UI_Menu)object),
	locationList(object.locationList)
{ }

LocationMenu& LocationMenu::operator=(const LocationMenu& object)
{
	((UI_Menu)(*this)) = ((UI_Menu)object);
	locationList = object.locationList;
	return(*this);
}

LocationMenu::LocationMenu(UI_Object* location_parent, const BASIC_MAP* location_map, const Rect& location_rect):
	UI_Menu(location_parent, location_rect, Size(0,0), DO_NOT_ADJUST, true),
	locationList()
{
	assignMap(location_map);
	resetData();
}

LocationMenu::LocationMenu(UI_Object* location_parent, const std::list<std::string>& location_list, const Rect& location_rect):
	UI_Menu(location_parent, location_rect, Size(0,0), DO_NOT_ADJUST, true),
	locationList(location_list)
{
	resetData();
}

void LocationMenu::resetData()
{
	std::list<std::string>::iterator i = locationList.begin();
	height=1;
	for(std::list<UI_MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
	{
		if(i == locationList.end())
		{
			(*m)->Hide();
			continue;
		}

		(*m)->Show();
		(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_6_BUTTON));
		(*m)->updateText(*i);
		Rect edge = Rect(Point(10, height * (FONT_SIZE + 9)), Size(0,0));
		(*m)->adjustRelativeRect(edge);
		++height;
		++i;
    	}
	for(;i!=locationList.end();++i)
	{
		UI_MenuEntry* entry = new UI_MenuEntry(this, Rect(Point(10, height * (FONT_SIZE + 9)), Size(160, FONT_SIZE)), *i);
		entry->setButtonColorsType(eButtonColorsType(UNIT_TYPE_6_BUTTON));
		menuEntries.push_back(entry);
		++height;
	}
	height+=2;
}


LocationMenu::~LocationMenu()
{ }

void LocationMenu::reloadOriginalSize()
{
	updateItemSizes(UI_Object::theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH));
	UI_Object::reloadOriginalSize();
	updateItemPositions(TWO_COLOUMNS_MENU);
}

void LocationMenu::assignMap(const std::list<std::string>& location_list)
{
	locationList.clear();
	locationList = location_list;
}

void LocationMenu::assignMap(const BASIC_MAP* location_map)
{
	locationList.clear();
	for(unsigned int i = 0;i < location_map->getMaxLocations();i++)
		locationList.push_back(location_map->getLocation(i)->getName());
}

void LocationMenu::process()
{
	UI_Menu::process();
	if(!isShown())
		return;
}

void LocationMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Menu::draw(dc);
}

