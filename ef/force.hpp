#ifndef _GUI_FORCE_HPP
#define _GUI_FORCE_HPP

#include "../ui/window.hpp"

#include "unitmenu.hpp"
#include "goalmenu.hpp"
#include "forcemenu.hpp"
#include "racemenu.hpp"

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
		ForceEntry(UI_Object* parent, Rect rahmen, Rect maxSize, string unit);
		~ForceEntry();
		const int changed() const;
		void updateText(string utext);
		void process();
		void draw(DC* dc) const;
		int oldForce;
		eUnitType type;
		int total;
		int goal;
	private:
		UI_Button* addUnit;
   		UI_Button* subUnit;
		UI_Button* cancelUnit;
};



class ForceWindow:public UI_Window
{
	public:
		ForceWindow(UI_Object* parent, ANARACE* anarace, const int windowNumber);
		~ForceWindow();
		
		void resetData();
		void process();
		void draw(DC* dc) const;
		const int getMarkedUnit() const;

		const bool hasChanged();		
	private:
		bool changed;
//		UI_Radio* radio;
		UI_Radio* menuRadio;
//		UI_Button* goalButton[3];
		UI_Button* menuButton[MAX_MENUS];

		UI_StaticText* nongoals;
		UI_StaticText* goals;

		void closeMenus();

		ForceEntry* forceEntry[GAS_SCV+1];
		
		int markedUnit;
		ANARACE* anarace;
		int oldForceList[UNIT_TYPE_COUNT];
		int currentUnitType;

		UnitMenu* unitMenu;
		GoalMenu* goalMenu;
		ForceMenu* forceMenu;
		RaceMenu* raceMenu;
};

#endif

