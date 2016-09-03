#ifndef __BOGRAPH_H
#define __BOGRAPH_H

#include "graphics.h"
#include "../scc2dll/anarace.h"
#include "list.h"

class BoGraphWindow:public GraphixScrollWindow
{
        public:
                BoGraphWindow(wxRect rahmen, wxRect maxSize);
                ~BoGraphWindow();
                void resetData();
                void CheckOrders();
                void MoveOrders();
                void showBoGraph(wxDC* dc);
                int CheckForInfoWindow();
		void setAnarace(ANARACE* anarace);
                OrderList orderList;
        private:
                ANARACE* anarace;
};

#endif

