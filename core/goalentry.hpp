#ifndef _CORE_GOALENTRY_HPP
#define _CORE_GOALENTRY_HPP

#include "location.hpp"
#include "goal.hpp"

struct GOAL_TREE
{
	std::vector<bool> checked;
	std::vector<unsigned int> level;
	std::vector<unsigned int> coloumn;
	unsigned int width[10];
	std::vector< std::list<unsigned int> > connection;
	std::list<unsigned int> unit[10]; // 10?
};

class GOAL_ENTRY
{
	public:
		GOAL_ENTRY(const std::string& goal_name, const unsigned int goal_race);
		~GOAL_ENTRY();

		GOAL_ENTRY(const GOAL_ENTRY& object);
		GOAL_ENTRY& operator=(const GOAL_ENTRY& object);
		const bool operator==(const GOAL_ENTRY& other) const;

		void resetData();
	
		const unsigned int countGoals() const;

		void calculateFinalTimes(const unsigned int location, const unsigned int unit, const unsigned int count, const unsigned int time);

// same as above, but count has just be equal or greater than goal->count
		void calculateFinalTimesAtBeginning(const unsigned int location, const unsigned int unit, const unsigned int count, const unsigned int time);
		const unsigned int calculateFitness(const UNIT (&units)[MAX_LOCATIONS], std::vector<unsigned int> (&bonus)[MAX_LOCATIONS]) const;
		const bool calculateReady(const UNIT (&units)[MAX_LOCATIONS]) const;

		const unsigned int calculateMaxFitness() const;
	
		const unsigned int getGlobalGoal(const unsigned int location, const unsigned int unit) const;
		
		const bool getIsBuildable(const unsigned int unit) const;
		const bool getIsGoal(const unsigned int unit) const;
		const bool getIsHaveable(const unsigned int unit) const;

		const std::vector<bool>& getIsBuildableAtStart() const;
		const std::vector<bool>& getIsCheckedAtStart() const;
		const std::vector<unsigned int>& getGenoAtStart() const;
		const unsigned int getMaxBuildTypesAtStart() const;
		const std::vector<NEED>& getNeedsAtStart() const;
		
		const unsigned int getMaxBuildTypes() const;
			

		const std::list<GOAL> tryToAddGoal(const unsigned int unit, const unsigned int time, const unsigned int location, const signed int count);
		void addNewGoalToList(const unsigned int unit, const unsigned int time, const unsigned int location, const signed int count);
		const bool hasSimilarGoal(const unsigned int unit, const unsigned int location);
		void adjustGoals(const UNIT& unit);
		
		const unsigned int calculateFastestBO(const UNIT& startForce) const;
		
		const GOAL_TREE getGoalTree(const UNIT& startForce, const unsigned int currentGoalUnit) const;

		const bool isChanged() const;
		void changeAccepted();
		
		const UNIT_STATISTICS& getpStats(const unsigned int unit_number) const;
		
		const unsigned int toGeno(const unsigned int phaeno) const;
		const unsigned int toPhaeno(const unsigned int geno) const;
		
		const std::string& getName() const;
		void setName(const std::string& goal_name);
		const unsigned int getRace() const;
//		void setRace(const unsigned int goal_race);
//		const bool isRaceInitialized() const;
		
		const bool getIsInitialized() const;
		void calculateBonus(std::vector<unsigned int> (&bonus)[MAX_LOCATIONS]) const;
		std::list<GOAL> goalList; // private?

		void resetWasRemoved();
		const unsigned int getWasRemoved() const;
	private:
		void fillBuildableListAndAddGoals();
		void addSpecialUnitsToBuildableList();
		void recheckTooManyGoals();

		
		std::string name;
		unsigned int race;

		std::vector<UNIT_STATISTICS>& pStats;
		
		std::vector<unsigned int> phaenoToGenotype;
		std::vector<unsigned int> genoToPhaenotype;
		unsigned int maxBuildTypes;
		unsigned int maxBuildTypesAtStart;
	
		std::vector<bool> isHaveable;
		std::vector<bool> isBuildable; // TODO
		std::vector<bool> isBuildableAtStart;

		std::vector<bool> isCheckedAtStart;
		
		std::vector<unsigned int> genoAtStart;
		std::vector<NEED> needsAtStart;
		
		std::vector<bool> isGoal;

// um auch Zwischenziele (z.B. Hydralisks beim morphen oder untere Techstufen (z.B. Infantry Lvl1) zu belohnen!
		std::vector<unsigned int> bonusTable;

		void fillBonusTable();
		void giveBonusTo(const unsigned int unit, unsigned int caller, std::vector< std::vector<bool> >& checked);

		void setIsBuildable(const unsigned int unit, const bool is_buildable = true);
		void setIsBuildableAtStart(const unsigned int unit, const bool is_buildable = true);
		void setIsGoal(const unsigned int unit, const bool is_goal = true);
		void setIsHaveable(const unsigned int unit, const bool is_haveable = true);
		
		const bool isError(const unsigned int j, const unsigned int unit) const;
		
