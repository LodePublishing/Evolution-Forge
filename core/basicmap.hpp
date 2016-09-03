#ifndef _CORE_BASICMAP_HPP
#define _CORE_BASICMAP_HPP

#include "location.hpp"

#include <list>

class EXPORT BASIC_MAP
{
	private:
		string name; // constant
		int maxLocations; // constant
		int maxPlayer; // constant ?
		void resetData();
		int startPosition[MAX_PLAYER];
		MAP_LOCATION location[MAX_LOCATIONS];
	public:		
		BASIC_MAP();
		~BASIC_MAP();

		const int getLocationMineralPatches(const int location) const;
		const int getLocationVespeneGeysirs(const int location) const;

		const int getStartPlayerPosition(const int player) const;
		const MAP_LOCATION* getLocation(const int location) const;
		const string& getName() const;
		const int getMaxLocations() const;
		const int getMaxPlayer() const;
		void setName(const string& name);
		void setMaxLocations(const int maxLocations);  // sets the number of locations of this map
		void setMaxPlayer(const int maxPlayer);	// sets the number of player that are part of this map

		void setLocationName(const int location, const string& name);
		void setLocationMineralDistance(const int location, const int mineralDistance);
		void setLocationDistance(const int location, list<string> distances);// needs never be changed so a list is fine
		void setLocationMineralPatches(const int location, const int mineralPatches);
		void setLocationVespeneGeysirs(const int location, const int vespeneGeysirs);
		void calculateLocationsDistances();
};

#endif // _CORE_BASICMAP_H

