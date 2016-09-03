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

inline const unsigned int PRERACE::getLocationTotal(const unsigned int location_number, const unsigned int unit_type) const
{
#ifdef _SCC_DEBUG
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::getLocationTotal): Value location_number out of range.");return(0);
	}
#endif
	return(location[location_number].getTotal(unit_type));
}

inline void PRERACE::setMarker(const unsigned int ip, const unsigned int value)
{
#ifdef _SCC_DEBUG
	if(ip >= MAX_LENGTH) {
		toLog("DEBUG: (PRERACE::setMarker): Value ip out of range.");return;
	}	
#endif
	Marker[ip]=value;
}


inline void PRERACE::setCode(const unsigned int ip, const unsigned int value)
{
#ifdef _SCC_DEBUG
	if(ip >= MAX_LENGTH) {
		toLog("DEBUG: (PRERACE::setCode): Value ip out of range.");return;
	}	
	if(value >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (PRERACE::setCode): Value out of range.");return;
	}
#endif
	Code[ip]=value;
}

inline const unsigned int PRERACE::getCode(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
	if(ip >= MAX_LENGTH) {
		toLog("DEBUG: (PRERACE::getCode): Value ip out of range.");return(0);
	}	
	if(Code[ip] >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (PRERACE::getCode): Variable Code not initialized.");return(0);
	}
#endif
	return(Code[ip]);
}

inline const unsigned int PRERACE::getCurrentCode() const
{
#ifdef _SCC_DEBUG
	if(Code[getIP()] >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (PRERACE::getCurrentCode): Variable Code not initialized.");return(0);
	}
#endif
	return(Code[getIP()]);
}

inline const unsigned int PRERACE::getMarker(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
	if(ip > MAX_LENGTH) {
		toLog("DEBUG: (PRERACE::getMarker): Value ip out of range.");return(0);
	}
#endif
	return(Marker[ip]); 
}



// ------ UNITS ------
inline const unsigned int PRERACE::getMapLocationAvailible(const unsigned int player, const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(player >= (*pMap)->getMaxPlayer()) {
		toLog("DEBUG: (PRERACE::getMapLocationAvailible): Value player out of range.");return(0);
	}
	if((location_number >= MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::getMapLocationAvailible): Value location_number out of range.");return(0);
	}
#endif
	return(unit[player][location_number].getAvailible(unit_type));
}

inline const unsigned int PRERACE::getMapLocationTotal(const unsigned int player, const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(player >= (*pMap)->getMaxPlayer()) {
		toLog("DEBUG: (PRERACE::getMapLocationTotal): Value player out of range.");return(0);
	}
	if((location_number >= MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::getMapLocationTotal): Value location_number out of range.");return(0);
	}
#endif
	return(unit[player][location_number].getTotal(unit_type));
}

inline void PRERACE::setMapLocationAvailible(const unsigned int player, const unsigned int location_number, const unsigned int unit_type, const unsigned int availible)
{
#ifdef _SCC_DEBUG
	if(player >= (*pMap)->getMaxPlayer()) {
		toLog("DEBUG: (PRERACE::setMapLocationAvailible): Value player out of range.");return;
	}
	if((location_number >= MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::setMapLocationAvailible): Value location_number out of range.");return;
	}
#endif
	unit[player][location_number].setAvailible(unit_type, availible);
}

inline void PRERACE::setMapLocationTotal(const unsigned int player, const unsigned int location_number, const unsigned int unit_type, const unsigned int total)
{
#ifdef _SCC_DEBUG
	if(player >= (*pMap)->getMaxPlayer()) {
		toLog("DEBUG: (PRERACE::setMapLocationTotal): Value player out of range.");return;
	}
	if((location_number >= MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::setMapLocationTotal): Value location_number out of range.");return;
	}
#endif
	unit[player][location_number].setTotal(unit_type, total);
}

inline void PRERACE::addOneMapLocationAvailible(const unsigned int player, const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(player >= (*pMap)->getMaxPlayer()) {
		toLog("DEBUG: (PRERACE::addOneMapLocationAvailible): Value player out of range.");return;
	}
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::addOneMapLocationAvailible): Value location_number out of range.");return;
	}
#endif
	unit[player][location_number].addOneAvailible(unit_type);
}

inline void PRERACE::addOneMapLocationTotal(const unsigned int player, const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(player >= (*pMap)->getMaxPlayer()) {
		toLog("DEBUG: (PRERACE::addOneMapLocationTotal): Value player out of range.");return;
	}
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::addOneMapLocationTotal): Value location_number out of range.");return;
	}
