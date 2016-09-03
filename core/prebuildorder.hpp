#ifndef _CORE_PREBUILDORDER_HPP
#define _CORE_PREBUILDORDER_HPP

#include "building.hpp"
#include "configuration.hpp"
#include "start.hpp"
#include <functional>
#include <queue>

struct PARALLEL_COMMAND
{
	unsigned int unit;
	unsigned int count;
};


class PREBUILDORDER
{
	public:
// ------ INITIALIZATION ------
                void setStartPosition(const unsigned int start_position);
		void setStartRace(const unsigned int race); // => gleichzeitig wird harvestspeed geaendert und condition und goal muessen u.U. neugewaehlt werden!
                void assignStartCondition(const START_CONDITION* start_condition, const bool neutral_player = false);
                void assignStartGoal(const GOAL_ENTRY* goal);		
// assigns player data from start (start minerals, supply etc.) and sets the appropriate optimized pointers (global, location, pMap etc.) CALL IT AFTER EACH MAP CHANGE AND PLAYER CHANGE!!

// ------ CALCULATION ------
		virtual void prepareForNewGeneration();
		virtual void postProcessing();

		
// ------ STATISTICS ------
		const bool haveConditionsChanged() const;
		
		const bool setAlwaysBuildWorkers(const bool always_build_workers = true);
		const bool setOnlySwapOrders(const bool only_swap_orders = true);

		const bool isAlwaysBuildWorkers() const;
		const bool isOnlySwapOrders() const;

		const BASIC_MAP* const* getMap() const; 	
		GOAL_ENTRY* getGoal() const;

		const unsigned int getHarvestedResource(const unsigned int resource_type) const;
		const unsigned int getResource(const unsigned int resource_type) const;
	
		const unsigned int getPlayerNumber() const;
		const unsigned int getNeedSupply() const;
		const unsigned int getHaveSupply() const;
		void setTimer(const unsigned int time);
		const unsigned int getTimer() const;
		const unsigned int getRealTimer() const;
		const unsigned int getLength() const;
		const unsigned int getLocationTotal(const unsigned int location_number, const unsigned int unit_type) const;
		const unsigned int getLocationAvailible(const unsigned int location_number, const unsigned int unit_type) const;

		const unsigned int getRace() const;
		const START_CONDITION* const* getStartCondition() const; //pointer to player in start
		void setConditionsChanged(const bool conditions_changed = true);

		std::priority_queue<Building, std::vector<Building> > buildingQueue;

// ------ CODE CONTROL ------
		void eraseIllegalCode();
		void mutateGeneCode(/*const bool* fixed_list*/);
		void resetGeneCode();//resets either to a pre-processed buildorder or a completely random one*/
		void assignCode(const PREBUILDORDER& player);
		void assignCode(const std::list<unsigned int>& src);
		void copyCode(std::list<unsigned int>& dst) const;
		const std::list<unsigned int>& getCode() const;
		const bool isDifferent(const std::list<unsigned int>& code) const;
		
		START* getpStart() const;

	protected:
		PREBUILDORDER(unsigned int player_number, START* start, UNIT (&units)[MAX_PLAYER][MAX_LOCATIONS]);
		virtual ~PREBUILDORDER();

		const bool calculationDone();
		void resetNeededResources();
		const bool executeAlwaysBuildInstructions();
		const bool executeNextInstruction();
		const bool codeFinished() const;
		void processBuildingQueue();
		void resetInstructionPointer();
		void eraseRemainingCode();
		void resetTimeOut();
		void leapForwardInTime();
		const bool calculateReady() const;
		const unsigned int calculatePrimaryFitness(const bool ready);
	
		virtual void resetData();
		void resetSpecial();


// HACK for anarace:
		eErrorMessages successType;
		unsigned int successUnit;

		unsigned int anaraceBuildFacility;
		unsigned int anaraceBuildLocation;
		unsigned int anaraceBuildTotalTime;
		unsigned int anaraceBuildUnit;
		unsigned int anaraceBuildFinishedTime;

		const std::vector<UNIT_STATISTICS>* getpStats() const;
		UNIT& getUnit(const unsigned int player_number, const unsigned int location_number) const;
		bool ready;

	private:
	
