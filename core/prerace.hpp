#ifndef _CORE_PRERACE_HPP
#define _CORE_PRERACE_HPP

#include "basicmap.hpp"
#include "ga.hpp"
#include "location.hpp"
#include "building.hpp"
#include "start.hpp"
#include <queue>
#include <deque>
#include <functional>

class EXPORT PRERACE
{
protected:
	static START* pStart;

//------------------ optimizations:-----------------
//	UNIT* global; // non-static pointer to player's global total force (location 0.total)
	UNIT* location; // non-static pointer to players total/availible units

	
	const START_CONDITION* const* pStartCondition; //pointer to player in start

	const static BASIC_MAP* const* pMap; // MAP is all the same for all players using 'start
//------------- end -------------------------------

	priority_queue<Building, deque<Building> >	buildingQueue;

// global best time so far
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
	void adjustLocationUnitsAfterCompletion(const int location, const eFacilityType facilityType, const int facility, const int facility2);
	const int calculatePrimaryFitness(const bool ready) const;
	void replaceCode(const int IP, const int code);

	int larvaInProduction[MAX_LOCATIONS]; // well... one of that ugly race-specific variables saves a lot of trouble...

//TODO: PlayerNum von anarace ist irgendwie 2 statt 1 oder so ... auch ist die Frage wie was upgedatet wird...
//		 Deutlich durchsichtiger machen...
	const int calculateIdleTime() const; // calculate the next time something significant will happen
	GOAL_ENTRY* getpGoal() const {
		return(*pGoal);
	}
	static GA* ga;

private:
	GOAL_ENTRY** pGoal; // pStart->getGoal()
	int playerNum;
	int minerals,gas,timer;
	int IP;
	int mineralHarvestPerSecond[MAX_LOCATIONS][45];
	int gasHarvestPerSecond[MAX_LOCATIONS][5];
	int harvestedGas,harvestedMinerals;
	int wastedGas,wastedMinerals;
	int needSupply;		// free supply
	int haveSupply; // total supply
//	int ftime[MAX_GOALS]; //when the goal is reached / when the last item is produced (ALL locations...*/
	int length,timeout;

	int Code[MAX_LENGTH];
	int Marker[MAX_LENGTH];

public:

// ------ PUBLIC VARIABLES ------


	static void assignStart(START* start);
	static void assignGA(GA* pga);
	static void initNoise();
	static void copyMap(); //copies the startforce from map to static 'units'
	static const BASIC_MAP* const* getMap(); 	

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
	void setHarvestedMinerals(const int harvestedMinerals);
	void setHarvestedGas(const int harvestedGas);
	
	const int getWastedMinerals() const;
	const int getWastedGas() const;
	void setWastedMinerals(const int minerals);
	void setWastedGas(const int gas);

// ------ INITIALIZATION ROUTINES ------
	static void resetGeneMarker();
	void setPlayerNum(const int playerNum); // assigns player data from start (start minerals, supply etc.) and sets the appropriate optimized pointers (global, location, pMap etc.) CALL IT AFTER EACH MAP CHANGE AND PLAYER CHANGE!!
	void initializePlayer();
	void prepareForNewGeneration();

	void eraseIllegalCode();
	void eraseUselessCode();
	void mutateGeneCode();
	void resetGeneCode();//resets either to a pre-processed buildorder or a completely random one*/
	void crossOver(PRERACE* parent2, PRERACE* child1, PRERACE* child2);
	
// ------ GET/SET ROUTINES ------
	const int getCode(const int IP) const;
	const int getCurrentCode() const;
	const int getMarker(const int IP) const;
	void copyCode(PRERACE& player);

	void addLarvaToQueue(const int location);
	void removeLarvaFromQueue(const int location);

	const eRace getRace() const;
	GOAL_ENTRY** getCurrentGoal() const;
	void setCurrentGoal(GOAL_ENTRY** pGoal);
	
    const START_CONDITION* const* getStartCondition(); //pointer to player in start
	
	const static int getMapLocationAvailible(const int player, const int location, const int unittype);
	const static int getMapLocationTotal(const int player, const int location, const int unittype);
																				
	static void setMapLocationAvailible(const int player, const int location, const int unittype, const int availible);
	static void setMapLocationTotal(const int player, const int location, const int unittype, const int total);
																				
	static void addMapLocationAvailible(const int player, const int location, const int unittype, const int availible);
	static void addMapLocationTotal(const int player, const int location, int unittype, const int total);
																				
	const int getLocationAvailible(const int location, const int unittype) const;
	const int getLocationTotal(const int location, const int unittype) const;
																				
	void setLocationAvailible(const int location, int unittype, int availible);
	void setLocationTotal(int location, const int unittype, int total);
																				
	void addLocationAvailible(const int location, const int unittype, const int availible);
	void addLocationTotal(const int location, const int unittype, const int total);

	void addOneLocationAvailible(const int location, const int unittype);
	void addOneLocationTotal(const int location, const int unittype);
	void removeOneLocationAvailible(const int location, const int unittype);
	void removeOneLocationTotal(const int location, const int unittype);

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
	const int getRealTimer() const;

	void setTimeOut(const int time);
	const int getTimeOut() const;

	const int getIP() const;
	void setIP(const int IP);

	//void setFinalTime(const int goal, const int time);
//	const int getFinalTime(const int goal) const;
	
	const int getLength() const;
	void setLength(const int length);
		
	PRERACE();
	~PRERACE();
};

#endif // __PRERACE_H

