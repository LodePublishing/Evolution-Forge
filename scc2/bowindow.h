#ifndef __BOWINDOW_H
#define __BOWINDOW_H

#include "graphics.h"
#include "../scc2dll/anarace.h"
#include "list.h"


class BoWindow:public GraphixScrollWindow
{
        public:
                BoWindow(wxRect rahmen, wxRect maxSize);
                ~BoWindow();
                void resetData();
                void drawBuildOrder(wxDC* dc);
                void drawSelectionStuff(wxDC* dc);
                void processButtons();
                void CheckOrders();
                void MoveOrders();
                int CheckForInfoWindow();
                OrderList orderList;
		void setAnarace(ANARACE* anarace);
        private:
                int infoWindowNumber;
                ANARACE* anarace;
                int optimizeMode;
                int boInsertPoint,boEndPoint;
                int unitButton[MAX_LENGTH];
                int optButton[MAX_LENGTH];
                int boGoalListOpened;
                int lastBogoal;
};

#endif

