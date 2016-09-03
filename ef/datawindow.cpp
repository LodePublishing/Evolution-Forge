#include "datawindow.hpp"
#include "../core/database.hpp"

DataBaseWindow::DataBaseWindow(UI_Object* database_parent) :
	UI_Window(database_parent, DATABASE_WINDOW_TITLE_STRING, theme.lookUpGlobalRect(DATABASE_WINDOW), theme.lookUpGlobalMaxHeight(DATABASE_WINDOW), NOT_SCROLLED),
	dataList(),
	dataListWindow(new UI_Window(this, NULL_STRING, theme.lookUpGlobalRect(DATALIST_WINDOW), theme.lookUpGlobalMaxHeight(DATALIST_WINDOW), SCROLLED)),
	currentRace(0),
	currentBuildOrder(999),
	currentGoal(NULL),
	game(NULL),
	needUpdate(true)
{
//	dataListWindow->Hide();
}

DataBaseWindow::~DataBaseWindow()
{
	std::list<DataBaseEntry*>::iterator i = dataList.begin();
	while(i!=dataList.end())
	{
		delete *i;
		i = dataList.erase(i);
	}
	delete dataListWindow;
}

void DataBaseWindow::mouseHasLeft()
{}

void DataBaseWindow::updateItemPositions()
{
	unsigned int height = 1;
	for(std::list<DataBaseEntry*>::iterator i = dataList.begin(); i != dataList.end(); ++i)
		if((*i)->isShown())
		{
			(*i)->setOriginalPosition(Point(0, height*15));
			++height;
		}
}

#include <sstream>
void DataBaseWindow::updateList()
{
	{
		std::list<DataBaseEntry*>::iterator i = dataList.begin(); 
		while(i != dataList.end())
		{
			delete *i;
			i = dataList.erase(i);
		}
	}
	for(unsigned int r = 3; r--;)
	{
		unsigned int goal_count = database.getGoalCount(r); 
		{
		// -1 is 'clear list'
			DataBaseEntry* entry = new DataBaseEntry(dataListWindow, Rect(Point(), Size(100, 12)), Size(0,0), DO_NOT_ADJUST, GAME::lookUpGameString(GAME::FIRST_RACE_STRING + r), DATA_RACE_ENTRY, goal_count-1);
			entry->Show();
			dataList.push_back(entry);
		}
		for(unsigned int i = 1; i < goal_count; i++)
		{
			const GOAL_ENTRY* my_goal = database.getGoal(r, i);
			unsigned int bo_count = database.getBuildOrderCount(r, my_goal);
			{
				DataBaseEntry* entry = new DataBaseEntry(dataListWindow, Rect(Point(), Size(100, 12)), Size(0,0), DO_NOT_ADJUST, my_goal->getName(), DATA_GOAL_ENTRY, bo_count);
				entry->myBO = 0;
				entry->myGoal = i;
				entry->myRace = r;
				entry->Hide();
				dataList.push_back(entry);
			}
			for(unsigned int j = 0; j < bo_count; j++)
			{
				BUILD_ORDER* my_bo = database.getBuildOrder(r, my_goal, j);
				DataBaseEntry* entry = new DataBaseEntry(dataListWindow, Rect(Point(), Size(100, 12)), Size(0,0), DO_NOT_ADJUST, my_bo->getName(), DATA_BO_ENTRY, coreConfiguration.getMaxTime() - my_bo->getTime());
				entry->myBO = j;
				entry->myGoal = i;
				entry->myRace = r;
				entry->Hide();
				dataList.push_back(entry);
			}
		}
	}
	updateItemPositions();
	needUpdate = false;
}

void DataBaseWindow::activateGame()
{
	toInitLog("* " + UI_Object::theme.lookUpString(START_CREATING_GAME_STRING));
//	TODO 'NUR VIEW' Mode machen... bodiagram, bgwindow ok, playerentry nur Zeit (aber in gross), bowindow ohne controls, forcewindow ohne controls
	if(game) 
		delete game;
	game = new Game(getParent(), database.getMap(0), 1, 2);
	game->setStartPosition(0, 1);
	toInitLog("* " + UI_Object::theme.lookUpString(START_ASSIGNING_HARVEST_SPEED_STRING));
//	game->setHarvestSpeed(1, TERRA, database.getHarvestSpeed(TERRA, 0));
//	game->setHarvestSpeed(1, PROTOSS, database.getHarvestSpeed(PROTOSS, 0));
//	game->setHarvestSpeed(1, ZERG, database.getHarvestSpeed(ZERG, 0));
	toInitLog("* " + UI_Object::theme.lookUpString(START_SHOWING_GAME_STRING));
	game->Show();

	game->setPlayerInitMode(0, INITIALIZED);
	game->assignRace(0, currentRace);
//	game->initSoup();
//	toInitLog("* " + UI_Object::theme.lookUpString(START_MEASURING_FITNESS_STRING));
//	game->newGeneration();
}


