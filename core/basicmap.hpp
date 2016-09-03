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
		void setLocationDistance(const unsigned int location_number, std::list<std::string> distances);// needs never be changed so a list is fine
		void setLocationMineralPatches(const unsigned int location_number, const unsigned int mineralPatches);
		void setLocationVespeneGeysirs(const unsigned int location_number, const unsigned int vespeneGeysirs);
		void calculateLocationsDistances();
};

#endif // _CORE_BASICMAP_H

