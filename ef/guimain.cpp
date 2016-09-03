#include "guimain.hpp"
#include "files.hpp"
//#include "../ui/editfield.hpp"
#include "../ui/tooltip.hpp"
#include "configuration.hpp"
#include <iomanip>

const std::string intro_string = "TEST TEST TEST#";
Main::Main():
	mainMenuLine(NULL),
	introWindow(NULL),
	helpWindow(NULL),
	settingsWindow(NULL),
	dataBaseWindow(NULL),
//	mapWindow(NULL),
	saveBox(NULL),
	languageMenu(NULL),
	maus(),
	gameCount(0),
	currentTab((eTabs)0),
	currentGame(0),
	gameTypeHasChanged(false),
	backGround(NULL),
	text(NULL)
{
	for(unsigned int i = MAX_TABS;i--;)
	{
		tabToGameList[i] = -1;
		tabToSplitGameList[i] = -1;
	}
	
	for(unsigned int i = MAX_GAME;i--;)
		game[i]=NULL;
}
#include <sstream>
const bool Main::initGUI()
{
	toInitLog(UI_Object::theme.lookUpString(START_LOAD_UI_BITMAPS_FONTS_STRING));
#ifdef __linux__
	UI_Object::theme.loadData("settings/ui/default.ui", "data/bitmaps/", "data/fonts/", UI_Object::dc);
	UI_Object::sound.loadSoundDataFile("data/sounds/");
	UI_Object::theme.loadWindowDataFile("settings/ui/windows.ui", 0, 1);
	UI_Object::theme.loadWindowDataFile("settings/ui/split_windows.ui", 0, 2);
	UI_Object::theme.loadWindowDataFile("settings/ui/split_windows.ui", 1, 2);
#elif __WIN32__
	UI_Object::theme.loadData("settings\\ui\\default.ui", "data\\bitmaps\\", "data\\fonts\\", UI_Object::dc);
	UI_Object::sound.loadSoundDataFile("data\\sounds\\");
	UI_Object::theme.loadWindowDataFile("settings\\ui\\windows.ui", 0, 1);
	UI_Object::theme.loadWindowDataFile("settings\\ui\\split_windows.ui", 0, 2);
	UI_Object::theme.loadWindowDataFile("settings\\ui\\split_windows.ui", 1, 2);
#endif

// ----- INITIALIZING WINDOWS -----
	toInitLog(UI_Object::theme.lookUpString(START_INIT_GUI_STRING)); // ? TODO

	toInitLog("* " + UI_Object::theme.lookUpString(START_INIT_MAIN_WINDOW_STRING));
	backGround = new UI_BackGround();

	mainMenuLine = new MainMenuLine(backGround);
	
//	toInitLog("* " + UI_Object::theme.lookUpString(START_INIT_HELP_WINDOW_STRING));
//	helpWindow = new HelpWindow(backGround);
	
	toInitLog("* " + UI_Object::theme.lookUpString(START_INIT_SETTINGS_WINDOW_STRING));
	settingsWindow = new SettingsWindow(backGround);
	
	toInitLog("* " + UI_Object::theme.lookUpString(START_INIT_DATABASE_WINDOW_STRING));
	dataBaseWindow = new DataBaseWindow(backGround);
	
//	toInitLog("* " + UI_Object::theme.lookUpString(START_INIT_MAP_WINDOW_STRING));
//	mapWindow = new MapWindow(backGround);
	
//	toInitLog("* " + UI_Object::theme.lookUpString(START_INIT_MSG_WINDOW_STRING));
//	Main::msgWindow = new MessageWindow(backGround);
	
	toInitLog("* " + UI_Object::theme.lookUpString(START_INIT_INTRO_WINDOW_STRING));
	introWindow = new IntroWindow(backGround);
	
//	text = new UI_LongText(backGround/*getScrollBar()*/, Rect(Point(20, 30), Size(Size(400,300))), Size(0, 0), intro_string, BRIGHT_TEXT_COLOR, FORCE_TEXT_COLOR, LARGE_FONT, TEXT_BUTTON, VISITED_TEXT_BUTTON, NULL);
	
	languageMenu = new UI_Menu(NULL, Rect(Point((UI_Object::max_x - UI_Object::theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH))/2, UI_Object::max_y/2), Size(0,0)), Size(0,0), DO_NOT_ADJUST, true, ONE_COLOUMN_MENU, SMALL_BUTTON_WIDTH, 2, SETTING_ENGLISH_LANGUAGE_STRING, UNIT_TYPE_5_BUTTON);
	
	debugWindow = new DebugWindow(backGround);

	toInitLog("* " + UI_Object::theme.lookUpString(START_INIT_TECHTREE_WINDOW_STRING));
	ForceWindow::techTreeWindow = new TechTreeWindow(backGround);
	
	toInitLog("* " + UI_Object::theme.lookUpString(START_HIDING_WINDOWS_STRING));

	ForceWindow::techTreeWindow->Hide();
	mainMenuLine->Hide();
