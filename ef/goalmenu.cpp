#include "goalmenu.hpp"

GoalMenu::GoalMenu(const GoalMenu& object) :
    Menu((Menu)object), // TODO 120
    anarace(object.anarace)
{ }

GoalMenu& GoalMenu::operator=(const GoalMenu& object)
{
	((Menu)(*this)) = ((Menu)object);
	anarace = object.anarace;
	return(*this);
}

GoalMenu::GoalMenu(UI_Object* goal_parent, ANARACE* goal_anarace, Rect goal_rect):
	Menu(goal_parent, goal_rect), // TODO 120
	anarace(goal_anarace)
{
    for(int i=0;i<20;i++) // TODO
    {
        MenuEntry* entry = new MenuEntry(this,
                        Rect(0, 0, 110, FONT_SIZE+4),
                        Rect(0, 0, 110, FONT_SIZE+4), "Error");
        menuEntries.push_back(entry);
    }
}

GoalMenu::~GoalMenu()
{ }

void GoalMenu::process()
{
	if(!isShown())
		return;
	Menu::process();
		
	if(menuLevel)
	{
		int i = 0;
        for(list<MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
		{
			Rect edge = Rect(Point(10, height * (FONT_SIZE + 9)), Size(getParent()->getWidth()+20, FONT_SIZE));
			if( i >= settings.getGoalCount(anarace->getPlayerNum()) )
			{
				(*m)->Hide();
				continue;
			}
//			if (fitItemToClientRect(edge, 1))
			{
            	(*m)->Show();
				(*m)->adjustRelativeRect(edge);
				(*m)->setButton(eButton(UNIT_TYPE_1_BUTTON));
				(*m)->updateText(settings.getGoal(anarace->getPlayerNum(), i)->getName());
			}
            if ((*m)->isLeftClicked())
            {
                pressedItem = i;
                open();
                break;
            }
			height++;
			i++;
		}
	}
	else 
        for(list<MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
			(*m)->Hide();
}

void GoalMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	Menu::draw(dc);
}

