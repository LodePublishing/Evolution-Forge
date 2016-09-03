#ifndef _CORE_PREBUILDORDER_HPP
#define _CORE_PREBUILDORDER_HPP

#include "building.hpp"
#include "configuration.hpp"
#include "start.hpp"
#include <functional>
#include <queue>
#include <map>

struct PARALLEL_COMMAND
{
	unsigned int unit;
	unsigned int count;
};

class PREBUILDORDER
{
	protected:

//------------------ optimizations:-----------------
//		UNIT (*location)[MAX_LOCATIONS]; // non-static pointer to players total/availible units (unit[playerNum])

//		const START_CONDITION* const* pStartCondition; //pointer to player in start

		const BASIC_MAP* const* pMap; // MAP is all the same for all players using 'start
//------------- end -------------------------------

		std::priority_queue<Building, std::vector<Building> > buildingQueue;
		std::list<PARALLEL_COMMAND*> parallelCommandQueues;

// global best time so far
//		static signed int noise[MAX_TIME];
		UNIT (*unit)[MAX_INTERNAL_PLAYER][MAX_LOCATIONS]; // player 0 is neutral player!
//		LAST last[MAX_LENGTH]; // last* is to save the last position, for movements
//		unsigned int lastcounter;
//		unsigned int lastunit;
		const UNIT_STATISTICS* const* pStats;
		unsigned int neededMinerals, neededGas;
	
		START* pStart;
	
		void createSpecial();
		void resetSpecial();
		void resetPrerace();

		const unsigned int harvestMinerals() const;
		const unsigned int harvestGas() const; 

		const bool calculateReady() const;
		void adjustAvailibility(const unsigned int location_number, const unsigned int fac, const UNIT_STATISTICS* stat);
		void adjustLocationUnitsAfterCompletion(const unsigned int location_number, const eFacilityType facilityType, const unsigned int facility, const unsigned int facility2, const unsigned int count);
		const unsigned int calculatePrimaryFitness(const bool ready);
		const bool replaceCode(const unsigned int ip, const unsigned int code);

		unsigned int larvaInProduction[MAX_LOCATIONS]; // well... one of that ugly race-specific variables saves a lot of trouble...

//TODO: PlayerNum von anarace ist irgendwie 2 statt 1 oder so ... auch ist die Frage wie was upgedatet wird...
//		 Deutlich durchsichtiger machen...
		const unsigned int calculateIdleTime() const; // calculate the next time something significant will happen, CONST MACHEN
		
		const bool checkForLarva(const unsigned int current_location_window);
	
		bool ready;
	
		GOAL_ENTRY* pGoal; // pStart->getGoal()
		
		bool alwaysBuildWorkers;
		bool onlySwapOrders;

	private:
		bool conditionsChanged;
		unsigned int Code[MAX_LENGTH];

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
		const bool haveConditionsChanged() const;
		
		const bool setAlwaysBuildWorkers(const bool always_build_workers = true);
		const bool setOnlySwapOrders(const bool only_swap_orders = true);

		const bool isAlwaysBuildWorkers() const;
		const bool isOnlySwapOrders() const;
	
		void assignStart(START* start);
		void assignUnits(UNIT (*units)[MAX_INTERNAL_PLAYER][MAX_LOCATIONS]);

                void setStartPosition(const unsigned int startPosition);
                void setRace(const eRace race); // => gleichzeitig wird harvestspeed geaendert und condition und goal muessen u.U. neugewaehlt werden!
                void assignStartCondition(const START_CONDITION* start_condition, const bool neutral_player = false);
                void assignGoal(const GOAL_ENTRY* goal);		
		
//		static map<CODE, SITUATION*> situationHashMap;

//		static void assignConfiguration(Configuration* pconfiguration);
//		static void initNoise();
		const BASIC_MAP* const* getMap() const; 	
		GOAL_ENTRY* getGoal() const;

		PREBUILDORDER();
		virtual ~PREBUILDORDER();

		PREBUILDORDER& operator=(const PREBUILDORDER& object);
		PREBUILDORDER(const PREBUILDORDER& object);

		const bool isDifferent(const unsigned int* code) const; //, const unsigned int* marker) const;

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
//		void crossOver(PREBUILDORDER* parent2, PREBUILDORDER* child1, PREBUILDORDER* child2); TODO


	
// ------ GET/SET ROUTINES ------
//		void setMarker(const unsigned int ip, const unsigned int value);
		void setCode(const unsigned int ip, const unsigned int value);
		const unsigned int* getCodePointer() const; 
		const unsigned int getCode(const unsigned int ip) const;
		const unsigned int getCurrentCode() const;
//		const unsigned int getMarker(const unsigned int ip) const;
		void copyCode(PREBUILDORDER& player);
		void copyCode(unsigned int* dst) const;

