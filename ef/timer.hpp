#ifndef _GUI_TIMER_HPP
#define _GUI_TIMER_HPP

#include "../ui/window.hpp"

#include "../core/anarace.hpp"

class TimerWindow : public UI_Window
{
	public:
		TimerWindow(UI_Object* parent, ANARACE* anarace, const int windowNumber);
		~TimerWindow();

		void resetData();
		void process();
		void draw(DC* dc) const;

		const int getCurrentMode() const;
		void setMode(const int mode);
	private:
		int oldTimeCounter[20],oldTime[20];
		int currentTime;
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

