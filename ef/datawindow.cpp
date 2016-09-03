#include "datawindow.hpp"

#include "../core/configuration.hpp"
#include "../ui/configuration.hpp"
#include "configuration.hpp"
#include <sstream>

DataBaseWindow::DataBaseWindow(UI_Object* mapwindow_parent):
	UI_Window(mapwindow_parent, DATABASE_WINDOW_TITLE_STRING, theme.lookUpGlobalRect(DATABASE_WINDOW), theme.lookUpGlobalMaxHeight(DATABASE_WINDOW), NOT_SCROLLED)
	//	map(new BASIC_MAP()),
//	mapSettings(new UI_Group(this, Rect(getRelativeClientRectPosition() + Point(15, 35), Size(0,0)), Size(0, 5), TOP_LEFT, MAPWINDOW_MAP_SETTINGS_STRING)),
//	mapName(new UI_EditField(mapSettings, Rect(Point(10, 0), Size(350, FONT_SIZE+6)), Size(0,0), SMALL_BOLD_FONT, ARRANGE_LEFT, MAPWINDOW_MAP_NAME_STRING, "my_map")),

//	maxPlayer(new UI_NumberField(mapSettings, Rect(Point(10, 0), Size(3*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)/2, 15)), Size(5,5), ARRANGE_LEFT, 1, 5, MAPWINDOW_MAX_PLAYER_STRING, MAPWINDOW_MAX_PLAYER_TOOLTIP_STRING, 1, 1)),
//	maxLocations(new UI_NumberField(mapSettings, Rect(Point(10, 0), Size(3*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)/2, 15)), Size(5,5), ARRANGE_LEFT,  MIN_LOCATIONS, MAX_LOCATIONS, MAPWINDOW_MAX_LOCATIONS_STRING, MAPWINDOW_MAX_LOCATIONS_TOOLTIP_STRING, 1, 2)),
//	symmetryButton(new UI_CheckButton(mapSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, MAPWINDOW_SYMMETRY_STRING, MAPWINDOW_SYMMETRY_TOOLTIP_STRING, true)),
	
//	locationSettings(new UI_Group(this, Rect(Point(15, 0), Size(350,0)), Size(0, 5), CENTER_LEFT, MAPWINDOW_LOCATION_SETTINGS_STRING)),
//	playerSettings(new UI_Radio(this, Rect(Point(15, 0), Size(350,0)), Size(0, 5), BOTTOM_LEFT, MAPWINDOW_PLAYER_SETTINGS_STRING)),

