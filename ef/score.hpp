#ifndef _GUI_SCORE_HPP
#define _GUI_SCORE_HPP

#include "../ui/window.hpp"
#include "mapmenu.hpp"
#include "playerentry.hpp"

enum eScoreMenu
{
	MAP_MENU,
	MAX_SCORE_MENUS
};

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

//		void setOptimizing(const bool opt=true);

		void setScore(const unsigned int player, const unsigned int score);
		void setGoalComplete(const unsigned int player, const unsigned int goal);
		void setPlayers(const unsigned int player_count);
		void setMaxPlayer(const unsigned int max_player);
		const unsigned int getPlayers() const;
		const bool isOptimizing(const unsigned int player_number) const;
		const signed int getAssignedMap() const;

		const signed int getAssignedRace(const unsigned int player_number) const;
	private:
		PlayerEntry* player[MAX_PLAYER];
//		UI_Button* resetButton;
		unsigned int players;
		unsigned int maxPlayer;

		void closeMenus();
		UI_Radio* menuRadio;
		UI_Button* menuButton[MAX_SCORE_MENUS];
		MapMenu* mapMenu;		
		signed int assignMap;
};

//inline const bool ScoreWindow::isOptimizing() const {
//	return optimizing;
//}

#endif

