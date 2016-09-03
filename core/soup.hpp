#ifndef _CORE_SOUP_HPP
#define _CORE_SOUP_HPP

#include "start.hpp"
#include "race.hpp"
#include "anarace.hpp"

// TODO: improve error checking, ranges etc. to recognize "stupid" settings, impossible goals etc.

class SOUP
{
	private:
		RACE* player[MAX_PROGRAMS];
		ANARACE* anaplayer[MAX_PLAYER];
		START* start;
		unsigned int mapPlayerNum;
		unsigned int runNumber;//, goalCount;
//		bool playerInitialized; //?
		bool newcalc;
		bool isNewRun;
//		ANARACE* Save[MAX_RUNS][MAX_PLAYER]; // TODO LISTE DRAUS MACHEN!
	public:
		SOUP();
		~SOUP();
		SOUP(const SOUP& object);
		SOUP& operator=(const SOUP& object);

		void setMapPlayerNum(const unsigned int mapPlayerNum);
		void calculateAnaplayer() const;
		void checkForChange() const; // manually update changes (when the engine is not optimizing for example)
// 		TODO evtl ueber Rueckgabewert
// 		initializes the soup (primarily it does some error checking whether all is initialized and allocates the memory for the players
		void initSoup();
//one generation, copies stats and results of best individual and average values of the soup in 'best'
		ANARACE** newGeneration(ANARACE* oldAnarace[MAX_PLAYER]); 
		const bool getIsNewRun();

// internal function (called by settings)
		void setParameters(START* start);
};

#endif // _CORE_SOUP_HPP

