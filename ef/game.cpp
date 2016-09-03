#include "game.hpp"
#include <sstream>

Game::Game(UI_Object* game_parent, const BASIC_MAP* game_map, const unsigned int game_number, const unsigned int game_max) :
	UI_Window(game_parent, GAME_WINDOW_TITLE_STRING, theme.lookUpGameRect(GAME_WINDOW, game_number, game_max), theme.lookUpGameMaxHeight(GAME_WINDOW, game_number, game_max), NOT_SCROLLED, NO_AUTO_SIZE_ADJUST, Rect(0,0,1280,1024), TRANSPARENT),
	scoreWindow(new ScoreWindow(this, game_number, game_max)),
	gameNumber(0),
	gameMax(0),
	soup(new SOUP()),
	mapPlayerCount(0),
	optimizing(false),
	resetFlag(false),
	boHasChanged(false),
	unchangedGenerations(0),
	mapPlayerCountInitialized(false),
	anaraceInitialized(false)
//	splitGameButton(new UI_Button(this, Rect(0, 0, 0, 0), Size(30, 0), MY_BUTTON, false, PRESS_BUTTON_MODE, COMPARE_GAME_STRING, DO_NOT_ADJUST, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
{
	for(unsigned int i=MAX_PLAYER;i--;) // TODO
	{
		anarace[i] = NULL;
		player[i] = new Player(this, game_number, game_max, 0, 1);
		player[i]->Hide();
		lastOptimizing[i] = true;
		
		// TODO Player hinzufuegen/entfernen
	}

	assignMap(game_map);
	setMode(game_number, game_max);

	scoreWindow->makeFirstChild();
}

Game::~Game()
{
	for(unsigned int i=MAX_PLAYER;i--;)
		delete player[i];
	delete soup;
	delete scoreWindow;
//	delete splitGameButton;
}

const bool Game::isCompletelyInitialized() const 
// except anarace of course
{
	bool completely_initialized = true;
	completely_initialized &= mapPlayerCountInitialized;
/*	for(unsigned int i = MAX_PLAYER; i--;)
		if((player[i])&&(player[i]->isShown()))
			completely_initialized &= start[i]->isCompletelyInitialized();*/ // TODO
	
	return(completely_initialized);	
}

const bool Game::openMenu(const ePlayerOrder order)
{
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
		
	} while(i != scoreWindow->currentPlayer);
	return(false);		
}

void Game::reloadOriginalSize()
{
	setOriginalRect(UI_Object::theme.lookUpGameRect(GAME_WINDOW, gameNumber, gameMax));
	setMaxHeight(UI_Object::theme.lookUpGameMaxHeight(GAME_WINDOW, gameNumber, gameMax));
	UI_Window::reloadOriginalSize();
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
	scoreWindow->setMaxPlayer(mapPlayerCount);
}

void Game::assignMap(const BASIC_MAP* game_map) 
{
	toInitLog("* " + UI_Object::theme.lookUpString(START_ASSIGNING_MAP_STRING));
#ifdef _SCC_DEBUG
	if(game_map == NULL) {
		toErrorLog("DEBUG: (Game::assignMap): Value game_map not initialized.");return;
	}
#endif
	soup->assignMap(game_map);
	setMapPlayerCount(game_map->getMaxPlayer());
	setMode(gameNumber, gameMax);
	setResetFlag();
}



