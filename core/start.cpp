#include "start.hpp"

START::START(UNIT (*start_force)[MAX_LOCATIONS]):
	tmpmap(NULL),
	startCondition(NULL),
	tmpgoal(NULL),
	totalForce(),
	startForce(start_force),
	currentGoal(),
	startPosition(0),
	playerRace(TERRA),
	pStats(NULL),//&(stats[playerRace][0])),
	
	mapInitialized(false),
	startConditionInitialized(false),
	totalForceInitialized(false),
	startForceInitialized(true), // TODO
	tmpgoalInitialized(false),
	currentGoalInitialized(false),
	startPositionInitialized(false),
	playerRaceInitialized(false),
	pStatsInitialized(false)
{
	for(unsigned int i = MAX_RACES;i--;)
	{
		harvest[i] = NULL;
		harvestSpeedInitialized[i] = false;
	}
}


START::~START()
{ }

const bool START::isCompletelyInitialized() const
{
	return(  mapInitialized && startConditionInitialized && totalForceInitialized && startForceInitialized && tmpgoalInitialized && currentGoalInitialized && startPositionInitialized && playerRaceInitialized && pStatsInitialized);										
}

void START::fillAsNeutralPlayer()
{
#ifdef _SCC_DEBUG
	if(!mapInitialized) {
		toErrorLog("DEBUG (START::fillAsNeutralPlayer()): Variable tmpmap was not initialized.");return;
	}
	if(!startForceInitialized) {
		toErrorLog("DEBUG (START::fillAsNeutralPlayer()): Variable startForce was not initialized.");return;
	}
#endif
	for(unsigned int j = tmpmap->getMaxLocations();j--;)
	{
		(*startForce)[j].setTotal(MINERAL_PATCH, tmpmap->getLocationMineralPatches(j));
		(*startForce)[j].setTotal(VESPENE_GEYSIR, tmpmap->getLocationVespeneGeysirs(j));
		(*startForce)[j].setAvailible(MINERAL_PATCH, tmpmap->getLocationMineralPatches(j));
		(*startForce)[j].setAvailible(VESPENE_GEYSIR, tmpmap->getLocationVespeneGeysirs(j));
	}

	totalForce.resetData();
	
//	startForce[GLOBAL] = *startCondition->getUnit(GLOBAL);
//	startForce[startPosition] = *startCondition->getUnit(0);
	// TODO bei mehreren Spielern!
	for(unsigned int j=1;j<tmpmap->getMaxLocations(); ++j)
		for(unsigned int k=UNIT_TYPE_COUNT;k--;)
			if((*startForce)[j].getTotal(k))
			{
//				GROUP g;
//				g.player=i;g.location=j;
//				g.count=startForce[i][j].getTotal(k);
//				g.type=k;
//				group[j].push_back();
				totalForce.addTotal(k, (*startForce)[j].getTotal(k));
			}
	totalForceInitialized = true;
}

// has to be called at the end of each starcondition change!!
void START::fillAsActivePlayer()
{
#ifdef _SCC_DEBUG
	if(!mapInitialized) {
		toErrorLog("DEBUG (START::fillAsActivePlayer()): Variable tmpmap was not initialized.");return;
	}
	if(!startConditionInitialized) {
		toErrorLog("DEBUG (START::fillAsActivePlayer()): Variable startCondition was not initialized.");return;
	}
	if(!startPositionInitialized) {
		toErrorLog("DEBUG (START::fillAsActivePlayer()): Variable startPosition was not initialized.");return;
	}
#endif
	totalForce.resetData();
	
	(*startForce)[GLOBAL] = *startCondition->getUnit(GLOBAL);
	(*startForce)[startPosition] = *startCondition->getUnit(0);
	// TODO bei mehreren Spielern!
	for(unsigned int j=1;j<tmpmap->getMaxLocations(); ++j)
		for(unsigned int k=UNIT_TYPE_COUNT;k--;)
			if((*startForce)[j].getTotal(k))
			{
//				GROUP g;
//				g.player=i;g.location=j;
//				g.count=startForce[i][j].getTotal(k);
//				g.type=k;
//				group[j].push_back();
				totalForce.addTotal(k, (*startForce)[j].getTotal(k));
			}
	totalForceInitialized = true;
}


// -------------------------------
// ------ GET/SET FUNCTIONS ------
// -------------------------------

const bool START::assignGoal(const GOAL_ENTRY* goal)
{
#ifdef _SCC_DEBUG
	if(!totalForceInitialized) {
		toErrorLog("DEBUG (START::assignGoal()): Variable totalForce was not initialized.");return(false);
	}
#endif
	if((tmpgoalInitialized)&&(currentGoalInitialized)&&(goal == tmpgoal)&&(currentGoal == *goal))
		return(false);
	tmpgoal = goal;
	currentGoal = *goal;
	currentGoal.adjustGoals(true, &totalForce); // TODO

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
	return(true);
}

const bool START::assignMap(const BASIC_MAP* map) // TODO
{
#ifdef _SCC_DEBUG
	if(!map) {
		toErrorLog("DEBUG: (START::assignMap): Value map out of range.");return(false);
	}
#endif
	if((mapInitialized) && (map == tmpmap))
		return(false);
	tmpmap = map;
	mapInitialized=true;
	return(true); 
}

const bool START::setPlayerRace(const eRace race)
{
	if((playerRaceInitialized) && (race == playerRace))
		return(false);
	playerRace = race;
	pStats = &(stats[race][0]);

	playerRaceInitialized = true;
	pStatsInitialized = true;
	return(true);
}


//void START::copyStartForce(UNIT (*target)[MAX_LOCATIONS]) const
//{
//	memcpy(target, &(startForce[0]), sizeof(startForce));
//}


/*

START::START(const START& start) :
	tmpmap(start.tmpmap),
	startConditionsInitialized(start.startConditionsInitialized),
	mapInitialized(start.mapInitialized)	
{
        for(unsigned int i = MAX_PLAYER; i--;)
        {
		currentGoal[i] = start.currentGoal[i];
		pCurrentGoal[i] = &(currentGoal[i]);
                startCondition[i] = start.startCondition[i];
                tmpgoal[i] = start.tmpgoal[i];
                startPosition[i] = start.startPosition[i];
                playerRace[i] = start.playerRace[i];
                pStats[i] = start.pStats[i];
        }

maxplayer rausfinden
	for(unsigned int i = 1; i < MAX_PLAYER; ++i)
		
	
        for(unsigned int i = MAX_RACES;i--;)
        {
                harvest[i] = start.harvest[i];
        }
}*/


