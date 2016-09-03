#include "mainwindow.hpp"
#include "game.hpp"

const unsigned int MAX_VISIBLE_GAME_TABS = 5;

MainWindow::MainWindow() : 
	UI_Window( NULL, NULL_STRING, theme.lookUpGlobalRect(MAIN_WINDOW), theme.lookUpGlobalMaxHeight(MAIN_WINDOW), NOT_SCROLLED, NO_AUTO_SIZE_ADJUST, Rect(0, 0, 1280, 1024), TRANSPARENT ),
	removeCurrentTabButton(new UI_Button(this, Rect( Point((theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH)+2)*MAX_VISIBLE_GAME_TABS + 30, 2), Size()), Size(), MENU_BUTTON, CANCEL_BITMAP, PRESS_BUTTON_MODE, NULL_STRING, DO_NOT_ADJUST)),
	scrollLeftButton(new UI_Button(this, Rect(0, 2, 8, 8), Size(0,0), ARROW_BUTTON, SMALL_ARROW_LEFT_BITMAP, PRESS_BUTTON_MODE, NULL_STRING)),
	scrollRightButton(new UI_Button(this, Rect( Point((theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH)+2)*MAX_VISIBLE_GAME_TABS + 15, 2), Size(8, 8)), Size(0,0), ARROW_BUTTON, SMALL_ARROW_RIGHT_BITMAP, PRESS_BUTTON_MODE, NULL_STRING)),
	gameTabCount(0),
	gameNumber(1),
	viewTabs(0),
	leftTabs(new UI_Group(this, Rect(Point(10, -3), Size(100,0)), Size(0, 0), HORIZONTAL_GROUP, false, TOP_LEFT, NULL_STRING)),
	rightTabs(new UI_Group(this, Rect(Point(0, -3), Size(100,0)), Size(0, 0), HORIZONTAL_GROUP, false, TOP_RIGHT, NULL_STRING)),
	oldTab((eTabs)0),
	tabHasChanged(true),
	markForRemove(false),
	markForNewGame(false),
	newGameTab(0)
{
// TODO: nach resolutions ordnen! *theme.lookUpRect etc. in data.txt eintragen
// left:

	for(unsigned int i = MAX_TABS;i--;)
	{
		gameNumbers[i]=0;
		tab[i]=NULL;
	}
 	tab[0] = new UI_Button(leftTabs, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(2, 0), TAB_BUTTON, NULL_BITMAP, TOP_TAB_BUTTON_MODE, NEW_GAME_STRING, DO_NOT_ADJUST, MIDDLE_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
// right:
	tab[DATABASE_TAB] = new UI_Button(rightTabs, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(2, 0), TAB_BUTTON, MAP_BITMAP, TOP_TAB_BUTTON_MODE, DATABASE_TAB_STRING, DO_NOT_ADJUST, MIDDLE_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH); // TODO
//	tab[MAP_TAB] = new UI_Button(NULL, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(2, 0), MAP_TAB_BUTTON, true, TOP_TAB_BUTTON_MODE, MAP_TAB_STRING, DO_NOT_ADJUST, MIDDLE_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
	tab[SETTINGS_TAB] = new UI_Button(rightTabs, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(2, 0), TAB_BUTTON, SETTING_BITMAP, TOP_TAB_BUTTON_MODE, SETTINGS_TAB_STRING, DO_NOT_ADJUST, MIDDLE_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
	tab[HELP_TAB]     = new UI_Button(rightTabs, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(2, 0), TAB_BUTTON, HELP_BITMAP, TOP_TAB_BUTTON_MODE, HELP_TAB_STRING, DO_NOT_ADJUST, MIDDLE_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);

	tab[DATABASE_TAB]->updateToolTip(DATABASE_TAB_TOOLTIP_STRING);
//	tab[MAP_TAB]->updateToolTip(MAP_TAB_TOOLTIP_STRING);
	tab[SETTINGS_TAB]->updateToolTip(SETTINGS_TAB_TOOLTIP_STRING);
	tab[HELP_TAB]->updateToolTip(HELP_TAB_TOOLTIP_STRING);
}

MainWindow::~MainWindow()
{
	for(unsigned int i=MAX_TABS;i--;)
		delete tab[i];
	delete removeCurrentTabButton;
	delete scrollLeftButton;
	delete scrollRightButton;
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
	removeCurrentTabButton->setOriginalPosition(Point((theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH)+2)*MAX_VISIBLE_GAME_TABS + 30, 2));
	scrollRightButton->setOriginalPosition(Point((theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH)+2)*MAX_VISIBLE_GAME_TABS + 15, 2));
	UI_Window::reloadOriginalSize();
}