#endif
	unit[player][location_number].addOneTotal(unit_type);
}

inline void PRERACE::removeOneMapLocationAvailible(const unsigned int player, const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(player >=(*pMap)->getMaxPlayer()) {
		toLog("DEBUG: (PRERACE::addOneMapLocationAvailible): Value player out of range.");return;
	}
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::addOneMapLocationAvailible): Value location_number out of range.");return;
	}
#endif
	unit[player][location_number].removeOneAvailible(unit_type);
}


inline void PRERACE::removeOneMapLocationTotal(const unsigned int player, const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(player >=(*pMap)->getMaxPlayer()) {
		toLog("DEBUG: (PRERACE::addOneMapLocationTotal): Value player out of range.");return;
	}
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::addOneMapLocationTotal): Value location_number out of range.");return;
	}
#endif
	unit[player][location_number].removeOneTotal(unit_type);
}

inline const unsigned int PRERACE::getLocationAvailible(const unsigned int location_number, const unsigned int unit_type) const
{
#ifdef _SCC_DEBUG
	if((location_number >= MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::getLocationAvailible): Value location_number out of range.");return(0);
	}
#endif
	return(location[location_number].getAvailible(unit_type));
}

inline void PRERACE::setLocationAvailible(const unsigned int location_number, const unsigned int unit_type, const unsigned int availible)
{
#ifdef _SCC_DEBUG
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::setLocationAvailible): Value location_number out of range.");return;
	}
#endif
	location[location_number].setAvailible(unit_type, availible);
}

inline void PRERACE::setLocationTotal(const unsigned int location_number, const unsigned int unit_type, const unsigned int total)
{
#ifdef _SCC_DEBUG
	if(location_number >= MAX_LOCATIONS)	{
		toLog("DEBUG: (PRERACE::setLocationTotal): Value location_number out of range.");return;
	}
#endif
	location[location_number].setTotal(unit_type, total);
}

inline void PRERACE::addLocationAvailible(const unsigned int location_number, const unsigned int unit_type, const unsigned int availible)
{
#ifdef _SCC_DEBUG
	if(location_number >= MAX_LOCATIONS) {
		toLog("DEBUG: (PRERACE::addLocationAvailible): Value location_number out of range.");return;
	}
#endif	
	location[location_number].addAvailible(unit_type, availible);
	if(location_number!=GLOBAL) //sonst waers ja doppelt...
		location[GLOBAL].addAvailible(unit_type, availible);
}

inline void PRERACE::addLocationTotal(const unsigned int location_number, const unsigned int unit_type, const unsigned int total)
{
#ifdef _SCC_DEBUG
	if(location_number >= MAX_LOCATIONS)	{
		toLog("DEBUG: (PRERACE::addLocationTotal): Value location_number out of range.");return;
	}
#endif
	location[location_number].addTotal( unit_type, total );
	if(location_number!=GLOBAL) // sonst waers ja doppelt wenn location = 0
		location[GLOBAL].addTotal(unit_type, total);
}

inline void PRERACE::addOneLocationAvailible(const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(location_number >= MAX_LOCATIONS) {
		toLog("DEBUG: (PRERACE::addLocationAvailible): Value location_number out of range.");return;
	}
#endif
	location[location_number].addOneAvailible( unit_type );
// also add one unit to the global location if global location was not already specified
	if(location_number!=GLOBAL) 
		location[GLOBAL].addOneAvailible( unit_type );
}

inline void PRERACE::addOneLocationTotal(const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(location_number >= MAX_LOCATIONS) {
		toLog("DEBUG: (PRERACE::addLocationTotal): Value location_number out of range.");return;
	}
#endif
	location[location_number].addOneTotal( unit_type );
	if(location_number!=GLOBAL) // sonst waers ja doppelt wenn location = 0
		location[GLOBAL].addOneTotal( unit_type );
}

inline void PRERACE::removeOneLocationAvailible(const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::removeOneLocationAvailible): Value location_number out of range.");return;
	}
#endif
	location[location_number].removeOneAvailible( unit_type );
// also add one unit to the global location if global location was not already specified
	if(location_number!=GLOBAL)
		location[GLOBAL].removeOneAvailible( unit_type );
}

inline void PRERACE::removeOneLocationTotal(const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::removeOneLocationTotal): Value location_number out of range.");return;
	}
#endif
	location[location_number].removeOneTotal( unit_type );
	if(location_number!=GLOBAL) 
		location[GLOBAL].removeOneTotal( unit_type );
}
// ------ END UNITS -----


