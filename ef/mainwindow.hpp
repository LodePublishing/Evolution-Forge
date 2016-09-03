#ifndef _GUI_MAINWINDOW_HPP
#define _GUI_MAINWINDOW_HPP

#include "../ui/button.hpp"
#include "../ui/window.hpp"

class MainWindow : public UI_Window
{
	public:
		MainWindow();
		~MainWindow();
		void process();
		void draw( DC* dc ) const;
	private:
		UI_Button* tab[MAX_TABS];
};

#endif

