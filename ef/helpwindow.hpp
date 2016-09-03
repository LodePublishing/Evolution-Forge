#ifndef _GUI_HELP_WINDOW_HPP
#define _GUI_HELP_WINDOW_HPP

#include "../ui/window.hpp"
#include "../ui/longtext.hpp"


class HelpWindow : public UI_Window
{
	public:
		HelpWindow(UI_Object* parent_window);
		~HelpWindow();
		void process();
		void draw() const;
		void reloadOriginalSize();
		void gotoChapter(unsigned int chapter);
		void goBack();
	private:
		eHelpChapter helpChapter; // ~~
		UI_LongText* text;
		UI_Button* indexButton;
		UI_Button* backButton;
		std::list<eHelpChapter> lastChapter;
};

#endif