// ----- SUPPLY ------
inline void PRERACE::setNeedSupply(const unsigned int need_supply)
{
#ifdef _SCC_DEBUG
	if(need_supply > 10*MAX_SUPPLY) {
		toLog("DEBUG: (PRERACE::setNeedSupply): Value need_supply out of range.");return;
	}
#endif
	needSupply = need_supply;
}

inline const unsigned int PRERACE::getNeedSupply() const
{
#ifdef _SCC_DEBUG
	if(needSupply > 10*MAX_SUPPLY) {
		toLog("DEBUG: (PRERACE::getNeedSupply): Variable not initialized.");return(0);
	}
#endif
	return(needSupply);
}

inline void PRERACE::setHaveSupply(const unsigned int have_supply)
{
#ifdef _SCC_DEBUG
	if(have_supply > 10*MAX_SUPPLY) {
		toLog("DEBUG: (PRERACE::setHaveSupply): Value have_supply out of range.");return;
	}
#endif
	haveSupply = have_supply;
}

inline const unsigned int PRERACE::getHaveSupply() const
{
#ifdef _SCC_DEBUG
	if(haveSupply>10*MAX_SUPPLY) {
		toLog("DEBUG: (PRERACE::getHaveSupply): Variable not initialized.");return(0);
	}
#endif
	return(haveSupply);
}
// ----- END SUPPLY ------

// ----- HARVEST ------

inline void PRERACE::setMinerals(const unsigned int have_minerals)
{
#ifdef _SCC_DEBUG
	if(have_minerals > MAX_MINERALS) {
		toLog("DEBUG: (PRERACE::setMinerals): Value have_minerals out of range.");return;
	}
#endif
	minerals = have_minerals;
}

inline const unsigned int PRERACE::getMinerals() const
{
#ifdef _SCC_DEBUG
	if(minerals > MAX_MINERALS) {
		toLog("DEBUG: (PRERACE::getMinerals): Variable minerals not initialized.");return(0);
	}
#endif
	return(minerals);
}

inline void PRERACE::setGas(const unsigned int have_gas)
{
#ifdef _SCC_DEBUG
	if(have_gas > MAX_GAS) {
		toLog("DEBUG: (PRERACE::setGas): Value have_gas out of range.");return;
	}
#endif
	gas = have_gas;
}

inline const unsigned int PRERACE::getGas() const
{
#ifdef _SCC_DEBUG
	if(gas > MAX_GAS) {
		toLog("DEBUG: (PRERACE::getGas): Variable gas not initialized.");return(0);
	}
#endif
	return(gas);
}

inline void PRERACE::setMineralHarvestPerSecond( const unsigned int location_number, const unsigned int worker, const unsigned int mineral_harvest_per_second )
{
#ifdef _SCC_DEBUG
	if(mineral_harvest_per_second >= MAX_MINERALS) {
		toLog("DEBUG: (PRERACE::setMineralHarvestPerSecond): Value mineral_harvest_per_second out of range.");return;
	}
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::setMineralHarvestPerSecond): Value location_number out of range.");return;
	}
	if(worker>=45) {
		toLog("DEBUG: (PRERACE::setMineralHarvestPerSecond): Value worker out of range.");return;
	}
#endif
	mineralHarvestPerSecond[location_number][worker] = mineral_harvest_per_second;
}

inline const unsigned int PRERACE::getMineralHarvestPerSecond( const unsigned int location_number, const unsigned int worker ) const
{
#ifdef _SCC_DEBUG
	if((location_number >= MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::getMineralHarvestPerSecond): Value location_number out of range.");return(0);
	}
	if(worker>=45) {
		toLog("DEBUG: (PRERACE::getMineralHarvestPerSecond): Value worker out of range.");return(0);
	}
	if(mineralHarvestPerSecond[location_number][worker]>MAX_MINERALS) {
		toLog("DEBUG: (PRERACE::getMineralHarvestPerSecond): Variable mineralHarvestPerSecond not initialized.");return(0);
	}
#endif
	return(mineralHarvestPerSecond[location_number][worker]);
}

inline void PRERACE::setGasHarvestPerSecond( const unsigned int location_number, const unsigned int worker, const unsigned int gas_harvest_per_second )
{
#ifdef _SCC_DEBUG
	if(gas_harvest_per_second >= MAX_GAS) {
		toLog("DEBUG: (PRERACE::setGasHarvestPerSecond): Value gas_harvest_per_second out of range.");return;
	}
	if((location_number >= MAX_LOCATIONS)) {
		toLog("DEBUG: (PRERACE::setGasHarvestPerSecond): Value location_number out of range.");return;
	}
	if(worker >= 5) {
		toLog("DEBUG: (PRERACE::setGasHarvestPerSecond): Value worker out of range.");return;
	}
#endif
	gasHarvestPerSecond[location_number][worker] = gas_harvest_per_second;
}

