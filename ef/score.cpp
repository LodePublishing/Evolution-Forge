#include "score.hpp"
#include <sstream>
#include <iomanip>

ScoreWindow::ScoreWindow(UI_Object* score_parent, const unsigned int game_number, const unsigned int game_max) :
	UI_Window(score_parent, TIMER_WINDOW_TITLE_STRING, theme.lookUpGameRect(SCORE_WINDOW, game_number, game_max), theme.lookUpGameMaxHeight(SCORE_WINDOW, game_number, game_max), NOT_SCROLLED),
// TODO irgendwas stimmt hier mit der Hoehe nicht
//	resetButton(new UI_Button(this, Rect(getRelativeClientRectPosition(), getClientRectSize()), RESET_BUILD_ORDER_STRING, MY_BUTTON, PRESS_BUTTON_MODE, CENTER_RIGHT, SMALL_BOLD_FONT, AUTO_SIZE))
	players(0),
	maxPlayer(0),
	mapMenuButton(new UI_Button(this, Rect(Point(10, 20), Size(100, 50)), Size(5,5), database.getMap(0)->getName(), MY_BUTTON, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	mapMenu(new MapMenu(mapMenuButton, Rect(10, 10, 100, 0), Size(0, 0), DO_NOT_ADJUST)),
	assignMap(-1)
{
/*TODO

- Map initialisieren ?
- Harvest speed initialisieren

*/
	mapMenu->Hide();
	for(unsigned int i = MAX_PLAYER;i--;)
	{
		player[i] = new PlayerEntry(this, Rect(0, 20+i*16, getRelativeClientRect().GetWidth(), 12), Size(0,0));
		player[i]->setNumber(i+1);
		player[i]->Hide();
	}
//		goalsFulfilledText[i] = new UI_StaticText(this, getRelativeClientRect() + Point(100, i * 20), IMPORTANT_COLOR, SMALL_BOLD_FONT, NO_TEXT_MODE); TODO => tooltip
//      resetButton->updateToolTip(RESET_BUILD_ORDER_TOOLTIP_STRING);
//	goalsFulfilledText->updateToolTip(GOALS_FULFILLED_TOOLTIP_STRING);
	resetData(); // TODO
}

void ScoreWindow::setMode(const unsigned int game_number, const unsigned int game_max)
{
	gameNumber = game_number;
	gameMax = game_max;
}

void ScoreWindow::reloadOriginalSize()
{
	setOriginalRect(theme.lookUpGameRect(SCORE_WINDOW, gameNumber, gameMax));
	setMaxHeight(theme.lookUpGameMaxHeight(SCORE_WINDOW, gameNumber, gameMax));
	
	for(unsigned int i = MAX_PLAYER;i--;)
		player[i]->setOriginalSize(Size(getRelativeClientRect().GetWidth(), 12));
	UI_Window::reloadOriginalSize();
}

ScoreWindow::~ScoreWindow()
{
	for(unsigned int i = MAX_PLAYER;i--;)
		delete player[i];
	delete mapMenuButton;
//	delete menuRadio;
	delete mapMenu;
//	delete resetButton;
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
			player[i]->adjustRelativeRect(Rect(getRelativeClientRectLeftBound(), line*16, getRelativeClientRect().GetWidth(), 12));
			line+=player[i]->getLineHeight(); // height of menu <-
		}
	}
	
// Alle Player durchlaufen, evtl Hoehe anpassen: 

	fitItemToRelativeClientRect(Rect(0,20*line,10,12)/*playerText[i]->getAbsoluteRect()*/,2); // TODO

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
	os.str("");
	os << 100*game->getUnchangedGenerations()  / configuration.getMaxGenerations() << "%";
	dc->DrawText(os.str(), getAbsolutePosition() + Size(getWidth() - 25, getHeight() - 30));*/
}


