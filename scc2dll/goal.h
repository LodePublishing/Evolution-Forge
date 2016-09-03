#ifndef __GOAL_H
#define __GOAL_H

#include "location.h"

class EXPORT GOAL_ENTRY
{
private:
	int number; // to look up in the goal name list
//	char name[128];
	int race;
	int raceInitialized;
	int maxBuildTypes;
 	const UNIT_STATISTICS* pStats;
	int initialized;
	int phaenoToGenotype[UNIT_TYPE_COUNT];
	int genoToPhaenotype[UNIT_TYPE_COUNT];
	int changed;
public:
	void copy(GOAL_ENTRY* goal);
	void resetData();
	int isChanged();
	void changeAccepted();
	int goalCount;
	int allGoal[UNIT_TYPE_COUNT];
	int globalGoal[MAX_LOCATIONS][UNIT_TYPE_COUNT];
	int isBuildable[UNIT_TYPE_COUNT];
	int isVariable[UNIT_TYPE_COUNT];

//TODO: evtl linked list draus machen
	GOAL goal[MAX_GOALS];
//	const char* getName();

	const UNIT_STATISTICS* getpStats();
	int toGeno(int num);
	int toPhaeno(int num);

	int getRace();
	int getMaxBuildTypes();
	int isRaceInitialized();
//	int setName(const char* line);
	int setRace(int num);
	int addGoal(int unit, int count, int time, int location);
	int adjustGoals(int allowGoalAdaption, UNITS* units=0);
	int isGoal(int unit);
	int getInitialized();

	int bestTime; // cancel calculation if this calculation is worse than 25%
	GOAL_ENTRY();
	~GOAL_ENTRY();
};

#endif // __GOAL_H

