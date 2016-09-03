#include "mainwindow.hpp"
#include "configuration.hpp"

MainWindow::MainWindow() : 
	UI_Window( NULL, NULL_STRING, theme.lookUpGlobalRect(MAIN_WINDOW), theme.lookUpGlobalMaxHeight(MAIN_WINDOW), NOT_SCROLLED, NO_AUTO_SIZE_ADJUST, Rect(0, 0, 1280, 1024), TRANSPARENT ),
	gameTabCount(0),
	gameNumber(1),
	leftTabs(new UI_Group(this, Rect(Point(10, 0), Size(100,0)), Size(0, 0), HORIZONTAL_GROUP, TOP_LEFT, NULL_STRING)),
	rightTabs(new UI_Group(this, Rect(Point(0, 0), Size(100,0)), Size(0, 0), HORIZONTAL_GROUP, TOP_RIGHT, NULL_STRING)),
	oldTab((eTabs)0)
{
// TODO: nach resolutions ordnen! *theme.lookUpRect etc. in data.txt eintragen
// left:

	for(unsigned int i = MAX_TABS;i--;)
	{
		gameNumbers[i]=0;
		tab[i]=NULL;
	}
 	tab[0] = new UI_Button(leftTabs, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(2, 0), TAB_BUTTON, false, TOP_TAB_BUTTON_MODE, NEW_GAME_STRING, DO_NOT_ADJUST, MIDDLE_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
// right:
	tab[DATABASE_TAB] = new UI_Button(rightTabs, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(2, 0), DATABASE_TAB_BUTTON, true, TOP_TAB_BUTTON_MODE, DATABASE_TAB_STRING, DO_NOT_ADJUST, MIDDLE_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
//	tab[MAP_TAB] = new UI_Button(NULL, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(2, 0), MAP_TAB_BUTTON, true, TOP_TAB_BUTTON_MODE, MAP_TAB_STRING, DO_NOT_ADJUST, MIDDLE_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
	tab[SETTINGS_TAB] = new UI_Button(rightTabs, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(2, 0), SETTINGS_TAB_BUTTON, true, TOP_TAB_BUTTON_MODE, SETTINGS_TAB_STRING, DO_NOT_ADJUST, MIDDLE_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
	tab[HELP_TAB] = new UI_Button(rightTabs, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(2, 0), HELP_TAB_BUTTON, true, TOP_TAB_BUTTON_MODE, HELP_TAB_STRING, DO_NOT_ADJUST, MIDDLE_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);

	tab[DATABASE_TAB]->updateToolTip(DATABASE_TAB_TOOLTIP_STRING);
//	tab[MAP_TAB]->updateToolTip(MAP_TAB_TOOLTIP_STRING);
	tab[SETTINGS_TAB]->updateToolTip(SETTINGS_TAB_TOOLTIP_STRING);
	tab[HELP_TAB]->updateToolTip(HELP_TAB_TOOLTIP_STRING);
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
//	rightTabs->adjustPosition();
}

void MainWindow::reloadStrings()
{
	for(unsigned int i = MAX_TABS;i--;)
		if(tab[i])
			tab[i]->updateText(UI_Object::theme.lookUpFormattedString(GAME_NUMBER_STRING, gameNumbers[i]));
	UI_Window::reloadStrings();
}

const eTabs MainWindow::getCurrentTab() const
{
	for(unsigned int i = MAX_TABS; i--;)
		if((tab[i])&&(tab[i]->isCurrentlyActivated()))
			return((eTabs)i);
	return((eTabs)0);
}

void MainWindow::activateTab(const eTabs tab_number)
{
	for(unsigned int i = MAX_TABS; i--;)
		if(tab[i])
		{
			if(i==tab_number)
				tab[i]->forcePress();
			else
				tab[i]->forceUnpress();
		}
}

void MainWindow::activateTabNumber(unsigned int tab_number)
{
	unsigned int n = 0;
	for(unsigned int i = 0; i < MAX_TABS; ++i)
		if(tab[i])
		{
			if(n!=tab_number)
				tab[i]->forceUnpress();
			else
				tab[i]->forcePress();
			++n;
		}
}

void MainWindow::addNewGameTab()
{
	delete(tab[gameTabCount]); // 'new game' loeschen
//	removeTab(gameTabCount);
	
	tab[gameTabCount] = new UI_Button(leftTabs, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(2, 0), TAB_BUTTON, false, TOP_TAB_BUTTON_MODE, UI_Object::theme.lookUpFormattedString(GAME_NUMBER_STRING, gameNumber), DO_NOT_ADJUST, MIDDLE_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
	activateTab((eTabs)gameTabCount);

	gameNumbers[gameTabCount] = gameNumber;
	++gameTabCount;
	++gameNumber;
	
	if(gameTabCount<MAX_GAME_TABS)
	{
	 	tab[gameTabCount] = new UI_Button(leftTabs, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(2, 0), TAB_BUTTON, false, TOP_TAB_BUTTON_MODE, NEW_GAME_STRING, DO_NOT_ADJUST, MIDDLE_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
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
		toErrorLog("DEBUG: (MainWindow::removeGameTab): Value game_number out of range.");return;
	}
#endif
//	removeTab(game_number);
	UI_Object::currentWindow = NULL;
	delete(tab[game_number]);
	for(unsigned int i = game_number; i < gameTabCount-1;++i)
		tab[i] = tab[i+1];
	--gameTabCount;
	if(gameTabCount==MAX_GAME_TABS-1) // alles voll => letztes durch new game ersetzen
	{
	 	tab[gameTabCount] = new UI_Button(leftTabs, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(2, 0), TAB_BUTTON, false, TOP_TAB_BUTTON_MODE, NEW_GAME_STRING, DO_NOT_ADJUST, MIDDLE_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
//		addTab(tab[gameTabCount], gameTabCount);
	} else 
	{
		tab[gameTabCount]=tab[gameTabCount+1];
		tab[gameTabCount+1]=NULL;
	}
}

void MainWindow::process()
{
	clearRedrawFlag();
	UI_Window::process();

	for(unsigned int  i = MAX_TABS; i--;)
		if((tab[i])&&((eTabs)i!=oldTab)&&(tab[i]->isCurrentlyActivated()))
		{
			tab[oldTab]->forceUnpress();
			break;
		}
	
	if(leftTabs->checkForNeedRedraw() || rightTabs->checkForNeedRedraw())
		setNeedRedrawNotMoved();
	oldTab = getCurrentTab();
}

void MainWindow::draw(DC* dc) const
{
/*	if(checkForNeedRedraw())
	{
		int y = tab[HELP_TAB]->getHeight();

		dc->setBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
	// draw outer border:
		dc->setPen(*theme.lookUpPen(OUTER_BORDER_PEN));
		dc->DrawEdgedRoundedRectangle(Point(1, 6) + getAbsolutePosition(), getSize() - Size(2, 7), 6);
	// draw inner border:
		if(UI_Object::currentWindow == this)
			dc->setPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
		else
			dc->setPen(*theme.lookUpPen(INNER_BORDER_PEN));
		dc->DrawEdgedRoundedRectangle(Point(3, 8) + getAbsolutePosition(), getSize() - Size(6, 11), 6);
	}*/
	UI_Object::draw(dc);
	if(checkForNeedRedraw())
	{
	// draw outer border:
/*		dc->setPen(*theme.lookUpPen(OUTER_BORDER_PEN));
		dc->setBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
		dc->DrawEdgedRoundedRectangle(Point(1, 20) + getAbsolutePosition(), getSize() - Size(2, 2), 6);
		
	// draw inner border:
		if(UI_Object::currentWindow == this)
			dc->setPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
		else
			dc->setPen(*theme.lookUpPen(INNER_BORDER_PEN));
		dc->setBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
		
		dc->DrawEdgedRoundedRectangle(
				tab[getCurrentTab()]->getAbsoluteLowerBound()
				
				Point(3, 22) + getAbsolutePosition(), getSize() - Size(6, 6), 6);*/

		dc->setPen(Pen(dc->changeRelativeBrightness(*UI_Object::theme.lookUpPen(UI_Object::theme.lookUpButtonColors(TAB_BUTTON)->startBorderPen[NORMAL_BUTTON_PHASE])->getColor(), 70), 1, SOLID_PEN_STYLE));
		dc->DrawHorizontalLine(0, tab[getCurrentTab()]->getAbsoluteLowerBound(), tab[getCurrentTab()]->getAbsoluteLeftBound());
		dc->DrawHorizontalLine(tab[getCurrentTab()]->getAbsoluteRightBound(), tab[getCurrentTab()]->getAbsoluteLowerBound(), UI_Object::max_x);
	
	}
//	
#if 0
// ------ MOUSE DRAWING ------
		if(efConfiguration.isSoftwareMouse())
		{
//			SDL_ShowCursor(SDL_DISABLE);
			Point p = UI_Object::mouse - Size(20,10);//Point(90, 140);
			dc->setFont(UI_Object::theme.lookUpFont(SMALL_ITALICS_BOLD_FONT));
			switch(UI_Object::mouseType)
			{
				case 0://dc->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_NONE), p);
					break;
				case 1:
//					dc->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_LEFT), p);
					dc->setTextForeground(DC::toSDL_Color(179,0,0));
					dc->DrawText("Add a unit", p.x-50, p.y+2);
				break;
				case 2:
//					dc->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_RIGHT), p);
					dc->setTextForeground(DC::toSDL_Color(0,177,188));
					dc->DrawText("Remove a unit", p.x+38, p.y+1);
				break;
				case 3:
//					dc->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_BOTH), p);
					dc->setTextForeground(DC::toSDL_Color(179,0,0));
					dc->DrawText("Add a unit", p.x-50, p.y+2);
					dc->setTextForeground(DC::toSDL_Color(0,177,188));
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


/*enum eIsTabbed
{
	NOT_TABBED,
	TABBED
};*/

/*		UI_Radio* tabRow;
		UI_Button* tab[MAX_TABS];
		void addTab(UI_Button* tab_button, const unsigned int button_id);
		void removeTab(const unsigned int button_id);
		const signed int getCurrentTab() const;
		void forcePressTab(const unsigned int press_tab);
// has this window tab buttons at the top?
		eIsTabbed isTabbed;
		void drawTabs(DC* dc) const;
	

void UI_Window::addTab(UI_Button* tab_button, const unsigned int button_id)
{
#ifdef _SCC_DEBUG
	if(isTabbed==NOT_TABBED) {
                toErrorLog("DEBUG: (UI_Window::addTab): Tried to add a tab while window is marked as not tabbed.");return;
        }
#endif
	tabRow->addButton(tab_button, button_id);
}

void UI_Window::removeTab(const unsigned int button_id)
{
#ifdef _SCC_DEBUG
	if(isTabbed==NOT_TABBED) {
                toErrorLog("DEBUG: (UI_Window::removeTab): Tried to remove a tab while window is marked as not tabbed.");return;
        }
#endif
	tabRow->removeButton(button_id);
	tabRow->updateIds(); // fill the space
}

void UI_Window::forcePressTab(const unsigned int press_tab)
{
	if(isTabbed==NOT_TABBED) 
		return;
	tabRow->forcePress(press_tab);
}

const signed int UI_Window::getCurrentTab() const
{
	if(isTabbed==NOT_TABBED) 
		return -1;
	return(tabRow->getMarked());
}*/
