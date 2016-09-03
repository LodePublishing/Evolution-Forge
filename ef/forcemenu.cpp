#include "forcemenu.hpp"

ForceMenu::ForceMenu(UI_Object* parent, ANARACE* anarace, Rect rect):Menu(parent, anarace, rect)
{
}
	
ForceMenu::~ForceMenu()
{
}

void ForceMenu::process()
{
	if(!shown)
		return;
	Menu::process();
	if(menuLevel)
	{
    	for (int i = 0; i < settings.getStartconditionCount(); i++)
       		if (menuEntry[i]->isPressed())
       	        pressedItem = i;
		
		for(int i = 0; i<settings.getStartconditionCount(); i ++)
			if(settings.getStartcondition(i)->getRace() == anarace->getRace())
			{
				Rect edge = Rect(Point(10, height*(FONT_SIZE+5)), Size(getParent()->getWidth(), FONT_SIZE));
	//			if (parent->fitItemToRelativeRect(edge, 1)) 
				{
					menuEntry[i]->Show();
					menuEntry[i]->setButton(eButton(UNIT_TYPE_2_BUTTON));
					menuEntry[i]->updateText(settings.getStartcondition(i)->getName());
					menuEntry[i]->adjustRelativeRect(edge);
				}
				height++;
			} else menuEntry[i]->Hide();
	} else
	for(int i=0;i<settings.getStartconditionCount();i++)
		menuEntry[i]->Hide();
}

void ForceMenu::draw(DC* dc) const
{
	if(!shown)
		return;
	UI_Object::draw(dc);
}

