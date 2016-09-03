#ifndef __PLAYER_H
#define __PLAYER_H

#include "main.h"
#include "goal.h"

// ok

class EXPORT PLAYER
{
	private:
		int minerals; //k
		int gas; //k
		int timer; //k
		eRace race;
		const int* basicMineralHarvestPerSecond; //k
		const int* basicGasHarvestPerSecond; //k
		bool initialized; //k
		int needSupply, haveSupply; //k
		bool changed; //did the settings change? //k
		GOAL_ENTRY* goal;  // only be set by 'start'
		// ~~ goals need to be copied so that each player has his own goals... but also think about 2vs2 with goals... i.e. build 10 marines etc.
	    const UNIT_STATISTICS* pStats;
	
	public:
//		void adjustGoals(const int adjust, const UNIT* unit);
		void copy(const PLAYER* player);	
		void resetData();
		void wasChanged();
//		void changeAccepted();
		
		const bool isChanged() const;
		const GOAL_ENTRY* getGoal() const;
		const bool getInitialized() const;
		const int getMinerals() const;
		const int getGas() const;
		const int getTimer() const;
		//const int getPosition() const;
		const eRace getRace() const;
		const int getBasicMineralHarvestPerSecond(const int workerCount) const;
		const int getBasicGasHarvestPerSecond(const int workerCount) const;
		const int getNeedSupply() const;
		const int getHaveSupply() const;
    	const UNIT_STATISTICS* getpStats() const;
	
		void setNeedSupply(const int needSupply);
		void setHaveSupply(const int haveSupply);
		void setMinerals(const int minerals);
		void setGas(const int gas);
		void setTimer(const int timer);
//		void setPosition(const int startPosition);
		void setRace(const eRace race);
		void setHarvest(const int* mining, const int* gasing);
		const int* getHarvestMining() const;
		const int* getHarvestGasing() const;
		void assignGoal(const GOAL_ENTRY* goal); // -> copy von aussen machen

		PLAYER();
		~PLAYER();
};

#endif // __PLAYER_H

