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

class PRERACE
{
protected:

//------------------ optimizations:-----------------
//	UNIT* global; // non-static pointer to player's global total force (location 0.total)
	UNIT* location; // non-static pointer to players total/availible units

	
	const START_CONDITION* const* pStartCondition; //pointer to player in start

	static const BASIC_MAP* const* pMap; // MAP is all the same for all players using 'start
//------------- end -------------------------------

	std::priority_queue<Building, std::deque<Building> > buildingQueue;

// global best time so far
	static signed int noise[MAX_TIME];
	static unsigned int markerCounter;
	static UNIT unit[MAX_PLAYER][MAX_LOCATIONS]; // player 0 is neutral player!
	LAST last[MAX_LENGTH]; // last* is to save the last position, for movements
	unsigned int lastcounter;
	unsigned int lastunit;
	const UNIT_STATISTICS* const* pStats;
	unsigned int neededMinerals, neededGas;
	
	static START* pStart;
	static GA* ga;
	
	void createSpecial();
	void resetSpecial();
	const unsigned int harvestMinerals() const;
	const unsigned int harvestGas() const; 

	const bool calculateReady() const;
	void adjustAvailibility(const unsigned int location, const unsigned int fac, const UNIT_STATISTICS* stat);
	void adjustLocationUnitsAfterCompletion(const unsigned int location, const eFacilityType facilityType, const unsigned int facility, const unsigned int facility2);
	const unsigned int calculatePrimaryFitness(const bool ready);
	void replaceCode(const unsigned int IP, const unsigned int code);

	unsigned int larvaInProduction[MAX_LOCATIONS]; // well... one of that ugly race-specific variables saves a lot of trouble...

//TODO: PlayerNum von anarace ist irgendwie 2 statt 1 oder so ... auch ist die Frage wie was upgedatet wird...
//		 Deutlich durchsichtiger machen...
	const unsigned int calculateIdleTime() const; // calculate the next time something significant will happen
	GOAL_ENTRY* getpGoal() const {
		return(*pGoal);
	}

	bool ready;
private:
	GOAL_ENTRY** pGoal; // pStart->getGoal()
	unsigned int playerNum;
	unsigned int minerals, gas, timer;
	unsigned int IP;
	unsigned int mineralHarvestPerSecond[MAX_LOCATIONS][45];
	unsigned int gasHarvestPerSecond[MAX_LOCATIONS][5];
	unsigned int harvestedMinerals, harvestedGas;
	unsigned int wastedMinerals, wastedGas;
	unsigned int needSupply;		// free supply
	unsigned int haveSupply; // total supply
//	int ftime[MAX_GOALS]; //when the goal is reached / when the last item is produced (ALL locations...*/
	unsigned int length;
	unsigned int timeout;

	unsigned int Code[MAX_LENGTH];
	unsigned int Marker[MAX_LENGTH];

public:

// ------ PUBLIC VARIABLES ------


	static void assignStart(START* start);
	static void assignGA(GA* pga);
	static void initNoise();
	static void copyMap(); //copies the startforce from map to static 'units'
	static const BASIC_MAP* const* getMap(); 	

// ------ HARVEST ROUTINES ------
	void adjustMineralHarvest(const unsigned int location);
	void adjustGasHarvest(const unsigned int location);
	void adjustHarvestAllLocations();
	void setMineralHarvestPerSecond(const unsigned int location, const unsigned int worker, const unsigned int minerals);
	const unsigned int getMineralHarvestPerSecond(const unsigned int location, const unsigned int worker) const;
	void setGasHarvestPerSecond(const unsigned int location, const unsigned int worker, const unsigned int gas);
	const unsigned int getGasHarvestPerSecond(const unsigned int location, const unsigned int worker) const;
	const unsigned int getHarvestedMinerals() const;
	const unsigned int getHarvestedGas() const;
	void setHarvestedMinerals(const unsigned int harvestedMinerals);
	void setHarvestedGas(const unsigned int harvestedGas);
	
