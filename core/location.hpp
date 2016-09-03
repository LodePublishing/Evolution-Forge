#ifndef _CORE_LOCATION_HPP
#define _CORE_LOCATION_HPP
#include "defs.hpp"

#define GLOBAL 0
//extern const int GLOBAL;

class EXPORT MAP_LOCATION
{
	private:
		string name;
		int mineralPatches;
		int vespeneGeysirs;
		int mineralDistance;
		int distance[MAX_LOCATIONS];
		int nearest[MAX_LOCATIONS];
	public:
		MAP_LOCATION();
		~MAP_LOCATION();
		
		const string& getName() const;
		const int getMineralDistance() const;
		const int getDistance(const int location) const;
		const int getNearest(const int step) const;
		const int getMineralPatches() const;
		const int getVespeneGeysirs() const;
	
		void setName(const string& name);
		void setMineralDistance(const int distance);
		void setDistance(const int target, const int distance);
		void setMineralPatches(const int mineralPatches);
		void setVespeneGeysirs(const int vespeneGeysirs);

		void resetData();
		void calculateDistances();
		void adjustDistances();
};

	  	

#endif // _CORE_LOCATION_HPP

