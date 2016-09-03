#include "timer.hpp"

#include <sstream>
#include <iomanip>

TimerWindow::TimerWindow(UI_Object* timer_parent, ANARACE* timer_anarace, const unsigned int timer_window_number):
	UI_Window(timer_parent, TIMER_WINDOW_TITLE_STRING, TIMER_WINDOW, timer_window_number, NOT_SCROLLED),
	currentTime(MAX_TIME-1),
	lastTime(MAX_TIME-1),
	mode(1),
	anarace(timer_anarace),
	goalsFulFilledText(new UI_StaticText(this, getRelativeClientRect(), HORIZONTALLY_CENTERED_TEXT_MODE, IMPORTANT_COLOR, SMALL_NORMAL_BOLD_FONT)),
	currentActionText(new UI_StaticText(this, getRelativeClientRect(), UPPER_CENTERED_TEXT_MODE, IMPORTANT_COLOR, LARGE_NORMAL_BOLD_FONT)),
	timeText(new UI_StaticText(this, getRelativeClientRect(), TOTAL_CENTERED_TEXT_MODE, IMPORTANT_COLOR, VERY_LARGE_NORMAL_BOLD_FONT)),
// TODO irgendwas stimmt hier mit der Hoehe nicht
	continueButton(new UI_Button(this, getRelativeClientRect(), getRelativeClientRect(), CLICK_TO_CONTINUE_STRING, CLICK_TO_PAUSE_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, BOTTOM_CENTER, SMALL_NORMAL_BOLD_FONT, AUTO_HEIGHT_FULL_WIDTH))
{
	continueButton->updateToolTip("Continue the optimization");
	resetData(); // TODO
}

TimerWindow::~TimerWindow()
{
	delete(goalsFulFilledText);
	delete(currentActionText);
	delete(timeText);
	delete(continueButton);
}
																				
void TimerWindow::resetData()
{
	for(int i=20;i--;)
	{
		oldTimeCounter[i]=0;
		oldTime[i]=MAX_TIME-1;
	}
	lastTime=MAX_TIME-1;
	currentTime=MAX_TIME-1;
}

void TimerWindow::process()
{
	if(!isShown()) 
		return;
	UI_Window::process();

//	if(continueButton->isJustPressed() || continueButton->isJustReleased()) //~~
	anarace->setOptimizing(continueButton->isCurrentlyActivated());
	
	if(getCurrentMode()==1)
	{																	
/*		if(anarace->getRealTimer()<lastTime)
		{
			int i=0;
			while(oldTimeCounter[i]>0)
			{
				i++;
				if(i>19) i=0;
			}
			oldTimeCounter[i]=1;
			oldTime[i]=lastTime;
		}
	
	lastTime=anarace->getRealTimer();
																				
	for(int i=0;i<20;i++)
		for(int j=0;j<i;j++)
			if(oldTimeCounter[j]<oldTimeCounter[i])
			{
				int tee=oldTimeCounter[i];
				oldTimeCounter[i]=oldTimeCounter[j];
				oldTimeCounter[j]=tee;
				tee=oldTime[i];
				oldTime[i]=oldTime[j];
				oldTime[j]=tee;
			}
	for(int i=0;i<20;i++)
		if(oldTimeCounter[i]>0)
		{
			dc->SetTextForeground(Colour(160-oldTimeCounter[i]*8,0,0));
			dc->SetFont(*theme.lookUpFont(VERY_LARGE_NORMAL_BOLD_FONT));
//Font(30,DECORATIVE,NORMAL,BOLD,false,_T(""),FONTENCODING_DEFAULT));
																				
			String bla=_T(String::Format(T("[%.2i:%.2i]"),oldTime[i]/60,oldTime[i]%60));
			unsigned int dx,dy;
			dc->GetTextExtent(bla,&dx,&dy);
																				
			dc->DrawText(bla,getClientRectLeftBound()+(getClientRectWidth()-dx)/2+oldTimeCounter[i],getClientRectUpperBound()+(getClientRectHeight()-dy)/2-oldTimeCounter[i]/2);
																				
			oldTimeCounter[i]++;
			if(oldTimeCounter[i]>20)
			{
				oldTimeCounter[i]=0;
				oldTime[i]=0;
			}
		}*/
	if(anarace->getTimer()==0)
	{
		goalsFulFilledText->updateText(theme.lookUpFormattedString(OF_GOALS_FULFILLED_STRING, anarace->getGoalPercentage()));
		if(!anarace->isOptimizing()) 
			currentActionText->updateText(PAUSED_STRING);
		else
			currentActionText->updateText(SEARCHING_STRING);
	}
	else
	{
		goalsFulFilledText->updateText(theme.lookUpFormattedString(OF_TIME_FULFILLED_STRING, anarace->getGoalPercentage()));
		if(!anarace->isOptimizing())
			currentActionText->updateText(PAUSED_STRING);
		else
			currentActionText->updateText(OPTIMIZING_STRING);
	}
	if(anarace->getRealTimer()<currentTime)
	{
		currentTime -= (currentTime - (anarace->getRealTimer()))/2;
	    if(anarace->getRealTimer()<currentTime)
			currentTime--;
	}
	std::ostringstream os;
//	os << "[" << (anarace->getRealTimer())/60 << ":" << std::setfill('0') << setw(2) << (anarace->getRealTimer())%60 << "]";
	if(currentTime >= 3600) // TODO ga->...
	{
		os << "[--:--]";
	}
	else
		os << "[" << currentTime/60 << ":" << std::setfill('0') << std::setw(2) << currentTime%60 << "]";
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

void TimerWindow::setMode(const unsigned int mode) // for different tabs different behaviour
{
	this->mode=mode;
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
}

