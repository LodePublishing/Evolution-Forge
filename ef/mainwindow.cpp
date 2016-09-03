#include "mainwindow.hpp"

MainWindow::MainWindow():UI_Window( NULL, MAIN_WINDOW_TITLE_STRING, MAIN_WINDOW, 0, NOT_SCROLLED, NO_AUTO_SIZE_ADJUST, TABBED )
{
// TODO: nach resolutions ordnen! *theme.lookUpRect etc. in data.txt eintragen
    tab[BASIC_TAB] = new UI_Button(this, Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), BASIC_TAB_STRING, BASIC_TAB_STRING, TAB_BUTTON, TOTAL_CENTERED_TEXT_MODE, TAB_BUTTON_MODE, ARRANGE_TOP_LEFT, MIDDLE_NORMAL_BOLD_FONT, CONSTANT_SIZE);
    tab[ADVANCED_TAB] = new UI_Button(this, Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), ADVANCED_TAB_STRING, ADVANCED_TAB_STRING, TAB_BUTTON, TOTAL_CENTERED_TEXT_MODE, TAB_BUTTON_MODE, ARRANGE_TOP_LEFT, MIDDLE_NORMAL_BOLD_FONT, CONSTANT_SIZE);
    tab[EXPERT_TAB] = new UI_Button(this, Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), EXPERT_TAB_STRING, EXPERT_TAB_STRING, TAB_BUTTON, TOTAL_CENTERED_TEXT_MODE, TAB_BUTTON_MODE, ARRANGE_TOP_LEFT, MIDDLE_NORMAL_BOLD_FONT, CONSTANT_SIZE);
    tab[GOSU_TAB] = new UI_Button(this, Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), GOSU_TAB_STRING, GOSU_TAB_STRING, TAB_BUTTON, TOTAL_CENTERED_TEXT_MODE, TAB_BUTTON_MODE, ARRANGE_TOP_LEFT, MIDDLE_NORMAL_BOLD_FONT, CONSTANT_SIZE);
    tab[TRANSCENDEND_TAB] = new UI_Button(this, Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), TRANSCENDEND_TAB_STRING, TRANSCENDEND_TAB_STRING, TAB_BUTTON, TOTAL_CENTERED_TEXT_MODE, TAB_BUTTON_MODE, ARRANGE_TOP_LEFT, MIDDLE_NORMAL_BOLD_FONT, CONSTANT_SIZE);

	tab[MAP_TAB] = new UI_Button(this, Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), MAP_TAB_STRING, MAP_TAB_STRING, TAB_BUTTON, TOTAL_CENTERED_TEXT_MODE, TAB_BUTTON_MODE, ARRANGE_TOP_LEFT, MIDDLE_NORMAL_BOLD_FONT, CONSTANT_SIZE);

	tab[SETTINGS_TAB] = new UI_Button(this, Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), SETTINGS_TAB_STRING, SETTINGS_TAB_STRING, TAB_BUTTON, TOTAL_CENTERED_TEXT_MODE, TAB_BUTTON_MODE, ARRANGE_TOP_LEFT, MIDDLE_NORMAL_BOLD_FONT, CONSTANT_SIZE);

	tab[TUTORIAL_TAB] = new UI_Button(this, Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), TUTORIAL_TAB_STRING, TUTORIAL_TAB_STRING, TAB_BUTTON, TOTAL_CENTERED_TEXT_MODE, TAB_BUTTON_MODE, ARRANGE_TOP_LEFT, MIDDLE_NORMAL_BOLD_FONT, CONSTANT_SIZE);

    tab[BASIC_TAB]->forcePress();
	int step=theme.lookUpButtonAnimation(TAB_BUTTON)->speed/(MAX_TABS-1);

	for(int i=BASIC_TAB;i<MAX_TABS;i++)
	{
		tab[i]->updateToolTip(*theme.lookUpString((eString)(BASIC_TAB_TOOLTIP_STRING+i-1)));
		addTab(tab[i]);
		tab[i]->setFrameNumber((i-1)*step);
	}
	for(int i = ADVANCED_TAB;i<=TUTORIAL_TAB;i++)
		tab[i]->Disable();
}


MainWindow::~MainWindow()
{
	for(int i=BASIC_TAB;i<MAX_TABS;i++)
		delete tab[i];
}

void MainWindow::process()
{
	UI_Object::rectnumber=0;
	UI_Object::hideToolTip();
	UI_Window::process();
}

void MainWindow::draw(DC* dc) const
{
	UI_Window::draw(dc);
	UI_Object::maybeShowToolTip(dc);
}


