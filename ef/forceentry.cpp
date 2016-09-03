#include "forceentry.hpp"

#include "../ui/window.hpp"
#include "../core/configuration.hpp"
#include <sstream>

ForceEntry::ForceEntry(UI_Object* entry_parent, const Rect entry_rect, const std::string& entry_unit):
	UI_Button(entry_parent, entry_rect, Size(0,0), FORCE_ENTRY_BUTTON, false, PRESS_BUTTON_MODE, entry_unit, SPECIAL_BUTTON_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_FULL_WIDTH),
	timeEntryBox(new UI_NumberField(this, Rect(entry_rect.GetWidth() - 210, -14, 0, 0), Size(10,10), DO_NOT_ADJUST, 0, coreConfiguration.getMaxTime(), NULL_STRING, NULL_STRING, 6, 0, TIME_NUMBER_TYPE)),
//	makeLocationGoal(new UI_Button(this, Rect(entry_rect.GetTopLeft() + Point(entry_rect.GetWidth() - 60, -11), Size(10,10)), entry_max_rect, GOAL_LOCATION_BUTTON, STATIC_BUTTON_MODE, ARRANGE_RIGHT)),
	makeTimeGoal(new UI_Button(this, Rect(Point(0, 0), Size(16,10)), Size(0,0), GOAL_TIME_BUTTON, true, STATIC_BUTTON_MODE, NULL_STRING, ARRANGE_RIGHT)),

	oldGoalCount(0),	
	startForce(0),
	targetForce(0),
	currentForce(0),
	totalNumber(1),
	highlight(0),
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
	oldGoalCount(object.oldGoalCount),	
	startForce(object.startForce),
	targetForce(object.targetForce),
	currentForce(object.currentForce),
	totalNumber(object.totalNumber),
	highlight(object.highlight),
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
	oldGoalCount = object.oldGoalCount;
	startForce = object.startForce;
	targetForce = object.targetForce;
	currentForce = object.currentForce;
	totalNumber = object.totalNumber;
	highlight = object.highlight;
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

void ForceEntry::reloadOriginalSize()
{
	timeEntryBox->setOriginalPosition(Point(getWidth() - 210, -14));
	UI_Object::reloadOriginalSize();
}

UI_Object* ForceEntry::checkToolTip()
{
	UI_Object* t = makeTimeGoal->checkToolTip();
	if(t == NULL)
	{
	//	t = makeLocationGoal->checkToolTip();
//		if(t == NULL)
//		{
			t = timeEntryBox->checkToolTip();
			if(t == NULL)
				return(UI_Button::checkToolTip());
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
	if(highlight>0)
	{
		highlight--;
		highlight=highlight*5/6;
		setNeedRedrawNotMoved();
	} else
	if(targetForce!=currentForce)
		setNeedRedrawNotMoved();
	Size::mv(currentForce, startForce, targetForce);
	showLocMenu = false;
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
			ForceEntry::forceEntryUnit = getUnit();
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
		ForceEntry::forceEntryUnit = getUnit();
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
		ForceEntry::forceEntryUnit = getUnit();
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
		highlight = 50;
	}
}

eForceEntryMessage ForceEntry::changed = NO_MESSAGE;
bool ForceEntry::forceEntryIsGoal = false;
unsigned int ForceEntry::forceEntryUnit = 0;
unsigned int ForceEntry::forceEntryLocation = 0;
unsigned int ForceEntry::forceEntryTime = 0;

