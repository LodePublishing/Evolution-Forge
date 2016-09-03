#include "mainwindow.hpp"
#include "configuration.hpp"

MainWindow::MainWindow() : 
	UI_Window( NULL, MAIN_WINDOW_TITLE_STRING, MAIN_WINDOW, 0, NOT_SCROLLED, NO_AUTO_SIZE_ADJUST, TABBED ),
	ani(1),
	ani2(0),
	gizmo(true)
{
// TODO: nach resolutions ordnen! *theme.lookUpRect etc. in data.txt eintragen
// left:
	tab[BASIC_TAB] = new UI_Button(this, Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), BASIC_TAB_STRING, TAB_BUTTON, TOTAL_CENTERED_TEXT_MODE, TAB_BUTTON_MODE, ARRANGE_TOP_LEFT, MIDDLE_NORMAL_BOLD_FONT, CONSTANT_SIZE);
	tab[ADVANCED_TAB] = new UI_Button(this, Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), ADVANCED_TAB_STRING, TAB_BUTTON, TOTAL_CENTERED_TEXT_MODE, TAB_BUTTON_MODE, ARRANGE_TOP_LEFT, MIDDLE_NORMAL_BOLD_FONT, CONSTANT_SIZE);
	tab[EXPERT_TAB] = new UI_Button(this, Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), EXPERT_TAB_STRING, TAB_BUTTON, TOTAL_CENTERED_TEXT_MODE, TAB_BUTTON_MODE, ARRANGE_TOP_LEFT, MIDDLE_NORMAL_BOLD_FONT, CONSTANT_SIZE);
	tab[GOSU_TAB] = new UI_Button(this, Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), GOSU_TAB_STRING, TAB_BUTTON, TOTAL_CENTERED_TEXT_MODE, TAB_BUTTON_MODE, ARRANGE_TOP_LEFT, MIDDLE_NORMAL_BOLD_FONT, CONSTANT_SIZE);

// right:
	tab[COMPARE_TAB] = new UI_Button(this, Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), COMPARE_TAB_STRING, TAB_BUTTON, TOTAL_CENTERED_TEXT_MODE, TAB_BUTTON_MODE, ARRANGE_TOP_RIGHT, MIDDLE_NORMAL_BOLD_FONT, CONSTANT_SIZE);
	tab[TUTORIAL_TAB] = new UI_Button(this, Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), TUTORIAL_TAB_STRING, TAB_BUTTON, TOTAL_CENTERED_TEXT_MODE, TAB_BUTTON_MODE, ARRANGE_TOP_RIGHT, MIDDLE_NORMAL_BOLD_FONT, CONSTANT_SIZE);
	tab[SETTINGS_TAB] = new UI_Button(this, Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), SETTINGS_TAB_STRING, TAB_BUTTON, TOTAL_CENTERED_TEXT_MODE, TAB_BUTTON_MODE, ARRANGE_TOP_RIGHT, MIDDLE_NORMAL_BOLD_FONT, CONSTANT_SIZE);
	tab[MAP_TAB] = new UI_Button(this, Rect(getRelativePosition()+Size(10,0), Size(getWidth()-20, 20)), MAP_TAB_STRING, TAB_BUTTON, TOTAL_CENTERED_TEXT_MODE, TAB_BUTTON_MODE, ARRANGE_TOP_RIGHT, MIDDLE_NORMAL_BOLD_FONT, CONSTANT_SIZE);

	int step=theme.lookUpButtonAnimation(TAB_BUTTON)->speed/(MAX_TABS-1);
	for(int i=BASIC_TAB;i<MAX_TABS;i++)
	{
		tab[i]->updateToolTip((eString)(BASIC_TAB_TOOLTIP_STRING+i-1));
		addTab(tab[i]);
		tab[i]->setFrameNumber((i-1)*step);
	}

	tab[EXPERT_TAB]->Disable(); // TODO
	tab[GOSU_TAB]->Disable();
	tab[TUTORIAL_TAB]->Disable();
	tab[MAP_TAB]->Disable();