//	helpWindow->Hide();
	settingsWindow->Hide();
	dataBaseWindow->Hide();
	debugWindow->Hide();
	if(!uiConfiguration.isFirstStart())
		languageMenu->Hide();
	
	UI_Object::addMessage(WELCOME_MSG1_STRING);
	UI_Object::addMessage(WELCOME_MSG2_STRING);
	UI_Object::addMessage("Visit www.clawsoftware.de - - - ");
// ----- END INITIALIZING WINDOWS -----

//	backGround->setZ(100);
	return(true); // TODO
}


const bool Main::initCore()
{
// ----- INITIALIZE DATABASE -----	
	toInitLog(UI_Object::theme.lookUpString(START_INIT_CORE_STRING));

	database.init();

/*	if(!loadHarvestData()) {
		toErrorLog("ERROR (Main::init()): Harvest data was not successfully initialized.");
		return(false);
	}*/

	if(!loadMaps()) {
		toErrorLog("ERROR (Main::init()): Map data was not successfully initialized.");
		return(false);
	}
	
	if(!loadStartConditions()) {
		toErrorLog("ERROR (Main::init()): Start conditions were not successfully initialized.");
		return(false);
	}
	
	if(!loadGoals()) {
		toErrorLog("ERROR (Main::init()): Goals were not successfully initialized.");
		return(false);
	}
	
	if(!loadBuildOrders()) {
		toErrorLog("ERROR (Main::init()): Build orders were not successfully initialized.");
		return(false);
	}

	// TODO Ausgabestring
//	fillInfluenceList();
	
// ----- END OF INITIALIZING DATABASE -----

	return(true);
}

void Main::reloadOriginalSize()
{
	backGround->reloadOriginalSize();
}

void Main::reloadStrings()
{
	backGround->reloadStrings();
}

const bool Main::isIntro() const
{
	return((introWindow->isShown()) || (languageMenu->isShown()));
}

