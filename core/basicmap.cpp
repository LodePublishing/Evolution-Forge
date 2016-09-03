#include "basicmap.hpp"

BASIC_MAP::BASIC_MAP():
	name("ERROR"),
	maxLocations(0),
	maxPlayer(0)
{
	resetData();
}

BASIC_MAP::~BASIC_MAP()
{
}

void BASIC_MAP::resetData()
{
	for(int i=MAX_LOCATIONS;i--;)
		location[i].resetData();
	for(int i=MAX_PLAYER;i--;)
		startPosition[i]=0;
	name="ERROR";
	maxLocations=0;
	maxPlayer=0;
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
	if(maxLocations>MAX_LOCATIONS) {
		toLog("DEBUG: (BASIC_MAP::getMaxLocations): Variable not initialized.");return(0);
	}
#endif
	return(maxLocations);
}

const unsigned int BASIC_MAP::getMaxPlayer() const
{
#ifdef _SCC_DEBUG
	if((maxPlayer<1)||(maxPlayer>MAX_PLAYER)) {
		toLog("DEBUG: (BASIC_MAP::getMaxPlayer): Variable not initialized.");return(0);
	}
#endif
	return(maxPlayer);
}

void BASIC_MAP::setName(const std::string& name)
{
	this->name=name;
}

void BASIC_MAP::setLocationName(const unsigned int location, const std::string& name)
{
#ifdef _SCC_DEBUG
	if((location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (BASIC_MAP::setLocationName): Value out of range.");return;
	}
#endif
	this->location[location].setName(name);
}

const unsigned int BASIC_MAP::getLocationMineralPatches(const unsigned int location) const
{
#ifdef _SCC_DEBUG
	if((location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (BASIC_MAP::getLocationMineralPatches): Value loc out of range.");return(0);	 
	}
#endif
	return(this->location[location].getMineralPatches());
}

const unsigned int BASIC_MAP::getLocationVespeneGeysirs(const unsigned int location) const
{
#ifdef _SCC_DEBUG
	if((location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (BASIC_MAP::getLocationVespeneGeysirs): Value loc out of range.");return(0);	 
	}
#endif
	return(this->location[location].getVespeneGeysirs());
}

void BASIC_MAP::setLocationMineralPatches(const unsigned int location, const unsigned int mineralPatches)
{
#ifdef _SCC_DEBUG
	if((location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (BASIC_MAP::setLocationMineralPatches): Value loc out of range.");return;	 
	}
#endif
	this->location[location].setMineralPatches(mineralPatches);
}

void BASIC_MAP::setLocationVespeneGeysirs(const unsigned int location, const unsigned int vespeneGeysirs)
{
#ifdef _SCC_DEBUG
	if((location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (BASIC_MAP::setLocationVespeneGeysirs): Value loc out of range.");return;	 
	}
#endif
	this->location[location].setVespeneGeysirs(vespeneGeysirs);
}

void BASIC_MAP::setLocationMineralDistance(const unsigned int location, const unsigned int mineralDistance)
{
#ifdef _SCC_DEBUG
	if((location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (BASIC_MAP::setMineralDistance): Value loc out of range.");return;	 
	}
#endif
	this->location[location].setMineralDistance(mineralDistance);
}
	
void BASIC_MAP::setLocationDistance(const unsigned int location, const std::list<std::string> distances)
{
#ifdef _SCC_DEBUG
	if((location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (MAP::setLocationDistance): Value location out of range.");return;
	}
	if(location >= distances.size()) {
		toLog("DEBUG: (MAP::setLocationDistance): Too many locations.");return;
	}
#endif
	std::list<std::string>::const_iterator i=distances.begin();
	unsigned int j=0;
	while(i!=distances.end())
	{
		this->location[location].setDistance(j, atoi(i->c_str()));
		i++;
		j++;
	}
#ifdef _SCC_DEBUG
	if( j < location ) {
		toLog("DEBUG: (MAP::setLocationDistance): Not enough locations.");return;
	}
#endif		
}

void BASIC_MAP::setMaxLocations(const unsigned int maxLocations)
{
#ifdef _SCC_DEBUG
	if(maxLocations>MAX_LOCATIONS) {
		toLog("DEBUG: (BASIC_MAP::setMaxLocations): Value out of range.");return;
	}
#endif
	this->maxLocations=maxLocations;
}

void BASIC_MAP::setMaxPlayer(const unsigned int maxPlayer)
{
#ifdef _SCC_DEBUG
	if((maxPlayer<1)||(maxPlayer>MAX_PLAYER)) {
		toLog("DEBUG: (BASIC_MAP::setMaxPlayer): Value num out of range.");return;
	}
#endif
	this->maxPlayer=maxPlayer;
}

const MAP_LOCATION* BASIC_MAP::getLocation(const unsigned int location) const
{
#ifdef _SCC_DEBUG
	if(location>=MAX_LOCATIONS) {
		toLog("DEBUG: (BASIC_MAP::getLocation): Value location out of range.");return(0);
	}
#endif
	return(&this->location[location]);
}


	

