#include "score.hpp"
#include <sstream>
#include <iomanip>

ScoreWindow::ScoreWindow(UI_Object* score_parent, const unsigned int game_number, const unsigned int game_max) :
	UI_Window(score_parent, TIMER_WINDOW_TITLE_STRING, theme.lookUpGameRect(SCORE_WINDOW, game_number, game_max), theme.lookUpGameMaxHeight(SCORE_WINDOW, game_number, game_max), NOT_SCROLLED),
// TODO irgendwas stimmt hier mit der Hoehe nicht
	players(0),
	maxPlayer(0),
	mapMenuButton(new UI_Button(this, Rect(Point(10, 20), Size(100, 50)), Size(5,5), MY_BUTTON, false, STATIC_BUTTON_MODE, database.getMap(0)->getName(), ARRANGE_TOP_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	mapMenu(new MapMenu(mapMenuButton, Rect(10, 10, 100, 0), Size(0, 0), DO_NOT_ADJUST)),
	assignMap(-1),
	gameNumber(0),
	gameMax(0)
{
/*TODO

- Map initialisieren ?
- Harvest speed initialisieren

*/
	mapMenu->Hide();
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
		player[i]->setOriginalSize(Size(getRelativeClientRect().GetWidth()-10, 12));
	UI_Window::reloadOriginalSize();
}

ScoreWindow::~ScoreWindow()
{
	for(unsigned int i = MAX_PLAYER;i--;)
		delete player[i];
	delete mapMenuButton;
//	delete menuRadio;
	delete mapMenu;
}

void ScoreWindow::resetPlayerTime(unsigned int player_number)
{
#ifdef _SCC_DEBUG
	if(player_number>=MAX_PLAYER)	{
		toLog("DEBUG: (ScoreWindow::resetPlayerTime): player_number out of range.");return;
	}
#endif
	player[player_number]->resetTime();
}

void ScoreWindow::resetData()
{
//	menuButton[MAP_MENU]->updateText(CHOOSE_MAP_STRING);
}

void ScoreWindow::closeMenus()
{
	mapMenu->close();
}

void ScoreWindow::process()
{
	if(!isShown()) 
		return;

	UI_Window::process();

	assignMap=-1;
	if(mapMenu->getPressedItem()>=0)
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
	}

	
        if(!isMouseInside())
	{
		mapMenuButton->forceUnpress();
		closeMenus();
		for(unsigned int i=MAX_PLAYER;i--;)
			player[i]->mouseHasLeft();
	}
							
        unsigned int line = 3;
//        if(mapMenu->getHeight() > line)
                line += mapMenu->getHeight();
	for(unsigned int i=MAX_PLAYER;i--;)
	{
		if(i>=maxPlayer)
			player[i]->Hide();
		else 
		{
			player[i]->Show();
			player[i]->adjustRelativeRect(Rect(getRelativeClientRectLeftBound()+5, line*16, getRelativeClientRect().GetWidth()-10, 12));
			line+=player[i]->getLineHeight(); // height of menu <-
			if(player[i]->checkForNeedRedraw())
				setNeedRedrawNotMoved();
		}
	}
	
// Alle Player durchlaufen, evtl Hoehe anpassen: 

	fitItemToRelativeClientRect(Rect(0,16*(line-1),10,12)/*playerText[i]->getAbsoluteRect()*/,2); // TODO
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
	os.str("");
	os << 100*game->getUnchangedGenerations()  / configuration.getMaxGenerations() << "%";
	dc->DrawText(os.str(), getAbsolutePosition() + Size(getWidth() - 25, getHeight() - 30));*/
}