void Main::initializeGame(const unsigned int tab_number)
{
	toInitLog(UI_Object::theme.lookUpString(START_PREPARE_FIRST_RUN_STRING));
	// erstes freies suchen:
	signed int game_number;
	unsigned int game_nr = 0;
	unsigned int game_max = 0;
	for(game_number = 0;game_number < MAX_GAME; ++game_number)
		if(game[game_number]==NULL)
			break;
	if(game_number == MAX_GAME)
		return;

	toInitLog("* " + UI_Object::theme.lookUpString(START_CHOOSING_GAME_TAB_STRING));

	if(tabToGameList[tab_number]>=0)
	{
#ifdef _SCC_DEBUG
		if(tabToSplitGameList[tab_number]>=0)
		{
			toErrorLog("ERROR: WTF!.");return;
			// => schon besetzt! Fehler!
		} else
#endif    
		{
			game_nr = 0;
			game_max = 2;
			game[tabToGameList[tab_number]]->setMode(1, game_max);
			tabToSplitGameList[tab_number] = game_number;
			currentGame = 1;
			// => als split window hernehmen!
		}
	} else
	{
	// => new tab
		game_nr = 0;
		game_max = 1;
		tabToGameList[tab_number] = game_number;
		mainMenuLine->addNewGameTab();
		currentGame = 0;
//		mainMenuLine->forcePressTab(tab_number);
	}
	toInitLog("* " + UI_Object::theme.lookUpString(START_CREATING_GAME_STRING));
	game[game_number] = new Game(backGround, database.getMap(0), game_nr, game_max); // TODO
//	game[game_number]->assignStartCondition(1, database.getStartCondition(TERRA, 0)); // <- evtl auswaehlen... jo, aber erst spaeter einbauen TODO
//	game[game_number]->setStartRace(1, TERRA); // <- ok
	game[game_number]->setStartPosition(0, 1); // <- TODO
//	game[game_number]->fillGroups();
//	game[game_number]->assignGoal(1, database.getGoal(TERRA, 0)); // <- default goal
	toInitLog("* " + UI_Object::theme.lookUpString(START_ASSIGNING_HARVEST_SPEED_STRING));
//	game[game_number]->setHarvestSpeed(0, TERRA, database.getHarvestSpeed(TERRA, 0));
//	game[game_number]->setHarvestSpeed(0, PROTOSS, database.getHarvestSpeed(PROTOSS, 0));
//	game[game_number]->setHarvestSpeed(0, ZERG, database.getHarvestSpeed(ZERG, 0));

	toInitLog("* " + UI_Object::theme.lookUpString(START_SHOWING_GAME_STRING));
//	game[game_number]->Show();
}


Main::~Main()
{
	toInitLog("* " + UI_Object::theme.lookUpString(END_FREEING_WINDOWS_STRING));
	delete mainMenuLine;
	delete introWindow;
	delete languageMenu;
	delete text;
	delete ForceWindow::techTreeWindow;
//	delete msgWindow;
	delete helpWindow;
	delete settingsWindow;
	delete dataBaseWindow;
	delete debugWindow;
//	delete mapWindow;
	delete backGround;
	delete saveBox;
	toInitLog("* " + UI_Object::theme.lookUpString(END_FREEING_GAMES_STRING));
	for(unsigned int i=MAX_GAME;i--;)
		delete game[i];
}

void Main::resetData()
{
	for(unsigned int i=MAX_GAME;i--;)
		if(game[i])
			game[i]->resetData();
}

void Main::goBack()
{
//	if(helpWindow->isShown())
//		helpWindow->goBack();
}

void Main::noticeFullscreen()
{
	if(settingsWindow)
	{
		settingsWindow->updateItems();
		settingsWindow->forceFullScreenChange();
	}
}

void Main::resetDataChange()
{
	if(settingsWindow)
		settingsWindow->resetDataChange();
	gameTypeHasChanged = false;
}