		void modifyLocationAvailible(const unsigned int location_number, const unsigned int unit_type, const signed int availible);
		void modifyLocationTotal(const unsigned int location_number, const unsigned int unit_type, const signed int total);
		void addOneLocationAvailible(const unsigned int location_number, const unsigned int unit_type);
		void addOneLocationTotal(const unsigned int location_number, const unsigned int unit_type);
		void removeLocationAvailible(const unsigned int location_number, const unsigned int unit_type, const unsigned int availible);
		void removeLocationTotal(const unsigned int location_number, const unsigned int unit_type, const unsigned int total);

		void removeOneLocationAvailible(const unsigned int location_number, const unsigned int unit_type);
		void removeOneLocationTotal(const unsigned int location_number, const unsigned int unit_type);

		void removeOneLocationTotalAllPlayers(const unsigned int location_number, const unsigned int unit_type);
		void removeOneLocationAvailibleAllPlayers(const unsigned int location_number, const unsigned int unit_type);

		void removeOneTotalEverywhere(const unsigned int unit_type);
		void removeOneAvailibleEverywhere(const unsigned int unit_type);

	

//		void setResourceHarvestPerFrame(const unsigned int resource_type, const unsigned int location_number, const unsigned int worker, const unsigned int harvest_speed);
		const unsigned int getResourceHarvestPerFrame(const unsigned int resource_type, const unsigned int location_number, const unsigned int worker) const;
		void setHarvestedResource(const unsigned int resource_type, const unsigned int harvested_resource);

		const unsigned int harvestResource(const unsigned int resource_type) const;

//		void adjustHarvest(const unsigned int resource_type, const unsigned int location_number);
//		void adjustHarvestAllLocations(const unsigned int resource_type);

		void adjustAccepterDistance(const unsigned int resource_type, const unsigned int location_number);
		void adjustAccepterDistanceAllLocations(const unsigned int resource_type);
			
		std::list<unsigned int> Code;
		std::list<unsigned int>::iterator IP;
		UNIT (&unit)[MAX_PLAYER][MAX_LOCATIONS];
		
		void setTimeOut(const unsigned int time);
		const unsigned int getTimeOut() const;
		
		void moveToNextInstruction();
		const unsigned int getCurrentCode() const;
		const unsigned int calculateIdleTime() const; // calculate the next time something significant will happen, CONST MACHEN
		const bool buildGene(const unsigned int build_unit);
		const bool putInBuildingQueue(const unsigned int build_unit);
		
		void adjustLocationUnitsAfterCompletion(const unsigned int location_number, const eFacilityType facility_type, const unsigned int facility, const unsigned int facility2, const unsigned int count);
		void adjustAvailibility(const unsigned int location, const eFacilityType facility_type, const unsigned int facility, const unsigned int facility2, signed int count);
		void cancelBuilding(const Building& build);

		void createSpecial();
		const bool checkForLarva(const unsigned int current_location_window);
		const bool isTimeOut() const;
		void addLarvaToQueue(const unsigned int location_number);
		void removeLarvaFromQueue(const unsigned int location_number);

		void setNeedSupply(const unsigned int need_supply);
		void setHaveSupply(const unsigned int have_supply);

		void setResource(const unsigned int resource_type, const unsigned int have_resource);

		unsigned int larvaInProduction[MAX_LOCATIONS]; // well... one of that ugly race-specific variables saves a lot of trouble...
		
		START* pStart;

		bool conditionsChanged;
		bool alwaysBuildWorkers;
		bool onlySwapOrders;

		unsigned int playerNum;
		unsigned int timer;

		std::vector<unsigned int> resource;
		std::vector<unsigned int> neededResource;
		std::vector<unsigned int> accepterDistance[MAX_LOCATIONS];
//		std::vector< std::vector< unsigned int > > resourceHarvestPerFrame[MAX_LOCATIONS];
		std::vector<unsigned int> harvestedResource;
		
		unsigned int needSupply;		// free supply
		unsigned int haveSupply; // total supply
		unsigned int timeout;

//		bool playerNumInitialized;
//		bool unitsInitialized;
//		bool pStartInitialized;
	
		PREBUILDORDER& operator=(const PREBUILDORDER& object);
		PREBUILDORDER(const PREBUILDORDER& object);
};
		
