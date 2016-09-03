#include "forceentry.hpp"

#include "../ui/window.hpp"
#include "../core/configuration.hpp"
#include <sstream>

ForceEntry::ForceEntry(UI_Object* entry_parent, const Rect entry_rect, const std::string& entry_unit):
	UI_Button(entry_parent, entry_rect, Size(0,0), FORCE_ENTRY_BUTTON, false, PRESS_BUTTON_MODE, entry_unit, SPECIAL_BUTTON_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_FULL_WIDTH),
	oldGoalCount(0),	
	startForce(0),
	targetForce(0),
	currentForce(0),
	totalNumber(1),
	highlight(0),
	unit(0),
	type(REMAINING_UNIT_TYPE),
	goal(NULL)
{}	

ForceEntry::ForceEntry(const ForceEntry& object) :
	UI_Button((UI_Button)object),
	oldGoalCount(object.oldGoalCount),	
	startForce(object.startForce),
	targetForce(object.targetForce),
	currentForce(object.currentForce),
	totalNumber(object.totalNumber),
	highlight(object.highlight),
	unit(object.unit),
	type(object.type),
	goal(object.goal)
{ }


ForceEntry& ForceEntry::operator=(const ForceEntry& object)
{
	((UI_Button)(*this)) = ((UI_Button)object);
	oldGoalCount = object.oldGoalCount;
	startForce = object.startForce;
	targetForce = object.targetForce;
	currentForce = object.currentForce;
	totalNumber = object.totalNumber;
	highlight = object.highlight;
	unit = object.unit;
	type = object.type;
	goal = object.goal;
	return(*this);
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

void ForceEntry::reloadOriginalSize()
{
	UI_Object::reloadOriginalSize();
}

UI_Object* ForceEntry::checkToolTip()
{
	return(UI_Button::checkToolTip());
}

UI_Object* ForceEntry::checkHighlight()
{
	return(UI_Button::checkHighlight());
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
		r.SetWidth(16);
		r.SetLeft(getParent()->getWidth()-10);
// situation 1: mouse just entered, both timeentrybox and maketimegoal is hidden => Show makeTimeGoalButton
		if((ForceEntry::currentForceEntry == NULL)&&(r.Inside(mouse))&&(!ForceEntry::makeTimeGoalButton)&&(!ForceEntry::timeEntryBox))
		{
			setOriginalSize(Size(getParent()->getWidth()-30, getHeight()));
			ForceEntry::makeTimeGoalButton = new UI_Button(getParent(), Rect(Point(getRelativePosition() + Size(getParent()->getWidth()-26, 0)), Size(16,10)), Size(0,0), GOAL_TIME_BUTTON, true, STATIC_BUTTON_MODE, NULL_STRING, DO_NOT_ADJUST);
			ForceEntry::makeTimeGoalButton->updateToolTip(FORCEENTRY_TIME_TOOLTIP_STRING);
			ForceEntry::currentForceEntry = this;
		} else 
		if((ForceEntry::currentForceEntry == this)&&(!r.Inside(mouse))&&(ForceEntry::makeTimeGoalButton)&&(!ForceEntry::timeEntryBox))
		{
// situation 2: mouse is outside and timeEntryBox is not shown => Remove makeTimeGoalButton
			// EVTL PROBLEM: Ich loesche es hier, es ist aber Kind vom Parent das gerade noch proces durchlaeuft... evtl an parent benachrichtigen, dass button geloescht werden soll... :o
			if(UI_Button::getCurrentButton() == ForceEntry::makeTimeGoalButton)
				UI_Button::resetButton();
			delete ForceEntry::makeTimeGoalButton;
			ForceEntry::makeTimeGoalButton = NULL;
		}
// situation 3: nothing is pressed, mouse is outside => return to normal width
		if((!r.Inside(mouse))&&(!ForceEntry::makeTimeGoalButton)&&(!ForceEntry::timeEntryBox))
		{
			setOriginalSize(Size(getParent()->getWidth()-10, getHeight()));
			ForceEntry::currentForceEntry = NULL;
		}
	}

	if(ForceEntry::currentForceEntry == this)
	{
		Rect r = getAbsoluteRect();
		r.SetWidth(getParent()->getWidth());
		if(!r.Inside(mouse))
		{
			if(UI_Button::getCurrentButton() == ForceEntry::makeTimeGoalButton)
				UI_Button::resetButton();
			delete ForceEntry::makeTimeGoalButton;
			ForceEntry::makeTimeGoalButton = NULL;
			delete ForceEntry::timeEntryBox;	
			ForceEntry::timeEntryBox = NULL;
			
		}
	
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
				ForceEntry::timeEntryBox = new UI_NumberField(getParent(), Rect(Point(getRelativePosition() + Size(getParent()->getWidth()-UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)-90, 0)), Size(0, 0)), Size(0,0), DO_NOT_ADJUST, 0, coreConfiguration.getMaxTime(), NULL_STRING, NULL_STRING, 6, getTime(), TIME_NUMBER_TYPE);
				setOriginalSize(Size(getParent()->getWidth()-110, getHeight()));
			}
		}
		if((ForceEntry::timeEntryBox)&&(ForceEntry::timeEntryBox->hasNumberChanged()))
		{
			if(getTime() && (!ForceEntry::timeEntryBox->getNumber()))
			{
				ForceEntry::forceEntryTimeRemoved = true;
				ForceEntry::forceEntryUnit = getUnit();
				ForceEntry::forceEntryLocation = goal->getLocation();
				ForceEntry::forceEntryTime = goal->getTime();
				ForceEntry::forceEntryCount = goal->getCount();
			}
			else
			{
				ForceEntry::forceEntryTimeRemoved = false;
				setTime(ForceEntry::timeEntryBox->getNumber());
			}
		}
	}
	

	bool left_clicked = isLeftClicked();
	bool right_clicked = isRightClicked();
	if(left_clicked || right_clicked)
	{
		if(left_clicked)
			changed = LEFT_CLICKED;
		else changed = RIGHT_CLICKED;
		ForceEntry::forceEntryUnit = getUnit();
		ForceEntry::forceEntryIsGoal = (goal!=NULL);
		ForceEntry::forceEntryLocation = goal->getLocation();
		ForceEntry::forceEntryTime = goal->getTime();
	}
	
	if((goal)&&(oldGoalCount != goal->getCount()))
	{
		resetGradient();
		oldGoalCount = goal->getCount();
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
		dc->SetBrush(Brush(dc->changeAbsoluteBrightness(*b.GetColor(), highlight), b.GetStyle()));
	else
		dc->SetBrush(b);
	Pen p = *theme.lookUpPen((ePen)(UNIT_TYPE_0_PEN+getType()));
	if(highlight>0)
		dc->SetPen(Pen(dc->changeAbsoluteBrightness(*p.GetColor(), highlight), p.GetWidth(), p.GetStyle()));
	else
		dc->SetPen(p);

	dc->DrawEdgedRoundedRectangle(Rect(getAbsolutePosition()+Point(getWidth()-currentForce-1, 1), Size(currentForce+1, FONT_SIZE+4)), 3);

	std::ostringstream os;
	os.str("");

// ------ Numbers
	
	if((goal)&&(totalNumber >= goal->getCount()))
	{
		dc->SetTextForeground(dc->mixColor(*theme.lookUpColor(BRIGHT_TEXT_COLOR), *theme.lookUpColor(FULFILLED_TEXT_COLOR)));
		if(totalNumber > goal->getCount()) 
			os << "+" << totalNumber - goal->getCount() << "    ";
	}
	else if(goal)
		dc->SetTextForeground(dc->mixColor(*theme.lookUpColor(BRIGHT_TEXT_COLOR), *theme.lookUpColor(NOT_FULFILLED_TEXT_COLOR)));
	else 
		dc->SetTextForeground(dc->mixColor(*theme.lookUpColor(BRIGHT_TEXT_COLOR), *theme.lookUpColor(FULFILLED_TEXT_COLOR)));

	if(goal)
		os << totalNumber << "/" << goal->getCount();
	else
		os << totalNumber;
		
	//dc->SetFont(UI_Object::theme.lookUpFont(SMALL_BOLD_FONT));
	Size s = dc->GetTextExtent(os.str());
	dc->DrawText(os.str(), getAbsolutePosition() + Point(getWidth() - s.GetWidth() - 2, 1+(FONT_SIZE+12-s.GetHeight())/2));
  
// ------ Time
	if((goal)&&(goal->getTime()>0)) // nur fuer tatsaechliche goals
	{
		os.str("");
		if ((goal->getFinalTime() > goal->getTime()) && (totalNumber >= goal->getCount()))
		{
			dc->SetTextForeground(dc->mixColor(*theme.lookUpColor(BRIGHT_TEXT_COLOR), *theme.lookUpColor(NOT_FULFILLED_TEXT_COLOR)));
			os << formatTime(goal->getFinalTime()) << " / " << formatTime(goal->getTime());
		}
		else if(totalNumber >= goal->getCount())
		{
			dc->SetTextForeground(dc->mixColor(*theme.lookUpColor(BRIGHT_TEXT_COLOR), *theme.lookUpColor(FULFILLED_TEXT_COLOR)));
			os << "+" << formatTime(goal->getTime() - goal->getFinalTime()) << " / " << formatTime(goal->getTime());
		} else
		{
			dc->SetTextForeground(dc->mixColor(*theme.lookUpColor(BRIGHT_TEXT_COLOR), *theme.lookUpColor(NOT_FULFILLED_TEXT_COLOR)));
			os << "[" << formatTime(goal->getTime()) << "]";
		}
		s = dc->GetTextExtent(os.str());
		dc->DrawText(os.str(), getAbsolutePosition() + Point(getWidth() - s.GetWidth() - 100, 1+(FONT_SIZE+12-s.GetHeight())/2 ));
	}
}
																																							
void ForceEntry::assignGoal(GOAL* assign_goal)
{
	goal = assign_goal;
}

void ForceEntry::HideIt()
{
	Hide();
}

void ForceEntry::setTotalNumber(const unsigned int total_number)
{
	if(totalNumber != total_number)
	{
		totalNumber = total_number;
		highlight = 50;
	}
}

eForceEntryMessage ForceEntry::changed = LEFT_CLICKED;
bool ForceEntry::forceEntryIsGoal = false;
bool ForceEntry::forceEntryTimeRemoved = false;
unsigned int ForceEntry::forceEntryUnit = 0;
unsigned int ForceEntry::forceEntryLocation = 0;
unsigned int ForceEntry::forceEntryTime = 0;
unsigned int ForceEntry::forceEntryCount = 0;

UI_Button* ForceEntry::makeTimeGoalButton = NULL;
UI_NumberField* ForceEntry::timeEntryBox = NULL;
ForceEntry* ForceEntry::currentForceEntry = NULL;
