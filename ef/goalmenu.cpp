#include "goalmenu.hpp"

GoalMenu::GoalMenu(const GoalMenu& object) :
    Menu((Menu)object)
{ }

GoalMenu& GoalMenu::operator=(const GoalMenu& object)
{
	((Menu)(*this)) = ((Menu)object);
	return(*this);
}

GoalMenu::GoalMenu(UI_Object* goal_parent, ANARACE* goal_anarace, Rect goal_rect):
	Menu(goal_parent, goal_rect, true)
{
	height = 3;
    for(unsigned int i=0;i<settings.getGoalCount(goal_anarace->getPlayerNum());i++)
    {
        MenuEntry* entry = new MenuEntry(this, Rect(Point(10, height * (FONT_SIZE + 9)), Size(160, FONT_SIZE)), settings.getGoal(goal_anarace->getPlayerNum(), i)->getName());
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

