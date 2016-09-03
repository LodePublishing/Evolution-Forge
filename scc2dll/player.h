#ifndef __PLAYER_H
#define __PLAYER_H

#include "main.h"
#include "goal.h"

class EXPORT PLAYER
{
	private:
		int mins; //k
		int gas; //k
		int timer; //k
		int position;
		int race;
		const int* basicMineralHarvestPerSecond; //k
		const int* basicGasHarvestPerSecond; //k
		int initialized; //k
		int supply,maxSupply; //k
		int changed; //did the settings change? //k
		GOAL_ENTRY goal; // ~~ goals need to be copied so that each player has his own goals... but also think about 2vs2 with goals... i.e. build 10 marines etc.
	    const UNIT_STATISTICS* pStats;
	
	public:
		void adjustGoals(int adjust, UNITS* units);
		void copy(PLAYER* player);	
		void resetData();
		void wasChanged();
		int isChanged();
		void changeAccepted();
		GOAL_ENTRY* getGoal();
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
    	const UNIT_STATISTICS* getpStats();
	
		int setSupply(int num);
		int setMaxSupply(int num);
		int setMins(int num);
		int setGas(int num);
		int setTimer(int num);
		int setPosition(int num);
		int setRace(int num);
		int setHarvest(const int* mining, const int* gasing);
		const int* getHarvestMining();
		const int* getHarvestGasing();
		void setGoal(GOAL_ENTRY* goal); // -> copy von aussen machen

		PLAYER();
		~PLAYER();
};

#endif // __PLAYER_H