inline const unsigned int PRERACE::getGasHarvestPerSecond( const unsigned int location_number, const unsigned int worker ) const
{
#ifdef _SCC_DEBUG
	if((location_number >= MAX_LOCATIONS))	{
		toLog("DEBUG: (PRERACE::getGasHarvestPerSecond): Value location_number out of range.");return(0);
	}
	if(worker>=5)	{
		toLog("DEBUG: (PRERACE::getGasHarvestPerSecond): Value worker out of range.");return(0);
	}
	if(gasHarvestPerSecond[location_number][worker]>MAX_GAS) {
		toLog("DEBUG: (PRERACE::getGasHarvestPerSecond): Variable gasHarvestPerSecond not initialized.");return(0);
	}
#endif
	return(gasHarvestPerSecond[location_number][worker]);
}

inline void PRERACE::setHarvestedMinerals( const unsigned int harvested_minerals )
{
#ifdef _SCC_DEBUG
	if(harvested_minerals >= MAX_MINERALS) {
		toLog("DEBUG: (PRERACE::setHarvestedMinerals): Value out of range.");return;
	}
#endif
	harvestedMinerals = harvested_minerals;
}

inline const unsigned int PRERACE::getHarvestedMinerals() const
{
#ifdef _SCC_DEBUG
	if(harvestedMinerals>MAX_MINERALS) {
		toLog("DEBUG: (PRERACE::getHarvestedMinerals): Variable harvestedMinerals not initialized.");return(0);
	}
#endif
	return(harvestedMinerals);
}

inline void PRERACE::setHarvestedGas( const unsigned int harvested_gas )
{
#ifdef _SCC_DEBUG
	if(harvested_gas >= MAX_GAS) {
		toLog("DEBUG: (PRERACE::setHarvestedGas): Value out of range.");return;
	}
#endif
	harvestedGas = harvested_gas;
}

inline const unsigned int PRERACE::getHarvestedGas() const
{
#ifdef _SCC_DEBUG
	if(harvestedGas > MAX_GAS) {
		toLog("DEBUG: (PRERACE::getHarvestedGas): Variable harvestedGas not initialized.");return(0);
	}
#endif
	return(harvestedGas);
}


inline void PRERACE::setWastedMinerals( const unsigned int wasted_minerals )
{
#ifdef _SCC_DEBUG
	if(wasted_minerals >= MAX_MINERALS*MAX_TIME) {
		toLog("DEBUG: (PRERACE::setWastedMinerals): Value out of range.");return;
	}
#endif
	wastedMinerals = wasted_minerals;
}

inline const unsigned int PRERACE::getWastedMinerals() const
{
#ifdef _SCC_DEBUG
	if(wastedMinerals >= MAX_MINERALS*MAX_TIME) {
		toLog("DEBUG: (PRERACE::getWastedMinerals): Variable wastedMinerals not initialized.");return(0);
	}
#endif
	return(wastedMinerals);
}

inline void PRERACE::setWastedGas(const unsigned int wasted_gas)
{
#ifdef _SCC_DEBUG
	if(wasted_gas >= MAX_GAS*MAX_TIME) {
		toLog("DEBUG: (PRERACE::setWastedGas): Value out of range.");return;
	}
#endif
	wastedGas = wasted_gas;
}

inline const unsigned int PRERACE::getWastedGas() const
{
#ifdef _SCC_DEBUG
	if(wastedGas >= MAX_GAS*MAX_TIME) {
		toLog("DEBUG: (PRERACE::getWastedGas): Variable wastedGas not initialized.");return(0);
	}
#endif
	return(wastedGas);
}
// ----- END HARVEST -----

inline void PRERACE::setCurrentGoal(GOAL_ENTRY** current_goal)
{
#ifdef _SCC_DEBUG
	if(!current_goal) {
		toLog("DEBUG: (PRERACE::setCurrentGoal): Variable current_goal not initialized.");return;
	}
#endif
	pGoal = current_goal;
}

inline const unsigned int PRERACE::getPlayerNumber() const
{
#ifdef _SCC_DEBUG
	if(playerNum >= MAX_PLAYER) {
		toLog("DEBUG: (PRERACE::getPlayerNumber): Variable not initialized.");return(0);
	}
#endif
	return(playerNum);
}

inline const START_CONDITION* const* PRERACE::getStartCondition()
{
	return(pStartCondition);
}


