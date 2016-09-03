#include "forceentry.hpp"

#include "../ui/window.hpp"
#include "../core/configuration.hpp"
#include <sstream>

ForceEntry::ForceEntry(UI_Object* entry_parent, const Rect entry_rect, const std::string& entry_unit):
	UI_Button(entry_parent, entry_rect, Size(0,0), entry_unit, FORCE_ENTRY_BUTTON, PRESS_BUTTON_MODE, DO_NOT_ADJUST, SMALL_NORMAL_BOLD_FONT, AUTO_HEIGHT_FULL_WIDTH),
	timeEntryBox(new UI_NumberField(this, Rect(entry_rect.GetWidth() - 210, -14, 0, 0), Size(10,10), DO_NOT_ADJUST, 0, coreConfiguration.getMaxTime(), 6, 0, NULL_STRING, NULL_STRING, TIME_NUMBER_TYPE)),
//	makeLocationGoal(new UI_Button(this, Rect(entry_rect.GetTopLeft() + Point(entry_rect.GetWidth() - 60, -11), Size(10,10)), entry_max_rect, GOAL_LOCATION_BUTTON, STATIC_BUTTON_MODE, ARRANGE_RIGHT)),
	makeTimeGoal(new UI_Button(this, Rect(entry_rect.GetTopLeft() + Point(entry_rect.GetWidth() - 29, -11), Size(16,10)), Size(0,0), GOAL_TIME_BUTTON, STATIC_BUTTON_MODE, ARRANGE_RIGHT)),

	startForce(0),
	targetForce(0),
	currentForce(0),
	totalNumber(1),
	unit(0),
	type(REMAINING_UNIT_TYPE),
	goal(NULL),
	showLocMenu(false)
{
//	makeLocationGoal->Hide();
	makeTimeGoal->Hide();
	makeTimeGoal->updateToolTip(FORCEENTRY_TIME_TOOLTIP_STRING);
	timeEntryBox->Hide();
}	

ForceEntry::ForceEntry(const ForceEntry& object) :
	UI_Button((UI_Button)object),
	timeEntryBox(new UI_NumberField(*object.timeEntryBox)),
//	makeLocationGoal(new UI_Button(*object.makeLocationGoal)),
	makeTimeGoal(new UI_Button(*object.makeTimeGoal)),
	startForce(object.startForce),
	targetForce(object.targetForce),
	currentForce(object.currentForce),
	totalNumber(object.totalNumber),
	unit(object.unit),
	type(object.type),
	goal(object.goal),
	showLocMenu(object.showLocMenu)
{ }


ForceEntry& ForceEntry::operator=(const ForceEntry& object)
{
	((UI_Button)(*this)) = ((UI_Button)object);
	timeEntryBox = new UI_NumberField(*object.timeEntryBox);
//	makeLocationGoal = new UI_Button(*object.makeLocationGoal);
	makeTimeGoal = new UI_Button(*object.makeTimeGoal);

	startForce = object.startForce;
	targetForce = object.targetForce;
	currentForce = object.currentForce;
	totalNumber = object.totalNumber;
	unit = object.unit;
	type = object.type;
	goal = object.goal;

	showLocMenu = object.showLocMenu;

	return(*this);
}
																																						   
ForceEntry::~ForceEntry()
{
	delete makeTimeGoal;
//	delete makeLocationGoal;
	delete timeEntryBox;
}

UI_Object* ForceEntry::checkTooltip()
{
	UI_Object* t = makeTimeGoal->checkTooltip();
	if(t == NULL)
	{
	//	t = makeLocationGoal->checkTooltip();
//		if(t == NULL)
//		{
			t = timeEntryBox->checkTooltip();
			if(t == NULL)
				return(UI_Button::checkTooltip());
			else return(t);
//		}
//		else return(t);
	}
	else return(t);
}

