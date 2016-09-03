#ifndef __DEFAULT_H
#define __DEFAULT_H

#include "player.h"
#include "units.h"
#include "harvest.h"

class EXPORT DEFAULT
{
	private:
		PLAYER player[3];  //mins, gas, timer, etc.... 1 player would be enough, but lets keep as general as possible
		UNITS units[3]; // relative to start position!
		void adjustSupply();
		void adjustGoals();
		void adjustResearches();
	public:
		void loadFromFile(const char* defaultFile, HARVEST_SPEED* harvest);
		PLAYER* getPlayer(int race);
		UNITS* getUnits(int race);
		DEFAULT();
		~DEFAULT();
};

#endif

