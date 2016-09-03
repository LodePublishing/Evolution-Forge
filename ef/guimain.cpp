#include "guimain.hpp"
#include "../ui/editfield.hpp"
#include "../ui/tooltip.hpp"
#include "configuration.hpp"

Main::Main(DC* dc):
	mainWindow(NULL),
	tutorialWindow(NULL),
	settingsWindow(NULL),
	maus(),
	currentTab(0)
{
// ----- INITIALIZE DATABASE -----	
//	UI_Object::theme.setTab(BASIC_TAB);
	toLog(*UI_Object::theme.lookUpString(START_LOAD_UI_BITMAPS_FONTS_STRING));
#ifdef __linux__
	UI_Object::theme.loadGraphicData("settings/ui/default.ui","data/bitmaps/","data/fonts/",dc);

	UI_Object::theme.loadWindowData("settings/ui/windows.ui", 0, 1);
	UI_Object::theme.loadWindowData("settings/ui/split_windows.ui", 0, 2);
	UI_Object::theme.loadWindowData("settings/ui/split_windows.ui", 1, 2);
//	bar->draw(dc, 20, START_UI_BITMAPS_FONTS_LOADED_STRING);
// Always do loadHarvestFile (mining speeds) before loadMapFile, because at the moment the mapfile also sets the gathering speed
//	bar->draw(dc, 2, START_LOAD_HARVEST_STRING);
//	database.loadHarvestFile("settings/harvest/default.hvt");
	
//	bar->draw(dc, 2, START_LOAD_MAPS_STRING);
//	database.loadMapFile("settings/maps/lt41.map");
//	bar->draw(dc, 4, START_LOAD_STARTCONDITIONS_STRING);
//	database.loadStartConditionFile("settings/start/default_terra.start");
//	database.loadStartConditionFile("settings/start/default_protoss.start");
//	database.loadStartConditionFile("settings/start/default_zerg.start");
#elif __WIN32__
	UI_Object::theme.loadGraphicData("settings\\ui\\default.ui","data\\bitmaps\\","data\\fonts\\",dc);
	UI_Object::theme.loadWindowData("settings\\ui\\windows.ui", 0, 1);
	UI_Object::theme.loadWindowData("settings\\ui\\split_windows.ui", 0, 2);
	UI_Object::theme.loadWindowData("settings\\ui\\split_windows.ui", 1, 2);
//	bar->draw(dc, 20, START_UI_BITMAPS_FONTS_LOADED_STRING);
// Always do loadHarvestFile (mining speeds) before loadMapFile, because at the moment the mapfile also sets the gathering speed
//	bar->draw(dc, 2, START_LOAD_HARVEST_STRING);
//	database.loadHarvestFile("settings\\harvest\\default.hvt");
	
//	bar->draw(dc, 2, START_LOAD_MAPS_STRING);
//	database.loadMapFile("settings\\maps\\lt41.map");
//	bar->draw(dc, 4, START_LOAD_STARTCONDITIONS_STRING);
//	database.loadStartConditionFile("settings\\start\\default_terra.start");
//	database.loadStartConditionFile("settings\\start\\default_protoss.start");
//	database.loadStartConditionFile("settings\\start\\default_zerg.start");
#endif
	loadHarvestData();
	loadMaps();
	loadStartConditions();
//	bar->draw(dc, 12, START_LOAD_GOALS_STRING);
	loadGoals();

// goal beschreibt Rasse, Ziele und Modus
	
//	bar->draw(dc, 8, START_ASSIGN_AND_ANALYZE_STRING);
// Map in "map.txt" is now map[0]
// choose the first map we loaded (map[0])
// ----- END OF INITIALIZING DATABASE -----
	UI_Object::assignStartTime();
//	bar->draw(dc, 2, START_INIT_GUI_STRING);
	mainWindow = new MainWindow();
	tutorialWindow = new TutorialWindow(mainWindow);
	settingsWindow = new SettingsWindow(mainWindow);
	Main::msgWindow = new MessageWindow(mainWindow);
	ForceWindow::techTreeWindow = new TechTreeWindow(mainWindow);
	
	mainWindow->Show();
	tutorialWindow->Hide();
	settingsWindow->Hide();
	msgWindow->addMessage(*(UI_Object::theme.lookUpString(WELCOME_MSG1_STRING)));
	msgWindow->addMessage(*(UI_Object::theme.lookUpString(WELCOME_MSG2_STRING)));
//	msgWindow->addMessage(UI_Object::theme.lookUpFormattedString(GAMES_LOADED_STRING, database.getMap(0)->getMaxPlayer()));
//	bar->draw(dc, 8, START_MAIN_INIT_COMPLETE_STRING);
//	mainWindow->forcePressTab(0);//BASIC_TAB); // !!

	for(unsigned int i = MAX_TABS;i--;)
	{
		tabToGameList[i] = -1;
		tabToSplitGameList[i] = -1;
	}
	for(unsigned int i = MAX_GAME;i--;)
		game[i]=NULL;
	
	initializeGame(0);
}

