#ifndef _GUI_SCORE_HPP
#define _GUI_SCORE_HPP

#include "../ui/window.hpp"

enum eScoreMode
{
	SCORE_FULFILL_MODE,
	SCORE_TIME_MODE,
	SCORE_MODE	
};

class ScoreWindow : public UI_Window
{
	public:
		ScoreWindow(UI_Object* score_parent);
		~ScoreWindow();

		void resetData();
		void process();
		void draw(DC* dc) const;

		const eScoreMode getCurrentMode(const unsigned int player) const;
		void setMode(const unsigned int player, const eScoreMode current_mode);

		void setOptimizing(const bool opt=true);
		const bool isOptimizing() const;

		void setScore(const unsigned int player, const unsigned int score);
		void setGoalComplete(const unsigned int player, const unsigned int goal);
		void setPlayers(const unsigned int player_count);
	private:
		unsigned int oldScoreCounter[MAX_PLAYER][20], oldScore[MAX_PLAYER][20];
		unsigned int currentScore[MAX_PLAYER], programScore[MAX_PLAYER];
		unsigned int goalComplete[MAX_PLAYER];
		
		eScoreMode mode[MAX_PLAYER]; // 0: Fulfill goal, 1: Time goal, 1: Score Goal (TODO)
		
		UI_StaticText* goalsFulfilledText[MAX_PLAYER];
		UI_StaticText* currentActionText[MAX_PLAYER];
		UI_StaticText* scoreText[MAX_PLAYER];
		UI_StaticText* playerText[MAX_PLAYER];

		UI_Button* continueButton;
//		UI_Button* resetButton;

		bool optimizing;
		unsigned int players;
		
};

inline const bool ScoreWindow::isOptimizing() const {
	return optimizing;
}

#endif