//	locationMenu(new LocationMenu(this, map, Rect(Point(100,200), Size(200, 300)))),
//	menuRadio(new UI_Radio(this, Rect(getRelativeClientRectPosition() + Point(105, 10), Size(0,0)), Size(0,0), ARRANGE_TOP_RIGHT )),

	
//	locationContents(new UI_Group(this, Rect(Point(15, 0), Size(350,0)), Size(0, 5), CENTER_RIGHT, MAPWINDOW_LOCATION_CONTENT_STRING)),
//	mineralBlocks(new UI_NumberField(locationContents, Rect(Point(10, 0), Size(3*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)/2, 15)), Size(5,5), ARRANGE_LEFT, 1, 5, MAPWINDOW_MINERAL_BLOCKS_STRING, MAPWINDOW_MINERAL_BLOCKS_TOOLTIP_STRING, 1, 1)),
//	vespeneGeysirs(new UI_NumberField(locationContents, Rect(Point(10, 0), Size(3*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)/2, 15)), Size(5,5), ARRANGE_LEFT, 1, 5, MAPWINDOW_VESPENE_GEYSIRS_STRING, MAPWINDOW_VESPENE_GEYSIRS_TOOLTIP_STRING, 1, 1)),
//	mineralDistance(new UI_NumberField(locationContents, Rect(Point(10, 0), Size(3*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)/2, 15)), Size(5,5), ARRANGE_LEFT, 1, 5, MAPWINDOW_MINERAL_DISTANCE_STRING, MAPWINDOW_MINERAL_DISTANCE_TOOLTIP_STRING, 1, 1))	
#if 0
	uiSettingsRadio(new UI_Radio(this, Rect(Point(0, 0), Size(0, 0)), Size(10, 5), CENTER_RIGHT, SETWINDOW_UI_SETTINGS_STRING)), 
	loadSaveSettings(new UI_Group(this, Rect(Point(0, 0), Size(300, 0)), Size(10, 5), BOTTOM_RIGHT, SETWINDOW_GUI_SETTINGS_STRING/*SETWINDOW_LOADSAVE_SETTINGS_STRING*/)),
	defaultSettingsRadio(new UI_Radio(this, Rect(Point(0, 35), Size(300, 0)), Size(10, 0), TOP_RIGHT, TITLE_PREDEFINED_SETTINGS_STRING)),
	
	maxRuns(new UI_NumberField(coreSettings, Rect(Point(10, 0), Size(3*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)/2, 15)), Size(5,5), ARRANGE_LEFT,  MIN_RUNS, MAX_RUNS, 1, coreConfiguration.getMaxRuns(), SETTING_MAX_RUNS_STRING, SETTING_MAX_RUNS_TOOLTIP_STRING)),

	maxGenerations(new UI_NumberField(coreSettings, Rect(Point(10, 0), Size(3*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)/2, 15)), Size(5,5), ARRANGE_LEFT,  MIN_GENERATIONS, MAX_GENERATIONS, 10, coreConfiguration.getMaxGenerations(), SETTING_MAX_GENERATIONS_STRING, SETTING_MAX_GENERATIONS_TOOLTIP_STRING)),
	maxTimeOut(new UI_NumberField(coreSettings, Rect(Point(10, 0), Size(3*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)/2, 15)), Size(5,5), ARRANGE_LEFT,  MIN_TIMEOUT, MAX_TIMEOUT, 1, coreConfiguration.getMaxTimeOut(), SETTING_MAX_TIMEOUT_STRING, SETTING_MAX_TIMEOUT_TOOLTIP_STRING)),
	breedFactor(new UI_NumberField(coreSettings, Rect(Point(10, 0), Size(3*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)/2, 15)), Size(5,5), ARRANGE_LEFT,  MIN_BREED_FACTOR, MAX_BREED_FACTOR, 1, coreConfiguration.getBreedFactor(), SETTING_BREED_FACTOR_STRING, SETTING_BREED_FACTOR_TOOLTIP_STRING, PERCENT_NUMBER_TYPE)),
	//crossingOver(new UI_NumberField(coreSettings, Rect(Point(10, 90), Size(200, 15)), Size(5, 5), ARRANGE_LEFT,  MIN_CROSSING_OVER, MAX_CROSSING_OVER, 1, coreConfiguration.getCrossingOver(), SETTING_CROSSING_OVER_STRING, SETTING_CROSSING_OVER_TOOLTIP_STRING, PERCENT_NUMBER_TYPE)),

//TODO
	alwaysBuildWorker(new UI_CheckButton(coreSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_ALWAYS_BUILD_WORKER_STRING, SETTING_ALWAYS_BUILD_WORKER_TOOLTIP_STRING, coreConfiguration.isAlwaysBuildWorker())),
	onlySwapOrders(new UI_CheckButton(coreSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_ONLY_SWAP_ORDERS_STRING, SETTING_ONLY_SWAP_ORDERS_TOOLTIP_STRING, coreConfiguration.isOnlySwapOrders())),

	restrictSC(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_RESTRICT_SC_STRING, SETTING_RESTRICT_SC_TOOLTIP_STRING, efConfiguration.isRestrictSC())),
 	facilityMode(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_FACILITY_MODE_STRING, SETTING_FACILITY_MODE_TOOLTIP_STRING, efConfiguration.isFacilityMode())),
//	preprocessBuildorder(new UI_CheckButton(coreSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_PREPROCESS_BUILDORDER_STRING, SETTING_PREPROCESS_BUILDORDER_TOOLTIP_STRING, coreConfiguration.isPreprocessBuildOrder())),
//	allowGoalAdaption(new UI_CheckButton(coreSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_ALLOW_GOAL_ADAPTION_STRING, SETTING_ALLOW_GOAL_ADAPTION_TOOLTIP_STRING, coreConfiguration.isAllowGoalAdaption())),

 	glowingButtons(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_GLOWING_BUTTONS_STRING, SETTING_GLOWING_BUTTONS_TOOLTIP_STRING, uiConfiguration.isGlowingButtons())),
 	dnaSpiral(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_DNA_SPIRAL_STRING, SETTING_DNA_SPIRAL_TOOLTIP_STRING, efConfiguration.isDnaSpiral())),
 	backgroundBitmap(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_BACKGROUND_BITMAP_STRING, SETTING_BACKGROUND_BITMAP_TOOLTIP_STRING, efConfiguration.isBackgroundBitmap())),
 	fullscreen(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_FULLSCREEN_STRING, SETTING_FULLSCREEN_TOOLTIP_STRING, efConfiguration.isFullScreen())),
 	tooltips(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_TOOLTIPS_STRING, SETTING_TOOLTIPS_TOOLTIP_STRING, efConfiguration.isToolTips())),
