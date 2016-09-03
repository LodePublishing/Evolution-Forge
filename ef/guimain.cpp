#include "guimain.hpp"
#include "../ui/editfield.hpp"
#include "../ui/tooltip.hpp"
#include "configuration.hpp"

Main::Main(DC* dc):
	mainWindow(NULL),
	helpWindow(NULL),
	settingsWindow(NULL),
	mapWindow(NULL),
	maus(),
	currentTab(0)
{
// ----- INITIALIZE DATABASE -----	
	toLog(UI_Object::theme.lookUpString(START_LOAD_UI_BITMAPS_FONTS_STRING));
#ifdef __linux__
	UI_Object::theme.loadGraphicData("settings/ui/default.ui","data/bitmaps/","data/fonts/",dc);
	UI_Object::theme.loadWindowData("settings/ui/windows.ui", 0, 1);
	UI_Object::theme.loadWindowData("settings/ui/split_windows.ui", 0, 2);
	UI_Object::theme.loadWindowData("settings/ui/split_windows.ui", 1, 2);
#elif __WIN32__
	UI_Object::theme.loadGraphicData("settings\\ui\\default.ui","data\\bitmaps\\","data\\fonts\\",dc);
	UI_Object::theme.loadWindowData("settings\\ui\\windows.ui", 0, 1);
	UI_Object::theme.loadWindowData("settings\\ui\\split_windows.ui", 0, 2);
	UI_Object::theme.loadWindowData("settings\\ui\\split_windows.ui", 1, 2);
#endif
	loadHarvestData();
	loadMaps();
	loadStartConditions();
	loadGoals();


	
// goal beschreibt Rasse, Ziele und Modus
	
// Map in "map.txt" is now map[0]
// choose the first map we loaded (map[0])
// ----- END OF INITIALIZING DATABASE -----
	mainWindow = new MainWindow();
	
	helpWindow = new HelpWindow(mainWindow);
	settingsWindow = new SettingsWindow(mainWindow);
	mapWindow = new MapWindow(mainWindow);
	
	Main::msgWindow = new MessageWindow(mainWindow);
	ForceWindow::techTreeWindow = new TechTreeWindow(mainWindow);
	ForceWindow::techTreeWindow->Hide();
	
	mainWindow->Show();
	helpWindow->Hide();
	settingsWindow->Hide();
	mapWindow->Hide();
	msgWindow->addMessage(UI_Object::theme.lookUpString(WELCOME_MSG1_STRING));
	msgWindow->addMessage(UI_Object::theme.lookUpString(WELCOME_MSG2_STRING));
	msgWindow->addMessage("Visit www.clawsoftware.de - - - ");

	for(unsigned int i = MAX_TABS;i--;)
	{
		tabToGameList[i] = -1;
		tabToSplitGameList[i] = -1;
	}
	for(unsigned int i = MAX_GAME;i--;)
		game[i]=NULL;
	
	initializeGame(0);
}

void Main::reloadOriginalSize()
{
	mainWindow->reloadOriginalSize();
	ForceWindow::techTreeWindow->reloadOriginalSize();
}

void Main::reloadStrings()
{
	mainWindow->reloadStrings();
	ForceWindow::techTreeWindow->reloadStrings();
}

void Main::initializeGame(const unsigned int tab_number)
{
	// erstes freies suchen:
	signed int game_number;
	int game_nr = 0;
	int game_max = 0;
	for(game_number = 0;game_number < MAX_GAME; ++game_number)
		if(game[game_number]==NULL)
			break;
	if(game_number == MAX_GAME)
		return;
	

	if(tabToGameList[tab_number]>=0)
	{
		if(tabToSplitGameList[tab_number]>=0)
		{
#ifdef _SCC_DEBUG
			toLog("ERROR: WTF!.");return;
#endif    
			// => schon besetzt! Fehler!
		} else
		{
			game_nr = 1;
			game_max = 2;
			game[tabToGameList[tab_number]]->setMode(0, game_max);
			tabToSplitGameList[tab_number] = game_number;
			// => als split window hernehmen!
		}
	} else
	{
	// => new tab
		game_nr = 0;
		game_max = 1;
		tabToGameList[tab_number] = game_number;
		mainWindow->addNewGameTab();
		mainWindow->forcePressTab(tab_number);
	}
	game[game_number] = new Game(mainWindow, game_nr, game_max); // TODO
	game[game_number]->assignMap(database.getMap(0));
	game[game_number]->assignStartCondition(1, database.getStartCondition(TERRA, 0)); // <- evtl auswaehlen... jo, aber erst spaeter einbauen TODO
	game[game_number]->setStartRace(1, TERRA); // <- ok
	game[game_number]->assignGoal(1, database.getGoal(TERRA, 0)); // <- immer auf 0 setzen
	game[game_number]->fillGroups();
	game[game_number]->setHarvestSpeed(1, TERRA, database.getHarvestSpeed(TERRA, 0));
	game[game_number]->setHarvestSpeed(1, PROTOSS, database.getHarvestSpeed(PROTOSS, 0));
	game[game_number]->setHarvestSpeed(1, ZERG, database.getHarvestSpeed(ZERG, 0));

	game[game_number]->setStartPosition(1, 1); // <- TODO
	game[game_number]->initSoup();
	game[game_number]->newGeneration();
	game[game_number]->Show();
	UI_Window::setResetFlag();
}