/*void Main::closeGame()
{
}*/

void Main::initializeGame(const unsigned int tab_number)
{
	// erstes freies suchen:
	signed int game_number;
	int game_nr = 0;
	int game_max = 0;
	for(game_number = 0;game_number < MAX_GAME;game_number++)
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
			game[tabToGameList[tab_number]]->setMode(0,game_max);
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
	}
	game[game_number] = new Game(mainWindow, game_nr, game_max); // TODO
	game[game_number]->assignMap(database.getMap(0));
	game[game_number]->setHarvestSpeed(1, TERRA, database.getHarvestSpeed(TERRA, 0));
	game[game_number]->setHarvestSpeed(1, PROTOSS, database.getHarvestSpeed(PROTOSS, 0));
	game[game_number]->setHarvestSpeed(1, ZERG, database.getHarvestSpeed(ZERG, 0));
	game[game_number]->setStartRace(1, TERRA); // <- ok
	game[game_number]->setStartPosition(1, 1); // <- TODO
	game[game_number]->assignGoal(1, database.getGoal(TERRA, 0)); // <- immer auf 0 setzen
	game[game_number]->assignStartCondition(1, database.getStartCondition(TERRA, 0)); // <- evtl auswaehlen... jo, aber erst spaeter einbauen TODO
	game[game_number]->initSoup();
	game[game_number]->fillGroups();
	game[game_number]->newGeneration();
	game[game_number]->Show();
	UI_Window::setResetFlag();
}


Main::~Main()
{
	delete mainWindow;
	delete msgWindow;
	delete tutorialWindow;
	delete settingsWindow;
	for(unsigned int i=MAX_GAME;i--;)
		delete game[i];
}

void Main::resetData()
{
	endrun = false;
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
	ForceWindow::techTreeWindow->Hide();
	
	UI_Object::windowSelected = false;
	
//	mainWindow->continueOptimizingAnimation(isOptimizing());
	
	if((efConfiguration.isToolTips())&&(UI_Object::tooltip))
		UI_Object::tooltip->process();

	
	for(std::list<std::string>::iterator i = UI_Object::msgList.begin(); i!= UI_Object::msgList.end(); i++)
	{
		msgWindow->addMessage(*i);
		i = UI_Object::msgList.erase(i);
	}

				

	mainWindow->process();


	if(!UI_Object::windowSelected)
	{
		if(UI_Object::currentWindow)
			UI_Object::currentWindow->setNeedRedrawNotMoved();
		UI_Object::currentWindow=NULL;
	}
	if(settingsWindow->hasLanguageChanged())
	{
		for(unsigned int i = MAX_GAME;i--;)
			if(game[i])
				game[i]->reloadStrings();
		mainWindow->reloadStrings();
		settingsWindow->reloadStrings();
		msgWindow->addMessage(*UI_Object::theme.lookUpString(LANGUAGE_HAS_CHANGED_STRING));
	}

	if(mainWindow->tabWasChanged())
	{
//		boHasChanged = true; an entsprechende (?) Games weiterleiten
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
				tutorialWindow->Hide();
			break;
			case SETTINGS_TAB:
				msgWindow->Hide();
				settingsWindow->Show();
				settingsWindow->updateItems();
				tutorialWindow->Hide();
			break;
			case TUTORIAL_TAB:
				msgWindow->Hide();
				settingsWindow->Hide();
				tutorialWindow->Hide();
			break;
			default:
			// => game[currentTab] wechseln, TODO mehrere auf eine Seite
				msgWindow->Show();
				settingsWindow->Hide();
				tutorialWindow->Hide();
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

//		UI_Object::theme.setTab(currentTab);
		
//		mainWindow->updateRectangles(0);
//		msgWindow->updateRectangles(0);
//		settingsWindow->updateRectangles(0);
//		tutorialWindow->updateRectangles(0);
//		unsigned int maxPlayer=0;
//		for(unsigned int i=database.getMap(0)->getMaxPlayer();i--;)
//			if(game[i]->isShown()) 
//				maxPlayer++;
//		for(unsigned int i=MAX_GAME;i--;)
//			if(game[i]->isShown())
//				game[i]->updateRectangles(maxPlayer-1);
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
			for(unsigned int i = currentTab;i < mainWindow->getGameTabCount()-1;i++)
				tabToGameList[i] = tabToGameList[i+1];
			tabToGameList[mainWindow->getGameTabCount()-1]=-1;
			mainWindow->removeGameTab(currentTab);
			if(currentTab == mainWindow->getGameTabCount())
				currentTab--;
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
	
	for(unsigned int i = MAX_GAME;i--;) // TODO
		if((game[i])&&(game[i]->checkForNeedRedraw()))
		{
			mainWindow->setNeedRedrawNotMoved();
			msgWindow->setNeedRedrawNotMoved();
			settingsWindow->setNeedRedrawNotMoved();
			mainWindow->setNeedRedrawMoved();
			break;
		}

	
}

void Main::stopAllOptimizing()
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
}