//	softwareMouse(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_SOFTWARE_MOUSE_STRING, SETTING_SOFTWARE_MOUSE_TOOLTIP_STRING, efConfiguration.isSoftwareMouse())),
//	transparency(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_TRANSPARENCY_STRING, SETTING_TRANSPARENCY_TOOLTIP_STRING, uiConfiguration.isTransparency())),
	smoothMovement(new UI_CheckButton(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5, 5), ARRANGE_LEFT, SETTING_SMOOTH_MOVEMENT_STRING, SETTING_SMOOTH_MOVEMENT_TOOLTIP_STRING, uiConfiguration.isSmoothMovements())),
	desiredFramerate(new UI_NumberField(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5,5), ARRANGE_LEFT,  EF_Configuration::MIN_DESIRED_FRAMERATE, EF_Configuration::MAX_DESIRED_FRAMERATE, 1, efConfiguration.getDesiredFramerate(), SETTING_DESIRED_FRAMERATE_STRING, SETTING_DESIRED_FRAMERATE_TOOLTIP_STRING)),
 	desiredCPU(new UI_NumberField(guiSettings, Rect(Point(10, 0), Size(200, 15)), Size(5,5), ARRANGE_LEFT,  EF_Configuration::MIN_CPU_USAGE, EF_Configuration::MAX_CPU_USAGE, 1, efConfiguration.getDesiredCPU(), SETTING_DESIRED_CPU_USAGE_STRING, SETTING_DESIRED_CPU_USAGE_TOOLTIP_STRING, PERCENT_NUMBER_TYPE)),

	minimalistButton(new UI_Button(defaultSettingsRadio, Rect(Point(0, 0), Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0)), Size(5,5), MY_BUTTON, false, TAB_BUTTON_MODE, SETTING_MINIMALIST_STRING, ARRANGE_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH/*AUTO_SIZE_ONCE*/)),
	fullButton(new UI_Button(defaultSettingsRadio, Rect(Point(0, 0), Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0)), Size(5,5), MY_BUTTON, false, TAB_BUTTON_MODE, SETTING_FULL_STRING, ARRANGE_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH/*AUTO_SIZE_ONCE*/)),
	customButton(new UI_Button(defaultSettingsRadio, Rect(Point(0, 0), Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0)), Size(5,5), MY_BUTTON, false, TAB_BUTTON_MODE, SETTING_CUSTOM_STRING, ARRANGE_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH/*AUTO_SIZE_ONCE*/)),
	
	reloadFromFileButton(new UI_Button(loadSaveSettings, Rect(Point(0, 0), Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0)), Size(5,5), MY_BUTTON, false, PRESS_BUTTON_MODE, SETTING_RELOAD_FROM_FILE_STRING, ARRANGE_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH/*AUTO_SIZE_ONCE*/)),
	loadFailsafeDefaultsButton(new UI_Button(loadSaveSettings, Rect(Point(0, 0), Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0)), Size(5,5), MY_BUTTON, false, PRESS_BUTTON_MODE, SETTING_LOAD_FAILSAFE_DEFAULTS_STRING, ARRANGE_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH/*AUTO_SIZE_ONCE*/)),
	saveToFileButton(new UI_Button(loadSaveSettings, Rect(Point(0, 0), Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0)), Size(5,5), MY_BUTTON, false, PRESS_BUTTON_MODE, SETTING_SAVE_TO_FILE_STRING, ARRANGE_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH/*AUTO_SIZE_ONCE*/)),
	
/*	languageMenuButton(new UI_Button(uiSettingsRadio, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(0, 0), MY_BUTTON, false, STATIC_BUTTON_MODE, SETTING_LANGUAGE_STRING, ARRANGE_TOP_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	resolutionMenuButton(new UI_Button(uiSettingsRadio, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(0, 0), MY_BUTTON, false, STATIC_BUTTON_MODE, SETTING_RESOLUTION_STRING, ARRANGE_TOP_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	bitDepthMenuButton(new UI_Button(uiSettingsRadio, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(0, 0), MY_BUTTON, false, STATIC_BUTTON_MODE, SETTING_BITDEPTH_STRING, ARRANGE_TOP_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),

	languageMenu(new LanguageMenu(languageMenuButton, Rect(Point(-theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 30),Size(200, 0)), Size(0, 0), TOP_LEFT)),
	resolutionMenu(new ResolutionMenu(resolutionMenuButton, Rect(Point(-theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 30),Size(200, 0)), Size(0, 0), TOP_LEFT)),
	bitDepthMenu(new BitDepthMenu(bitDepthMenuButton, Rect(Point(-theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 30),Size(200, 0)), Size(0, 0), TOP_LEFT)),*/
	

	languageHasChanged(false),
	resolutionHasChanged(false),
	bitDepthHasChanged(false),
	fullScreenHasChanged(false)
