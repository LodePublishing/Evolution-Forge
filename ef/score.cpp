#include "score.hpp"
#include <sstream>
#include <iomanip>

ScoreWindow::ScoreWindow(UI_Object* score_parent, const unsigned int game_number, const unsigned int game_max) :
	UI_Window(score_parent, TIMER_WINDOW_TITLE_STRING, theme.lookUpGameRect(SCORE_WINDOW, game_number, game_max), theme.lookUpGameMaxHeight(SCORE_WINDOW, game_number, game_max), NOT_SCROLLED),
// TODO irgendwas stimmt hier mit der Hoehe nicht
//	resetButton(new UI_Button(this, Rect(getRelativeClientRectPosition(), getClientRectSize()), RESET_BUILD_ORDER_STRING, MY_BUTTON, PRESS_BUTTON_MODE, CENTER_RIGHT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE))
	players(0),
	maxPlayer(0),
	menuRadio(new UI_Radio(this, Rect(getRelativeClientRectPosition() + Point(0, 10), Size(0,0)))),
	mapMenu(new MapMenu(this, Rect(10, 10, getTargetWidth()-100, 0))),
	assignMap(-1)
{
/*TODO

- Map initialisieren ?
- Harvest speed initialisieren

*/

	menuButton[MAP_MENU] = new UI_Button(this, Rect(Point(0, 0), getClientRectSize()), Size(5,5), CHOOSE_MAP_STRING, MY_BUTTON, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE_ONCE);
	mapMenu->Hide();
	for(unsigned int i=0; i<MAX_SCORE_MENUS; i++)
		menuRadio->addButton(menuButton[i], i);
	menuRadio->calculateSameWidthOfButtons(true);
		
	for(unsigned int i = MAX_PLAYER;i--;)
	{
		player[i] = new PlayerEntry(this, Rect(getRelativeClientRect().GetLeft(), 20+i*16, getRelativeClientRect().GetWidth(), 12), Size(0,0));
		player[i]->setNumber(i+1);
		player[i]->Hide();
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

void ScoreWindow::resetData()
{
// menuButton[MAP_MENU]->updateText(CHOOSE_MAP_STRING);
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