//	tab[COMPARE_TAB]->Disable();
}


MainWindow::~MainWindow()
{
	for(int i=BASIC_TAB;i<MAX_TABS;i++)
		delete tab[i];
}

void MainWindow::process()
{
//	UI_Object::rectnumber=0;
	clearRedrawFlag();
	UI_Window::process();
	if(tabRow->checkForNeedRedraw())
		setNeedRedrawNotMoved();
}

void MainWindow::continueOptimizationAnimation(const bool running)
{
	if(running)
	{
		if(ani2>1)
		{
			ani++;
			ani2 = 0;
		} else ani2++;
	}
	else ani = 1;
	if(ani>30) ani = 1;
}

const Size MainWindow::helper(DC* dc, const unsigned int dx, const int i, const std::string& str) const
{
	dc->SetTextForeground(DC::toSDL_Color(
				(Uint8)((0==ani%(20+i))*35+((0==ani%(19+i))+(0==ani%(21+i)))*15+20),
				(Uint8)((0==ani%(20+i))*35+((0==ani%(19+i))+(0==ani%(21+i)))*15+20),
				(Uint8)((0==ani%(20+i))*35+((0==ani%(19+i))+(0==ani%(21+i)))*30+60)));
	dc->DrawText(str.substr(str.size()-1, str.size()), getAbsoluteClientRectPosition()+Point(20+dx,20));
	return(dc->GetTextExtent(str.c_str()));
}

void MainWindow::draw(DC* dc) const
{
//jedem player ein mainwindow zuweisen!
	UI_Window::draw(dc);

	dc->SetFont(UI_Object::theme.lookUpFont(HUGE_DEFAULT_BOLD_FONT));
	std::string str="Evolution";
	Size s;
	for(unsigned int i=0;i<str.size();i++)
		s = helper(dc, s.GetWidth(), i, str.substr(0, i+1));

	dc->SetTextForeground(DC::toSDL_Color(25, 25, 85));
	dc->DrawText("Forge", getAbsoluteClientRectPosition() + Point(50, 58));
	dc->SetTextForeground(DC::toSDL_Color(0,0,85));
	dc->DrawText(CORE_VERSION, getAbsoluteClientRectPosition()+Point(78, 98));
	dc->SetTextForeground(DC::toSDL_Color(50, 50, 85));
	dc->DrawText(CORE_VERSION, getAbsoluteClientRectPosition()+Point(75, 95));
//	if(UI_Object::tooltip)
//		UI_Object::tooltip->draw(dc);
		// ------ MOUSE DRAWING ------
		if(efConfiguration.isSoftwareMouse())
		{
//			SDL_ShowCursor(SDL_DISABLE);
			Point p = UI_Object::mouse - Size(20,10);//Point(90, 140);
			dc->SetFont(UI_Object::theme.lookUpFont(SMALL_ITALICS_BOLD_FONT));
			switch(UI_Object::mouseType)
			{
				case 0://dc->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_NONE), p);
					break;
				case 1:
//					dc->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_LEFT), p);
					dc->SetTextForeground(DC::toSDL_Color(179,0,0));
					dc->DrawText("Add a unit", p.x-50, p.y+2);
				break;
				case 2:
//					dc->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_RIGHT), p);
					dc->SetTextForeground(DC::toSDL_Color(0,177,188));
					dc->DrawText("Remove a unit", p.x+38, p.y+1);
				break;
				case 3:
//					dc->DrawBitmap(*UI_Object::theme.lookUpBitmap(MOUSE_BOTH), p);
					dc->SetTextForeground(DC::toSDL_Color(179,0,0));
					dc->DrawText("Add a unit", p.x-50, p.y+2);
					dc->SetTextForeground(DC::toSDL_Color(0,177,188));
					dc->DrawText("Remove a unit", p.x+38, p.y+1);
				break;
			}
		}
//		else
//			SDL_ShowCursor(SDL_ENABLE);
// ------ END MOUSE DRAWING ------
}


