#ifndef _CORE_LOCATION_HPP
#define _CORE_LOCATION_HPP
#include "defs.hpp"

class MAP_LOCATION
{
	private:
		std::string name;
		int mineralPatches;
		int vespeneGeysirs;
		int mineralDistance;
		int distance[MAX_LOCATIONS];
		int nearest[MAX_LOCATIONS];
	public:
		MAP_LOCATION();
		~MAP_LOCATION();
		MAP_LOCATION& operator=(const MAP_LOCATION& object);
		MAP_LOCATION(const MAP_LOCATION& object);
		
		const std::string& getName() const;
		const int getMineralDistance() const;
		const int getDistance(const int location) const;
		const int getNearest(const int step) const;
		const int getMineralPatches() const;
		const int getVespeneGeysirs() const;
	
		void setName(const std::string& location_name);
		void setMineralDistance(const int mineral_distance);
		void setDistance(const int target, const int location_distance);
		void setMineralPatches(const int mineral_patches);
		void setVespeneGeysirs(const int vespene_geysirs);

		void resetData();
		void calculateDistances();
		void adjustDistances();
};

	  	

#endif // _CORE_LOCATION_HPP

