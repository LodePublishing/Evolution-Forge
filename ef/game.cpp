#include "game.hpp"

Game::Game(UI_Object* game_parent, const unsigned int gameNumber):
	UI_Object(game_parent),
	soup(new SOUP()),
	mode(0),
	optimizing(false),
	boHasChanged(true),
	scoreWindow(new ScoreWindow(this))
{
	for(unsigned int i=MAX_PLAYER;i--;) // TODO
	{
		anarace[i] = soup->getAnalyzedBuildOrder(i);
		player[i] = new Player(this, i);
		player[i]->Hide();
		start[i] = new START(&(unit[i]));
	}
//	Hide();
}

Game::~Game()
{
	for(unsigned int i=MAX_PLAYER;i--;)
	{
		delete player[i];
		delete start[i];
	}
	delete soup;
	delete scoreWindow;
}


void Game::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Object::draw(dc);
}


void Game::assignMap(const BASIC_MAP* game_map) {
	map = game_map;
	for(unsigned int i = 0; i <= map->getMaxPlayer();i++)
		start[i]->assignMap(game_map);
	soup->setMapPlayerNum(map->getMaxPlayer());
	scoreWindow->setPlayers(map->getMaxPlayer());
}

void Game::setHarvestSpeed(const unsigned int player_num, const eRace harvest_race, const HARVEST_SPEED* harvest_speed) {
	start[player_num]->setHarvestSpeed(harvest_race, harvest_speed);
}

void Game::setStartRace(const unsigned int player_num, const eRace player_race) {
#ifdef _SCC_DEBUG
	if((player_num < 1) || (player_num > map->getMaxPlayer())) {
		toLog("DEBUG: (Game::setStartRace): Value player_num out of range.");return;
	}
#endif
	start[player_num]->setPlayerRace(player_race);
}

void Game::assignStartCondition(const unsigned int player_num, const START_CONDITION* start_condition) {
#ifdef _SCC_DEBUG
	if((player_num < 1) || (player_num > map->getMaxPlayer())) {
		toLog("DEBUG: (Game::assignStartCondition): Value player_num out of range.");return;
	}                       
#endif
	start[player_num]->assignStartCondition(start_condition);
}

void Game::initSoup() 
{
// TODO pruefen ob alles initiiert wurde...
	ANABUILDORDER::resetStaticData(); // TODO
	soup->initSoup(&start);
}

void Game::setStartPosition(const unsigned int player_num, const unsigned int player_position) {
	start[player_num]->setStartPosition(player_position);
}

void Game::assignGoal(const unsigned int player_num, const GOAL_ENTRY* player_goal) {
	start[player_num]->assignGoal(player_goal);
}

void Game::fillGroups() 
{
	start[0]->fillAsNeutralPlayer(); // TODO
	for(unsigned int i = 1; i <= map->getMaxPlayer();i++)
		start[i]->fillAsActivePlayer();
}

void Game::setMode(const eTab tab, const unsigned int gameNum)//, int game1, int game2)
{
//	this->mode=tab*2+gameNum-2;
//	for(int i=BUILD_ORDER_WINDOW;i<=INFO_WINDOW;i++)
//		window[(eWindow)i]->setTitleParameter(*theme.lookUpString((eString)(tab+HIDE_MODE_STRING)));
	switch(tab)
	{
		case ZERO_TAB:break;
		case BASIC_TAB:
		case ADVANCED_TAB:
		case EXPERT_TAB:
		case GOSU_TAB:
				this->Show();
				scoreWindow->Show();
			break;
		case TUTORIAL_TAB:
		case SETTINGS_TAB:
		case MAP_TAB:
		case COMPARE_TAB: // TODO
				this->Hide();
				scoreWindow->Hide();
		default:break;
	}
	player[0]->setMode(tab, 1);// mehrere players TODO
	resetData();
	setNeedRedrawMoved(); //?
	// TODO modes der einzelnen Windows (z.B> timer oder force)
	// TODO einzelne Player aktivieren
	// TODO scoreWindow
}

void Game::process()
{
	if(!isShown())
		return;
	UI_Object::process();

	if(boHasChanged)
	{
		boHasChanged = false;
		for(unsigned int i = map->getMaxPlayer();i--;)
			if(player[i]->isShown())
				player[i]->CheckOrders();
	}
// ...

//	soup->checkForChange(); ??
	// TODO nicht gesamt Reset machen sondern je nach dem welcher Player resettet wurde!!
	if(UI_Window::getChangedFlag())
	{
		for(unsigned int i=map->getMaxPlayer();i--;)
			player[i]->resetData();
		UI_Window::changeAccepted();
		boHasChanged=true;
	} else
	if(UI_Window::getResetFlag())
	{
		for(unsigned int i=map->getMaxPlayer();i--;)
			player[i]->resetData();

		soup->initSoup(&start);
//		soup->setParameters(start); ??
		
		newGeneration();
		// TODO players durchlaufen;
		UI_Window::resetAccepted();
		UI_Window::changeAccepted();
		boHasChanged=true; // TODO
	}
	for(unsigned int i=map->getMaxPlayer();i--;)
		if(player[i]->checkForNeedRedraw())
		{
			setNeedRedrawMoved();
			scoreWindow->setNeedRedrawNotMoved();
			break;
		}
}

void Game::newGeneration()
{
//	unsigned int oldCode[MAX_PLAYER][MAX_LENGTH];
//	for(unsigned int i=(*start->getMap())->getMaxPlayer();i--;)
//		anarace[i]->copyCode(oldCode[i]);
			
//TODO: nach Ende eines Durchlaufs ist anarace 0, aber viele anderen Teile des Codes greifen noch drauf zu!!
	if(soup->newGeneration(anarace, &unit))
		for(unsigned int i=map->getMaxPlayer();i--;)
		{
//			if(anarace[i]->isDifferent(oldCode[i]))//, oldMarker[i]))
				boHasChanged=true;
			player[i]->assignAnarace(anarace[i]);
		}
// update score window
	for(unsigned int i = map->getMaxPlayer();i--;)
		{
			if(anarace[i]->getTimer()==0)
			{
				scoreWindow->setMode(i, SCORE_FULFILL_MODE);
				scoreWindow->setGoalComplete(i, anarace[i]->getGoalPercentage());
			}
			else
			{
				scoreWindow->setMode(i, SCORE_TIME_MODE);
				scoreWindow->setScore(i, anarace[i]->getRealTimer());
				scoreWindow->setGoalComplete(i, anarace[i]->getGoalPercentage());
			}
		}

	
}


void Game::updateRectangles(const unsigned int maxGame)
{
//	for(unsigned int = MAX_PLAYER;i--;) TODO
//		player[i]->updateRectangles(maxGame); // TODO
}

const bool Game::isOptimizing() const
{
	return(scoreWindow->isOptimizing());
}

void Game::setOptimizing(const bool opt)
{
	scoreWindow->setOptimizing(opt);
}

void Game::resetData()
{
	boHasChanged = true;
}

//virtual machen
//resetData, updateItems, assignAnarace, checkOrders

