#include "score.hpp"

#include <sstream>
#include <iomanip>

ScoreWindow::ScoreWindow(UI_Object* score_parent) :
	UI_Window(score_parent, TIMER_WINDOW_TITLE_STRING, TIMER_WINDOW, NOT_SCROLLED),
// TODO irgendwas stimmt hier mit der Hoehe nicht
	continueButton(new UI_Button(this, getRelativeClientRect(), CLICK_TO_CONTINUE_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, BOTTOM_CENTER, SMALL_NORMAL_BOLD_FONT, AUTO_HEIGHT_FULL_WIDTH)),
//	resetButton(new UI_Button(this, Rect(getRelativeClientRectPosition(), getClientRectSize()), RESET_BUILD_ORDER_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, PRESS_BUTTON_MODE, CENTER_RIGHT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE))
	optimizing(false),
	players(0)
{
	for(unsigned int i = MAX_PLAYER;i--;)
	{
//		goalsFulfilledText[i] = new UI_StaticText(this, getRelativeClientRect() + Point(100, i * 20), IMPORTANT_COLOR, SMALL_NORMAL_BOLD_FONT, NO_TEXT_MODE); TODO => tooltip
		currentActionText[i] = new UI_StaticText(this, getRelativeClientRect() + Point(50, i * 20), IMPORTANT_COLOR, SMALL_NORMAL_BOLD_FONT, NO_TEXT_MODE);
		scoreText[i] = new UI_StaticText(this, getRelativeClientRect() + Point(120, i * 20), IMPORTANT_COLOR, SMALL_NORMAL_BOLD_FONT, NO_TEXT_MODE);
		playerText[i] = new UI_StaticText(this, "Player 1:", getRelativeClientRect() + Point(0, i * 20), IMPORTANT_COLOR, SMALL_NORMAL_BOLD_FONT, NO_TEXT_MODE);
		playerText[i]->Hide();
		mode[i]=SCORE_FULFILL_MODE;
		goalComplete[i]=0;
	}
//      resetButton->updateToolTip(RESET_BUILD_ORDER_TOOLTIP_STRING);
	continueButton->updateToolTip(CONTINUE_OPTIMIZATION_TOOLTIP_STRING);
//	goalsFulfilledText->updateToolTip(GOALS_FULFILLED_TOOLTIP_STRING);
	resetData(); // TODO
}

ScoreWindow::~ScoreWindow()
{
	for(unsigned int i = MAX_PLAYER;i--;)
	{
//		delete goalsFulfilledText[i];
		delete currentActionText[i];
		delete scoreText[i];
	}
	delete continueButton;
//	delete resetButton;
}

void ScoreWindow::setScore(const unsigned int player, const unsigned int score) {
	programScore[player] = score;
}

void ScoreWindow::setGoalComplete(const unsigned int player, const unsigned int goal) {
	goalComplete[player] = goal;
}

void ScoreWindow::setPlayers(const unsigned int player_count) {
	players = player_count;
}

void ScoreWindow::resetData()
{
	for(unsigned int j = MAX_PLAYER;j--;)
	{
		for(unsigned int i=20;i--;)
		{
			oldScoreCounter[j][i] = 0;
			oldScore[j][i] = MAX_TIME-1;
		}
		currentScore[j] = MAX_TIME;
		programScore[j] = MAX_TIME;
		goalComplete[j] = 0;
		mode[j] = SCORE_FULFILL_MODE;
	}
}

void ScoreWindow::setOptimizing(const bool opt)
{
	if(!opt)
		continueButton->forcePress();
	else continueButton->forceUnpress();
	optimizing = opt;
}

