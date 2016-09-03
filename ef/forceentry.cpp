#include "forceentry.hpp"

#include "../ui/window.hpp"
#include "../core/configuration.hpp"
#include "configuration.hpp"
#include <sstream>

ForceEntry::ForceEntry(UI_Object* entry_parent, const Rect entry_rect, const std::string& entry_unit, const unsigned int unit_num, const eUnitType unit_type, GOAL* unit_goal):
	UI_Button(entry_parent, entry_rect, Size(0,0), FORCE_ENTRY_BUTTON, false, PRESS_BUTTON_MODE, entry_unit, SPECIAL_BUTTON_LEFT, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_FULL_WIDTH),
	oldGoalCount(0),
	oldGoalTime(0),
	oldGoalFinalTime(0),
	startForce(0),
	targetForce(0),
	currentForce(0),
	totalNumber(1),
	highlight(0),
	unit(unit_num),
	type(unit_type),
	goal(unit_goal)
{
	//toErrorLog(entry_unit);
}	

ForceEntry::~ForceEntry()
{
	if(makeTimeGoalButton)
	{
		if(UI_Button::getCurrentButton() == makeTimeGoalButton)
			UI_Button::resetButton();
		delete makeTimeGoalButton;
		makeTimeGoalButton = NULL;
	}
	if(timeEntryBox)
	{
		delete timeEntryBox;
		timeEntryBox = NULL;
	}
}

