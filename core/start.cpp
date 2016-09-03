#include "start.hpp"

START::START(UNIT (&start_force)[MAX_LOCATIONS]):
	tmpmap(NULL),
	startCondition(NULL),
	tmpgoal(NULL),
	startForce(start_force),
	currentGoal(NULL),
	startPosition(0),
	playerRace(0),
	pStats(NULL),
	
	mapInitialized(false),
	startConditionInitialized(false),
	startForceInitialized(false),
	tmpgoalInitialized(false),
	currentGoalInitialized(false),
	startPositionInitialized(false),
	playerRaceInitialized(false),
	pStatsInitialized(false)
{}


START::~START()
{ }

const bool START::isCompletelyInitialized() const
{
	return(  mapInitialized && startConditionInitialized && startForceInitialized && tmpgoalInitialized && currentGoalInitialized && startPositionInitialized && playerRaceInitialized && pStatsInitialized);										
}

#include <sstream>
// has to be called at the end of each starcondition change!!
void START::fillMapWithStartUnits()
{
// startposition:
	startForce[GLOBAL] = (*getStartCondition())->getUnit(GLOBAL);
	startForce[getStartPosition()] = (*getStartCondition())->getUnit(GLOBAL);

// now add the neutral units: (mineral patches, geysirs etc.)
	
	for(unsigned int j = 1; j<(*getMap())->getMaxLocations(); ++j) // oder bei = 0 anfangen?
		for(unsigned int k = RACE::UNIT_TYPE_COUNT; k--;)
			if((*getMap())->getLocation(j)->getUnits().getTotal(k)>0)
			{
				startForce[j].modifyTotal(k, (*getMap())->getLocation(j)->getUnits().getTotal(k));
				startForce[j].modifyAvailible(k, (*getMap())->getLocation(j)->getUnits().getTotal(k));
				startForce[GLOBAL].modifyTotal(k, (*getMap())->getLocation(j)->getUnits().getTotal(k));
				startForce[GLOBAL].modifyAvailible(k, (*getMap())->getLocation(j)->getUnits().getTotal(k));
			}
	startForceInitialized = true;
}


void START::readjustGoals()
{
#ifdef _SCC_DEBUG
	if(!startForceInitialized) {
		toErrorLog("DEBUG (START::assignGoal()): Variable StartForce was not initialized.");return;
	}
	if(!currentGoalInitialized) {
		toErrorLog("DEBUG (START::readjustGoal()): currentGoal not yet initialized."); return;
	}
#endif
	currentGoal->adjustGoals(startForce[getStartPosition()]);
}

// -------------------------------
// ------ GET/SET FUNCTIONS ------
// -------------------------------

const bool START::assignGoal(const GOAL_ENTRY* goal)
{
	if((tmpgoalInitialized) && (currentGoalInitialized) && (goal != NULL) && (goal == tmpgoal) && (currentGoal != NULL) && (*currentGoal == *goal))
		return(false);
#ifdef _SCC_DEBUG
	if(!startForceInitialized) {
		toErrorLog("DEBUG (START::assignGoal()): Variable StartForce was not initialized.");return(false);
	}
#endif
	tmpgoal = goal;
	delete currentGoal;
	currentGoal = new GOAL_ENTRY(*goal);
	currentGoal->adjustGoals(startForce[getStartPosition()]);
			//(*getStartCondition())->getUnit(GLOBAL)); // TODO

	tmpgoalInitialized = true;
	currentGoalInitialized = true;
	return(true);
}

const bool START::assignStartCondition(const START_CONDITION* start_condition)
{
#ifdef _SCC_DEBUG
	if(start_condition == NULL) {
		toErrorLog("DEBUG (START::assignStartCondition()): Value start_condition was not initialized.");return(false);
	}
#endif
	if((startConditionInitialized) && (startCondition == start_condition))
		return(false);
	startCondition = start_condition;
		
	startConditionInitialized = true;
	startForceInitialized = false;
	return(true);
}

const bool START::assignMap(const BASIC_MAP* map) // TODO
{
#ifdef _SCC_DEBUG
	if(map == NULL) {
		toErrorLog("DEBUG (START::assignMap()): Value map was not initialized.");return(false);
	}
#endif
	if((mapInitialized) && (map == tmpmap))
		return(false);
	tmpmap = map;
	mapInitialized = true;
	startForceInitialized = false;
	return(true); 
}

const bool START::setPlayerRace(const unsigned int race)
{
#ifdef _SCC_DEBUG
	if(race >= GAME::MAX_RACES) {
		toErrorLog("DEBUG (START::setPlayerRace()): race out of range.");return(false);
	}
#endif
	if((playerRaceInitialized) && (race == playerRace))
		return(false);
	playerRace = race;
	pStats = &(GAME::race[race].stats);

	playerRaceInitialized = true;
	pStatsInitialized = true;
	return(true);
}