void Main::process()
{
	setMouse(maus);
	UI_Window::gotoHelpChapter = -1;
	UI_Object::windowSelected = false;

	BoEntry::entryIsMoving = false;
	debugWindow->updateData(frameRateControl.getPercentList(), frameRateControl.getFramesPerSecond(), frameRateControl.getGenerationsPerSecond());
	UI_Object::processAll();
// ------ PROCESSING ------
//	if((efConfiguration.isToolTips())&&(UI_Object::tooltip))
//		UI_Object::tooltip->process();

	if((!mainMenuLine->isShown()) && (!isIntro()))
	{
		mainMenuLine->Show();
		debugWindow->Show(efConfiguration.isShowDebug());
	}
// ------ PROCESSING ------
	checkTab();

	if(!UI_Object::windowSelected)
		UI_Object::currentWindow=NULL;

	if(saveBox == NULL)
	{
		if(UI_Window::needSaveBox)
			saveBox = new SaveBox(NULL, UI_Window::saveBoxParameter);
	} else
	{
		bool is_canceled = saveBox->isCanceled();
		bool is_done = saveBox->isDone();
		if(is_canceled || is_done)
		{
			if(is_canceled)
				UI_Window::saveBoxCanceled = true;
			else if(is_done)
			{
				UI_Window::saveBoxString = saveBox->getString();
				UI_Window::saveBoxDone = true;
			}
			delete saveBox;
			saveBox = NULL;
			UI_Button::resetButton();
			UI_Window::focus = NULL;
			UI_Window::needSaveBox = false;
		}
	}
	

	
	{
		int pressed = -1;
		if(((pressed = languageMenu->getPressedItem())>=0))
		{
			UI_Object::theme.setLanguage((eLanguage)(pressed+1));
			uiConfiguration.setFirstStart(false);
		}
	}
	
	{
		unsigned int pressed = 0;
		if((pressed = introWindow->getGameType())>0)
		{
			if(coreConfiguration.setGameType(pressed))
			{
				gameTypeHasChanged = true;
				UI_Button::resetButton();
			}
		}
	}
	
	if(settingsWindow)
	{
		if(settingsWindow->hasCompactDisplayModeChanged())
			for(unsigned int i = MAX_GAME; i--;)
				if(game[i])
					game[i]->compactDisplayModeHasChanged();
		if(settingsWindow->hasLanguageChanged())
		{
			backGround->reloadStrings();
			UI_Object::addMessage(LANGUAGE_HAS_CHANGED_STRING);
		}

		if(settingsWindow->hasAllowWaitOrdersChanged())
		{
			for(unsigned int i = MAX_GAME; i--;)
				if(game[i])
					game[i]->setResetFlag();
			UI_Window::setChangedFlag();
		}

	/*	if(settingsWindow->hasGameSpeedChanged())
		{
			for(unsigned int i = MAX_GAME; i--;)
				if(game[i])
					game[i]->resetPlayerTime();
		}*/ // TODO see settings.hpp
		if(UI_Window::gotoHelpChapter>=0)
		{
			mainMenuLine->activateTab(HELP_TAB);
//			helpWindow->gotoChapter(UI_Window::gotoHelpChapter);
		}
	}

	for(std::list<std::string>::iterator i = UI_Object::msgList.begin(); i!= UI_Object::msgList.end(); ++i)
	{
		UI_Object::addMessage(*i);
		i = UI_Object::msgList.erase(i);
	}
	
	database.changeAccepted();

	frameRateControl.updateConfiguration();
	{
	/*		std::ostringstream os;
			os << uiConfiguration.getDesiredFramerate();
			toInitLog("* " + UI_Object::theme.lookUpString(START_SET_DESIRED_FRAMERATE_STRING) + " " + os.str());
		}
	
		{
			std::ostringstream os;
			os << uiConfiguration.getDesiredCPU();
			toInitLog("* " + UI_Object::theme.lookUpString(START_SET_DESIRED_CPU_STRING) + " " + os.str() + "%");*/ // TODO
	}

	
}

