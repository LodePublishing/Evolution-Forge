#ifndef __BODIAGRAM_H
#define __BODIAGRAM_H

#include "UI_Window.h"
#include "../scc2dll/anarace.h"
#include "info.h"

class BoDiagramWindow:public UI_Window
{
	public:
		BoDiagramWindow(UI_Object* parent, ANARACE* anarace, InfoWindow* infoWindow, const int windowNumber);
        ~BoDiagramWindow();

		void resetData();
		void process();		
		void draw(DC* dc);
		
        private:
		InfoWindow* infoWindow;
                ANARACE* anarace;
};


#endif

