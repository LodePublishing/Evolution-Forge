#include "locmenu.hpp"

LocationMenu::LocationMenu(const LocationMenu& object) :
    Menu((Menu)object),
	anarace(object.anarace)
{ }

LocationMenu& LocationMenu::operator=(const LocationMenu& object)
{
	((Menu)(*this)) = ((Menu)object);
	anarace = object.anarace;
	return(*this);
}

LocationMenu::LocationMenu(UI_Object* location_parent, ANABUILDORDER* location_anarace, Rect location_rect):
	Menu(location_parent, location_rect, true),
	anarace(location_anarace)
{
	resetData();
}

void LocationMenu::resetData()
{
	unsigned int i=0;
	height=1;
	for(std::list<MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
	{
		if(i >= (*anarace->getMap())->getMaxLocations())
		{
			(*m)->Hide();
			continue;
		}

		(*m)->Show();
		(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_6_BUTTON));
		(*m)->updateText((*anarace->getMap())->getLocation(i)->getName());
		Rect edge = Rect(Point(10, height * (FONT_SIZE + 9)), Size(0,0));
		(*m)->adjustRelativeRect(edge);
		++height;
		++i;
    	}
	for(;i<(*anarace->getMap())->getMaxLocations();++i)
	{
		MenuEntry* entry = new MenuEntry(this, Rect(Point(10, height * (FONT_SIZE + 9)), Size(160, FONT_SIZE)), (*anarace->getMap())->getLocation(i)->getName());
		entry->setButtonColorsType(eButtonColorsType(UNIT_TYPE_6_BUTTON));
		menuEntries.push_back(entry);
		++height;
	}
	height+=2;
}


LocationMenu::~LocationMenu()
{ }

void LocationMenu::assignAnarace(ANABUILDORDER* location_anarace)
{
	anarace = location_anarace;
}

void LocationMenu::process()
{
	Menu::process();
	if(!isShown())
		return;
}

void LocationMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	Menu::draw(dc);
}

