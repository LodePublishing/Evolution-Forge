#ifndef _CORE_GOAL_HPP
#define _CORE_GOAL_HPP

#include "location.hpp"
#include "unit.hpp"
#include <list>

class GOAL_ENTRY
{
	private:
		unsigned int number; // to look up in the goal name list //???? TODO
		std::string name;
		eRace race;
		unsigned int maxBuildTypes;
		const UNIT_STATISTICS* pStats;
//		bool initialized;
		unsigned int phaenoToGenotype[UNIT_TYPE_COUNT];
		unsigned int genoToPhaenotype[UNIT_TYPE_COUNT];
//		int mode;

		unsigned int allGoal[UNIT_TYPE_COUNT];
		unsigned int globalGoal[MAX_LOCATIONS][UNIT_TYPE_COUNT];
		bool changed;
		bool raceInitialized;
		bool isBuildable[UNIT_TYPE_COUNT];
		bool isVariable[UNIT_TYPE_COUNT];	
		std::list<GOAL> goal;
	public:
		GOAL_ENTRY();
		GOAL_ENTRY(const GOAL_ENTRY& object);
		~GOAL_ENTRY();
		GOAL_ENTRY& operator=(const GOAL_ENTRY& object);

//		const unsigned int getMode() const; // 0: normal, 1: based on success of enemy
//		void setMode(int mode);
//		GOAL goal[MAX_GOALS];

		const unsigned int countGoals() const;
		void calculateFinalTimes(const unsigned int location, const unsigned int unit, const unsigned int time);
		const bool calculateReady(const UNIT* units) const;
		const unsigned int getAllGoal(const unsigned int unit) const;
		const unsigned int getGlobalGoal(const unsigned int location, const unsigned int unit) const;
		const bool getIsBuildable(const unsigned int unit) const;

		void copy(const GOAL_ENTRY* goal_entry); // TODO!
		void resetData();

		const bool getNextGoal(std::list<GOAL>::const_iterator& current, const bool first) const;
		void addGoal(const unsigned int unit, const signed int count, const unsigned int time, const unsigned int location);
		void adjustGoals(const bool allowGoalAdaption, const UNIT* unit=0);

		const bool isChanged() const;
		void changeAccepted();
		const std::string& getName() const;
		const UNIT_STATISTICS* getpStats() const;
		const unsigned int toGeno(const unsigned int phaeno) const;
		const unsigned int toPhaeno(const unsigned int geno) const;
		const eRace getRace() const;
		const unsigned int getMaxBuildTypes() const;
		const bool isRaceInitialized() const;
		void setName(const std::string& name);
		void setRace(const eRace race);
		const bool isGoal(const unsigned int unit) const;
		const bool getInitialized() const;

};

#endif // _CORE_GOAL_HPP

