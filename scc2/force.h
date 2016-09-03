#ifndef __FORCE_H
#define __FORCE_H

#include "UI_Window.h"
#include "UI_Button.h"
#include "../scc2dll/anarace.h"
#include "../scc2dll/settings.h"

/*
class UnitMenu
{
	v
// to choose units from
};
*/
class ForceEntry:public UI_Button
{
	public:
		ForceEntry(UI_Object* parent, Rect rahmen, Rect maxSize, string unit);
		~ForceEntry();
		int changed();
		void updateText(string utext);
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
		void draw(DC* dc);
		int getMarkedUnit();
		
	private:
		UI_Radio* radio;
		UI_Button* goalButton[4];

		ForceEntry* forceEntry[GAS_SCV+1];
		
		int markedUnit;
		ANARACE* anarace;
		int oldForceList[UNIT_TYPE_COUNT];
		int goalListOpened;
//		int addGoalButton;
		int currentUnitType;
//		int buttonType[11]; //button ID -> unit
//		int selectGoalButton[100];
//		int goalFileListButton;
		int goalFileListOpened;
};

#endif

