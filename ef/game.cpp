#include "game.hpp"
#include <sstream>

Game::Game(UI_Object* game_parent, const BASIC_MAP* game_map, const unsigned int game_number, const unsigned int game_max) :
	UI_Window(game_parent, GAME_WINDOW_TITLE_STRING, theme.lookUpGameRect(GAME_WINDOW, game_number, game_max), theme.lookUpGameMaxHeight(GAME_WINDOW, game_number, game_max), NOT_SCROLLED, NO_AUTO_SIZE_ADJUST, Rect(0,0,1280,1024), TRANSPARENT),
	scoreWindow(new ScoreWindow(this, game_number, game_max)),
	gameNumber(0),
	gameMax(0),
	soup(new SOUP()),
	mapPlayerCount(0),
	map(NULL),
	optimizing(false),
	resetFlag(false),
	boHasChanged(false),
	unchangedGenerations(0),
	mapPlayerCountInitialized(false),
	anaraceInitialized(false)
//	buttonGroup(new UI_Group(this, Rect(Point(20, 10), Size(100,0)), Size(0, 0), TOP_LEFT, NULL_STRING)),
//	splitGameButton(new UI_Button(buttonGroup, Rect(0, 0, 0, 0), Size(30, 0), MY_BUTTON, false, PRESS_BUTTON_MODE, COMPARE_GAME_STRING, DO_NOT_ADJUST, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
//	removeButton(new UI_Button(buttonGroup, Rect(0, 0, 0, 0), Size(30, 0), MY_BUTTON, false, PRESS_BUTTON_MODE, REMOVE_GAME_STRING, DO_NOT_ADJUST, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH))
{
	for(unsigned int i=MAX_PLAYER;i--;) // TODO
	{
		anarace[i] = NULL;
		player[i] = new Player(this, game_number, game_max, 0, 1);
		player[i]->Hide();
		lastOptimizing[i] = true;
		// TODO Player hinzufuegen/entfernen
	}
	for(unsigned int i=MAX_INTERNAL_PLAYER;i--;)
		start[i] = new START(&(startForce[i]));

	assignMap(game_map);
	setMode(game_number, game_max);

	memset(oldCode, 999, MAX_PLAYER * MAX_LENGTH * sizeof(int));

	scoreWindow->makeFirstChild();
}

Game::~Game()
{
	for(unsigned int i=MAX_PLAYER;i--;)
		delete player[i];
	for(unsigned int i=MAX_INTERNAL_PLAYER;i--;)
		delete start[i];
	delete soup;
	delete scoreWindow;
//	delete splitGameButton;
//	delete removeButton;
//	delete buttonGroup;
}

const bool Game::isCompletelyInitialized() const 
// except anarace of course
{
	bool completely_initialized = true;
	completely_initialized &= mapPlayerCountInitialized;
	for(unsigned int i = MAX_PLAYER; i--;)
		if((player[i])&&(player[i]->isShown()))
			completely_initialized &= start[i+1]->isCompletelyInitialized();
//	completely_initialized &= start[0]->isCompletelyInitialized();
	return(completely_initialized);	
}

const bool Game::openMenu(const ePlayerOrder order)
{
#ifdef _SCC_DEBUG
	if(!mapPlayerCountInitialized) {
                toErrorLog("DEBUG (Game::setMode()): Variable mapPlayerCount not initialized.");return(false);
	}
#endif

	if(mapPlayerCount == 0)
		return(false);
	if((order == OPEN_RACE_MENU)||(order == ADD_PLAYER))
		return(scoreWindow->openMenu(order));
	unsigned int i = scoreWindow->currentPlayer;
	do
	{
		if((player[i])&&(player[i]->isShown()))
		{
			if(order == RESTART_CALCULATION)
			{
				resetPlayer(i);				
				scoreWindow->currentPlayer = i;
				return(true);
			} else
			if(player[i]->openMenu(order))
			{
				scoreWindow->currentPlayer = i;
				return(true);
			}
		}
		i++;
		if(i >= MAX_PLAYER)
		i = 0; // ? oder Block doch an den Anfang der Schleife? Kommt halt drauf an ob User zwischen den Playern hin und her switchen kann TODO
		
	} while(i!=scoreWindow->currentPlayer);
	return(false);		
}

void Game::reloadOriginalSize()
{
	setOriginalRect(UI_Object::theme.lookUpGameRect(GAME_WINDOW, gameNumber, gameMax));
	setMaxHeight(UI_Object::theme.lookUpGameMaxHeight(GAME_WINDOW, gameNumber, gameMax));
//	buttonGroup->alignWidth(UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH));
//	buttonGroup->calculateBoxSize(ONE_COLOUMN_GROUP);
	UI_Window::reloadOriginalSize();
	boHasChanged = true;
}

