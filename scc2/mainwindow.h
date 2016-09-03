#ifndef __MAIN_WINDOW_H
#define __MAIN_WINDOW_H

#include "UI_Button.h"
#include "UI_Window.h"

class MainWindow:public UI_Window
{
	public:
		MainWindow();
		~MainWindow();
		void process();
		void draw(DC* dc);	
	private:
		UI_Button* tab[MAX_TABS];
};

#endif

