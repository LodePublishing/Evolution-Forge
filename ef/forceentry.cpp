#include "forceentry.hpp"

#include "../ui/window.hpp"
#include "../core/configuration.hpp"
#include "configuration.hpp"
#include <sstream>

ForceEntry::ForceEntry(UI_Object* entry_parent, const Rect entry_rect, const std::string& entry_unit, const unsigned int unit_num, const eUnitType unit_type, GOAL* unit_goal):
	UI_Button(entry_parent, entry_rect, Size(0,0), FORCE_ENTRY_BUTTON, NULL_BITMAP, PRESS_BUTTON_MODE, entry_unit, SPECIAL_BUTTON_LEFT, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_FULL_WIDTH),
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
	if(ForceEntry::currentForceEntry == this)
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
		if(alwaysBuildButton)
		{
			if(UI_Button::getCurrentButton() == alwaysBuildButton)
				UI_Button::resetButton();
			delete alwaysBuildButton;
		}
		if(buildMuchButton)
		{
			if(UI_Button::getCurrentButton() == buildMuchButton)
				UI_Button::resetButton();
			delete buildMuchButton;
		}
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
		makePufferInvalid();
	} else
	if(targetForce!=currentForce)
		makePufferInvalid();
	Size::mv(currentForce, startForce, targetForce);
	
	UI_Button::process();

	if(goal!=NULL)
	{
		// wichtig: Speichern des forceentries damit nicht KOnkurrenz zwischen den entries betsteht!
		Rect rect_right = getAbsoluteRect();
		rect_right.setWidth(40);
		rect_right.setLeft(getParent()->getAbsoluteRightBound()-40);

// situation 1: mouse just entered, both timeentrybox and maketimegoal is hidden => Show makeTimeGoalButton

		if(!ForceEntry::timeEntryBox)
		{
			if(ForceEntry::currentForceEntry == NULL)
			{
				// just entered the right_side of the entry => show time button
				if((rect_right.isTopLeftCornerInside(mouse)) && (ForceEntry::makeTimeGoalButton==NULL))
				{
					setOriginalSize(Size(getParent()->getWidth()-60, getHeight()));
					ForceEntry::makeTimeGoalButton = new UI_Button(getParent(), Rect(getRelativePosition() + Size(getParent()->getWidth()-52, 0), Size(16,10)), Size(0,0), GOAL_TIME_BUTTON, TIME_BUTTON_BITMAP, STATIC_BUTTON_MODE, NULL_STRING, DO_NOT_ADJUST);
					ForceEntry::alwaysBuildButton = new UI_Button(getParent(), Rect(getRelativePosition() + Size(getParent()->getWidth()-34, 0), Size(10,10)), Size(0,0), CHECK_BUTTON, RADIO_OFF, CHECK_BUTTON_MODE, NULL_STRING, DO_NOT_ADJUST);
					ForceEntry::buildMuchButton = new UI_Button(getParent(), Rect(getRelativePosition() + Size(getParent()->getWidth()-22, 0), Size(10,10)), Size(0,0), CHECK_BUTTON, RADIO_OFF, CHECK_BUTTON_MODE, NULL_STRING, DO_NOT_ADJUST);
					if(goal->getIsAlwaysBuild())
						ForceEntry::alwaysBuildButton->forcePress();
					if(goal->getIsBuildAsMuchAsPossible())
						ForceEntry::buildMuchButton->forcePress();


					ForceEntry::makeTimeGoalButton->updateToolTip(FORCEENTRY_TIME_TOOLTIP_STRING);
					ForceEntry::alwaysBuildButton->updateToolTip(FORCEENTRY_ALWAYS_BUILD_TOOLTIP_STRING);
					ForceEntry::buildMuchButton->updateToolTip(FORCEENTRY_BUILD_MUCH_TOOLTIP_STRING);
					ForceEntry::currentForceEntry = this;
					UI_Button::resetButton();
					UI_Button::setCurrentButton(ForceEntry::makeTimeGoalButton);
				}
			} else
			// left the right side
			if(ForceEntry::currentForceEntry == this)
			{
			
				if(!rect_right.isTopLeftCornerInside(mouse))
				{
					if((ForceEntry::makeTimeGoalButton)&&(!ForceEntry::makeTimeGoalButton->isCurrentlyActivated()))
					{
// situation 2: mouse is outside and timeEntryBox is not shown => Remove makeTimeGoalButton
			// EVTL PROBLEM: Ich loesche es hier, es ist aber Kind vom Parent das gerade noch proces durchlaeuft... evtl an parent benachrichtigen, dass button geloescht werden soll... :o
						if((UI_Button::getCurrentButton() == ForceEntry::makeTimeGoalButton) || (UI_Button::getCurrentButton() == ForceEntry::alwaysBuildButton) || (UI_Button::getCurrentButton() == ForceEntry::buildMuchButton))
							UI_Button::resetButton();
						delete ForceEntry::makeTimeGoalButton;
						delete ForceEntry::alwaysBuildButton;
						delete ForceEntry::buildMuchButton;
						ForceEntry::alwaysBuildButton = NULL;
						ForceEntry::buildMuchButton = NULL;
						ForceEntry::makeTimeGoalButton = NULL;

						setOriginalSize(Size(getParent()->getWidth()-10, getHeight())); //?
						ForceEntry::currentForceEntry = NULL; //?
					}
// situation 3: nothing is pressed, mouse is outside => return to normal width
					else if((ForceEntry::makeTimeGoalButton == NULL) && (ForceEntry::alwaysBuildButton == NULL))
					{
						setOriginalSize(Size(getParent()->getWidth()-10, getHeight()));
						ForceEntry::currentForceEntry = NULL;
					}
				}

/*				if((!rect_left.isTopLeftCornerInside(mouse)) && (ForceEntry::alwaysBuildButton!=NULL))
				{
					setOriginalPosition(rect_left.getTopLeft());
					setOriginalSize(Size(getParent()->getWidth()-10, getHeight()));
				}*/
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
	} // end 'goal != NULL

	if(ForceEntry::currentForceEntry == this)
	{
/*		Rect r = getAbsoluteRect();
		r.setWidth(getParent()->getWidth());
		if(!r.isTopLeftCornerInside(mouse))
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
				setOriginalSize(Size(getParent()->getWidth()-60, getHeight()));
			}
			else
			{
				ForceEntry::timeEntryBox = new UI_NumberField(getParent(), Rect(Point(getRelativePosition() + Size(getParent()->getWidth() - UI_Object::theme.lookUpButtonWidth(LARGE_BUTTON_WIDTH)-40, 0)), Size(0, 0)), Size(0,0), DO_NOT_ADJUST, 0, coreConfiguration.getMaxTime(), NULL_STRING, NULL_STRING, 150, getTime(), false, TIME_NUMBER_TYPE);
				setOriginalSize(Size(getParent()->getWidth() - UI_Object::theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH)-40, getHeight()));
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
		if((ForceEntry::alwaysBuildButton!=NULL)&&(ForceEntry::alwaysBuildButton->isLeftClicked()))
		{
			if(ForceEntry::alwaysBuildButton->isCurrentlyActivated())
			{
				goal->setIsAlwaysBuild(true);			
				if(ForceEntry::buildMuchButton->isCurrentlyActivated())
				{
					ForceEntry::buildMuchButton->forceUnpress();
					goal->setIsBuildAsMuchAsPossible(false);
				}
			} else goal->setIsAlwaysBuild(false);
			ForceEntry::changed = GOAL_ALWAYS_BUILD_CHANGED;
//			// time?
		}
		if((ForceEntry::buildMuchButton!=NULL)&&(ForceEntry::buildMuchButton->isLeftClicked()))
		{
			if(ForceEntry::buildMuchButton->isCurrentlyActivated())
			{
				goal->setIsBuildAsMuchAsPossible(true);			
				if(ForceEntry::alwaysBuildButton->isCurrentlyActivated())
				{
					ForceEntry::alwaysBuildButton->forceUnpress();
					goal->setIsAlwaysBuild(false);
				}
			} else goal->setIsBuildAsMuchAsPossible(false);
			ForceEntry::changed = GOAL_BUILD_MUCH_CHANGED;
//			// time?
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
		makePufferInvalid();
		highlight = 50;
		resetGradient();
	}
}

void ForceEntry::draw() const
{
	UI_Button::draw();
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

	dc->DrawEdgedRoundedRectangle(Rect(Point(getWidth()-currentForce-1, 1), Size(currentForce+1, FONT_SIZE+4)), 3);

	std::ostringstream os;
	os.str("");

// ------ Numbers
	if(goal)
	{
		if(goal->getIsAlwaysBuild())
			os << "*";
		else if(goal->getIsBuildAsMuchAsPossible())
			os << "++";
		else
		if(totalNumber > goal->getCount())
				os << "+" << totalNumber - goal->getCount() << "    ";
	
		if(totalNumber >= goal->getCount())
			dc->setTextForeground(dc->mixColor(*theme.lookUpColor(BRIGHT_TEXT_COLOR), *theme.lookUpColor(FULFILLED_TEXT_COLOR)));
		else if(goal)
			dc->setTextForeground(dc->mixColor(*theme.lookUpColor(BRIGHT_TEXT_COLOR), *theme.lookUpColor(NOT_FULFILLED_TEXT_COLOR)));
		else 
			dc->setTextForeground(dc->mixColor(*theme.lookUpColor(BRIGHT_TEXT_COLOR), *theme.lookUpColor(FULFILLED_TEXT_COLOR)));
	}

	if(goal)
		os << totalNumber << " / " << goal->getCount();
	else
		os << totalNumber;
		
	//dc->setFont(UI_Object::theme.lookUpFont(SMALL_BOLD_FONT));
	Size s = dc->getTextExtent(os.str());
	dc->DrawText(os.str(), Point(getWidth() - s.getWidth() - 2, 1+(FONT_SIZE+12-s.getHeight())/2));
  
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
		dc->DrawText(os.str(), Point(getWidth() - s.getWidth() - getParent()->getWidth()/3, 1+(FONT_SIZE+12-s.getHeight())/2 ));
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

void ForceEntry::mouseHasLeftWindow()
{
	if(ForceEntry::currentForceEntry!=NULL)
	{
		if((UI_Button::getCurrentButton() == ForceEntry::makeTimeGoalButton) || (UI_Button::getCurrentButton() == ForceEntry::alwaysBuildButton) || (UI_Button::getCurrentButton() == ForceEntry::buildMuchButton))
			UI_Button::resetButton();
		delete ForceEntry::makeTimeGoalButton;
		delete ForceEntry::alwaysBuildButton;
		delete ForceEntry::buildMuchButton;
		delete ForceEntry::timeEntryBox;
		ForceEntry::alwaysBuildButton = NULL;
		ForceEntry::buildMuchButton = NULL;
		ForceEntry::makeTimeGoalButton = NULL;
		ForceEntry::timeEntryBox = NULL;
		ForceEntry::currentForceEntry->setOriginalSize(Size(ForceEntry::currentForceEntry->getParent()->getWidth()-10, ForceEntry::currentForceEntry->getHeight())); //?
		ForceEntry::currentForceEntry = NULL; //?
		
	}
// situation 3: nothing is pressed, mouse is outside => return to normal width
}

bool ForceEntry::doCompleteSound = false;
eForceEntryMessage ForceEntry::changed = NO_MESSAGE;
bool ForceEntry::forceEntryIsGoal = false;
unsigned int ForceEntry::forceEntryUnit = 0;
unsigned int ForceEntry::forceEntryLocation = 0;
unsigned int ForceEntry::forceEntryTime = 0;
signed int ForceEntry::forceEntryCount = 0;

UI_Button* ForceEntry::alwaysBuildButton = NULL;
UI_Button* ForceEntry::buildMuchButton = NULL;
UI_Button* ForceEntry::makeTimeGoalButton = NULL;
UI_NumberField* ForceEntry::timeEntryBox = NULL;
ForceEntry* ForceEntry::currentForceEntry = NULL;
