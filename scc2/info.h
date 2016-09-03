#ifndef __INFO_H
#define __INFO_H

#include "UI_Window.h"
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

class InfoWindow:public UI_Window
{
        public:
                InfoWindow(UI_Object* parent, wxRect rahmen, wxRect maxSize, ANARACE* anarace);
                ~InfoWindow();
                void resetData();
				void process();
                void draw(wxDC* dc);

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