void Main::checkTab()
{
	if(mainMenuLine->hasTabChanged())
	{
		currentTab = mainMenuLine->getCurrentTab();
		switch(currentTab)
		{
			case -1:break;
			case DATABASE_TAB:
				for(unsigned int i = MAX_GAME; i--;) 
					if(game[i]!=NULL) game[i]->Hide();
//				msgWindow->Show();
//				msgWindow->adjustRelativeRect(Rect(Point(5, UI_Object::theme.lookUpGlobalRect(MESSAGE_WINDOW).getTop()), UI_Object::theme.lookUpGlobalRect(MESSAGE_WINDOW).getSize()));
//				debugWindow->adjustRelativeRect(Rect(Point(msgWindow->getTargetRect().getRight()+5, UI_Object::theme.lookUpGlobalRect(DEBUG_WINDOW).getTop()), UI_Object::theme.lookUpGlobalRect(DEBUG_WINDOW).getSize()));
				settingsWindow->Hide();
				dataBaseWindow->Show();
//				mapWindow->Hide();
//				helpWindow->Hide();
			break;
/*			case MAP_TAB:
				for(unsigned int i = MAX_GAME; i--;)
					if(game[i]!=NULL) game[i]->Hide();
				msgWindow->Show();
				settingsWindow->Hide();
				dataBaseWindow->Hide();
				mapWindow->Show();
				helpWindow->Hide();
			break;*/
			case SETTINGS_TAB:
				for(unsigned int i = MAX_GAME; i--;) 
					if(game[i]!=NULL) game[i]->Hide();
//				msgWindow->Hide();
				debugWindow->adjustRelativeRect(Rect(Point(5, UI_Object::theme.lookUpGlobalRect(DEBUG_WINDOW).getTop()), UI_Object::theme.lookUpGlobalRect(DEBUG_WINDOW).getSize()));
				settingsWindow->Show();
				settingsWindow->updateItems();
				dataBaseWindow->Hide();
//				mapWindow->Hide();
//				helpWindow->Hide();
			break;
			case HELP_TAB:
				for(unsigned int i = MAX_GAME; i--;) 
					if(game[i]!=NULL) game[i]->Hide();
//				msgWindow->Hide();
				debugWindow->Hide();
				settingsWindow->Hide();
				dataBaseWindow->Hide();
//				mapWindow->Hide();
//				helpWindow->Show();
			break;
			default:
				for(unsigned int i = MAX_GAME; i--;) 
					if(game[i]!=NULL) game[i]->Hide();
//				msgWindow->Show();
//				msgWindow->adjustRelativeRect(UI_Object::theme.lookUpGlobalRect(MESSAGE_WINDOW));
				debugWindow->adjustRelativeRect(UI_Object::theme.lookUpGlobalRect(DEBUG_WINDOW));
				settingsWindow->Hide();
				dataBaseWindow->Hide();
//				mapWindow->Hide();
//				helpWindow->Hide();
				if(tabToGameList[currentTab]>=0)
				{
					game[tabToGameList[currentTab]]->Show(); //...
					if(tabToSplitGameList[currentTab]>=0)
						game[tabToSplitGameList[currentTab]]->Show();
				}
	//			else // new game!
	//				initializeGame(currentTab);
			break;
		} // end switch getCurrentTabs
//		msgWindow->makeFirstChild();
	}
	
	if((tabToGameList[currentTab]>=0)&&(game[tabToGameList[currentTab]]->isSplitGame())&&(tabToSplitGameList[currentTab]==-1))
	{
		initializeGame(currentTab);
//		msgWindow->makeFirstChild();
	}

	if(mainMenuLine->markedForRemove())
	{
		if(tabToSplitGameList[currentTab]>=0)
		{
			delete game[tabToSplitGameList[currentTab]];
			game[tabToSplitGameList[currentTab]] = NULL;
			tabToSplitGameList[currentTab] = -1;
		}
		if(tabToGameList[currentTab]>=0)
		{
			delete game[tabToGameList[currentTab]];
			game[tabToGameList[currentTab]] = NULL;
			tabToGameList[currentTab] = -1;
		}
		UI_Object::currentWindow = NULL;
		UI_Object::windowSelected = false;
		currentGame = 0;
		mainMenuLine->removeGameTab(currentTab);
	} else
	if(mainMenuLine->markedForNewGame())
	{
		initializeGame(currentTab);
	} else
	{
		if((tabToGameList[currentTab]>=0)&&(game[tabToGameList[currentTab]]->isRemoveGame()))
		{
			if(tabToSplitGameList[currentTab]>=0) // delete just the first game, move second game to the first
			{
				delete game[tabToGameList[currentTab]];
				game[tabToGameList[currentTab]]=NULL;
				tabToGameList[currentTab] = tabToSplitGameList[currentTab];
				tabToSplitGameList[currentTab]=-1;
				game[tabToGameList[currentTab]]->setMode(0,1);
				UI_Object::currentWindow = NULL;
				UI_Object::windowSelected = false;
				currentGame = 0;
			} else if(mainMenuLine->getGameTabCount()>1) // delete the whole tab if it's not the last
			{
				delete game[tabToGameList[currentTab]];
				game[tabToGameList[currentTab]]=NULL;
				for(unsigned int i = currentTab;i < mainMenuLine->getGameTabCount()-1;++i)
					tabToGameList[i] = tabToGameList[i+1];
				tabToGameList[mainMenuLine->getGameTabCount()-1]=-1;
				mainMenuLine->removeGameTab(currentTab);
				if(currentTab == (signed int)(mainMenuLine->getGameTabCount()))
					currentTab = (eTabs)(currentTab - 1);
				mainMenuLine->activateTab(currentTab);
				game[tabToGameList[currentTab]]->Show(); //...
				if(tabToSplitGameList[currentTab]>=0)
					game[tabToSplitGameList[currentTab]]->Show();
	//			if(mainMenuLine->getGameTabCount()) TODO show/hide removeButton;
				UI_Object::currentWindow = NULL;
				UI_Object::windowSelected = false;
				currentGame = 0;
			}
		} else if((tabToSplitGameList[currentTab]>=0)&&(game[tabToSplitGameList[currentTab]]->isRemoveGame())) // just the second game
		{
			delete game[tabToSplitGameList[currentTab]];
			game[tabToSplitGameList[currentTab]]=NULL;
			game[tabToGameList[currentTab]]->setMode(0,1);
			tabToSplitGameList[currentTab]=-1;
			UI_Object::currentWindow = NULL;
			UI_Object::windowSelected = false;
			currentGame = 0;
		}
	}
}

