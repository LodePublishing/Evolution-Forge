#include "basicmap.hpp"

BASIC_MAP::BASIC_MAP():
	name("ERROR"),
	maxLocations(0),
	maxPlayer(0)
{
	resetData();
}

BASIC_MAP::~BASIC_MAP()
{ }

void BASIC_MAP::resetData()
{
	for(int i=MAX_LOCATIONS;i--;)
		location[i].resetData();
	for(int i=MAX_PLAYER;i--;)
		startPosition[i] = 0;
	name = "ERROR";
	maxLocations = 0;
	maxPlayer = 0;
}

void BASIC_MAP::calculateLocationsDistances()
{
	for(int i=1;i<MAX_LOCATIONS;i++)
		location[i].calculateDistances();
}

// -------------------------------
// ------ GET/SET FUNCTIONS ------
// -------------------------------

const std::string& BASIC_MAP::getName() const
{
	return(name);
}

const unsigned int BASIC_MAP::getMaxLocations() const
{
#ifdef _SCC_DEBUG
	if(maxLocations > MAX_LOCATIONS) {
		toLog("DEBUG: (BASIC_MAP::getMaxLocations): Variable not initialized.");return(0);
	}
#endif
	return(maxLocations);
}

const unsigned int BASIC_MAP::getMaxPlayer() const
{
#ifdef _SCC_DEBUG
	if((maxPlayer < 1) || (maxPlayer > MAX_PLAYER)) {
		toLog("DEBUG: (BASIC_MAP::getMaxPlayer): Variable not initialized.");return(0);
	}
#endif
	return(maxPlayer);
}

void BASIC_MAP::setName(const std::string& map_name)
{
	name = map_name;
}

void BASIC_MAP::setLocationName(const unsigned int location_number, const std::string& location_name)
{
#ifdef _SCC_DEBUG
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (BASIC_MAP::setLocationName): Value out of range.");return;
	}
#endif
	location[location_number].setName(location_name);
}

const unsigned int BASIC_MAP::getLocationMineralPatches(const unsigned int location_number) const
{
#ifdef _SCC_DEBUG
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (BASIC_MAP::getLocationMineralPatches): Value out of range.");return(0);	 
	}
#endif
	return(location[location_number].getMineralPatches());
}

const unsigned int BASIC_MAP::getLocationVespeneGeysirs(const unsigned int location_number) const
{
#ifdef _SCC_DEBUG
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (BASIC_MAP::getLocationVespeneGeysirs): Value out of range.");return(0);	 
	}
#endif
	return(location[location_number].getVespeneGeysirs());
}

void BASIC_MAP::setLocationMineralPatches(const unsigned int location_number, const unsigned int mineral_patches)
{
#ifdef _SCC_DEBUG
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (BASIC_MAP::setLocationMineralPatches): Value out of range.");return;	 
	}
#endif
	location[location_number].setMineralPatches(mineral_patches);
}

void BASIC_MAP::setLocationVespeneGeysirs(const unsigned int location_number, const unsigned int vespene_geysirs)
{
#ifdef _SCC_DEBUG
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (BASIC_MAP::setLocationVespeneGeysirs): Value out of range.");return;	 
	}
#endif
	location[location_number].setVespeneGeysirs(vespene_geysirs);
}

void BASIC_MAP::setLocationMineralDistance(const unsigned int location_number, const unsigned int mineral_distance)
{
#ifdef _SCC_DEBUG
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (BASIC_MAP::setMineralDistance): Value out of range.");return;
	}
#endif
	location[location_number].setMineralDistance(mineral_distance);
}
	
void BASIC_MAP::setLocationDistance(const unsigned int location_number, const std::list<std::string> distances)
{
#ifdef _SCC_DEBUG
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (MAP::setLocationDistance): Value out of range.");return;
	}
	if(location_number >= distances.size()) {
		toLog("DEBUG: (MAP::setLocationDistance): Too many locations.");return;
	}
#endif
	std::list<std::string>::const_iterator i=distances.begin();
	unsigned int j=0;
	while(i!=distances.end())
	{
		location[location_number].setDistance(j, atoi(i->c_str()));
		i++;
		j++;
	}
#ifdef _SCC_DEBUG
	if( j < location_number ) {
		toLog("DEBUG: (MAP::setLocationDistance): Not enough locations.");return;
	}
#endif		
}

void BASIC_MAP::setMaxLocations(const unsigned int max_locations)
{
#ifdef _SCC_DEBUG
	if(max_locations > MAX_LOCATIONS) {
		toLog("DEBUG: (BASIC_MAP::setMaxLocations): Value out of range.");return;
	}
#endif
	maxLocations = max_locations;
}

void BASIC_MAP::setMaxPlayer(const unsigned int max_player)
{
#ifdef _SCC_DEBUG
	if((max_player < 1) || (max_player > MAX_PLAYER)) {
		toLog("DEBUG: (BASIC_MAP::setMaxPlayer): Value out of range.");return;
	}
#endif
	maxPlayer = max_player;
}

const MAP_LOCATION* BASIC_MAP::getLocation(const unsigned int location_number) const
{
#ifdef _SCC_DEBUG
	if(location_number >= MAX_LOCATIONS) {
		toLog("DEBUG: (BASIC_MAP::getLocation): Value out of range.");return(0);
	}
#endif
	return(&location[location_number]);
}


	

