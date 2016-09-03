#ifndef _CORE_PRERACE_HPP
#define _CORE_PRERACE_HPP

#include "building.hpp"
#include "configuration.hpp"
#include "start.hpp"
#include <functional>
#include <deque>
#include <queue>
#include <map>

/*class CODE
{
	public:
		CODE(const unsigned int* code, const unsigned int code_length);
		~CODE();
		const bool operator==(const CODE& object) const;
		const bool operator<(const CODE& object) const;
		CODE& operator=(const unsigned int* code);
	private:
		unsigned int Code[MAX_LENGTH];
		unsigned int length;
};

class SITUATION
{
	public:
		SITUATION();
		SITUATION(const SITUATION& object);
		~SITUATION();
		
		unsigned int getTMaxBuildTypes() const;
		unsigned int getForce(const unsigned int unit) const;
		unsigned int getBuildable(const unsigned int unit) const;
		unsigned int getTGeno(const unsigned int unit) const;
		unsigned int getTTGeno(const unsigned int unit) const;

		void setTMaxBuildTypes(const unsigned int count);
		void addTMaxBuildTypes();
		void subTMaxBuildTypes();
		void setForce(const unsigned int unit, const unsigned int count);
		void setBuildable(const unsigned int unit, const unsigned int count);
		void setTGeno(const unsigned int unit, const unsigned int count);
		void setTTGeno(const unsigned int unit, const unsigned int count);


	private:
		unsigned int tMaxBuildTypes;
		unsigned int  force[UNIT_TYPE_COUNT];
		unsigned int buildable[UNIT_TYPE_COUNT];
		unsigned int tGeno[UNIT_TYPE_COUNT]; // !! keine anderen units drueber nehmen!
		unsigned int ttGeno[UNIT_TYPE_COUNT];
};*/

class PRERACE
{
protected:

//------------------ optimizations:-----------------
//	UNIT* global; // non-static pointer to player's global total force (location 0.total)
	UNIT* location; // non-static pointer to players total/availible units

	const START_CONDITION* const* pStartCondition; //pointer to player in start

	static const BASIC_MAP* const* pMap; // MAP is all the same for all players using 'start
//------------- end -------------------------------

	priority_queue<Building, deque<Building> > buildingQueue;

// global best time so far
	static signed int noise[MAX_TIME];
	static unsigned int markerCounter;
	static UNIT unit[MAX_PLAYER][MAX_LOCATIONS]; // player 0 is neutral player!
//	LAST last[MAX_LENGTH]; // last* is to save the last position, for movements
//	unsigned int lastcounter;
//	unsigned int lastunit;
	const UNIT_STATISTICS* const* pStats;
	unsigned int neededMinerals, neededGas;
	
	static START* pStart;
	
	void createSpecial();
	void resetSpecial();
	void resetPrerace();
	static void resetStaticPrerace();

	const unsigned int harvestMinerals() const;
	const unsigned int harvestGas() const; 

	const bool calculateReady() const;
	void adjustAvailibility(const unsigned int location_number, const unsigned int fac, const UNIT_STATISTICS* stat);
	void adjustLocationUnitsAfterCompletion(const unsigned int location_number, const eFacilityType facilityType, const unsigned int facility, const unsigned int facility2);
	const unsigned int calculatePrimaryFitness(const bool ready);
	void replaceCode(const unsigned int ip, const unsigned int code);

	unsigned int larvaInProduction[MAX_LOCATIONS]; // well... one of that ugly race-specific variables saves a lot of trouble...

//TODO: PlayerNum von anarace ist irgendwie 2 statt 1 oder so ... auch ist die Frage wie was upgedatet wird...
//		 Deutlich durchsichtiger machen...
	const unsigned int calculateIdleTime() const; // calculate the next time something significant will happen
	GOAL_ENTRY* getpGoal() const {
		return(*pGoal);
	}

	bool ready;
	
	GOAL_ENTRY** pGoal; // pStart->getGoal()
private:
	unsigned int Code[MAX_LENGTH];
	unsigned int Marker[MAX_LENGTH];

	unsigned int playerNum;
	unsigned int minerals, gas, timer;
	unsigned int IP;
	unsigned int mineralHarvestPerSecond[MAX_LOCATIONS][45];
	unsigned int gasHarvestPerSecond[MAX_LOCATIONS][5];
	unsigned int harvestedMinerals, harvestedGas;
	unsigned int wastedMinerals, wastedGas;
	unsigned int needSupply;		// free supply
	unsigned int haveSupply; // total supply
	unsigned int length;
	unsigned int timeout;



public:
	static void assignStart(START* start);
//	static map<CODE, SITUATION*> situationHashMap;

//	static void assignConfiguration(Configuration* pconfiguration);
	static void initNoise();
	static void copyMap(); //copies the startforce from map to static 'units'
	static const BASIC_MAP* const* getMap(); 	

	PRERACE();
	virtual ~PRERACE();

	PRERACE& operator=(const PRERACE& object);
	PRERACE(const PRERACE& object);