inline START* PREBUILDORDER::getpStart() const {
	return(pStart);
}

inline const unsigned int PREBUILDORDER::getPlayerNumber() const {
	return(playerNum);
}

inline const bool PREBUILDORDER::haveConditionsChanged() const {
	return(conditionsChanged);
}

inline const unsigned int PREBUILDORDER::getRace() const {
	return(pStart->getPlayerRace());
}

inline const BASIC_MAP* const* PREBUILDORDER::getMap() const {
	return(pStart->getMap());
}

inline GOAL_ENTRY* PREBUILDORDER::getGoal() const 
{
/*#ifdef _SCC_DEBUG
	if(!goalInitialized) {
		toErrorLog("DEBUG (PREBUILDORDER::getGoal()()): Variable pGoal was not initialized.");
	}
#endif
	return(pGoal);*/
	return(pStart->getCurrentGoal());
}

inline void PREBUILDORDER::setStartPosition(const unsigned int start_position) 
{
	if(pStart->setStartPosition(start_position))
		setConditionsChanged();
}

inline void PREBUILDORDER::setStartRace(const unsigned int start_race) // => gleichzeitig wird harvestspeed geaendert und condition und goal muessen u.U. neugewaehlt werden!
{
	if(pStart->setPlayerRace(start_race))
		setConditionsChanged();
}

inline void PREBUILDORDER::assignStartGoal(const GOAL_ENTRY* start_goal) {
	if(pStart->assignGoal(start_goal))
		setConditionsChanged();
}


inline void PREBUILDORDER::moveToNextInstruction() {
	if(IP != Code.end())
		++IP;
}

inline const bool PREBUILDORDER::codeFinished() const {
	return(IP == Code.end());
}

inline void PREBUILDORDER::resetInstructionPointer() {
	IP = Code.begin();
}

inline const unsigned int PREBUILDORDER::getCurrentCode() const
{
#ifdef _SCC_DEBUG
	if(*IP >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG (PREBUILDORDER::getCurrentCode()): Variable Code not initialized.");return(0);
	}
#endif
	return(*IP);
}

// ------ UNITS ------

inline UNIT& PREBUILDORDER::getUnit(const unsigned int player_number, const unsigned int location_number) const {
#ifdef _SCC_DEBUG
	if(player_number >= (*getMap())->getMaxPlayer()) {
		toErrorLog("DEBUG (PREBUILDORDER::getUnit()()): Value player_number out of range.");
	}
	if((location_number >= (*getMap())->getMaxLocations())) {
		toErrorLog("DEBUG (PREBUILDORDER::getUnit()()): Value location_number out of range.");
	}
#endif
	return(unit[player_number][location_number]);
}

inline const unsigned int PREBUILDORDER::getLocationTotal(const unsigned int location_number, const unsigned int unit_type) const {
	return(getUnit(playerNum, location_number).getTotal(unit_type));
}

inline const unsigned int PREBUILDORDER::getLocationAvailible(const unsigned int location_number, const unsigned int unit_type) const {
	return(getUnit(playerNum, location_number).getAvailible(unit_type));
}

inline void PREBUILDORDER::modifyLocationAvailible(const unsigned int location_number, const unsigned int unit_type, const signed int availible)
{
	getUnit(getPlayerNumber(), location_number).modifyAvailible(unit_type, availible);
	if(location_number!=GLOBAL) //sonst waers ja doppelt...
		getUnit(getPlayerNumber(), GLOBAL).modifyAvailible(unit_type, availible);
}

inline void PREBUILDORDER::modifyLocationTotal(const unsigned int location_number, const unsigned int unit_type, const signed int total)
{
	getUnit(getPlayerNumber(), location_number).modifyTotal( unit_type, total );
	if(location_number!=GLOBAL) // sonst waers ja doppelt wenn location = 0
		getUnit(getPlayerNumber(), GLOBAL).modifyTotal(unit_type, total);
}

inline void PREBUILDORDER::addOneLocationAvailible(const unsigned int location_number, const unsigned int unit_type)
{
	getUnit(getPlayerNumber(), location_number).addOneAvailible( unit_type );
// also add one unit to the global location if global location was not already specified
	if(location_number!=GLOBAL) 
		getUnit(getPlayerNumber(), GLOBAL).addOneAvailible( unit_type );
}

