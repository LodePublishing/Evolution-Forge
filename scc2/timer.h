#ifndef __TIMER_H
#define __TIMER_H

#include "graphics.h"
#include "../scc2dll/anarace.h"

class TimerWindow:public GraphixScrollWindow
{
	public:
		void setAnarace(ANARACE* anarace);
		TimerWindow(wxRect rahmen, wxRect maxSize);
		~TimerWindow();
		void resetData();
		void drawTimer(wxDC* dc);
		void processButtons();
	private:
		int oldTimeCounter[20],oldTime[20];
		int lastTime;
		ANARACE* anarace;
		int continueButton;
		int animation;
};

#endif