#endif
{
#if 0
	languageMenuButton = new UI_Button(uiSettingsRadio, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(0, 0), MY_BUTTON, false, STATIC_BUTTON_MODE, SETTING_LANGUAGE_STRING, ARRANGE_TOP_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
	resolutionMenuButton = new UI_Button(uiSettingsRadio, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(0, 0), MY_BUTTON, false, STATIC_BUTTON_MODE, SETTING_RESOLUTION_STRING, ARRANGE_TOP_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
	bitDepthMenuButton = new UI_Button(uiSettingsRadio, Rect(0, 0, theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0), Size(0, 0), MY_BUTTON, false, STATIC_BUTTON_MODE, SETTING_BITDEPTH_STRING, ARRANGE_TOP_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);

	languageMenu = new LanguageMenu(languageMenuButton, Rect(Point(-theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 30),Size(200, 0)), Size(0, 0), TOP_LEFT);
	resolutionMenu = new ResolutionMenu(resolutionMenuButton, Rect(Point(-theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 30),Size(200, 0)), Size(0, 0), TOP_LEFT);
	bitDepthMenu = new BitDepthMenu(bitDepthMenuButton, Rect(Point(-theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 30),Size(200, 0)), Size(0, 0), TOP_LEFT);
	
	languageMenu->Hide();
	resolutionMenu->Hide();
	bitDepthMenu->Hide();

//	reloadFromFile();


	defaultSettingsRadio->addButton(minimalistButton, 0);
	defaultSettingsRadio->addButton(fullButton, 1);
	defaultSettingsRadio->addButton(customButton, 2);

	uiSettingsRadio->addButton(languageMenuButton, 0);
	uiSettingsRadio->addButton(resolutionMenuButton, 1);
	uiSettingsRadio->addButton(bitDepthMenuButton, 2);

	defaultSettingsRadio->calculateBoxSize();
	coreSettings->calculateBoxSize();
	guiSettings->calculateBoxSize();
	uiSettingsRadio->calculateBoxSize(true);
	loadSaveSettings->calculateBoxSize();

	customButton->updateToolTip(SETTING_CUSTOM_TOOLTIP_STRING);
	fullButton->updateToolTip(SETTING_FULL_TOOLTIP_STRING);
	minimalistButton->updateToolTip(SETTING_MINIMALIST_TOOLTIP_STRING);
	defaultSettingsRadio->forcePress(2);
#endif
/*	for(unsigned int i = 0; i < MAX_LOCATIONS; i++)
	{
		std::ostringstream os;os.str("");
		os << i;
		locationName[i] = new UI_EditField(locationSettings, Rect(Point(10, 0), Size(350, FONT_SIZE+6)), Size(0,0), SMALL_BOLD_FONT, ARRANGE_LEFT, os.str(), "location_name " + os.str());
	}

	for(unsigned int i = 0;i < MAX_LOCATIONS; i++)
	{
		distance[i] = new UI_NumberField(locationContents, Rect(Point(10, 0), Size(3*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)/2, 15)), Size(5,5), ARRANGE_LEFT, 1, 5, "-", MAPWINDOW_DISTANCES_TOOLTIP_STRING, 1, 1);
	}
	for(unsigned int i = 0; i < MAX_INTERNAL_PLAYER; i++)
	{
		std::ostringstream os;os.str("");
		os << i;
		playerStart[i] = new UI_Button(playerSettings, Rect(Point(0, 0), Size(200, FONT_SIZE+6)), Size(0,0), EDIT_FIELD_BUTTON, false, STATIC_BUTTON_MODE, "Player " + os.str() + " start location", SPECIAL_BUTTON_ARRANGE_LEFT, SMALL_BOLD_FONT);
		playerSettings->addButton(playerStart[i], i);
	}
	for(unsigned int i = maxLocations->getNumber();i--;)
	{
		locationName[i]->Show();	
		distance[i]->Show();
	}
	for(unsigned int i = maxLocations->getNumber();i < MAX_LOCATIONS;i++)
	{
		locationName[i]->Hide();
		distance[i]->Hide();
	}
	for(unsigned int i = maxPlayer->getNumber();i--;)
		playerStart[i]->Show();	
	for(unsigned int i = maxPlayer->getNumber();i < MAX_INTERNAL_PLAYER;i++)
		playerStart[i]->Hide();

	mapSettings->calculateBoxSize();
	locationSettings->calculateBoxSize();
	playerSettings->calculateBoxSize();
	locationContents->calculateBoxSize();
	
	mapName->Hide();
	locationMenu->Hide();
	
	menuButton[MAPWINDOW_NEW_BUTTON] = new UI_Button(menuRadio, Rect(Point(5, 5), Size(21,20)), Size(5,5), NEW_BUTTON, true, STATIC_BUTTON_MODE, NULL_STRING, ARRANGE_TOP_LEFT);
	menuButton[MAPWINDOW_RESET_BUTTON] = new UI_Button(menuRadio, Rect(Point(5, 5), Size(21,20)), Size(5,5), REFRESH_BUTTON, true, STATIC_BUTTON_MODE, NULL_STRING, ARRANGE_TOP_LEFT);
	menuButton[MAPWINDOW_LOAD_BUTTON] = new UI_Button(menuRadio, Rect(Point(5, 5), Size(21,20)), Size(5,5), LOAD_BUTTON, true, STATIC_BUTTON_MODE, NULL_STRING, ARRANGE_TOP_LEFT);
	menuButton[MAPWINDOW_SAVE_BUTTON] = new UI_Button(menuRadio, Rect(Point(5,5), Size(21,20)), Size(5,5), SAVE_BUTTON, true, STATIC_BUTTON_MODE, NULL_STRING, ARRANGE_TOP_LEFT),
	
	menuButton[MAPWINDOW_NEW_BUTTON]->updateToolTip(MAPWINDOW_CREATE_NEW_MAP_TOOLTIP_STRING);
	menuButton[MAPWINDOW_RESET_BUTTON]->updateToolTip(MAPWINDOW_RESET_MAP_TOOLTIP_STRING);
	menuButton[MAPWINDOW_LOAD_BUTTON]->updateToolTip(MAPWINDOW_LOAD_MAP_TOOLTIP_STRING);
	menuButton[MAPWINDOW_SAVE_BUTTON]->updateToolTip(MAPWINDOW_SAVE_MAP_TOOLTIP_STRING);

	menuRadio->addButton(menuButton[MAPWINDOW_NEW_BUTTON], 0);
	menuRadio->addButton(menuButton[MAPWINDOW_RESET_BUTTON], 1);
	menuRadio->addButton(menuButton[MAPWINDOW_LOAD_BUTTON], 2);
	menuRadio->addButton(menuButton[MAPWINDOW_SAVE_BUTTON], 3);
	menuRadio->calculateBoxSize(true);
	addHelpButton(DESCRIPTION_MAP_WINDOW_CHAPTER);*/


/*	game = new Game(getParent(), 0, 2)

			
	unsigned int game_max = 0;
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
//			if(is_compare_mode)
//			{
//				compareWindow initialisieren (links)
//				...
//				oder doch eigenes? ueber tabs compare? 
//				naja... vergleicht ja nicht mit aktuellem game
//				sondern schmeisst das game raus und guggt sich gespeicherte spiele an...
				ok -> auf comparetab!
//			}
			
			game_nr = 0;
			game_max = 2;
			game[tabToGameList[tab_number]]->setMode(1, game_max);
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
}*/



}

void DataBaseWindow::reloadOriginalSize()
{
	setOriginalRect(theme.lookUpGlobalRect(DATABASE_WINDOW));
	setMaxHeight(theme.lookUpGlobalMaxHeight(DATABASE_WINDOW));

#if 0
	languageMenuButton->setOriginalSize(Size(theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0));
	resolutionMenuButton->setOriginalSize(Size(theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0));
	bitDepthMenuButton->setOriginalSize(Size(theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0));
	
	reloadFromFileButton->setOriginalSize(Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0));
	loadFailsafeDefaultsButton->setOriginalSize(Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0));
	saveToFileButton->setOriginalSize(Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0));

	languageMenu->setOriginalPosition(Point(-theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 30));
	resolutionMenu->setOriginalPosition(Point(-theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 30));
	bitDepthMenu->setOriginalPosition(Point(-theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 30));

	minimalistButton->setOriginalSize(Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0));
	fullButton->setOriginalSize(Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0));
	customButton->setOriginalSize(Size(theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0));

