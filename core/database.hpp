#ifndef _CORE_DATABASE_HPP
#define _CORE_DATABASE_HPP

#include "boentry.hpp"	
//#include "harvest.hpp"
#include "basicmap.hpp"
#include "anabuildorder.hpp"

#include <vector>
#include <string>

// generally all function return 0 if there was an error and 1 if there was no error

class DATABASE
{
	public:
		DATABASE();
		~DATABASE();

		void init(); // call after game change
// ----- MAPS -----
		void addMap(BASIC_MAP* basic_map);
		const unsigned int getMapCount() const;
		const BASIC_MAP* getMap(const unsigned int map_number) const;
		const bool isMapDataInitialized() const;

// ----- GOALS ------
		void addGoal(GOAL_ENTRY* my_goal);
		const unsigned int getGoalCount(const unsigned int race) const;
		const GOAL_ENTRY* getGoal(const unsigned int race, const unsigned int goal_number) const;
		void addDefaultGoal(const unsigned int race);
		const bool isGoalDataInitialized() const;
		
// ----- BUILD ORDERS ------
		void addBuildOrder(BUILD_ORDER* build_order);
		const unsigned int getBuildOrderCount(const unsigned int race, const GOAL_ENTRY* goal) const;
		BUILD_ORDER* getBuildOrder(const unsigned int race, const GOAL_ENTRY* goal, const unsigned int bo_number) const;
		const bool isBuildOrderDataInitialized() const;

// ----- STARTCONDITION -----
		void addStartCondition(START_CONDITION* start_condition);
		const unsigned int getStartConditionCount(const unsigned int race) const;
		const START_CONDITION* getStartCondition(const unsigned int race, const unsigned int start_condition_number) const;
		const bool isStartConditionDataInitialized() const;

// ----- HARVEST DATA -----
//		void addHarvestSpeed(const unsigned int race, HARVEST_SPEED* harvest_speed);
//		const HARVEST_SPEED* getHarvestSpeed(const unsigned int race, const unsigned int harvest_number) const;
//		const bool isHarvestDataInitialized() const;

		const bool wereGoalsChanged() const;
		const bool wereBosChanged() const;
		const bool wereMapsChanged() const;
		const bool wereStartConditionsChanged() const;
		void changeAccepted();
	private:
		std::vector< std::vector<GOAL_ENTRY*> > loadedGoal;
		std::vector< std::vector<BUILD_ORDER*> > loadedBuildOrder;
		std::vector< std::vector<START_CONDITION*> > loadedStartCondition;
		std::vector< std::vector<HARVEST_SPEED*> > loadedHarvestSpeed;
		std::vector<BASIC_MAP*> loadedMap; //modes: 0: ignore map settings and make up a default force, 1: use map settings

		bool goalDataInitialized;
		bool buildOrderDataInitialized;
		bool startConditionDataInitialized;
		bool mapDataInitialized;
		bool harvestDataInitialized;	

		bool goalsWereChanged;
		bool bosWereChanged;
		bool mapsWereChanged;
		bool startConditionsWereChanged;
};

inline const bool DATABASE::wereGoalsChanged() const {
	return (goalsWereChanged);
}
inline const bool DATABASE::wereBosChanged() const {
	return (bosWereChanged);
}
inline const bool DATABASE::wereMapsChanged() const {
	return (mapsWereChanged);
}
inline const bool DATABASE::wereStartConditionsChanged() const {
	return (startConditionsWereChanged);
}


// -------------------------------
// ------ GET/SET FUNCTIONS ------
// -------------------------------

inline const BASIC_MAP* DATABASE::getMap(const unsigned int map_number) const
{
#ifdef _SCC_DEBUG
	if(map_number>loadedMap.size()) {
		toErrorLog("WARNING: (DATABASE::getMap): Value out of range.");return(0);
	}
#endif
	return(loadedMap[map_number]);
}

inline const unsigned int DATABASE::getStartConditionCount(const unsigned int race) const {
#ifdef _SCC_DEBUG
	if(race >= GAME::MAX_RACES) {
		toErrorLog("DEBUG (DATABASE::getStartConditionCount()): Value race out of range.");return(0);
	}
#endif
	return(loadedStartCondition[race].size());
}

inline const unsigned int DATABASE::getGoalCount(const unsigned int race) const {
#ifdef _SCC_DEBUG
	if(race >= GAME::MAX_RACES) {
		toErrorLog("DEBUG (DATABASE::getGoalCount()): Value race out of range.");return(0);
	}
#endif
	return(loadedGoal[race].size());
}


inline const unsigned int DATABASE::getMapCount() const {
	return(loadedMap.size());
}

inline const GOAL_ENTRY* DATABASE::getGoal(const unsigned int race, const unsigned int goal) const
{
#ifdef _SCC_DEBUG
	if(race >= GAME::MAX_RACES) {
		toErrorLog("DEBUG (DATABASE::getGoal()): Value race out of range.");return(NULL);
	}
	if(goal>=loadedGoal[race].size()) {
		toErrorLog("WARNING: (DATABASE::getGoal()): Value goal out of range.");return(NULL);
	}
#endif
	return(loadedGoal[race][goal]);
}

inline const START_CONDITION* DATABASE::getStartCondition(const unsigned int race, const unsigned int start_condition_number) const
{
#ifdef _SCC_DEBUG
	if(race >= GAME::MAX_RACES) {
		toErrorLog("DEBUG (DATABASE::getStartCondition()): Value race out of range.");return(NULL);
	}
	if(start_condition_number >= loadedStartCondition[race].size()) {
		toErrorLog("WARNING: (DATABASE::getStartCondition()): Value start_condition_number out of range.");return(NULL);
	}
#endif
	return(loadedStartCondition[race][start_condition_number]);
}
/*
inline const HARVEST_SPEED* DATABASE::getHarvestSpeed(const unsigned int race, const unsigned int harvest_number) const
{
#ifdef _SCC_DEBUG
	if(race >= GAME::MAX_RACES) {
		toErrorLog("DEBUG (DATABASE::getHarvestSpeed()): Value race out of range.");return(NULL);
	}
	if(harvest_number >= loadedHarvestSpeed[race].size()) {
		toErrorLog("WARNING: (DATABASE::getHarvestSpeed): Value harvest_number out of range.");return(NULL);
	}
#endif
	return(loadedHarvestSpeed[race][harvest_number]);
}

inline const bool DATABASE::isHarvestDataInitialized() const {
	return(harvestDataInitialized);
}*/

inline const bool DATABASE::isGoalDataInitialized() const {
	return(goalDataInitialized);
}
inline const bool DATABASE::isMapDataInitialized() const {
	return(mapDataInitialized);
}
inline const bool DATABASE::isBuildOrderDataInitialized() const {
	return(buildOrderDataInitialized);
}
inline const bool DATABASE::isStartConditionDataInitialized() const {
	return(startConditionDataInitialized);
}

// one global database
extern DATABASE database;

#endif

