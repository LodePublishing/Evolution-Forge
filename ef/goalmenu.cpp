#include "goalmenu.hpp"

GoalMenu::GoalMenu(const GoalMenu& object) :
	UI_Menu((UI_Menu)object),
	anarace(object.anarace)
{ }

GoalMenu& GoalMenu::operator=(const GoalMenu& object)
{
	((UI_Menu)(*this)) = ((UI_Menu)object);
	anarace = object.anarace;
	return(*this);
}

GoalMenu::GoalMenu(UI_Object* goal_parent, const Rect goal_rect, const Size distance_bottom_right, const ePositionMode position_mode):
	UI_Menu(goal_parent, goal_rect, distance_bottom_right, position_mode, true),
	anarace(NULL)
{ }

void GoalMenu::assignAnarace(ANABUILDORDER* goal_anarace)
{
	bool race_has_changed = false;
	if((anarace==NULL)||(anarace->getRace() != goal_anarace->getRace()))
		race_has_changed = true;
	anarace = goal_anarace;
	if(race_has_changed)
		resetData();
}

void GoalMenu::reloadOriginalSize()
{
	for(std::list<UI_MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
		(*m)->setOriginalSize(Size(getParent()->getWidth()-40, FONT_SIZE+7));
	resetData();
	UI_Menu::reloadOriginalSize();
}
#include <sstream>
void GoalMenu::resetData()
{
	if(!anarace)
		return;
	unsigned int i=0;
	height=1;
	for(std::list<UI_MenuEntry*>::iterator m=menuEntries.begin(); m!=menuEntries.end(); ++m)
	{
		if(i >= database.getGoalCount(anarace->getRace()))
		{
			(*m)->Hide();
			continue;
		}
		(*m)->Show();
		if(i==0)
			(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_2_BUTTON));
		else
			(*m)->setButtonColorsType(eButtonColorsType(UNIT_TYPE_1_BUTTON));
		
		const GOAL_ENTRY* my_goal = database.getGoal(anarace->getRace(), i);
		(*m)->updateText(my_goal->getName());

		std::ostringstream os;
		for(std::list<GOAL>::const_iterator j = my_goal->goal.begin();j!=my_goal->goal.end();++j)
		{
			os << j->getCount() << "x $" << stats[anarace->getRace()][j->getUnit()].name << "$";
			if(j->getTime()>0)
				os << " [" << formatTime(j->getTime()) << "]"; 
			os << "#";
		}
		(*m)->updateToolTip(os.str());
		
		Rect edge = Rect(Point(10, height * (FONT_SIZE + 9)), Size(getParent()->getWidth()-40, FONT_SIZE+7));
		(*m)->adjustRelativeRect(edge);
		++height;
		++i;
	}
	for(;i<database.getGoalCount(anarace->getRace());++i)
	{
		UI_MenuEntry* entry = new UI_MenuEntry(this, Rect(Point(10, height * (FONT_SIZE + 9)), Size(getParent()->getWidth()-40, FONT_SIZE+7)), database.getGoal(anarace->getRace(), i)->getName());
		if(i==0)
			entry->setButtonColorsType(eButtonColorsType(UNIT_TYPE_2_BUTTON));
		else
			entry->setButtonColorsType(eButtonColorsType(UNIT_TYPE_1_BUTTON));
		menuEntries.push_back(entry);
		++height;
	}
	++height;
}


GoalMenu::~GoalMenu()
{ }

void GoalMenu::process()
{
	UI_Menu::process();
	if(!isShown())
		return;
}

void GoalMenu::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Menu::draw(dc);
}

