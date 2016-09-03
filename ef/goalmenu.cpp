#include "goalmenu.hpp"

GoalMenu::GoalMenu(const GoalMenu& object) :
    Menu((Menu)object),
	anarace(object.anarace)
{ }

GoalMenu& GoalMenu::operator=(const GoalMenu& object)
{
	((Menu)(*this)) = ((Menu)object);
	anarace = object.anarace;
	return(*this);
}

GoalMenu::GoalMenu(UI_Object* goal_parent, ANARACE* goal_anarace, Rect goal_rect):
	Menu(goal_parent, goal_rect, true),
	anarace(goal_anarace)
{
	resetData();
}

void GoalMenu::resetData()
{
	unsigned int i=0;
	height=3;
	for(list<MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
	{
	if(i >= settings.getGoalCount(anarace->getPlayerNumber()))
		{
			(*m)->Hide();
			continue;
		}

		(*m)->Show();
		(*m)->setButton(eButton(UNIT_TYPE_1_BUTTON));
		(*m)->updateText(settings.getGoal(anarace->getPlayerNumber(), i)->getName());
		Rect edge = Rect(Point(10, height * (FONT_SIZE + 9)), Size(0,0));
		(*m)->adjustRelativeRect(edge);
		height++;
		i++;
		
    }
	for(;i<settings.getGoalCount(anarace->getPlayerNumber());i++)
	{
        MenuEntry* entry = new MenuEntry(this, Rect(Point(10, height * (FONT_SIZE + 9)), Size(160, FONT_SIZE)), settings.getGoal(anarace->getPlayerNumber(), i)->getName());
        entry->setButton(eButton(UNIT_TYPE_1_BUTTON));
        menuEntries.push_back(entry);
        height++;
	}
}


GoalMenu::~GoalMenu()
{ }

void GoalMenu::process()
{
	Menu::process();
	if(!isShown())
		return;
}

void GoalMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	Menu::draw(dc);
}