Main::~Main()
{
	delete mainWindow;
	delete ForceWindow::techTreeWindow;
	delete msgWindow;
	delete helpWindow;
	delete settingsWindow;
	delete mapWindow;
	for(unsigned int i=MAX_GAME;i--;)
		delete game[i];
}

void Main::resetData()
{
	for(unsigned int i=MAX_GAME;i--;)
		if(game[i])
			game[i]->resetData();
}

void Main::noticeFullscreen()
{
	settingsWindow->updateItems();
}

void Main::process()
{
	UI_Window::gotoHelpChapter = -1;
	mainWindow->resetMinXY();
//	ForceWindow::techTreeWindow->Hide();
	
	UI_Object::windowSelected = false;
	
	if((efConfiguration.isToolTips())&&(UI_Object::tooltip))
		UI_Object::tooltip->process();
	
	for(std::list<std::string>::iterator i = UI_Object::msgList.begin(); i!= UI_Object::msgList.end(); ++i)
	{
		msgWindow->addMessage(*i);
		i = UI_Object::msgList.erase(i);
	}

	mainWindow->process();

	if((UI_Object::tooltip)&&(UI_Object::toolTipParent->checkForNeedRedraw()))
	{
		UI_Object::tooltip->setNeedRedrawNotMoved();
	}

	ForceWindow::techTreeWindow->process();
//	if(UI_Button::getCurrentButton()==NULL) // TODO verschaerfen, boolvariable setzen wenn currentButton von !NULL auf NULL gesetzt wurde
		setMouse(maus);

	if(!UI_Object::windowSelected)
	{
//		if(UI_Object::currentWindow)
//			UI_Object::currentWindow->setNeedRedrawNotMoved();
		UI_Object::currentWindow=NULL;
	}
	
	if(settingsWindow->hasLanguageChanged())
	{
//		for(unsigned int i = MAX_GAME;i--;)
//			if(game[i])
//				game[i]->reloadStrings();
		mainWindow->reloadStrings();
		ForceWindow::techTreeWindow->reloadStrings();
//		settingsWindow->reloadStrings();
		msgWindow->addMessage(UI_Object::theme.lookUpString(LANGUAGE_HAS_CHANGED_STRING));
	} else

/*	if(settingsWindow->hasResolutionChanged())
	{
                for(unsigned int i = MAX_GAME;i--;)
                        if(game[i])
                                game[i]->reloadStrings();
                mainWindow->reloadStrings();
                settingsWindow->reloadStrings();
// TODO msgWindow add Message
	}*/

	if(UI_Window::gotoHelpChapter>=0)
	{
		mainWindow->forcePressTab(HELP_TAB);
		helpWindow->gotoChapter(UI_Window::gotoHelpChapter);
	}

	if(mainWindow->tabWasChanged())
	{
//		mainWindow->setGizmo(false);
		for(unsigned int i = MAX_GAME; i--;)
			if(game[i]!=NULL)
				game[i]->Hide();
		currentTab = mainWindow->getCurrentTab();
		switch(currentTab)
		{
			case MAP_TAB:
				msgWindow->Show();
				settingsWindow->Hide();
				mapWindow->Show();
				helpWindow->Hide();
			break;
			case SETTINGS_TAB:
				msgWindow->Hide();
				settingsWindow->Show();
				settingsWindow->updateItems();
				mapWindow->Hide();
				helpWindow->Hide();
			break;
			case HELP_TAB:
				msgWindow->Hide();
				settingsWindow->Hide();
				mapWindow->Hide();
				helpWindow->Show();
			break;
			default:
			// => game[currentTab] wechseln, TODO mehrere auf eine Seite
				msgWindow->Show();
				settingsWindow->Hide();
				mapWindow->Hide();
				helpWindow->Hide();
				if(tabToGameList[currentTab]>=0)
				{
					game[tabToGameList[currentTab]]->Show(); //...
					if(tabToSplitGameList[currentTab]>=0)
						game[tabToSplitGameList[currentTab]]->Show();
				}
				else // new game!
				{
					initializeGame(currentTab);
				}
			break;
// TODO delete game!

			
		} // end switch getCurrentTabs
//		for(unsigned int i=MAX_GAME;i--;)
//			game[i]->setMode(currentTab, i); // TODO
	} // end tabwasChanged*/

	if((tabToGameList[currentTab]>=0)&&(game[tabToGameList[currentTab]]->isSplitGame())&&(tabToSplitGameList[currentTab]==-1))
	{
		initializeGame(currentTab);
	}
	if((tabToGameList[currentTab]>=0)&&(game[tabToGameList[currentTab]]->isRemoveGame()))
	{
		if(tabToSplitGameList[currentTab]>=0) // delete just the first game, move second game to the first
		{
			delete game[tabToGameList[currentTab]];
			game[tabToGameList[currentTab]]=NULL;
			tabToGameList[currentTab] = tabToSplitGameList[currentTab];
			tabToSplitGameList[currentTab]=-1;
			game[tabToGameList[currentTab]]->setMode(0,1);
		} else if(mainWindow->getGameTabCount()>1) // delete the whole tab if it's not the last
		{
			delete game[tabToGameList[currentTab]];
			game[tabToGameList[currentTab]]=NULL;
			for(unsigned int i = currentTab;i < mainWindow->getGameTabCount()-1;++i)
				tabToGameList[i] = tabToGameList[i+1];
			tabToGameList[mainWindow->getGameTabCount()-1]=-1;
			mainWindow->removeGameTab(currentTab);
			if(currentTab == mainWindow->getGameTabCount())
				currentTab--;
			mainWindow->forcePressTab(currentTab);
			game[tabToGameList[currentTab]]->Show(); //...
			if(tabToSplitGameList[currentTab]>=0)
				game[tabToSplitGameList[currentTab]]->Show();
//			if(mainWindow->getGameTabCount()) TODO show/hide removeButton;
		}
	} else if((tabToSplitGameList[currentTab]>=0)&&(game[tabToSplitGameList[currentTab]]->isRemoveGame())) // just the second game
	{
		delete game[tabToSplitGameList[currentTab]];
		game[tabToSplitGameList[currentTab]]=NULL;
		game[tabToGameList[currentTab]]->setMode(0,1);
		tabToSplitGameList[currentTab]=-1;
	}
	
/*	for(unsigned int i = MAX_GAME;i--;) // TODO
		if((game[i])&&(game[i]->checkForNeedRedraw()))
		{
			mainWindow->setNeedRedrawNotMoved();
			msgWindow->setNeedRedrawNotMoved();
			settingsWindow->setNeedRedrawNotMoved();
			helpWindow->setNeedRedrawNotMoved();
//			mapWindow->setNeedRedrawNotMoved();
			mainWindow->setNeedRedrawMoved();
			break;
		}*/
}