inline void PREBUILDORDER::addOneLocationTotal(const unsigned int location_number, const unsigned int unit_type)
{
	getUnit(getPlayerNumber(), location_number).addOneTotal( unit_type );
	if(location_number!=GLOBAL) // sonst waers ja doppelt wenn location = 0
		getUnit(getPlayerNumber(), GLOBAL).addOneTotal( unit_type );
}

inline void PREBUILDORDER::removeOneLocationAvailible(const unsigned int location_number, const unsigned int unit_type)
{
	getUnit(getPlayerNumber(), location_number).removeOneAvailible( unit_type );
// also add one unit to the global location if global location was not already specified
	if(location_number!=GLOBAL)
		getUnit(getPlayerNumber(), GLOBAL).removeOneAvailible( unit_type );
}

inline void PREBUILDORDER::removeOneLocationTotal(const unsigned int location_number, const unsigned int unit_type)
{
	getUnit(getPlayerNumber(), location_number).removeOneTotal( unit_type );
	if(location_number!=GLOBAL) 
		getUnit(getPlayerNumber(), GLOBAL).removeOneTotal( unit_type );
}

inline void PREBUILDORDER::removeOneLocationTotalAllPlayers(const unsigned int location_number, const unsigned int unit_type)
{
	for(unsigned int i = (*getMap())->getMaxPlayer(); i--;)
		getUnit(i, location_number).removeOneTotal( unit_type );
}

inline void PREBUILDORDER::removeOneLocationAvailibleAllPlayers(const unsigned int location_number, const unsigned int unit_type)
{
	for(unsigned int i = (*getMap())->getMaxPlayer(); i--;)
		getUnit(i, location_number).removeOneAvailible( unit_type );
}

inline void PREBUILDORDER::removeOneTotalEverywhere(const unsigned int unit_type) // for researchs
{
	for(unsigned int i = (*getMap())->getMaxLocations(); i--;)
		getUnit(getPlayerNumber(), i).removeOneTotal( unit_type );
}

inline void PREBUILDORDER::removeOneAvailibleEverywhere(const unsigned int unit_type) // for researchs
{
	for(unsigned int i = (*getMap())->getMaxLocations(); i--;)
		getUnit(getPlayerNumber(), i).removeOneAvailible( unit_type );
}

// ------ END UNITS -----


// ----- SUPPLY ------
inline void PREBUILDORDER::setNeedSupply(const unsigned int need_supply) {
#ifdef _SCC_DEBUG
	if(need_supply > 2*GAME::MAX_SUPPLY) {
		toErrorLog("DEBUG (PREBUILDORDER::setNeedSupply()): Value need_supply out of range.");return;
	}
#endif
	needSupply = need_supply;
}

inline const unsigned int PREBUILDORDER::getNeedSupply() const {
#ifdef _SCC_DEBUG
	if(needSupply > 2*GAME::MAX_SUPPLY) {
		toErrorLog("DEBUG (PREBUILDORDER::getNeedSupply()): Variable not initialized.");return(0);
	}
#endif
	return(needSupply);
}

inline void PREBUILDORDER::setHaveSupply(const unsigned int have_supply) {
#ifdef _SCC_DEBUG
	if(have_supply > 2*GAME::MAX_SUPPLY) {
		toErrorLog("DEBUG (PREBUILDORDER::setHaveSupply()): Value have_supply out of range.");return;
	}
#endif
	haveSupply = have_supply;
}

inline const unsigned int PREBUILDORDER::getHaveSupply() const {
#ifdef _SCC_DEBUG
	if(haveSupply > 2*GAME::MAX_SUPPLY) {
		toErrorLog("DEBUG (PREBUILDORDER::getHaveSupply()): Variable not initialized.");return(0);
	}
#endif
	return(haveSupply);
}
// ----- END SUPPLY ------

// ----- HARVEST ------

