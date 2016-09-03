#ifndef _GUI_TUTORIAL_HPP
#define _GUI_TUTORIAL_HPP

#include "../ui/window.hpp"

class TutorialWindow : public UI_Window
{
	public:
		TutorialWindow(UI_Window* parentWindow);
		~TutorialWindow();
		void process();
		void draw(DC* dc) const;
	private:
		int tutorialChapter; // ~~
};

#endif

