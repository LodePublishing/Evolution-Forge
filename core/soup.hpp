#ifndef _CORE_SOUP_HPP
#define _CORE_SOUP_HPP

#include "buildorder.hpp"
#include "anabuildorder.hpp"

// TODO: improve error checking, ranges etc. to recognize "stupid" settings, impossible goals etc.

class SOUP
{
	private:
		BUILDORDER* buildOrder[MAX_PROGRAMS];
		ANABUILDORDER* analyzedBuildOrder[MAX_PLAYER];
		unsigned int mapPlayerNum;
		unsigned int runNumber;
		bool newcalc;
		bool isNewRun;
		void calculateAnalyzedBuildOrder();
		void calculateBuildOrder(const unsigned int bo_num);
		UNIT temporaryForce[MAX_INTERNAL_PLAYER][MAX_LOCATIONS];
	public:
		SOUP();
		~SOUP();
		SOUP(const SOUP& object);
		SOUP& operator=(const SOUP& object);

		void setMapPlayerNum(const unsigned int map_player_num);
		void checkForChange() const; // manually update changes (when the engine is not optimizing for example)
// 		TODO evtl ueber Rueckgabewert
// 		initializes the soup (primarily it does some error checking whether all is initialized and allocates the memory for the players
		void initSoup(START* (*start)[MAX_INTERNAL_PLAYER]);
//one generation, copies stats and results of best individual and average values of the soup in 'best'
		const bool newGeneration(ANABUILDORDER* previous_build_order[MAX_PLAYER], const UNIT (*startForce)[MAX_INTERNAL_PLAYER][MAX_LOCATIONS]); 
		const bool getIsNewRun();

// internal function (called by settings)

		ANABUILDORDER* getAnalyzedBuildOrder(const unsigned int player_num);
};

#endif // _CORE_SOUP_HPP