void Main::stopAllOptimizing()
{
	for(unsigned int i=MAX_GAME;i--;)
		if((game[i])&&(game[i]->isShown()))
			game[i]->stopOptimizing();
}

void Main::startLastOptimizing()
{
	for(unsigned int i=MAX_GAME;i--;)
		if((game[i])&&(game[i]->isShown()))
			game[i]->startLastOptimizing();
}

const bool Main::isAnyOptimizing() const
{
	for(unsigned int i=MAX_GAME;i--;)
		if((game[i])&&(game[i]->isShown())&&(game[i]->isAnyOptimizing()))
			return(true);
	return(false);
}

void Main::draw() const
{
	UI_Object::redrawnObjects = 0;

	UI_Object::updateScreen();	

	UI_Object::theme.setColorTheme(UI_Object::theme.getMainColorTheme());
/*	if(languageMenu->isShown())
	{
//		dc->setBrush(*UI_Object::theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH));
//		dc->setPen(*UI_Object::theme.lookUpPen(NULL_PEN));
//		dc->DrawRectangle(mainMenuLine->getAbsoluteRect());
	}*/
}

										
//settings: log level (none, debug only, +final result, +result of each run, +snapshot every X generations, +snapshot every generation, EVERYTHING (~2MB/generation!)

void Main::newGeneration()
{
	frameRateControl.delay(isAnyOptimizing()); 
	poll(IDLE_TICKS);

	if(isIntro())
		return;
//	evtl savebox oder so
//	if(UI_Object::focus==NULL) // TODO, bei Force/Bowindow abfragen
	while(frameRateControl.allowCalculation())
	{
		if((!BoEntry::entryIsMoving) || (!efConfiguration.isWaitAfterChange()))
		{
			for(unsigned int i=MAX_GAME;i--;)
				if((game[i])&&(game[i]->isShown()))
					game[i]->newGeneration();
			if(dataBaseWindow->isShown())
				dataBaseWindow->newGeneration();
		} else SDL_Delay(50);
	}
	poll(GENERATION_TICKS);
}