void MainWindow::reloadStrings()
{
	for(unsigned int i = MAX_TABS;i--;)
		if((i < gameTabCount) && (tab[i]))
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
#ifdef _SCC_DEBUG
	if(tab_number<0) {
		toErrorLog("DEBUG (MainWindow::activateTab()): Value tab_number out of range.");return;
	}
#endif
	for(unsigned int i = MAX_TABS; i--;)
		if(tab[i])
		{
			if(i == (unsigned int)tab_number)
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
	
	tab[gameTabCount] = new UI_Button(leftTabs, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(2, 0), TAB_BUTTON, NULL_BITMAP, TOP_TAB_BUTTON_MODE, UI_Object::theme.lookUpFormattedString(GAME_NUMBER_STRING, gameNumber), DO_NOT_ADJUST, MIDDLE_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
	activateTab((eTabs)gameTabCount);

	gameNumbers[gameTabCount] = gameNumber;
	++gameTabCount;
	++gameNumber;
	
	if(gameTabCount<MAX_GAME_TABS)
	{
	 	tab[gameTabCount] = new UI_Button(leftTabs, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(2, 0), TAB_BUTTON, NULL_BITMAP, TOP_TAB_BUTTON_MODE, NEW_GAME_STRING, DO_NOT_ADJUST, MIDDLE_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
		if(gameTabCount >= MAX_VISIBLE_GAME_TABS)
			scrollRight();
		UI_Button::resetButton();
		UI_Button::setCurrentButton(tab[gameTabCount]);
	}

}

void MainWindow::adjustView()
{
	for(unsigned int i = MAX_GAME_TABS; i--;)
		if(tab[i])
		{
			if((i >= viewTabs) && (i < viewTabs + MAX_VISIBLE_GAME_TABS))
				tab[i]->Show();
			else tab[i]->Hide();
		}
	leftTabs->reloadOriginalSize();
}

void MainWindow::scrollLeft()
{
	leftTabs->reloadOriginalSize();
	if(viewTabs == 0)
		return;
	--viewTabs;
	adjustView();
}

void MainWindow::scrollRight()
{
	leftTabs->reloadOriginalSize();
	if(viewTabs + MAX_VISIBLE_GAME_TABS > gameTabCount)
		return;
	++viewTabs;
	adjustView();
}
		
void MainWindow::removeGameTab(const unsigned int game_number)
{
#ifdef _SCC_DEBUG
	if(game_number>=gameTabCount) {
		toErrorLog("DEBUG: (MainWindow::removeGameTab): Value game_number out of range.");return;
	}
#endif
	bool was_pressed = tab[game_number]->isCurrentlyActivated();
	UI_Object::currentWindow = NULL;
	delete(tab[game_number]);
	if((game_number>0) && (game_number == gameTabCount-1)) // last one
		tab[game_number-1]->forcePress();
	else if(game_number < gameTabCount-1)
		tab[game_number+1]->forcePress();
	for(unsigned int i = game_number; i < gameTabCount-1;++i)
		tab[i] = tab[i+1];
	--gameTabCount;
	if(gameTabCount == MAX_GAME_TABS-1) // alles voll => letztes durch new game ersetzen
	{
	 	tab[gameTabCount] = new UI_Button(leftTabs, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(2, 0), TAB_BUTTON, NULL_BITMAP, TOP_TAB_BUTTON_MODE, NEW_GAME_STRING, DO_NOT_ADJUST, MIDDLE_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);		
		adjustView();
	} else 
	{
		tab[gameTabCount] = tab[gameTabCount+1];
		tab[gameTabCount+1] = NULL;
		if(viewTabs + MAX_VISIBLE_GAME_TABS > gameTabCount)
			scrollLeft();
		else adjustView();
	}
}

void MainWindow::process()
{
	clearRedrawFlag();
	UI_Window::process();

	markForRemove = false;
	markForNewGame = false;

	for(unsigned int  i = MAX_TABS; i--;)
		if((tab[i]) && ((eTabs)i != oldTab) && (tab[i]->isCurrentlyActivated()))
		{
			tab[oldTab]->forceUnpress();
			break;
		}
	
	if(leftTabs->checkForNeedRedraw() || rightTabs->checkForNeedRedraw())
		setNeedRedrawNotMoved();

	tabHasChanged = false;
	eTabs t = getCurrentTab();
#ifdef _SCC_DEBUG
	if(t < 0)
		toErrorLog("DEBUG (MainWindow::process()): getCurrentTab out of range.");
#endif
	if(oldTab != t)
	{
		oldTab = t;
		tabHasChanged = true;
		if(oldTab == (eTabs)gameTabCount)
			markForNewGame = true;
	}

	if((oldTab != HELP_TAB) && (oldTab != SETTINGS_TAB) && /*(oldTab != MAP_TAB) && */(oldTab != DATABASE_TAB) && ((unsigned int)oldTab < gameTabCount) && (gameTabCount>1))
	{
		removeCurrentTabButton->Show();
		if(removeCurrentTabButton->isLeftClicked())
			markForRemove = true;
	}
	else removeCurrentTabButton->Hide();

	if(viewTabs == 0)
		scrollLeftButton->Hide();
	else scrollLeftButton->Show();
	if(viewTabs + MAX_VISIBLE_GAME_TABS > gameTabCount)
		scrollRightButton->Hide();
	else scrollRightButton->Show();

	if(scrollLeftButton->isLeftClicked())
		scrollLeft();
	if(scrollRightButton->isLeftClicked())
		scrollRight();
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

		if(tab[getCurrentTab()]->isShown())
		{
			dc->setPen(Pen(dc->changeRelativeBrightness(*UI_Object::theme.lookUpPen(UI_Object::theme.lookUpButtonColors(TAB_BUTTON)->startBorderPen[NORMAL_BUTTON_PHASE])->getColor(), NOT_PRESSED_BRIGHTEN), 1, SOLID_PEN_STYLE));
			dc->DrawHorizontalLine(0, tab[getCurrentTab()]->getAbsoluteLowerBound(), tab[getCurrentTab()]->getAbsoluteLeftBound());
			dc->setPen(Pen(dc->changeRelativeBrightness(*UI_Object::theme.lookUpPen(UI_Object::theme.lookUpButtonColors(TAB_BUTTON)->startBorderPen[NORMAL_BUTTON_PHASE])->getColor(), NOT_PRESSED_DARKEN), 1, SOLID_PEN_STYLE));
			dc->DrawHorizontalLine(tab[getCurrentTab()]->getAbsoluteRightBound(), tab[getCurrentTab()]->getAbsoluteLowerBound(), UI_Object::max_x);
		} else if(getCurrentTab() > viewTabs)
		{
			dc->setPen(Pen(dc->changeRelativeBrightness(*UI_Object::theme.lookUpPen(UI_Object::theme.lookUpButtonColors(TAB_BUTTON)->startBorderPen[NORMAL_BUTTON_PHASE])->getColor(), NOT_PRESSED_BRIGHTEN), 1, SOLID_PEN_STYLE));
			dc->DrawHorizontalLine(0, tab[getCurrentTab()]->getAbsoluteLowerBound(), UI_Object::max_x);
		} else
		{
			dc->setPen(Pen(dc->changeRelativeBrightness(*UI_Object::theme.lookUpPen(UI_Object::theme.lookUpButtonColors(TAB_BUTTON)->startBorderPen[NORMAL_BUTTON_PHASE])->getColor(), NOT_PRESSED_DARKEN), 1, SOLID_PEN_STYLE));
			dc->DrawHorizontalLine(0, tab[getCurrentTab()]->getAbsoluteLowerBound(), UI_Object::max_x);

		}
	
	}
	UI_Object::draw(dc);
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


