#ifndef __INFO_H
#define __INFO_H

#include "UI_Window.h"
#include "../scc2dll/anarace.h"
#include "order.h"

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
                InfoWindow(UI_Object* parent, ANARACE* anarace, const int windowNumber);
                ~InfoWindow();
                void resetData();
				void process();
                void draw(DC* dc);

		const int getBx() const;
		const int getBWidth() const;
		const int getUnit() const;

		void setBx(const int bx);
		void setBWidth(const int bwidth);
		void setUnit(const int unit);
		void setIP(const int key);
		const int getIP() const; 

		const int isSet() const;
		void setupOk(const int ok); // already setuped by bowindow
	private:
		int bx,bwidth,unit,key;
		int setup;
		int IP;
		ANARACE* anarace;
};

#endif

