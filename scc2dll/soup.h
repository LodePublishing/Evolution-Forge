#ifndef __SOUP_H
#define __SOUP_H

#include "anarace.h"
#include "race.h"
#include "goal.h"
#include "basicmap.h"
#include "ga.h"
#include "start.h"

// TODO: improve error checking, ranges etc. to recognize "stupid" settings, impossible goals etc.

class EXPORT SOUP
{
private:
	RACE* player[MAX_PROGRAMS];
	START* start;

	// --- cache ----
	const BASIC_MAP** pMap; //-> start->currentMap(), simplifies changes in start

	
	int run_number, goalCount;
	bool goalsInitialized; //?
	bool playerInitialized; //?
	bool newcalc;
	bool gaInitialized;
	GOAL_ENTRY* goal[MAX_PLAYER];
	GA* ga;
	ANARACE* anaplayer[MAX_PLAYER];
	ANARACE* Save[MAX_RUNS][MAX_PLAYER];
public:
//	void initializeMap(const BASIC_MAP* map); //~~~
	void calculateAnaplayer() const;
	void setGoal(GOAL_ENTRY* goal, const int player);
	//int setMap(MAP* map);
	void checkForChange() const; // manually update changes (when the engine is not optimizing for example)
	// TODO evtl ueber Rueckgabewert
// initializes the soup (primarily it does some error checking whether all is initialized and allocates the memory for the players
	void initSoup();
//one generation, copies stats and results of best individual and average values of the soup in 'best'
	ANARACE** newGeneration(ANARACE* oldAnarace[MAX_PLAYER]); 

	SOUP();
	~SOUP();

// internal function (called by settings)
	void setParameters(GA* ga, START* start);
};

#endif

