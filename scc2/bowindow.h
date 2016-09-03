#ifndef __BOWINDOW_H
#define __BOWINDOW_H

#include "graphics.h"
#include "../scc2dll/anarace.h"
#include "list.h"
#include "info.h"


class BoWindow:public GraphixScrollWindow
{
	public:
		BoWindow(wxRect rahmen, wxRect maxSize);
		~BoWindow();
		void resetData();
		void drawBuildOrder(wxDC* dc, OrderList* orderList);
		void drawSelectionStuff(wxDC* dc);
		void processButtons();
		void setAnarace(ANARACE* anarace);
		void assignInfoWindow(InfoWindow* infoWindow);
		bool mouseOnOrder(ORDER* order);
		int makeSpace;
		void setMarkedUnit(int unit);
	private:
		int markedUnit;
		int markAni;
		InfoWindow* infoWindow;
		ANARACE* anarace;
		int optimizeMode;
		int boInsertPoint,boEndPoint;
		int makeSpaceButton;
		int unitButton[2*MAX_LENGTH];
		int optButton[2*MAX_LENGTH];
		int orderButton[2*MAX_LENGTH];
		int boGoalListOpened;
		int lastBogoal;
};

#endif