const bool Main::newRun()
{
//	endrun=true; TODO
/*	if(efConfiguration.isAutoSaveRuns()) TODO
	{
		ostringstream os, os2;
		os << anarace[0]->getRun() << UI_Object::theme.lookUpString(ENDRUN_FINISHED_STRING) << "[" << formatTime2(anarace[0]->getRealTimer()) << "]";
		addMessage(os.str());
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
		addMessage(UI_Object::theme.lookUpString(ENDRUN_SAVED_BUILDORDER_STRING));
		endrun=false;
	} else
	{
		ostringstream os;
		os << "bo_" << (*anarace[0]->getCurrentGoal())->getName() << "_" << formatTime2(anarace[0]->getRealTimer()) << "_" << anarace[0]->getRun();

		if(UI_Object::editTextField==NULL)
			UI_Object::editTextField = new UI_EndRunDialog(mainMenuLine, os.str());
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
					addMessage(os.str());
					database.saveBuildOrder(UI_Object::editTextField->getString(), anarace[0]); // TODO, evtl alle saven...
					addMessage(UI_Object::theme.lookUpString(ENDRUN_SAVED_BUILDORDER_STRING));
				}
				delete UI_Object::editTextField;
				UI_Object::editTextField=NULL;
				endrun = false;
			} else
			if((UI_Object::editTextField->isCanceled())&&(UI_Object::editTextField->getCaller()==NULL))
			{
				delete UI_Object::editTextField;
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

/*const bool Main::loadHarvestData()
{
	toInitLog("* " + UI_Object::theme.lookUpString(START_LOAD_HARVEST_STRING));
	std::list<std::string> harvestFiles = findFiles("settings", "harvest", "", "");
	for(std::list<std::string>::iterator j = harvestFiles.begin(); j!=harvestFiles.end(); ++j)
		FILES::loadHarvestFile(*j);
	return(isHarvestDataInitialized());
}*/

const bool Main::loadStartConditions()
{
	toInitLog("* " + UI_Object::theme.lookUpString(START_LOAD_STARTCONDITIONS_STRING));
	for(unsigned int i = 0; i < GAME::MAX_RACES; ++i)
	{
		std::list<std::string> startFiles = findFiles(GAME::gameDirectory, "start", GAME::race[i].raceString, "");
		for(std::list<std::string>::iterator j = startFiles.begin(); j!=startFiles.end(); ++j)
			FILES::loadStartConditionFile(*j);
	}
	return(database.isStartConditionDataInitialized());
}

const bool Main::loadMaps()
{
	toInitLog("* " + UI_Object::theme.lookUpString(START_LOAD_MAPS_STRING));
	std::list<std::string> mapFiles = findFiles(GAME::gameDirectory, "maps", "", "");
	for(std::list<std::string>::iterator j = mapFiles.begin(); j!=mapFiles.end(); ++j)
		FILES::loadMapFile(*j);
	return(database.isMapDataInitialized());
}

const bool Main::loadGoals()
{
	toInitLog("* " + UI_Object::theme.lookUpString(START_LOAD_GOALS_STRING));
	for(unsigned int i = 0; i < GAME::MAX_RACES; ++i)
	{
		database.addDefaultGoal(i);
		std::list<std::string> goalFiles = findFiles(GAME::gameDirectory, "goals", GAME::race[i].raceString, "");
		for(std::list<std::string>::iterator j = goalFiles.begin(); j!=goalFiles.end(); ++j)
			if(!FILES::loadGoalFile(*j))
				toErrorLog("WARNING (Main::loadGoals()): Goal file " + *j + " could not be loaded => file is ignored.");
	}
	return(database.isGoalDataInitialized());
}

const bool Main::loadBuildOrders()
{
	toInitLog("* " + UI_Object::theme.lookUpString(START_LOAD_BUILD_ORDERS_STRING));
	for(unsigned int i = 0; i < GAME::MAX_RACES; ++i)
	{
//		database.addDefaultBuildOrder(i); :/ TODO
		std::list<std::string> boFiles = findFiles(GAME::gameDirectory, "bos", GAME::race[i].raceString, "");
		for(std::list<std::string>::iterator j = boFiles.begin(); j!=boFiles.end(); ++j)
			FILES::loadBuildOrderFile(*j);
	}
	return(database.isBuildOrderDataInitialized());
}


// ------ EVENTS ------

void Main::setMouse(const Point p)
{
	if((p == UI_Object::mouse) && (!UI_Button::wasResetted))
		return;
	UI_Button::wasResetted = false;
	maus = p;
	UI_Object::mouse = p;
	
	if(UI_Button::isCurrentButtonHasAlreadyLeft())
	{
		if((UI_Button::getCurrentButton()!=NULL)&&(UI_Button::getCurrentButton()->getAbsoluteRect().isTopLeftCornerInside(p)))
		{
			UI_Button::getCurrentButton()->mouseHasEnteredArea();
			UI_Button::setCurrentButtonHasAlreadyLeft(false);
		}
		else return;
	}
	
	if((UI_Button::getCurrentButton())&&(!UI_Button::getCurrentButton()->getAbsoluteRect().isTopLeftCornerInside(p)))
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
			temp_button = backGround->checkHighlight();
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
// first we have to check the object with checkToolTip.
// The result is either the object itself or one of its children that owns a tooltip
			
			UI_Object* temp=UI_Object::toolTipParent;
			UI_Object* temp2 = NULL;
			UI_Object::toolTipParent = NULL;

			temp2=backGround->checkToolTip();
			if(temp2 != NULL)
				UI_Object::toolTipParent = temp2;
// toolTipParent changed or tooltip has to be deleted?
			if(((UI_Object::toolTipParent != temp) || (UI_Object::tooltip == NULL)))
			{
				delete UI_Object::tooltip;
				if(UI_Object::toolTipParent==NULL)
					UI_Object::tooltip=NULL;
				else 
				{
					if(UI_Object::toolTipParent->getToolTipEString()!=NULL_STRING)
						UI_Object::tooltip = new UI_ToolTip(backGround, UI_Object::toolTipParent->getToolTipEString());
					else
						UI_Object::tooltip = new UI_ToolTip(backGround, UI_Object::toolTipParent->getToolTipString());
				}
			}

		}
	}

	if((!efConfiguration.isToolTips()) && (UI_Object::tooltip))
	{
		delete UI_Object::tooltip;
		UI_Object::tooltip=NULL;
	}
}