		void addLarvaToQueue(const unsigned int location_number);
		void removeLarvaFromQueue(const unsigned int location_number);

		const eRace getRace() const;
		void setGoal(GOAL_ENTRY* current_goal);
	
		const START_CONDITION* const* getStartCondition(); //pointer to player in start
	
		const unsigned int getMapLocationAvailible(const unsigned int player, const unsigned int location_number, const unsigned int unit_type);
		const unsigned int getMapLocationTotal(const unsigned int player, const unsigned int location_number, const unsigned int unit_type);
																				
		void setMapLocationAvailible(const unsigned int player, const unsigned int location_number, const unsigned int unit_type, const unsigned int availible);
		void setMapLocationTotal(const unsigned int player, const unsigned int location_number, const unsigned int unit_type, const unsigned int total);
																				
		void setConditionsChanged(const bool conditions_changed = true);
		
		void addOneMapLocationAvailible(const unsigned int player, const unsigned int location_number, const unsigned int unit_type);
		void addOneMapLocationTotal(const unsigned int player, const unsigned int location_number, const unsigned int unit_type);
		void removeOneMapLocationAvailible(const unsigned int player, const unsigned int location_number, const unsigned int unit_type);
		void removeOneMapLocationTotal(const unsigned int player, const unsigned int location_number, const unsigned int unit_type);
																			
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

inline const bool PREBUILDORDER::haveConditionsChanged() const
{
	return(conditionsChanged);
}

inline const BASIC_MAP* const* PREBUILDORDER::getMap() const
{
	return(pMap);
}

inline void PREBUILDORDER::setIP(const unsigned int ip)
{
#ifdef _SCC_DEBUG
	if(ip >= MAX_LENGTH) {
		toLog("DEBUG: (PREBUILDORDER::setIP): Value out of range.");return;
	}
#endif
	IP = ip;
}

inline const unsigned int PREBUILDORDER::getIP() const
{
#ifdef _SCC_DEBUG
	if(IP>MAX_LENGTH) {
		toLog("DEBUG: (PREBUILDORDER::getIP): Variable IP not initialized.");return(0);
	}
#endif
	return(IP);
}

inline GOAL_ENTRY* PREBUILDORDER::getGoal() const {
	return(pGoal);
}

inline void PREBUILDORDER::setStartPosition(const unsigned int startPosition) 
{
	if(pStart->setStartPosition(startPosition))
		setConditionsChanged();
}

inline void PREBUILDORDER::setRace(const eRace race) // => gleichzeitig wird harvestspeed geaendert und condition und goal muessen u.U. neugewaehlt werden!
{
	if(pStart->setPlayerRace(race))
		setConditionsChanged();
}



inline void PREBUILDORDER::assignGoal(const GOAL_ENTRY* goal) {
	if(pStart->assignGoal(goal))
		setConditionsChanged();
}

inline const unsigned int PREBUILDORDER::getLocationTotal(const unsigned int location_number, const unsigned int unit_type) const
{
#ifdef _SCC_DEBUG
	if((location_number >= (*getMap())->getMaxLocations())) {
		toLog("DEBUG: (PREBUILDORDER::getLocationTotal): Value location_number out of range.");return(0);
	}
#endif
	return((*unit)[playerNum][location_number].getTotal(unit_type));
//	return((*location)[location_number].getTotal(unit_type));
}


inline void PREBUILDORDER::setCode(const unsigned int ip, const unsigned int value)
{
#ifdef _SCC_DEBUG
	if(ip >= MAX_LENGTH) {
		toLog("DEBUG: (PREBUILDORDER::setCode): Value ip out of range.");return;
	}	
	if(value >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (PREBUILDORDER::setCode): Value out of range.");return;
	}
#endif
	Code[ip]=value;
}

inline const unsigned int* PREBUILDORDER::getCodePointer() const {
	return(Code);
}

inline const unsigned int PREBUILDORDER::getCode(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
	if(ip >= MAX_LENGTH) {
		toLog("DEBUG: (PREBUILDORDER::getCode): Value ip out of range.");return(0);
	}	
	if(Code[ip] >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (PREBUILDORDER::getCode): Variable Code not initialized.");return(0);
	}
#endif
	return(Code[ip]);
}

inline const unsigned int PREBUILDORDER::getCurrentCode() const
{
#ifdef _SCC_DEBUG
	if(Code[getIP()] >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (PREBUILDORDER::getCurrentCode): Variable Code not initialized.");return(0);
	}
#endif
	return(Code[getIP()]);
}

/*inline const unsigned int PREBUILDORDER::getMarker(const unsigned int ip) const
{
#ifdef _SCC_DEBUG
	if(ip > MAX_LENGTH) {
		toLog("DEBUG: (PREBUILDORDER::getMarker): Value ip out of range.");return(0);
	}
#endif
	return(Marker[ip]); 
}*/



// ------ UNITS ------
inline const unsigned int PREBUILDORDER::getMapLocationAvailible(const unsigned int player, const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(player >= (*getMap())->getMaxPlayer()) {
		toLog("DEBUG: (PREBUILDORDER::getMapLocationAvailible): Value player out of range.");return(0);
	}
	if((location_number >= (*getMap())->getMaxLocations()))	{
		toLog("DEBUG: (PREBUILDORDER::getMapLocationAvailible): Value location_number out of range.");return(0);
	}
#endif
	return((*unit)[player][location_number].getAvailible(unit_type));
}

inline const unsigned int PREBUILDORDER::getMapLocationTotal(const unsigned int player, const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(player >= (*getMap())->getMaxPlayer()) {
		toLog("DEBUG: (PREBUILDORDER::getMapLocationTotal): Value player out of range.");return(0);
	}
	if((location_number >= (*getMap())->getMaxLocations()))	{
		toLog("DEBUG: (PREBUILDORDER::getMapLocationTotal): Value location_number out of range.");return(0);
	}
#endif
	return((*unit)[player][location_number].getTotal(unit_type));
}

inline void PREBUILDORDER::setMapLocationAvailible(const unsigned int player, const unsigned int location_number, const unsigned int unit_type, const unsigned int availible)
{
#ifdef _SCC_DEBUG
	if(player >= (*getMap())->getMaxPlayer()) {
		toLog("DEBUG: (PREBUILDORDER::setMapLocationAvailible): Value player out of range.");return;
	}
	if((location_number >= (*getMap())->getMaxLocations()))	{
		toLog("DEBUG: (PREBUILDORDER::setMapLocationAvailible): Value location_number out of range.");return;
	}
#endif
	(*unit)[player][location_number].setAvailible(unit_type, availible);
}

inline void PREBUILDORDER::setMapLocationTotal(const unsigned int player, const unsigned int location_number, const unsigned int unit_type, const unsigned int total)
{
#ifdef _SCC_DEBUG
	if(player >= (*getMap())->getMaxPlayer()) {
		toLog("DEBUG: (PREBUILDORDER::setMapLocationTotal): Value player out of range.");return;
	}
	if((location_number >= (*getMap())->getMaxLocations()))	{
		toLog("DEBUG: (PREBUILDORDER::setMapLocationTotal): Value location_number out of range.");return;
	}
#endif
	(*unit)[player][location_number].setTotal(unit_type, total);
}

inline void PREBUILDORDER::addOneMapLocationAvailible(const unsigned int player, const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(player >= (*getMap())->getMaxPlayer()) {
		toLog("DEBUG: (PREBUILDORDER::addOneMapLocationAvailible): Value player out of range.");return;
	}
	if((location_number >= (*getMap())->getMaxLocations())) {
		toLog("DEBUG: (PREBUILDORDER::addOneMapLocationAvailible): Value location_number out of range.");return;
	}
#endif
	(*unit)[player][location_number].addOneAvailible(unit_type);
}

inline void PREBUILDORDER::addOneMapLocationTotal(const unsigned int player, const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(player >= (*getMap())->getMaxPlayer()) {
		toLog("DEBUG: (PREBUILDORDER::addOneMapLocationTotal): Value player out of range.");return;
	}
	if((location_number >= (*getMap())->getMaxLocations())) {
		toLog("DEBUG: (PREBUILDORDER::addOneMapLocationTotal): Value location_number out of range.");return;
	}
#endif
	(*unit)[player][location_number].addOneTotal(unit_type);
}

inline void PREBUILDORDER::removeOneMapLocationAvailible(const unsigned int player, const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(player >=(*getMap())->getMaxPlayer()) {
		toLog("DEBUG: (PREBUILDORDER::addOneMapLocationAvailible): Value player out of range.");return;
	}
	if((location_number >= (*getMap())->getMaxLocations())) {
		toLog("DEBUG: (PREBUILDORDER::addOneMapLocationAvailible): Value location_number out of range.");return;
	}
#endif
	(*unit)[player][location_number].removeOneAvailible(unit_type);
}


inline void PREBUILDORDER::removeOneMapLocationTotal(const unsigned int player, const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(player >=(*getMap())->getMaxPlayer()) {
		toLog("DEBUG: (PREBUILDORDER::addOneMapLocationTotal): Value player out of range.");return;
	}
	if((location_number >= (*getMap())->getMaxLocations())) {
		toLog("DEBUG: (PREBUILDORDER::addOneMapLocationTotal): Value location_number out of range.");return;
	}
#endif
	(*unit)[player][location_number].removeOneTotal(unit_type);
}

inline const unsigned int PREBUILDORDER::getLocationAvailible(const unsigned int location_number, const unsigned int unit_type) const
{
#ifdef _SCC_DEBUG
	if((location_number >= (*getMap())->getMaxLocations()))	{
		toLog("DEBUG: (PREBUILDORDER::getLocationAvailible): Value location_number out of range.");return(0);
	}
#endif
//	return((*location)[location_number].getAvailible(unit_type));
	return((*unit)[playerNum][location_number].getAvailible(unit_type));
}

inline void PREBUILDORDER::setLocationAvailible(const unsigned int location_number, const unsigned int unit_type, const unsigned int availible)
{
#ifdef _SCC_DEBUG
	if((location_number >= (*getMap())->getMaxLocations())) {
		toLog("DEBUG: (PREBUILDORDER::setLocationAvailible): Value location_number out of range.");return;
	}
#endif
//	(*location)[location_number].setAvailible(unit_type, availible);
	(*unit)[playerNum][location_number].setAvailible(unit_type, availible);
}

inline void PREBUILDORDER::setLocationTotal(const unsigned int location_number, const unsigned int unit_type, const unsigned int total)
{
#ifdef _SCC_DEBUG
	if(location_number >= (*getMap())->getMaxLocations())	{
		toLog("DEBUG: (PREBUILDORDER::setLocationTotal): Value location_number out of range.");return;
	}
#endif
//	(*location)[location_number].setTotal(unit_type, total);
	(*unit)[playerNum][location_number].setTotal(unit_type, total);
}

inline void PREBUILDORDER::addLocationAvailible(const unsigned int location_number, const unsigned int unit_type, const unsigned int availible)
{
#ifdef _SCC_DEBUG
	if(location_number >= (*getMap())->getMaxLocations()) {
		toLog("DEBUG: (PREBUILDORDER::addLocationAvailible): Value location_number out of range.");return;
	}
#endif	
	(*unit)[playerNum][location_number].addAvailible(unit_type, availible);
	if(location_number!=GLOBAL) //sonst waers ja doppelt...
		(*unit)[playerNum][GLOBAL].addAvailible(unit_type, availible);

//	(*location)[location_number].addAvailible(unit_type, availible);
//	if(location_number!=GLOBAL) //sonst waers ja doppelt...
//		(*location)[GLOBAL].addAvailible(unit_type, availible);
}

inline void PREBUILDORDER::addLocationTotal(const unsigned int location_number, const unsigned int unit_type, const unsigned int total)
{
#ifdef _SCC_DEBUG
	if(location_number >= (*getMap())->getMaxLocations())	{
		toLog("DEBUG: (PREBUILDORDER::addLocationTotal): Value location_number out of range.");return;
	}
#endif
	(*unit)[playerNum][location_number].addTotal( unit_type, total );
	if(location_number!=GLOBAL) // sonst waers ja doppelt wenn location = 0
		(*unit)[playerNum][GLOBAL].addTotal(unit_type, total);

//	(*location)[location_number].addTotal( unit_type, total );
//	if(location_number!=GLOBAL) // sonst waers ja doppelt wenn location = 0
//		(*location)[GLOBAL].addTotal(unit_type, total);
}

inline void PREBUILDORDER::addOneLocationAvailible(const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(location_number >= (*getMap())->getMaxLocations()) {
		toLog("DEBUG: (PREBUILDORDER::addLocationAvailible): Value location_number out of range.");return;
	}
#endif
	(*unit)[playerNum][location_number].addOneAvailible( unit_type );
// also add one unit to the global location if global location was not already specified
	if(location_number!=GLOBAL) 
		(*unit)[playerNum][GLOBAL].addOneAvailible( unit_type );

//	(*location)[location_number].addOneAvailible( unit_type );
// also add one unit to the global location if global location was not already specified
//	if(location_number!=GLOBAL) 
//		(*location)[GLOBAL].addOneAvailible( unit_type );
}

inline void PREBUILDORDER::addOneLocationTotal(const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if(location_number >= (*getMap())->getMaxLocations()) {
		toLog("DEBUG: (PREBUILDORDER::addLocationTotal): Value location_number out of range.");return;
	}
#endif
	(*unit)[playerNum][location_number].addOneTotal( unit_type );
	if(location_number!=GLOBAL) // sonst waers ja doppelt wenn location = 0
		(*unit)[playerNum][GLOBAL].addOneTotal( unit_type );

//	(*location)[location_number].addOneTotal( unit_type );
//	if(location_number!=GLOBAL) // sonst waers ja doppelt wenn location = 0
//		(*location)[GLOBAL].addOneTotal( unit_type );
}

inline void PREBUILDORDER::removeOneLocationAvailible(const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if((location_number >= (*getMap())->getMaxLocations())) {
		toLog("DEBUG: (PREBUILDORDER::removeOneLocationAvailible): Value location_number out of range.");return;
	}
#endif
	(*unit)[playerNum][location_number].removeOneAvailible( unit_type );
// also add one unit to the global location if global location was not already specified
	if(location_number!=GLOBAL)
		(*unit)[playerNum][GLOBAL].removeOneAvailible( unit_type );

//	(*location)[location_number].removeOneAvailible( unit_type );
// also add one unit to the global location if global location was not already specified
//	if(location_number!=GLOBAL)
//		(*location)[GLOBAL].removeOneAvailible( unit_type );
}

inline void PREBUILDORDER::removeOneLocationTotal(const unsigned int location_number, const unsigned int unit_type)
{
#ifdef _SCC_DEBUG
	if((location_number >= (*getMap())->getMaxLocations())) {
		toLog("DEBUG: (PREBUILDORDER::removeOneLocationTotal): Value location_number out of range.");return;
	}
#endif
	(*unit)[playerNum][location_number].removeOneTotal( unit_type );
	if(location_number!=GLOBAL) 
		(*unit)[playerNum][GLOBAL].removeOneTotal( unit_type );

//	(*location)[location_number].removeOneTotal( unit_type );
//	if(location_number!=GLOBAL) 
//		(*location)[GLOBAL].removeOneTotal( unit_type );
}
// ------ END UNITS -----


// ----- SUPPLY ------
inline void PREBUILDORDER::setNeedSupply(const unsigned int need_supply)
{
#ifdef _SCC_DEBUG
	if(need_supply > 10*MAX_SUPPLY) {
		toLog("DEBUG: (PREBUILDORDER::setNeedSupply): Value need_supply out of range.");return;
	}
#endif
	needSupply = need_supply;
}

inline const unsigned int PREBUILDORDER::getNeedSupply() const
{
#ifdef _SCC_DEBUG
	if(needSupply > 10*MAX_SUPPLY) {
		toLog("DEBUG: (PREBUILDORDER::getNeedSupply): Variable not initialized.");return(0);
	}
#endif
	return(needSupply);
}

inline void PREBUILDORDER::setHaveSupply(const unsigned int have_supply)
{
#ifdef _SCC_DEBUG
	if(have_supply > 10*MAX_SUPPLY) {
		toLog("DEBUG: (PREBUILDORDER::setHaveSupply): Value have_supply out of range.");return;
	}
#endif
	haveSupply = have_supply;
}

inline const unsigned int PREBUILDORDER::getHaveSupply() const
{
#ifdef _SCC_DEBUG
	if(haveSupply>10*MAX_SUPPLY) {
		toLog("DEBUG: (PREBUILDORDER::getHaveSupply): Variable not initialized.");return(0);
	}
#endif
	return(haveSupply);
}
// ----- END SUPPLY ------

// ----- HARVEST ------

inline void PREBUILDORDER::setMinerals(const unsigned int have_minerals)
{
#ifdef _SCC_DEBUG
	if(have_minerals > MAX_MINERALS) {
		toLog("DEBUG: (PREBUILDORDER::setMinerals): Value have_minerals out of range.");return;
	}
#endif
	minerals = have_minerals;
}

inline const unsigned int PREBUILDORDER::getMinerals() const
{
#ifdef _SCC_DEBUG
	if(minerals > MAX_MINERALS) {
		toLog("DEBUG: (PREBUILDORDER::getMinerals): Variable minerals not initialized.");return(0);
	}
#endif
	return(minerals);
}

inline void PREBUILDORDER::setGas(const unsigned int have_gas)
{
#ifdef _SCC_DEBUG
	if(have_gas > MAX_GAS) {
		toLog("DEBUG: (PREBUILDORDER::setGas): Value have_gas out of range.");return;
	}
#endif
	gas = have_gas;
}

inline const unsigned int PREBUILDORDER::getGas() const
{
#ifdef _SCC_DEBUG
	if(gas > MAX_GAS) {
		toLog("DEBUG: (PREBUILDORDER::getGas): Variable gas not initialized.");return(0);
	}
#endif
	return(gas);
}

inline void PREBUILDORDER::setMineralHarvestPerSecond( const unsigned int location_number, const unsigned int worker, const unsigned int mineral_harvest_per_second )
{
#ifdef _SCC_DEBUG
	if(mineral_harvest_per_second >= MAX_MINERALS) {
		toLog("DEBUG: (PREBUILDORDER::setMineralHarvestPerSecond): Value mineral_harvest_per_second out of range.");return;
	}
	if((location_number >= (*getMap())->getMaxLocations())) {
		toLog("DEBUG: (PREBUILDORDER::setMineralHarvestPerSecond): Value location_number out of range.");return;
	}
	if(worker>=45) {
		toLog("DEBUG: (PREBUILDORDER::setMineralHarvestPerSecond): Value worker out of range.");return;
	}
#endif
	mineralHarvestPerSecond[location_number][worker] = mineral_harvest_per_second;
}

inline const unsigned int PREBUILDORDER::getMineralHarvestPerSecond( const unsigned int location_number, const unsigned int worker ) const
{
#ifdef _SCC_DEBUG
	if((location_number >= (*getMap())->getMaxLocations()))	{
		toLog("DEBUG: (PREBUILDORDER::getMineralHarvestPerSecond): Value location_number out of range.");return(0);
	}
	if(worker>=45) {
		toLog("DEBUG: (PREBUILDORDER::getMineralHarvestPerSecond): Value worker out of range.");return(0);
	}
	if(mineralHarvestPerSecond[location_number][worker]>MAX_MINERALS) {
		toLog("DEBUG: (PREBUILDORDER::getMineralHarvestPerSecond): Variable mineralHarvestPerSecond not initialized.");return(0);
	}
#endif
	return(mineralHarvestPerSecond[location_number][worker]);
}

inline void PREBUILDORDER::setGasHarvestPerSecond( const unsigned int location_number, const unsigned int worker, const unsigned int gas_harvest_per_second )
{
#ifdef _SCC_DEBUG
	if(gas_harvest_per_second >= MAX_GAS) {
		toLog("DEBUG: (PREBUILDORDER::setGasHarvestPerSecond): Value gas_harvest_per_second out of range.");return;
	}
	if((location_number >= (*getMap())->getMaxLocations())) {
		toLog("DEBUG: (PREBUILDORDER::setGasHarvestPerSecond): Value location_number out of range.");return;
	}
	if(worker >= 5) {
		toLog("DEBUG: (PREBUILDORDER::setGasHarvestPerSecond): Value worker out of range.");return;
	}
#endif
	gasHarvestPerSecond[location_number][worker] = gas_harvest_per_second;
}

inline const unsigned int PREBUILDORDER::getGasHarvestPerSecond( const unsigned int location_number, const unsigned int worker ) const
{
#ifdef _SCC_DEBUG
	if((location_number >= (*getMap())->getMaxLocations()))	{
		toLog("DEBUG: (PREBUILDORDER::getGasHarvestPerSecond): Value location_number out of range.");return(0);
	}
	if(worker>=5)	{
		toLog("DEBUG: (PREBUILDORDER::getGasHarvestPerSecond): Value worker out of range.");return(0);
	}
	if(gasHarvestPerSecond[location_number][worker]>MAX_GAS) {
		toLog("DEBUG: (PREBUILDORDER::getGasHarvestPerSecond): Variable gasHarvestPerSecond not initialized.");return(0);
	}
#endif
	return(gasHarvestPerSecond[location_number][worker]);
}

inline void PREBUILDORDER::setHarvestedMinerals( const unsigned int harvested_minerals )
{
#ifdef _SCC_DEBUG
	if(harvested_minerals >= MAX_MINERALS) {
		toLog("DEBUG: (PREBUILDORDER::setHarvestedMinerals): Value out of range.");return;
	}
#endif
	harvestedMinerals = harvested_minerals;
}

inline const unsigned int PREBUILDORDER::getHarvestedMinerals() const
{
#ifdef _SCC_DEBUG
	if(harvestedMinerals>MAX_MINERALS) {
		toLog("DEBUG: (PREBUILDORDER::getHarvestedMinerals): Variable harvestedMinerals not initialized.");return(0);
	}
#endif
	return(harvestedMinerals);
}

inline void PREBUILDORDER::setHarvestedGas( const unsigned int harvested_gas )
{
#ifdef _SCC_DEBUG
	if(harvested_gas >= MAX_GAS) {
		toLog("DEBUG: (PREBUILDORDER::setHarvestedGas): Value out of range.");return;
	}
#endif
	harvestedGas = harvested_gas;
}

inline const unsigned int PREBUILDORDER::getHarvestedGas() const
{
#ifdef _SCC_DEBUG
	if(harvestedGas > MAX_GAS) {
		toLog("DEBUG: (PREBUILDORDER::getHarvestedGas): Variable harvestedGas not initialized.");return(0);
	}
#endif
	return(harvestedGas);
}


inline void PREBUILDORDER::setWastedMinerals( const unsigned int wasted_minerals )
{
#ifdef _SCC_DEBUG
	if(wasted_minerals >= MAX_MINERALS*MAX_TIME) {
		toLog("DEBUG: (PREBUILDORDER::setWastedMinerals): Value out of range.");return;
	}
#endif
	wastedMinerals = wasted_minerals;
}

inline const unsigned int PREBUILDORDER::getWastedMinerals() const
{
#ifdef _SCC_DEBUG
	if(wastedMinerals >= MAX_MINERALS*MAX_TIME) {
		toLog("DEBUG: (PREBUILDORDER::getWastedMinerals): Variable wastedMinerals not initialized.");return(0);
	}
#endif
	return(wastedMinerals);
}

inline void PREBUILDORDER::setWastedGas(const unsigned int wasted_gas)
{
#ifdef _SCC_DEBUG
	if(wasted_gas >= MAX_GAS*MAX_TIME) {
		toLog("DEBUG: (PREBUILDORDER::setWastedGas): Value out of range.");return;
	}
#endif
	wastedGas = wasted_gas;
}

inline const unsigned int PREBUILDORDER::getWastedGas() const
{
#ifdef _SCC_DEBUG
	if(wastedGas >= MAX_GAS*MAX_TIME) {
		toLog("DEBUG: (PREBUILDORDER::getWastedGas): Variable wastedGas not initialized.");return(0);
	}
#endif
	return(wastedGas);
}
// ----- END HARVEST -----

inline void PREBUILDORDER::setGoal(GOAL_ENTRY* current_goal)
{
#ifdef _SCC_DEBUG
	if(!current_goal) {
		toLog("DEBUG: (PREBUILDORDER::setGoal): Variable current_goal not initialized.");return;
	}
#endif
	pGoal = current_goal;
}

inline const unsigned int PREBUILDORDER::getPlayerNumber() const
{
#ifdef _SCC_DEBUG
	if(playerNum > MAX_PLAYER) {
		toLog("DEBUG: (PREBUILDORDER::getPlayerNumber): Variable not initialized.");return(0);
	}
#endif
	return(playerNum);
}

inline const START_CONDITION* const* PREBUILDORDER::getStartCondition()
{
	return(pStart->getStartCondition());
}


inline void PREBUILDORDER::setTimer(const unsigned int time)
{
#ifdef _SCC_DEBUG
	if(time > coreConfiguration.getMaxTime()) {
		toLog("DEBUG: (PREBUILDORDER::setTimer): Value time out of range.");return;
	}
#endif
	timer = time;
}

inline const unsigned int PREBUILDORDER::getTimer() const
{
#ifdef _SCC_DEBUG
	if(timer > coreConfiguration.getMaxTime()) {
		toLog("DEBUG: (PREBUILDORDER::getTimer): Variable timer not initialized.");return(0);
	}
#endif
	return(timer);
}


inline const unsigned int PREBUILDORDER::getRealTimer() const
{
#ifdef _SCC_DEBUG
	if(timer > coreConfiguration.getMaxTime()) {
		toLog("DEBUG: (PREBUILDORDER::getRealTimer): Variable timer not initialized.");return(0);
	}
#endif
	return(coreConfiguration.getMaxTime()-timer);
	// TODO auf > checken
}

inline void PREBUILDORDER::setpStats(const UNIT_STATISTICS* const* player_stats)
{
/*#ifdef _SCC_DEBUG
	if((*player_stats)[0].minerals!=0) { // TODO
		toLog("DEBUG: (PREBUILDORDER::setpStats): Variable not initialized.");return;
	}
#endif*/
	pStats = player_stats;
}

inline const UNIT_STATISTICS* const * PREBUILDORDER::getpStats() const
{
#ifdef _SCC_DEBUG
	if(!pStats)	{
		toLog("DEBUG: (PREBUILDORDER::getpStats): Variable not initialized.");return(0);
	}
#endif
	return(pStats);
}


/*const unsigned int PREBUILDORDER::getFinalTime(const unsigned int goal) const
{
#ifdef _SCC_DEBUG
	if((goal<0)||(goal>=MAX_GOALS))	{
		toLog("DEBUG: (PREBUILDORDER::getFinalTime): Value goal out of range.");return(0);
	}
	if((ftime[goal]<0)||(ftime[goal]>=MAX_TIME)) {
		toLog("DEBUG: (PREBUILDORDER::getFinalTime): Variable ftime not initialized.");return(0);
	}
#endif
	return(ftime[goal]);
}*/

inline const unsigned int PREBUILDORDER::getLength() const
{
#ifdef _SCC_DEBUG
	if(length>MAX_LENGTH) {
		toLog("DEBUG: (PREBUILDORDER::getLength): Variable length not initialized.");return(0);
	}
#endif
	return(length);
}

/*void PREBUILDORDER::setFinalTime(const unsigned int goal, const unsigned int time)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>=MAX_TIME)) {
		toLog("DEBUG: (PREBUILDORDER::setFinalTime): Value time out of range.");return;
	}
	if((goal<0)||(goal>=MAX_GOALS)) {
		toLog("DEBUG: (PREBUILDORDER::setFinalTime): Value goal out of range.");return;
	}
#endif
	ftime[goal]=time;
}*/

inline void PREBUILDORDER::setLength(const unsigned int bo_length)
{
#ifdef _SCC_DEBUG
	if(bo_length > MAX_LENGTH)
	{
		toLog("DEBUG: (PREBUILDORDER::setLength): Value bo_length out of range.");return;
	}
#endif
	length = bo_length;
}


inline void PREBUILDORDER::setTimeOut(const unsigned int time_out)
{
	if(timeout == time_out)
		return;		
#ifdef _SCC_DEBUG
	if(time_out > coreConfiguration.getMaxTimeOut()) {
		toLog("DEBUG: (PREBUILDORDER::setTimeOut): Value time_out out of range.");return;
	}
#endif
	timeout = time_out;
}

inline const unsigned int PREBUILDORDER::getTimeOut() const
{
#ifdef _SCC_DEBUG
	if(timeout > coreConfiguration.getMaxTimeOut()) {
		toLog("DEBUG: (PREBUILDORDER::getTimeOut): Variable timeout not initialized.");return(0);
	}
#endif
	return(timeout);
}


// --------------------------------------
// ------ END OF GET/SET FUNCTIONS ------
// --------------------------------------

inline void PREBUILDORDER::removeLarvaFromQueue(const unsigned int location_number)
{
#ifdef _SCC_DEBUG
	if((location_number<1) || (location_number >= (*getMap())->getMaxLocations())) {
		toLog("DEBUG: (PREBUILDORDER::removeLarvaFromQueue): Value location_number out of range.");return;
	}
	if((larvaInProduction[location_number]<1)||(larvaInProduction[location_number]>=MAX_SUPPLY)) {
		toLog("DEBUG: (PREBUILDORDER::removeLarvaFromQueue): Variable larvaInProduction not initialized or out of range.");return;
	}
#endif
	--larvaInProduction[location_number];
}

inline void PREBUILDORDER::addLarvaToQueue(const unsigned int location_number)
{
#ifdef _SCC_DEBUG
	if((location_number<1) || (location_number >= (*getMap())->getMaxLocations())) {
		toLog("DEBUG: (PREBUILDORDER::addLarvaFromQueue): Value location_number out of range.");return;
	}
	if(larvaInProduction[location_number]>=MAX_SUPPLY) {
		toLog("DEBUG: (PREBUILDORDER::addLarvaFromQueue): Variable larvaInProduction not initialized or out of range.");return;
	}
#endif
	++larvaInProduction[location_number];
}


inline const bool PREBUILDORDER::isAlwaysBuildWorkers() const {
	return(alwaysBuildWorkers);
}

inline const bool PREBUILDORDER::isOnlySwapOrders() const {
	return(onlySwapOrders);
}

#endif // __PREBUILDORDER_H

