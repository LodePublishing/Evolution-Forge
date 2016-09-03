#include "score.hpp"
#include <sstream>
#include <iomanip>

ScoreWindow::ScoreWindow(UI_Object* score_parent, const unsigned int game_number, const unsigned int game_max) :
	UI_Window(score_parent, TIMER_WINDOW_TITLE_STRING, theme.lookUpGameRect(SCORE_WINDOW, game_number, game_max), theme.lookUpGameMaxHeight(SCORE_WINDOW, game_number, game_max), NOT_SCROLLED),
// TODO irgendwas stimmt hier mit der Hoehe nicht
//	resetButton(new UI_Button(this, Rect(getRelativeClientRectPosition(), getClientRectSize()), RESET_BUILD_ORDER_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, PRESS_BUTTON_MODE, CENTER_RIGHT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE))
	players(0),
	maxPlayer(0),
	menuRadio(new UI_Radio(this, getRelativeClientRectPosition() + Point(0, 10))),
	mapMenu(new MapMenu(this, Rect(10, 10, getWidth()-100, 0))),
	assignMap(-1)
{

/*TODO

- Map initialisieren
- Harvest speed initialisieren

*/

	menuButton[MAP_MENU] = new UI_Button(this, Rect(Point(0, 0), getClientRectSize()), CHOOSE_MAP_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE_ONCE);
	mapMenu->Hide();
	for(unsigned int i=0; i<MAX_SCORE_MENUS; i++)
		menuRadio->addButton(menuButton[i], i);
	menuRadio->calculateSameWidthOfButtons(true);
		
	for(unsigned int i = MAX_PLAYER;i--;)
	{
		player[i] = new PlayerEntry(this, Rect(getRelativeClientRect().GetLeft(), 20+i*16, getRelativeClientRect().GetWidth(), 12));
		player[i]->setNumber(i+1);
	}
//		goalsFulfilledText[i] = new UI_StaticText(this, getRelativeClientRect() + Point(100, i * 20), IMPORTANT_COLOR, SMALL_NORMAL_BOLD_FONT, NO_TEXT_MODE); TODO => tooltip
//      resetButton->updateToolTip(RESET_BUILD_ORDER_TOOLTIP_STRING);
//	goalsFulfilledText->updateToolTip(GOALS_FULFILLED_TOOLTIP_STRING);
	resetData(); // TODO
}

ScoreWindow::~ScoreWindow()
{
	for(unsigned int i = MAX_PLAYER;i--;)
		delete player[i];
	for(unsigned int i = 0; i < MAX_SCORE_MENUS;i++)
		delete menuButton[i];
	delete menuRadio;
	delete mapMenu;
//	delete resetButton;
}

void ScoreWindow::setScore(const unsigned int player_number, const unsigned int score) 
{
#ifdef _SCC_DEBUG       
	if(player_number >= maxPlayer) {
		toLog("DEBUG: (ScoreWindow::setScore): Value player_number out of range.");return;
	}
#endif
	player[player_number]->setScore(score);
}

void ScoreWindow::setGoalComplete(const unsigned int player_number, const unsigned int goal) {
#ifdef _SCC_DEBUG       
	if(player_number >= maxPlayer) {
		toLog("DEBUG: (ScoreWindow::setGoalComplete): Value player_number out of range.");return;
	}
#endif
	player[player_number]->setGoalComplete(goal);
}

void ScoreWindow::setPlayers(const unsigned int player_count) {
#ifdef _SCC_DEBUG       
	if(player_count >= maxPlayer) {
		toLog("DEBUG: (ScoreWindow::setPlayers): Value player_count out of range.");return;
	}
#endif
	players = player_count;
}

const unsigned int ScoreWindow::getPlayers() const
{
	return(players);
}

void ScoreWindow::resetData()
{
// menuButton[MAP_MENU]->updateText(CHOOSE_MAP_STRING);
}

const signed int ScoreWindow::getAssignedMap() const
{
	return(assignMap);
}

