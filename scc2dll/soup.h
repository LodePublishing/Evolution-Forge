#ifndef __SOUP_H
#define __SOUP_H

#include "anarace.h"
#include "race.h"
#include "goal.h"
#include "map.h"
#include "ga.h"

// TODO: improve error checking, ranges etc. to recognize "stupid" settings, impossible goals etc.

class EXPORT SOUP
{
private:
	RACE* player[MAX_PROGRAMS];
	MAP* pMap;
	int run_number,goalsInitialized,playerInitialized,mapInitialized,goalCount,gaInitialized,newcalc;
	GOAL_ENTRY* goal[MAX_PLAYER];
	GA* ga;
	ANARACE* anaplayer[MAX_PLAYER];
	ANARACE* Save[MAX_RUNS][MAX_PLAYER];
public:
	void calculateAnaplayer();
	int setGoal(GOAL_ENTRY* goal, int player);
	int setMap(MAP* map);
	void checkForChange(); // manually update changes (when the engine is not optimizing for example)
// initializes the soup (primarily it does some error checking whether all is initialized and allocates the memory for the players
	int initSoup();
//one generation, copies stats and results of best individual and average values of the soup in 'best'
	ANARACE** newGeneration(ANARACE* oldAnarace[MAX_PLAYER]); 

	SOUP();
	~SOUP();

// internal function (called by settings)
	int setParameters(GA* ga);
};

#endif