void DataBaseWindow::reloadOriginalSize()
{
	setOriginalRect(theme.lookUpGlobalRect(DATABASE_WINDOW));
	setMaxHeight(theme.lookUpGlobalMaxHeight(DATABASE_WINDOW));
	dataListWindow->setOriginalRect(theme.lookUpGlobalRect(DATALIST_WINDOW));
	dataListWindow->setMaxHeight(theme.lookUpGlobalMaxHeight(DATALIST_WINDOW));

	UI_Window::reloadOriginalSize();
}

void DataBaseWindow::resetData()
{}

void DataBaseWindow::process()
{
	if(!isShown())
	{
		if(game)
			game->Hide();
		return;
	} else if(game)
		game->Show();

	if(database.wereBosChanged() || database.wereGoalsChanged())
		needUpdate = true;		
			
	UI_Window::process();

	if(!isMouseInside()) 
		mouseHasLeft();

	if(needUpdate)
		updateList();
	bool reorder = false;
	bool was_loaded = false;
	for(std::list<DataBaseEntry*>::iterator i = dataList.begin(); i != dataList.end(); ++i)
	{
		if((*i)->isShown())
		switch((*i)->getEntryType())
		{
			case DATA_RACE_ENTRY:
			{
				std::list<DataBaseEntry*>::iterator j = i;
				++j;
				for(;(j!=dataList.end()) && ((*j)->getEntryType() != DATA_RACE_ENTRY);++j)
					if((*j)->getEntryType() == DATA_GOAL_ENTRY)
					{
						if(((*i)->isCurrentlyActivated()) && (!(*j)->isShown()))
						{
							(*j)->Show();
							reorder = true;
						} else 
						if((!(*i)->isCurrentlyActivated()) && ((*j)->isShown()))
						{
							(*j)->Hide();
							std::list<DataBaseEntry*>::iterator k = j;
							++k;
							for(;(k != dataList.end()) && ((*k)->getEntryType() == DATA_BO_ENTRY); ++k)
								(*k)->Hide();
							reorder = true;
						}
							
					}
			}break;
			case DATA_GOAL_ENTRY:
			{
				std::list<DataBaseEntry*>::iterator j = i;
				++j;
				for(; (j != dataList.end()) && ((*j)->getEntryType() == DATA_BO_ENTRY); ++j)
				{
					if(((*i)->isCurrentlyActivated()) && (!(*j)->isShown()))
					{
						(*j)->Show();
						reorder = true;
					} else 
					if((!(*i)->isCurrentlyActivated()) && ((*j)->isShown()))
					{
						(*j)->Hide();
						reorder = true;
					}
					if((*j)->isClicked() && (*j)->isCurrentlyActivated())
					{
						currentBuildOrder = (*j)->myBO;
						currentGoal = (*i)->myGoal;
						currentRace = (*i)->myRace;
						activateGame();
						game->doReset();
						game->assignGoal(0, (*i)->myGoal);
						game->loadBuildOrder(0, (*j)->myBO);
						was_loaded = true;
//						setNeedRedrawMoved(); TODO
					}
				}
			}break;	
			default:break;
		}
	}
	
	if(was_loaded)
	for(std::list<DataBaseEntry*>::iterator i = dataList.begin(); i != dataList.end(); ++i)
		if((*i)->getEntryType()==DATA_GOAL_ENTRY)
		{
			std::list<DataBaseEntry*>::iterator j = i;
			++j;
			while((j != dataList.end())&&((*j)->getEntryType()==DATA_BO_ENTRY))
			{
				if(((*j)->isCurrentlyActivated()) && (((*j)->myBO != currentBuildOrder) || ((*i)->myGoal != currentGoal)))
					(*j)->forceUnpress();
				++j;
			}
		}
	
	for(std::list<DataBaseEntry*>::iterator i = dataList.begin(); i != dataList.end(); ++i)
	{
		if((*i)->isJustChecked())
		{
			std::list<DataBaseEntry*>::iterator j = i;
			++j;
			for(;j != dataList.end(); ++j)
			{
						
				if( (((*i)->getEntryType() == DATA_GOAL_ENTRY) && ((*j)->getEntryType() == DATA_BO_ENTRY) && ((*i)->myGoal == (*j)->myGoal) && ((*i)->myRace == (*j)->myRace)) ||
				(((*i)->getEntryType() == DATA_RACE_ENTRY) && ((*j)->getEntryType() == DATA_BO_ENTRY) && ((*i)->myRace == (*j)->myRace)) ||
				(((*i)->getEntryType() == DATA_RACE_ENTRY) && ((*j)->getEntryType() == DATA_GOAL_ENTRY) && ((*i)->myRace == (*j)->myRace)))
					(*j)->check((*i)->isCurrentlyActivated());
//				TODO
			}
		}
	}
	
	
//				if((!was_loaded) && ((*j)->isCurrentlyHighlighted()) && (((*j)->myBO != currentBuildOrder) || ((*i)->myGoal != currentGoal)))
//				{
//					if((*j)->isClicked())
//											
//				}

	
// TODO: geklickte Eintraege immer nehmen, de-klicken ermoeglichen, beim klicken andere entklicken
	
	if(reorder)
		updateItemPositions();
}

void DataBaseWindow::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Window::draw(dc);
}

void DataBaseWindow::newGeneration()
{
	if(game)
		game->newGeneration();
}

