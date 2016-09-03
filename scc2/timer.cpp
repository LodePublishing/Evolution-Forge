#include "timer.h"
#include "UI_Theme.h"

TimerWindow::TimerWindow(UI_Object* parent, wxRect rahmen, wxRect maxSize, ANARACE* anarace):UI_Window(parent, TIMER_WINDOW_TITLE_STRING, rahmen,maxSize,NOT_SCROLLED)
{
	this->anarace=anarace;

	goalsFulFilledText = new UI_StaticText(this, getRelativeClientRect(), HORIZONTALLY_CENTERED_TEXT_MODE, SMALL_NORMAL_BOLD_FONT, IMPORTANT_COLOUR);
	currentActionText = new UI_StaticText(this, getRelativeClientRect(), UPPER_CENTERED_TEXT_MODE, LARGE_NORMAL_BOLD_FONT, IMPORTANT_COLOUR);
	timeText = new UI_StaticText(this, getRelativeClientRect(), LOWER_CENTERED_TEXT_MODE, VERY_LARGE_NORMAL_BOLD_FONT, IMPORTANT_COLOUR);

	continueButton = new UI_Button(this, getRelativeClientRect(), getRelativeClientRect(), CLICK_TO_CONTINUE_STRING, CLICK_TO_PAUSE_STRING, MY_BUTTON, STATIC_BUTTON_MODE, BOTTOM_CENTER, SMALL_NORMAL_BOLD_FONT, AUTO_HEIGHT_FULL_WIDTH);
			
	resetData();
};

TimerWindow::~TimerWindow()
{
	if(goalsFulFilledText) delete(goalsFulFilledText);
	if(currentActionText) delete(currentActionText);
	if(timeText) delete(timeText);
	if(continueButton) delete(continueButton);
//	if(cbut) delete(cbut);
};
																			    
void TimerWindow::resetData()
{
	for(int i=0;i<20;i++)
	{
		oldTimeCounter[i]=0;
		oldTime[i]=anarace->ga->maxTime;
	}
	lastTime=anarace->ga->maxTime;
	mode=1;
};

void TimerWindow::process()
{
	UI_Window::process();

	if(continueButton->isJustPressed() || continueButton->isJustReleased())
		anarace->setOptimizing(continueButton->isCurrentlyPressed());
//	else if(continueButton->isJustReleased())
//		anarace->setOptimizing(false);
	
	if(getCurrentMode()==1)
	{																    
/*		if(anarace->ga->maxTime-anarace->getTimer()<lastTime)
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
	
	lastTime=anarace->ga->maxTime-anarace->getTimer();
																			    
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
			dc->SetTextForeground(wxColour(160-oldTimeCounter[i]*8,0,0));
			dc->SetFont(*theme.lookUpFont(VERY_LARGE_NORMAL_BOLD_FONT));
//wxFont(30,wxDECORATIVE,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
																			    
			wxString bla=_T(wxString::Format(wxT("[%.2i:%.2i]"),oldTime[i]/60,oldTime[i]%60));
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
        goalsFulFilledText->updateText(theme.lookUpFormattedString(OF_GOALS_FULFILLED_STRING, anarace->getTimePercentage()));
        if(!anarace->isOptimizing())
            currentActionText->updateText(PAUSED_STRING);
        else
            currentActionText->updateText(OPTIMIZING_STRING);
		timeText->updateText(_T(wxString::Format(wxT("[%.2i:%.2i]"),(anarace->ga->maxTime-anarace->getTimer())/60,(anarace->ga->maxTime-anarace->getTimer())%60)));
	}

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


};

void TimerWindow::setMode(int mode) // for different tabs different behaviour
{
	this->mode=mode;
};

int TimerWindow::getCurrentMode()
{
	return(mode);
};

void TimerWindow::draw(wxDC* dc)
{
	UI_Window::draw(dc);
	if(!isShown())
		return;
}
