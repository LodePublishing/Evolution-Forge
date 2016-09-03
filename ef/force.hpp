#ifndef _GUI_FORCE_HPP
#define _GUI_FORCE_HPP

#include "../ui/window.hpp"

#include "unitmenu.hpp"
#include "goalmenu.hpp"
#include "racemenu.hpp"
#include "message.hpp"
#include "forceentry.hpp"
#include "locmenu.hpp"

#include "../core/anarace.hpp"
#include "../core/settings.hpp"
#include "../ui/radio.hpp"


enum eMenu
{
	RACE_MENU,
	UNIT_MENU,
	GOAL_MENU,
	MAX_MENUS
};

class ForceWindow : public UI_Window
{
	public:
		ForceWindow(UI_Object* force_parent, ANARACE* force_anarace, MessageWindow* force_msg_window, const unsigned int force_window_number);
		~ForceWindow();
		
		void processList();
		void process();
		void draw(DC* dc) const;
		const unsigned int getMarkedUnit() const;
		void setMarkedUnit(const unsigned int marked_unit);
		void assignAnarace(ANARACE* force_anarace);
		void drawTechTree(DC* dc) const;
		void reloadStrings();
	private:
		unsigned int addUnit, addTime, addLocation;
		signed int addCount;
		
		unsigned int currentGoalUnit; // which goal is currently highlighted?
		unsigned int startLine;
	
		void closeMenus();

		MessageWindow* msgWindow;

		UI_Radio* menuRadio;
		UI_Button* menuButton[MAX_MENUS];
		UI_Button* saveGoalButton;

		UI_StaticText* nongoalsText;
		UI_StaticText* goalsText;
		UI_StaticText* locationName[2][MAX_LOCATIONS];
		UI_StaticText* legend;

		list<ForceEntry*> goalForceList;
		list<ForceEntry*> nongoalForceList;
		
		unsigned int markedUnit, oldMarkedUnit;
		ANARACE* anarace;

		UnitMenu* unitMenu;
		GoalMenu* goalMenu;
		RaceMenu* raceMenu;
		LocationMenu* locationMenu;
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

