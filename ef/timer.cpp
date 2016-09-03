#include "timer.hpp"

#include <sstream>
#include <iomanip>

TimerWindow::TimerWindow(UI_Object* timer_parent, ANARACE* timer_anarace, const unsigned int timer_window_number):
	UI_Window(timer_parent, TIMER_WINDOW_TITLE_STRING, TIMER_WINDOW, timer_window_number, NOT_SCROLLED),
	currentTime(MAX_TIME-1),
	lastTime(MAX_TIME-1),
	mode(1),
	anarace(timer_anarace),
	goalsFulFilledText(new UI_StaticText(this, getRelativeClientRect(), IMPORTANT_COLOR, SMALL_NORMAL_BOLD_FONT, HORIZONTALLY_CENTERED_TEXT_MODE)),
	currentActionText(new UI_StaticText(this, getRelativeClientRect(), IMPORTANT_COLOR, LARGE_NORMAL_BOLD_FONT, UPPER_CENTERED_TEXT_MODE)),
	timeText(new UI_StaticText(this, getRelativeClientRect(), IMPORTANT_COLOR, VERY_LARGE_NORMAL_BOLD_FONT, TOTAL_CENTERED_TEXT_MODE)),
// TODO irgendwas stimmt hier mit der Hoehe nicht
	continueButton(new UI_Button(this, getRelativeClientRect(), CLICK_TO_CONTINUE_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, BOTTOM_CENTER, SMALL_NORMAL_BOLD_FONT, AUTO_HEIGHT_FULL_WIDTH))
{
	continueButton->updateToolTip(CONTINUE_OPTIMIZATION_TOOLTIP_STRING);
	goalsFulFilledText->updateToolTip(GOALS_FULFILLED_TOOLTIP_STRING);
	resetData(); // TODO
}

TimerWindow::~TimerWindow()
{
	delete(goalsFulFilledText);
	delete(currentActionText);
	delete(timeText);
	delete(continueButton);
}

void TimerWindow::assignAnarace(ANARACE* timer_anarace)
{
	anarace = timer_anarace;
}
																				
void TimerWindow::resetData()
{
	for(int i=20;i--;)
	{
		oldTimeCounter[i] = 0;
		oldTime[i] = MAX_TIME-1;
	}
	lastTime = MAX_TIME;
	currentTime = MAX_TIME;
}

void TimerWindow::forcePause(const bool pause)
{
	if(pause)
		continueButton->forcePress();
	else continueButton->forceUnpress();
}

void TimerWindow::process()
{
	if(!isShown()) 
		return;
	UI_Window::process();

	if(continueButton->isLeftClicked())
	{
		if(continueButton->isCurrentlyActivated())
		{
			anarace->setOptimizing(true);
			continueButton->updateText(CLICK_TO_PAUSE_STRING);
			continueButton->updateToolTip(PAUSE_OPTIMIZATION_TOOLTIP_STRING);
		}
		else
		{
			anarace->setOptimizing(false);
			continueButton->updateText(CLICK_TO_CONTINUE_STRING);
			continueButton->updateToolTip(CONTINUE_OPTIMIZATION_TOOLTIP_STRING);
		}
	}
	
	if(getCurrentMode()==1)
	{																	
		if(anarace->getTimer()==0)
		{
			goalsFulFilledText->updateText(theme.lookUpFormattedString(OF_GOALS_FULFILLED_STRING, anarace->getGoalPercentage()/100));
			if(!anarace->isOptimizing()) 
				currentActionText->updateText(PAUSED_STRING);
			else
				currentActionText->updateText(SEARCHING_STRING);
		}
		else
		{
			goalsFulFilledText->updateText(theme.lookUpFormattedString(OF_TIME_FULFILLED_STRING, anarace->getGoalPercentage()));
//			goalsFulFilledText->updateText(theme.lookUpFormattedString(OF_TIME_FULFILLED_STRING, anarace->getFastestGoalTime()));
			if(!anarace->isOptimizing())
				currentActionText->updateText(PAUSED_STRING);
			else
				currentActionText->updateText(OPTIMIZING_STRING);
		}
		if(anarace->getRealTimer()!=currentTime)
		{
			currentTime -= (currentTime - (anarace->getRealTimer()))/2;
		    	if(anarace->getRealTimer()<currentTime)
				currentTime--;
		}
		std::ostringstream os;
		if(currentTime >= 3600) // TODO ga->...
			os << "[--:--]";
		else
			os << "[" << formatTime(currentTime) << "]";
		timeText->updateText(os.str());
	}
	else if(getCurrentMode()==2)
	{
		goalsFulFilledText->updateText(theme.lookUpFormattedString(RES_UNITS_STRUCT_STRING, 0,0,0));
		if(!anarace->isOptimizing())
			currentActionText->updateText(PAUSED_STRING);
		else
			currentActionText->updateText(OPTIMIZING_STRING);
		timeText->updateText(theme.lookUpFormattedString(TOTAL_STRING, 123));
	}
}

void TimerWindow::setMode(const unsigned int current_mode) // for different tabs different behaviour
{
	mode = current_mode;
}

const unsigned int TimerWindow::getCurrentMode() const
{
	return(mode);
}

void TimerWindow::draw(DC* dc) const
{
	if(!isShown()) 
		return;
	UI_Window::draw(dc);
	dc->SetPen(*UI_Object::theme.lookUpPen(BODIAGRAM_FITNESS_PEN));
	dc->DrawHorizontalLine(getAbsolutePosition().x + 10, getAbsolutePosition().y + getHeight() - 30, getAbsolutePosition().x + 10 + ((getWidth()-35)*anarace->getUnchangedGenerations())  / configuration.getMaxGenerations() );
	ostringstream os;
	os << 100*anarace->getUnchangedGenerations()  / configuration.getMaxGenerations() << "%";
	dc->DrawText(os.str(), getAbsolutePosition() + Size(getWidth() - 25, getHeight() - 30));
}

