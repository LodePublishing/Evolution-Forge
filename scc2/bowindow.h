#ifndef __BOWINDOW_H
#define __BOWINDOW_H

#include "UI_Window.h"
#include "../scc2dll/anarace.h"
#include "list.h"
#include "info.h"


class BoWindow:public UI_Window
{
	public:
		BoWindow(UI_Object* parent, wxRect rahmen, wxRect maxSize, ANARACE* anarace, InfoWindow* infoWindow, OrderList* orderList);
		~BoWindow();
		
		void resetData();
		void draw(wxDC* dc);
		void process();
		
		void setMarkedUnit(int unit);
	private:
		void drawSelectionStuff(wxDC* dc);
		void checkForInfoWindow();
	
		void resetButtons();
		int markedUnit;
		int markAni;
		InfoWindow* infoWindow;
		ANARACE* anarace;
		OrderList* orderList;
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

