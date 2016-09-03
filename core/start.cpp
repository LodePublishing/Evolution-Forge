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
	pStats(&(stats[playerRace][0])),
	startConditionsInitialized(false),
	mapInitialized(false)
{
	for(unsigned int i = MAX_RACES;i--;)
		harvest[i] = NULL;
}
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

START::~START()
{ }




void START::fillAsNeutralPlayer()
{
#ifdef _SCC_DEBUG
	if(!mapInitialized) {
		toErrorLog("DEBUG: (START::fillAsMap): No map was initialized.");return;
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

}

// has to be called at the end of each starcondition change!!
void START::fillAsActivePlayer()
{
#ifdef _SCC_DEBUG
	if(!mapInitialized) {
		toErrorLog("DEBUG: (START::fillAsActivePlayer): No map was initialized.");return;
	}
	if(!startConditionsInitialized) {
		toErrorLog("DEBUG: (START::fillAsActivePlayer): Not all startConditions were initialized.");return;
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
}


// -------------------------------
// ------ GET/SET FUNCTIONS ------
// -------------------------------

const bool START::setPlayerRace(const eRace race)
{
	if(race == playerRace)
		return(false);
	playerRace = race; // TODO
	pStats = &(stats[race][0]);
	return(true);
}

const bool START::assignGoal(const GOAL_ENTRY* goal)
{
	// TODO
//	if((goal == tmpgoal)&&(currentGoal == *goal))
//		return(false);
	tmpgoal = goal;
	currentGoal = *goal;
	currentGoal.adjustGoals(true, &totalForce); // TODO
	return(true);
}

const bool START::assignStartCondition(const START_CONDITION* start_condition)
{
	if(startCondition == start_condition)
		return(false);
	startCondition = start_condition;
	if(mapInitialized)
		startConditionsInitialized=true;
	else toErrorLog("ERROR");
		
	return(true);
}

const bool START::assignMap(const BASIC_MAP* map) // TODO
{
	if(map == tmpmap)
		return(false);
#ifdef _SCC_DEBUG
	if(!map) {
		toErrorLog("DEBUG: (START::assignMap): Value map out of range.");return(false);
	}
#endif
	tmpmap = map;
	mapInitialized=true;
	// initialize Map???
	// player 0 ?
	return(true); 
}

//void START::copyStartForce(UNIT (*target)[MAX_LOCATIONS]) const
//{
//	memcpy(target, &(startForce[0]), sizeof(startForce));
//}