void Main::leftDown()
{
	if(UI_Button::wasResetted)
		return;
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
	if(UI_Button::wasResetted)
		return;
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

void Main::wheelToTop()
{
	if(UI_Object::currentWindow)
		UI_Object::currentWindow->moveScrollBarToTop();
}

void Main::wheelToBottom()
{
	if(UI_Object::currentWindow)
		UI_Object::currentWindow->moveScrollBarToBottom();
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

const bool Main::openMenu(const ePlayerOrder order)
{
#if 0
	// links?
	if(currentGame==0)
	{
		if(tabToGameList[currentTab]<0)
			return(false);
		if(!game[tabToGameList[currentTab]]->openMenu(order))
		{
			if((tabToSplitGameList[currentTab]>=0)&&(game[tabToSplitGameList[currentTab]]->openMenu(order)))
			{				
				currentGame = 1;
				return(true);
			}
			else return(false);
		} else return(true);
	} // oder rechts?
	else if(currentGame==1)
	{
		if((tabToSplitGameList[currentTab]==-1)||(!game[tabToSplitGameList[currentTab]]->openMenu(order)))
		{
			if((tabToGameList[currentTab]) && (game[tabToGameList[currentTab]]->openMenu(order)))
			{
				currentGame = 0;
				return(true);
			} else return(false);
		} 
		return(false);
	}
#endif 
	return(false);
}
// ------ END EVENTS ------

//InfoWindow* Main::infoWindow = NULL;
//MessageWindow* Main::msgWindow = NULL;