void Game::setMapPlayerCount(const unsigned int player_count)
{
#ifdef _SCC_DEBUG
	if(player_count > MAX_PLAYER) {
		toErrorLog("DEBUG (Game::setMapPlayerCount()): Value player_count out of range.");return;
	}
#endif
	mapPlayerCount = player_count;
	mapPlayerCountInitialized = true;
}

void Game::assignMap(const BASIC_MAP* game_map) 
{
	toInitLog("* " + UI_Object::theme.lookUpString(START_ASSIGNING_MAP_STRING));
#ifdef _SCC_DEBUG
	if(game_map == NULL) {
		toErrorLog("DEBUG: (Game::assignMap): Value game_map not initialized.");return;
	}
#endif
	map = game_map;

	setMapPlayerCount(map->getMaxPlayer());
	for(unsigned int i = mapPlayerCount+1;i--;)
		start[i]->assignMap(game_map);
	
/*	for(unsigned int i = mapPlayerCount;i--;)
	{
		setHarvestSpeed(i+1, TERRA, database.getHarvestSpeed(TERRA, 0));
		setHarvestSpeed(i+1, PROTOSS, database.getHarvestSpeed(PROTOSS, 0));
		setHarvestSpeed(i+1, ZERG, database.getHarvestSpeed(ZERG, 0));
		//setStartPosition(i+1, 3*i+1); // <- TODO
	}*/ // ? Warum darf assignMap das? mmmh...
	
//	TODO: start initialisieren!
	setMode(gameNumber, gameMax);
	soup->setMapPlayerNum(mapPlayerCount);
	scoreWindow->setMaxPlayer(mapPlayerCount);
	setResetFlag();
}



void Game::loadBuildOrder(const unsigned int player_num, const unsigned int number)
{
#ifdef _SCC_DEBUG
	if(!mapPlayerCountInitialized) {
                toErrorLog("DEBUG (Game::loadBuildOrder()): Variable mapPlayerCount not initialized.");return;
	}
	if((player_num < 0) || (player_num >= mapPlayerCount)) {
		toErrorLog("DEBUG (Game::loadBuildOrder()): Value player_num out of range.");return;
	}
#endif
	anarace[player_num]->copyProgramList(database.getBuildOrder(anarace[player_num]->getRace(), anarace[player_num]->getGoal(), number)->getProgramList());
	boHasChanged = true;
//	initSoup(player_num);
//	newGeneration();
// 	resetPlayer(player_num); ?
//	UI_Window::setChangedFlag(); ?
}
	
void Game::assignRace(const unsigned int player_num, const eRace assigned_race)
{
#ifdef _SCC_DEBUG
	if(!mapPlayerCountInitialized) {
                toErrorLog("DEBUG (Game::assignRace()): Variable mapPlayerCount not initialized.");return;
	}
	if((player_num < 0) || (player_num >= mapPlayerCount)) {
		toErrorLog("DEBUG (Game::assignRace()): Value player_num out of range.");return;
	}
#endif
	toInitLog("* " + UI_Object::theme.lookUpString(START_SETTING_RACE_STRING));
	
	start[player_num+1]->setPlayerRace(assigned_race);
	start[player_num+1]->assignStartCondition(database.getStartCondition(assigned_race, 0)); // assign default startcondition, make a menu later
	fillGroups();
	assignGoal(player_num, 0); // assign default goal
	
//	setChangedFlag(); ?
	initSoup(player_num);
	newGeneration();
	player[player_num]->resetData();
	player[player_num]->assignAnarace(anarace[player_num]); // !
}

void Game::assignGoal(const unsigned int player_num, const unsigned int player_goal) 
{
	toInitLog("* " + UI_Object::theme.lookUpString(START_ASSIGNING_GOAL_STRING));
#ifdef _SCC_DEBUG
	if(!mapPlayerCountInitialized) {
                toErrorLog("DEBUG (Game::assignGoal()): Variable mapPlayerCount not initialized.");return;
	}
        if(player_num >= mapPlayerCount) {
                toErrorLog("DEBUG: (Game::assignGoal): Value player_num out of range.");return;
        }       
#endif
	start[player_num+1]->assignGoal(database.getGoal(start[player_num+1]->getPlayerRace(), player_goal));
}