UI_Object* ForceEntry::checkHighlight()
{
	UI_Object* t = UI_Object::checkHighlight();
	if(t == NULL)
	{
		t = makeTimeGoal->checkHighlight();
		if(t == NULL)
		{
//			t = makeLocationGoal->checkHighlight();
//			if(t == NULL)
//			{
				t = timeEntryBox->checkHighlight();
				if(t == NULL)
					return(UI_Button::checkHighlight());
				else return(t);
//			}
//			else return(t);
		}
		else return(t);
	} else return(t);
}

const bool ForceEntry::isLocationGoalClicked() const
{
	return(showLocMenu);
//	makeLocationGoal->isLeftClicked());
}

void ForceEntry::process()
{
	if(!isShown())
		return;
	Size::mv(currentForce, startForce, targetForce);
	showLocMenu=false;
	if(makeTimeGoal->isLeftClicked())
	{
		timeEntryBox->Show(!timeEntryBox->isShown());
//		makeLocationGoal->forceUnpress();
//		showLocMenu=false;
	} else
/*	if(makeLocationGoal->isLeftClicked())
	{
		timeEntryBox->Hide();
		makeTimeGoal->forceUnpress();
		showLocMenu=true;
	} else*/
	if(!goal)
	{
//		makeLocationGoal->Hide();
		makeTimeGoal->Hide();
//		buttonPlacementArea.SetWidth(((UI_Window*)getParent())->getClientRectWidth()); TODO
	}
	else
	{
		if(goal->getTime() != timeEntryBox->getNumber())
		{
			goal->setTime(timeEntryBox->getNumber());
			changed = GOAL_TIME_HAS_CHANGED;
			forceEntryUnit = getUnit();
			forceEntryLocation = goal->getLocation();
			forceEntryTime = goal->getTime();
			forceEntryIsGoal = true;
		}
		
		Rect r;
		r.SetTopLeft(getAbsolutePosition());
//		r.SetHeight(buttonPlacementArea.GetHeight()); TODO
		r.SetWidth(((UI_Window*)getParent())->getClientRectWidth());
		if(r.Inside(mouse))
		{
			if(timeEntryBox->isShown())
				r.SetWidth(((UI_Window*)getParent())->getClientRectWidth()-100);
			else
				r.SetWidth(((UI_Window*)getParent())->getClientRectWidth()-30);
//			makeLocationGoal->Show();
			makeTimeGoal->Show();
			if(r.GetWidth() < getWidth())
				setNeedRedrawMoved();
//			setWidth(r.GetWidth());
		}
		else
		if(!((UI_Window*)getParent())->getAbsoluteRect().Inside(mouse))
		{
//			makeLocationGoal->forceUnpress();
			makeTimeGoal->forceUnpress();
//			makeLocationGoal->Hide();
			makeTimeGoal->Hide();
			timeEntryBox->Hide();
//			buttonPlacementArea.SetWidth(r.GetWidth());
//			setWidth(r.GetWidth());
		} else //if(!makeTimeGoal->isCurrentlyPressed())
		{
//			makeLocationGoal->forceUnpress();
			makeTimeGoal->forceUnpress();
//			makeLocationGoal->Hide();
			makeTimeGoal->Hide();
			timeEntryBox->Hide();
//			buttonPlacementArea.SetWidth(r.GetWidth());
//			setWidth(r.GetWidth());
		} 
	}
	UI_Button::process();
	if(isLeftClicked())
	{
		changed = LEFT_CLICKED;
		forceEntryUnit = getUnit();
		if(goal)
		{
			forceEntryLocation = goal->getLocation();
			forceEntryTime = goal->getTime();
			forceEntryIsGoal = true;
		} else
		{
			forceEntryLocation = 0;
			forceEntryTime = 0;
			forceEntryIsGoal = false;
		}
	
	}
	if(isRightClicked())
	{
		changed = RIGHT_CLICKED;
		forceEntryUnit = getUnit();
		if(goal)
		{
			forceEntryLocation = goal->getLocation();
			forceEntryTime = goal->getTime();
			forceEntryIsGoal = true;
		} else
		{
			forceEntryLocation = 0;
			forceEntryTime = 0;
			forceEntryIsGoal = false;
		}	
	}
	if(timeEntryBox->checkForNeedRedraw())
		setNeedRedrawMoved();
}
	
																																							
void ForceEntry::setTargetForce(const unsigned int force)
{
	if(targetForce != force)
	{
		startForce = currentForce;
		targetForce = force;
		setNeedRedrawNotMoved();
	}
}

