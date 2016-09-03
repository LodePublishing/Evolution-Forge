#ifndef _CORE_LOCATION_HPP
#define _CORE_LOCATION_HPP

#include "unit.hpp"

class MAP_LOCATION
{
	public:
		MAP_LOCATION();
		~MAP_LOCATION();
		
		const std::string& getName() const;
		const unsigned int getResourceDistance(const unsigned int resource_type) const;
		const unsigned int getDistance(const unsigned int location) const;
		const unsigned int getNearest(const unsigned int step) const;
	
		void setName(const std::string& location_name);
		void setResourceDistance(const unsigned int resource_type, const unsigned int resource_distance);
		void setDistance(const unsigned int target, const unsigned int location_distance);
		void setUnit(const unsigned int unit, const unsigned int count);

		const UNIT& getUnits() const;

		void resetData();
		void calculateDistances(const unsigned int this_location_number);
		void adjustDistances();
		
		static void setMaxLocations(const unsigned int max_locations);
	private:
		std::string name;
		UNIT units;
		std::vector<unsigned int> resourceDistance;
		
		unsigned int distance[MAX_LOCATIONS];
		unsigned int nearest[MAX_LOCATIONS];
		static unsigned int maxLocations;
		static const unsigned int getMaxLocations();

		MAP_LOCATION& operator=(const MAP_LOCATION& object);
		MAP_LOCATION(const MAP_LOCATION& object);
};
		
inline void MAP_LOCATION::setUnit(const unsigned int unit, const unsigned int count) // TODO
{
	units.setTotal(unit, count);
}

inline void MAP_LOCATION::setMaxLocations(const unsigned int max_locations) {
#ifdef _SCC_DEBUG
// TODO constant
        if(max_locations > MAX_LOCATIONS) {
                toErrorLog("DEBUG ((MAP_LOCATION::setMaxLocations()): Value out of range.");return;
        }
#endif
	maxLocations = max_locations;
}

inline const unsigned int MAP_LOCATION::getMaxLocations() {
#ifdef _SCC_DEBUG
// TODO constant
        if(maxLocations > MAX_LOCATIONS) {
                toErrorLog("DEBUG ((MAP_LOCATION::getMaxLocations()): Variable maxLocations out of range.");return(0);
        }
#endif
	return(maxLocations);
}

inline void MAP_LOCATION::setName(const std::string& location_name) {
	name.assign(location_name);
}

inline const UNIT& MAP_LOCATION::getUnits() const {
	return(units);
}

inline void MAP_LOCATION::setResourceDistance(const unsigned int resource_type, const unsigned int resource_distance) {
#ifdef _SCC_DEBUG
	if(resource_type >= RACE::MAX_RESOURCE_TYPES) {
		toErrorLog("DEBUG (MAP_LOCATION::setResourceDistance()): Value resource_type out of range.");return;
	}
	if(resource_distance > 50) {
		toErrorLog("DEBUG (MAP_LOCATION::setResourceDistance()): Value resource_distance out of range.");return;
	}
#endif
	resourceDistance[resource_type] = resource_distance;
}

inline const unsigned int MAP_LOCATION::getNearest(const unsigned int step) const {
#ifdef _SCC_DEBUG
	if(step >= maxLocations) {
		toErrorLog("DEBUG ((MAP_LOCATION::getNearest()): Value step out of range.");return(0);
	}
	if(nearest[step] >= 9999) {
		toErrorLog("DEBUG ((MAP_LOCATION::getNearest()): Variable nearest not initialized.");return(0);
	}
#endif
	return(nearest[step]);
}


inline void MAP_LOCATION::setDistance(const unsigned int target, const unsigned int location_distance) {
#ifdef _SCC_DEBUG
	if(target >= maxLocations) {
		toErrorLog("DEBUG ((MAP_LOCATION::setDistance()): Value target out of range.");return;
	}
//	if(location_distance>) {
//		toErrorLog("DEBUG ((MAP_LOCATION::setDistance()): Value distance out of range.");return;
//	}
#endif
	distance[target] = location_distance;
}

inline const std::string& MAP_LOCATION::getName() const {
	return(name);
}

inline const unsigned int MAP_LOCATION::getResourceDistance(const unsigned int resource_type) const {
#ifdef _SCC_DEBUG
	if(resource_type >= RACE::MAX_RESOURCE_TYPES) {
		toErrorLog("DEBUG (MAP_LOCATION::setResourceDistance()): Value resource_type out of range.");return(0);
	}
	if(resourceDistance[resource_type] > 50) {
		toErrorLog("DEBUG (MAP_LOCATION::getResourceDistance()): Variable resourceDistance not initialized.");return(0);
	}
#endif
	return(resourceDistance[resource_type]);
}

inline const unsigned int MAP_LOCATION::getDistance(const unsigned int location) const {
#ifdef _SCC_DEBUG
	if(location >= maxLocations) {
		toErrorLog("DEBUG ((MAP_LOCATION::getDistance()): Value out of range.");return(0);
	}
//	if(distance[location]>500) {
//		toErrorLog("DEBUG ((MAP_LOCATION::getDistance()): Value distance out of range.");return(0);
//	}
#endif
	return(distance[location]);
}


#endif // _CORE_LOCATION_HPP

