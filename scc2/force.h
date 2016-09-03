#ifndef __FORCE_H
#define __FORCE_H

#include "graphics.h"
#include "../scc2dll/anarace.h"
#include "../scc2dll/settings.h"

const int FORCE_LIST_NUMBER=23;

struct BOLOG
{
		int count;
		int order;
};  //this is for logging the build order


class ForceWindow:public GraphixScrollWindow
{
	public:
		void setAnarace(ANARACE* anarace);
		ForceWindow(wxRect rahmen, wxRect maxSize);
		~ForceWindow();
		void resetData();
		void drawGoalList(wxDC* dc);
		int maxUnitForce; //single unit
		void processButtons();
		int getMarkedUnit();
	private:
		int markedUnit;
		ANARACE* anarace;
		int addUnitButton[UNIT_TYPE_COUNT];
		BOLOG globalForcelog[UNIT_TYPE_COUNT];
		int oldForceList[UNIT_TYPE_COUNT];
		int goalListOpened;
		int addGoalButton;
		int goalButton[4];
		wxString buttonText[4];
		int goalButtonAni[4];
		int buttonType[3*UNIT_TYPE_COUNT]; //button ID -> unit
		int selectGoalButton[UNIT_TYPE_COUNT];//~~
		int goalFileListButton;
		int goalFileListButtonAni;
		int goalFileListOpened;
};

#endif

