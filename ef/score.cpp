#include "score.hpp"
#include <sstream>
#include "configuration.hpp"

ScoreWindow::ScoreWindow(UI_Object* score_parent, const unsigned int game_number, const unsigned int game_max) :
	UI_Window(score_parent, TIMER_WINDOW_TITLE_STRING, theme.lookUpGameRect(SCORE_WINDOW, game_number, game_max), theme.lookUpGameMaxHeight(SCORE_WINDOW, game_number, game_max), NOT_SCROLLED),
	currentPlayer(0),
	players(0),
	maxPlayer(0),
	unchangedGenerations(0),
//	mapMenuButton(new UI_Button(this, Rect(Point(10, 20), Size(100, 50)), Size(5,5), MY_BUTTON, false, STATIC_BUTTON_MODE, database.getMap(0)->getName(), ARRANGE_TOP_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
//	mapMenu(new MapMenu(mapMenuButton, Rect(10, 10, 100, 0), Size(0, 0), DO_NOT_ADJUST)),
	assignMap(-1),
	gameNumber(0),
	gameMax(0)
{
/*TODO
- Map initialisieren ?
- Harvest speed initialisieren
*/
	for(unsigned int i = MAX_PLAYER;i--;)
	{
		player[i] = new PlayerEntry(this, Rect(), Size(5,5));
		player[i]->setNumber(i+1);
		player[i]->Hide();
	}
//		goalsFulfilledText[i] = new UI_StaticText(this, getRelativeClientRect() + Point(100, i * 20), IMPORTANT_COLOR, SMALL_BOLD_FONT, NO_TEXT_MODE); TODO => tooltip
//	goalsFulfilledText->updateToolTip(GOALS_FULFILLED_TOOLTIP_STRING);
	resetData(); // TODO
	addHelpButton(DESCRIPTION_SCORE_WINDOW_CHAPTER);
}

ScoreWindow::~ScoreWindow()
{
	for(unsigned int i = MAX_PLAYER;i--;)
		delete player[i];
//	delete mapMenuButton;
//	delete mapMenu;
}

void ScoreWindow::setMode(const unsigned int game_number, const unsigned int game_max)
{
	if((game_number == gameNumber) && (game_max == gameMax))
		return;
	gameNumber = game_number;
	gameMax = game_max;
}

void ScoreWindow::reloadOriginalSize()
{
	setOriginalRect(theme.lookUpGameRect(SCORE_WINDOW, gameNumber, gameMax));
	setMaxHeight(theme.lookUpGameMaxHeight(SCORE_WINDOW, gameNumber, gameMax));
	
	for(unsigned int i = MAX_PLAYER;i--;)
		player[i]->setOriginalSize(Size(getRelativeClientRect().getWidth()-10, 12));
	UI_Window::reloadOriginalSize();
}


void ScoreWindow::setUnchangedGenerations(const unsigned int unchanged_generations) 
{
	if(unchangedGenerations == unchanged_generations)
		return;
	if((efConfiguration.isAutoRuns())&&(unchangedGenerations < unchanged_generations))
		setNeedRedrawNotMoved();
	unchangedGenerations = unchanged_generations;
}

void ScoreWindow::resetPlayerTime(unsigned int player_number)
{
#ifdef _SCC_DEBUG
	if(player_number>=MAX_PLAYER)	{
		toErrorLog("DEBUG: (ScoreWindow::resetPlayerTime): player_number out of range.");return;
	}
#endif
	player[player_number]->resetTime();
}

void ScoreWindow::resetData()
{
}

void ScoreWindow::closeMenus()
{
//	mapMenu->close();
}

void ScoreWindow::stopOptimizing(const unsigned int player_number)
{
#ifdef _SCC_DEBUG
	if(player_number>=MAX_PLAYER)	{
		toErrorLog("DEBUG: (ScoreWindow::stopOptimizing): player_number out of range.");return;
	}
#endif
	player[player_number]->setOptimizing(false);
}

