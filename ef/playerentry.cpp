#include "playerentry.hpp"

PlayerEntry::PlayerEntry(UI_Object* player_parent, const Rect rect, const Size distance_bottom_right) :
	UI_Object(player_parent, rect, distance_bottom_right),
	currentScore(MAX_TIME),
	programScore(MAX_TIME),
	goalComplete(0),
	initMode(INACTIVE),
	scoreMode(SCORE_FULFILL_MODE),

	currentActionButton(new UI_Button(this, Rect(Point(0,0), Size(70, 12)), Size(5,0), CLICK_TO_CONTINUE_STRING, MY_BUTTON, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, NO_AUTO_SIZE)),
	scoreText(new UI_StaticText(this, Rect(Point(170, 0), rect.GetSize()), Size(5,0), IMPORTANT_COLOR, SMALL_NORMAL_BOLD_FONT)),
	playerText(new UI_StaticText(this, "Player 1:", Rect(Point(5, 0), rect.GetSize()), Size(0, 0), IMPORTANT_COLOR, SMALL_NORMAL_BOLD_FONT, DO_NOT_ADJUST)),
	addPlayerButton(new UI_Button(this, Rect(Point(0,0), Size(60, 12)), Size(5,0), ADD_PLAYER_STRING, MY_BUTTON, PRESS_BUTTON_MODE, BOTTOM_CENTER, SMALL_NORMAL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	removePlayerButton(new UI_Button(this, Rect(Point(205,2), Size(8,8)), Size(5,0), CANCEL_BUTTON, PRESS_BUTTON_MODE)), // Evtl bitmap
	optimizing(false),
	menuRadio(new UI_Radio(this, Rect(Point(120, 0), Size(0,0)))),
	raceMenu(new RaceMenu(this, Rect(120, 0, getWidth()-100, 0))),
	assignRace(-1)
{
/* TODO:
- Startposition
- Startbedingungen
 * */
	
	//menuButton[RACE_MENU] = new UI_Button(this, Rect(Point(0, 0), Size(0,0)), Size(0,0), CHOOSE_RACE_STRING, MY_BUTTON, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, NO_AUTO_SIZE);
	menuButton[RACE_MENU] = new UI_Button(this, Rect(Point(0, 0), Size(0,0)), Size(0,0), CHOOSE_RACE_STRING, TAB_BUTTON, TAB_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
//	tab[TUTORIAL_TAB] = new UI_Button(NULL, Rect(10, 1, theme.lookUpButtonWidth(TAB_BUTTON), 10), Size(20, 0), TUTORIAL_TAB_STRING, TAB_BUTTON, TAB_BUTTON_MODE, ARRANGE_TOP_RIGHT, MIDDLE_NORMAL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
	raceMenu->Hide();
	for(unsigned int i=0; i<MAX_PLAYER_ENTRY_MENUS; i++)
		menuRadio->addButton(menuButton[i], i);
	menuRadio->calculateSameWidthOfButtons(true);
			
	
	currentActionButton->Hide();
	scoreText->Hide();
	playerText->Hide();
	addPlayerButton->Show();
	removePlayerButton->Hide();
	menuButton[RACE_MENU]->Hide();
	menuRadio->Hide();
	for(unsigned int i=20;i--;)
	{
		oldScoreCounter[i] = 0;
		oldScore[i] = MAX_TIME-1;
	}
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
        for(unsigned int i = 0; i < MAX_PLAYER_ENTRY_MENUS;i++)
		delete menuButton[i];
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
//		menuButton[RACE_MENU]->Show();
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
			currentScore -= (currentScore - programScore)/2;
		    	if(programScore<currentScore)
				currentScore--;
		}
		std::ostringstream os;
		if(currentScore >= 3600) // TODO ga->...
			os << "[--:--]";
		else
			os << "[" << formatTime(currentScore) << "]";
		scoreText->updateText(os.str());	
	}
	else if(initMode == INACTIVE)
	{
		playerText->Hide();
		scoreText->Hide();
		currentActionButton->Hide();
		menuButton[RACE_MENU]->Hide();
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
		menuButton[RACE_MENU]->Show();
		menuRadio->Show();
	}
	if(removePlayerButton->isLeftClicked())
	{
		setInitMode(INACTIVE);
		addPlayerButton->Show();
		removePlayerButton->Hide();
		playerText->Hide();
		scoreText->Hide();
		currentActionButton->Hide();
		menuButton[RACE_MENU]->updateText(CHOOSE_RACE_STRING);
		menuButton[RACE_MENU]->Hide();
		menuRadio->Hide();
	}

	assignRace=-1;
	if(raceMenu->getPressedItem()>=0)
	{	
		assignRace=raceMenu->getPressedItem();
		menuButton[RACE_MENU]->updateText((eString)(TERRA_STRING+assignRace));
		menuRadio->forceUnpressAll();
		menuButton[RACE_MENU]->forceUnpress();
		currentActionButton->Show();
	}

	if(menuRadio->buttonHasChanged())
        {
                switch(menuRadio->getMarked())
                {
                        case RACE_MENU:
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


