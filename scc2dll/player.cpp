#include "player.h"
#include "debug.h"
#include "location.h"

//void EXPORT PLAYER::adjustGoals(const int adjust, const UNIT* unit)
//{
//	goal.adjustGoals(adjust, unit);
//};

void EXPORT PLAYER::setGoal(const GOAL_ENTRY* goal)
{
	this->goal.copy(goal);
};

void EXPORT PLAYER::copy(const PLAYER* player)
{
	setMinerals(player->getMinerals());
	setGas(player->getGas());
	setTimer(player->getTimer());
	setRace(player->getRace()); //problem: uebergebene Player ist der von der geladenen MAP, also die Mapsettings... deshalb muss auch in /scc2/force.cpp zuerst die map, dann die Rasse gewechselt werden
	setHarvest(player->getHarvestMining(), player->getHarvestGasing());
	setNeedSupply(player->getNeedSupply());
	setHaveSupply(player->getHaveSupply());
//	setGoal(player->getGoal());	
};

const int* PLAYER::getHarvestMining() const
{
#ifdef _SCC_DEBUG
    if(!basicMineralHarvestPerSecond)
    {
        toLog("DEBUG: (PLAYER::getHarvestMining): Variable not initialized.");
        return(0);
    }
#endif
	return basicMineralHarvestPerSecond;
};

const int* PLAYER::getHarvestGasing() const
{
#ifdef _SCC_DEBUG
    if(!basicGasHarvestPerSecond)
    {
        toLog("DEBUG: (PLAYER::getHarvestGasing): Variable not initialized.");
        return(0);
    }
#endif
	return basicGasHarvestPerSecond;
};

const GOAL_ENTRY* EXPORT PLAYER::getGoal() const
{
	return(&goal);
};

void EXPORT PLAYER::wasChanged()
{
	changed=true;
};

const bool EXPORT PLAYER::isChanged() const
{
	return((changed)||(goal.isChanged()));
};
/*
void EXPORT PLAYER::changeAccepted()
{
	if(goal.isChanged())
		goal.changeAccepted();
	changed=0;
};*/



const int EXPORT PLAYER::getNeedSupply() const
{
#ifdef _SCC_DEBUG
	if((needSupply<0)||(needSupply>MAX_SUPPLY))
	{
		toLog("DEBUG: (PLAYER::getNeedSupply): Variable not initialized.");
		return(0);
	}
#endif
	return(needSupply);
};

const int EXPORT PLAYER::getHaveSupply() const
{
#ifdef _SCC_DEBUG
	if((haveSupply<0)||(haveSupply>MAX_SUPPLY))
	{
		toLog("DEBUG: (PLAYER::getHaveSupply): Variable not initialized.");
		return(0);
	}
#endif
	return(haveSupply);
};

const int EXPORT PLAYER::getMinerals() const
{
#ifdef _SCC_DEBUG
	if((minerals<0)||(minerals>MAX_MINS))
	{
		toLog("DEBUG: (PLAYER::getMinerals): Variable not initialized.");
		return(0);
	}
#endif
	return(minerals);
};

const int EXPORT PLAYER::getGas() const
{
#ifdef _SCC_DEBUG
	if((gas<0)||(gas>MAX_GAS))
	{
		toLog("DEBUG: (PLAYER::getGas): Variable not initialized.");
		return(0);
	}
#endif
	return(gas);
};

const int EXPORT PLAYER::getTimer() const
{
#ifdef _SCC_DEBUG
	if((timer<0)||(timer>MAX_TIME))
	{
		toLog("DEBUG: (PLAYER::getTimer): Variable not initialized.");
		return(0);
	}
#endif
	return(timer);
};

/*const int EXPORT PLAYER::getStartPosition() const
{
#ifdef _SCC_DEBUG
	if((startPosition<0)||(startPosition>=MAX_LOCATIONS))
	{
		toLog("DEBUG: (PLAYER::getPosition): Variable not initialized.");
		return(0);
	}
#endif
	return(startPosition);
};*/

const eRace EXPORT PLAYER::getRace() const
{
	return(race);
};

const int EXPORT PLAYER::getBasicMineralHarvestPerSecond(const int workerCount) const
{
	int tmp=workerCount;
	if(tmp>44) tmp=44;
#ifdef _SCC_DEBUG
	if(tmp<0)
	{
		toLog("DEBUG: (PLAYER::getBasicMineralHarvestPerSecond): Variable out of range.");
		return(0);
	}
	if((basicMineralHarvestPerSecond[tmp]<0)||(basicMineralHarvestPerSecond[tmp]>5000))
	{
		toLog("DEBUG: (PLAYER::getBasicMineralHarvestPerSecond): Variable not initialized.");
		return(0);
	}
#endif
	return(basicMineralHarvestPerSecond[tmp]);
};