/*void Main::stopAllOptimizing()
{
	for(unsigned int i=MAX_GAME;i--;)
		if((game[i])&&(game[i]->isShown()))
			game[i]->setOptimizing(false);
}

void Main::startAllOptimizing()
{
	for(unsigned int i=MAX_GAME;i--;)
		if((game[i])&&(game[i]->isShown()))
			game[i]->setOptimizing(true);
}*/

const bool Main::isAnyOptimizing() const
{
	for(unsigned int i=MAX_GAME;i--;)
		if((game[i])&&(game[i]->isShown())&&(game[i]->isAnyOptimizing()))
			return(true);
	return(false);
}


void Main::draw(DC* dc) const
{
	if(mainWindow->isShown())
	{
		bool redraw = mainWindow->checkForNeedRedraw();
		for(unsigned int i=MAX_GAME;i--;)
			if((game[i])&&(game[i]->checkForNeedRedraw()))
				redraw=true;
		if((UI_Object::tooltip!=NULL)&&(UI_Object::tooltip->checkForNeedRedraw()))
			redraw=true;
		if(UI_Object::toolTipWasDeleted)
		{
			UI_Object::toolTipWasDeleted = false;
			redraw=true;
		}

		if(redraw)
		{
			SDL_Rect rc;
			rc.x = 0;rc.y = 0; rc.w = UI_Object::max_x; rc.h = UI_Object::max_y;
			if(efConfiguration.isBackgroundBitmap())
				dc->Blit(UI_Object::theme.lookUpBitmap(BACKGROUND_BITMAP), rc);
			else
				dc->clearScreen();
			msgWindow->setNeedRedrawNotMoved();
			mainWindow->setNeedRedrawNotMoved();
		}
		mainWindow->draw(dc);
	}
	ForceWindow::techTreeWindow->draw(dc);
}

										
//settings: log level (none, debug only, +final result, +result of each run, +snapshot every X generations, +snapshot every generation, EVERYTHING (~2MB/generation!)