const bool Main::isAnyOptimizing() const
{
//	for(unsigned int i=MAX_GAME;i--;)
//		if((game[i])&&(game[i]->isShown())&&(game[i]->isOptimizing()))
			return(true);
//	return(false);
}



void Main::draw(DC* dc) const
{
	if(mainWindow->isShown())
	{
		bool redraw = mainWindow->checkForNeedRedraw();
		for(unsigned int i=MAX_GAME;i--;)
			if((game[i])&&(game[i]->checkForNeedRedraw()))
				redraw=true;
		if(redraw)
		{
			SDL_Rect rc;
			rc.x = 0;rc.y = 0; rc.w = UI_Object::max_x; rc.h = UI_Object::max_y;
			if(efConfiguration.isBackgroundBitmap())
				SDL_BlitSurface(UI_Object::theme.lookUpBitmap(BACKGROUND_BITMAP) , 0, dc->GetSurface(), &rc);
			else
				SDL_FillRect(dc->GetSurface(), &rc, 0);
		}
		mainWindow->draw(dc);
	}
//	std::ostringstream os;os << "TAB NUMBER: " << currentTab << " / " << mainWindow->getGameTabCount();
//	dc->DrawText(os.str(), 50, 600);
									

	
/*	SDL_Rect c;
	c.x=maus.x;
	c.y=maus.y;
	c.w=32;
	c.h=32;	
//	get_bg(dc, cursor_save, maus.x, maus.y);
//	SDL_BlitSurface(*UI_Object::theme.lookUpBitmap(MAUS_BITMAP) , 0, dc->GetSurface(), &c );
//	RS_Blit(dc, cursor, maus.x, maus.y);
	//RS_Blit(dc, cursor_save, maus.x, maus.y);*/
}

										
//settings: log level (none, debug only, +final result, +result of each run, +snapshot every X generations, +snapshot every generation, EVERYTHING (~2MB/generation!)

void Main::OnIdle()
{
	if((!UI_Object::editTextField))
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
		os << anarace[0]->getRun() << *UI_Object::theme.lookUpString(ENDRUN_FINISHED_STRING) << "[" << formatTime2(anarace[0]->getRealTimer()) << "]";
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
		msgWindow->addMessage(*UI_Object::theme.lookUpString(ENDRUN_SAVED_BUILDORDER_STRING));
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
					os << anarace[0]->getRun() << *UI_Object::theme.lookUpString(ENDRUN_FINISHED_STRING) << " [" << formatTime2(anarace[0]->getRealTimer()) << "]";
					msgWindow->addMessage(os.str());
					database.saveBuildOrder(UI_Object::editTextField->getString(), anarace[0]); // TODO, evtl alle saven...
					msgWindow->addMessage(*UI_Object::theme.lookUpString(ENDRUN_SAVED_BUILDORDER_STRING));
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
	for(unsigned int i = 0; i < MAX_RACES; i++)
	{
		std::list<std::string> harvestFiles = database.findFiles("settings", "harvest", raceString[i]);
		for(std::list<std::string>::iterator j = harvestFiles.begin(); j!=harvestFiles.end(); j++)
			database.loadHarvestFile(*j);
	}
//TODO flag setzen ob was geladen wurde
}


void Main::loadStartConditions()
{
	for(unsigned int i = 0; i < MAX_RACES; i++)
	{
		std::list<std::string> startFiles = database.findFiles("settings", "start", raceString[i]);
		for(std::list<std::string>::iterator j = startFiles.begin(); j!=startFiles.end(); j++)
			database.loadStartConditionFile(*j);
	}
//TODO flag setzen ob was geladen wurde
}

void Main::loadMaps()
{
	std::list<std::string> mapFiles = database.findFiles("settings", "maps");
	for(std::list<std::string>::iterator j = mapFiles.begin(); j!=mapFiles.end(); j++)
		database.loadMapFile(*j);
//TODO flag setzen ob was geladen wurde
}

