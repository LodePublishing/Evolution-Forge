#ifndef _CORE_GOALENTRY_HPP
#define _CORE_GOALENTRY_HPP

#include "location.hpp"
#include "unit.hpp"
#include "goal.hpp"
#include <list>

struct GOAL_TREE
{
	bool checked[LAST_UNIT];
	unsigned int level[LAST_UNIT];
	unsigned int coloumn[LAST_UNIT];
	unsigned int width[10];
	std::list<unsigned int> con[LAST_UNIT];
	std::list<unsigned int> unit[10];
};

struct ALLOW
{
	std::list<unsigned int> facility;
	std::list<unsigned int> facility2;
	std::list<unsigned int> prerequisite;
};

struct NEED
{
	public:
		const bool facilityIsDone();
		const bool facility2IsDone();
		const bool prerequisiteIsDone();
		NEED& operator=(const NEED& object);
	
		bool facility;
		bool facility2;
		unsigned int prerequisite; // we need all... no double?
};

class GOAL_ENTRY
{
	public:
		GOAL_ENTRY();
		~GOAL_ENTRY();

		GOAL_ENTRY(const GOAL_ENTRY& object);
		GOAL_ENTRY& operator=(const GOAL_ENTRY& object);
		const bool operator==(const GOAL_ENTRY& other) const;
	
		std::list<GOAL> goalList; // private?
		NEED need[LAST_UNIT];
		ALLOW allow[LAST_UNIT];
		
 
//		const unsigned int getMode() const; // 0: normal, 1: based on success of enemy
//		void setMode(int mode);

		const unsigned int countGoals() const;
		void calculateFinalTimes(const unsigned int location, const unsigned int unit, const unsigned int count, const unsigned int time);

// same as above, but count has just be equal or greater than goal->count
		void calculateFinalTimesAtBeginning(const unsigned int location, const unsigned int unit, const unsigned int count, const unsigned int time);
		const unsigned int calculateFitness(const UNIT* units, unsigned int (&bonus)[MAX_LOCATIONS][LAST_UNIT]) const;
		const bool calculateReady(const UNIT* units) const;
//		const unsigned int getAllGoal(const unsigned int unit) const;
		const unsigned int getGlobalGoal(const unsigned int location, const unsigned int unit) const;
		const bool getIsBuildable(const unsigned int unit) const;
		const bool getIsHaveable(const unsigned int unit) const;
		const bool getIsStatic(const unsigned int unit) const;
		const bool getIsGoal(const unsigned int unit) const;

		void resetData();

//		const GOAL& getGoal(const int goal_number) const;
//		const bool getNextGoal(std::list<GOAL>::const_iterator& current, const bool first) const;

		
		const std::list<GOAL> tryToAddGoal(const unsigned int unit, const unsigned int time, const unsigned int location, const signed int count);
		void addNewGoalToList(const unsigned int unit, const unsigned int time, const unsigned int location, const signed int count);
		
		void adjustGoals(const bool allowGoalAdaption, const UNIT* unit=0);
		const unsigned int calculateFastestBO(const UNIT* startForce) const;
		const GOAL_TREE getGoalTree(const UNIT* startForce, const unsigned int currentGoalUnit) const;

		const bool isChanged() const;
		void changeAccepted();
		const std::string& getName() const;
		const UNIT_STATISTICS* getpStats() const;
		const unsigned int toGeno(const unsigned int phaeno) const;
		const unsigned int toPhaeno(const unsigned int geno) const;
		const eRace getRace() const;
		const unsigned int getMaxBuildTypes() const;
		const bool isRaceInitialized() const;
		void setName(const std::string& goal_name);
		void setRace(const eRace goal_race);
		const bool getInitialized() const;
	private:
		std::string name;
		eRace race;
		
		unsigned int maxBuildTypes;
		const UNIT_STATISTICS* pStats;
//		bool initialized;
		unsigned int phaenoToGenotype[LAST_UNIT];
		unsigned int genoToPhaenotype[LAST_UNIT];
//		int mode;

		const bool isError(const unsigned int j, const unsigned int unit) const;

		bool changed;
		bool raceInitialized;
		bool isBuildable[LAST_UNIT];
		bool isStatic[LAST_UNIT];	
		bool isHaveable[LAST_UNIT]; // all units that are goals, can be build or are build by the bo (larva etc.)
		bool isGoal[LAST_UNIT];

		void setIsBuildable(const unsigned int unit, const bool is_buildable = true);
		void setIsStatic(const unsigned int unit, const bool is_static = true);
		void setIsHaveable(const unsigned int unit, const bool is_haveable = true);
		void setIsGoal(const unsigned int unit, const bool is_goal = true);
};


inline void GOAL_ENTRY::setIsBuildable(const unsigned int unit, const bool is_buildable)
{
#ifdef _SCC_DEBUG
	if(unit>=LAST_UNIT) {
		toErrorLog("DEBUG: (GOAL_ENTRY::setIsBuildable): Value unit out of range.");return;
	}
#endif
	isBuildable[unit] = is_buildable;
}

inline void GOAL_ENTRY::setIsStatic(const unsigned int unit, const bool is_static)
{
#ifdef _SCC_DEBUG
	if(unit>=LAST_UNIT) {
		toErrorLog("DEBUG: (GOAL_ENTRY::setIsStatic): Value unit out of range.");return;
	}
#endif
	isStatic[unit] = is_static;
}

