#ifndef __BODIAGRAM_H
#define __BODIAGRAM_H

#include "graphics.h"
#include "../scc2dll/anarace.h"

class BoDiagramWindow:public GraphixScrollWindow
{
        public:
                BoDiagramWindow(wxRect rahmen, wxRect maxSize);
                ~BoDiagramWindow();
                void resetData();
                void showProgramGraph(wxDC* dc);
		void setAnarace(ANARACE* anarace);
        private:
                ANARACE* anarace;
};


#endif

