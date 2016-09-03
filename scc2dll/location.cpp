#include "location.h"
#include "string.h"
#include "debug.h"

void MAP_LOCATION::setName(const string& name)
{
	this->name=name;
};

void MAP_LOCATION::setMineralDistance(const int mineralDistance)
{
#ifdef _SCC_DEBUG
	if((mineralDistance<0)||(mineralDistance>5))
	{
		toLog("DEBUG: (MAP_LOCATION::setMineralDistance): Value out of range.");
		return;
	}
#endif
	this->mineralDistance=mineralDistance;
};

const int MAP_LOCATION::getNearest(const int step) const
{
#ifdef _SCC_DEBUG
    if((step<=0)||(step>=MAX_LOCATIONS))
    {
        toLog("DEBUG: (MAP_LOCATION::getNearest): Value out of range.");
        return(0);
    }
#endif
	return(nearest[step]);
};

void MAP_LOCATION::calculateDistances()
{
	for(int i=MAX_LOCATIONS;i--;)
		nearest[i]=0;
	for(int step=1;step<MAX_LOCATIONS;step++)
	{
		int min=200;
		for(int loc=1;loc<MAX_LOCATIONS;loc++)
			if(getDistance(loc)<min)
			{
				bool alreadyProcessed=false;
				for(int k=1;k<step;k++)
					if(nearest[k]==loc) alreadyProcessed=true;
				if(!alreadyProcessed)
				{
					min=getDistance(loc);
					nearest[step]=loc;
				}
			}
	};
};
                                                                                
void MAP_LOCATION::setDistance(const int target, const int distance)
{
#ifdef _SCC_DEBUG
	if((target<0)||(target>=MAX_LOCATIONS))
	{
		toLog("DEBUG: (MAP_LOCATION::setDistance): Value target out of range.");
		return;
	}
	if((distance<0)||(distance>500))
	{
		toLog("DEBUG: (MAP_LOCATION::setDistance): Value distance out of range.");
		return;
	}
#endif
	this->distance[target]=distance;
};

const string& MAP_LOCATION::getName() const
{
	return(name);
};

const int MAP_LOCATION::getMineralDistance() const
{
#ifdef _SCC_DEBUG
	if((mineralDistance<0)||(mineralDistance>5))
	{
		toLog("DEBUG: (MAP_LOCATION::getMineralDistance): Variable not initialized.");
		return(0);
	}
#endif
	return(mineralDistance);
};

const int MAP_LOCATION::getDistance(const int num) const
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>=MAX_LOCATIONS))
	{
		toLog("DEBUG: (MAP_LOCATION::getDistance): Value out of range.");
		return(0);
	}
	if((distance[num]<0)||(distance[num]>500))
	{
		toLog("DEBUG: (MAP_LOCATION::getDistance): Value distance out of range.");
		return(0);
	}
#endif
	return(distance[num]);
};

void MAP_LOCATION::resetData()
{
	name="ERROR";
	mineralDistance=0;
	for(int i=MAX_LOCATIONS;i--;)
	{
		setDistance(i,0);
		nearest[i]=0;
	};		
};


MAP_LOCATION::MAP_LOCATION()
{
	resetData();
};

MAP_LOCATION::~MAP_LOCATION()
{
};

//-------------------------------


