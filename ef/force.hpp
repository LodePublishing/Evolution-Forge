#ifndef _GUI_FORCE_HPP
#define _GUI_FORCE_HPP

#include "../ui/window.hpp"
#include "../ui/button.hpp"

#include "unitmenu.hpp"

#include "../core/anarace.hpp"
#include "../core/settings.hpp"

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
		UI_Radio* radio;
		UI_Button* goalButton[4];
		UI_StaticText* nongoals;

		ForceEntry* forceEntry[GAS_SCV+1];
		
		int markedUnit;
		ANARACE* anarace;
		int oldForceList[UNIT_TYPE_COUNT];
//		int addGoalButton;
		int currentUnitType;
//		int buttonType[11]; //button ID -> unit
//		int selectGoalButton[100];
//		int goalFileListButton;
		UnitMenu* unitMenu;
};

#endif