		bool changed;
//		bool raceInitialized;
		bool isInitialized;
		unsigned int wasRemoved;
};

inline const bool GOAL_ENTRY::getIsInitialized() const {
	return(isInitialized);
}

inline const bool GOAL_ENTRY::getIsHaveable(const unsigned int unit) const {
	return(isHaveable[unit]);
}

inline const std::vector<bool>& GOAL_ENTRY::getIsBuildableAtStart() const {
	return(isBuildableAtStart);
}

inline void GOAL_ENTRY::setIsHaveable(const unsigned int unit, const bool is_haveable) {
#ifdef _SCC_DEBUG
	if(unit >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG (GOAL_ENTRY::setIsHaveable()): Value unit out of range.");return;
	}
#endif
	isHaveable[unit] = is_haveable;
}

inline const std::vector<unsigned int>& GOAL_ENTRY::getGenoAtStart() const {
	return(genoAtStart);
}

inline const unsigned int GOAL_ENTRY::getMaxBuildTypesAtStart() const {
	return(maxBuildTypesAtStart);
}

inline const std::vector<NEED>& GOAL_ENTRY::getNeedsAtStart() const {
	return(needsAtStart);
}

inline const std::vector<bool>& GOAL_ENTRY::getIsCheckedAtStart() const {
	return(isCheckedAtStart);
}

inline const unsigned int GOAL_ENTRY::countGoals() const {
	return(goalList.size());
}



inline void GOAL_ENTRY::setIsBuildableAtStart(const unsigned int unit, const bool is_buildable) {
#ifdef _SCC_DEBUG
	if(unit >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG (GOAL_ENTRY::setIsBuildableAtStart()): Value unit out of range.");return;
	}
#endif
	isBuildableAtStart[unit] = is_buildable;
}

inline void GOAL_ENTRY::setIsGoal(const unsigned int unit, const bool is_goal) {
#ifdef _SCC_DEBUG
	if(unit >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG (GOAL_ENTRY::setIsGoal()): Value unit out of range.");return;
	}
#endif
	isGoal[unit] = is_goal;
}

inline const UNIT_STATISTICS& GOAL_ENTRY::getpStats(const unsigned int unit_number) const {
#ifdef _SCC_DEBUG
	if(unit_number >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG (GOAL_ENTRY::getpStats()): Value unit_number out of range.");return(pStats[0]);
	}
#endif
	return(pStats[unit_number]);
}

inline const bool GOAL_ENTRY::getIsGoal(const unsigned int unit) const {
#ifdef _SCC_DEBUG
	if(unit >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG (GOAL_ENTRY::getIsGoal()): Value unit out of range.");return(0);
	}
#endif
	return(isGoal[unit]);
}

inline const unsigned int GOAL_ENTRY::getMaxBuildTypes() const {
#ifdef _SCC_DEBUG
	if(maxBuildTypes > RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG (GOAL_ENTRY::getMaxBuildTypes()): Variable not initialized.");return(0);
	}
#endif
	return(maxBuildTypes);
}

inline const unsigned int GOAL_ENTRY::toGeno(const unsigned int phaeno) const {
#ifdef _SCC_DEBUG
	if(phaeno >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG (GOAL_ENTRY::toGeno()): Value out of range.");return(0);
	}
	if(phaenoToGenotype[phaeno] >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG (GOAL_ENTRY::toGeno()): Variable not initialized.");return(0);
	}
#endif
	return(phaenoToGenotype[phaeno]);
}

inline const unsigned int GOAL_ENTRY::toPhaeno(const unsigned int geno) const {
#ifdef _SCC_DEBUG
	if(geno >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG (GOAL_ENTRY::toPhaeno()): Value out of range.");return(0);
	}
	if(genoToPhaenotype[geno] >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG (GOAL_ENTRY::toPhaeno()): Variable not initialized.");return(0);
	}
#endif
	return(genoToPhaenotype[geno]);
}

inline const bool GOAL_ENTRY::isChanged() const {
	return(changed);
}

inline void GOAL_ENTRY::changeAccepted() {
//	ajdjustGoals(true); //PROBLEM: unitforce wird nicht mit einbezogen!
	changed=false;
}

inline const std::string& GOAL_ENTRY::getName() const {
	return name;
}

inline const unsigned int GOAL_ENTRY::getRace() const {
#ifdef _SCC_DEBUG
	if((race >= GAME::MAX_RACES)) {
		toErrorLog("DEBUG (GOAL_ENTRY::getRace()): race not initialized.");return(0);
	}
#endif
	return race;
}

inline void GOAL_ENTRY::setName(const std::string& goal_name) {
	name.assign(goal_name);
}

inline const bool GOAL_ENTRY::getIsBuildable(const unsigned int unit) const {
#ifdef _SCC_DEBUG
	if(unit >= RACE::UNIT_TYPE_COUNT) {
		toErrorLog("DEBUG (GOAL_ENTRY::getIsBuildable()): Value unit out of range.");return(false);
	}
#endif
	return(isBuildable[unit]);
}

#endif // _CORE_GOALENTRY_HPP

