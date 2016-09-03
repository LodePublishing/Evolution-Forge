#ifndef _CORE_PRERACE_HPP
#define _CORE_PRERACE_HPP

#include "main.hpp"
#include "basicmap.hpp"
#include "ga.hpp"
#include "location.hpp"
#include "building.hpp"
#include "start.hpp"
#include "blist.hpp"

class EXPORT PRERACE
{
protected:
	static START* pStart;

//------------------ optimizations:-----------------
//	UNIT* global; // non-static pointer to player's global total force (location 0.total)
	UNIT* location; // non-static pointer to players total/availible units

	
	const START_CONDITION* const* pStartCondition; //pointer to player in start
	GOAL_ENTRY** pGoal; // pStart->getGoal()

	const static BASIC_MAP* const* pMap; // MAP is all the same for all players using 'start
//------------- end -------------------------------
	
	BuildingList buildingList;
	
	static int noise[MAX_TIME];
	static int markerCounter;
	static UNIT unit[MAX_PLAYER][MAX_LOCATIONS]; // player 0 is neutral player!
	LAST last[MAX_LENGTH]; // last* is to save the last position, for movements
	int lastcounter;
	int lastunit;
	bool ready;
	const UNIT_STATISTICS* const* pStats;
	int neededMinerals,neededGas;
	
	void createSpecial();
	void resetSpecial();
	const int harvestMinerals() const;
	const int harvestGas() const; 

	const bool calculateReady() const;
	void adjustAvailibility(const int location, const int fac, const UNIT_STATISTICS* stat);
	const int calculatePrimaryFitness(const bool ready) const;
	void replaceCode(const int IP, const int num);

	int larvaInProduction[MAX_LOCATIONS]; // well... one of that ugly race-specific variables saves a lot of trouble...

	const int calculateIdleTime() const; // calculate the next time something significant will happen
private:
	int playerNum;
	int minerals,gas,timer;
	int IP;
	int mineralHarvestPerSecond[MAX_LOCATIONS][45];
	int gasHarvestPerSecond[MAX_LOCATIONS][5];
	int harvestedGas,harvestedMinerals;
	int needSupply;		// free supply
	int haveSupply; // total supply
	int ftime[MAX_GOALS]; //when the goal is reached / when the last item is produced (ALL locations...*/
	int length,timeout;
public:

// ------ PUBLIC VARIABLES ------

	int Code[MAX_LENGTH];
	int Marker[MAX_LENGTH];
	static GA* ga;

	static void assignStart(START* start);
	static void initNoise();
	static void copyMap(); //copies the startforce from map to static 'units'

// ------ HARVEST ROUTINES ------
	void adjustMineralHarvest(const int location);
	void adjustGasHarvest(const int location);
	void adjustHarvestAllLocations();
	void setMineralHarvestPerSecond(const int location, const int worker, const int minerals);
	const int getMineralHarvestPerSecond(const int location, const int worker) const;
	void setGasHarvestPerSecond(const int location, const int worker, const int gas);
	const int getGasHarvestPerSecond(const int location, const int worker) const;
	const int getHarvestedMinerals() const;
	const int getHarvestedGas() const;
	void setHarvestedMinerals(const int minerals);
	void setHarvestedGas(const int gas);
	
// ------ INITIALIZATION ROUTINES ------
	static void resetGeneMarker();
	void setPlayerNum(const int playerNum); // assigns player data from start (start minerals, supply etc.) and sets the appropriate optimized pointers (global, location, pMap etc.) CALL IT AFTER EACH MAP CHANGE AND PLAYER CHANGE!!
	void initializePlayer();

// ------ GET/SET ROUTINES ------

	GOAL_ENTRY** getCurrentGoal() const;
	void setCurrentGoal(GOAL_ENTRY** pGoal);
	
    const START_CONDITION* const* getStartCondition(); //pointer to player in start
	
	const static int getMapLocationAvailible(const int player, const int location, const int unittype);
	const static int getMapLocationTotal(const int player, const int location, const int unittype);
																				
	static void setMapLocationAvailible(const int player, const int location, const int unittype, const int num);
	static void setMapLocationTotal(const int player, const int location, const int unittype, const int num);
																				
	static void addMapLocationAvailible(const int player, const int location, const int unittype, const int num);
	static void addMapLocationTotal(const int player, const int location, int unittype, const int num);
																				
	const int getLocationAvailible(const int location, const int unittype) const;
	const int getLocationTotal(const int location, const int unittype) const;
																				
	void setLocationAvailible(const int location, int unittype, int num);
	void setLocationTotal(int location, const int unittype, int num);
																				
	void addLocationAvailible(const int location, const int unittype, const int num);
	void addLocationTotal(const int location, const int unittype, const int num);

	void setNeedSupply(const int needSupply);
	void setHaveSupply(const int haveSupply);
	void setMinerals(const int minerals);
	void setGas(const int gas);

	void setpStats(const UNIT_STATISTICS* const* pStats);
	const UNIT_STATISTICS* const* getpStats() const;

	const int getPlayerNum() const;
	const int getNeedSupply() const;
	const int getHaveSupply() const;
	const int getMinerals() const;
	const int getGas() const;

	void setTimer(const int time);
	const int getTimer() const;

	void setTimeOut(const int time);
	const int getTimeOut() const;

	const int getIP() const;
	void setIP(const int IP);



	void setFinalTime(const int goal, const int time);
	const int getFinalTime(const int goal) const;
	const int getLength() const;
	void setLength(const int length);
		
	PRERACE();
	~PRERACE();
};

#endif // __PRERACE_H

