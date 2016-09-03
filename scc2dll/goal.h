#ifndef __GOAL_H
#define __GOAL_H

#include "location.h"
#include "unit.h"

class EXPORT GOAL_ENTRY
{
	private:
		int number; // to look up in the goal name list
		string name;
		eRace race;
		bool raceInitialized;
		int maxBuildTypes;
		const UNIT_STATISTICS* pStats;
		bool initialized;
		int phaenoToGenotype[UNIT_TYPE_COUNT];
		int genoToPhaenotype[UNIT_TYPE_COUNT];
		bool changed;
		int mode;
	public:
		const int getMode() const; // 0: normal, 1: based on success of enemy
		void setMode(int mode);
		
		void copy(const GOAL_ENTRY* goal);
		void resetData();
		const bool isChanged() const;
		void changeAccepted();

		int goalCount;
		int allGoal[UNIT_TYPE_COUNT];
		int globalGoal[MAX_LOCATIONS][UNIT_TYPE_COUNT];
		bool isBuildable[UNIT_TYPE_COUNT];
		bool isVariable[UNIT_TYPE_COUNT];

	//TODO: evtl linked list draus machen
		GOAL goal[MAX_GOALS];
		const string& getName() const;

		const UNIT_STATISTICS* getpStats() const;
		const int toGeno(const int phaeno) const;
		const int toPhaeno(const int geno) const;

		const eRace getRace() const;
		const int getMaxBuildTypes() const;
		const bool isRaceInitialized() const;
		void setName(const string& name);
		void setRace(const eRace race);
		void addGoal(const int unit, const int count, const int time, const int location);
		void adjustGoals(const bool allowGoalAdaption, const UNIT* unit=0);
		const bool isGoal(const int unit) const;
		const bool getInitialized() const;

		int bestTime; // cancel calculation if this calculation is worse than 25%
		GOAL_ENTRY();
		~GOAL_ENTRY();
};

#endif // __GOAL_H

