#include "start.hpp"

START::START():
	tmpmap(NULL)	
{
	for(int i=MAX_PLAYER;i--;)
		pCurrentGoal[i]=&(currentGoal[i]);
}

START::~START()
{
}

// has to be called at the end of each starcondition change!!
void START::fillGroups()
{
	for(int i=MAX_PLAYER;i--;)
		totalForce[i].resetData();
	int maxp=tmpmap->getMaxPlayer();

	for(int j=MAX_LOCATIONS;j--;)
	{
		startForce[0][j].setTotal(MINERAL_PATCH, tmpmap->getLocationMineralPatches(j));
		startForce[0][j].setTotal(VESPENE_GEYSIR, tmpmap->getLocationVespeneGeysirs(j));
		startForce[0][j].setAvailible(MINERAL_PATCH, tmpmap->getLocationMineralPatches(j));
		startForce[0][j].setAvailible(VESPENE_GEYSIR, tmpmap->getLocationVespeneGeysirs(j));
	}
	for(int i=maxp;i--;)
	{
		startForce[i+1][0].copy(startcondition[i+1]->getUnit(0));
		startForce[i+1][startPosition[i+1]].copy(startcondition[i+1]->getUnit(0));
		for(int j=1;j<MAX_LOCATIONS;j++)
		{
			for(int k=UNIT_TYPE_COUNT;k--;)
				if(startForce[i+1][j].getTotal(k))
				{
//					GROUP g;
//					g.player=i;g.location=j;
//				    g.count=startForce[i][j].getTotal(k);
//					g.type=k;
//					group[j].push_back();
					totalForce[i+1].addTotal(k, startForce[i+1][j].getTotal(k));
				}
		}
	}
}


// -------------------------------
// ------ GET/SET FUNCTIONS ------
// -------------------------------

void START::setHarvestSpeed(const eRace race, const HARVEST_SPEED* harvest)
{
	this->harvest[race]=harvest;
}

void START::assignGoal(const unsigned int playerNum, const GOAL_ENTRY* goal)
{
#ifdef _SCC_DEBUG
    if((playerNum<1)||(playerNum>=MAX_PLAYER)) {
        toLog("DEBUG: (START::assignGoal): Value playerNum out of range.");return;
    }
#endif
	tmpgoal[playerNum]=goal;
	currentGoal[playerNum].copy(goal);
	currentGoal[playerNum].adjustGoals(&(totalForce[playerNum]));
}

void START::assignStartcondition(const unsigned int player, const START_CONDITION* startcondition)
{
	this->startcondition[player]=startcondition;
}

void START::assignMap(const BASIC_MAP* map)
{
	this->tmpmap=map;
	// initialize Map???
	// player 0 ?
}

const unsigned int START::getBasicMineralHarvestSpeedPerSecond(const unsigned int playerNum, const unsigned int worker) const // 'player' noch rausoptimieren!
{
#ifdef _SCC_DEBUG
    if((playerNum<1)||(playerNum>=MAX_PLAYER)) {
        toLog("DEBUG: (START::getCurrentGoal): Value playerNum out of range.");return(0);
    }
#endif
	return(harvest[playerRace[playerNum]]->getHarvestMineralSpeed(worker));
}

const unsigned int START::getBasicGasHarvestSpeedPerSecond(const unsigned int playerNum, const unsigned int worker) const
{
#ifdef _SCC_DEBUG
    if((playerNum<1)||(playerNum>=MAX_PLAYER)) {
        toLog("DEBUG: (START::getBasicGasHarvestSpeedPerSecond): Value playerNum out of range.");return(0);
    }
#endif
	return(harvest[playerRace[playerNum]]->getHarvestGasSpeed(worker));
			
}
				
const BASIC_MAP* const* START::getMap() const
{
	return(&tmpmap);
}

void START::copyStartForce(void* target) const
{
	memcpy(target, &(startForce[0][0]), sizeof(startForce));
}

const START_CONDITION* const* START::getStartcondition(const unsigned int playerNum) const
{
#ifdef _SCC_DEBUG
    if((playerNum<1)||(playerNum>=MAX_PLAYER)) {
        toLog("DEBUG: (START::getStartcondition): Value playerNum out of range.");return(NULL);
    }
#endif
	return(&(startcondition[playerNum]));
}

void START::setStartPosition(const unsigned int playerNum, const unsigned int startPosition)
{
#ifdef _SCC_DEBUG
    if((playerNum<1)||(playerNum>=MAX_PLAYER)) {
        toLog("DEBUG: (START::setStartPosition): Value playerNum out of range.");return;
    }
#endif
	this->startPosition[playerNum]=startPosition;
}

const eRace START::getPlayerRace(const unsigned int playerNum) const
{
#ifdef _SCC_DEBUG
    if((playerNum<1)||(playerNum>=MAX_PLAYER)) {
        toLog("DEBUG: (START::getPlayerRace): Value playerNum out of range.");return(TERRA);
    }
#endif
	return(playerRace[playerNum]);
}

void START::setPlayerRace(const unsigned int playerNum, const eRace race)
{
#ifdef _SCC_DEBUG
    if((playerNum<1)||(playerNum>=MAX_PLAYER)) {
        toLog("DEBUG: (START::setPlayerRace): Value playerNum out of range.");return;
    }
#endif
	playerRace[playerNum]=race; // TODO
	pStats[playerNum]=&(stats[race][0]);
}

GOAL_ENTRY** START::getCurrentGoal(const unsigned int playerNum)
{
#ifdef _SCC_DEBUG
	if((playerNum<1)||(playerNum>=MAX_PLAYER)) {
		toLog("DEBUG: (START::getCurrentGoal): Value playerNum out of range.");return(NULL);
	}
#endif
	return(&(pCurrentGoal[playerNum]));
}

const UNIT_STATISTICS* const* START::getpStats(const unsigned int playerNum) const
{
#ifdef _SCC_DEBUG
    if((playerNum<1)||(playerNum>=MAX_PLAYER)) {
        toLog("DEBUG: (START::getpStats): Value playerNum out of range.");return(NULL);
    }
#endif
	return(&(pStats[playerNum]));
}

