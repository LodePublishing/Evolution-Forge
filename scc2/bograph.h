#ifndef __BOGRAPH_H
#define __BOGRAPH_H

#include "UI_Window.h"
#include "../scc2dll/anarace.h"
#include "list.h"
#include "info.h"

class BoGraphWindow:public UI_Window
{
	public:
		BoGraphWindow(UI_Object* parent, wxRect rahmen, wxRect maxSize, ANARACE* anarace, InfoWindow* infoWindow, OrderList* orderList);
		~BoGraphWindow();
		
		void resetData();
		void process();
		void draw(wxDC* dc);
		void setMarkedUnit(int unit);
	private:
		void checkForInfoWindow();
		int markedUnit;
		int markAni;
		OrderList* orderList;
		InfoWindow* infoWindow;
		ANARACE* anarace;
};

#endif