	const bool isDifferent(const unsigned int* code, const unsigned int* marker) const;

// ------ HARVEST ROUTINES ------
	void adjustMineralHarvest(const unsigned int location_number);
	void adjustGasHarvest(const unsigned int location_number);
	void adjustHarvestAllLocations();
	void setMineralHarvestPerSecond(const unsigned int location_number, const unsigned int worker, const unsigned int harvest_minerals);
	const unsigned int getMineralHarvestPerSecond(const unsigned int location_number, const unsigned int worker) const;
	void setGasHarvestPerSecond(const unsigned int location_number, const unsigned int worker, const unsigned int harvest_gas);
	const unsigned int getGasHarvestPerSecond(const unsigned int location_nubmer, const unsigned int worker) const;
	const unsigned int getHarvestedMinerals() const;
	const unsigned int getHarvestedGas() const;
	void setHarvestedMinerals(const unsigned int harvested_minerals);
	void setHarvestedGas(const unsigned int harvested_gas);
	
	const unsigned int getWastedMinerals() const;
	const unsigned int getWastedGas() const;
	void setWastedMinerals(const unsigned int wasted_minerals);
	void setWastedGas(const unsigned int wasted_gas);

// ------ INITIALIZATION ROUTINES ------
	void setPlayerNumber(const unsigned int player_number); // assigns player data from start (start minerals, supply etc.) and sets the appropriate optimized pointers (global, location, pMap etc.) CALL IT AFTER EACH MAP CHANGE AND PLAYER CHANGE!!
	void initializePlayer();
	void prepareForNewGeneration();

	void eraseIllegalCode();
	void eraseUselessCode();
	void mutateGeneCode(/*const bool* fixed_list*/);
	void resetGeneCode();//resets either to a pre-processed buildorder or a completely random one*/
//	void crossOver(PRERACE* parent2, PRERACE* child1, PRERACE* child2); TODO


	const bool buildIt(const unsigned int build_unit);
	
// ------ GET/SET ROUTINES ------
	void setMarker(const unsigned int ip, const unsigned int value);
	void setCode(const unsigned int ip, const unsigned int value);
	const unsigned int getCode(const unsigned int ip) const;
	const unsigned int getCurrentCode() const;
	const unsigned int getMarker(const unsigned int ip) const;
	void copyCode(PRERACE& player);

	void addLarvaToQueue(const unsigned int location_number);
	void removeLarvaFromQueue(const unsigned int location_number);

	const eRace getRace() const;
	GOAL_ENTRY** getCurrentGoal() const;
	void setCurrentGoal(GOAL_ENTRY** current_goal);
	
    const START_CONDITION* const* getStartCondition(); //pointer to player in start
	
	static const unsigned int getMapLocationAvailible(const unsigned int player, const unsigned int location_number, const unsigned int unit_type);
	static const unsigned int getMapLocationTotal(const unsigned int player, const unsigned int location_number, const unsigned int unit_type);
																				
	static void setMapLocationAvailible(const unsigned int player, const unsigned int location_number, const unsigned int unit_type, const unsigned int availible);
	static void setMapLocationTotal(const unsigned int player, const unsigned int location_number, const unsigned int unit_type, const unsigned int total);
																				
	static void addOneMapLocationAvailible(const unsigned int player, const unsigned int location_number, const unsigned int unit_type);
	static void addOneMapLocationTotal(const unsigned int player, const unsigned int location_number, const unsigned int unit_type);
	static void removeOneMapLocationAvailible(const unsigned int player, const unsigned int location_number, const unsigned int unit_type);
	static void removeOneMapLocationTotal(const unsigned int player, const unsigned int location_number, const unsigned int unit_type);
																			
	const unsigned int getLocationAvailible(const unsigned int location_number, const unsigned int unit_type) const;
	const unsigned int getLocationTotal(const unsigned int location_number, const unsigned int unit_type) const;
																				
	void setLocationAvailible(const unsigned int location_number, const unsigned int unit_type, const unsigned int availible);
	void setLocationTotal(const unsigned int location_number, const unsigned int unit_type, const unsigned int total);
																				
	void addLocationAvailible(const unsigned int location_number, const unsigned int unit_type, const unsigned int availible);
	void addLocationTotal(const unsigned int location_number, const unsigned int unit_type, const unsigned int total);

	void addOneLocationAvailible(const unsigned int location_number, const unsigned int unit_type);
	void addOneLocationTotal(const unsigned int location_number, const unsigned int unit_type);
	void removeOneLocationAvailible(const unsigned int location_number, const unsigned int unit_type);
	void removeOneLocationTotal(const unsigned int location_number, const unsigned int unit_type);

	void setNeedSupply(const unsigned int need_supply);
	void setHaveSupply(const unsigned int have_supply);
	void setMinerals(const unsigned int have_minerals);
	void setGas(const unsigned int have_gas);

	void setpStats(const UNIT_STATISTICS* const* player_stats);
	const UNIT_STATISTICS* const* getpStats() const;

	const unsigned int getPlayerNumber() const;
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
	void setIP(const unsigned int ip);

	const unsigned int getLength() const;
	void setLength(const unsigned int bo_length);

};

#endif // __PRERACE_H

