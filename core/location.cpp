#include "location.hpp"

MAP_LOCATION::MAP_LOCATION():
	name("ERROR"),
	units(),
	resourceDistance(RACE::MAX_RESOURCE_TYPES)
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
	resourceDistance.resize(RACE::MAX_RESOURCE_TYPES);
	for(unsigned int i = MAX_LOCATIONS;i--;)
	{
		distance[i] = 9999;
		nearest[i] = 0;
	}
}

MAP_LOCATION::MAP_LOCATION(const MAP_LOCATION& object) :
	name(object.name),
	units(object.units),
	resourceDistance(object.resourceDistance)
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
	units = object.units;
	resourceDistance = object.resourceDistance;
	for(unsigned int i = MAX_LOCATIONS;i--;)
	{
		distance[i] = object.distance[i];
		nearest[i] = object.nearest[i];
	}
	return(*this);					
}


void MAP_LOCATION::calculateDistances(const unsigned int this_location_number)
{
	for(unsigned int i=MAX_LOCATIONS;i--;)
		nearest[i] = 0;
	nearest[0] = this_location_number;
	for(unsigned int step = 1; step < getMaxLocations(); ++step)
	{
		unsigned int min = 200;
		for(unsigned int loc = 1;loc < getMaxLocations(); ++loc)
			if(getDistance(loc) < min)
			{
				bool alreadyProcessed = false;
				for(unsigned int k=1; k<step; ++k)
					if(nearest[k] == loc) 
						alreadyProcessed = true;
				if(!alreadyProcessed)
				{
					min = getDistance(loc);
					nearest[step] = loc;
				}
			}
	}
}

unsigned int MAP_LOCATION::maxLocations = 0;

