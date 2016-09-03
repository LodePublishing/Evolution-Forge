#ifndef _CORE_BASICMAP_HPP
#define _CORE_BASICMAP_HPP

#include "location.hpp"

#include <list>

class BASIC_MAP
{
	private:
		std::string name; // constant
		unsigned int maxLocations; // constant
		unsigned int maxPlayer; // constant ?
		void resetData();
		unsigned int startPosition[MAX_PLAYER];
		MAP_LOCATION location[MAX_LOCATIONS];
		unsigned int calculateDistance(unsigned int i, unsigned int j);
	public:		
		BASIC_MAP();
		~BASIC_MAP();

		const unsigned int getLocationMineralPatches(const unsigned int mineral_location) const;
		const unsigned int getLocationVespeneGeysirs(const unsigned int gas_location) const;

		const unsigned int getStartPlayerPosition(const unsigned int player) const;
		const MAP_LOCATION* getLocation(const unsigned int location_number) const;
		const std::string& getName() const;
		const unsigned int getMaxLocations() const;
		const unsigned int getMaxPlayer() const;
		void setName(const std::string& map_name);
		void setMaxLocations(const unsigned int max_locations);  // sets the number of locations of this map
		void setMaxPlayer(const unsigned int max_player);	// sets the number of player that are part of this map

		void setLocationName(const unsigned int location_number, const std::string& location_name);
		void setLocationMineralDistance(const unsigned int location_number, const unsigned int mineralDistance);
		void setLocationDistance(const unsigned int location_number, const unsigned int target, const unsigned int distance);
		void setLocationMineralPatches(const unsigned int location_number, const unsigned int mineralPatches);
		void setLocationVespeneGeysirs(const unsigned int location_number, const unsigned int vespeneGeysirs);
		void calculateLocationsDistances();
};

inline const std::string& BASIC_MAP::getName() const
{
	return(name);
}

inline const unsigned int BASIC_MAP::getMaxLocations() const
{
#ifdef _SCC_DEBUG
	if(maxLocations > MAX_LOCATIONS) {
		toLog("DEBUG: (BASIC_MAP::getMaxLocations): Variable not initialized.");return(0);
	}
#endif
	return(maxLocations);
}

inline const unsigned int BASIC_MAP::getMaxPlayer() const
{
#ifdef _SCC_DEBUG
	if((maxPlayer < 1) || (maxPlayer > MAX_PLAYER)) {
		toLog("DEBUG: (BASIC_MAP::getMaxPlayer): Variable not initialized.");return(0);
	}
#endif
	return(maxPlayer);
}

inline void BASIC_MAP::setName(const std::string& map_name)
{
	name = map_name;
}

inline void BASIC_MAP::setLocationName(const unsigned int location_number, const std::string& location_name)
{
#ifdef _SCC_DEBUG
	if((location_number >= maxLocations)) {
		toLog("DEBUG: (BASIC_MAP::setLocationName): Value out of range.");return;
	}
#endif
	location[location_number].setName(location_name);
}

inline const unsigned int BASIC_MAP::getLocationMineralPatches(const unsigned int location_number) const
{
#ifdef _SCC_DEBUG
	if((location_number >= maxLocations)) {
		toLog("DEBUG: (BASIC_MAP::getLocationMineralPatches): Value out of range.");return(0);	 
	}
#endif
	return(location[location_number].getMineralPatches());
}

inline const unsigned int BASIC_MAP::getLocationVespeneGeysirs(const unsigned int location_number) const
{
#ifdef _SCC_DEBUG
	if((location_number >= maxLocations)) {
		toLog("DEBUG: (BASIC_MAP::getLocationVespeneGeysirs): Value out of range.");return(0);	 
	}
#endif
	return(location[location_number].getVespeneGeysirs());
}

inline void BASIC_MAP::setLocationMineralPatches(const unsigned int location_number, const unsigned int mineral_patches)
{
#ifdef _SCC_DEBUG
	if((location_number >= maxLocations)) {
		toLog("DEBUG: (BASIC_MAP::setLocationMineralPatches): Value out of range.");return;	 
	}
#endif
	location[location_number].setMineralPatches(mineral_patches);
}

inline void BASIC_MAP::setLocationVespeneGeysirs(const unsigned int location_number, const unsigned int vespene_geysirs)
{
#ifdef _SCC_DEBUG
	if((location_number >= maxLocations)) {
		toLog("DEBUG: (BASIC_MAP::setLocationVespeneGeysirs): Value out of range.");return;	 
	}
#endif
	location[location_number].setVespeneGeysirs(vespene_geysirs);
}

inline void BASIC_MAP::setLocationMineralDistance(const unsigned int location_number, const unsigned int mineral_distance)
{
#ifdef _SCC_DEBUG
	if((location_number >= maxLocations)) {
		toLog("DEBUG: (BASIC_MAP::setMineralDistance): Value out of range.");return;
	}
#endif
	location[location_number].setMineralDistance(mineral_distance);
}
	
inline void BASIC_MAP::setLocationDistance(const unsigned int location_number, const unsigned int target, const unsigned int distance)
{
#ifdef _SCC_DEBUG
	if((location_number >= maxLocations)) {
		toLog("DEBUG: (MAP::setLocationDistance): Value location_number out of range.");return;
	}
	if((target >= maxLocations)) {
		toLog("DEBUG: (MAP::setLocationDistance): Value target out of range.");return;
	}
//	if(distance >= 1000) {
//		toLog("DEBUG: (MAP::setLocationDistance): Value distance out of range.");return;
//	}
	
#endif
	location[location_number].setDistance(target, distance);
}

inline void BASIC_MAP::setMaxLocations(const unsigned int max_locations)
{
#ifdef _SCC_DEBUG
	if(max_locations > MAX_LOCATIONS) {
		toLog("DEBUG: (BASIC_MAP::setMaxLocations): Value out of range.");return;
	}
#endif
	maxLocations = max_locations;
	MAP_LOCATION::setMaxLocations(max_locations);
}

inline void BASIC_MAP::setMaxPlayer(const unsigned int max_player)
{
#ifdef _SCC_DEBUG
	if((max_player < 1) || (max_player > MAX_PLAYER)) {
		toLog("DEBUG: (BASIC_MAP::setMaxPlayer): Value out of range.");return;
	}
#endif
	maxPlayer = max_player;
}

inline const MAP_LOCATION* BASIC_MAP::getLocation(const unsigned int location_number) const
{
#ifdef _SCC_DEBUG
	if(location_number >= maxLocations) {
		toLog("DEBUG: (BASIC_MAP::getLocation): Value out of range.");return(0);
	}
#endif
	return(&location[location_number]);
}

#endif // _CORE_BASICMAP_H

