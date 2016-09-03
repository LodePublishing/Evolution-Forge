#ifndef _CORE_BASICMAP_HPP
#define _CORE_BASICMAP_HPP

#include "location.hpp"


class BASIC_MAP
{
	private:
		std::string name; // constant
		unsigned int maxLocations; // constant
		unsigned int maxPlayer; // constant ?
		void resetData();
		MAP_LOCATION location[MAX_LOCATIONS];
		unsigned int calculateDistance(unsigned int i, unsigned int j);
	public:		
		BASIC_MAP();
		~BASIC_MAP();

		const unsigned int getStartPlayerPosition(const unsigned int player) const;
		const MAP_LOCATION* getLocation(const unsigned int location_number) const;
		const std::string& getName() const;
		const unsigned int getMaxLocations() const;
		const unsigned int getMaxPlayer() const;
		void setName(const std::string& map_name);
		const bool setMaxLocations(const unsigned int max_locations);  // sets the number of locations of this map
		const bool setMaxPlayer(const unsigned int max_player);	// sets the number of player that are part of this map

		void setLocationUnitTotal(const unsigned int location_number, const unsigned int unit, const unsigned int total);

		void setLocationName(const unsigned int location_number, const std::string& location_name);
		void setLocationResourceDistance(const unsigned int location_number, const unsigned int resource_type, const unsigned int resource_distance);
		void setLocationDistance(const unsigned int location_number, const unsigned int target, const unsigned int distance);
		void calculateLocationDistances();
};
		
inline void BASIC_MAP::setLocationUnitTotal(const unsigned int location_number, const unsigned int unit, const unsigned int total) {
#ifdef _SCC_DEBUG
	if(location_number >= getMaxLocations()) {
		toErrorLog("DEBUG (BASIC_MAP::setLocationUnitTotal()): Value location_number out of range.");return;
	}
#endif
	location[location_number].setUnit(unit, total);
}
		
inline const std::string& BASIC_MAP::getName() const {
	return(name);
}

inline void BASIC_MAP::setName(const std::string& map_name) {
	name = map_name;
}

inline const unsigned int BASIC_MAP::getMaxLocations() const
{
#ifdef _SCC_DEBUG
	if(maxLocations > MAX_LOCATIONS) {
		toErrorLog("DEBUG (BASIC_MAP::getMaxLocations()): Variable maxLocations not initialized.");return(0);
	}
#endif
	return(maxLocations);
}

inline const unsigned int BASIC_MAP::getMaxPlayer() const
{
#ifdef _SCC_DEBUG
	if((maxPlayer < 1) || (maxPlayer > MAX_PLAYER)) {
		toErrorLog("DEBUG (BASIC_MAP::getMaxPlayer()): Variable not initialized.");return(0);
	}
#endif
	return(maxPlayer);
}


inline void BASIC_MAP::setLocationName(const unsigned int location_number, const std::string& location_name)
{
#ifdef _SCC_DEBUG
	if((location_number >= maxLocations)) {
		toErrorLog("DEBUG (BASIC_MAP::setLocationName()): Value out of range.");return;
	}
#endif
	location[location_number].setName(location_name);
}

inline void BASIC_MAP::setLocationResourceDistance(const unsigned int location_number, const unsigned int resource_type, const unsigned int resource_distance)
{
#ifdef _SCC_DEBUG
	if((location_number >= maxLocations)) {
		toErrorLog("DEBUG (BASIC_MAP::setLocationResourceDistance()): Value location_number out of range.");return;
	}
	if(resource_type >= RACE::MAX_RESOURCE_TYPES) {
		toErrorLog("DEBUG (BASIC_MAP::setLocationResourceDistance()): Value resource_type out of range.");return;
	}
	if(resource_distance > 9999) {
		toErrorLog("DEBUG (BASIC_MAP::setLocationResourceDistance()): Value resource_distance out of range.");return;
	}
#endif
	location[location_number].setResourceDistance(resource_type, resource_distance);
}
	
inline void BASIC_MAP::setLocationDistance(const unsigned int location_number, const unsigned int target, const unsigned int distance)
{
#ifdef _SCC_DEBUG
	if((location_number >= maxLocations)) {
		toErrorLog("DEBUG (MAP::setLocationDistance()): Value location_number out of range.");return;
	}
	if((target >= maxLocations)) {
		toErrorLog("DEBUG (MAP::setLocationDistance()): Value target out of range.");return;
	}
//	if(distance >= 1000) {
//		toErrorLog("DEBUG (MAP::setLocationDistance()): Value distance out of range.");return;
//	}
	
#endif
	location[location_number].setDistance(target, distance);
}

inline const bool BASIC_MAP::setMaxLocations(const unsigned int max_locations)
{
#ifdef _SCC_DEBUG
	if(max_locations > MAX_LOCATIONS) {
		toErrorLog("DEBUG (BASIC_MAP::setMaxLocations()): Value max_locations out of range.");return(false);
	}
#endif
	maxLocations = max_locations;
	MAP_LOCATION::setMaxLocations(max_locations);
	return(true);
}

inline const bool BASIC_MAP::setMaxPlayer(const unsigned int max_player)
{
#ifdef _SCC_DEBUG
	if((max_player < 1) || (max_player > MAX_PLAYER)) {
		toErrorLog("DEBUG (BASIC_MAP::setMaxPlayer()): Value max_player out of range.");return(false);
	}
#endif
	maxPlayer = max_player;
	return(true);
}

inline const MAP_LOCATION* BASIC_MAP::getLocation(const unsigned int location_number) const
{
#ifdef _SCC_DEBUG
	if(location_number >= maxLocations) {
		toErrorLog("DEBUG (BASIC_MAP::getLocation()): Value out of range.");return(0);
	}
#endif
	return(&location[location_number]);
}

#endif // _CORE_BASICMAP_H

