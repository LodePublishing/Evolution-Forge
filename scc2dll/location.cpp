#include "location.h"
#include "string.h"
#include "debug.h"

int MAP_LOCATION::setName(const char* line)
{
#ifdef _SCC_DEBUG
	if(!line)
	{
		debug.toLog(0,"DEBUG: (MAP_LOCATION::setMineralDistance): Value line not initialized [%i].",line);
		return(0);
	}
#endif
	strcpy(name,line);
	return(1);
};

int MAP_LOCATION::setMineralDistance(int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>5))
	{
		debug.toLog(0,"DEBUG: (MAP_LOCATION::setMineralDistance): Value [%i] out of range.",num);
		return(0);
	}
#endif
	mineralDistance=num;
	return(1);
};

int MAP_LOCATION::setAvailible(int player, int unit, int num)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=MAX_PLAYER))
	{
		debug.toLog(0,"DEBUG: (MAP_LOCATION::setAvailible): Value player [%i] out of range.",player);
		return(0);
	}
#endif
	units[player].setAvailible(unit, num);
	return(1);
};

int MAP_LOCATION::getAvailible(int player,int unit)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=MAX_PLAYER))
	{
		debug.toLog(0,"DEBUG: (MAP_LOCATION::getAvailible): Value player [%i] out of range.",player);
		return(0);
	}
#endif
	return(units[player].getAvailible(unit));
};



int MAP_LOCATION::setForce(int player, int unit, int num)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=MAX_PLAYER))
	{
		debug.toLog(0,"DEBUG: (MAP_LOCATION::setForce): Value player [%i] out of range.",player);
		return(0);
	}
#endif
	units[player].setTotal(unit, num);
	return(1);
};

UNITS* MAP_LOCATION::getUnits(int player)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=MAX_PLAYER))
	{
		debug.toLog(0,"DEBUG: (MAP_LOCATION::getUnits): Value player [%i] out of range.",player);
		return(0);
	}
#endif
	return(&units[player]);
};

void MAP_LOCATION::copy(int player, UNITS* units)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=MAX_PLAYER))
	{
		debug.toLog(0,"DEBUG: (MAP_LOCATION::copy): Value player [%i] out of range.",player);
		return;
	}
#endif
	this->units[player].copy(units);
};

void MAP_LOCATION::copyBasic(MAP_LOCATION* location)
{
	setName(location->getName());
	setMineralDistance(location->getMineralDistance());
	for(int i=MAX_LOCATIONS;i--;)
		setDistance(i,location->getDistance(i));
};


void MAP_LOCATION::copy(MAP_LOCATION* location)
{
	for(int i=MAX_PLAYER;i--;)
		units[i].copy(location->getUnits(i));
	setName(location->getName());
	setMineralDistance(location->getMineralDistance());
	for(int i=MAX_LOCATIONS;i--;)
		setDistance(i,location->getDistance(i));
};

int MAP_LOCATION::getForce(int player,int unit)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=MAX_PLAYER))
	{
		debug.toLog(0,"DEBUG: (MAP_LOCATION::getForce): Value player [%i] out of range.",player);
		return(0);
	}
#endif
	return(units[player].getTotal(unit));
};



void MAP_LOCATION::setDistance(int target, int distance)
{
#ifdef _SCC_DEBUG
	if((target<0)||(target>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (MAP_LOCATION::setDistance): Value target [%i] out of range.",target);
		return;
	}
	if((distance<0)||(distance>500))
	{
		debug.toLog(0,"DEBUG: (MAP_LOCATION::setDistance): Value distance [%i] out of range.",target, distance);
		return;
	}
#endif
	this->distance[target]=distance;
};

const char* MAP_LOCATION::getName()
{
	return(name);
};

int MAP_LOCATION::getMineralDistance()
{
#ifdef _SCC_DEBUG
	if((mineralDistance<0)||(mineralDistance>5))
	{
		debug.toLog(0,"DEBUG: (MAP_LOCATION::getMineralDistance): Variable not initialized [%i].",mineralDistance);
		return(0);
	}
#endif
	return(mineralDistance);
};

int MAP_LOCATION::getDistance(int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (MAP_LOCATION::getDistance): Value [%i] out of range.",num);
		return(0);
	}
	if((distance[num]<0)||(distance[num]>500))
	{
		debug.toLog(0,"DEBUG: (MAP_LOCATION::getDistance): Value distance[%i] out of range [%i].",num,distance[num]);
		return(0);
	}
#endif
	return(distance[num]);
};

void MAP_LOCATION::adjustSupply(int player, int race, int& supply, int& maxSupply)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=MAX_PLAYER))
	{
		debug.toLog(0,"DEBUG: (MAP_LOCATION::adjustSupply): Value player [%i] out of range.",player);
		return;
	}
#endif
	units[player].adjustSupply(race, supply, maxSupply);
};

void MAP_LOCATION::adjustResearches(int player, int race)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=MAX_PLAYER))
	{
		debug.toLog(0,"DEBUG: (MAP_LOCATION::adjustResearches): Value player [%i] out of range.",player);
		return;
	}
#endif
	units[player].adjustResearches(race);
};


void MAP_LOCATION::resetForce()
{
	for(int i=0;i<MAX_PLAYER;i++)
		for(int j=0;j<UNIT_TYPE_COUNT;j++)
		{
			units[i].setTotal(j, 0);
			units[i].setAvailible(j, 0);
		}
};

void MAP_LOCATION::resetData()
{
	strcpy(name,"Error!");
	mineralDistance=0;
	for(int i=0;i<MAX_LOCATIONS;i++)
		setDistance(i,0);
	resetForce();
};


MAP_LOCATION::MAP_LOCATION()
{
	resetData();
};

MAP_LOCATION::~MAP_LOCATION()
{
};

//-------------------------------


