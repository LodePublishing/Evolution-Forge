#ifndef _CORE_SOUP_HPP
#define _CORE_SOUP_HPP

#include "buildorder.hpp"
#include "anabuildorder.hpp"

class SOUP
{
	public:
		SOUP();
		~SOUP();
	
//	initializes the soup (primarily it does some error checking whether all is initialized and allocates the memory for the players
		void assignMap(const BASIC_MAP* basic_map);
		void assignGoal(const unsigned int player_num, const unsigned int player_goal);
		void assignRace(const unsigned int player_num, const unsigned int race);
		
		void initSoup();
		void initSoup(unsigned int player_number);
//one generation, copies stats and results of best individual and average values of the soup in 'best'
		const bool newGeneration(ANABUILDORDER* previous_build_order[MAX_PLAYER]); 
		const bool recalculateGeneration(ANABUILDORDER* previous_analyzed_buildorder[MAX_PLAYER], const bool active[MAX_PLAYER]);

		void assignStartCondition(const unsigned int player_num, const START_CONDITION* start_condition);
		void setStartPosition(const unsigned int player_num, const unsigned int player_position);

// internal function (called by settings)
	private:
		START* start[MAX_PLAYER];
		const BASIC_MAP* map;
		bool mapInitialized;
		
		UNIT startForce[MAX_PLAYER][MAX_LOCATIONS];
		const unsigned int getMapPlayerCount() const;
		void setMapPlayerCount(const unsigned int map_player_count);

		BUILDORDER* buildOrder[MAX_PROGRAMS];
		ANABUILDORDER* analyzedBuildOrder[MAX_PLAYER];
		unsigned int mapPlayerCount;
		bool mapPlayerCountInitialized;
		
		UNIT temporaryForce[MAX_PLAYER][MAX_LOCATIONS];

		void calculateAnalyzedBuildOrder();
		void calculateBuildOrder(const unsigned int bo_num);

		SOUP(const SOUP& object);
		SOUP& operator=(const SOUP& object);
};

inline const unsigned int SOUP::getMapPlayerCount() const {
#ifdef _SCC_DEBUG
	if(!mapPlayerCountInitialized) {
		toErrorLog("DEBUG (SOUP::getMapPlayerCount()): Variable mapPlayerCount not initialized.");return(0);
	}
#endif
	return(mapPlayerCount);
}

#endif // _CORE_SOUP_HPP

