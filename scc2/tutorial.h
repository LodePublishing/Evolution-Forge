#ifndef __TUTORIAL_H
#define __TUTORIAL_H

#include "UI_Window.h"

class TutorialWindow : public UI_Window
{
	public:
		TutorialWindow(UI_Window* parentWindow);
		~TutorialWindow();
		void process();
		void draw(DC* dc);
	private:
		int tutorialChapter; // ~~
};

#endif

