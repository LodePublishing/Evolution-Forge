#ifndef _CORE_GOAL_HPP
#define _CORE_GOAL_HPP

#include "location.hpp"
#include "unit.hpp"
#include <list>
using namespace std;


class EXPORT GOAL_ENTRY
{
	private:
		int number; // to look up in the goal name list
		string name;
		eRace race;
		bool raceInitialized;
		int maxBuildTypes;
		const UNIT_STATISTICS* pStats;
//		bool initialized;
		int phaenoToGenotype[UNIT_TYPE_COUNT];
		int genoToPhaenotype[UNIT_TYPE_COUNT];
		bool changed;
//		int mode;

		int allGoal[UNIT_TYPE_COUNT];
		int globalGoal[MAX_LOCATIONS][UNIT_TYPE_COUNT];
		bool isBuildable[UNIT_TYPE_COUNT];
		bool isVariable[UNIT_TYPE_COUNT];	
		list<GOAL> goal;
	public:
//		const int getMode() const; // 0: normal, 1: based on success of enemy
//		void setMode(int mode);

//		GOAL goal[MAX_GOALS];

		const int countGoals() const;
		void calculateFinalTimes(const int location, const int unit, const int time);
		const bool calculateReady(const UNIT* units) const;
		const int getAllGoal(const int unit) const;
		const int getGlobalGoal(const int location, const int unit) const;
		const bool getIsBuildable(const int unit) const;
		
		void copy(const GOAL_ENTRY* goal, const UNIT* unit=0);
		void resetData();

		const bool getNextGoal(list<GOAL>::const_iterator& current, const bool first) const;
		void addGoal(const int unit, const int count, const int time, const int location);
		void adjustGoals(const bool allowGoalAdaption, const UNIT* unit=0);

		const bool isChanged() const;
		void changeAccepted();
		const string& getName() const;
		const UNIT_STATISTICS* getpStats() const;
		const int toGeno(const int phaeno) const;
		const int toPhaeno(const int geno) const;
		const eRace getRace() const;
		const int getMaxBuildTypes() const;
		const bool isRaceInitialized() const;
		void setName(const string& name);
		void setRace(const eRace race);
		const bool isGoal(const int unit) const;
		const bool getInitialized() const;

		GOAL_ENTRY();
		~GOAL_ENTRY();
};

#endif // _CORE_GOAL_HPP

