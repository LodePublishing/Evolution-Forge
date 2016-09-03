#include "datawindow.hpp"
#include "../core/database.hpp"

DataBaseWindow::DataBaseWindow(UI_Object* database_parent) :
	UI_Window(database_parent, DATABASE_WINDOW_TITLE_STRING, theme.lookUpGlobalRect(DATABASE_WINDOW), theme.lookUpGlobalMaxHeight(DATABASE_WINDOW), NOT_SCROLLED),
	dataList(),
	dataListWindow(new UI_Window(this, NULL_STRING, theme.lookUpGlobalRect(DATALIST_WINDOW), theme.lookUpGlobalMaxHeight(DATALIST_WINDOW), SCROLLED)),
	raceMenuButton(new UI_Button(this, Rect(Point(5, FONT_SIZE+8), Size(UI_Object::theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH)/2, 0)), Size(0, 0), TAB_BUTTON, false, STATIC_BUTTON_MODE, CHOOSE_RACE_STRING, DO_NOT_ADJUST, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
	raceMenu(new RaceMenu(this, Rect(15, 15, 0, 0), Size(0,0), DO_NOT_ADJUST )),
	// TODO staticText 'race' oder Beschreibung
	currentRace(TERRA),
	game(NULL),
	raceMenuOpenedExternally(false),
	lastBoLoaded(999),
	lastGoalLoaded(NULL)
{
//	dataListWindow->Hide();
	raceMenu->setPositionParent(raceMenuButton);

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
	delete raceMenuButton;
	delete raceMenu;
}

void DataBaseWindow::mouseHasLeft()
{
	if(!raceMenuOpenedExternally)
	{
		if(raceMenuButton->isCurrentlyActivated())
		{
			raceMenuButton->forceUnpress();	
			raceMenu->close();
		}
	}
}

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
	unsigned int goal_count = database.getGoalCount(currentRace);
	for(unsigned int i = 1; i < goal_count; i++)
	{
		const GOAL_ENTRY* my_goal = database.getGoal(currentRace, i);
		unsigned int bo_count = database.getBuildOrderCount(currentRace, my_goal);
		{
			DataBaseEntry* entry = new DataBaseEntry(dataListWindow, Rect(Point(), Size(100, 12)), Size(0,0), DO_NOT_ADJUST, my_goal->getName(), true, bo_count);
			entry->myBO = 0;
			entry->myGoal = i;
			dataList.push_back(entry);
		}
		for(unsigned int j = 0; j < bo_count; j++)
		{
			BUILD_ORDER* my_bo = database.getBuildOrder(currentRace, my_goal, j);
			DataBaseEntry* entry = new DataBaseEntry(dataListWindow, Rect(Point(), Size(100, 12)), Size(0,0), DO_NOT_ADJUST, my_bo->getName(), false);
			entry->myBO = j;
			entry->myGoal = i;
			entry->Hide();
			dataList.push_back(entry);
		}
	}
	updateItemPositions();
}

void DataBaseWindow::activateGame()
{
	toInitLog("* " + UI_Object::theme.lookUpString(START_CREATING_GAME_STRING));
//	TODO 'NUR VIEW' Mode machen... bodiagram, bgwindow ok, playerentry nur Zeit (aber in gross), bowindow ohne controls, forcewindow ohne controls
	if(game) 
		delete game;
	game = new Game(getParent(), database.getMap(0), 1, 2);
	game->setStartPosition(1, 1);
	toInitLog("* " + UI_Object::theme.lookUpString(START_ASSIGNING_HARVEST_SPEED_STRING));
	game->setHarvestSpeed(1, TERRA, database.getHarvestSpeed(TERRA, 0));
	game->setHarvestSpeed(1, PROTOSS, database.getHarvestSpeed(PROTOSS, 0));
	game->setHarvestSpeed(1, ZERG, database.getHarvestSpeed(ZERG, 0));
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

	raceMenuButton->setOriginalSize(Size(UI_Object::theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH)/2, 0));
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
	UI_Window::process();

	int assignRace=-1;
	if((assignRace = raceMenu->getPressedItem())>=0)
	{	
		raceMenuButton->updateText((eString)(TERRA_STRING+assignRace));
		raceMenuButton->forceUnpress();
		currentRace = (eRace)assignRace;
		activateGame();
		updateList();
	} else
	if(raceMenuButton->isLeftClicked())
	{
		raceMenu->open();
		if(!raceMenu->isOpen())
		{
			raceMenuButton->forceUnpress();		
			raceMenu->close();
			raceMenuOpenedExternally = false;
		} else
		{
			raceMenu->close();
			raceMenu->open();
		}
	}
	
	if(!isMouseInside()) 
		mouseHasLeft();

	bool reorder = false;
	bool was_loaded = false;
	for(std::list<DataBaseEntry*>::iterator i = dataList.begin(); i != dataList.end(); ++i)
		if((*i)->isGoal())
		{
			std::list<DataBaseEntry*>::iterator j = i;
			++j;
			while((j != dataList.end())&&(!(*j)->isGoal()))
			{
				if((!(*i)->isMarked())&&((*j)->isShown()))
				{
					(*j)->Hide();
					reorder = true;
				}
				else if(((*i)->isMarked())&&(!(*j)->isShown()))
				{
					(*j)->Show();
					reorder = true;
				}
				if((*j)->isClicked() && (*j)->isMarked())
				{
					game->assignGoal(0, (*i)->myGoal);
					game->loadBuildOrder(0, (*j)->myBO);
					was_loaded = true;
					lastBoLoaded = (*j)->myBO;
					lastGoalLoaded = (*i)->myGoal;
					setNeedRedrawMoved();
				}
				++j;
			}
		}
	if(was_loaded)
	for(std::list<DataBaseEntry*>::iterator i = dataList.begin(); i != dataList.end(); ++i)
		if((*i)->isGoal())
		{
			std::list<DataBaseEntry*>::iterator j = i;
			++j;
			while((j != dataList.end())&&(!(*j)->isGoal()))
			{
				if(((*j)->isMarked()) && (((*j)->myBO != lastBoLoaded)||((*i)->myGoal != lastGoalLoaded)))
					(*j)->forceUnpress();
				++j;
			}
		}
	
//				if((!was_loaded) && ((*j)->isCurrentlyHighlighted()) && (((*j)->myBO != lastBoLoaded) || ((*i)->myGoal != lastGoalLoaded)))
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

