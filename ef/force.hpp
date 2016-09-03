#ifndef _GUI_FORCE_HPP
#define _GUI_FORCE_HPP

#include "../ui/window.hpp"

#include "unitmenu.hpp"
#include "goalmenu.hpp"
//#include "forcemenu.hpp"
#include "racemenu.hpp"
#include "message.hpp"
#include "forceentry.hpp"

#include "../core/anarace.hpp"
#include "../core/settings.hpp"
#include "../ui/radio.hpp"

enum eMenu
{
	RACE_MENU,
	UNIT_MENU,
	GOAL_MENU,
//	FORCE_MENU,
	MAX_MENUS
};

class ForceWindow : public UI_Window
{
	public:
		ForceWindow(UI_Object* force_parent, ANARACE* force_anarace, MessageWindow* force_msg_window, const unsigned int force_window_number);
		~ForceWindow();
		
		void resetData();
		void process();
		void draw(DC* dc) const;
		const unsigned int getMarkedUnit() const;
		void setMarkedUnit(const unsigned int marked_unit);

	private:
		void drawTechTree(DC* dc) const;

		unsigned int currentGoalUnit; // which goal is currently highlighted?
	
		void closeMenus();

		MessageWindow* msgWindow;

		UI_Radio* menuRadio;
		UI_Button* menuButton[MAX_MENUS];
		UI_Button* saveGoalButton;

		UI_StaticText* nongoals;
		UI_StaticText* goals;
		UI_StaticText* legend;

		list<ForceEntry*> forceList;

		
		unsigned int markedUnit, oldMarkedUnit;
		ANARACE* anarace;

		UnitMenu* unitMenu;
		GoalMenu* goalMenu;
//		ForceMenu* forceMenu; TODO -> in Map-tab reinschieben
		RaceMenu* raceMenu;
};

#endif

