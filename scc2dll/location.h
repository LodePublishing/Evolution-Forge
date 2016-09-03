#ifndef __LOCATION_H
#define __LOCATION_H

#include "main.h"

class EXPORT MAP_LOCATION_BASIC
{
	private:
		char name[32];
		int mineralDistance;
		int distance[MAX_LOCATIONS];
	//	int isCliff; //only reachable per dropship
	//	int canShootbetweenwhichconnection... // maybe for cliff drop later
		int force[MAX_PLAYER][UNIT_TYPE_COUNT];
	public:
		int setName(const char* line);
		int setMineralDistance(int num);
		int setDistance(int num,int dist);

		const char* getName();
		int getMineralDistance();

		int getDistance(int num);
		MAP_LOCATION_BASIC();
		~MAP_LOCATION_BASIC();
};

struct MAP_LOCATION
{
		int availible[UNIT_TYPE_COUNT];
		int force[UNIT_TYPE_COUNT];
};

#endif // __LOCATION_H

