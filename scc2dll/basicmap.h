#ifndef __BASICMAP_H
#define __BASICMAP_H

#include "player.h"
#include "location.h"
#include <list>
using namespace std;

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
//		void setLocationDistance(const int location, const int target, const int distance);
		void calculateLocationsDistances();
};







// this needs still some work to make everything private

// make map so, that the map contains only the neutral player!
/*class EXPORT MAP_2
{
	private:
		PLAYER startPlayer[MAX_PLAYER];
		void copy(PLAYER* player);
	public:
		void adjustGoals(int player);
		void copy(MAP* map);
		void copyBasic(MAP* map);
		void copy(DEFAULT* defaults);
		void resetForce();
		int setStartPlayerRace(int player, int race);
		int setStartPlayerHarvestSpeed(int player, const int* mining, const int* gasing);
		int setStartPlayerPosition(int player, int position);
		int setStartPlayerMins(int player, int mins);
		int setStartPlayerGas(int player, int gas);
		int setStartPlayerTime(int player, int time);

		int getStartPlayerRace(int player);
		void setStartPlayerGoal(int player, GOAL_ENTRY* goal);

		int setLocationForce(int loc, int player, int unit, int num);
		int setLocationAvailible(int loc, int player, int unit, int num);
	
		void adjustSupply();			// adjust supply depending on the units that were included in the map file
		void adjustResearches();		// fill the remaining items with either 0, 1 or 3, depending on NULL, research, upgrade 
// this is a list which holds the sorted distances, e.g. locationList[4][i] is the number of location which has the i-th smallest distance to location 4
// look at location.h for description
// => player.h
// Constructor and destructor
		MAP();
		~MAP();
};*/

#endif // __BASICMAP_H