#endif	
	UI_Window::reloadOriginalSize();
//	mapSettings->calculateBoxSize();
//	locationSettings->calculateBoxSize();
//	playerSettings->calculateBoxSize();
//	menuRadio->calculateBoxSize(true);
//	locationContents->calculateBoxSize();
}


DataBaseWindow::~DataBaseWindow()
{
/*	delete mapSettings;
	delete mapName;
	delete maxPlayer;
	delete maxLocations;
	delete symmetryButton;
	delete playerSettings;

	delete locationSettings;
	for(unsigned int i = MAX_LOCATIONS;i--;)
	{
		delete locationName[i];
		delete distance[i];
	}
	for(unsigned int i = MAX_INTERNAL_PLAYER;i--;)
		delete playerStart[i];

	delete locationMenu;
	delete map;

	for(unsigned int i=MAX_MAPWINDOW_BUTTONS;i--;)
		delete menuButton[i];
	delete menuRadio;

	delete locationContents;
	delete mineralBlocks;
	delete vespeneGeysirs;
	delete mineralDistance;*/
}

void DataBaseWindow::resetData()
{}

void DataBaseWindow::closeMenus()
{
/*	if(locationMenu->isOpen())
	{
		locationMenu->close();
		setNeedRedrawNotMoved();
	}*/
}

