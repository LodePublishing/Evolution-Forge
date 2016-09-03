#include "dataentry.hpp"
#include "configuration.hpp"
#include <sstream>

DataBaseEntry::DataBaseEntry(UI_Object* entry_parent, const Rect& entry_rect, const Size distance_bottom_right, const ePositionMode position_mode, const std::string& text, const eDataEntry data_entry_type, const unsigned int entry_count) :
	UI_Object(entry_parent, Rect(entry_rect.getTopLeft(), Size(entry_parent->getWidth(), 12)), distance_bottom_right, position_mode, AUTO_HEIGHT_CONST_WIDTH),
	myGoal(0),
	myBO(0),
	myRace(0),
	entry(new UI_Button(this, Rect(Point(data_entry_type==DATA_RACE_ENTRY?15:data_entry_type==DATA_GOAL_ENTRY?30:45, 0), 
					Size(getWidth() - 40 - (data_entry_type==DATA_RACE_ENTRY?15:data_entry_type==DATA_GOAL_ENTRY?30:45), 12)), Size(), 
				data_entry_type==DATA_BO_ENTRY?NON_GOAL_ENTRY_BUTTON:OPEN_TREE_BUTTON, INCREASE_BITMAP, 
				((data_entry_type!=DATA_BO_ENTRY) && (entry_count==0))?NO_BUTTON_MODE:CHECK_BUTTON_MODE, text, SPECIAL_BUTTON_LEFT)),
	checkButton(new UI_Button(this, Rect(Point(getWidth() - 30, 0), Size(12, 12)), Size(), CHECK_BUTTON, RADIO_OFF, CHECK_BUTTON_MODE, NULL_STRING, DATA_ENTRY_CHECK_BUTTON_TOOLTIP_STRING)),
	entryType(data_entry_type),
	entryCount(entry_count),
	entryText(text)
{
	reloadStrings();
}

DataBaseEntry::~DataBaseEntry()
{
	delete entry;
	delete checkButton;
}


void DataBaseEntry::reloadStrings()
{
	if(entryCount > 0)
	{
		if(entryType==DATA_BO_ENTRY)
		{
			std::ostringstream os;
			os << "[" << formatTime(entryCount, efConfiguration.getGameSpeed()) << "] " << entryText;
			entry->updateText(os.str());
			entry->updateToolTip(NULL_STRING);
		} else
		if(entryType!=DATA_BO_ENTRY)
		{
			std::ostringstream os;
			os << entryText << " [" << entryCount << "]";
			entry->updateText(os.str());
			entry->updateToolTip(DATA_ENTRY_OPEN_BUTTON_TOOLTIP_STRING);
		}
	}
	UI_Object::reloadStrings();
}

void DataBaseEntry::reloadOriginalSize()
{
	UI_Object::reloadOriginalSize();
	setOriginalSize(Size(getParent()->getWidth(), getHeight()));
	entry->setOriginalSize(Size(getWidth() - 40 - (entryType==DATA_RACE_ENTRY?15:entryType==DATA_GOAL_ENTRY?30:45), 12));
	checkButton->setOriginalPosition(Point(getWidth() - 24 , 0));
}

void DataBaseEntry::check(const bool do_check)
{
	if((!do_check) && (checkButton->isCurrentlyActivated()))
		checkButton->forceUnpress();
	else if((do_check) && (!checkButton->isCurrentlyActivated()))
		checkButton->forcePress();
}

void DataBaseEntry::process()
{
	UI_Object::process();
//	if(checkButton->checkForNeedRedraw()) TODO
//		setNeedRedrawMoved();
}

void DataBaseEntry::draw() const
{
	UI_Object::draw();
}
