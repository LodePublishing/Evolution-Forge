#ifndef __TIMER_H
#define __TIMER_H

#include "UI_Window.h"
#include "UI_Button.h"
#include "UI_StaticText.h"
#include "../scc2dll/anarace.h"

class TimerWindow:public UI_Window
{
	public:
		TimerWindow(UI_Object* parent, ANARACE* anarace, const int windowNumber);
		~TimerWindow();

		void resetData();
		void process();
		void draw(DC* dc);

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

