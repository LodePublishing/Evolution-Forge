#include "location.h"
#include "string.h"
#include "debug.h"

int MAP_LOCATION_BASIC::setName(const char* line)
{
	if(!line)
		return(0);
	strcpy(name,line);
	return(1);
};

int MAP_LOCATION_BASIC::setMineralDistance(int num)
{
	if((num<0)||(num>5))
	{
		debug.toLog(0,"DEBUG: (MAP_LOCATION::setMineralDistance): Value [%i] out of range.",num);
		return(0);
	}
	mineralDistance=num;
	return(1);
};

int MAP_LOCATION_BASIC::setDistance(int num,int dist)
{
	if((num<0)||(num>=MAX_LOCATIONS)||(dist<0)||(dist>500))
	{
		debug.toLog(0,"DEBUG: (MAP_LOCATION::setDistance): Value [%i/%i] out of range.",num,dist);
		return(0);
	}
	distance[num]=dist;
	return(1);
};

const char* MAP_LOCATION_BASIC::getName()
{
	return(name);
};

int MAP_LOCATION_BASIC::getMineralDistance()
{
	if((mineralDistance<0)||(mineralDistance>5))
	{
		debug.toLog(0,"DEBUG: (MAP_LOCATION::getMineralDistance): Variable not initialized [%i].",mineralDistance);
		return(0);
	}
	return(mineralDistance);
};

int MAP_LOCATION_BASIC::getDistance(int num)
{
	if((num<0)||(num>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (MAP_LOCATION::getDistance): Value [%i] out of range.",num);
		return(0);
	}
	if((distance[num]<0)||(distance[num]>500))
	{
		debug.toLog(0,"DEBUG: (MAP_LOCATION::getDistance): Variable not initialized [%i].",distance[num]);
		return(0);
	}
	return(distance[num]);
}


MAP_LOCATION_BASIC::MAP_LOCATION_BASIC()
{
	int i,j;
	strcpy(name,"Error!");
	mineralDistance=0;
	for(i=0;i<MAX_LOCATIONS;i++)
		setDistance(i,0);
	for(i=0;i<MAX_PLAYER;i++)
		for(j=0;j<UNIT_TYPE_COUNT;j++)
			force[i][j]=0;
};

MAP_LOCATION_BASIC::~MAP_LOCATION_BASIC()
{
};

//-------------------------------


