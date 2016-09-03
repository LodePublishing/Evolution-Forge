#ifndef _CORE_SOUP_HPP
#define _CORE_SOUP_HPP

#include "ga.hpp"
#include "start.hpp"
#include "race.hpp"
#include "anarace.hpp"

// TODO: improve error checking, ranges etc. to recognize "stupid" settings, impossible goals etc.

class SOUP
{
	private:
		RACE* player[MAX_PROGRAMS];
		START* start;
		unsigned int mapPlayerNum;
		unsigned int run_number;//, goalCount;
//		bool playerInitialized; //?
		bool newcalc;
		bool isNewRun;
//		bool gaInitialized;
		GA* ga;
		ANARACE* anaplayer[MAX_PLAYER];
		ANARACE* Save[MAX_RUNS][MAX_PLAYER]; // TODO LISTE DRAUS MACHEN!
	public:
//		SOUP(const SOUP& object);
//		SOUP& operator=(const SOUP& object);
	
		void setMapPlayerNum(const unsigned int mapPlayerNum);
		void calculateAnaplayer() const;
		void checkForChange() const; // manually update changes (when the engine is not optimizing for example)
// 		TODO evtl ueber Rueckgabewert
// 		initializes the soup (primarily it does some error checking whether all is initialized and allocates the memory for the players
		void initSoup();
//one generation, copies stats and results of best individual and average values of the soup in 'best'
		ANARACE** newGeneration(ANARACE* oldAnarace[MAX_PLAYER]); 
		const bool getIsNewRun();

		SOUP();
		~SOUP();

// internal function (called by settings)
		void setParameters(GA* ga, START* start);
};

#endif // _CORE_SOUP_HPP

