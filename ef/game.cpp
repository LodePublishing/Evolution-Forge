#include "game.hpp"
#include <sstream>

Game::Game(UI_Object* game_parent, const unsigned int game_number, const unsigned int game_max):
	UI_Window(game_parent, GAME_WINDOW_TITLE_STRING, theme.lookUpGameRect(GAME_WINDOW, game_number, game_max), theme.lookUpGameMaxHeight(GAME_WINDOW, game_number, game_max), NOT_SCROLLED, NO_AUTO_SIZE_ADJUST, NOT_TABBED, Rect(0,0,1280,1024), TRANSPARENT),
	resetFlag(false),
	soup(new SOUP()),
	map(NULL),
	optimizing(false),
	boHasChanged(false),
	scoreWindow(new ScoreWindow(this, game_number, game_max)),
	mapPlayerCount(0),
	unchangedGenerations(0),
	currentRun(0),
	totalGeneration(0),
	gameNumber(0),
	gameMax(0),
	splitGameButton(new UI_Button(this, Rect(40, 0, 0, 0), Size(10, 0), MY_BUTTON, false, PRESS_BUTTON_MODE, COMPARE_GAME_STRING, ARRANGE_TOP_LEFT, SMALL_BOLD_FONT, AUTO_SIZE)),
	removeButton(new UI_Button(this, Rect(40, 0, 0, 0), Size(10, 0), MY_BUTTON, false, PRESS_BUTTON_MODE, REMOVE_GAME_STRING, ARRANGE_TOP_LEFT, SMALL_BOLD_FONT, AUTO_SIZE))
{
	for(unsigned int i=MAX_PLAYER;i--;) // TODO
	{
		anarace[i] = soup->getAnalyzedBuildOrder(i);
		player[i] = new Player(this, game_number, game_max, 0, 1);
		player[i]->Hide();
		// TODO Player hinzufuegen/entfernen
	}
	for(unsigned int i=MAX_INTERNAL_PLAYER;i--;)
		start[i] = new START(&(startForce[i]));
	setMode(game_number, game_max);
	std::ostringstream os;
	os.str("");
	os << game_number+1;
	setTitleParameter(os.str());
	scoreWindow->setTitleParameter(UI_Object::theme.lookUpFormattedString(GAME_WINDOW_TITLE_STRING, os.str()));
	
	for(unsigned int i=MAX_PLAYER;i--;)
		for(unsigned int j = MAX_LENGTH;j--;)
			oldCode[i][j]=999;
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
	delete splitGameButton;
	delete removeButton;
}

void Game::reloadOriginalSize()
{
	setOriginalRect(UI_Object::theme.lookUpGameRect(GAME_WINDOW, gameNumber, gameMax));
	setMaxHeight(UI_Object::theme.lookUpGameMaxHeight(GAME_WINDOW, gameNumber, gameMax));
	UI_Window::reloadOriginalSize();
	setBoHasChanged();
}

void Game::assignMap(const BASIC_MAP* game_map) 
{
	map = game_map;
	mapPlayerCount = map->getMaxPlayer();
//	TODO
	for(unsigned int i = mapPlayerCount+1;i--;)
		start[i]->assignMap(game_map);
	
	for(unsigned int i = mapPlayerCount;i--;)
	{
		setHarvestSpeed(i+1, TERRA, database.getHarvestSpeed(TERRA, 0));
		setHarvestSpeed(i+1, PROTOSS, database.getHarvestSpeed(PROTOSS, 0));
		setHarvestSpeed(i+1, ZERG, database.getHarvestSpeed(ZERG, 0));
		setStartPosition(i+1, 3*i+1); // <- TODO
	}
	
//	TODO: start initialisieren!
	setMode(gameNumber, gameMax);
	soup->setMapPlayerNum(mapPlayerCount);
	scoreWindow->setMaxPlayer(mapPlayerCount);
	setResetFlag();
}

void Game::assignStartCondition(const unsigned int player_num, const START_CONDITION* start_condition) 
{
#ifdef _SCC_DEBUG
	if((player_num < 1) || (player_num > mapPlayerCount)) {
		toLog("DEBUG: (Game::assignStartCondition): Value player_num out of range.");return;
	}
#endif
	start[player_num]->assignStartCondition(start_condition);
}

void Game::assignRace(const unsigned int player_num, const eRace assigned_race)
{
       	anarace[player_num]->setRace(assigned_race);
	start[player_num+1]->assignStartCondition(database.getStartCondition(anarace[player_num]->getRace(), 0)); // assign default startcondition, make a menu later
	fillGroups();
	anarace[player_num]->assignGoal(database.getGoal(anarace[player_num]->getRace(), 0)); // assign default goal
	scoreWindow->setInitMode(player_num, INITIALIZED); //?
//	setChangedFlag(); ?

	initSoup(player_num);
	newGeneration();
	player[player_num]->resetData();
	scoreWindow->resetPlayerTime(player_num);
}

void Game::resetPlayer(const unsigned int player_num)
{
	initSoup(player_num);
	newGeneration();
	player[player_num]->resetData();
	scoreWindow->resetPlayerTime(player_num);
}


void Game::initSoup() 
{
// TODO pruefen ob alles initiiert wurde...
	ANABUILDORDER::resetStaticData(); // TODO
	soup->initSoup(&start);
}

