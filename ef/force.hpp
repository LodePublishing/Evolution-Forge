#ifndef _GUI_FORCE_HPP
#define _GUI_FORCE_HPP

#include "../ui/window.hpp"

#include "unitmenu.hpp"
#include "goalmenu.hpp"
//#include "racemenu.hpp"
#include "forceentry.hpp"
#include "locmenu.hpp"

#include "techtree.hpp"

#include "../core/anabuildorder.hpp"
#include "../core/database.hpp"
#include "../ui/radio.hpp"
#include "../ui/checkbutton.hpp"


enum eMenu
{
//	RACE_MENU,
	UNIT_MENU,
	GOAL_MENU,
	
	MAX_FORCE_MENUS
};

class ForceWindow : public UI_Window
{
	public:
		ForceWindow(UI_Object* force_parent, const unsigned int game_number, const unsigned int max_games, const unsigned int player_number, const unsigned int max_players);
		~ForceWindow();
		
		void processList();
		void process();
		void draw(DC* dc) const;
		const unsigned int getMarkedUnit() const;
		void setMarkedUnit(const unsigned int marked_unit);
		void assignAnarace(ANABUILDORDER* force_anarace);
		void reloadStrings();
		static TechTreeWindow* techTreeWindow;
	private:
		unsigned int addUnit, addTime, addLocation;
		signed int addCount;
		
		unsigned int currentGoalUnit; // which goal is currently highlighted?
		unsigned int startLine;
	
		void closeMenus();

		UI_Radio* menuRadio;
		UI_Button* menuButton[MAX_FORCE_MENUS];
		UI_Button* saveGoalButton;

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

};

inline void ForceWindow::setMarkedUnit(const unsigned int marked_unit)
{
#ifdef _SCC_DEBUG
	if(marked_unit >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ForceWindow::setMarkedUnit): Value marked_unit out of range.");return;
	}
#endif
	markedUnit = marked_unit;
}
	
inline const unsigned int ForceWindow::getMarkedUnit() const
{
#ifdef _SCC_DEBUG
	if(markedUnit >= UNIT_TYPE_COUNT) {
		toLog("DEBUG: (ForceWindow::getMarkedUnit): Value markedUnit out of range.");return(0);
	}
#endif
	return (markedUnit);
}

#endif