inline void PRERACE::setTimer(const unsigned int time)
{
#ifdef _SCC_DEBUG
	if(time > configuration.getMaxTime()) {
		toLog("DEBUG: (PRERACE::setTimer): Value time out of range.");return;
	}
#endif
	timer = time;
}

inline const unsigned int PRERACE::getTimer() const
{
#ifdef _SCC_DEBUG
	if(timer > configuration.getMaxTime()) {
		toLog("DEBUG: (PRERACE::getTimer): Variable timer not initialized.");return(0);
	}
#endif
	return(timer);
}


inline const unsigned int PRERACE::getRealTimer() const
{
#ifdef _SCC_DEBUG
	if(timer > configuration.getMaxTime()) {
		toLog("DEBUG: (PRERACE::getRealTimer): Variable timer not initialized.");return(0);
	}
#endif
	return(configuration.getMaxTime()-timer);
	// TODO auf > checken
}

inline void PRERACE::setpStats(const UNIT_STATISTICS* const* player_stats)
{
#ifdef _SCC_DEBUG
	if((*player_stats)[0].minerals!=0) { // TODO
		toLog("DEBUG: (PRERACE::setpStats): Variable not initialized.");return;
	}
#endif
	pStats = player_stats;
}

inline const UNIT_STATISTICS* const * PRERACE::getpStats() const
{
#ifdef _SCC_DEBUG
	if(!pStats)	{
		toLog("DEBUG: (PRERACE::getpStats): Variable not initialized.");return(0);
	}
#endif
	return(pStats);
}

inline void PRERACE::setIP(const unsigned int ip)
{
#ifdef _SCC_DEBUG
	if(ip >= MAX_LENGTH) {
		toLog("DEBUG: (PRERACE::setIP): Value out of range.");return;
	}
#endif
	IP = ip;
}

inline const unsigned int PRERACE::getIP() const
{
#ifdef _SCC_DEBUG
	if(IP>MAX_LENGTH) {
		toLog("DEBUG: (PRERACE::getIP): Variable IP not initialized.");return(0);
	}
#endif
	return(IP);
}

/*const unsigned int PRERACE::getFinalTime(const unsigned int goal) const
{
#ifdef _SCC_DEBUG
	if((goal<0)||(goal>=MAX_GOALS))	{
		toLog("DEBUG: (PRERACE::getFinalTime): Value goal out of range.");return(0);
	}
	if((ftime[goal]<0)||(ftime[goal]>=MAX_TIME)) {
		toLog("DEBUG: (PRERACE::getFinalTime): Variable ftime not initialized.");return(0);
	}
#endif
	return(ftime[goal]);
}*/

inline const unsigned int PRERACE::getLength() const
{
#ifdef _SCC_DEBUG
	if(length>MAX_LENGTH) {
		toLog("DEBUG: (PRERACE::getLength): Variable length not initialized.");return(0);
	}
#endif
	return(length);
}

/*void PRERACE::setFinalTime(const unsigned int goal, const unsigned int time)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>=MAX_TIME)) {
		toLog("DEBUG: (PRERACE::setFinalTime): Value time out of range.");return;
	}
	if((goal<0)||(goal>=MAX_GOALS)) {
		toLog("DEBUG: (PRERACE::setFinalTime): Value goal out of range.");return;
	}
#endif
	ftime[goal]=time;
}*/

inline void PRERACE::setLength(const unsigned int bo_length)
{
#ifdef _SCC_DEBUG
	if(bo_length > MAX_LENGTH)
	{
		toLog("DEBUG: (PRERACE::setLength): Value bo_length out of range.");return;
	}
#endif
	length = bo_length;
}


inline void PRERACE::setTimeOut(const unsigned int time_out)
{
#ifdef _SCC_DEBUG
	if(time_out > configuration.getMaxTimeOut()) {
		toLog("DEBUG: (PRERACE::setTimeOut): Value time_out out of range.");return;
	}
#endif
	timeout = time_out;
}

inline const unsigned int PRERACE::getTimeOut() const
{
#ifdef _SCC_DEBUG
	if(timeout > configuration.getMaxTimeOut()) {
		toLog("DEBUG: (PRERACE::getTimeOut): Variable timeout not initialized.");return(0);
	}
#endif
	return(timeout);
}

inline GOAL_ENTRY** PRERACE::getCurrentGoal() const
{
	return(pGoal);
}

inline const BASIC_MAP* const* PRERACE::getMap()
{
	return(pMap);
}

// --------------------------------------
// ------ END OF GET/SET FUNCTIONS ------
// --------------------------------------




#endif // __PRERACE_H