const int EXPORT PLAYER::getBasicGasHarvestPerSecond(const int workerCount) const
{
	int tmp=workerCount;
	if(tmp>4) tmp=4;
#ifdef _SCC_DEBUG
	if(tmp<0)
	{
		toLog("DEBUG: (PLAYER::getBasicGasHarvestPerSecond): Variable out of range.");
		return(0);
	}
	if((basicGasHarvestPerSecond[tmp]<0)||(basicGasHarvestPerSecond[tmp]>5000))
	{
		toLog("DEBUG: (PLAYER::getBasicGasHarvestPerSecond): Variable not initialized.");
		return(0);
	}
#endif
	return(basicGasHarvestPerSecond[tmp]);
};

void EXPORT PLAYER::setMinerals(const int minerals)
{
#ifdef _SCC_DEBUG
	if((minerals<0)||(minerals>MAX_MINS))
	{
		toLog("DEBUG: (PLAYER::setMinerals): Value out of range.");
		return;
	}
#endif
	this->minerals=minerals;
	wasChanged();
};

void EXPORT PLAYER::setGas(const int gas)
{
#ifdef _SCC_DEBUG
	if((gas<0)||(gas>MAX_GAS))
	{
		toLog("DEBUG: (PLAYER::setGas): Value out of range.");
		return;
	}
#endif
	this->gas=gas;
	wasChanged();
};

void EXPORT PLAYER::setTimer(const int timer)
{
#ifdef _SCC_DEBUG
	if((timer<0)||(timer>MAX_TIME))
	{
		toLog("DEBUG: (PLAYER::setTimer): Value out of range.");
		return;
	}
#endif
	this->timer=timer;
	wasChanged();
};

/*void EXPORT PLAYER::setStartPosition(const int startPosition)
{
#ifdef _SCC_DEBUG
	if((startPosition<0)||(startPosition>=MAX_LOCATIONS))
	{
		toLog("DEBUG: (PLAYER::setPosition): Value out of range.");
		return;
	}
#endif
	this->startPosition=startPosition;
	wasChanged();
};*/

void EXPORT PLAYER::setRace(const eRace race)
{
	if(goal.getRace()!=race)
	{
		pStats=&(stats[this->race=race][0]);
		goal.setRace(race);
		wasChanged();
	}; // TODO
};

const UNIT_STATISTICS* EXPORT PLAYER::getpStats() const
{
#ifdef _SCC_DEBUG
	if(!pStats)
	{
		toLog("DEBUG: (PLAYER::getpStats): Variable pstats is not initialized.");
		return(0); //evtl auf initialized pruefen
	}
#endif
	return(pStats);
};
		

void EXPORT PLAYER::setHarvest(const int* mining, const int* gasing)
{
#ifdef _SCC_DEBUG
	if((!mining)||(!gasing))
	{
		toLog("DEBUG: (PLAYER::setHarvest): Variable mining/gasing is not initialized.");
		return;
	}
#endif
	basicMineralHarvestPerSecond=mining;
	basicGasHarvestPerSecond=gasing;
	wasChanged();
};

void EXPORT PLAYER::setNeedSupply(const int supply)
{
#ifdef _SCC_DEBUG
	if((supply<0)||(supply>MAX_SUPPLY))
	{
		toLog("DEBUG: (PLAYER::setNeedSupply): Value out of range.");
		return;
	}
#endif
	this->supply=supply;
	wasChanged();
};

void EXPORT PLAYER::setHaveSupply(const int maxSupply)
{
#ifdef _SCC_DEBUG
	if((maxSupply<0)||(maxSupply>MAX_SUPPLY))
	{
		toLog("DEBUG: (PLAYER::setHaveSupply): Value out of range.");
		return;
	}
#endif
	this->maxSupply=maxSupply;
	wasChanged();
};


const bool EXPORT PLAYER::getInitialized() const
{
	return(initialized);
};

void EXPORT PLAYER::resetData()
{
	setMinerals(0);
	setGas(0);
	setNeedSupply(0);
	setHaveSupply(0);
	setTimer(0);
	setPosition(0);
	setRace(TERRA);
	basicMineralHarvestPerSecond=0;
	basicGasHarvestPerSecond=0;
	initialized=true; //?
	changed=false;
	pStats=0;
};

PLAYER::PLAYER()
{
	resetData();
};

PLAYER::~PLAYER()
{
};

