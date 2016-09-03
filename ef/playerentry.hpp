#ifndef _GUI_PLAYERENTRY_HPP
#define _GUI_PLAYERENTRY_HPP

#include "../ui/object.hpp"
#include "../ui/radio.hpp"

#include "racemenu.hpp"

enum eScoreMode
{
	SCORE_FULFILL_MODE,
	SCORE_TIME_MODE,
	SCORE_MODE	
};

enum eInitMode
{
	INACTIVE,
	NOT_INITIALIZED,
	RACE_CHOSEN_BUT_NOT_INITIALIZED,
	INITIALIZED	
};

enum ePlayerEntryMenu
{
        RACE_MENU,
        MAX_PLAYER_ENTRY_MENUS
};


class PlayerEntry : public UI_Object
{
	public:
		PlayerEntry(UI_Object* player_parent, const Rect rect, const Size distance_bottom_right);
		~PlayerEntry();

		void setScore(const unsigned int score);
		void setGoalComplete(const unsigned int goal_complete);
		void setOptimizing(const bool opt=true);
		void setInitMode(const eInitMode init_mode);
		void setScoreMode(const eScoreMode score_mode);
		void setNumber(const unsigned int player_number);
		const eScoreMode getScoreMode() const;
		const eInitMode getInitMode() const;

		void resetData();
		const bool isOptimizing() const;

		void process();
		void draw(DC* dc) const;

		const unsigned int getLineHeight() const;

		const signed int getAssignedRace() const;
		void mouseHasLeft();
	private:
		void closeMenus();
		unsigned int oldScoreCounter[20], oldScore[20];
		unsigned int currentScore, programScore;
		unsigned int goalComplete;

		eInitMode initMode;
		eScoreMode scoreMode;
//		UI_StaticText* goalsFulfilledText;
		UI_Button* currentActionButton;
		UI_StaticText* scoreText;
		UI_StaticText* playerText;
		UI_Button* addPlayerButton;
		UI_Button* removePlayerButton;
		bool optimizing;
		unsigned int number;

		UI_Radio* menuRadio;
		UI_Button* menuButton[MAX_PLAYER_ENTRY_MENUS];
		RaceMenu* raceMenu;
		signed int assignRace;

};



#endif
