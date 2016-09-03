#ifndef __BOWINDOW_H
#define __BOWINDOW_H

#include "UI_Window.h"
#include "UI_Button.h"
#include "../scc2dll/anarace.h"
#include "info.h"
#include "order.h"
#include <map>


class BoEntry:public UI_Button
{
    public:
        BoEntry(UI_Object* parent, Rect rect, Rect maxRect, string unit);
        ~BoEntry();
        int changed();
        void updateText(string utext);
    private:
//        UI_Button* addUnit;
//      UI_Button* subUnit;
//    UI_Button* cancelUnit; TODO
};


class BoWindow:public UI_Window
{
	public:
		BoWindow(UI_Object* parent, ANARACE* anarace, InfoWindow* infoWindow, map <long, Order>* orderList, const int windowNumber);
		~BoWindow();
		
		void resetData();
		void draw(DC* dc);
		void process();
		
		void setMarkedUnit(int unit);
	private:
		void drawSelectionStuff(DC* dc);
		void checkForInfoWindow();
        BoEntry* boEntry[MAX_LENGTH];
	
//		void resetButtons();
		int markedUnit;
//		int markAni;
		InfoWindow* infoWindow;
		ANARACE* anarace;
		map <long, Order>* orderList;
		int optimizeMode;
		int boInsertPoint,boEndPoint;
//		int makeSpaceButton;
//		int unitButton[2*MAX_LENGTH];
//		int optButton[2*MAX_LENGTH];
//		int orderButton[2*MAX_LENGTH];
		int boGoalListOpened;
		int lastBogoal;
};

#endif