void ForceEntry::process()
{
	if(!isShown())
		return;
	if(highlight>0)
	{
		highlight--;
		highlight=highlight*5/6;
		setNeedRedrawNotMoved();
	} else
	if(targetForce!=currentForce)
		setNeedRedrawNotMoved();
	Size::mv(currentForce, startForce, targetForce);
	
	UI_Button::process();

	if(goal!=NULL)
	{
		// wichtig: Speichern des forceentries damit nicht KOnkurrenz zwischen den entries betsteht!
		Rect r = getAbsoluteRect();
		r.setWidth(14);
		r.setLeft(getParent()->getAbsoluteRightBound()-18);
// situation 1: mouse just entered, both timeentrybox and maketimegoal is hidden => Show makeTimeGoalButton

		if(!ForceEntry::timeEntryBox)
		{
			if((ForceEntry::currentForceEntry == NULL) && (r.Inside(mouse)) && (ForceEntry::makeTimeGoalButton==NULL))
			{
				setOriginalSize(Size(getParent()->getWidth()-30, getHeight()));
				ForceEntry::makeTimeGoalButton = new UI_Button(getParent(), Rect(Point(getRelativePosition() + Size(getParent()->getWidth()-28, 0)), Size(16,10)), Size(0,0), GOAL_TIME_BUTTON, true, STATIC_BUTTON_MODE, NULL_STRING, DO_NOT_ADJUST);
				ForceEntry::makeTimeGoalButton->updateToolTip(FORCEENTRY_TIME_TOOLTIP_STRING);
				ForceEntry::currentForceEntry = this;
				UI_Button::resetButton();
				UI_Button::setCurrentButton(ForceEntry::makeTimeGoalButton);
			} else 
			if((ForceEntry::currentForceEntry == this) && (!r.Inside(mouse)))
			{
				if((ForceEntry::makeTimeGoalButton)&&(!ForceEntry::makeTimeGoalButton->isCurrentlyActivated()))
				{
// situation 2: mouse is outside and timeEntryBox is not shown => Remove makeTimeGoalButton
			// EVTL PROBLEM: Ich loesche es hier, es ist aber Kind vom Parent das gerade noch proces durchlaeuft... evtl an parent benachrichtigen, dass button geloescht werden soll... :o
					if(UI_Button::getCurrentButton() == ForceEntry::makeTimeGoalButton)
						UI_Button::resetButton();
					delete ForceEntry::makeTimeGoalButton;
					ForceEntry::makeTimeGoalButton = NULL;
				}
// situation 3: nothing is pressed, mouse is outside => return to normal width
				else if(ForceEntry::makeTimeGoalButton == NULL)
				{
					setOriginalSize(Size(getParent()->getWidth()-10, getHeight()));
					ForceEntry::currentForceEntry = NULL;
				}
			}
		}
		
		if(goal->getTime()>0)
		{
			if(totalNumber >= goal->getCount())
			{
				if((oldGoalFinalTime > oldGoalTime) && (goal->getFinalTime() <= goal->getTime())) 
				{
					toInitLog("k");
					ForceEntry::doCompleteSound = true;
				}
			}
			
			if(oldGoalFinalTime != goal->getFinalTime())
			{
				resetGradient();
				oldGoalFinalTime = goal->getFinalTime();
			}
		}

		if(oldGoalTime != goal->getTime())
			oldGoalTime = goal->getTime();

		if(oldGoalCount != goal->getCount())
		{
			resetGradient();
			oldGoalCount = goal->getCount();
		}
	}

	if(ForceEntry::currentForceEntry == this)
	{
/*		Rect r = getAbsoluteRect();
		r.setWidth(getParent()->getWidth());
		if(!r.Inside(mouse))
		{
			if(UI_Button::getCurrentButton() == ForceEntry::makeTimeGoalButton)
				UI_Button::resetButton();
			delete ForceEntry::makeTimeGoalButton;
			ForceEntry::makeTimeGoalButton = NULL;
			delete ForceEntry::timeEntryBox;	
			ForceEntry::timeEntryBox = NULL;
		}*/
	
		if((ForceEntry::makeTimeGoalButton)&&(ForceEntry::makeTimeGoalButton->isLeftClicked()))
		{
			if(ForceEntry::timeEntryBox)
			{
				delete ForceEntry::timeEntryBox;
				ForceEntry::timeEntryBox = NULL;
				setOriginalSize(Size(getParent()->getWidth()-30, getHeight()));
			}
			else
			{
				ForceEntry::timeEntryBox = new UI_NumberField(getParent(), Rect(Point(getRelativePosition() + Size(getParent()->getWidth() - UI_Object::theme.lookUpButtonWidth(LARGE_BUTTON_WIDTH)-20, 0)), Size(0, 0)), Size(0,0), DO_NOT_ADJUST, 0, coreConfiguration.getMaxTime(), NULL_STRING, NULL_STRING, 150, getTime(), false, TIME_NUMBER_TYPE);
				setOriginalSize(Size(getParent()->getWidth() - UI_Object::theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH)-20, getHeight()));
			}
			// evtl ohne time button? mmmh.. => ueberlegen
		}
		if((ForceEntry::timeEntryBox)&&(ForceEntry::timeEntryBox->hasNumberChanged()))
		{
			ForceEntry::changed = GOAL_TIME_HAS_CHANGED;
			ForceEntry::forceEntryIsGoal = true; // only goals can have time restrictions
			// time removed from goal -> try to remove the goal!
			if(getTime() && (!ForceEntry::timeEntryBox->getNumber()))
			{
				ForceEntry::forceEntryUnit = getUnit();
				ForceEntry::forceEntryLocation = goal->getLocation();
				ForceEntry::forceEntryTime = goal->getTime();
				ForceEntry::forceEntryCount = -goal->getCount();
			}
			else
			{
				setTime(ForceEntry::timeEntryBox->getNumber());
				ForceEntry::forceEntryUnit = 0;
				ForceEntry::forceEntryLocation = 0;
				ForceEntry::forceEntryTime = 0;
				ForceEntry::forceEntryCount = 0;
			}
		}
	}
	

	bool left_clicked = isLeftClicked();
	bool right_clicked = isRightClicked();
	if(left_clicked || ((right_clicked) && (goal!=NULL)))
	{
		ForceEntry::changed = NUMBER_HAS_CHANGED;
		ForceEntry::forceEntryUnit = getUnit();
		ForceEntry::forceEntryIsGoal = (goal!=NULL);
		ForceEntry::forceEntryLocation = goal?goal->getLocation():GLOBAL;
		ForceEntry::forceEntryTime = goal?goal->getTime():0;
		ForceEntry::forceEntryCount = left_clicked?1:-1;
	}
	
}

void ForceEntry::setTargetForce(const unsigned int force)
{
	if(targetForce != force)
	{
		startForce = currentForce;
		targetForce = force;
		setNeedRedrawNotMoved();
		highlight = 50;
		resetGradient();
	}
}

void ForceEntry::draw(DC* dc) const
{
	if(!isShown()) 
		return;
	if(!checkForNeedRedraw())
		return;
	UI_Button::draw(dc);
	Brush b = *theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+getType()));

	if(highlight>0)
		dc->setBrush(Brush(dc->changeAbsoluteBrightness(*b.getColor(), highlight), b.getStyle()));
	else
		dc->setBrush(b);
	Pen p = *theme.lookUpPen((ePen)(UNIT_TYPE_0_PEN+getType()));
	if(highlight>0)
		dc->setPen(Pen(dc->changeAbsoluteBrightness(*p.getColor(), highlight), p.getWidth(), p.getStyle()));
	else
		dc->setPen(p);

	dc->DrawEdgedRoundedRectangle(Rect(getAbsolutePosition()+Point(getWidth()-currentForce-1, 1), Size(currentForce+1, FONT_SIZE+4)), 3);

	std::ostringstream os;
	os.str("");