inline void PREBUILDORDER::setResource(const unsigned int resource_type, const unsigned int have_resource) {
#ifdef _SCC_DEBUG
	if(resource_type >= RACE::MAX_RESOURCE_TYPES) {
		toErrorLog("DEBUG (PREBUILDORDER::setResource()): Value resource_type out of range.");return;
	}
	if(have_resource > GAME::MAX_RESOURCES) {
		toErrorLog("DEBUG (PREBUILDORDER::setResource()): Value have_resource out of range.");return;
	}
#endif
	resource[resource_type] = have_resource;
}

inline const unsigned int PREBUILDORDER::getResource(const unsigned int resource_type) const {
#ifdef _SCC_DEBUG
	if(resource_type >= RACE::MAX_RESOURCE_TYPES) {
		toErrorLog("DEBUG (PREBUILDORDER::getResource()): Value resource_type out of range.");return(0);
	}
	if(resource[resource_type] > GAME::MAX_RESOURCES) {
		toErrorLog("DEBUG (PREBUILDORDER::getResource()): Variable resource not initialized.");return(0);
	}
#endif
	return(resource[resource_type]);
}
/*
inline void PREBUILDORDER::setResourceHarvestPerFrame( const unsigned int resource_type, const unsigned int location_number, const unsigned int worker, const unsigned int resource_harvest_per_frame ) {
#ifdef _SCC_DEBUG
	if(resource_type >= RACE::MAX_RESOURCE_TYPES) {
		toErrorLog("DEBUG (PREBUILDORDER::setResourceHarvestPerFrame()): Value resource_type out of range.");return;
	}
	if(resource_harvest_per_frame >= GAME::MAX_RESOURCES) {
		toErrorLog("DEBUG (PREBUILDORDER::setResourceHarvestPerFrame()): Value resource_harvest_per_frame out of range.");return;
	}
	if((location_number >= (*getMap())->getMaxLocations())) {
		toErrorLog("DEBUG (PREBUILDORDER::setResourceHarvestPerFrame()): Value location_number out of range.");return;
	}
	if(worker>=45) { // TODO
		toErrorLog("DEBUG (PREBUILDORDER::setResourceHarvestPerFrame()): Value worker out of range.");return;
	}
#endif
	resourceHarvestPerFrame[location_number][resource_type][worker] = resource_harvest_per_frame;
}*/



inline void PREBUILDORDER::setHarvestedResource( const unsigned int resource_type, const unsigned int harvested_resource ) {
#ifdef _SCC_DEBUG
	if(resource_type >= RACE::MAX_RESOURCE_TYPES) {
		toErrorLog("DEBUG (PREBUILDORDER::setHarvestedResource()): Value resource_type out of range.");return;
	}
	if(harvested_resource >= GAME::MAX_RESOURCES) {
		toErrorLog("DEBUG (PREBUILDORDER::setHarvestedResource()): Value harvest_resource, out of range.");return;
	}
#endif
	harvestedResource[resource_type] = harvested_resource;
}

inline const unsigned int PREBUILDORDER::getHarvestedResource( const unsigned int resource_type ) const {
#ifdef _SCC_DEBUG
	if(resource_type >= RACE::MAX_RESOURCE_TYPES) {
		toErrorLog("DEBUG (PREBUILDORDER::getHarvestedResource()): Value resource_type out of range.");return(0);
	}
	if(harvestedResource[resource_type] > GAME::MAX_RESOURCES) {
		toErrorLog("DEBUG (PREBUILDORDER::getHarvestedResource()): Variable harvestedResource not initialized.");return(0);
	}
#endif
	return(harvestedResource[resource_type]);
}

// ----- END HARVEST -----


inline const START_CONDITION* const* PREBUILDORDER::getStartCondition() const {
	return(pStart->getStartCondition());
}


inline void PREBUILDORDER::setTimer(const unsigned int time) {
#ifdef _SCC_DEBUG
	if(time > coreConfiguration.getMaxTime()) {
		toErrorLog("DEBUG (PREBUILDORDER::setTimer()): Value time out of range.");return;
	}
#endif
	timer = time;
}

inline const unsigned int PREBUILDORDER::getTimer() const {
#ifdef _SCC_DEBUG
	if(timer > coreConfiguration.getMaxTime()) {
		toErrorLog("DEBUG (PREBUILDORDER::getTimer()): Variable timer not initialized.");return(0);
	}
#endif
	return(timer);
}

