#ifndef _GUI_GAME_HPP
#define _GUI_GAME_HPP

#include "player.hpp"
#include "../ui/object.hpp"
#include "../core/anabuildorder.hpp"
#include "../core/soup.hpp"
#include "score.hpp"
#include "configuration.hpp"


class Game : public UI_Window
{
	public:
		Game(UI_Object* game_parent, const unsigned int game_number, const unsigned int game_max);
		~Game();
//		void refreshGameNumbers(const unsigned int gameNumber, const unsigned int totalGames);
		
		void update();
		void resetData();

		void draw(DC* dc) const;
		void process();

		const bool isOptimizing(const unsigned int player_number) const;
		const bool isAnyOptimizing() const;
		void stopOptimizing();
		void startLastOptimizing();

		void newGeneration();
	
//		const unsigned int getUnchangedGenerations() const;	// gets number of generations where no change in fitness took place
//		const unsigned int getTotalGeneration() const;			// gets number of total generations of this run
//		void setUnchangedGenerations(const unsigned int unchanged_generations); 
//		void setTotalGeneration(const unsigned int total_generations);

		void assignMap(const BASIC_MAP* game_map);
		void assignRace(const unsigned int player_num, const eRace race);
		void resetPlayer(const unsigned int player_num);
		void setHarvestSpeed(const unsigned int player_num, const eRace harvest_race, const HARVEST_SPEED* harvest_speed);
		void setStartRace(const unsigned int player_num, const eRace player_race);
		void initSoup();
		void initSoup(unsigned int player_number);
		void assignStartCondition(const unsigned int player_num, const START_CONDITION* start_condition);
		void loadBuildOrder(const unsigned int player_num, const unsigned int number, const GOAL_ENTRY* player_goal);
		void setStartPosition(const unsigned int player_num, const unsigned int player_position);
		void assignGoal(const unsigned int player_num, const GOAL_ENTRY* player_goal);
		void fillGroups(); // TODO
		
		void reloadOriginalSize();
		void setMode(const unsigned int game_number, const unsigned int max_games);
		const unsigned int getMapPlayerCount() const;

		const unsigned int getGameNumber();
		const unsigned int getGameMax();

		const bool isSplitGame() const;
		const bool isRemoveGame() const;

		const bool getResetFlag();
		void setResetFlag(const bool flag=true);

		void compactDisplayModeHasChanged();
		const bool needSaveBox(SaveBoxParameter& savebox_parameter, int* caller_canceled, int* caller_done);

		const bool openMenu(const ePlayerOrder order);
		void setPlayerInitMode(const unsigned int player_number, const eInitMode init_mode);
	private:
		bool resetFlag;
		SOUP* soup;
		START* start[MAX_INTERNAL_PLAYER];
		Player* player[MAX_PLAYER];
		ANABUILDORDER* anarace[MAX_PLAYER];
		unsigned int oldCode[MAX_PLAYER][MAX_LENGTH];
		UNIT startForce[MAX_INTERNAL_PLAYER][MAX_LOCATIONS];

		void setBoHasChanged(const bool bo_has_changed = true);
		
		const BASIC_MAP* map;
		bool optimizing;
		bool boHasChanged;
		ScoreWindow* scoreWindow;
		//DatabaseWindow* databaseWindow;

		unsigned int mapPlayerCount;

		unsigned int unchangedGenerations;
 		unsigned int currentRun;
//		unsigned int totalGenerations;

		unsigned int gameNumber;
		unsigned int gameMax;

		bool lastOptimizing[MAX_PLAYER];

//		UI_Group* buttonGroup;
//		UI_Button* splitGameButton;
//		UI_Button* removeButton; // remove Game
};

/*inline const unsigned int Game::getUnchangedGenerations() const
{
#ifdef _SCC_DEBUG	
	if(unchangedGenerations > efConfiguration.getMaxGenerations()) {
		toErrorLog("DEBUG: (Game::getUnchangedGenerations): Variable unchangedGenerations not initialized.");return(0);
	}
#endif
	return(unchangedGenerations);
}

inline const unsigned int Game::getTotalGeneration() const
{
	return(totalGeneration);
}

inline void Game::setUnchangedGenerations(const unsigned int unchanged_generations)
{
#ifdef _SCC_DEBUG	
	if(unchanged_generations > efConfiguration.getMaxGenerations()) {
		toErrorLog("DEBUG: (Game::setUnchangedGenerations): Value out of range.");return;
	}
#endif
	unchangedGenerations = unchanged_generations;
}

inline void Game::setTotalGeneration(const unsigned int total_generation) {
	totalGeneration = total_generation;
}*/

inline void Game::setHarvestSpeed(const unsigned int player_num, const eRace harvest_race, const HARVEST_SPEED* harvest_speed) {
#ifdef _SCC_DEBUG
        if((player_num < 1) || (player_num > mapPlayerCount)) {
                toErrorLog("DEBUG: (Game::setHarvestSpeed): Value player_num out of range.");return;
        }
#endif
	start[player_num]->setHarvestSpeed(harvest_race, harvest_speed);
}

inline void Game::setStartRace(const unsigned int player_num, const eRace player_race) {
#ifdef _SCC_DEBUG
	if((player_num < 1) || (player_num > mapPlayerCount)) {
		toErrorLog("DEBUG: (Game::setStartRace): Value player_num out of range.");return;
	}
#endif
	start[player_num]->setPlayerRace(player_race);
}

inline void Game::setStartPosition(const unsigned int player_num, const unsigned int player_position) {
#ifdef _SCC_DEBUG
        if((player_num < 1) || (player_num > mapPlayerCount)) {
                toErrorLog("DEBUG: (Game::setStartPosition): Value player_num out of range.");return;
        }
#endif
	start[player_num]->setStartPosition(player_position);
}

inline void Game::assignGoal(const unsigned int player_num, const GOAL_ENTRY* player_goal) {
#ifdef _SCC_DEBUG
        if((player_num < 1) || (player_num > mapPlayerCount)) {
                toErrorLog("DEBUG: (Game::assignGoal): Value player_num out of range.");return;
        }       
#endif
	start[player_num]->assignGoal(player_goal);
}

inline const bool Game::isRemoveGame() const {
	return false;//removeButton->isLeftClicked();
}

inline const bool Game::isSplitGame() const {
	return false;//splitGameButton->isLeftClicked();
}

inline const unsigned int Game::getMapPlayerCount() const {
	return(mapPlayerCount);
}

inline const bool Game::isOptimizing(const unsigned int player_number) const {
	return(scoreWindow->isOptimizing(player_number));
}

inline void Game::resetData() {
	boHasChanged = true;
}

inline void Game::setResetFlag(const bool flag) {
	resetFlag=flag;
}

inline const bool Game::getResetFlag() {
	return(resetFlag);
}

#endif