void Main::loadGoals()
{
	for(unsigned int i = 0; i < MAX_RACES; i++)
	{
		std::list<std::string> goalFiles = database.findFiles("settings", "goals", raceString[i]);
		for(std::list<std::string>::iterator j = goalFiles.begin(); j!=goalFiles.end(); j++)
			database.loadGoalFile(*j);
	}
//TODO flag setzen ob was geladen wurde
}


void Main::leftDown()
{
	if(UI_Object::currentButton)
	{
		UI_Object::currentButton->mouseLeftButtonPressed();
		UI_Object::currentButtonPressed=true;
	}
}

void Main::leftUp(const Point p)
{
	if((UI_Object::currentButton)&&(UI_Object::currentButtonPressed))
		UI_Object::currentButton->mouseLeftButtonReleased();
	UI_Object::currentButtonPressed=false;
	UI_Object::currentButtonHasAlreadyLeft=false;
	setMouse(p);
}

void Main::rightDown()
{
	if(UI_Object::currentButton)
	{
		UI_Object::currentButton->mouseRightButtonPressed();
		UI_Object::currentButtonPressed=true;
	}
}

void Main::rightUp(const Point p)
{
	if((UI_Object::currentButton)&&(UI_Object::currentButtonPressed))
		UI_Object::currentButton->mouseRightButtonReleased();
	UI_Object::currentButtonPressed=false;
	UI_Object::currentButtonHasAlreadyLeft=false;
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
//	if(p == UI_Object::mouse)
//		return;
	maus = p;
	UI_Object::mouse = p;
//	((BoGraphWindow*)(game[0]->window[BO_GRAPH_WINDOW]))->mouseHasMoved(); // TODO
//	else if(game[1]->window[BO_GRAPH_WINDOW]->Inside(p))
//		(BoGraphWindow*)(game[1]->window[BO_GRAPH_WINDOW])->mouseHasMoved();
	
	if(UI_Object::currentButtonHasAlreadyLeft)
	{
		if((UI_Object::currentButton)&&(UI_Object::currentButton->getAbsoluteRect().Inside(p)))
		{
			UI_Object::currentButton->mouseHasEnteredArea();
			UI_Object::currentButtonHasAlreadyLeft=false;
		}
		else return;
	}
	if((UI_Object::currentButton)&&(!UI_Object::currentButton->getAbsoluteRect().Inside(p)))
	{
		UI_Object::currentButton->mouseHasLeftArea();
		if(!UI_Object::currentButtonPressed)
			UI_Object::currentButton=0;
		else
			UI_Object::currentButtonHasAlreadyLeft=true;
	}
// ignore mousemove if button is still pressed		
	if(UI_Object::currentButton)
	{
		if(UI_Object::currentButtonPressed)
		{
			UI_Object::currentButton->forceHighlighted();
//			UI_Object::currentButton->mouseHasMoved();
		}
		return;
	}
	UI_Object::currentButton=NULL;
	if(UI_Object::editTextField==NULL)
	{
		for(unsigned int i=MAX_GAME;i--;)
			if((game[i])&&(!UI_Object::currentButton))
				UI_Object::currentButton = (UI_Button*) (game[i]->checkHighlight());
				
		if(!UI_Object::currentButton)
			UI_Object::currentButton = (UI_Button*) (mainWindow->checkHighlight());
		if(!UI_Object::currentButton)
			UI_Object::currentButton = (UI_Button*) (settingsWindow->checkHighlight());
	} else
	if((!UI_Object::currentButton)&&(UI_Object::editTextField!=NULL))
		UI_Object::currentButton = (UI_Button*) (UI_Object::editTextField->checkHighlight());
	if(UI_Object::currentButton)
	{
		UI_Object::currentButton->mouseHasEnteredArea();
		UI_Object::currentButtonHasAlreadyLeft=false;
	}
	if(efConfiguration.isToolTips())
	{
//		UI_Object* temp=UI_Object::toolTipParent;
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
		if((temp2!=NULL) && (temp2->getToolTipString()!=NULL_STRING))
			UI_Object::toolTipParent = temp2;
//		if((UI_Object::toolTipParent!=temp)//||(UI_Object::tooltip==NULL))
// TODO
		{
			delete UI_Object::tooltip;
			if(/*(temp!=NULL)&&*/(UI_Object::toolTipParent==NULL))
				UI_Object::tooltip=NULL;
			else
				UI_Object::tooltip=new UI_ToolTip(mainWindow, (UI_Object::toolTipParent)->getToolTipString());
			mainWindow->setNeedRedrawNotMoved();
		}
	} else if(UI_Object::tooltip)
	{
		delete UI_Object::tooltip;
		UI_Object::tooltip=NULL;
		mainWindow->setNeedRedrawNotMoved();
	}
}


//ProgressBar* Main::bar;
InfoWindow* Main::infoWindow = NULL;
MessageWindow* Main::msgWindow = NULL;
