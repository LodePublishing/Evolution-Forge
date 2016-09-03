#ifndef _GUI_INTRO_WINDOW_HPP
#define _GUI_INTRO_WINDOW_HPP

#include "../ui/window.hpp"
#include "../ui/longtext.hpp"

class IntroWindow : public UI_Window
{
	public:
		IntroWindow(UI_Object* intro_parent);
		~IntroWindow();
		void process();
		void draw(DC* dc) const;
		void reloadOriginalSize();
		void gotoChapter(unsigned int chapter);

	private:
		UI_LongText* text;
		UI_Button* doneButton;
};

#endif

