#include "player.h"
#include "debug.h"
#include "location.h"

void EXPORT PLAYER::adjustGoals(int adjust, UNITS* units)
{
	goal.adjustGoals(adjust, units);
};

void EXPORT PLAYER::setGoal(GOAL_ENTRY* goal)
{
	this->goal.copy(goal);
};

void EXPORT PLAYER::copy(PLAYER* player)
{
	setMins(player->getMins());
	setGas(player->getGas());
	setTimer(player->getTimer());
	setRace(player->getRace()); //problem: uebergebene Player ist der von der geladenen MAP, also die Mapsettings... deshalb muss auch in /scc2/force.cpp zuerst die map, dann die Rasse gewechselt werden
	setHarvest(player->getHarvestMining(),player->getHarvestGasing());
	setSupply(player->getSupply());
	setMaxSupply(player->getMaxSupply());
	setGoal(player->getGoal());	
};

const int* PLAYER::getHarvestMining()
{
#ifdef _SCC_DEBUG
	if(!basicMineralHarvestPerSecond)
	{
		debug.toLog(0,"DEBUG: (PLAYER::getHarvestMining): Variable not initialized [%i].",basicMineralHarvestPerSecond);
		return(0);
	}
#endif
	return basicMineralHarvestPerSecond;
};

const int* PLAYER::getHarvestGasing()
{
#ifdef _SCC_DEBUG
	if(!basicGasHarvestPerSecond)
	{
		debug.toLog(0,"DEBUG: (PLAYER::getHarvestGasing): Variable not initialized [%i].",basicGasHarvestPerSecond);
		return(0);
	}
#endif
	return basicGasHarvestPerSecond;
};

GOAL_ENTRY* EXPORT PLAYER::getGoal()
{
	return(&goal);
};

void EXPORT PLAYER::wasChanged()
{
	changed=1;
};

int EXPORT PLAYER::isChanged()
{
	if((changed)||(goal.isChanged()))
		return(1);
	else return(0);
};

void EXPORT PLAYER::changeAccepted()
{
	if(goal.isChanged())
		goal.changeAccepted();
	changed=0;
};



int EXPORT PLAYER::getSupply()
{
#ifdef _SCC_DEBUG
	if((supply<0)||(supply>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PLAYER::getSupply): Variable not initialized [%i].",supply);
		return(0);
	}
#endif
	return(supply);
};

int EXPORT PLAYER::getMaxSupply()
{
#ifdef _SCC_DEBUG
	if((maxSupply<0)||(maxSupply>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PLAYER::getMaxSupply): Variable not initialized [%i].",maxSupply);
		return(0);
	}
#endif
	return(maxSupply);
};

