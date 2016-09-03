#include "corewindow.h"

CoreWindow::CoreWindow(UI_Window* parentWindow):UI_Window(parentWindow, CORE_WINDOW_TITLE_STRING, THE_CORE_WINDOW, NOT_SCROLLED)		
{
};

CoreWindow::~CoreWindow()
{
};

void CoreWindow::process()
{
	if(!isShown()) return;
	UI_Window::process();
};


void CoreWindow::draw(DC* dc)
{
    if(!isShown()) return;
	UI_Window::draw(dc);
};
