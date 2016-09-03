#ifndef __TIMER_H
#define __TIMER_H

#include "UI_Window.h"
#include "UI_Button.h"
#include "UI_StaticText.h"
#include "../scc2dll/anarace.h"

class TimerWindow:public UI_Window
{
	public:
		TimerWindow(UI_Object* parent, wxRect rahmen, wxRect maxSize, ANARACE* anarace);
		~TimerWindow();

		void resetData();
		void process();
		void draw(wxDC* dc);

		int getCurrentMode();
		void setMode(int mode);
	private:
		int oldTimeCounter[20],oldTime[20];
		int lastTime;
		int mode;
		ANARACE* anarace;

		UI_StaticText* goalsFulFilledText;
		UI_StaticText* currentActionText;
		UI_StaticText* timeText;

		UI_Button* continueButton;
		ButtonAnimation* cbut;		
};

#endif

