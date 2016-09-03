#ifndef __MAP_H
#define __MAP_H

#include "player.h"
#include "location.h"

// this needs still some work to make everything private

class EXPORT START
{
	MAP* map;
	PLAYER* startPlayer;
	MAP_LOCATION_BASIC location[MAX_LOCATIONS]; // initialized map
};



class EXPORT MAP
{
	private:
		char* name; // constant
		int maxLocations; // constant
		int maxPlayer; // constant ?
		int locationList[MAX_LOCATIONS][MAX_LOCATIONS];	// constant
		
		MAP_LOCATION_BASIC location[MAX_LOCATIONS]; // initialized map

		
		
		PLAYER startPlayer[MAX_PLAYER];
		
		int UMS;
	public:
		static int defaultForce[MAX_RACES][UNIT_TYPE_COUNT];
		int mapSettingsForce[UNIT_TYPE_COUNT];
		
		void resetData();
		void resetForce();
		void setUMS(int UMS);
		int getUMS();
		int setStartPlayerRace(int player, int race);
		int setStartPlayerHarvestSpeed(int player, const int* mining, const int* gasing);
		int setStartPlayerPosition(int player, int position);
		int setStartPlayerMins(int player, int mins);
		int setStartPlayerGas(int player, int gas);
		int setStartPlayerTime(int player, int time);

		int getStartPlayerPosition(int player);
		
		int getStartPlayerRace(int player);
		int setStartPlayerGoal(int player, GOAL_ENTRY* goal);

		int setLocationForce(int loc, int player, int unit, int num);
		int setLocationAvailible(int loc, int player, int unit, int num);
	
		PLAYER* getStartPlayer(int player);
		int getDistance(int loc1, int loc1);
		int getLocationForce(int loc, int player, int unit);
		int getNearestLocation(int start, int step);
		int setMineralDistance(int loc, int num);
		int setDistance(int loc1, int loc2, int num);
		const char* getName();
		int getMaxLocations();
		int getMaxPlayer();
		
		int setLocationName(int loc, const char* line);
		const char* getLocationName(int loc);

		int setName(const char* line);  // every map needs a name :)
		int setMaxLocations(int num);  // sets the number of locations of this map
		int setMaxPlayer(int num);	// sets the number of player that are part of this map

		int adjustDistanceList();	// adjust the distances between the locations and sorts them
		int adjustSupply();			// adjust supply depending on the units that were included in the map file
		int adjustResearches();		// fill the remaining items with either 0, 1 or 3, depending on NULL, research, upgrade 

// this is a list which holds the sorted distances, e.g. locationList[4][i] is the number of location which has the i-th smallest distance to location 4

// look at location.h for description
// => player.h

// Constructor and destructor
		MAP();
		~MAP();
};

#endif // __MAP_H

