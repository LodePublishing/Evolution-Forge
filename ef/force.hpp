#ifndef _GUI_FORCE_HPP
#define _GUI_FORCE_HPP

#include "unitmenu.hpp"
#include "goalmenu.hpp"
//#include "racemenu.hpp"
#include "forceentry.hpp"
//#include "locationmenu.hpp"
#include "techtree.hpp"

#include "../ui/radio.hpp"

#include "../core/anabuildorder.hpp"
#include "../core/database.hpp"

enum eMenu
{
//	RACE_MENU,
	UNIT_MENU,
	GOAL_MENU,
	SAVE_GOALS,
	
	MAX_FORCE_MENUS
};

class ForceWindow : public UI_Window
{
	public:
		ForceWindow(UI_Object* force_parent, const unsigned int game_number, const unsigned int game_max, const unsigned int player_number, const unsigned int player_max);
		~ForceWindow();
		
		void processList();
		void process();

		void draw(DC* dc) const;
		const unsigned int getMarkedUnit() const;
		void setMarkedUnit(const unsigned int marked_unit);
		void assignAnarace(ANABUILDORDER* force_anarace);
		void reloadStrings();
		void setMode(const unsigned int game_number, const unsigned int game_max, const unsigned int player_number, const unsigned int player_max);
		void reloadOriginalSize();
		static TechTreeWindow* techTreeWindow;

		void resetData();

		void needSaveBox();
		void saveBoxIsCanceled();
		void saveBoxIsDone(std::string& input_string);

		const bool saveGoal();
		const bool openGoalMenu();
		const bool openUnitMenu();
		const signed int getAssignedGoal() const;
	private:
		unsigned int currentGoalUnit; // which goal is currently highlighted?
		unsigned int startLine;

		
		void closeMenus();

		UI_Radio* menuRadio;
		UI_Button* menuButton[MAX_FORCE_MENUS];

		UI_StaticText* nongoalsText;
		UI_StaticText* goalsText;
//		UI_StaticText* locationName[2][MAX_LOCATIONS];
		UI_StaticText* legend;

		std::list<ForceEntry*> goalForceList;
		std::list<ForceEntry*> nongoalForceList;

		unsigned int markedUnit, oldMarkedUnit;
		ANABUILDORDER* anarace;

		UnitMenu* unitMenu;
		GoalMenu* goalMenu;
//		RaceMenu* raceMenu;
//		LocationMenu* locationMenu;
		unsigned int gameNumber;
		unsigned int gameMax;
		unsigned int playerNumber;
		unsigned int playerMax;
		bool saveBox;

		signed int assignGoal;

		void mouseHasLeft();
		
		bool goalMenuOpenedExternally;
		bool unitMenuOpenedExternally;
};

inline const signed int ForceWindow::getAssignedGoal() const {
	return(assignGoal);
}

inline void ForceWindow::setMarkedUnit(const unsigned int marked_unit)
{
#ifdef _SCC_DEBUG
	if(marked_unit >= LAST_UNIT) {
		toErrorLog("DEBUG: (ForceWindow::setMarkedUnit): Value marked_unit out of range.");return;
	}
#endif
	markedUnit = marked_unit;
}
	
inline const unsigned int ForceWindow::getMarkedUnit() const
{
#ifdef _SCC_DEBUG
	if(markedUnit >= LAST_UNIT) {
		toErrorLog("DEBUG: (ForceWindow::getMarkedUnit): Value markedUnit out of range.");return(0);
	}
#endif
	return (markedUnit);
}

#endif

