#ifndef __INFO_H
#define __INFO_H

#include "graphics.h"
#include "../scc2dll/anarace.h"
#include "list.h"

class InfoWindow:public GraphixScrollWindow
{
        public:
                InfoWindow(wxRect rahmen, wxRect maxSize);
                ~InfoWindow();
                void resetData();
                void showInfoWindow(wxDC* dc);
		void setAnarace(ANARACE* anarace);
        private:
                ANARACE* anarace;
                ORDER* infoWindowNumber;
};

#endif

