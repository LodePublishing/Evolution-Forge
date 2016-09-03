#include "mainwindow.hpp"
#include "configuration.hpp"

MainWindow::MainWindow() : 
	UI_Window( NULL, MAIN_WINDOW_TITLE_STRING, theme.lookUpGlobalRect(MAIN_WINDOW), theme.lookUpGlobalMaxHeight(MAIN_WINDOW), NOT_SCROLLED, NO_AUTO_SIZE_ADJUST, TABBED, Rect(0, 0, 1280, 1024), TRANSPARENT ),
	ani(1),
	ani2(0),
	gizmo(true),
	gameTabCount(0),
	gameNumber(1)
{
// TODO: nach resolutions ordnen! *theme.lookUpRect etc. in data.txt eintragen
// left:

	for(unsigned int i = MAX_TABS;i--;)
	{
		gameNumbers[i]=0;
		tab[i]=NULL;
	}
 	tab[0] = new UI_Button(NULL, Rect(10, 1, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(20, 0), NEW_GAME_STRING, TAB_BUTTON, TAB_BUTTON_MODE, ARRANGE_TOP_LEFT, MIDDLE_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
// right:
	tab[MAP_TAB] = new UI_Button(NULL, Rect(0, 1, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(20, 0), MAP_TAB_STRING, TAB_BUTTON, TAB_BUTTON_MODE, ARRANGE_TOP_RIGHT, MIDDLE_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
	tab[SETTINGS_TAB] = new UI_Button(NULL, Rect(0, 1, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(20, 0), SETTINGS_TAB_STRING, TAB_BUTTON, TAB_BUTTON_MODE, ARRANGE_TOP_RIGHT, MIDDLE_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
	tab[HELP_TAB] = new UI_Button(NULL, Rect(0, 1, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(20, 0), HELP_TAB_STRING, TAB_BUTTON, TAB_BUTTON_MODE, ARRANGE_TOP_RIGHT, MIDDLE_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);

	for(unsigned int i=0;i<MAX_TABS;i++)
		if(tab[i]!=NULL)
		{
//			tab[i]->updateToolTip((eString)(BASIC_TAB_TOOLTIP_STRING+i-1));
			addTab(tab[i], i);
		}
}

MainWindow::~MainWindow()
{
	for(unsigned int i=MAX_TABS;i--;)
		delete tab[i];
}

void MainWindow::reloadOriginalSize()
{
	for(unsigned int i = 0; i < MAX_TABS; i++)
		if(tab[i])
		{
			tab[i]->setOriginalSize(Size(theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0));
			tab[i]->reloadOriginalSize();
		}
	setOriginalRect(theme.lookUpGlobalRect(MAIN_WINDOW));
	setMaxHeight(theme.lookUpGlobalMaxHeight(MAIN_WINDOW));
	UI_Window::reloadOriginalSize();
}

void MainWindow::reloadStrings()
{
	for(unsigned int i = MAX_TABS;i--;)
		if(tab[i])
			tab[i]->updateText(UI_Object::theme.lookUpFormattedString(GAME_NUMBER_STRING, gameNumbers[i]));
	UI_Window::reloadStrings();
}

void MainWindow::addNewGameTab()
{
	delete(tab[gameTabCount]); // 'new game' loeschen
	removeTab(gameTabCount);
	UI_Button::resetButton();

	tab[gameTabCount] = new UI_Button(this, Rect(10, 1, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(20, 0), UI_Object::theme.lookUpFormattedString(GAME_NUMBER_STRING, gameNumber), TAB_BUTTON, TAB_BUTTON_MODE, ARRANGE_TOP_LEFT, MIDDLE_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
	addTab(tab[gameTabCount], gameTabCount);
	gameNumbers[gameTabCount] = gameNumber;
	++gameTabCount;
	++gameNumber;
	
	if(gameTabCount<MAX_GAME_TABS)
	{
	 	tab[gameTabCount] = new UI_Button(this, Rect(10, 1, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(20, 0), NEW_GAME_STRING, TAB_BUTTON, TAB_BUTTON_MODE, ARRANGE_TOP_LEFT, MIDDLE_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
		addTab(tab[gameTabCount], gameTabCount);
	}	
}

const unsigned int MainWindow::getGameTabCount() const
{
	return gameTabCount;
}

void MainWindow::removeGameTab(const unsigned int game_number)
{
#ifdef _SCC_DEBUG
	if(game_number>=gameTabCount) {
		toLog("DEBUG: (MainWindow::removeGameTab): Value game_number out of range.");return;
	}
#endif
	removeTab(game_number);
//	UI_Button::resetButton();
	UI_Object::currentWindow = NULL;
	delete(tab[game_number]);
	for(unsigned int i = game_number; i < gameTabCount-1;++i)
		tab[i] = tab[i+1];
	--gameTabCount;
	if(gameTabCount==MAX_GAME_TABS-1) // alles voll => letztes durch new game ersetzen
	{
	 	tab[gameTabCount] = new UI_Button(this, Rect(10, 1, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(20, 0), NEW_GAME_STRING, TAB_BUTTON, TAB_BUTTON_MODE, ARRANGE_TOP_LEFT, MIDDLE_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
		addTab(tab[gameTabCount], gameTabCount);
	} else 
	{
		tab[gameTabCount]=tab[gameTabCount+1];
		tab[gameTabCount+1]=NULL;
	}
}

void MainWindow::process()
{
//	UI_Object::rectnumber=0;
	clearRedrawFlag();
	UI_Window::process();
	if(tabRow->checkForNeedRedraw())
		setNeedRedrawNotMoved();
}

void MainWindow::continueOptimizationAnimation(const bool running)
{
	if(running)
	{
		if(ani2>1)
		{
			++ani;
			ani2 = 0;
		} else ++ani2;
	}
	else ani = 1;
	if(ani>30) ani = 1;
}

const Size MainWindow::helper(DC* dc, Point point, const unsigned int dx, const int i, const std::string& str) const
{
	dc->SetTextForeground(DC::toSDL_Color(
				(Uint8)((0==ani%(20+i))*35+((0==ani%(19+i))+(0==ani%(21+i)))*15+20),
				(Uint8)((0==ani%(20+i))*35+((0==ani%(19+i))+(0==ani%(21+i)))*15+20),
				(Uint8)((0==ani%(20+i))*35+((0==ani%(19+i))+(0==ani%(21+i)))*30+60)));
	dc->DrawText(str.substr(str.size()-1, str.size()), point + Size(dx, 0));
	return(dc->GetTextExtent(str.c_str()));
}

void MainWindow::draw(DC* dc) const
{
//jedem player ein mainwindow zuweisen!

	UI_Window::draw(dc);
//	
#if 0
// ------ MOUSE DRAWING ------
		if(efConfiguration.isSoftwareMouse())
		{
//			SDL_ShowCursor(SDL_DISABLE);
			Point p = UI_Object::mouse - Size(20,10);//Point(90, 140);
			dc->SetFont(UI_Object::theme.lookUpFont(SMALL_ITALICS_BOLD_FONT));
			switch(UI_Object::mouseType)
			{
				case 0://dc->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_NONE), p);
					break;
				case 1:
//					dc->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_LEFT), p);
					dc->SetTextForeground(DC::toSDL_Color(179,0,0));
					dc->DrawText("Add a unit", p.x-50, p.y+2);
				break;
				case 2:
//					dc->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_RIGHT), p);
					dc->SetTextForeground(DC::toSDL_Color(0,177,188));
					dc->DrawText("Remove a unit", p.x+38, p.y+1);
				break;
				case 3:
//					dc->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_BOTH), p);
					dc->SetTextForeground(DC::toSDL_Color(179,0,0));
					dc->DrawText("Add a unit", p.x-50, p.y+2);
					dc->SetTextForeground(DC::toSDL_Color(0,177,188));
					dc->DrawText("Remove a unit", p.x+38, p.y+1);
				break;
				default:toLog("error, mouseType out of range");break;
			}
		}
//		else
//			SDL_ShowCursor(SDL_ENABLE);
#endif
// ------ END MOUSE DRAWING ------
}


