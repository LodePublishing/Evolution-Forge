#include "forceentry.hpp"

#include "../ui/window.hpp"
#include <sstream>

ForceEntry::ForceEntry(UI_Object* entry_parent, const Rect entry_rect, const Rect entry_max_rect, const string& entry_unit):
    UI_Button(entry_parent, Rect(entry_rect.GetTopLeft(), Size(entry_rect.GetWidth(), entry_rect.GetHeight())), entry_max_rect, entry_unit, entry_unit, FORCE_ENTRY_BUTTON, NO_TEXT_MODE, PRESS_BUTTON_MODE, DO_NOT_ADJUST, SMALL_NORMAL_BOLD_FONT, NO_AUTO_SIZE),
	timeEntryBox(new NumberField(this, Rect(entry_rect.GetTopLeft() + Point(entry_rect.GetWidth() - 40, -11), Size(10,10)), 0, TIME_NUMBER_TYPE)),
//	makeLocationGoal(new UI_Button(this, Rect(entry_rect.GetTopLeft() + Point(entry_rect.GetWidth() - 30, -11), Size(10,10)), entry_max_rect, GOAL_LOCATION_BUTTON, STATIC_BUTTON_MODE, ARRANGE_RIGHT)),
	makeTimeGoal(new UI_Button(this, Rect(entry_rect.GetTopLeft() + Point(entry_rect.GetWidth() - 29, -11), Size(16,10)), entry_max_rect, GOAL_TIME_BUTTON, STATIC_BUTTON_MODE, ARRANGE_RIGHT)),

    startForce(20),
    targetForce(20),
    currentForce(20),
    totalNumber(1),
	unit(0),
	type(REMAINING_UNIT_TYPE),
	goal(NULL)
{
//	makeLocationGoal->Hide();
	makeTimeGoal->Hide();
	timeEntryBox->Hide();
}	

ForceEntry::ForceEntry(const ForceEntry& object) :
    UI_Button((UI_Button)object),
	timeEntryBox(new NumberField(*object.timeEntryBox)),
//	makeLocationGoal(new UI_Button(*object.makeLocationGoal)),
	makeTimeGoal(new UI_Button(*object.makeTimeGoal)),
    startForce(object.startForce),
    targetForce(object.targetForce),
    currentForce(object.currentForce),
    totalNumber(object.totalNumber),
	unit(object.unit),
	type(object.type),
	goal(object.goal)
{ }


ForceEntry& ForceEntry::operator=(const ForceEntry& object)
{
	((UI_Button)(*this)) = ((UI_Button)object);
	timeEntryBox = new NumberField(*object.timeEntryBox);
//	makeLocationGoal = new UI_Button(*object.makeLocationGoal);
	makeTimeGoal = new UI_Button(*object.makeTimeGoal);

    startForce = object.startForce;
    targetForce = object.targetForce;
    currentForce = object.currentForce;
    totalNumber = object.totalNumber;
	unit = object.unit;
	type = object.type;
	goal = object.goal;

	return(*this);
}
                                                                                                                                                           
ForceEntry::~ForceEntry()
{
	delete makeTimeGoal;
//	delete makeLocationGoal;
	delete timeEntryBox;
}

UI_Object* ForceEntry::checkHighlight()
{
	UI_Object* t = makeTimeGoal->checkHighlight();
	if(t == NULL)
	{
//		t = makeLocationGoal->checkHighlight();
		if(t == NULL)
		{
			t = timeEntryBox->checkHighlight();
			if(t == NULL)
			    return(UI_Button::checkHighlight());
			else return(t);
		}
		else return(t);
	}
	else return(t);
}
                                                                                                                                                            
