#include "location.hpp"

MAP_LOCATION::MAP_LOCATION():
	name("ERROR"),
	mineralPatches(0),
	vespeneGeysirs(0),
	mineralDistance(0)
{
	for(unsigned int i=MAX_LOCATIONS;i--;)
	{
		distance[i] = 9999;
		nearest[i] = 0;
	}
}

MAP_LOCATION::~MAP_LOCATION()
{ }

void MAP_LOCATION::resetData()
{
	name="ERROR";
	mineralDistance=0;
	for(unsigned int i = MAX_LOCATIONS;i--;)
	{
		distance[i] = 9999;
		nearest[i] = 0;
	}
}

MAP_LOCATION::MAP_LOCATION(const MAP_LOCATION& object) :
	name(object.name),
	mineralPatches(object.mineralPatches),
	vespeneGeysirs(object.vespeneGeysirs),
	mineralDistance(object.mineralDistance)
{
	for(unsigned int i = MAX_LOCATIONS;i--;)
	{
		distance[i] = object.distance[i];
		nearest[i] = object.nearest[i];
	}
}

MAP_LOCATION& MAP_LOCATION::operator=(const MAP_LOCATION& object)
{
	name = object.name;
	mineralPatches = object.mineralPatches;
	vespeneGeysirs = object.vespeneGeysirs;
	mineralDistance = object.mineralDistance;
	for(unsigned int i = MAX_LOCATIONS;i--;)
	{
		distance[i] = object.distance[i];
		nearest[i] = object.nearest[i];
	}
	return(*this);					
}


void MAP_LOCATION::calculateDistances()
{
	for(unsigned int i=MAX_LOCATIONS;i--;)
		nearest[i] = 0;
	for(unsigned int step = 1; step < getMaxLocations(); ++step)
	{
		unsigned int min = 200;
		for(unsigned int loc = 1;loc < getMaxLocations(); ++loc)
			if(getDistance(loc) < min)
			{
				bool alreadyProcessed = false;
				for(unsigned int k=1; k<step; ++k)
					if(nearest[k] == loc) alreadyProcessed = true;
				if(!alreadyProcessed)
				{
					min = getDistance(loc);
					nearest[step] = loc;
				}
			}
	}
}

unsigned int MAP_LOCATION::maxLocations = 0;

