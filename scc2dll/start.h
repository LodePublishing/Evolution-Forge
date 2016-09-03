#ifndef __START_H
#define __START_H
#include "map.h"
#include "default.h"
#include "goal.h"

class START
{
	private:
		MAP map; // distances, names etc.
	public:
		void adjustGoals(int player);
		void assignMap(MAP* map, DEFAULT* defaults=0);
		void setPlayerRace(int player, int race);
		MAP* getMap();
		START();
		~START();
};

#endif

