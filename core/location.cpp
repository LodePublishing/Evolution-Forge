#include "location.hpp"

//const int GLOBAL = 0;

MAP_LOCATION::MAP_LOCATION():
	name("ERROR"),
	mineralPatches(0),
	vespeneGeysirs(0),
	mineralDistance(0)
{
	resetData();
}

MAP_LOCATION::~MAP_LOCATION()
{ }

void MAP_LOCATION::resetData()
{
	name="ERROR";
	mineralDistance=0;
	for(int i=MAX_LOCATIONS;i--;)
	{
		setDistance(i,0);
		nearest[i]=0;
	}		
}

MAP_LOCATION::MAP_LOCATION(const MAP_LOCATION& object) :
	name(object.name),
	mineralPatches(object.mineralPatches),
	vespeneGeysirs(object.vespeneGeysirs),
	mineralDistance(object.mineralDistance)
{
	for(int i = MAX_LOCATIONS;i--;)
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
	for(int i = MAX_LOCATIONS;i--;)
	{
		distance[i] = object.distance[i];
		nearest[i] = object.nearest[i];
	}
	return(*this);					
}


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
	}
}

void MAP_LOCATION::setName(const std::string& location_name)
{
	name.assign(location_name);
}

const int MAP_LOCATION::getMineralPatches() const
{
	return(mineralPatches);
}

const int MAP_LOCATION::getVespeneGeysirs() const
{
	return(vespeneGeysirs);
}

void MAP_LOCATION::setMineralPatches(const int mineral_patches)
{
#ifdef _SCC_DEBUG
// TODO constant
	if((mineral_patches < 0)||(mineral_patches > 100)) {
		toLog("DEBUG: (MAP_LOCATION::setMineralPatches): Value out of range.");return;
	}
#endif
	mineralPatches = mineral_patches;
}

void MAP_LOCATION::setVespeneGeysirs(const int vespene_geysirs)
{
#ifdef _SCC_DEBUG
// TODO constantv
	if((vespene_geysirs < 0) || (vespene_geysirs > 100)) {
		toLog("DEBUG: (MAP_LOCATION::setVespeneGeysirs): Value out of range.");return;
	}
#endif
	vespeneGeysirs = vespene_geysirs;
}

void MAP_LOCATION::setMineralDistance(const int mineral_distance)
{
#ifdef _SCC_DEBUG
	if((mineral_distance < 0) || (mineral_distance > 5)) {
		toLog("DEBUG: (MAP_LOCATION::setMineralDistance): Value out of range.");return;
	}
#endif
	mineralDistance = mineral_distance;
}

const int MAP_LOCATION::getNearest(const int step) const
{
#ifdef _SCC_DEBUG
	if((step<=0)||(step>=MAX_LOCATIONS)) {
		toLog("DEBUG: (MAP_LOCATION::getNearest): Value out of range.");return(0);
	}
#endif
	return(nearest[step]);
}


void MAP_LOCATION::setDistance(const int target, const int location_distance)
{
#ifdef _SCC_DEBUG
	if((target<0)||(target>=MAX_LOCATIONS)) {
		toLog("DEBUG: (MAP_LOCATION::setDistance): Value target out of range.");return;
	}
	if((location_distance<0)||(location_distance>500)) {
		toLog("DEBUG: (MAP_LOCATION::setDistance): Value distance out of range.");return;
	}
#endif
	distance[target] = location_distance;
}

const std::string& MAP_LOCATION::getName() const
{
	return(name);
}

const int MAP_LOCATION::getMineralDistance() const
{
#ifdef _SCC_DEBUG
	if((mineralDistance<0)||(mineralDistance>5)) {
		toLog("DEBUG: (MAP_LOCATION::getMineralDistance): Variable not initialized.");return(0);
	}
#endif
	return(mineralDistance);
}

const int MAP_LOCATION::getDistance(const int location) const
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (MAP_LOCATION::getDistance): Value out of range.");return(0);
	}
	if((distance[location]<0)||(distance[location]>500)) {
		toLog("DEBUG: (MAP_LOCATION::getDistance): Value distance out of range.");return(0);
	}
#endif
	return(distance[location]);
}