	const unsigned int getWastedMinerals() const;
	const unsigned int getWastedGas() const;
	void setWastedMinerals(const unsigned int minerals);
	void setWastedGas(const unsigned int gas);

// ------ INITIALIZATION ROUTINES ------
	static void resetGeneMarker();
	void setPlayerNum(const unsigned int playerNum); // assigns player data from start (start minerals, supply etc.) and sets the appropriate optimized pointers (global, location, pMap etc.) CALL IT AFTER EACH MAP CHANGE AND PLAYER CHANGE!!
	void initializePlayer();
	void prepareForNewGeneration();

	void eraseIllegalCode();
	void eraseUselessCode();
	void mutateGeneCode();
	void resetGeneCode();//resets either to a pre-processed buildorder or a completely random one*/
//	void crossOver(PRERACE* parent2, PRERACE* child1, PRERACE* child2); TODO
	
// ------ GET/SET ROUTINES ------
	const unsigned int getCode(const unsigned int IP) const;
	const unsigned int getCurrentCode() const;
	const unsigned int getMarker(const unsigned int IP) const;
	void copyCode(PRERACE& player);

	void addLarvaToQueue(const unsigned int location);
	void removeLarvaFromQueue(const unsigned int location);

	const eRace getRace() const;
	GOAL_ENTRY** getCurrentGoal() const;
	void setCurrentGoal(GOAL_ENTRY** pGoal);
	
    const START_CONDITION* const* getStartCondition(); //pointer to player in start
	
	static const unsigned int getMapLocationAvailible(const unsigned int player, const unsigned int location, const unsigned int unittype);
	static const unsigned int getMapLocationTotal(const unsigned int player, const unsigned int location, const unsigned int unittype);
																				
	static void setMapLocationAvailible(const unsigned int player, const unsigned int location, const unsigned int unittype, const unsigned int availible);
	static void setMapLocationTotal(const unsigned int player, const unsigned int location, const unsigned int unittype, const unsigned int total);
																				
	static void addOneMapLocationAvailible(const unsigned int player, const unsigned int location, const unsigned int unittype);
	static void addOneMapLocationTotal(const unsigned int player, const unsigned int location, const unsigned int unittype);
	static void removeOneMapLocationAvailible(const unsigned int player, const unsigned int location, const unsigned int unittype);
	static void removeOneMapLocationTotal(const unsigned int player, const unsigned int location, const unsigned int unittype);
																			
	const unsigned int getLocationAvailible(const unsigned int location, const unsigned int unittype) const;
	const unsigned int getLocationTotal(const unsigned int location, const unsigned int unittype) const;
																				
	void setLocationAvailible(const unsigned int location, const unsigned int unittype, const unsigned int availible);
	void setLocationTotal(const unsigned int location, const unsigned int unittype, const unsigned int total);
																				
	void addLocationAvailible(const unsigned int location, const unsigned int unittype, const unsigned int availible);
	void addLocationTotal(const unsigned int location, const unsigned int unittype, const unsigned int total);

	void addOneLocationAvailible(const unsigned int location, const unsigned int unittype);
	void addOneLocationTotal(const unsigned int location, const unsigned int unittype);
	void removeOneLocationAvailible(const unsigned int location, const unsigned int unittype);
	void removeOneLocationTotal(const unsigned int location, const unsigned int unittype);

	void setNeedSupply(const unsigned int needSupply);
	void setHaveSupply(const unsigned int haveSupply);
	void setMinerals(const unsigned int minerals);
	void setGas(const unsigned int gas);

	void setpStats(const UNIT_STATISTICS* const* pStats);
	const UNIT_STATISTICS* const* getpStats() const;

	const unsigned int getPlayerNum() const;
	const unsigned int getNeedSupply() const;
	const unsigned int getHaveSupply() const;
	const unsigned int getMinerals() const;
	const unsigned int getGas() const;

	void setTimer(const unsigned int time);
	const unsigned int getTimer() const;
	const unsigned int getRealTimer() const;

	void setTimeOut(const unsigned int time);
	const unsigned int getTimeOut() const;

	const unsigned int getIP() const;
	void setIP(const unsigned int IP);

	//void setFinalTime(const unsigned int goal, const unsigned int time);
//	const unsigned int getFinalTime(const unsigned int goal) const;
	
	const unsigned int getLength() const;
	void setLength(const unsigned int length);

	PRERACE& operator=(const PRERACE& prerace);
	PRERACE(const PRERACE& prerace);

	PRERACE();
	virtual ~PRERACE();
};

#endif // __PRERACE_H

