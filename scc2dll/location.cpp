#include "location.h"
#include "string.h"
#include "debug.h"

int MAP_LOCATION_BASIC::setName(const char* line)
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

int MAP_LOCATION_BASIC::setMineralDistance(int num)
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

int MAP_LOCATION_BASIC::setAvailible(int player, int unit, int num)
{
#ifdef _SCC_DEBUG
        if((player<0)||(player>=MAX_PLAYER))
        {
                debug.toLog(0,"DEBUG: (MAP_LOCATION_BASIC::setAvailible): Value player [%i] out of range.",player);
                return(0);
        }
        if((unit<0)||(unit>=UNIT_TYPE_COUNT))
        {
                debug.toLog(0,"DEBUG: (MAP_LOCATION_BASIC::setAvailible): Value unit [%i] out of range.",unit);
                return(0);
        }
        if((num<0)||(num>=MAX_SUPPLY))
        {
                debug.toLog(0,"DEBUG: (MAP_LOCATION_BASIC::setAvailible): Value num [%i] out of range.",num);
                return(0);
        }
#endif
	availible[player][unit]=num;
	return(1);
};

int MAP_LOCATION_BASIC::getAvailible(int player,int unit)
{
#ifdef _SCC_DEBUG
        if((player<0)||(player>=MAX_PLAYER))
        {
                debug.toLog(0,"DEBUG: (MAP_LOCATION_BASIC::getAvailible): Value player [%i] out of range.",player);
                return(0);
        }
        if((unit<0)||(unit>=UNIT_TYPE_COUNT))
        {
                debug.toLog(0,"DEBUG: (MAP_LOCATION_BASIC::getAvailible): Value unit [%i] out of range.",unit);
                return(0);
        }
        if((availible[player][unit]<0)||(availible[player][unit]>MAX_SUPPLY))
        {
                debug.toLog(0,"DEBUG: (MAP_LOCATION_BASIC::getAvailible): Value availible[%i][%i] out of range [%i].",player,unit,availible[player][unit]);
                return(0);
        }
#endif
	return(availible[player][unit]);
};



int MAP_LOCATION_BASIC::setForce(int player, int unit, int num)
{
#ifdef _SCC_DEBUG
        if((player<0)||(player>=MAX_PLAYER))
        {
                debug.toLog(0,"DEBUG: (MAP_LOCATION_BASIC::setForce): Value player [%i] out of range.",player);
                return(0);
        }
        if((unit<0)||(unit>=UNIT_TYPE_COUNT))
        {
                debug.toLog(0,"DEBUG: (MAP_LOCATION_BASIC::setForce): Value unit [%i] out of range.",unit);
                return(0);
        }
        if((num<0)||(num>=MAX_SUPPLY))
        {
                debug.toLog(0,"DEBUG: (MAP_LOCATION_BASIC::setForce): Value num [%i] out of range.",num);
                return(0);
        }
#endif
	force[player][unit]=num;
	return(1);
};

int MAP_LOCATION_BASIC::getForce(int player,int unit)
{
#ifdef _SCC_DEBUG
        if((player<0)||(player>=MAX_PLAYER))
        {
                debug.toLog(0,"DEBUG: (MAP_LOCATION_BASIC::getForce): Value player [%i] out of range.",player);
                return(0);
        }
        if((unit<0)||(unit>=UNIT_TYPE_COUNT))
        {
                debug.toLog(0,"DEBUG: (MAP_LOCATION_BASIC::getForce): Value unit [%i] out of range.",unit);
                return(0);
        }
        if((force[player][unit]<0)||(force[player][unit]>MAX_SUPPLY))
        {
                debug.toLog(0,"DEBUG: (MAP_LOCATION_BASIC::getForce): Value force[%i][%i] out of range [%i].",player,unit,force[player][unit]);
                return(0);
        }
#endif
	return(force[player][unit]);
};



int MAP_LOCATION_BASIC::setDistance(int num,int dist)
{
#ifdef _SCC_DEBUG
        if((num<0)||(num>=MAX_LOCATIONS))
        {
                debug.toLog(0,"DEBUG: (MAP_LOCATION::setDistance): Value num [%i] out of range.",num);
                return(0);
        }
        if((distance[num]<0)||(distance[num]>500))
        {
                debug.toLog(0,"DEBUG: (MAP_LOCATION::setDistance): Value distance[%i] out of range [%i].",num,distance[num]);
                return(0);
        }
#endif
	distance[num]=dist;
	return(1);
};

const char* MAP_LOCATION_BASIC::getName()
{
	return(name);
};

int MAP_LOCATION_BASIC::getMineralDistance()
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

int MAP_LOCATION_BASIC::getDistance(int num)
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

void MAP_LOCATION_BASIC::resetForce()
{
	for(int i=0;i<MAX_PLAYER;i++)
		for(int j=0;j<UNIT_TYPE_COUNT;j++)
		{
			force[i][j]=0;
			availible[i][j]=0;
		}
};

void MAP_LOCATION_BASIC::resetData()
{
	strcpy(name,"Error!");
	mineralDistance=0;
	for(int i=0;i<MAX_LOCATIONS;i++)
		setDistance(i,0);
	resetForce();
};


MAP_LOCATION_BASIC::MAP_LOCATION_BASIC()
{
	resetData();
};

MAP_LOCATION_BASIC::~MAP_LOCATION_BASIC()
{
};

//-------------------------------


