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

GoalMenu::GoalMenu(UI_Object* goal_parent, Rect goal_rect):
	Menu(goal_parent, goal_rect, true),
	anarace(NULL)
{ }

void GoalMenu::assignAnarace(ANABUILDORDER* goal_anarace)
{
	anarace = goal_anarace;
	resetData();
}

void GoalMenu::resetData()
{
	unsigned int i=0;
	height=3;
	for(std::list<MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
	{
		if(i >= database.getGoalCount(anarace->getRace()))
		{
			(*m)->Hide();
			continue;
		}
		(*m)->Show();
		(*m)->setButton(eButton(UNIT_TYPE_1_BUTTON));
		(*m)->updateText(database.getGoal(anarace->getRace(), i)->getName());
		Rect edge = Rect(Point(10, height * (FONT_SIZE + 9)), Size(160,0));
		(*m)->adjustRelativeRect(edge);
		height++;
		i++;
	}
	for(;i<database.getGoalCount(anarace->getRace());i++)
	{
		MenuEntry* entry = new MenuEntry(this, Rect(Point(10, height * (FONT_SIZE + 9)), Size(160, FONT_SIZE)), database.getGoal(anarace->getRace(), i)->getName());
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