void DataBaseWindow::process()
{
/*	languageHasChanged=false;
	bitDepthHasChanged=false;
	resolutionHasChanged=false;
	fullScreenHasChanged=false;*/

/*	if(!isShown())
	{
		mapName->Hide();
		return;
	}
	if(!mapName->isShown())
	{
		mapName->Show();
//		if(UI_Object::focus == NULL) // TODO
//			UI_Object::focus = mapName;
	}

*/	
	UI_Window::process();/*
	if(maxLocations->hasNumberChanged())
	{
		for(unsigned int i = maxLocations->getNumber();i--;)
		{
			locationName[i]->Show();	
			distance[i]->Show();
		}
		for(unsigned int i = maxLocations->getNumber();i < MAX_LOCATIONS;i++)
		{
			locationName[i]->Hide();
			distance[i]->Hide();
		}
	}
	if(maxPlayer->hasNumberChanged())
	{
		for(unsigned int i = maxPlayer->getNumber();i--;)
			playerStart[i]->Show();	
		for(unsigned int i = maxPlayer->getNumber();i < MAX_INTERNAL_PLAYER;i++)
			playerStart[i]->Hide();
	}

	std::list<std::string> location_list;
	for(unsigned int i = 0; i < maxLocations->getNumber(); i++)
	{
		location_list.push_back(locationName[i]->getText()->getString());
		distance[i]->updateText(locationName[i]->getText()->getString());
	}
	locationMenu->assignMap(location_list);
	

	mapSettings->calculateBoxSize();
	locationSettings->calculateBoxSize();
	playerSettings->calculateBoxSize();
	menuRadio->calculateBoxSize(true);
	locationContents->calculateBoxSize();

//	for(unsigned int i = maxPlayer->getNumber();i--;)
//	{
		if(locationMenu->getPressedItem()>=0)
		{
//			assignGoal = goalMenu->getPressedItem();
			playerSettings->forceUnpressAll();
//			playerStart[i]->forceUnpress();
		}
//	}
*/	
// ------ OPEN/CLOSE MENUES
//	if(playerSettings->buttonHasChanged())
//	{
//		locationMenu->setParent(playerStart[playerSettings->getMarked()]);
//		locationMenu->open();
//		if(!locationMenu->isOpen())
//		{
//			playerSettings->forceUnpressAll();
//			closeMenus();
//		} else
//		{
//			closeMenus();
//			locationMenu->open();
//		}
//	}



	
/* playerSettings: symmetrische Karte?
 * - Auswahl nur zwischen Zentrum und den uebrigen Gebieten
 * - */

	
#if 0
	if(uiSettingsRadio->buttonHasChanged())
	{
		setNeedRedrawNotMoved();
		switch(uiSettingsRadio->getMarked())
		{
			case 0:languageMenu->open();
			       if(!languageMenu->isOpen())
			       {
				       uiSettingsRadio->forceUnpressAll();
				       closeMenus();
				} else
				{
					languageMenuButton->forcePress();
					closeMenus();
					languageMenu->open();
				}break;
				       
			case 1:resolutionMenu->open();
			       if(!resolutionMenu->isOpen())
			       {
				       uiSettingsRadio->forceUnpressAll();
				       closeMenus();
				} else
				{
					resolutionMenuButton->forcePress();
					closeMenus();
					resolutionMenu->open();
				}break;
			       
			case 2:bitDepthMenu->open();
			       if(!bitDepthMenu->isOpen())
			       {
				       uiSettingsRadio->forceUnpressAll();
				       closeMenus();
				} else
				{
					bitDepthMenuButton->forcePress();
					closeMenus();
					bitDepthMenu->open();
				}break;
			default:break;
		}
	}
        if(!isMouseInside())
	{
		uiSettingsRadio->forceUnpressAll();
		closeMenus();
	}
	
	if(languageMenu->getPressedItem()>=0)
	{
		eLanguage newLanguage = (eLanguage)(languageMenu->getPressedItem()+1);
		if(newLanguage!=UI_Object::theme.getLanguage())
		{
			UI_Object::theme.setLanguage(newLanguage);
			uiConfiguration.setLanguage(UI_Object::theme.getLanguage());
			languageHasChanged = true;
		}
		languageMenuButton->forceUnpress();
		languageMenu->close();
		setNeedRedrawNotMoved();
	} 
	else if(resolutionMenu->getPressedItem()>=0)
	{
		eResolution newResolution = (eResolution)(resolutionMenu->getPressedItem()+1);
		if(newResolution != UI_Object::theme.getResolution())
		{
			UI_Object::theme.setResolution(newResolution);
			uiConfiguration.setResolution(UI_Object::theme.getResolution());
			resolutionHasChanged = true;
		}
		resolutionMenuButton->forceUnpress();
		resolutionMenu->close();
		setNeedRedrawNotMoved();
	} 
	else if(bitDepthMenu->getPressedItem()>=0)
	{
		eBitDepth newBitDepth = (eBitDepth)bitDepthMenu->getPressedItem();
		if(newBitDepth != UI_Object::theme.getBitDepth())
		{
			UI_Object::theme.setBitDepth(newBitDepth);
			uiConfiguration.setBitDepth(UI_Object::theme.getBitDepth());
			bitDepthHasChanged = true;
			resolutionHasChanged = true;
		}
		bitDepthMenuButton->forceUnpress();
		bitDepthMenu->close();
		setNeedRedrawNotMoved();
	}

	if(fullscreen->isClicked())
		fullScreenHasChanged = true;

	coreConfiguration.setMaxTime ( maxTime->getNumber() );
	coreConfiguration.setMaxLength ( maxLength->getNumber() );
	coreConfiguration.setMaxGenerations ( maxGenerations->getNumber() );
	coreConfiguration.setMaxRuns ( maxRuns->getNumber() );
	coreConfiguration.setMaxTimeOut ( maxTimeOut->getNumber() );
	coreConfiguration.setBreedFactor ( breedFactor->getNumber() );
//	coreConfiguration.setCrossingOver ( crossingOver->getNumber() );
	efConfiguration.setDesiredFramerate ( desiredFramerate->getNumber() );
	efConfiguration.setDesiredCPU ( desiredCPU->getNumber() );

	efConfiguration.setRestrictSC( restrictSC->isChecked() );
	efConfiguration.setFacilityMode( facilityMode->isChecked() );
	efConfiguration.setAutoSaveRuns ( autoSaveRuns->isChecked() );
	coreConfiguration.setAlwaysBuildWorker( alwaysBuildWorker->isChecked() );
	coreConfiguration.setOnlySwapOrders( onlySwapOrders->isChecked() );
//	coreConfiguration.setPreprocessBuildOrder ( preprocessBuildorder->isChecked() );
//	coreConfiguration.setAllowGoalAdaption ( allowGoalAdaption->isChecked() );
	uiConfiguration.setGlowingButtons ( glowingButtons->isChecked() );
	efConfiguration.setDnaSpiral ( dnaSpiral->isChecked() );
	efConfiguration.setBackgroundBitmap ( backgroundBitmap->isChecked() );
	if(efConfiguration.isFullScreen()!=fullscreen->isChecked())
		fullScreenHasChanged=true;
	efConfiguration.setFullScreen ( fullscreen->isChecked() ); 
	efConfiguration.setToolTips ( tooltips->isChecked() );
//	efConfiguration.setSoftwareMouse ( softwareMouse->isChecked() );
//	uiConfiguration.setTransparency ( transparency->isChecked() );
	uiConfiguration.setSmoothMovements ( smoothMovement->isChecked() );

	if(reloadFromFileButton->isLeftClicked())
		reloadFromFile();
	if(loadFailsafeDefaultsButton->isLeftClicked())
		loadFailsafeDefaults();
	if(saveToFileButton->isLeftClicked())
	{
		coreConfiguration.saveToFile();
		uiConfiguration.saveToFile();
		efConfiguration.saveToFile();
	}

	if(minimalistButton->isLeftClicked())
	{
//		desiredFramerate->updateNumber ( 10 ); ~TODO
//		desiredCPU->updateNumber ( 50 );
		glowingButtons->check ( false );
		dnaSpiral->check ( false );
		backgroundBitmap->check ( false );
		fullscreen->check ( false );
		tooltips->check ( false );
//		softwareMouse->check ( false );
//		transparency->check ( false );
		smoothMovement->check ( false );
	} else
	if(fullButton->isLeftClicked())
	{
//		desiredFramerate->updateNumber ( 35 );
//		desiredCPU->updateNumber ( 50 );
		glowingButtons->check ( true );
		dnaSpiral->check ( true );
		backgroundBitmap->check ( true );
		fullscreen->check ( true );
		tooltips->check ( true );
//	  softwareMouse->check ( true );
//	  transparency->check ( true );
		smoothMovement->check ( true );
		// TODO disable...
	} else
	{
		if(glowingButtons->isClicked() || dnaSpiral->isClicked() || backgroundBitmap->isClicked() || fullscreen->isClicked() || tooltips->isClicked() || /*softwareMouse->isClicked() ||*//* transparency->isClicked() ||*/smoothMovement->isClicked() /*|| desiredFramerate->hasNumberChanged() ||  desiredCPU->hasNumberChanged()*/)
			defaultSettingsRadio->forcePress(2); // custombutton
	}
// TODO reihenfolge und Radiobuttons...
#endif
}
#if 0
// reload from file
void DataBaseWindow::reloadFromFile()
{
	efConfiguration.loadConfigurationFile();
	uiConfiguration.loadConfigurationFile();
	coreConfiguration.loadConfigurationFile();
	updateItems();
}

