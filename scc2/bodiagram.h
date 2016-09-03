#ifndef __BODIAGRAM_H
#define __BODIAGRAM_H

#include "UI_Window.h"
#include "../scc2dll/anarace.h"
#include "info.h"

class BoDiagramWindow:public UI_Window
{
        public:
                BoDiagramWindow(UI_Object* parent, wxRect rahmen, wxRect maxSize, ANARACE* anarace, InfoWindow* infoWindow);
                ~BoDiagramWindow();

		void resetData();
		void process();		
		void draw(wxDC* dc);
		
        private:
		InfoWindow* infoWindow;
                ANARACE* anarace;
};


#endif