void Game::initSoup(unsigned int player_number) 
{
// TODO pruefen ob alles initiiert wurde...
	ANABUILDORDER::resetStaticData(); // TODO
	soup->initSoup(player_number, start[player_number+1]);
}

void Game::fillGroups() 
{
	start[0]->fillAsNeutralPlayer(); // TODO
	for(unsigned int i = 1; i <= mapPlayerCount;++i)
		start[i]->fillAsActivePlayer();
}

void Game::setMode(const unsigned int game_number, const unsigned int game_max)
{
#ifdef _SCC_DEBUG
	if(game_number > game_max) {
                toLog("DEBUG: (Game::setMode): Value game_number out of range.");return;
        }
#endif
	if((game_number == gameNumber)&&(game_max == gameMax))
		return;
	gameNumber = game_number;
	gameMax = game_max;
	scoreWindow->setMode(game_number, game_max);
	for(unsigned int i = mapPlayerCount;i--;)
		player[i]->setMode(gameNumber, gameMax, i, mapPlayerCount);
	
	reloadOriginalSize();
//	resetData();
	setNeedRedrawMoved();
	if((game_max>1)||(game_number==1))
		splitGameButton->Hide();
	else splitGameButton->Show();
}


void Game::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Object::theme.setColorTheme(UI_Object::theme.getMainColorTheme());
	UI_Window::draw(dc);
}

void Game::setBoHasChanged(const bool bo_has_changed)
{
	boHasChanged = bo_has_changed;
}



void Game::process()
{
	if(!isShown())
		return;
	
	UI_Window::process();

	
	
// ------ Did the user change optimization?
	for(unsigned int i = mapPlayerCount;i--;)
	{
		if(player[i]->wasResetted())
			resetPlayer(i);

		if(scoreWindow->getAssignedRace(i)>=0)
			assignRace(i, (eRace)scoreWindow->getAssignedRace(i));
		anarace[i]->setOptimizing(scoreWindow->isOptimizing(i));
	}		
		
	if(scoreWindow->getAssignedMap()>=0)
	{
		assignMap(database.getMap(scoreWindow->getAssignedMap()));
		fillGroups();
	}

	if(scoreWindow->getPlayers()>mapPlayerCount)
	{
		
	}
// ...

//	soup->checkForChange(); ??
// TODO nicht gesamt Reset machen sondern je nach dem welcher Player resettet wurde!!

	if(UI_Window::getChangedFlag())
	{
		for(unsigned int i=mapPlayerCount;i--;)
//			if(scoreWindow->isOptimizing(i)) // <- :o
			{
				player[i]->recheckSomeDataAfterChange();
				anarace[i]->restartData(); // TODO
				setBoHasChanged();
			}
		UI_Window::changeAccepted();
	}
	
	if(getResetFlag())
	{
		for(unsigned int i=mapPlayerCount;i--;)
	//		if(scoreWindow->getInitMode(i) == INITIALIZED)
			player[i]->resetData();
// soup->
		initSoup();
		newGeneration();
		setResetFlag(false);
		UI_Window::changeAccepted();
	}
	for(unsigned int i = MAX_PLAYER;i--;)
	{
		if((i<mapPlayerCount)&&(scoreWindow->getInitMode(i)==INITIALIZED))
			player[i]->Show();
		else
			player[i]->Hide();
	}

	for(unsigned int i=mapPlayerCount;i--;)
		if(player[i]->checkForNeedRedraw())
		{
			setNeedRedrawNotMoved();
			scoreWindow->setNeedRedrawNotMoved();
			break;
		} //?
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

const bool Game::isAnyOptimizing() const 
{
	for(unsigned int i = mapPlayerCount;i--;)
		if(scoreWindow->isOptimizing(i))
			return(true);
	return(false);
}

void Game::newGeneration()
{
	bool is_any_optimizing = isAnyOptimizing();
	
	for(unsigned int i=mapPlayerCount;i--;)
		if((anarace[i]==NULL)||(anarace[i]->isDifferent(oldCode[i])))
		{
			is_any_optimizing = false;
			break;
		}
	
	if(!is_any_optimizing)
	{
		if(soup->recalculateGeneration(anarace, &startForce)) // <- konstant
		{
			for(unsigned int i=mapPlayerCount;i--;)
			{
				if(anarace[i]->isDifferent(oldCode[i]))//, oldMarker[i]))
				{
					setBoHasChanged();
					player[i]->assignAnarace(anarace[i]);
				}
			}
		}
	} else
//TODO: nach Ende eines Durchlaufs ist anarace 0, aber viele anderen Teile des Codes greifen noch drauf zu!! ?
	if(soup->newGeneration(anarace, &startForce))
		for(unsigned int i=mapPlayerCount;i--;)
			if((scoreWindow->isOptimizing(i))&&(anarace[i]->isDifferent(oldCode[i])))
			{
				setBoHasChanged();
				player[i]->assignAnarace(anarace[i]);
			}
	
	for(unsigned int i=mapPlayerCount;i--;)
		if(anarace[i])
			anarace[i]->copyCode(oldCode[i]);
		else
		for(unsigned int j = MAX_LENGTH;j--;)
			oldCode[i][j]=999;
}

//virtual machen
//resetData, updateItems, assignAnarace, checkOrders



