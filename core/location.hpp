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
	
		void setName(const std::string& name);
		void setMineralDistance(const int distance);
		void setDistance(const int target, const int distance);
		void setMineralPatches(const int mineralPatches);
		void setVespeneGeysirs(const int vespeneGeysirs);

		void resetData();
		void calculateDistances();
		void adjustDistances();
};

	  	

#endif // _CORE_LOCATION_HPP