void ScoreWindow::startOptimizing(const unsigned int player_number)
{
#ifdef _SCC_DEBUG
	if(player_number>=MAX_PLAYER)	{
		toErrorLog("DEBUG: (ScoreWindow::startOptimizing): player_number out of range.");return;
	}
#endif
	player[player_number]->setOptimizing(true);
}

void ScoreWindow::process()
{
	if(!isShown()) 
		return;

	UI_Window::process();

	assignMap=-1;
/*	if(pressed = etc. mapMenu->getPressedItem()>=0)
	{
		assignMap = mapMenu->getPressedItem();
		mapMenuButton->updateText(NULL_STRING);
		mapMenuButton->updateText(database.getMap(assignMap)->getName());
		mapMenuButton->forceUnpress();
	}

	if(mapMenuButton->isLeftClicked())
	{
		mapMenu->open();
		if(!mapMenu->isOpen())
		{
			mapMenuButton->forceUnpress();
			mapMenu->close();
		} else
		{
			mapMenu->close();
			mapMenu->open();
		}
	}*/

	
        if(!isMouseInside())
	{
	//	mapMenuButton->forceUnpress();
		closeMenus();
		for(unsigned int i=MAX_PLAYER;i--;)
			if(player[i]->isShown())
				player[i]->mouseHasLeft();
	}
							
        unsigned int line = 1;
//        if(mapMenu->getHeight() > line)
//              line += mapMenu->getHeight();
	for(unsigned int i=MAX_PLAYER;i--;)
	{
		if(i>=maxPlayer)
			player[i]->Hide();
		else 
		{
			player[i]->Show();
			player[i]->adjustRelativeRect(Rect(getRelativeClientRectLeftBound()+5, line*(2*FONT_SIZE+10), getRelativeClientRect().getWidth()-10, 12)); // TODO
			line+=player[i]->getLineHeight(); // height of menu <-
			if(player[i]->checkForNeedRedraw())
				setNeedRedrawNotMoved();
		}
	}
	
// Alle Player durchlaufen, evtl Hoehe anpassen: 
// TODO
	fitItemToRelativeClientRect(Rect(Point(0, (2*FONT_SIZE+10)*(line-1)), Size(100,12)), true);
}


void ScoreWindow::draw(DC* dc) const
{
	if(!isShown()) 
		return;
	UI_Window::draw(dc);
	if(!checkForNeedRedraw())
		return;
	
	if(efConfiguration.isAutoRuns())
	{
		dc->setPen(*UI_Object::theme.lookUpPen(BODIAGRAM_FITNESS_PEN));
		dc->DrawHorizontalLine(getAbsoluteLeftBound() + 10, getAbsoluteLowerBound() - 6, getAbsoluteLeftBound() + 10 + ((getWidth()-35)*unchangedGenerations)  / efConfiguration.getMaxGenerations() );
		std::ostringstream os;
		os.str("");
		os << 100 * unchangedGenerations / efConfiguration.getMaxGenerations() << "% (" << unchangedGenerations << "/" << efConfiguration.getMaxGenerations() << ")";
	
		dc->setTextForeground(*UI_Object::theme.lookUpColor(BRIGHT_TEXT_COLOR));
		dc->setFont(UI_Object::theme.lookUpFont(SMALL_BOLD_FONT));
		dc->DrawText(os.str(), getAbsolutePosition() + Size(getWidth() - 55, getHeight() - 12));
	}
}

const bool ScoreWindow::openMenu(const ePlayerOrder order)
{
	unsigned int i = currentPlayer;
	do
	{
		i++;
		if(i >= MAX_PLAYER)
			i = 0;
		if( (player[i]) && (player[i]->isShown()) && ( ((order == OPEN_RACE_MENU)&&(player[i]->openRaceMenu()))/* || ((order == ADD_PLAYER)&&(player[i]->addPlayer()))*/ ) )
		{
			currentPlayer = i;
			return(true);
		}
	} while(i!=currentPlayer);
	return(false);
}
