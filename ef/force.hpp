#ifndef _GUI_FORCE_HPP
#define _GUI_FORCE_HPP

#include "../ui/window.hpp"

#include "unitmenu.hpp"
#include "goalmenu.hpp"
#include "forcemenu.hpp"
#include "racemenu.hpp"
#include "message.hpp"

#include "../core/anarace.hpp"
#include "../core/settings.hpp"

enum eMenu
{
	RACE_MENU,
	UNIT_MENU,
	GOAL_MENU,
	FORCE_MENU,
	MAX_MENUS
};

class ForceEntry : public UI_Button
{
	public:
		ForceEntry(UI_Object* entry_parent, const Rect entry_rect, const Rect entry_max_size, const string& entry_unit);
		~ForceEntry();
		//const int changed() const;
		void updateText(const string& utext);
		void process();
		void draw(DC* dc) const;

		void setTargetForce(const unsigned int force);

		void setType(const eUnitType type);
		const eUnitType getType() const;
		void setTotal(const unsigned int total);
		void setGoal(const unsigned int goal);
		void HideIt();
	private:
        
		unsigned int startForce;
		unsigned int targetForce;
		unsigned int currentForce;
		
		eUnitType type;
		unsigned int total;
		unsigned int goal;
};



class ForceWindow:public UI_Window
{
	public:
		ForceWindow(UI_Object* force_parent, ANARACE* force_anarace, MessageWindow* force_msg_window, const unsigned int force_window_number);
		~ForceWindow();
		
		void resetData();
		void process();
		void draw(DC* dc) const;
		const unsigned int getMarkedUnit() const;
		void setMarkedUnit(const unsigned int markedUnit);

		const bool hasChanged();		

	private:
		void closeMenus();

		MessageWindow* msgWindow;

//		UI_Button* addUnit;
// 		UI_Button* subUnit;
//		UI_Button* cancelUnit;

		bool changed;
//		UI_Radio* radio;
		UI_Radio* menuRadio;
//		UI_Button* goalButton[3];
		UI_Button* menuButton[MAX_MENUS];

		UI_StaticText* nongoals;
		UI_StaticText* goals;
		UI_StaticText* legend;


		ForceEntry* forceEntry[GAS_SCV+1];
		
		unsigned int markedUnit, oldMarkedUnit;
		ANARACE* anarace;
//		int oldForceList[UNIT_TYPE_COUNT];
		unsigned int currentUnitType;

		unsigned int unitTypeCount[UNIT_TYPE_TYPES];
		unsigned int startUnitTypeCount[UNIT_TYPE_TYPES];
		unsigned int currentUnitTypeCount[UNIT_TYPE_TYPES];
		unsigned int targetUnitTypeCount[UNIT_TYPE_TYPES];
		unsigned int totalUnits;

		UnitMenu* unitMenu;
		GoalMenu* goalMenu;
		ForceMenu* forceMenu;
		RaceMenu* raceMenu;
};

#endif