int EXPORT PLAYER::getMins()
{
#ifdef _SCC_DEBUG
	if((mins<0)||(mins>MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (PLAYER::getMins): Variable not initialized [%i].",mins);
		return(0);
	}
#endif
	return(mins);
};

int EXPORT PLAYER::getGas()
{
#ifdef _SCC_DEBUG
	if((gas<0)||(gas>MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (PLAYER::getGas): Variable not initialized [%i].",gas);
		return(0);
	}
#endif
	return(gas);
};

int EXPORT PLAYER::getTimer()
{
#ifdef _SCC_DEBUG
	if((timer<0)||(timer>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (PLAYER::getTimer): Variable not initialized [%i].",timer);
		return(0);
	}
#endif
	return(timer);
};

int EXPORT PLAYER::getPosition()
{
#ifdef _SCC_DEBUG
	if((position<0)||(position>MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PLAYER::getPosition): Variable not initialized [%i].",position);
		return(0);
	}
#endif
	return(position);
};

int EXPORT PLAYER::getRace()
{
#ifdef _SCC_DEBUG
	if((race<0)||(race>2))
	{
		debug.toLog(0,"DEBUG: (PLAYER::getRace): Variable not initialized [%i].",race);
		return(0);
	}
#endif
	return(race);
};

int EXPORT PLAYER::getBasicMineralHarvestPerSecond(int num)
{
#ifdef _SCC_DEBUG
	if(num<0)
	{
		debug.toLog(0,"DEBUG: (PLAYER::getBasicMineralHarvestPerSecond): Variable [%i] out of range.",num);
		return(0);
	}
	if(num>44) num=44;
	if((basicMineralHarvestPerSecond[num]<0)||(basicMineralHarvestPerSecond[num]>5000))
	{
		debug.toLog(0,"DEBUG: (PLAYER::getBasicMineralHarvestPerSecond): Variable not initialized [%i].",basicMineralHarvestPerSecond[num]);
		return(0);
	}
#endif
	return(basicMineralHarvestPerSecond[num]);
};

int EXPORT PLAYER::getBasicGasHarvestPerSecond(int num)
{
#ifdef _SCC_DEBUG
	if(num<0)
	{
		debug.toLog(0,"DEBUG: (PLAYER::getBasicGasHarvestPerSecond): Variable [%i] out of range.",num);
		return(0);
	}
	if(num>4) num=4;
	if((basicGasHarvestPerSecond[num]<0)||(basicGasHarvestPerSecond[num]>5000))
	{
		debug.toLog(0,"DEBUG: (PLAYER::getBasicGasHarvestPerSecond): Variable not initialized [%i].",basicGasHarvestPerSecond[num]);
		return(0);
	}
#endif
	return(basicGasHarvestPerSecond[num]);
};

int EXPORT PLAYER::setMins(int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (PLAYER::setMins): Value [%i] out of range.",num);
		return(0);
	}
#endif
	wasChanged();
	mins=num;
	return(1);
};

int EXPORT PLAYER::setGas(int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (PLAYER::setGas): Value [%i] out of range.",num);
		return(0);
	}
#endif
	wasChanged();
	gas=num;
	return(1);
};

int EXPORT PLAYER::setTimer(int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (PLAYER::setTimer): Value [%i] out of range.",num);
		return(0);
	}
#endif
	wasChanged();
	timer=num;
	return(1);
};

int EXPORT PLAYER::setPosition(int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PLAYER::setPosition): Value [%i] out of range.",num);
		return(0);
	}
#endif
	wasChanged();
	position=num;
	return(1);
};

int EXPORT PLAYER::setRace(int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>2))
	{
		debug.toLog(0,"DEBUG: (PLAYER::setRace): Value [%i] out of range.",num);
		return(0);
	}
#endif
	wasChanged();
	pStats=&(stats[race=num][0]);
	goal.setRace(num);
	return(1);
};

const UNIT_STATISTICS* EXPORT PLAYER::getpStats()
{
#ifdef _SCC_DEBUG
	if(!pStats)
	{
		debug.toLog(0,"DEBUG: (PLAYER::getpStats): Variable pstats is not initialized.");
		return(0); //evtl auf initialized pruefen
	}
#endif
	return(pStats);
};
		

int EXPORT PLAYER::setHarvest(const int* mining, const int* gasing)
{
#ifdef _SCC_DEBUG
	if((!mining)||(!gasing))
	{
		debug.toLog(0,"DEBUG: (PLAYER::setHarvest): Variable mining/gasing is not initialized.");
		return(0); //evtl auf initialized pruefen
	}
#endif
	wasChanged();
	basicMineralHarvestPerSecond=mining;
	basicGasHarvestPerSecond=gasing;
	return(1);
};

int EXPORT PLAYER::setSupply(int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PLAYER::setSupply): Value [%i] out of range.",num);
		return(0);
	}
#endif
	wasChanged();
	supply=num;
	return(1);
};

int EXPORT PLAYER::setMaxSupply(int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PLAYER::setMaxSupply): Value [%i] out of range.",num);
		return(0);
	}
#endif
	wasChanged();
	maxSupply=num;
	return(1);
};


int EXPORT PLAYER::getInitialized()
{
#ifdef _SCC_DEBUG
	if((initialized<0)||(initialized>1))
	{
		debug.toLog(0,"DEBUG: (PLAYER::getInitialized): Variable initialized not initialized [%i].",initialized);
		return(0);
	}
#endif
	return(initialized);
};

void EXPORT PLAYER::resetData()
{
	setMins(0);
	setGas(0);
	setSupply(0);
	setMaxSupply(0);
	setTimer(0);
	setPosition(0);
	setRace(0);
	basicMineralHarvestPerSecond=0;
	basicGasHarvestPerSecond=0;
	initialized=1; //?
	pStats=0;
};

PLAYER::PLAYER()
{
	resetData();
};

PLAYER::~PLAYER()
{
};

