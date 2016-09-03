#ifndef __BOGRAPH_H
#define __BOGRAPH_H

#include "graphics.h"
#include "../scc2dll/anarace.h"
#include "list.h"
#include "info.h"

class BoGraphWindow:public GraphixScrollWindow
{
	public:
		BoGraphWindow(wxRect rahmen, wxRect maxSize);
		~BoGraphWindow();
		void resetData();
		void showBoGraph(wxDC* dc, OrderList* orderList);
		void setAnarace(ANARACE* anarace);
		void setOrderList(OrderList* orderList);
		void assignInfoWindow(InfoWindow* infoWindow);
		bool mouseOnOrder(ORDER* order);
		void setMarkedUnit(int unit);
	private:
		int markedUnit;
		int markAni;
		InfoWindow* infoWindow;
		ANARACE* anarace;
};

#endif