inline const unsigned int PREBUILDORDER::getRealTimer() const {
#ifdef _SCC_DEBUG
	if(timer > coreConfiguration.getMaxTime()) {
		toErrorLog("DEBUG (PREBUILDORDER::getRealTimer()): Variable timer not initialized.");return(0);
	}
#endif
	return(coreConfiguration.getMaxTime()-timer);
	// TODO auf > checken
}

inline const std::vector<UNIT_STATISTICS>* PREBUILDORDER::getpStats() const {
/*#ifdef _SCC_DEBUG
	if(!pStatsInitialized)	{
		toErrorLog("DEBUG (PREBUILDORDER::getpStats()()): Variable pStats not initialized.");return(0);
	}
#endif
	return(pStats);*/
	return(pStart->getpStats());
}

inline const unsigned int PREBUILDORDER::getLength() const
{
#ifdef _SCC_DEBUG
	if(Code.size() >= MAX_LENGTH) {
		toErrorLog("DEBUG (PREBUILDORDER::getLength()): Variable Code too long.");return(0);
	}
#endif
	return(Code.size());
}

inline void PREBUILDORDER::resetTimeOut() {
	timeout = coreConfiguration.getMaxTimeOut();
}

inline void PREBUILDORDER::setTimeOut(const unsigned int time_out) {
	if(timeout == time_out)
		return;		
#ifdef _SCC_DEBUG
	if(time_out > coreConfiguration.getMaxTimeOut()) {
		toErrorLog("DEBUG (PREBUILDORDER::setTimeOut()): Value time_out out of range.");return;
	}
#endif
	timeout = time_out;
}

inline const bool PREBUILDORDER::isTimeOut() const {
#ifdef _SCC_DEBUG
	if(timeout > coreConfiguration.getMaxTimeOut()) {
		toErrorLog("DEBUG (PREBUILDORDER::isTimeOut()): Variable timeout not initialized.");return(0);
	}
#endif
	return(timeout==0);
}

inline const unsigned int PREBUILDORDER::getTimeOut() const {
#ifdef _SCC_DEBUG
	if(timeout > coreConfiguration.getMaxTimeOut()) {
		toErrorLog("DEBUG (PREBUILDORDER::getTimeOut()): Variable timeout not initialized.");return(0);
	}
#endif
	return(timeout);
}


// --------------------------------------
// ------ END OF GET/SET FUNCTIONS ------
// --------------------------------------

inline void PREBUILDORDER::removeLarvaFromQueue(const unsigned int location_number) {
#ifdef _SCC_DEBUG
	if((location_number<1) || (location_number >= (*getMap())->getMaxLocations())) {
		toErrorLog("DEBUG (PREBUILDORDER::removeLarvaFromQueue()): Value location_number out of range.");return;
	}
	if((larvaInProduction[location_number]<1)||(larvaInProduction[location_number] >= GAME::MAX_SUPPLY)) {
		toErrorLog("DEBUG (PREBUILDORDER::removeLarvaFromQueue()): Variable larvaInProduction not initialized or out of range.");return;
	}
#endif
	--larvaInProduction[location_number];
}

inline void PREBUILDORDER::addLarvaToQueue(const unsigned int location_number) {
#ifdef _SCC_DEBUG
	if((location_number<1) || (location_number >= (*getMap())->getMaxLocations())) {
		toErrorLog("DEBUG (PREBUILDORDER::addLarvaFromQueue()): Value location_number out of range.");return;
	}
	if(larvaInProduction[location_number] >= GAME::MAX_SUPPLY) {
		toErrorLog("DEBUG (PREBUILDORDER::addLarvaFromQueue()): Variable larvaInProduction not initialized or out of range.");return;
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

inline void PREBUILDORDER::assignCode(const PREBUILDORDER& player) {
	Code = player.Code;
}

inline void PREBUILDORDER::assignCode(const std::list<unsigned int>& src) {
	Code = src;
}

inline void PREBUILDORDER::copyCode(std::list<unsigned int>& dst) const {
	dst = Code;
}

inline const bool PREBUILDORDER::isDifferent(const std::list<unsigned int>& code) const {
	return(Code != code);
}


#endif // __PREBUILDORDER_H



