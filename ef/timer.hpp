#ifndef _GUI_TIMER_HPP
#define _GUI_TIMER_HPP

#include "../ui/window.hpp"

#include "../core/anarace.hpp"

class TimerWindow : public UI_Window
{
	public:
		TimerWindow(UI_Object* timer_parent, ANARACE* timer_anarace, const unsigned int timer_window_wumber);
		~TimerWindow();

		void resetData();
		void process();
		void draw(DC* dc) const;

		const unsigned int getCurrentMode() const;
		void setMode(const unsigned int current_mode);

		void forcePause(const bool pause=true);
		void assignAnarace(ANARACE* timer_anarace);
	private:
		unsigned int oldTimeCounter[20], oldTime[20];
		unsigned int currentTime;
		unsigned int lastTime;
		unsigned int mode;
		ANARACE* anarace;

		UI_StaticText* goalsFulFilledText;
		UI_StaticText* currentActionText;
		UI_StaticText* timeText;

		UI_Button* continueButton;
};

#endif

