// -----------------------------------------------------------------------------------------
// DATABASE holds all map, harvest, goal and start-condition date for other classes to read
// In addition it saves and loads these data from/to files.
// -----------------------------------------------------------------------------------------

#include "database.hpp"

#include <map>
#include <sstream>
#include <fstream>
#include <iostream>

DATABASE::DATABASE():
	loadedMap(),
	goalDataInitialized(false),
	buildOrderDataInitialized(true), // we don't need bos...
	startConditionDataInitialized(false),
	mapDataInitialized(false),
//	harvestDataInitialized(false),
	goalsWereChanged(false),
	bosWereChanged(false),
	mapsWereChanged(false),
	startConditionsWereChanged(false)
{}

DATABASE::~DATABASE()
{
	for(std::vector<BASIC_MAP*>::iterator i = loadedMap.begin(); i!=loadedMap.end(); ++i)
		delete *i;
	loadedMap.clear();
	for(unsigned int j = 0; j < GAME::MAX_RACES; ++j)
	{
		for(std::vector<START_CONDITION*>::iterator i = loadedStartCondition[j].begin(); i!=loadedStartCondition[j].end(); ++i)
			delete *i;
		for(std::vector<GOAL_ENTRY*>::iterator i = loadedGoal[j].begin(); i!=loadedGoal[j].end(); ++i)
			delete *i;
		for(std::vector<BUILD_ORDER*>::iterator i = loadedBuildOrder[j].begin(); i!=loadedBuildOrder[j].end(); ++i)
			delete *i;
	
		loadedStartCondition[j].clear();
		loadedGoal[j].clear();
		loadedBuildOrder[j].clear();
	}
}

void DATABASE::changeAccepted() 
{
	goalsWereChanged = false;
	bosWereChanged = false;
	mapsWereChanged = false;
	startConditionsWereChanged = false;
}

void DATABASE::init() // TODO spaeter in den Konstruktor
{
// TODO Elemente loeschen...
// TODO was wenn MAX_RACES veraendert wurde? => Speicherleck!
	loadedGoal.resize(GAME::MAX_RACES);
	loadedBuildOrder.resize(GAME::MAX_RACES);
	loadedStartCondition.resize(GAME::MAX_RACES);
	loadedHarvestSpeed.resize(GAME::MAX_RACES);
}

void DATABASE::addDefaultGoal(const unsigned int race)
{
#ifdef _SCC_DEBUG
	if(race >= GAME::MAX_RACES) {
		toErrorLog("DEBUG (DATABASE::addDefaultGoal()): Value race out of range.");return;
	}
#endif
	GOAL_ENTRY* goal = new GOAL_ENTRY("Clear list", race);
	loadedGoal[goal->getRace()].push_back(goal);
	goalDataInitialized = true;
}


/*void DATABASE::addHarvestSpeed(const unsigned int race, HARVEST_SPEED* harvest_speed)
{
#ifdef _SCC_DEBUG
	if(race >= GAME::MAX_RACES) {
		toErrorLog("DEBUG (DATABASE::addHarvestSpeed()): Value race out of range.");return;
	}
#endif
	loadedHarvestSpeed[race].push_back(harvest_speed);
	harvestDataInitialized = true;
	for(unsigned int i = GAME::MAX_RACES; i--;)
		if(loadedHarvestSpeed[i].size() == 0)
		{
			harvestDataInitialized = false;
			break;
		}
}
*/

void DATABASE::addGoal(GOAL_ENTRY* my_goal)
{
	loadedGoal[my_goal->getRace()].push_back(my_goal);
	goalDataInitialized = true;
	goalsWereChanged = true;
}

void DATABASE::addMap(BASIC_MAP* basic_map)
{
	loadedMap.push_back(basic_map);
	mapDataInitialized = true;
	mapsWereChanged = true;
}


void DATABASE::addBuildOrder(BUILD_ORDER* build_order)
{
	loadedBuildOrder[build_order->getRace()].push_back(build_order);
	buildOrderDataInitialized = true;
	bosWereChanged = true;
}

void DATABASE::addStartCondition(START_CONDITION* start_condition)
{
	start_condition->adjustResearches();
	start_condition->adjustSupply();
	loadedStartCondition[start_condition->getRace()].push_back(start_condition);
	startConditionDataInitialized = true;
	for(unsigned int i = GAME::MAX_RACES; i--;)
		if(loadedStartCondition[i].size() == 0)
		{
			startConditionDataInitialized = false;
			break;
		}
	startConditionsWereChanged = true;
}

const unsigned int DATABASE::getBuildOrderCount(const unsigned int race, const GOAL_ENTRY* goal) const 
{
#ifdef _SCC_DEBUG
	if(race >= GAME::MAX_RACES) {
		toErrorLog("DEBUG (DATABASE::getBuildOrderCount()): Value race out of range.");return(0);
	}
#endif
	unsigned int count = 0;
	for(unsigned int i = loadedBuildOrder[race].size(); i--;)
	{
		if(loadedBuildOrder[race][i]->getGoal() == *goal)
			count++;
	}
	return(count);
}


BUILD_ORDER* DATABASE::getBuildOrder(const unsigned int race, const GOAL_ENTRY* goal, const unsigned int build_order) const
{
#ifdef _SCC_DEBUG
	if(build_order >= getBuildOrderCount(race, goal)) {
		toErrorLog("DEBUG WARNING (DATABASE::getBuildOrder()): Value 'build_order' out of range.");return(NULL);
	}
#endif
	unsigned int count = 0;
	for(unsigned int i = 0; i < loadedBuildOrder[race].size(); i++)
	{
		if(!(loadedBuildOrder[race][i]->getGoal() == *goal))
			continue;
		if(count == build_order)
			return(loadedBuildOrder[race][i]);
		count++;
	}
	return(NULL);
}


// --------------------------------------
// ------ END OF GET/SET FUNCTIONS ------
// --------------------------------------

DATABASE database;
