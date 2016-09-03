#ifndef _GUI_SCORE_HPP
#define _GUI_SCORE_HPP

#include "../ui/window.hpp"
#include "mapmenu.hpp"
#include "playerentry.hpp"

class ScoreWindow : public UI_Window
{
	public:
		ScoreWindow(UI_Object* score_parent, const unsigned int game_number, const unsigned int game_max);
		~ScoreWindow();

		void resetData();
		void process();
		void draw(DC* dc) const;

		const eScoreMode getScoreMode(const unsigned int player_number) const;
		void setScoreMode(const unsigned int player_number, const eScoreMode score_mode);

		const eInitMode getInitMode(const unsigned int player_number) const;
		void setInitMode(const unsigned int player_number, const eInitMode init_mode);

		void setScore(const unsigned int player, const unsigned int score);
		void setGoalComplete(const unsigned int player, const unsigned int goal);
		void setPlayers(const unsigned int player_count);
		void setMaxPlayer(const unsigned int max_player);
		const unsigned int getPlayers() const;
		const bool isOptimizing(const unsigned int player_number) const;
		const signed int getAssignedMap() const;

		void reloadOriginalSize();
		void setMode(const unsigned int game_number, const unsigned int game_max);

		const signed int getAssignedRace(const unsigned int player_number) const;
	private:
		PlayerEntry* player[MAX_PLAYER];
//		UI_Button* resetButton;
		unsigned int players;
		unsigned int maxPlayer;

		void closeMenus();
		UI_Button* mapMenuButton;
		MapMenu* mapMenu;		
		signed int assignMap;

		unsigned int gameNumber;
		unsigned int gameMax;
};

//inline const bool ScoreWindow::isOptimizing() const {
//	return optimizing;
//}

inline void ScoreWindow::setScore(const unsigned int player_number, const unsigned int score) 
{
#ifdef _SCC_DEBUG       
	if(player_number >= maxPlayer) {
		toLog("DEBUG: (ScoreWindow::setScore): Value player_number out of range.");return;
	}
#endif
	player[player_number]->setScore(score);
}

inline void ScoreWindow::setGoalComplete(const unsigned int player_number, const unsigned int goal) {
#ifdef _SCC_DEBUG       
	if(player_number >= maxPlayer) {
		toLog("DEBUG: (ScoreWindow::setGoalComplete): Value player_number out of range.");return;
	}
#endif
	player[player_number]->setGoalComplete(goal);
}

inline void ScoreWindow::setPlayers(const unsigned int player_count) {
#ifdef _SCC_DEBUG       
	if(player_count >= maxPlayer) {
		toLog("DEBUG: (ScoreWindow::setPlayers): Value player_count out of range.");return;
	}
#endif
	players = player_count;
}

inline const unsigned int ScoreWindow::getPlayers() const {
	return(players);
}

inline const signed int ScoreWindow::getAssignedMap() const {
	return(assignMap);
}

inline const bool ScoreWindow::isOptimizing(const unsigned int player_number) const
{
#ifdef _SCC_DEBUG       
        if(player_number >= maxPlayer) {
                toLog("DEBUG: (ScoreWindow::isOptimizing): Value player_number out of range.");return(false);
        }
#endif
	return(player[player_number]->isOptimizing());
}

inline void ScoreWindow::setMaxPlayer(const unsigned int max_player)
{
#ifdef _SCC_DEBUG       
	if(max_player > MAX_PLAYER) {
		toLog("DEBUG: (ScoreWindow::setMaxPlayer): Value max_player out of range.");return;
	}
#endif
	maxPlayer = max_player;
}

inline void ScoreWindow::setScoreMode(const unsigned int player_number, eScoreMode score_mode) // for different tabs different behaviour
{
#ifdef _SCC_DEBUG       
	if(player_number >= maxPlayer) {
		toLog("DEBUG: (ScoreWindow::setScoreMode): Value player_number out of range.");return;
	}
#endif
	player[player_number]->setScoreMode(score_mode);
}

inline const eScoreMode ScoreWindow::getScoreMode(const unsigned int player_number) const
{
#ifdef _SCC_DEBUG       
	if(player_number >= maxPlayer) {
		toLog("DEBUG: (ScoreWindow::getScoreMode): Value player_number out of range.");return(SCORE_FULFILL_MODE);
	}
#endif
	return(player[player_number]->getScoreMode());
}

inline void ScoreWindow::setInitMode(const unsigned int player_number, eInitMode init_mode) // for different tabs different behaviour
{
#ifdef _SCC_DEBUG
	if(player_number >= maxPlayer) {
		toLog("DEBUG: (ScoreWindow::setInitMode): Value player_number out of range.");return;
	}
#endif
	player[player_number]->setInitMode(init_mode);
}

inline const eInitMode ScoreWindow::getInitMode(const unsigned int player_number) const
{
#ifdef _SCC_DEBUG       
	if(player_number >= maxPlayer) {
		toLog("DEBUG: (ScoreWindow::getInitMode): Value player_number out of range.");return(INACTIVE);
	}
#endif
	return(player[player_number]->getInitMode());
}

inline const signed int ScoreWindow::getAssignedRace(const unsigned int player_number) const
{
#ifdef _SCC_DEBUG       
        if(player_number >= maxPlayer) {
                toLog("DEBUG: (ScoreWindow::getAssignedRace): Value player_number out of range.");return(-1);
        }
#endif
	return(player[player_number]->getAssignedRace());
}


#endif

