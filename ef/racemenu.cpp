#include "racemenu.hpp"

RaceMenu& RaceMenu::operator=(const RaceMenu& object)
{
	((Menu)(*this)) = ((Menu)object);
	anarace = object.anarace;
	return(*this);
}

RaceMenu::RaceMenu(const RaceMenu& object) :
	Menu((Menu)object),
	anarace(object.anarace)
{ }

RaceMenu::RaceMenu(UI_Object* race_parent, ANARACE* race_anarace, Rect race_rect) :
	Menu(race_parent, race_rect), // 75
	anarace(race_anarace)
{
	for(int i=0;i<3;i++)
	{
		MenuEntry* entry = new MenuEntry(this,
						Rect(0, 0, 75, FONT_SIZE+4), 
						Rect(0, 0, 75, FONT_SIZE+4), *theme.lookUpString((eString)(TERRA_STRING+i)));
		menuEntries.push_back(entry);
	}
    list<MenuEntry*>::iterator m = menuEntries.begin();
    (*m)->updateToolTip(*theme.lookUpString(TERRA_STRING));
	(*m)->setButton(eButton(UNIT_TYPE_5_BUTTON));m++;
    (*m)->updateToolTip(*theme.lookUpString(PROTOSS_STRING));
	(*m)->setButton(eButton(UNIT_TYPE_7_BUTTON));m++;
    (*m)->updateToolTip(*theme.lookUpString(ZERG_STRING));
	(*m)->setButton(eButton(UNIT_TYPE_3_BUTTON));
}
	
RaceMenu::~RaceMenu()
{ }

void RaceMenu::process()
{
	if(!isShown())
		return;
	Menu::process();

	if(menuLevel)
	{
		int i=0;
    	for(list<MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
		{
			(*m)->Show();
			Rect edge = Rect(Point(10 + i * 90, height * (FONT_SIZE+9)), Size(75, FONT_SIZE+3));
//			if (parent->fitItemToRelativeRect(edge, 1)) 
			{
				(*m)->adjustRelativeRect(edge);
			}
       		if ((*m)->isLeftClicked())
			{
       	        pressedItem = i;
				open();
				break;
			}
			i++;
		}
		height++;
	}
	else
    	for(list<MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
				(*m)->Hide();

}

void RaceMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	Menu::draw(dc);
}

