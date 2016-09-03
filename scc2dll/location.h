#ifndef __LOCATION_H
#define __LOCATION_H

#include "main.h"
#include "units.h"

#define GLOBAL 0

class EXPORT MAP_LOCATION
{
	private:
		char name[32];
		int mineralDistance;
		int distance[MAX_LOCATIONS];
	//	int isCliff; //only reachable per dropship
	//	int canShootbetweenwhichconnection... // maybe for cliff drop later

//		int force[MAX_PLAYER][UNIT_TYPE_COUNT];
//		int availible[MAX_PLAYER][UNIT_TYPE_COUNT];
		UNITS units[MAX_PLAYER];
// later: a linked list for both force and availible for faster fighting calculation
	public:
		UNITS* getUnits(int player);
		void copy(int player, UNITS* units);
		void copy(MAP_LOCATION* location);
		void copyBasic(MAP_LOCATION* location); // copy the location without units!
		void resetData();
		void resetForce();
		int setName(const char* line);
		int setMineralDistance(int num);
		void setDistance(int target, int distance);
		const char* getName();
		int getMineralDistance();
		int getDistance(int num);
		int getForce(int player, int unit);
		int setForce(int player, int unit, int num);

		void adjustResearches(int player, int race);
		void adjustSupply(int player, int race, int& supply, int& maxSupply);

		int getAvailible(int player, int unit);
		int setAvailible(int player, int unit, int num);
		MAP_LOCATION();
		~MAP_LOCATION();
};

#endif // __LOCATION_H

