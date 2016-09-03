#include "timer.hpp"

TimerWindow::TimerWindow(UI_Object* parent, ANARACE* anarace, const int windowNumber):UI_Window(parent, TIMER_WINDOW_TITLE_STRING, TIMER_WINDOW, windowNumber, NOT_SCROLLED)
{
	this->anarace=anarace;

	goalsFulFilledText = new UI_StaticText(this, getRelativeClientRect(), HORIZONTALLY_CENTERED_TEXT_MODE, IMPORTANT_COLOUR, SMALL_NORMAL_BOLD_FONT);
	currentActionText = new UI_StaticText(this, getRelativeClientRect(), UPPER_CENTERED_TEXT_MODE, IMPORTANT_COLOUR, LARGE_NORMAL_BOLD_FONT);
	timeText = new UI_StaticText(this, getRelativeClientRect(), TOTAL_CENTERED_TEXT_MODE, IMPORTANT_COLOUR, VERY_LARGE_NORMAL_BOLD_FONT);

// TODO irgendwas stimmt hier mit der Hoehe nicht
	continueButton = new UI_Button(this, getRelativeClientRect(), getRelativeClientRect(), CLICK_TO_CONTINUE_STRING, CLICK_TO_PAUSE_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, BOTTOM_CENTER, SMALL_NORMAL_BOLD_FONT, AUTO_HEIGHT_FULL_WIDTH);
	continueButton->updateToolTip("Continue the optimization");
			
	resetData();
	mode=1;
	currentTime=MAX_TIME;
}

TimerWindow::~TimerWindow()
{
	delete(goalsFulFilledText);
	delete(currentActionText);
	delete(timeText);
	delete(continueButton);
//	delete(cbut);
}
																				
void TimerWindow::resetData()
{
	for(int i=20;i--;)
	{
		oldTimeCounter[i]=0;
		oldTime[i]=MAX_TIME;
	}
	lastTime=MAX_TIME;
	currentTime=MAX_TIME;
}

void TimerWindow::process()
{
	if(!shown) return;
	UI_Window::process();

//	if(continueButton->isJustPressed() || continueButton->isJustReleased()) //~~
	anarace->setOptimizing(continueButton->isCurrentlyPressed());
	
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
			int dx,dy;
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
		goalsFulFilledText->updateText(theme.lookUpFormattedString(OF_GOALS_FULFILLED_STRING, anarace->getGoalPercentage()));
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
	ostringstream os;
//	os << "[" << (anarace->getRealTimer())/60 << ":" << setfill('0') << setw(2) << (anarace->getRealTimer())%60 << "]";
	os << "[" << currentTime/60 << ":" << setfill('0') << setw(2) << currentTime%60 << "]";
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

void TimerWindow::setMode(const int mode) // for different tabs different behaviour
{
	this->mode=mode;
}

const int TimerWindow::getCurrentMode() const
{
	return(mode);
}

void TimerWindow::draw(DC* dc) const
{
	if(!shown) return;
	UI_Window::draw(dc);
}

