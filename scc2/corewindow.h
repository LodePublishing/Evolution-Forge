#ifndef __COREWINDOW_H
#define __COREWINDOW_H

#include "UI_Window.h"

class CoreWindow : public UI_Window
{
	public:
		CoreWindow(UI_Window* parentWindow);
		~CoreWindow();
		void process();
		void draw(DC* dc);
};

#endif