void Main::OnIdle()
{
//	if(UI_Object::focus==NULL) // TODO, bei Force/Bowindow abfragen
	{
		for(unsigned int i=MAX_GAME;i--;)
			if((game[i])&&(game[i]->isShown()))
				game[i]->newGeneration();
	}
}


const bool Main::newRun()
{
//	endrun=true; TODO
/*	if(efConfiguration.isAutoSaveRuns()) TODO
	{
		ostringstream os, os2;
		os << anarace[0]->getRun() << UI_Object::theme.lookUpString(ENDRUN_FINISHED_STRING) << "[" << formatTime2(anarace[0]->getRealTimer()) << "]";
		msgWindow->addMessage(os.str());
		os2 << "bo_" << (*anarace[0]->getCurrentGoal())->getName() << "_" << formatTime2(anarace[0]->getRealTimer()) << "_" << anarace[0]->getRun();
//		if(UI_EndRunDialog::getLastString()!="bo")
//			database.saveBuildOrder(UI_EndRunDialog::getLastString(), anarace[0]); // TODO, evtl alle saven...
//		else
			database.saveBuildOrder(os2.str(), anarace[0]); // TODO, evtl alle saven...
		// TODO Name muss mitangegeben werden...
		// Dann ist auch die Frage wie die Ergebnisse gespeichert werden sollen, klar, nach Rasse sortiert 
		// Voreinstellung fuer Editfeld ist letzter benutzter Name  (oder 'leer') + fortlaufende Zahl (Reset bei neuem Namen)
		// Spaeter: ueberschreiben pruefen
		// In DateiName evtl auch Zeit einbauen
		msgWindow->addMessage(UI_Object::theme.lookUpString(ENDRUN_SAVED_BUILDORDER_STRING));
		endrun=false;
	} else
	{
		ostringstream os;
		os << "bo_" << (*anarace[0]->getCurrentGoal())->getName() << "_" << formatTime2(anarace[0]->getRealTimer()) << "_" << anarace[0]->getRun();

		if(UI_Object::editTextField==NULL)
			UI_Object::editTextField = new UI_EndRunDialog(mainWindow, os.str());
		// Dialog aufmachen: Weiterrechnen oder neubeginnen? (checkbox: nicht mehr fragen) Neuanfang kann u.U. einen neuen Weg ermoeglichen und u.U. ein besseres Ergebnis bringen... Abspeichern Checkbox, EXIT, CONTINUE, NEW ROUND...
		// you may want to use 'comparison' (not availible yet) to show all results 
		else
		{
			if((UI_Object::editTextField->isDone())&&(UI_Object::editTextField->getCaller()==NULL))
			{
				if(UI_Object::editTextField->getString().length()>0)
				{
					ostringstream os;
					os << anarace[0]->getRun() << UI_Object::theme.lookUpString(ENDRUN_FINISHED_STRING) << " [" << formatTime2(anarace[0]->getRealTimer()) << "]";
					msgWindow->addMessage(os.str());
					database.saveBuildOrder(UI_Object::editTextField->getString(), anarace[0]); // TODO, evtl alle saven...
					msgWindow->addMessage(UI_Object::theme.lookUpString(ENDRUN_SAVED_BUILDORDER_STRING));
				}
				delete UI_Object::editTextField;
				UI_Object::resetButton();
				UI_Object::editTextField=NULL;
				endrun = false;
			} else
			if((UI_Object::editTextField->isCanceled())&&(UI_Object::editTextField->getCaller()==NULL))
			{
				delete UI_Object::editTextField;
				UI_Object::resetButton();
				UI_Object::editTextField=NULL;
				endrun = false;
			}
		}
	}
	if(!endrun)
		for(unsigned int i=database.getMap(0)->getMaxPlayer();i--;)
			game[i]->resetData();
	return(endrun);*/
	return(false);
}	

