#ifndef __MAP_H
#define __MAP_H

#include "player.h"
#include "location.h"

// this needs still some work to make everything private

class EXPORT MAP
{
	private:
		char* name;
		int maxLocations;//¬¬
		int maxPlayer;
	public:
		const char* getName();
		int getMaxLocations();
		int getMaxPlayer();		

		int setName(const char* line);  // every map needs a name :)
		int setMaxLocations(int num);  // sets the number of locations of this map
		int setMaxPlayer(int num);	// sets the number of player that are part of this map

		int adjustDistanceList();	// adjust the distances between the locations and sorts them
		int adjustSupply();			// adjust supply depending on the units that were included in the map file

// this is a list which holds the sorted distances, e.g. locationList[4][i] is the number of location which has the i-th smallest distance to location 4
		int locationList[MAX_LOCATIONS][MAX_LOCATIONS];	 

// look at location.h for description
		MAP_LOCATION_BASIC location[MAX_LOCATIONS];
// => player.h
		PLAYER player[MAX_PLAYER];

// Constructor and destructor
		MAP();
		~MAP();
};

#endif // __MAP_H

