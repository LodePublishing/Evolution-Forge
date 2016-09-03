#ifndef _GUI_COREWINDOW_HPP
#define _GUI_COREWINDOW_HPP

#include "../ui/window.hpp"

class CoreWindow : public UI_Window
{
	public:
		CoreWindow(UI_Window* parentWindow);
		~CoreWindow();
		void process();
		void draw(DC* dc) const; 
};

#endif // _GUI_COREWINDOW_HPP