void Game::resetPlayer(const unsigned int player_num)
{
	initSoup(player_num);
	newGeneration();
	player[player_num]->resetData();
	scoreWindow->resetPlayerTime(player_num);
}

void Game::resetPlayerTime()
{
	for(unsigned int i = 0; i < mapPlayerCount; ++i)
		scoreWindow->resetPlayerTime(i);
	boHasChanged = true;
}

void Game::initSoup() 
{
	toInitLog("* " + UI_Object::theme.lookUpString(START_CREATING_BUILD_ORDERS_STRING));
	unchangedGenerations = 0;
// TODO pruefen ob alles initiiert wurde...
	ANABUILDORDER::resetStaticData(); // TODO
	soup->initSoup(&start);
}

void Game::initSoup(unsigned int player_number) 
{
#ifdef _SCC_DEBUG
	if(!mapPlayerCountInitialized) {
                toErrorLog("DEBUG (Game::initSoup()): Variable mapPlayerCount not initialized.");return;
	}
        if(player_number >= mapPlayerCount) {
                toErrorLog("DEBUG (Game::initSoup()): Value player_number out of range.");return;
        }       
#endif
	unchangedGenerations = 0;
// TODO pruefen ob alles initiiert wurde...
	ANABUILDORDER::resetStaticData(); // TODO
	soup->initSoup(player_number, start[player_number+1]);
}

void Game::fillGroups() 
{
#ifdef _SCC_DEBUG
	if(!mapPlayerCountInitialized) {
                toErrorLog("DEBUG (Game::fillGroups()): Variable mapPlayerCount not initialized.");return;
	}
#endif
	toInitLog("* " + UI_Object::theme.lookUpString(START_CREATING_START_UNITS_STRING));
	start[0]->fillAsNeutralPlayer(); // TODO
	for(unsigned int i = 1; i <= mapPlayerCount;++i)
		start[i]->fillAsActivePlayer();
}

void Game::setPlayerInitMode(const unsigned int player_number, const eInitMode init_mode)
{
#ifdef _SCC_DEBUG
	if(!mapPlayerCountInitialized) {
                toErrorLog("DEBUG (Game::setPlayerInitMode()): Variable mapPlayerCount not initialized.");return;
	}
        if(player_number >= mapPlayerCount) {
                toErrorLog("DEBUG (Game::setPlayerInitMode()): Value player_number out of range.");return;
        }       
#endif
	scoreWindow->setInitMode(player_number, init_mode);
	if(init_mode == INITIALIZED)
		player[player_number]->Show();
}

void Game::setMode(const unsigned int game_number, const unsigned int game_max)
{
#ifdef _SCC_DEBUG
	if(game_max > MAX_GAME) {
                toErrorLog("DEBUG (Game::setMode()): Value game_max out of range.");return;
	}
	if(game_number > game_max) {
                toErrorLog("DEBUG (Game::setMode()): Value game_number out of range.");return;
        }
	if(!mapPlayerCountInitialized) {
                toErrorLog("DEBUG (Game::setMode()): Variable mapPlayerCount not initialized.");return;
	}
		
#endif
	if((game_number == gameNumber)&&(game_max == gameMax))
		return;
	gameNumber = game_number;
	gameMax = game_max;
	
	scoreWindow->setMode(game_number, game_max);
	for(unsigned int i = mapPlayerCount; i--;)
		player[i]->setMode(gameNumber, gameMax, i, mapPlayerCount);
	
// ---------------	
	reloadOriginalSize();
	setNeedRedrawMoved();

//	if((game_max>1)||(game_number==1))
//		splitGameButton->Hide();
//	else splitGameButton->Show();
//	buttonGroup->alignWidth(UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH));
//	buttonGroup->calculateBoxSize(ONE_COLOUMN_GROUP);

	std::ostringstream os;
	os.str("");
	os << game_number+1;
	setTitleParameter(os.str());
	scoreWindow->setTitleParameter(UI_Object::theme.lookUpFormattedString(GAME_WINDOW_TITLE_STRING, os.str()));
	
}

void Game::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Object::theme.setColorTheme(UI_Object::theme.getMainColorTheme());
	UI_Object::draw(dc);
}