void Main::loadHarvestData()
{
	for(unsigned int i = 0; i < MAX_RACES; ++i)
	{
		std::list<std::string> harvestFiles = database.findFiles("settings", "harvest", raceString[i]);
		for(std::list<std::string>::iterator j = harvestFiles.begin(); j!=harvestFiles.end(); ++j)
			database.loadHarvestFile(*j);
	}
//TODO flag setzen ob was geladen wurde
}


void Main::loadStartConditions()
{
	for(unsigned int i = 0; i < MAX_RACES; ++i)
	{
		std::list<std::string> startFiles = database.findFiles("settings", "start", raceString[i]);
		for(std::list<std::string>::iterator j = startFiles.begin(); j!=startFiles.end(); ++j)
			database.loadStartConditionFile(*j);
	}
//TODO flag setzen ob was geladen wurde
}

void Main::loadMaps()
{
	std::list<std::string> mapFiles = database.findFiles("settings", "maps");
	for(std::list<std::string>::iterator j = mapFiles.begin(); j!=mapFiles.end(); ++j)
		database.loadMapFile(*j);
//TODO flag setzen ob was geladen wurde
}

void Main::loadGoals()
{
	for(unsigned int i = 0; i < MAX_RACES; ++i)
	{
		database.addDefaultGoal((eRace)i);
		std::list<std::string> goalFiles = database.findFiles("settings", "goals", raceString[i]);
		for(std::list<std::string>::iterator j = goalFiles.begin(); j!=goalFiles.end(); ++j)
			database.loadGoalFile(*j);
	}
//TODO flag setzen ob was geladen wurde
}


void Main::leftDown()
{
	if(UI_Button::getCurrentButton()!=NULL)
	{
		UI_Button::getCurrentButton()->mouseLeftButtonPressed();
		UI_Button::setCurrentButtonPressed();
	} else
	if(UI_Object::focus!=NULL)
		UI_Object::focus=NULL;
}

void Main::leftUp(const Point p)
{
	if((UI_Button::getCurrentButton()!=NULL)&&(UI_Button::isCurrentButtonPressed()))
		UI_Button::getCurrentButton()->mouseLeftButtonReleased();
	UI_Button::setCurrentButtonPressed(false);
	UI_Button::setCurrentButtonHasAlreadyLeft(false);
	setMouse(p);

	if(UI_Button::isMoveByMouse()==true)
	{
		UI_Button::setMoveByMouse(false);
		UI_Button::setMouseMovePoint(Point(0, 0));
	}
	if((UI_Button::getCurrentButton()==NULL)&&(UI_Object::focus!=NULL))
		UI_Object::focus=NULL;

}

void Main::rightDown()
{
	if(UI_Button::getCurrentButton()!=NULL)
	{
		UI_Button::getCurrentButton()->mouseRightButtonPressed();
		UI_Button::setCurrentButtonPressed();
	}
}

void Main::rightUp(const Point p)
{
	if((UI_Button::getCurrentButton()!=NULL)&&(UI_Button::isCurrentButtonPressed()))
		UI_Button::getCurrentButton()->mouseRightButtonReleased();
	UI_Button::setCurrentButtonPressed(false);
	UI_Button::setCurrentButtonHasAlreadyLeft(false);
	setMouse(p);
}

void Main::wheelUp()
{
	if(UI_Object::currentWindow)
		UI_Object::currentWindow->wheelUp();
}

void Main::wheelDown()
{
	if(UI_Object::currentWindow)
		UI_Object::currentWindow->wheelDown();
}

