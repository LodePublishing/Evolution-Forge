#include "goalmenu.hpp"

GoalMenu::GoalMenu(UI_Object* parent, ANARACE* anarace, Rect rect):Menu(parent, anarace, rect)
{
}

GoalMenu::~GoalMenu()
{
}

void GoalMenu::process()
{
	Menu::process();
		
	if(menuLevel)
	{
        for (int i = 0; i < settings.getGoalCount(); i++)
			if (menuEntry[i]->isPressed())
				pressedItem = i;
	
		for (int i = 0; i < settings.getGoalCount(); i++)
			if (settings.getGoal(i)->getRace() == anarace->getRace())
			{
				menuEntry[i]->Show();
				Rect edge = Rect(Point(10, height * (FONT_SIZE + 5)), Size(270, FONT_SIZE + 5));
//				if (fitItemToClientRect(edge, 1))
				{
					menuEntry[i]->setButton(eButton(UNIT_TYPE_1_BUTTON));
					menuEntry[i]->updateText(settings.getGoal(i)->getName());
					menuEntry[i]->adjustRelativeRect(edge);
				}
				height++;
			} else 
				menuEntry[i]->Hide();
	}
	else 
	for(int i=0;i<settings.getGoalCount();i++)
		menuEntry[i]->Hide();
}

void GoalMenu::draw(DC* dc) const
{
	if(!shown)
		return;
	UI_Object::draw(dc);
}

