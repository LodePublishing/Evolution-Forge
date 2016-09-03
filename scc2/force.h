#ifndef __FORCE_H
#define __FORCE_H

#include "UI_Window.h"
#include "UI_Button.h"
#include "../scc2dll/anarace.h"
#include "../scc2dll/settings.h"

const int FORCE_LIST_NUMBER=23;
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
		ForceEntry(UI_Object* parent, wxRect rect, wxRect maxRect, wxString unit);
		~ForceEntry();
		int changed();
		void updateText(wxString utext);
	private:
		UI_Button* addUnit;
   		UI_Button* subUnit;
		UI_Button* cancelUnit;
};



class ForceWindow:public UI_Window
{
	public:
		ForceWindow(UI_Object* parent, wxRect rahmen, wxRect maxSize, ANARACE* anarace);
		~ForceWindow();
		
		void resetData();
		void process();
		void draw(wxDC* dc);
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

