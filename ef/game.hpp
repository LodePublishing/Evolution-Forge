#ifndef _GUI_GAME_HPP
#define _GUI_GAME_HPP

#include "player.hpp"
#include "../ui/object.hpp"
#include "../core/anabuildorder.hpp"
#include "../core/soup.hpp"
#include "score.hpp"
#include "../core/configuration.hpp"

enum eGameMode
{
	HIDE_MODE,
	BASIC_Game_MODE,
	ADVANCED_Game_MODE,
	EXPERT_Game_MODE,
	EXPERT_COMPUTER_MODE,
	GOSU_Game_MODE,
	GOSU_COMPUTER_MODE,
	TRANSCENDEND_COMPUTER_MODE,
	
	MAX_MODES
};
	
class Game : public UI_Object
{
	public:
		Game(UI_Object* parent, const unsigned int gameNumber);
		~Game();
		
		void update();

		void resetData();
		

		void draw(DC* dc) const;
		void process();

		const bool isOptimizing() const;
		void setOptimizing(const bool opt=true);

		void updateRectangles(const unsigned int maxGame);

		void newGeneration();
	
		const unsigned int getUnchangedGenerations() const;	// gets number of generations where no change in fitness took place
		const unsigned int getRun() const;					// gets number of runs (one run is complete when no <unchangedGenerations> > <maxGenerations>)
		const unsigned int getTotalGeneration() const;			// gets number of total generations of this run
	
		void setUnchangedGenerations(const unsigned int unchanged_generations); 
		void setRun(const unsigned int current_run);
		void setTotalGeneration(const unsigned int total_generations);

		void assignMap(const BASIC_MAP* game_map);
		void setHarvestSpeed(const unsigned int player_num, const eRace harvest_race, const HARVEST_SPEED* harvest_speed);
		void setStartRace(const unsigned int player_num, const eRace player_race);
		void initSoup();
		void assignStartCondition(const unsigned int player_num, const START_CONDITION* start_condition);
		void setStartPosition(const unsigned int player_num, const unsigned int player_position);
		void assignGoal(const unsigned int player_num, const GOAL_ENTRY* player_goal);
		void fillGroups(); // TODO
		void setMode(const eTab tab, const unsigned int gameNum);
	private:
		SOUP* soup;
		START* start[MAX_PLAYER];
		Player* player[MAX_PLAYER];
		ANABUILDORDER* anarace[MAX_PLAYER];
		UNIT unit[MAX_PLAYER][MAX_LOCATIONS];
		
		const BASIC_MAP* map;
		unsigned int mode;
		bool optimizing;
		bool boHasChanged;
		ScoreWindow* scoreWindow;

		unsigned int unchangedGenerations;
 		unsigned int currentRun;
		unsigned int totalGeneration;		
};

inline const unsigned int Game::getUnchangedGenerations() const
{
#ifdef _SCC_DEBUG	
	if(unchangedGenerations > coreConfiguration.getMaxGenerations()) {
		toLog("DEBUG: (Game::getUnchangedGenerations): Variable unchangedGenerations not initialized.");return(0);
	}
#endif
	return(unchangedGenerations);
}

inline const unsigned int Game::getRun() const
{
#ifdef _SCC_DEBUG	
	if(currentRun > coreConfiguration.getMaxRuns()) {
		toLog("DEBUG: (Game::getRun): Variable currentRun not initialized.");return(0);
	}
#endif
	return(currentRun);
}

inline const unsigned int Game::getTotalGeneration() const
{
	return(totalGeneration);
}

inline void Game::setUnchangedGenerations(const unsigned int unchanged_generations)
{
#ifdef _SCC_DEBUG	
	if(unchanged_generations > coreConfiguration.getMaxGenerations()) {
		toLog("DEBUG: (Game::setUnchangedGenerations): Value out of range.");return;
	}
#endif
	unchangedGenerations = unchanged_generations;
}

inline void Game::setRun(const unsigned int current_run)
{
#ifdef _SCC_DEBUG
	if(current_run > coreConfiguration.getMaxRuns()) {
		toLog("DEBUG: (Game::setRun): Value out of range.");return;
	}
#endif
	currentRun = current_run;
}

inline void Game::setTotalGeneration(const unsigned int total_generation) {
	totalGeneration = total_generation;
}



#endif

