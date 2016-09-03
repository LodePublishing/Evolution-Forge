#ifndef __FORCE_H
#define __FORCE_H

#include "graphics.h"
#include "../scc2dll/anarace.h"

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
        private:
                ANARACE* anarace;
                int addUnitButton[UNIT_TYPE_COUNT];
                int oldForceList[FORCE_LIST_NUMBER];
                BOLOG globalForcelog[UNIT_TYPE_COUNT];
		int goalListOpened;
		int addGoalButton;
		int resetButton;
		int goalReset; //TODO
		int buttonType[3*UNIT_TYPE_COUNT]; //button ID -> unit
};

#endif