void ForceEntry::process()
{
    if(!isShown())
        return;
    Size::mv(currentForce, startForce, targetForce);
    UI_Button::process();

	if(timeEntryBox->subClicked())
	{
		if(goal->getTime() > 5)
			goal->setTime(goal->getTime()-5);
		else goal->setTime(0);
		timeEntryBox->updateNumber(goal->getTime());
	}
		
	if(timeEntryBox->addClicked())
	{
		if(goal->getTime() < MAX_TIME-5)//settings.getGa()->maxTime-5) TODO
			goal->setTime(goal->getTime()+5);
		else goal->setTime(MAX_TIME);//settings.getGa()->maxTime); TODO
		timeEntryBox->updateNumber(goal->getTime());
	}
	if(timeEntryBox->subRightClicked())
	{
		if(goal->getTime() > 60)
			goal->setTime(goal->getTime()-60);
		else goal->setTime(0);
		timeEntryBox->updateNumber(goal->getTime());
	}
		
	if(timeEntryBox->addRightClicked())
	{
		if(goal->getTime() < MAX_TIME-60)//settings.getGa()->maxTime-5) TODO
			goal->setTime(goal->getTime()+60);
		else goal->setTime(MAX_TIME);//settings.getGa()->maxTime); TODO
		timeEntryBox->updateNumber(goal->getTime());
	}
					
	if(makeTimeGoal->isLeftClicked())
	{
		timeEntryBox->Show(!timeEntryBox->isShown());
//		makeLocationGoal->forceUnpress();
		// locationMenu->Hide();
	} 
/*	if(makeLocationGoal->isLeftClicked())
	{
		timeEntryBox->Hide();
		makeTimeGoal->forceUnpress();
		// locationMenu->Show();
	} else */
	if(!goal)
	{
//		makeLocationGoal->Hide();
		makeTimeGoal->Hide();
		buttonPlacementArea.SetWidth(((UI_Window*)getParent())->getClientRectWidth());
	}
	else
	{
		Rect r;
		r.SetTopLeft(getAbsolutePosition());
		r.SetHeight(buttonPlacementArea.GetHeight());
		r.SetWidth(((UI_Window*)getParent())->getClientRectWidth());
		if(r.Inside(mouse))
		{
			if(timeEntryBox->isShown())
				r.SetWidth(((UI_Window*)getParent())->getClientRectWidth()-86);
			else
				r.SetWidth(((UI_Window*)getParent())->getClientRectWidth()-26);
//			makeLocationGoal->Show();
			makeTimeGoal->Show();
		}
		else
		{
//			makeLocationGoal->forceUnpress();
			makeTimeGoal->forceUnpress();
//			makeLocationGoal->Hide();
			makeTimeGoal->Hide();
			timeEntryBox->Hide();
		}
	
		buttonPlacementArea.SetWidth(r.GetWidth());
	}
}

void ForceEntry::setUnit(const unsigned int unit)
{
	this->unit = unit; // TODO
}

const unsigned int ForceEntry::getUnit() const
{
	return(unit);
}

const eUnitType ForceEntry::getType() const
{
	return(type);
}

void ForceEntry::setType(const eUnitType type)
{
	this->type = type;
}	
                                                                                                                                                            
void ForceEntry::setTargetForce(const unsigned int force)
{
    if(targetForce!=force)
    {
        startForce=currentForce;
        targetForce=force;
    }
}

void ForceEntry::draw(DC* dc) const
{
    if(!isShown()) return;
    UI_Button::draw(dc);

	dc->SetBrush(*theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+getType())));
    dc->SetPen(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN+getType())));
    dc->DrawRoundedRectangle(Rect(getAbsolutePosition()+Point(getWidth()-currentForce, 0), Size(currentForce+1,FONT_SIZE+5)), 3);

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
		
    Size s = dc->GetTextExtent(os.str());
    dc->DrawText(os.str(), getAbsolutePosition() + Point(getWidth() - s.GetWidth() - 2, 2));
  
// ------ Time
	if((goal)&&(goal->getTime()>0)) // nur fuer tatsaechliche goals
    {
        os.str("");
        if (goal->getFinalTime() > goal->getTime())
		{
            dc->SetTextForeground(dc->mixColor(theme.lookUpColor(BRIGHT_TEXT_COLOR), theme.lookUpColor(NOT_FULFILLED_TEXT_COLOR)));
			if(goal->getFinalTime()%60>9)
	            os << goal->getFinalTime()/60 << ":" << goal->getFinalTime()%60;
			else
				os << goal->getFinalTime()/60 << ":0" << goal->getFinalTime()%60;
		}
        else
		{
            dc->SetTextForeground(dc->mixColor(theme.lookUpColor(BRIGHT_TEXT_COLOR), theme.lookUpColor(FULFILLED_TEXT_COLOR)));
			if((goal->getTime() - goal->getFinalTime())%60>9)
	            os << "-" << (goal->getTime() - goal->getFinalTime())/60 << ":" << (goal->getTime() - goal->getFinalTime())%60;
			else
	            os << "-" << (goal->getTime() - goal->getFinalTime())/60 << ":0" << (goal->getTime() - goal->getFinalTime())%60;
		}
                                                                                                                                                            
        os << " / " << goal->getTime()/60 << ":" << goal->getTime()%60;
        Size s = dc->GetTextExtent(os.str());
        dc->DrawText(os.str(), getAbsolutePosition() + Point(getWidth() - s.GetWidth() - 100, 2));
                                                                                                                                                            
    }
}
                                                                                                                                                            
void ForceEntry::assignGoal(GOAL* goal)
{
	this->goal=goal;
}

void ForceEntry::HideIt()
{
    this->Hide();
}

void ForceEntry::updateText(const string& utext)
{
    updateNormalText(utext);
    updatePressedText(utext);
}
                                                                                                                                                            
void ForceEntry::setTotalNumber(const unsigned int totalNumber)
{
    this->totalNumber=totalNumber;
}
                                                                                                                                                            