void Main::setMouse(const Point p)
{
	if(p == UI_Object::mouse)
		return;
	maus = p;
	UI_Object::mouse = p;
//	((BoGraphWindow*)(game[0]->window[BO_GRAPH_WINDOW]))->mouseHasMoved(); // TODO
//	else if(game[1]->window[BO_GRAPH_WINDOW]->Inside(p))
//		(BoGraphWindow*)(game[1]->window[BO_GRAPH_WINDOW])->mouseHasMoved();
	
	if(UI_Button::isCurrentButtonHasAlreadyLeft())
	{
		if((UI_Button::getCurrentButton()!=NULL)&&(UI_Button::getCurrentButton()->getAbsoluteRect().Inside(p)))
		{
			UI_Button::getCurrentButton()->mouseHasEnteredArea();
			UI_Button::setCurrentButtonHasAlreadyLeft(false);
		}
		else return;
	}
	
	if((UI_Button::getCurrentButton())&&(!UI_Button::getCurrentButton()->getAbsoluteRect().Inside(p)))
	{
		UI_Button::getCurrentButton()->mouseHasLeftArea();
		if(!UI_Button::isCurrentButtonPressed())
			UI_Button::resetButton();
		else
			UI_Button::setCurrentButtonHasAlreadyLeft();
	}
	
// ignore mousemove if button is still pressed		
	if(UI_Button::getCurrentButton())
	{
		if(UI_Button::isCurrentButtonPressed())
		{
			UI_Button::getCurrentButton()->doHighlight();
//			UI_Button::getCurrentButton()->mouseHasMoved();
		}
		return;
	}
	
	if(UI_Button::isMoveByMouse()==false)
	{
		UI_Object* temp_button = NULL;
//		if(UI_Object::focus==NULL) // TODO
		{
			for(unsigned int i=MAX_GAME;i--;)
				if((game[i])&&(!temp_button))
					temp_button = game[i]->checkHighlight();
			if(!temp_button)
				temp_button = mainWindow->checkHighlight();
			if(!temp_button)
				temp_button = settingsWindow->checkHighlight();
			if(!temp_button)
				temp_button = mapWindow->checkHighlight();		
			if(!temp_button)
				temp_button = helpWindow->checkHighlight();
//			if(!temp_button)
//				temp_button = (UI_Button*) (mapWindow->checkHighlight();
		} //else
//		if((!temp_button)&&(UI_Object::editFieldActive()))
//			temp_button = UI_Object::getEditField()->checkHighlight());
		if((temp_button) && (temp_button!=UI_Button::getCurrentButton()))
		{
			UI_Button::resetButton();
			UI_Button::setCurrentButton( (UI_Button*)temp_button);
			if(temp_button!=NULL)
			{
				UI_Button::getCurrentButton()->mouseHasEnteredArea();
				UI_Button::setCurrentButtonHasAlreadyLeft(false);
			}
		}

			if(efConfiguration.isToolTips())
			{
				UI_Object* temp=UI_Object::toolTipParent;
				UI_Object* temp2 = NULL;
				UI_Object::toolTipParent = NULL;

				for(unsigned int i=MAX_GAME;i--;)
					if(game[i])
					{
						if(UI_Object::toolTipParent==NULL)
							temp2 = game[i]->checkToolTip();
						if((temp2!=NULL) && (temp2->getToolTipString()!=NULL_STRING))
							UI_Object::toolTipParent = temp2;
						temp2=NULL;
					}
				
				if(UI_Object::toolTipParent==NULL)
					temp2 = mainWindow->checkToolTip();
				if((temp2!=NULL) && (temp2->getToolTipString()!=NULL_STRING))
					UI_Object::toolTipParent = temp2;
				temp2=NULL;
				if(UI_Object::toolTipParent==NULL)
					temp2 = settingsWindow->checkToolTip();
				if(temp2==NULL)
					temp2 = mapWindow->checkToolTip();
				if(temp2==NULL)
					temp2 = helpWindow->checkToolTip();

				if((temp2!=NULL) && (temp2->getToolTipString()!=NULL_STRING))
					UI_Object::toolTipParent = temp2;

// toolTipParent changed or tooltip has to be deleted?
				if(((UI_Object::toolTipParent!=temp)||(UI_Object::tooltip==NULL)))
				{
					delete UI_Object::tooltip;
					if(UI_Object::toolTipParent==NULL)
						UI_Object::tooltip=NULL;
					else
						UI_Object::tooltip=new UI_ToolTip(mainWindow, (UI_Object::toolTipParent)->getToolTipString());
				}

			}
//		}
	}


		
	if((!efConfiguration.isToolTips()) && (UI_Object::tooltip))
	{
		delete UI_Object::tooltip;
		UI_Object::tooltip=NULL;
		mainWindow->setNeedRedrawNotMoved();
	}
}


InfoWindow* Main::infoWindow = NULL;
MessageWindow* Main::msgWindow = NULL;
