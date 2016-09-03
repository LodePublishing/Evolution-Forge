#ifndef __BOGRAPH_H
#define __BOGRAPH_H

#include "UI_Window.h"
#include "../scc2dll/anarace.h"
#include "order.h"
#include "info.h"

#include <map>

class BoGraphWindow:public UI_Window
{
	public:
		BoGraphWindow(UI_Object* parent, ANARACE* anarace, InfoWindow* infoWindow, map <long, Order>* orderList, const int windowNumber);
		~BoGraphWindow();
		
		void resetData();
		void process();
		void draw(DC* dc);
		void setMarkedUnit(int unit);
	private:
		void checkForInfoWindow();
		int markedUnit;
		int markAni;
 		map <long, Order>* orderList;
		InfoWindow* infoWindow;
		ANARACE* anarace;
};

#endif

