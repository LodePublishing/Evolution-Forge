#include "player.h"
#include "debug.h"
#include "location.h"


int EXPORT PLAYER::getSupply()
{
	if((supply<0)||(supply>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PLAYER::getSupply): Variable not initialized [%i].",supply);
		return(0);
	}
	return(supply);
};

int EXPORT PLAYER::getMaxSupply()
{
	if((maxSupply<0)||(maxSupply>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PLAYER::getMaxSupply): Variable not initialized [%i].",maxSupply);
		return(0);
	}
	return(maxSupply);
};

int EXPORT PLAYER::getMins()
{
	if((mins<0)||(mins>MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (PLAYER::getMins): Variable not initialized [%i].",mins);
		return(0);
	}
	return(mins);
};

int EXPORT PLAYER::getGas()
{
	if((gas<0)||(gas>MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (PLAYER::getGas): Variable not initialized [%i].",gas);
		return(0);
	}
	return(gas);
};

int EXPORT PLAYER::getTimer()
{
	if((timer<0)||(timer>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (PLAYER::getTimer): Variable not initialized [%i].",timer);
		return(0);
	}
	return(timer);
};

int EXPORT PLAYER::getPosition()
{
	if((position<0)||(position>MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PLAYER::getPosition): Variable not initialized [%i].",position);
		return(0);
	}
	return(position);
};

int EXPORT PLAYER::getRace()
{
	if((race<0)||(race>2))
	{
		debug.toLog(0,"DEBUG: (PLAYER::getRace): Variable not initialized [%i].",race);
		return(0);
	}
	return(race);
};

int EXPORT PLAYER::getBasicMineralHarvestPerSecond(int num)
{
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
	return(basicMineralHarvestPerSecond[num]);
};

int EXPORT PLAYER::getBasicGasHarvestPerSecond(int num)
{
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
	return(basicGasHarvestPerSecond[num]);
};

int EXPORT PLAYER::setMins(int num)
{
	if((num<0)||(num>MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (PLAYER::setMins): Value [%i] out of range.",num);
		return(0);
	}
	mins=num;
	return(1);
};

int EXPORT PLAYER::setGas(int num)
{
	if((num<0)||(num>MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (PLAYER::setGas): Value [%i] out of range.",num);
		return(0);
	}
	gas=num;
	return(1);
};

int EXPORT PLAYER::setTimer(int num)
{
	if((num<0)||(num>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (PLAYER::setTimer): Value [%i] out of range.",num);
		return(0);
	}
	timer=num;
	return(1);
};

int EXPORT PLAYER::setPosition(int num)
{
	if((num<0)||(num>MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PLAYER::setPosition): Value [%i] out of range.",num);
		return(0);
	}
	position=num;
	return(1);
};

int EXPORT PLAYER::setRace(int num)
{
	if((num<0)||(num>2))
	{
		debug.toLog(0,"DEBUG: (PLAYER::setRace): Value [%i] out of range.",num);
		return(0);
	}
	race=num;
	return(1);
};

int EXPORT PLAYER::setHarvest(const int* mining, const int* gasing)
{
	if((!mining)||(!gasing))
	{
		debug.toLog(0,"DEBUG: (PLAYER::setHarvest): Variable is not initialized.");
		return(0); //evtl auf initialized pruefen
	}
	basicMineralHarvestPerSecond=mining;
	basicGasHarvestPerSecond=gasing;
	return(1);
};

int EXPORT PLAYER::setGoal(GOAL_ENTRY* goal)
{
	if(!goal->getInitialized())
	{
		debug.toLog(0,"DEBUG: (PLAYER::setGoal): Variable not initialized.");
		return(0);
	};
	this->goal=goal;
	return(1);
};


int EXPORT PLAYER::setSupply(int num)
{
	if((num<0)||(num>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PLAYER::setSupply): Value [%i] out of range.",num);
		return(0);
	}
	supply=num;
	return(1);
};

int EXPORT PLAYER::setMaxSupply(int num)
{
	if((num<0)||(num>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PLAYER::setMaxSupply): Value [%i] out of range.",num);
		return(0);
	}
	maxSupply=num;
	return(1);
};


int EXPORT PLAYER::getInitialized()
{
	return(initialized);
};


PLAYER::PLAYER()
{
	setMins(0);
	setGas(0);
	setSupply(0);
	setMaxSupply(0);
	setTimer(0);
	setPosition(0);
	setRace(0);
	initialized=1;
};

PLAYER::~PLAYER()
{
};

