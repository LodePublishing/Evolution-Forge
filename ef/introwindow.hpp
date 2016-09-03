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
		void draw() const;
		void reloadOriginalSize();
		void init();
		const unsigned int getGameType() const;
	private:
		bool was_initialized;
		UI_LongText* text;
		UI_Button* doneButton;
		unsigned int gameType;
};

inline const unsigned int IntroWindow::getGameType() const {
	return(gameType);
}

#endif