// use failsafe default settings
void DataBaseWindow::loadFailsafeDefaults()
{
	uiConfiguration.initDefaults();
	efConfiguration.initDefaults();
	coreConfiguration.initDefaults();
	updateItems();
}

void DataBaseWindow::updateItems()
{
	maxTime->updateNumber ( coreConfiguration.getMaxTime() );
	maxLength->updateNumber ( coreConfiguration.getMaxLength() );
	maxGenerations->updateNumber ( coreConfiguration.getMaxGenerations() );
	maxRuns->updateNumber ( coreConfiguration.getMaxRuns() );
	maxTimeOut->updateNumber ( coreConfiguration.getMaxTimeOut());
	breedFactor->updateNumber ( coreConfiguration.getBreedFactor() );
//	crossingOver->updateNumber ( coreConfiguration.getCrossingOver() );
	desiredFramerate->updateNumber ( efConfiguration.getDesiredFramerate() );
	desiredCPU->updateNumber ( efConfiguration.getDesiredCPU() );
	
	eLanguage newLanguage = uiConfiguration.getLanguage();
	if(newLanguage!=UI_Object::theme.getLanguage())
	{
		UI_Object::theme.setLanguage( newLanguage );
		uiConfiguration.setLanguage( newLanguage );
		languageHasChanged = true;
		resolutionHasChanged = true;
	}
	
	eResolution newResolution = uiConfiguration.getResolution();
	if(newResolution != UI_Object::theme.getResolution())
	{
		UI_Object::theme.setResolution( newResolution );
		uiConfiguration.setResolution( newResolution );
		resolutionHasChanged = true;
	}

	eBitDepth newBitDepth = uiConfiguration.getBitDepth();
	if(newBitDepth != UI_Object::theme.getBitDepth())
	{
		UI_Object::theme.setBitDepth( newBitDepth );
		uiConfiguration.setBitDepth( newBitDepth );
		bitDepthHasChanged = true;
	}
	
	restrictSC->check ( efConfiguration.isRestrictSC() );
	facilityMode->check ( efConfiguration.isFacilityMode() );
	autoSaveRuns->check ( efConfiguration.isAutoSaveRuns() );
	alwaysBuildWorker->check ( coreConfiguration.isAlwaysBuildWorker() );
	onlySwapOrders->check ( coreConfiguration.isOnlySwapOrders() );
//	preprocessBuildorder->check ( coreConfiguration.isPreprocessBuildOrder() );
//	allowGoalAdaption->check ( coreConfiguration.isAllowGoalAdaption() );
	glowingButtons->check ( uiConfiguration.isGlowingButtons() );
	dnaSpiral->check ( efConfiguration.isDnaSpiral() );
	backgroundBitmap->check ( efConfiguration.isBackgroundBitmap() );
	if(fullscreen->isChecked() != efConfiguration.isFullScreen())
		fullScreenHasChanged=true;
	fullscreen->check ( efConfiguration.isFullScreen() );
	tooltips->check ( efConfiguration.isToolTips() );
//	softwareMouse->check ( efConfiguration.isSoftwareMouse() );
//	transparency->check ( uiConfiguration.isTransparency() );
	smoothMovement->check ( uiConfiguration.isSmoothMovements() );
}
#endif 
void DataBaseWindow::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Window::draw(dc);
}
#if 0
void DataBaseWindow::forceLanguageChange()
{
	languageHasChanged=true;
}
#endif