void ScoreWindow::process()
{
	if(!isShown()) 
		return;

	UI_Window::process();
	if(continueButton->isLeftClicked())
	{
		if(continueButton->isCurrentlyActivated())
		{
			optimizing=true;
			continueButton->updateText(CLICK_TO_PAUSE_STRING);
			continueButton->updateToolTip(PAUSE_OPTIMIZATION_TOOLTIP_STRING);
		}
		else
		{
			optimizing=false;
			continueButton->updateText(CLICK_TO_CONTINUE_STRING);
			continueButton->updateToolTip(CONTINUE_OPTIMIZATION_TOOLTIP_STRING);
		}
	}

// Alle Player durchlaufen, evtl Hoehe anpassen: 

	for(unsigned int i = players;i--;)
	{
		playerText[i]->Show();
		fitItemToRelativeClientRect(Rect(0,0,10,10)/*playerText[i]->getAbsoluteRect()*/,2);
		scoreText[i]->Show();
		currentActionText[i]->Show();
//		goalsFulfilledText[i]->Show();
		if(getCurrentMode(i)==SCORE_FULFILL_MODE)
		{																	
//			goalsFulfilledText[i]->updateText(theme.lookUpFormattedString(OF_GOALS_FULFILLED_STRING, goalComplete[i]/100));
			if(!isOptimizing()) 
				currentActionText[i]->updateText(PAUSED_STRING);
			else
				currentActionText[i]->updateText(SEARCHING_STRING);
		}
		else if(getCurrentMode(i)==SCORE_TIME_MODE)
		{
//			goalsFulfilledText[i]->updateText(theme.lookUpFormattedString(OF_TIME_FULFILLED_STRING, goalComplete[i]));
			if(!isOptimizing())
				currentActionText[i]->updateText(PAUSED_STRING);
			else
				currentActionText[i]->updateText(OPTIMIZING_STRING);
		} 
		// else see below
/*	else if(getCurrentMode()==SCORE_MODE
	{
		goalsFulfilledText->updateText(theme.lookUpFormattedString(RES_UNITS_STRUCT_STRING, 0,0,0));
		if(!anarace->isOptimizing())
			currentActionText->updateText(PAUSED_STRING);
		else
			currentActionText->updateText(OPTIMIZING_STRING);
		scoreText->updateText(theme.lookUpFormattedString(TOTAL_STRING, 123));
	}*/
	
		if(programScore[i]!=currentScore[i])
		{
			currentScore[i] -= (currentScore[i] - programScore[i])/2;
		    	if(programScore[i]<currentScore[i])
				currentScore[i]--;
		}
		std::ostringstream os;
		if(currentScore[i] >= 3600) // TODO ga->...
			os << "[--:--]";
		else
			os << "[" << formatTime(currentScore[i]) << "]";
		scoreText[i]->updateText(os.str());
	}
	for(unsigned int i = players;i<MAX_PLAYER;i++)
	{
		playerText[i]->Hide();
		scoreText[i]->Hide();
		currentActionText[i]->Hide();
//		goalsFulfilledText[i]->Hide();
	}

		
//        if(resetButton->isLeftClicked())
  //      {
    //            setResetFlag();
//		UI_Object::msgList.push_back("Resetted build order...");
  //      }
}

void ScoreWindow::setMode(const unsigned int player, eScoreMode current_mode) // for different tabs different behaviour
{
	mode[player] = current_mode;
}

const eScoreMode ScoreWindow::getCurrentMode(const unsigned int player) const
{
	return(mode[player]);
}

void ScoreWindow::draw(DC* dc) const
{
//	if(!isShown()) 
//		return;
	UI_Window::draw(dc);
	if(!checkForNeedRedraw())
		return;
		
/*	dc->SetPen(*UI_Object::theme.lookUpPen(BODIAGRAM_FITNESS_PEN));
	dc->DrawHorizontalLine(getAbsoluteLeftBound() + 10, getAbsoluteLowerBound() - 30, getAbsoluteLeftBound() + 10 + ((getWidth()-35)*game->getUnchangedGenerations())  / configuration.getMaxGenerations() );
	ostringstream os;
	os << 100*game->getUnchangedGenerations()  / configuration.getMaxGenerations() << "%";
	dc->DrawText(os.str(), getAbsolutePosition() + Size(getWidth() - 25, getHeight() - 30));*/
}

