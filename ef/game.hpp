#ifndef _GUI_GAME_HPP
#define _GUI_GAME_HPP

#include "../core/soup.hpp"
#include "configuration.hpp"

#include "player.hpp"
#include "score.hpp"

class Game : public UI_Window
{
	public:
		Game(UI_Object* game_parent, const BASIC_MAP* game_map, const unsigned int game_number, const unsigned int game_max);
		~Game();
		
		void resetData();

		void draw(DC* dc) const;
		void process();
		void reloadOriginalSize();

		const bool isOptimizing(const unsigned int player_number) const;
		const bool isAnyOptimizing() const;
		void stopOptimizing();
		void startLastOptimizing();

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

		void setMapPlayerCount(const unsigned int player_count);
		

		void newGeneration();
	
		void resetPlayerTime();
//		const unsigned int getUnchangedGenerations() const;	// gets number of generations where no change in fitness took place
//		const unsigned int getTotalGeneration() const;			// gets number of total generations of this run
//		void setUnchangedGenerations(const unsigned int unchanged_generations); 
//		void setTotalGeneration(const unsigned int total_generations);

		void assignMap(const BASIC_MAP* game_map);
		void assignRace(const unsigned int player_num, const eRace race);
		void resetPlayer(const unsigned int player_num);
		void setHarvestSpeed(const unsigned int player_num, const eRace harvest_race, const HARVEST_SPEED* harvest_speed);
//		void setStartRace(const unsigned int player_num, const eRace player_race);
		void initSoup();
		void initSoup(unsigned int player_number);
		void assignStartCondition(const unsigned int player_num, const START_CONDITION* start_condition);
		void loadBuildOrder(const unsigned int player_num, const unsigned int number);
		void setStartPosition(const unsigned int player_num, const unsigned int player_position);
		void assignGoal(const unsigned int player_num, const unsigned int player_goal);
		void fillGroups(); // TODO

		const bool isCompletelyInitialized() const;
	private:
		Player* player[MAX_PLAYER];
		ScoreWindow* scoreWindow;

		
		unsigned int gameNumber;
		unsigned int gameMax;

		SOUP* soup;
		START* start[MAX_INTERNAL_PLAYER];
		ANABUILDORDER* anarace[MAX_PLAYER];
		unsigned int oldCode[MAX_PLAYER][MAX_LENGTH];
		UNIT startForce[MAX_INTERNAL_PLAYER][MAX_LOCATIONS];
		unsigned int mapPlayerCount;
		const BASIC_MAP* map;
		
		bool optimizing;
		bool lastOptimizing[MAX_PLAYER];
		
		bool resetFlag;
		bool boHasChanged;

		unsigned int unchangedGenerations;

		bool mapPlayerCountInitialized;
		bool anaraceInitialized;

//		UI_Group* buttonGroup;
//		UI_Button* splitGameButton;
//		UI_Button* removeButton; // remove Game
};

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
	resetFlag = flag;
}

inline const bool Game::getResetFlag() {
	return(resetFlag);
}

#endif

