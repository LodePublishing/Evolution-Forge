#ifndef __LOCATION_H
#define __LOCATION_H

#include "main.h"

#define GLOBAL 0

class EXPORT MAP_LOCATION
{
	private:
		string name;
		int mineralDistance;
		int distance[MAX_LOCATIONS];
		int nearest[MAX_LOCATIONS];
	//	int isCliff; //only reachable per dropship
	//	int canShootbetweenwhichconnection... // maybe for cliff drop later
	public:
		const string& getName() const;
		const int getMineralDistance() const;
		const int getDistance(const int distance) const;
		const int getNearest(const int step) const;
		
		void setName(const string& name);
		void setMineralDistance(const int distance);
		void setDistance(const int target, const int distance);

		void resetData();
		void calculateDistances();
		void adjustDistances();

		MAP_LOCATION();
		~MAP_LOCATION();
};

#endif // __LOCATION_H

