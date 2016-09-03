#include "dataentry.hpp"
#include <sstream>

DataBaseEntry::DataBaseEntry(UI_Object* entry_parent, const Rect& entry_rect, const Size distance_bottom_right, const ePositionMode position_mode, const std::string& text, const bool is_goal, const unsigned int bo_count):
	UI_Object(entry_parent, Rect(entry_rect.getTopLeft(), Size(entry_parent->getWidth(), 12)), distance_bottom_right, position_mode, AUTO_HEIGHT_CONST_WIDTH),
	myGoal(0),
	myBO(0),
	entry(new UI_Button(this, Rect(Point(is_goal?15:30, 0), Size(getWidth() - (is_goal?45:60), 12)), Size(1, 1), is_goal?OPEN_TREE_BUTTON:NON_GOAL_ENTRY_BUTTON, true, (is_goal && (bo_count==0))?NO_BUTTON_MODE:CHECK_BUTTON_MODE, text, SPECIAL_BUTTON_LEFT)),
	checkButton(new UI_Button(this, Rect(Point(getWidth() - 30, 0), Size(12, 12)), Size(1, 1), CHECK_BUTTON, true, CHECK_BUTTON_MODE, NULL_STRING, DATA_ENTRY_CHECK_BUTTON_TOOLTIP_STRING)),
	goal(is_goal)
{
	if(goal)
	{
		std::ostringstream os;
		os << text << " [" << bo_count << "]";
		entry->updateText(os.str());
		entry->updateToolTip(DATA_ENTRY_OPEN_BUTTON_TOOLTIP_STRING);
	}
}

DataBaseEntry::~DataBaseEntry()
{
	delete entry;
	delete checkButton;
}

void DataBaseEntry::process()
{
	UI_Object::process();
	if(checkButton->checkForNeedRedraw())
		setNeedRedrawMoved();
}

void DataBaseEntry::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Object::draw(dc);
}

void DataBaseEntry::reloadOriginalSize()
{
	UI_Object::reloadOriginalSize();
	setOriginalSize(Size(getParent()->getWidth(), getHeight()));
	entry->setOriginalSize(Size(getWidth() - (goal?45:60), 12));
	checkButton->setOriginalPosition(Point(getWidth() - 24 , 0));
}