// ------ Numbers
	
	if((goal)&&(totalNumber >= goal->getCount()))
	{
		dc->setTextForeground(dc->mixColor(*theme.lookUpColor(BRIGHT_TEXT_COLOR), *theme.lookUpColor(FULFILLED_TEXT_COLOR)));
		if(totalNumber > goal->getCount()) 
			os << "+" << totalNumber - goal->getCount() << "    ";
	}
	else if(goal)
		dc->setTextForeground(dc->mixColor(*theme.lookUpColor(BRIGHT_TEXT_COLOR), *theme.lookUpColor(NOT_FULFILLED_TEXT_COLOR)));
	else 
		dc->setTextForeground(dc->mixColor(*theme.lookUpColor(BRIGHT_TEXT_COLOR), *theme.lookUpColor(FULFILLED_TEXT_COLOR)));

	if(goal)
		os << totalNumber << " / " << goal->getCount();
	else
		os << totalNumber;
		
	//dc->setFont(UI_Object::theme.lookUpFont(SMALL_BOLD_FONT));
	Size s = dc->getTextExtent(os.str());
	dc->DrawText(os.str(), getAbsolutePosition() + Point(getWidth() - s.getWidth() - 2, 1+(FONT_SIZE+12-s.getHeight())/2));
  
// ------ Time
	if((goal) && (goal->getTime()>0) && (ForceEntry::timeEntryBox==NULL)) // nur fuer tatsaechliche goals und nur wenn timeentrybox gerade nicht angezeigt wird
	{
		os.str("");
		if ((goal->getFinalTime() > goal->getTime()) && (totalNumber >= goal->getCount()))
		{
			dc->setTextForeground(dc->mixColor(*theme.lookUpColor(BRIGHT_TEXT_COLOR), *theme.lookUpColor(NOT_FULFILLED_TEXT_COLOR)));
			os << formatTime(goal->getFinalTime(), efConfiguration.getGameSpeed()) << " / " << formatTime(goal->getTime(), efConfiguration.getGameSpeed());
		}
		else if(totalNumber >= goal->getCount())
		{
			dc->setTextForeground(dc->mixColor(*theme.lookUpColor(BRIGHT_TEXT_COLOR), *theme.lookUpColor(FULFILLED_TEXT_COLOR)));
			os << "+" << formatTime(goal->getTime() - goal->getFinalTime(), efConfiguration.getGameSpeed()) << " / " << formatTime(goal->getTime(), efConfiguration.getGameSpeed());
		} else
		{
			dc->setTextForeground(dc->mixColor(*theme.lookUpColor(BRIGHT_TEXT_COLOR), *theme.lookUpColor(NOT_FULFILLED_TEXT_COLOR)));
			os << "[" << formatTime(goal->getTime(), efConfiguration.getGameSpeed()) << "]";
		}
		s = dc->getTextExtent(os.str());
		dc->DrawText(os.str(), getAbsolutePosition() + Point(getWidth() - s.getWidth() - getParent()->getWidth()/3, 1+(FONT_SIZE+12-s.getHeight())/2 ));
	}
}
																																							
void ForceEntry::setTotalNumber(const unsigned int total_number)
{
	if(totalNumber != total_number)
	{
		highlight = 50;
		if((goal)&&(totalNumber < goal->getCount())&&(total_number >= goal->getCount())&&((goal->getTime()==0)||((goal->getTime()>0)&&(goal->getFinalTime() <= goal->getTime()))))
			ForceEntry::doCompleteSound = true;
		totalNumber = total_number;
	}
}

bool ForceEntry::doCompleteSound = false;
eForceEntryMessage ForceEntry::changed = NO_MESSAGE;
bool ForceEntry::forceEntryIsGoal = false;
unsigned int ForceEntry::forceEntryUnit = 0;
unsigned int ForceEntry::forceEntryLocation = 0;
unsigned int ForceEntry::forceEntryTime = 0;
signed int ForceEntry::forceEntryCount = 0;

UI_Button* ForceEntry::makeTimeGoalButton = NULL;
UI_NumberField* ForceEntry::timeEntryBox = NULL;
ForceEntry* ForceEntry::currentForceEntry = NULL;