void Game::loadBuildOrder(const unsigned int player_num, const unsigned int number)
{
#ifdef _SCC_DEBUG
	if(player_num >= getMapPlayerCount()) {
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
	
void Game::assignRace(const unsigned int player_num, const unsigned int assigned_race)
{
#ifdef _SCC_DEBUG
	if(player_num >= getMapPlayerCount()) {
		toErrorLog("DEBUG (Game::assignRace()): Value player_num out of range.");return;
	}
#endif
	toInitLog("* " + UI_Object::theme.lookUpString(START_SETTING_RACE_STRING));

	soup->assignRace(player_num, assigned_race);
	newGeneration();

	
	player[player_num]->resetData();
	if(anaraceInitialized)
		player[player_num]->assignAnarace(anarace[player_num]); // !
}

void Game::assignGoal(const unsigned int player_num, const unsigned int player_goal) 
{
	toInitLog("* " + UI_Object::theme.lookUpString(START_ASSIGNING_GOAL_STRING));
	soup->assignGoal(player_num, player_goal);
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
	for(unsigned int i = getMapPlayerCount(); --i;)
		scoreWindow->resetPlayerTime(i);
	boHasChanged = true;
}

void Game::initSoup() 
{
	toInitLog("* " + UI_Object::theme.lookUpString(START_CREATING_BUILD_ORDERS_STRING));
	unchangedGenerations = 0;
// TODO pruefen ob alles initiiert wurde...
	soup->initSoup();
}

void Game::initSoup(unsigned int player_number) 
{
#ifdef _SCC_DEBUG
        if(player_number >= getMapPlayerCount()) {
                toErrorLog("DEBUG (Game::initSoup()): Value player_number out of range.");return;
        }       
#endif
	unchangedGenerations = 0;
// TODO pruefen ob alles initiiert wurde...
	soup->initSoup(player_number);
}

void Game::setPlayerInitMode(const unsigned int player_number, const eInitMode init_mode)
{
#ifdef _SCC_DEBUG
        if(player_number >= getMapPlayerCount()) {
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
#endif
	if((game_number == gameNumber)&&(game_max == gameMax))
		return;
	gameNumber = game_number;
	gameMax = game_max;
	
	scoreWindow->setMode(game_number, game_max);
	for(unsigned int i = getMapPlayerCount(); i--;)
		player[i]->setMode(gameNumber, gameMax, i, getMapPlayerCount());
	
// ---------------	
	reloadOriginalSize();

//	if((game_max>1)||(game_number==1))
//		splitGameButton->Hide();
//	else splitGameButton->Show();

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
	UI_Object::theme.setColorTheme(UI_Object::theme.getMainColorTheme());
}

void Game::doReset()
{
	for(unsigned int i = getMapPlayerCount(); i--;)
	//		if(scoreWindow->getInitMode(i) == INITIALIZED)
		player[i]->resetData();
	initSoup();
	newGeneration();
	setResetFlag(false);
}

void Game::process()
{
// TODO nicht gesamt Reset machen sondern je nach dem welcher Player resettet wurde!!
	if(UI_Window::getChangedFlag())
	{
		for(unsigned int i = getMapPlayerCount(); i--;)
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
		doReset();

	if(!isShown())
		return;

/*	for(unsigned int i = MAX_PLAYER;i--;)
		if(player[i]->checkForNeedRedraw())
		{
			setNeedRedrawMoved();
			break;
		}*/ // TODO !

	UI_Window::process();

	for(unsigned int i = MAX_PLAYER;i--;)
		if((i<getMapPlayerCount())&&(scoreWindow->getInitMode(i)==INITIALIZED))
			player[i]->Show();
		else
			player[i]->Hide();

	for(unsigned int i = getMapPlayerCount();i--;)
	{
		if(player[i]->wasResetted())
			resetPlayer(i);
		if(player[i]->getLoadedBuildOrder() >= 0)
			loadBuildOrder(i, player[i]->getLoadedBuildOrder());
		if(player[i]->getAssignedGoal() >= 0)
			soup->assignGoal(i, player[i]->getAssignedGoal());			
		if(scoreWindow->getAssignedRace(i) >= 0)
			assignRace(i, scoreWindow->getAssignedRace(i));
	}

	
	if(anaraceInitialized)
	{
// ------ Did the user change optimization?
		for(unsigned int i = getMapPlayerCount(); i--;)
			anarace[i]->setOptimizing(scoreWindow->isOptimizing(i));
	
		if(boHasChanged)
		{
			boHasChanged = false;
			for(unsigned int i = getMapPlayerCount(); i--;)
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
		assignMap(database.getMap(scoreWindow->getAssignedMap()));


	scoreWindow->setUnchangedGenerations(unchangedGenerations);
}


void Game::newGeneration()
{
	if(!isCompletelyInitialized())
		return;
	bool is_any_optimizing = isAnyOptimizing();
	bool active[MAX_PLAYER];
	for(unsigned int i = MAX_PLAYER; i--;) active[i] = false;
	for(unsigned int i = MAX_PLAYER; i--;)
		if((player[i]) && (player[i]->isShown()))
			active[i] = true;

	for(unsigned int i = getMapPlayerCount(); i--;) 
		if((anarace[i]==NULL)||(anarace[i]->isDifferent(oldCode[i])))
		{
			is_any_optimizing = false;
			break;
		}
	
	if(!is_any_optimizing)
	{
		if(soup->recalculateGeneration(anarace, active)) // <- konstant
		{
			toErrorLog((int)anarace[0]);
			anaraceInitialized = true;
			for(unsigned int i = getMapPlayerCount(); i--;)
				if(anarace[i]->isDifferent(oldCode[i]))
				{
					boHasChanged = true;
					player[i]->assignAnarace(anarace[i]);
				}
		}
	} else
	if(soup->newGeneration(anarace))
	{
		anaraceInitialized = true;
		for(unsigned int i = getMapPlayerCount(); i--;)
			if((scoreWindow->isOptimizing(i)) && (anarace[i]->isDifferent(oldCode[i])))
			{
				boHasChanged = true;
				player[i]->assignAnarace(anarace[i]);
			}
		unchangedGenerations++;
	}
	
	if(boHasChanged)
		unchangedGenerations = 0;

	for(unsigned int i=getMapPlayerCount(); i--;)
		if(anarace[i] != NULL)
			anarace[i]->copyCode(oldCode[i]);
		else
			oldCode[i].clear();
}

void Game::startLastOptimizing()
{
	for(unsigned int i = getMapPlayerCount();i--;)
		if(lastOptimizing[i])
			scoreWindow->startOptimizing(i);
}

void Game::stopOptimizing()
{
	for(unsigned int i = getMapPlayerCount(); i--;)
		if(scoreWindow->isOptimizing(i))
		{
			lastOptimizing[i] = true;
			scoreWindow->stopOptimizing(i);
		} else lastOptimizing[i] = true;
}

const bool Game::isAnyOptimizing() const 
{
	for(unsigned int i = getMapPlayerCount();i--;)
		if(scoreWindow->isOptimizing(i))
			return(true);
	return(false);
}

void Game::compactDisplayModeHasChanged()
{
	for(unsigned int i = getMapPlayerCount(); i--;)
		player[i]->compactDisplayModeHasChanged();
}

/*
void Game::setHarvestSpeed(const unsigned int player_num, const eRace harvest_race, const HARVEST_SPEED* harvest_speed) 
{
#ifdef _SCC_DEBUG
        if(player_num >= getMapPlayerCount()) {
                toErrorLog("DEBUG: (Game::setHarvestSpeed): Value player_num out of range.");return;
        }
#endif
	start[player_num]->setHarvestSpeed(harvest_race, harvest_speed);
}*/

void Game::setStartPosition(const unsigned int player_num, const unsigned int player_position) 
{
	toInitLog("* " + UI_Object::theme.lookUpString(START_SETTING_START_POSITION_STRING));
	soup->setStartPosition(player_num, player_position);
}

void Game::assignStartCondition(const unsigned int player_num, const START_CONDITION* start_condition) 
{
	toInitLog("* " + UI_Object::theme.lookUpString(START_ASSIGNING_START_CONDITION_STRING));
	soup->assignStartCondition(player_num, start_condition);
}
//virtual machen ? TODO
//resetData, updateItems, assignAnarace, checkOrders
//
