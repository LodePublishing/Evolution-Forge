#ifndef __INFO_H
#define __INFO_H

#include "graphics.h"
#include "../scc2dll/anarace.h"
#include "list.h"

const char error_message[ERROR_MESSAGES][25]=
{
        "possible.", //= OK
        "enough minerals.", 
        "enough gas.",
        "supply is satisfied.",
        "prerequisite",
        "facility",
        "hell freezes.", //timeout
        "SC2 comes out." //unknown
};

class InfoWindow:public GraphixScrollWindow
{
        public:
                InfoWindow(wxRect rahmen, wxRect maxSize);
                ~InfoWindow();
                void resetData();
                void drawInfo(wxDC* dc);
		void setAnarace(ANARACE* anarace);

		int getBx();
		int getBWidth();
		int getUnit();
		int getKey();

		void setBx(int bx);
		void setBWidth(int bwidth);
		void setUnit(int unit);
		void setKey(int key);

		void setOrder(ORDER* order);
		ORDER* getOrder();

		int isSet();
		void setupOk(int ok); // already setuped by bowindow
        private:
		int bx,bwidth,unit,key;
		int setup;
		ORDER* order;
                ANARACE* anarace;
};

#endif

