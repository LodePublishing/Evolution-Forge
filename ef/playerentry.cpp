#include "playerentry.hpp"
#include "../ui/window.hpp"

PlayerEntry::PlayerEntry(UI_Object* player_parent, const Rect rect, const Size distance_bottom_right) :
	UI_Object(player_parent, rect, distance_bottom_right),
	currentScore(MAX_TIME),
	programScore(MAX_TIME),
	goalComplete(0),
	initMode(INACTIVE),
	scoreMode(SCORE_FULFILL_MODE),
// TODO UI_Object:: arrange top left :(
// 
	currentActionButton(new UI_Button(this, Rect(Point(0, 0), Size(UI_Object::theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH)*9/10, 0)), Size(0,0), CLICK_TO_CONTINUE_STRING, MY_BUTTON, STATIC_BUTTON_MODE, DO_NOT_ADJUST, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	playerText(new UI_StaticText(this, "Player 1:", Rect(Point(UI_Object::theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(0,0)), Size(0, 0), IMPORTANT_COLOR, SMALL_BOLD_FONT, DO_NOT_ADJUST)),
	menuRadio(new UI_Radio(this, Rect(Point(playerText->getRelativeRect().GetLeft()+playerText->getTextSize().GetWidth()+5, 0), Size(0, 0)), Size(0,0), DO_NOT_ADJUST)), // TODO
	raceMenuButton(new UI_Button(menuRadio, Rect(Point(0, 0), Size(UI_Object::theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH)/2, 0)), Size(0, 0), CHOOSE_RACE_STRING, TAB_BUTTON, STATIC_BUTTON_MODE, TOP_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	raceMenu(new RaceMenu(raceMenuButton, Rect(10, 15, 0, 0), Size(0,0), TOP_LEFT )),

	removePlayerButton(new UI_Button(this, Rect(Point(getParent()->getWidth()-20, 1), Size(8, 8)), Size(5, 0), CANCEL_BUTTON, PRESS_BUTTON_MODE, DO_NOT_ADJUST)), // Evtl bitmap
	scoreText(new UI_StaticText(this, Rect(Point(getParent()->getWidth()-50, 0), Size(0, 0)), Size(5, 0), IMPORTANT_COLOR, SMALL_BOLD_FONT, DO_NOT_ADJUST)),
	addPlayerButton(new UI_Button(this, Rect(Point(5, 0), Size(60, 0)), Size(5, 0), ADD_PLAYER_STRING, MY_BUTTON, PRESS_BUTTON_MODE, TOP_CENTER, SMALL_BOLD_FONT, AUTO_SIZE)),
	optimizing(false),
	assignRace(-1)
{
/* TODO:
- Startposition
- Startbedingungen
 * */
	menuRadio->addButton(raceMenuButton, 0);
	menuRadio->calculateBoxSize(true);
	
	raceMenu->Hide();
	currentActionButton->Hide();
	scoreText->Hide();
	playerText->Hide();
	addPlayerButton->Show();
	removePlayerButton->Hide();
	raceMenuButton->Hide();
	menuRadio->Hide();
	for(unsigned int i=20;i--;)
	{
		oldScoreCounter[i] = 0;
		oldScore[i] = MAX_TIME-1;
	}
}

void PlayerEntry::reloadOriginalSize()
{
	playerText->setOriginalPosition(Point(UI_Object::theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0));
	currentActionButton->setOriginalSize(Size(UI_Object::theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH)*9/10, 0));
	raceMenuButton->setOriginalSize(Size(UI_Object::theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH)/2, 0));
	menuRadio->setOriginalPosition(Point(playerText->getRelativeRect().GetLeft()+playerText->getTextSize().GetWidth()+5, 0));

	removePlayerButton->setOriginalPosition(Point(getParent()->getWidth()-20, 1));
	scoreText->setOriginalPosition(Point(getParent()->getWidth()-50, 0));
	UI_Object::reloadOriginalSize();
}

const unsigned int PlayerEntry::getLineHeight() const
{
	return(1+raceMenu->getHeight());
}

#include <sstream>

void PlayerEntry::setNumber(const unsigned int player_number)
{
	number = player_number;
	std::ostringstream os;
	os.str("");
	os << "Player " << number << ":";
	playerText->updateText(os.str());
}

PlayerEntry::~PlayerEntry()
{
	delete currentActionButton;
	delete scoreText;
	delete playerText;
	delete addPlayerButton;
	delete removePlayerButton;
	delete raceMenuButton;
	delete menuRadio;
	delete raceMenu;
}

void PlayerEntry::closeMenus()
{
	raceMenu->close();
}

void PlayerEntry::mouseHasLeft()
{
	menuRadio->forceUnpressAll();
	raceMenuButton->forceUnpress();	
	closeMenus();
}

	

void PlayerEntry::resetData()
{
	for(unsigned int i=20;i--;)
	{
		oldScoreCounter[i] = 0;
		oldScore[i] = MAX_TIME-1;
	}
	currentScore = MAX_TIME;
	programScore = MAX_TIME;
	goalComplete = 0;
	initMode = INACTIVE;
	scoreMode = SCORE_FULFILL_MODE;
}

void PlayerEntry::setScore(const unsigned int score)
{
	programScore = score;	
	setNeedRedrawMoved();
}

void PlayerEntry::setGoalComplete(const unsigned int goal_complete)
{
	goalComplete = goal_complete;
	setNeedRedrawMoved();
}

void PlayerEntry::setOptimizing(const bool opt)
{
	if(!opt)
		currentActionButton->forcePress();
	else currentActionButton->forceUnpress();
	optimizing = opt;
	setNeedRedrawMoved();
}

void PlayerEntry::setInitMode(const eInitMode init_mode)
{
	initMode = init_mode;
	setNeedRedrawMoved();
}

void PlayerEntry::setScoreMode(const eScoreMode score_mode)
{
	scoreMode = score_mode;
	setNeedRedrawMoved();
}

const eScoreMode PlayerEntry::getScoreMode() const
{
	return(scoreMode);
}

const eInitMode PlayerEntry::getInitMode() const
{
	return(initMode);
}


const bool PlayerEntry::isOptimizing() const
{
	if(initMode!=INITIALIZED)
		return(false);
	else return(optimizing);
}


void PlayerEntry::process()
{
	if(!isShown())
		return;
	UI_Object::process();

	if(initMode!=INACTIVE)
	{
		if(currentActionButton->isLeftClicked())
		{
			if(currentActionButton->isCurrentlyActivated())
			{
				optimizing=true;
//				currentActionButton->updateText(CLICK_TO_PAUSE_STRING);
				currentActionButton->updateToolTip(PAUSE_OPTIMIZATION_TOOLTIP_STRING);
			}
			else
			{
				optimizing=false;
//				currentActionButton->updateText(CLICK_TO_CONTINUE_STRING);
				currentActionButton->updateToolTip(CONTINUE_OPTIMIZATION_TOOLTIP_STRING);
			}
		}
//		currentActionButton->Show();
//		scoreText->Show();
//		playerText->Show();
//		addPlayerButton->Hide();
//		removePlayerButton->Show();
//		raceMenuButton->Show();
//		goalsFulfilledText->Show();
		if(scoreMode==SCORE_FULFILL_MODE)
		{																	
//			goalsFulfilledText[i]->updateText(theme.lookUpFormattedString(OF_GOALS_FULFILLED_STRING, goalComplete/100));
			if(!isOptimizing()) 
				currentActionButton->updateText(PAUSED_STRING);
			else
				currentActionButton->updateText(SEARCHING_STRING);
		}
		else if(scoreMode==SCORE_TIME_MODE)
		{
//			goalsFulfilledText->updateText(theme.lookUpFormattedString(OF_TIME_FULFILLED_STRING, goalComplete));
			if(!isOptimizing())
				currentActionButton->updateText(PAUSED_STRING);
			else
				currentActionButton->updateText(OPTIMIZING_STRING);
		} 
		// else see below
/*	else if(surrentMode==SCORE_MODE
	{
		goalsFulfilledText->updateText(theme.lookUpFormattedString(RES_UNITS_STRUCT_STRING, 0,0,0));
		if(!anarace->isOptimizing())
			currentActionText->updateText(PAUSED_STRING);
		else
			currentActionText->updateText(OPTIMIZING_STRING);
		scoreText->updateText(theme.lookUpFormattedString(TOTAL_STRING, 123));
	}*/
	
		if(programScore!=currentScore)
		{
			currentScore -= ((signed int)currentScore - (signed int)programScore)/2;
		    	if(programScore<currentScore)
				--currentScore;
			else if(programScore>currentScore)
				++currentScore;
			std::ostringstream os;
			if(currentScore >= 3600) // TODO ga->...
				os << "[--:--]";
			else
				os << "[" << formatTime(currentScore) << "]";
			scoreText->updateText(os.str());
		}
	}
	else if(initMode == INACTIVE)
	{
		playerText->Hide();
		scoreText->Hide();
		currentActionButton->Hide();
		raceMenuButton->Hide();
		menuRadio->Hide();
//		goalsFulfilledText->Hide();

	}
	if(addPlayerButton->isLeftClicked())
	{
		setInitMode(NOT_INITIALIZED);
		addPlayerButton->Hide();
		removePlayerButton->Show();
		playerText->Show();
		scoreText->Show();
		currentActionButton->Hide();
		raceMenuButton->Show();
		menuRadio->Show();
		reloadOriginalSize();
	}
	if(removePlayerButton->isLeftClicked())
	{
		setInitMode(INACTIVE);
		addPlayerButton->Show();
		removePlayerButton->Hide();
		playerText->Hide();
		scoreText->Hide();
		currentActionButton->Hide();
		raceMenuButton->updateText(CHOOSE_RACE_STRING);
		raceMenuButton->Hide();
		menuRadio->Hide();
	}

	assignRace=-1;
	if(raceMenu->getPressedItem()>=0)
	{	
		assignRace=raceMenu->getPressedItem();
		raceMenuButton->updateText((eString)(TERRA_STRING+assignRace));
		menuRadio->forceUnpressAll();
		raceMenuButton->forceUnpress();
		currentActionButton->Show();
	}

	if(menuRadio->buttonHasChanged())
	{
		switch(menuRadio->getMarked())
		{
			case 0:
				raceMenu->open();
				if(!raceMenu->isOpen())
				{
					menuRadio->forceUnpressAll();
					closeMenus();
				} else
				{
					closeMenus();
					raceMenu->open();
				}
			break;
			default:break;
		}
	}
}

void PlayerEntry::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Object::draw(dc);
}


const signed int PlayerEntry::getAssignedRace() const
{
	return(assignRace);
}