void ForceEntry::draw(DC* dc) const
{
	if(!isShown()) 
		return;
	UI_Button::draw(dc);
	if(!checkForNeedRedraw())
		return;

	dc->SetBrush(*theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+getType())));
	dc->SetPen(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN+getType())));
	dc->DrawEdgedRoundedRectangle(Rect(getAbsolutePosition()+Point(getWidth()-currentForce-1, 1), Size(currentForce+1, FONT_SIZE+4)), 3);

	std::ostringstream os;

// ------ Numbers
	
	if((goal)&&(totalNumber >= goal->getCount()))
	{
		dc->SetTextForeground(dc->mixColor(theme.lookUpColor(BRIGHT_TEXT_COLOR), theme.lookUpColor(FULFILLED_TEXT_COLOR)));
		if(totalNumber > goal->getCount()) 
			os << "+" << totalNumber - goal->getCount() << "    ";
	}
	else if(goal)
		dc->SetTextForeground(dc->mixColor(theme.lookUpColor(BRIGHT_TEXT_COLOR), theme.lookUpColor(NOT_FULFILLED_TEXT_COLOR)));
	else 
		dc->SetTextForeground(dc->mixColor(theme.lookUpColor(BRIGHT_TEXT_COLOR), theme.lookUpColor(FULFILLED_TEXT_COLOR)));

	if(goal)
		os << totalNumber << "/" << goal->getCount();
	else
		os << totalNumber;
		
	//dc->SetFont(UI_Object::theme.lookUpFont(SMALL_ITALICS_BOLD_FONT));
	Size s = dc->GetTextExtent(os.str());
	dc->DrawText(os.str(), getAbsolutePosition() + Point(getWidth() - s.GetWidth() - 2, 4));
  
// ------ Time
	if((goal)&&(goal->getTime()>0)) // nur fuer tatsaechliche goals
	{
		os.str("");
		if ((goal->getFinalTime() > goal->getTime()) && (totalNumber >= goal->getCount()))
		{
			dc->SetTextForeground(dc->mixColor(theme.lookUpColor(BRIGHT_TEXT_COLOR), theme.lookUpColor(NOT_FULFILLED_TEXT_COLOR)));
			os << formatTime(goal->getFinalTime()) << " / " << formatTime(goal->getTime());
		}
		else if(totalNumber >= goal->getCount())
		{
			dc->SetTextForeground(dc->mixColor(theme.lookUpColor(BRIGHT_TEXT_COLOR), theme.lookUpColor(FULFILLED_TEXT_COLOR)));
			os << "+" << formatTime(goal->getTime() - goal->getFinalTime()) << " / " << formatTime(goal->getTime());
		} else
		{
			dc->SetTextForeground(dc->mixColor(theme.lookUpColor(BRIGHT_TEXT_COLOR), theme.lookUpColor(NOT_FULFILLED_TEXT_COLOR)));
			os << "[" << formatTime(goal->getTime()) << "]";
		}
		s = dc->GetTextExtent(os.str());
		dc->DrawText(os.str(), getAbsolutePosition() + Point(getWidth() - s.GetWidth() - 100, 2));
	}
}
																																							
void ForceEntry::assignGoal(GOAL* assign_goal)
{
	goal = assign_goal;
	if(goal)
		timeEntryBox->updateNumber(goal->getTime());
	
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
//		setNeedRedraw();
	}
}

eForceEntryMessage ForceEntry::changed = NO_MESSAGE;
bool ForceEntry::forceEntryIsGoal = false;
unsigned int ForceEntry::forceEntryUnit = 0;
unsigned int ForceEntry::forceEntryLocation = 0;
unsigned int ForceEntry::forceEntryTime = 0;

