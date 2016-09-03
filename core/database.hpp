#ifndef _CORE_DATABASE_HPP
#define _CORE_DATABASE_HPP

#include "goalentry.hpp"	
#include "harvest.hpp"
#include "basicmap.hpp"
#include "anabuildorder.hpp"

#include <vector>
#include <string>

// generally all function return 0 if there was an error and 1 if there was no error

class DATABASE
{
	private:
		std::vector<GOAL_ENTRY*> loadedGoal[MAX_RACES];
		std::vector<START_CONDITION*> loadedStartCondition[MAX_RACES];
		std::vector<BASIC_MAP*> loadedMap; //modes: 0: ignore map settings and make up a default force, 1: use map settings
		std::vector<HARVEST_SPEED*> loadedHarvestSpeed[MAX_RACES];
	public:
		static std::list<std::string> findFiles(const std::string& directory1, const std::string& directory2, const std::string& directory3);
// ----- MAPS -----
		void loadMapFile(const std::string& mapFile);
		const unsigned int getMapCount() const;
		const BASIC_MAP* getMap(const unsigned int map_number) const;

// ----- GOALS ------
		void loadGoalFile(const std::string& goalFile);
		void saveGoal(const std::string& name, GOAL_ENTRY* goalentry);
		const unsigned int getGoalCount(const eRace race) const;
		const GOAL_ENTRY* getGoal(const eRace race, const unsigned int goal_number) const;


// ----- STARTCONDITION -----
		void loadStartConditionFile(const std::string& startConditionFile);
		const unsigned int getStartConditionCount(const eRace race) const;
		const START_CONDITION* getStartCondition(const eRace race, const unsigned int start_condition_number) const;

// ----- BUILD ORDERS -----		
		void saveBuildOrder(const std::string& name, const ANABUILDORDER* anarace) const;
		
// ----- HARVEST DATA -----
		void loadHarvestFile(const std::string& harvestFile);
		const HARVEST_SPEED* getHarvestSpeed(const eRace race, const unsigned int harvest_number) const;

		DATABASE();
		~DATABASE();
};

// -------------------------------
// ------ GET/SET FUNCTIONS ------
// -------------------------------

inline const BASIC_MAP* DATABASE::getMap(const unsigned int map_number) const
{
#ifdef _SCC_DEBUG
	if(map_number>loadedMap.size()) {
		toLog("WARNING: (DATABASE::getMap): Value out of range.");return(0);
	}
#endif
	return(loadedMap[map_number]);
}

inline const unsigned int DATABASE::getStartConditionCount(const eRace race) const {
	return(loadedStartCondition[race].size());
}

inline const unsigned int DATABASE::getGoalCount(const eRace race) const {
	return(loadedGoal[race].size());
}

inline const unsigned int DATABASE::getMapCount() const {
	return(loadedMap.size());
}

inline const GOAL_ENTRY* DATABASE::getGoal(const eRace race, const unsigned int goal) const
{
#ifdef _SCC_DEBUG
	if(goal>=loadedGoal[race].size()) {
		toLog("WARNING: (DATABASE::getGoal): Value out of range.");return(NULL);
	}
#endif
	return(loadedGoal[race][goal]);
}

inline const START_CONDITION* DATABASE::getStartCondition(const eRace race, const unsigned int start_condition_number) const
{
#ifdef _SCC_DEBUG
	if(start_condition_number >= loadedStartCondition[race].size()) {
		toLog("WARNING: (DATABASE::getStartCondition): Value out of range.");return(NULL);
	}
#endif
	return(loadedStartCondition[race][start_condition_number]);
}

inline const HARVEST_SPEED* DATABASE::getHarvestSpeed(const eRace race, const unsigned int harvest_number) const
{
#ifdef _SCC_DEBUG
	if(harvest_number >= loadedHarvestSpeed[race].size()) {
		toLog("WARNING: (DATABASE::getHarvestSpeed): Value out of range.");return(NULL);
	}
#endif
	return(loadedHarvestSpeed[race][harvest_number]);
}

// one global database
extern DATABASE database;

#endif

