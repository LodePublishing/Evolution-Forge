#include "player.h"
#include "debug.h"
#include "location.h"

void EXPORT PLAYER::setUseMapSettings(int use)
{
#ifdef _SCC_DEBUG
	if((use!=0)&&(use!=1))
	{
		debug.toLog(0,"DEBUG: (PLAYER::setUseMapSettings): Variable use out of range [%i].",use);
		return;
	}
#endif
	useMapSettings=use;
	wasChanged();
};

int EXPORT PLAYER::getUseMapSettings()
{
#ifdef _SCC_DEBUG
    if((useMapSettings!=0)&&(useMapSettings!=1))
    {
		debug.toLog(0,"DEBUG: (PLAYER::getUseMapSettings): Variable useMapSettings not initialized [%i].",useMapSettings);
        return(0);
    }
#endif
	wasChanged();
	return(useMapSettings);
};

GOAL_ENTRY* EXPORT PLAYER::getGoal()
{
#ifdef _SCC_DEBUG
        if(!goal)
        {
                debug.toLog(0,"DEBUG: (PLAYER::getGoal): Variable not initialized [%i].",goal);
                return(0);
        }
#endif
	return(goal);
};

void EXPORT PLAYER::wasChanged()
{
	changed=1;
};

int EXPORT PLAYER::isChanged()
{
	if((changed)||(goal->isChanged()))
		return(1);
	else return(0);
};

void EXPORT PLAYER::changeAccepted()
{
	if(goal->isChanged())
		goal->changeAccepted();
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

int EXPORT PLAYER::setTime(int time)
{
#ifdef _SCC_DEBUG
        if((time<0)||(time>MAX_TIME))
        {
                debug.toLog(0,"DEBUG: (PLAYER::setTime): Value [%i] out of range.",time);
                return(0);
        }
#endif
    wasChanged(); //TODO
	timer=time;
    return(1);
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
	race=num;
	return(1);
};

int EXPORT PLAYER::setHarvest(const int* mining, const int* gasing)
{
#ifdef _SCC_DEBUG
	if((!mining)||(!gasing))
	{
		debug.toLog(0,"DEBUG: (PLAYER::setHarvest): Variable is not initialized.");
		return(0); //evtl auf initialized pruefen
	}
#endif
	wasChanged();
	basicMineralHarvestPerSecond=mining;
	basicGasHarvestPerSecond=gasing;
	return(1);
};

int EXPORT PLAYER::setGoal(GOAL_ENTRY* goal)
{
#ifdef _SCC_DEBUG
	if(!goal->getInitialized())
	{
		debug.toLog(0,"DEBUG: (PLAYER::setGoal): Variable not initialized.");
		return(0);
	};
#endif
	wasChanged();
	this->goal=goal;
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
    setUseMapSettings(0);
	basicMineralHarvestPerSecond=0;
	basicGasHarvestPerSecond=0;
	goal=0;
	initialized=1; //?
};

PLAYER::PLAYER()
{
	resetData();
};

PLAYER::~PLAYER()
{
};