void Game::process()
{
// TODO nicht gesamt Reset machen sondern je nach dem welcher Player resettet wurde!!
#ifdef _SCC_DEBUG
	if(!mapPlayerCountInitialized) {
                toErrorLog("DEBUG (Game::process()): Variable mapPlayerCount not initialized.");return;
	}
#endif
	if(UI_Window::getChangedFlag())
	{
		for(unsigned int i=mapPlayerCount;i--;)
//			if(scoreWindow->isOptimizing(i)) // <- :o
			{
				player[i]->recheckSomeDataAfterChange();
				if(anaraceInitialized) 
					anarace[i]->restartData(); // TODO
				boHasChanged = true;
			}
		UI_Window::changeAccepted();
	}
	if(getResetFlag())
	{
		for(unsigned int i=mapPlayerCount;i--;)
	//		if(scoreWindow->getInitMode(i) == INITIALIZED)
			player[i]->resetData();
		initSoup();
		newGeneration();
		setResetFlag(false);
	}

	if(!isShown())
		return;
	

	UI_Window::process();

	for(unsigned int i = MAX_PLAYER;i--;)
		if((i<mapPlayerCount)&&(scoreWindow->getInitMode(i)==INITIALIZED))
			player[i]->Show();
		else
			player[i]->Hide();

	for(unsigned int i = mapPlayerCount;i--;)
	{
		if(player[i]->wasResetted())
			resetPlayer(i);
		if(player[i]->getLoadedBuildOrder()>=0)
			loadBuildOrder(i, player[i]->getLoadedBuildOrder());
		if(player[i]->getAssignedGoal()>=0)
			assignGoal(i, player[i]->getAssignedGoal());			
		if(scoreWindow->getAssignedRace(i)>=0)
			assignRace(i, (eRace)scoreWindow->getAssignedRace(i));
	}

	
	if(anaraceInitialized)
	{
// ------ Did the user change optimization?
		for(unsigned int i = mapPlayerCount;i--;)
			anarace[i]->setOptimizing(scoreWindow->isOptimizing(i));
	
		if(boHasChanged)
		{
			boHasChanged = false;
			for(unsigned int i = mapPlayerCount;i--;)
			{
				if(player[i]->isShown())
					player[i]->CheckOrders();
	// update score window
			
				if(scoreWindow->getInitMode(i) == INITIALIZED)
				{
					if(anarace[i]->getTimer()==0)
					{
						scoreWindow->setScore(i, 0);
						scoreWindow->setScoreMode(i, SCORE_FULFILL_MODE);
						scoreWindow->setGoalComplete(i, anarace[i]->getGoalPercentage());
					}
					else
					{
						scoreWindow->setScoreMode(i, SCORE_TIME_MODE);
						scoreWindow->setScore(i, anarace[i]->getRealTimer());
						scoreWindow->setGoalComplete(i, anarace[i]->getGoalPercentage());
					}
				}
			}	
		}
	}

	if(scoreWindow->getAssignedMap()>=0)
	{
		assignMap(database.getMap(scoreWindow->getAssignedMap()));
		fillGroups();
	}


	scoreWindow->setUnchangedGenerations(unchangedGenerations);
}


void Game::newGeneration()
{
#ifdef _SCC_DEBUG
	if(!mapPlayerCountInitialized) {
                toErrorLog("DEBUG (Game::newGeneration()): Variable mapPlayerCount not initialized.");return;
	}
#endif
	if(!isCompletelyInitialized())
		return;
	bool is_any_optimizing = isAnyOptimizing();
	bool active[MAX_PLAYER];
	for(unsigned int i = MAX_PLAYER; i--;) active[i] = false;
	for(unsigned int i = MAX_PLAYER; i--;)
		if((player[i]) && (player[i]->isShown()))
			active[i] = true;
	for(unsigned int i = mapPlayerCount; i--;) 
		if((anarace[i]==NULL)||(anarace[i]->isDifferent(oldCode[i])))
		{
			is_any_optimizing = false;
			break;
		}
	if(!is_any_optimizing)
	{
		if(soup->recalculateGeneration(anarace, &startForce, active)) // <- konstant
		{
			anaraceInitialized = true;
			for(unsigned int i=mapPlayerCount;i--;)
				if(anarace[i]->isDifferent(oldCode[i]))
				{
					boHasChanged = true;
					player[i]->assignAnarace(anarace[i]);
				}
		}
	} else
	if(soup->newGeneration(anarace, &startForce))
	{
		anaraceInitialized = true;
		for(unsigned int i=mapPlayerCount;i--;)
			if((scoreWindow->isOptimizing(i)) && (anarace[i]->isDifferent(oldCode[i])))
			{
				boHasChanged = true;
				player[i]->assignAnarace(anarace[i]);
			}
	
		unchangedGenerations++;
	}
	if(boHasChanged)
		unchangedGenerations = 0;
	for(unsigned int i=mapPlayerCount;i--;)
		if(anarace[i])
			anarace[i]->copyCode(oldCode[i]);
		else
			memset(oldCode[i], 999, MAX_LENGTH * sizeof(int));
}

