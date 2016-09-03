#ifndef _GUI_GAME_HPP
#define _GUI_GAME_HPP

#include "player.hpp"
#include "../ui/object.hpp"
#include "../core/anabuildorder.hpp"
#include "../core/soup.hpp"
#include "score.hpp"
#include "../core/configuration.hpp"

class Game : public UI_Window
{
	public:
		Game(UI_Object* game_parent, const unsigned int gameNumber, const unsigned int gameMax);
		~Game();
//		void refreshGameNumbers(const unsigned int gameNumber, const unsigned int totalGames);
		
		void update();
		void resetData();

		void draw(DC* dc) const;
		void process();

		const bool isOptimizing(const unsigned int player_number) const;
		void setOptimizing(const bool opt=true);


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
		
		void setMode(const unsigned int gameNumber, const unsigned int maxGames);
		const unsigned int getMapPlayerCount() const;

		const bool isSplitGame() const;
		const bool isRemoveGame() const;
	private:
		SOUP* soup;
		START* start[MAX_INTERNAL_PLAYER];
		Player* player[MAX_PLAYER];
		ANABUILDORDER* anarace[MAX_PLAYER];
		UNIT startForce[MAX_INTERNAL_PLAYER][MAX_LOCATIONS];
		
		const BASIC_MAP* map;
		bool optimizing;
		bool boHasChanged;
		ScoreWindow* scoreWindow;
		//DatabaseWindow* databaseWindow;

		unsigned int mapPlayerCount;

		unsigned int unchangedGenerations;
 		unsigned int currentRun;
		unsigned int totalGeneration;		

		unsigned int gameNumber;
		unsigned int gameMax;
		UI_Button* splitGameButton;
		UI_Button* removeButton; // remove Game
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

inline void Game::setHarvestSpeed(const unsigned int player_num, const eRace harvest_race, const HARVEST_SPEED* harvest_speed) {
#ifdef _SCC_DEBUG
        if((player_num < 1) || (player_num > mapPlayerCount)) {
                toLog("DEBUG: (Game::setHarvestSpeed): Value player_num out of range.");return;
        }
#endif
	start[player_num]->setHarvestSpeed(harvest_race, harvest_speed);
}

inline void Game::setStartRace(const unsigned int player_num, const eRace player_race) {
#ifdef _SCC_DEBUG
	if((player_num < 1) || (player_num > mapPlayerCount)) {
		toLog("DEBUG: (Game::setStartRace): Value player_num out of range.");return;
	}
#endif
	start[player_num]->setPlayerRace(player_race);
}

inline void Game::assignStartCondition(const unsigned int player_num, const START_CONDITION* start_condition) {
#ifdef _SCC_DEBUG
	if((player_num < 1) || (player_num > mapPlayerCount)) {
		toLog("DEBUG: (Game::assignStartCondition): Value player_num out of range.");return;
	}                       
#endif
	start[player_num]->assignStartCondition(start_condition);
}



inline void Game::setStartPosition(const unsigned int player_num, const unsigned int player_position) {
#ifdef _SCC_DEBUG
        if((player_num < 1) || (player_num > mapPlayerCount)) {
                toLog("DEBUG: (Game::setStartPosition): Value player_num out of range.");return;
        }
#endif
	start[player_num]->setStartPosition(player_position);
}

inline void Game::assignGoal(const unsigned int player_num, const GOAL_ENTRY* player_goal) {
#ifdef _SCC_DEBUG
        if((player_num < 1) || (player_num > mapPlayerCount)) {
                toLog("DEBUG: (Game::assignGoal): Value player_num out of range.");return;
        }       
#endif
	start[player_num]->assignGoal(player_goal);
}

inline const bool Game::isRemoveGame() const {
	return removeButton->isLeftClicked();
}

inline const bool Game::isSplitGame() const {
	return splitGameButton->isLeftClicked();
}

inline const unsigned int Game::getMapPlayerCount() const {
	return(mapPlayerCount);
}

inline const bool Game::isOptimizing(const unsigned int player_number) const {
	return(scoreWindow->isOptimizing(player_number));
}

inline void Game::setOptimizing(const bool opt) {
//	scoreWindow->setOptimizing(opt);
}

inline void Game::resetData() {
	boHasChanged = true;
}

#endif