const bool ScoreWindow::isOptimizing(const unsigned int player_number) const
{
#ifdef _SCC_DEBUG       
        if(player_number >= maxPlayer) {
                toLog("DEBUG: (ScoreWindow::isOptimizing): Value player_number out of range.");return(false);
        }
#endif
	return(player[player_number]->isOptimizing());
}

void ScoreWindow::setMaxPlayer(const unsigned int max_player)
{
#ifdef _SCC_DEBUG       
	if(max_player > MAX_PLAYER) {
		toLog("DEBUG: (ScoreWindow::setMaxPlayer): Value max_player out of range.");return;
	}
#endif
	maxPlayer = max_player;
}

void ScoreWindow::setScoreMode(const unsigned int player_number, eScoreMode score_mode) // for different tabs different behaviour
{
#ifdef _SCC_DEBUG       
	if(player_number >= maxPlayer) {
		toLog("DEBUG: (ScoreWindow::setScoreMode): Value player_number out of range.");return;
	}
#endif
	player[player_number]->setScoreMode(score_mode);
}

const eScoreMode ScoreWindow::getScoreMode(const unsigned int player_number) const
{
#ifdef _SCC_DEBUG       
	if(player_number >= maxPlayer) {
		toLog("DEBUG: (ScoreWindow::getScoreMode): Value player_number out of range.");return(SCORE_FULFILL_MODE);
	}
#endif
	return(player[player_number]->getScoreMode());
}


void ScoreWindow::setInitMode(const unsigned int player_number, eInitMode init_mode) // for different tabs different behaviour
{
#ifdef _SCC_DEBUG
	if(player_number >= maxPlayer) {
		toLog("DEBUG: (ScoreWindow::setInitMode): Value player_number out of range.");return;
	}
#endif
	player[player_number]->setInitMode(init_mode);
}

const eInitMode ScoreWindow::getInitMode(const unsigned int player_number) const
{
#ifdef _SCC_DEBUG       
	if(player_number >= maxPlayer) {
		toLog("DEBUG: (ScoreWindow::getInitMode): Value player_number out of range.");return(INACTIVE);
	}
#endif
	return(player[player_number]->getInitMode());
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
		menuButton[MAP_MENU]->updateText(database.getMap(assignMap)->getName());
		menuRadio->forceUnpressAll();
		menuButton[MAP_MENU]->forceUnpress();
	}
	
	if(menuRadio->buttonHasChanged())
		switch(menuRadio->getMarked())
		{
			case MAP_MENU:
                                mapMenu->open();
                                if(!mapMenu->isOpen())
                                {
                                        menuRadio->forceUnpressAll();
                                        closeMenus();
                                } else
                                {
                                        closeMenus();
                                        mapMenu->open();
                                }
			break;
			default:break;
		}

	
        if(!isMouseInside())
	{
		menuRadio->forceUnpressAll(); 
		closeMenus();
		for(unsigned int i=MAX_PLAYER;i--;)
			player[i]->mouseHasLeft();
	}
							
        unsigned int line = 3;
        if(mapMenu->getHeight() > line)
                line = mapMenu->getHeight();
	for(unsigned int i=MAX_PLAYER;i--;)
	{
		if(i>=maxPlayer)
			player[i]->Hide();
		else 
		{
			player[i]->Show();
			player[i]->adjustRelativeRect(Rect(getRelativeClientRect().GetLeft(), line*16, getRelativeClientRect().GetWidth(), 12));
			line+=player[i]->getLineHeight(); // height of menu <-
		}
	}
	
// Alle Player durchlaufen, evtl Hoehe anpassen: 

	fitItemToRelativeClientRect(Rect(0,16*line,10,12)/*playerText[i]->getAbsoluteRect()*/,2); // TODO

//        if(resetButton->isLeftClicked())
//      {
//            setResetFlag();
//		UI_Object::msgList.push_back("Resetted build order...");
//      }
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

const signed int ScoreWindow::getAssignedRace(const unsigned int player_number) const
{
#ifdef _SCC_DEBUG       
        if(player_number >= maxPlayer) {
                toLog("DEBUG: (ScoreWindow::getAssignedRace): Value player_number out of range.");return(-1);
        }
#endif
	return(player[player_number]->getAssignedRace());
}