void Game::startLastOptimizing()
{
#ifdef _SCC_DEBUG
	if(!mapPlayerCountInitialized) {
                toErrorLog("DEBUG (Game::startLastOptimizing()): Variable mapPlayerCount not initialized.");return;
	}
#endif
	for(unsigned int i = mapPlayerCount;i--;)
		if(lastOptimizing[i])
			scoreWindow->startOptimizing(i);
}

void Game::stopOptimizing()
{
#ifdef _SCC_DEBUG
	if(!mapPlayerCountInitialized) {
                toErrorLog("DEBUG (Game::stopOptimizing()): Variable mapPlayerCount not initialized.");return;
	}
#endif
	for(unsigned int i = mapPlayerCount; i--;)
		if(scoreWindow->isOptimizing(i))
		{
			lastOptimizing[i] = true;
			scoreWindow->stopOptimizing(i);
		} else lastOptimizing[i] = true;
}

const bool Game::isAnyOptimizing() const 
{
#ifdef _SCC_DEBUG
	if(!mapPlayerCountInitialized) {
                toErrorLog("DEBUG (Game::isAnyOptimizing()): Variable mapPlayerCount not initialized.");return(false);
	}
#endif
	for(unsigned int i = mapPlayerCount;i--;)
		if(scoreWindow->isOptimizing(i))
			return(true);
	return(false);
}

void Game::compactDisplayModeHasChanged()
{
#ifdef _SCC_DEBUG
	if(!mapPlayerCountInitialized) {
                toErrorLog("DEBUG (Game::compactDisplayModeHasChanged()): Variable mapPlayerCount not initialized.");return;
	}
#endif
	for(unsigned int i = mapPlayerCount; i--;)
		player[i]->compactDisplayModeHasChanged();
}

void Game::setHarvestSpeed(const unsigned int player_num, const eRace harvest_race, const HARVEST_SPEED* harvest_speed) 
{
#ifdef _SCC_DEBUG
	if(!mapPlayerCountInitialized) {
                toErrorLog("DEBUG (Game::setHarvestSpeed()): Variable mapPlayerCount not initialized.");return;
	}
        if((player_num < 1) || (player_num > mapPlayerCount)) {
                toErrorLog("DEBUG: (Game::setHarvestSpeed): Value player_num out of range.");return;
        }
#endif
	start[player_num]->setHarvestSpeed(harvest_race, harvest_speed);
}

/*void Game::setStartRace(const unsigned int player_num, const eRace player_race) 
{
#ifdef _SCC_DEBUG
	if(!mapPlayerCountInitialized) {
                toErrorLog("DEBUG (Game::setStartRace()): Variable mapPlayerCount not initialized.");return;
	}
	if((player_num < 1) || (player_num > mapPlayerCount)) {
		toErrorLog("DEBUG: (Game::setStartRace): Value player_num out of range.");return;
	}
#endif
	start[player_num]->setPlayerRace(player_race);
}*/

void Game::setStartPosition(const unsigned int player_num, const unsigned int player_position) 
{
	toInitLog("* " + UI_Object::theme.lookUpString(START_SETTING_START_POSITION_STRING));
#ifdef _SCC_DEBUG
	if(!mapPlayerCountInitialized) {
                toErrorLog("DEBUG (Game::setStartPosition()): Variable mapPlayerCount not initialized.");return;
	}
        if((player_num < 1) || (player_num > mapPlayerCount)) {
                toErrorLog("DEBUG: (Game::setStartPosition): Value player_num out of range.");return;
        }
#endif
	start[player_num]->setStartPosition(player_position);
}

void Game::assignStartCondition(const unsigned int player_num, const START_CONDITION* start_condition) 
{
	toInitLog("* " + UI_Object::theme.lookUpString(START_ASSIGNING_START_CONDITION_STRING));
#ifdef _SCC_DEBUG
	if(!mapPlayerCountInitialized) {
                toErrorLog("DEBUG (Game::assignStartCondition()): Variable mapPlayerCount not initialized.");return;
	}
	if((player_num < 1) || (player_num > mapPlayerCount)) {
		toErrorLog("DEBUG: (Game::assignStartCondition): Value player_num out of range.");return;
	}
#endif
	start[player_num]->assignStartCondition(start_condition);
}
//virtual machen ? TODO
//resetData, updateItems, assignAnarace, checkOrders
//
