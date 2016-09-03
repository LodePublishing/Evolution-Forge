#ifndef _CORE_LOCATION_HPP
#define _CORE_LOCATION_HPP

#include "starcraft.hpp"
#include "../stl/misc.hpp"

class MAP_LOCATION
{
	private:
		std::string name;
		unsigned int mineralPatches;
		unsigned int vespeneGeysirs;
		unsigned int mineralDistance;
		unsigned int distance[MAX_LOCATIONS];
		unsigned int nearest[MAX_LOCATIONS];
		static unsigned int maxLocations;
		static const unsigned int getMaxLocations();
	public:
		MAP_LOCATION();
		~MAP_LOCATION();
		MAP_LOCATION& operator=(const MAP_LOCATION& object);
		MAP_LOCATION(const MAP_LOCATION& object);
		
		const std::string& getName() const;
		const unsigned int getMineralDistance() const;
		const unsigned int getDistance(const unsigned int location) const;
		const unsigned int getNearest(const unsigned int step) const;
		const unsigned int getMineralPatches() const;
		const unsigned int getVespeneGeysirs() const;
	
		void setName(const std::string& location_name);
		void setMineralDistance(const unsigned int mineral_distance);
		void setDistance(const unsigned int target, const unsigned int location_distance);
		void setMineralPatches(const unsigned int mineral_patches);
		void setVespeneGeysirs(const unsigned int vespene_geysirs);

		void resetData();
		void calculateDistances();
		void adjustDistances();
		
		static void setMaxLocations(const unsigned int max_locations);
};

inline void MAP_LOCATION::setMaxLocations(const unsigned int max_locations)
{
#ifdef _SCC_DEBUG
// TODO constant
        if(max_locations > MAX_LOCATIONS) {
                toLog("DEBUG: (MAP_LOCATION::setMaxLocations): Value out of range.");return;
        }
#endif
	maxLocations=max_locations;
}

inline const unsigned int MAP_LOCATION::getMaxLocations()
{
#ifdef _SCC_DEBUG
// TODO constant
        if(maxLocations > MAX_LOCATIONS) {
                toLog("DEBUG: (MAP_LOCATION::getMaxLocations): Variable maxLocations out of range.");return(0);
        }
#endif
	return(maxLocations);
}

inline void MAP_LOCATION::setName(const std::string& location_name)
{
	name.assign(location_name);
}

inline const unsigned int MAP_LOCATION::getMineralPatches() const
{
#ifdef _SCC_DEBUG
// TODO constant
	if(mineralPatches > 100) {
		toLog("DEBUG: (MAP_LOCATION::getMineralPatches): Variable mineralPatches out of range.");return(0);
	}
#endif			
	return(mineralPatches);
}

inline const unsigned int MAP_LOCATION::getVespeneGeysirs() const
{
#ifdef _SCC_DEBUG
// TODO constantv
        if(vespeneGeysirs > 100) {
                toLog("DEBUG: (MAP_LOCATION::getVespeneGeysirs): Value vespeneGeysirs out of range.");return(0);
        }
#endif
	return(vespeneGeysirs);
}

inline void MAP_LOCATION::setMineralPatches(const unsigned int mineral_patches)
{
#ifdef _SCC_DEBUG
// TODO constant
	if(mineral_patches > 100) {
		toLog("DEBUG: (MAP_LOCATION::setMineralPatches): Value out of range.");return;
	}
#endif
	mineralPatches = mineral_patches;
}

inline void MAP_LOCATION::setVespeneGeysirs(const unsigned int vespene_geysirs)
{
#ifdef _SCC_DEBUG
// TODO constantv
	if(vespene_geysirs > 100) {
		toLog("DEBUG: (MAP_LOCATION::setVespeneGeysirs): Value out of range.");return;
	}
#endif
	vespeneGeysirs = vespene_geysirs;
}

inline void MAP_LOCATION::setMineralDistance(const unsigned int mineral_distance)
{
#ifdef _SCC_DEBUG
	if(mineral_distance > 5) {
		toLog("DEBUG: (MAP_LOCATION::setMineralDistance): Value out of range.");return;
	}
#endif
	mineralDistance = mineral_distance;
}

inline const unsigned int MAP_LOCATION::getNearest(const unsigned int step) const
{
#ifdef _SCC_DEBUG
	if((step<=0)||(step>=maxLocations)) {
		toLog("DEBUG: (MAP_LOCATION::getNearest): Value out of range.");return(0);
	}
#endif
	return(nearest[step]);
}


inline void MAP_LOCATION::setDistance(const unsigned int target, const unsigned int location_distance)
{
#ifdef _SCC_DEBUG
	if(target>=maxLocations) {
		toLog("DEBUG: (MAP_LOCATION::setDistance): Value target out of range.");return;
	}
//	if(location_distance>) {
//		toLog("DEBUG: (MAP_LOCATION::setDistance): Value distance out of range.");return;
//	}
#endif
	distance[target] = location_distance;
}

inline const std::string& MAP_LOCATION::getName() const
{
	return(name);
}

inline const unsigned int MAP_LOCATION::getMineralDistance() const
{
#ifdef _SCC_DEBUG
	if(mineralDistance>5) {
		toLog("DEBUG: (MAP_LOCATION::getMineralDistance): Variable not initialized.");return(0);
	}
#endif
	return(mineralDistance);
}

inline const unsigned int MAP_LOCATION::getDistance(const unsigned int location) const
{
#ifdef _SCC_DEBUG
	if(location>=maxLocations) {
		toLog("DEBUG: (MAP_LOCATION::getDistance): Value out of range.");return(0);
	}
//	if(distance[location]>500) {
//		toLog("DEBUG: (MAP_LOCATION::getDistance): Value distance out of range.");return(0);
//	}
#endif
	return(distance[location]);
}


#endif // _CORE_LOCATION_HPP

