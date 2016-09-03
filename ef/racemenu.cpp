#include "racemenu.hpp"

RaceMenu::RaceMenu(UI_Object* parent, ANARACE* anarace, Rect rect):Menu(parent, anarace, rect)
{
}
	
RaceMenu::~RaceMenu()
{
}

void RaceMenu::process()
{
	if(!shown)
		return;
	Menu::process();

	if(menuLevel)
	{
    	for (int i = 0; i < MAX_RACES; i++)
       		if (menuEntry[i]->isPressed())
       	        pressedItem = i;
		
		for(int i = 0; i<MAX_RACES;	i ++)
		{
			Rect edge = Rect(Point(10, height*(FONT_SIZE+6)), Size(getParent()->getWidth(), FONT_SIZE+5));
//			if (parent->fitItemToRelativeRect(edge, 1)) 
			{
				menuEntry[i]->Show();
				menuEntry[i]->setButton(eButton(UNIT_TYPE_4_BUTTON));
				menuEntry[i]->updateText(*theme.lookUpString((eString)(TERRA_STRING+i)));
				menuEntry[i]->adjustRelativeRect(edge);
			}
			height++;
		} 
	} else 
	for(int i=0;i<MAX_RACES;i++)
		menuEntry[i]->Hide();
}

void RaceMenu::draw(DC* dc) const
{
	if(!shown)
		return;
	UI_Object::draw(dc);
}

