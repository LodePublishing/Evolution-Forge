#ifndef __PLAYER_H
#define __PLAYER_H

#include "main.h"
#include "goal.h"
class PLAYER
{
	private:
		int mins;
		int gas;
		int timer;
		int position;
		int race;
        const int* basicMineralHarvestPerSecond;
        const int* basicGasHarvestPerSecond;
		int initialized;
		int supply,maxSupply;
	public:
		GOAL_ENTRY* goal;
		int getInitialized();
		int getMins();
		int getGas();
		int getTimer();
		int getPosition();
		int getRace();
		int getBasicMineralHarvestPerSecond(int num);
		int getBasicGasHarvestPerSecond(int num);
		int getSupply();
		int getMaxSupply();

		int setSupply(int num);
		int setMaxSupply(int num);
		int setMins(int num);
		int setGas(int num);
		int setTimer(int num);
		int setPosition(int num);
		int setRace(int num);
		int setHarvest(const int* mining, const int* gasing);
		int setGoal(GOAL_ENTRY* goal);

		PLAYER();
		~PLAYER();
};

#endif // __PLAYER_H