inline void GOAL_ENTRY::setIsHaveable(const unsigned int unit, const bool is_haveable)
{
#ifdef _SCC_DEBUG
	if(unit>=LAST_UNIT) {
		toErrorLog("DEBUG: (GOAL_ENTRY::setIsHaveable): Value unit out of range.");return;
	}
#endif
	isHaveable[unit] = is_haveable;
}

inline void GOAL_ENTRY::setIsGoal(const unsigned int unit, const bool is_goal)
{
#ifdef _SCC_DEBUG
	if(unit>=LAST_UNIT) {
		toErrorLog("DEBUG: (GOAL_ENTRY::setIsGoal): Value unit out of range.");return;
	}
#endif
	isGoal[unit] = is_goal;
}

inline const UNIT_STATISTICS* GOAL_ENTRY::getpStats() const
{
#ifdef _SCC_DEBUG
	if(pStats==NULL) {
		toErrorLog("DEBUG: (GOAL_ENTRY::getpStats): Variable pStats not initialized.");return(0);
	}
#endif
	return(pStats);
}

inline const bool GOAL_ENTRY::getIsGoal(const unsigned int unit) const
{
#ifdef _SCC_DEBUG
	if(unit>=LAST_UNIT) {
		toErrorLog("DEBUG: (GOAL_ENTRY::getIsGoal): Value unit out of range.");return(0);
	}
#endif
	return(isGoal[unit]);
}

inline const unsigned int GOAL_ENTRY::getMaxBuildTypes() const
{
#ifdef _SCC_DEBUG
	if(maxBuildTypes > LAST_UNIT) {
		toErrorLog("DEBUG: (GOAL_ENTRY::getMaxBuildTypes): Variable not initialized.");return(0);
	}
#endif
	return(maxBuildTypes);
}

/*const bool GOAL_ENTRY::getInitialized() const
{
	return(initialized);
}*/

inline const unsigned int GOAL_ENTRY::toGeno(const unsigned int phaeno) const
{
#ifdef _SCC_DEBUG
	if(phaeno>=LAST_UNIT) {
		toErrorLog("DEBUG: (GOAL_ENTRY::toGeno): Value out of range.");return(0);
	}
	if(phaenoToGenotype[phaeno]>=LAST_UNIT) {
		toErrorLog("DEBUG: (GOAL_ENTRY::toGeno): Variable not initialized.");return(0);
	}
#endif
	return(phaenoToGenotype[phaeno]);
}

inline const unsigned int GOAL_ENTRY::toPhaeno(const unsigned int geno) const
{
#ifdef _SCC_DEBUG
	if(geno>=LAST_UNIT) {
		toErrorLog("DEBUG: (GOAL_ENTRY::toPhaeno): Value out of range.");return(0);
	}
	if(genoToPhaenotype[geno]>=LAST_UNIT) {
		toErrorLog("DEBUG: (GOAL_ENTRY::toPhaeno): Variable not initialized.");return(0);
	}
#endif
	return(genoToPhaenotype[geno]);
}

inline const bool GOAL_ENTRY::isChanged() const
{
	return(changed);
}

inline void GOAL_ENTRY::changeAccepted()
{
//	ajdjustGoals(true); //PROBLEM: unitforce wird nicht mit einbezogen!
	changed=false;
}

inline const std::string& GOAL_ENTRY::getName() const
{
	return name;
}

inline const eRace GOAL_ENTRY::getRace() const
{
#ifdef _SCC_DEBUG
	if(!raceInitialized) {
		toErrorLog("DEBUG: (GOAL_ENTRY::getRace): race not initialized.");return(TERRA);
	}
#endif
	return race;
}

inline void GOAL_ENTRY::setName(const std::string& goal_name)
{
	name.assign(goal_name);
}

/*inline const unsigned int GOAL_ENTRY::getAllGoal(const unsigned int unit) const
{
#ifdef _SCC_DEBUG
	if(unit >= LAST_UNIT) {
		toErrorLog("DEBUG: (GOAL_ENTRY::getAllGoal): Value unit out of range.");return(0);
	}
	if(allGoal[unit]>200) {
		toErrorLog("DEBUG: (GOAL_ENTRY::getAllGoal): Variable allGoal out of range.");return(0);
	}	
#endif
	return(allGoal[unit]);		
}*/

inline const bool GOAL_ENTRY::getIsStatic(const unsigned int unit) const
{
#ifdef _SCC_DEBUG
	if(unit >= LAST_UNIT) {
		toErrorLog("DEBUG: (GOAL_ENTRY::getIsStatic): Value unit out of range.");return(false);
	}
#endif
	return(isStatic[unit]);
}
inline const bool GOAL_ENTRY::getIsHaveable(const unsigned int unit) const
{
#ifdef _SCC_DEBUG
	if(unit >= LAST_UNIT) {
		toErrorLog("DEBUG: (GOAL_ENTRY::getIsHaveable): Value unit out of range.");return(false);
	}
#endif
	return(isHaveable[unit]);
}

inline const bool GOAL_ENTRY::getIsBuildable(const unsigned int unit) const
{
#ifdef _SCC_DEBUG
	if(unit >= LAST_UNIT) {
		toErrorLog("DEBUG: (GOAL_ENTRY::getIsBuildable): Value unit out of range.");return(false);
	}
#endif
	return(isBuildable[unit]);
}


/*const unsigned int GOAL_ENTRY::getMode() const // TODO
{
	return(mode);
}

void GOAL_ENTRY::setMode(const unsigned int mode)
{
	this->mode=mode;
}*/
#endif // _CORE_GOALENTRY_HPP

