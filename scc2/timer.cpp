#include "timer.h"

void TimerWindow::setAnarace(ANARACE* anarace)
{
	this->anarace=anarace;
};

TimerWindow::TimerWindow(wxRect rahmen, wxRect maxSize):GraphixScrollWindow(0,rahmen,maxSize,NOT_SCROLLED)
{
	resetData();
};
																			    
void TimerWindow::resetData()
{
	for(int i=0;i<20;i++)
	{
		oldTimeCounter[i]=0;
		oldTime[i]=anarace->ga->maxTime;
	}
	lastTime=anarace->ga->maxTime;
	continueButton=99999;
	animation=0;
};

void TimerWindow::processButtons()
{
	checkButtons();
	if(isActivated(continueButton))
	{
		if(anarace->isOptimizing())
			anarace->setOptimizing(0);
		else anarace->setOptimizing(1);
//TODO evtl irgendwie reset der anderen, v.a. boInsertPoint etc.
	}
	clearButtons();
};
																			    
																			    
void TimerWindow::drawTimer(wxDC* dc)
{
//      dc->DrawBitmap(bmpTimer,THIRD_COLOUMN-10,0,false);
			
	if(isShown()==1)
	{																    
	if(anarace->ga->maxTime-anarace->getTimer()<lastTime)
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
			dc->SetFont(GraphixScrollWindow::font4);
//wxFont(30,wxDECORATIVE,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
																			    
			wxString bla=_T(wxString::Format(wxT("[%.2i:%.2i]"),oldTime[i]/60,oldTime[i]%60));
			int dx,dy;
			dc->GetTextExtent(bla,&dx,&dy);
																			    
			dc->DrawText(bla,getInnerLeftBound()+(getInnerWidth()-dx)/2+oldTimeCounter[i],getInnerUpperBound()+(getInnerHeight()-dy)/2-oldTimeCounter[i]/2);
																			    
			oldTimeCounter[i]++;
			if(oldTimeCounter[i]>20)
			{
				oldTimeCounter[i]=0;
				oldTime[i]=0;
			}
		}
	wxString bla;
	wxString bla2;
	dc->SetFont(GraphixScrollWindow::font5);
//wxFont(24,wxDECORATIVE,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
	dc->SetTextForeground(wxColour(200,0,0));
	if(anarace->getTimer()==0)
	{
		int count=0;
		for(int i=0;i<MAX_GOALS;i++)
			if(anarace->getPlayer()->getGoal()->goal[i].count>0)
				count++;
		int percentage=0;
		if(count>0)
			percentage=anarace->getCurrentpFitness()/count;
		else percentage=100;

                dc->SetFont(GraphixScrollWindow::font6);
//wxFont(12,wxDECORATIVE,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
                dc->DrawText(_T(wxString::Format(wxT("%i%% of goals fulfilled"),percentage)),getInnerLeftBound(),getInnerUpperBound());
		dc->SetFont(GraphixScrollWindow::font5);
          
		if(!anarace->isOptimizing()) bla=_T("(paused)");
		else
			bla=_T("Searching...");
		int dx,dy;
		dc->GetTextExtent(bla,&dx,&dy);
		dc->DrawText(bla,getInnerLeftBound()+(getInnerWidth()-dx)/2,getInnerUpperBound()+(getInnerHeight()-dy)/2);
	}
	else
	{
		dc->SetFont(GraphixScrollWindow::font6);
//wxFont(12,wxDECORATIVE,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
		dc->DrawText(_T(wxString::Format(wxT("%i%% theoretical optimum"),anarace->getPercentage())),getInnerLeftBound(),getInnerUpperBound());
		dc->SetFont(GraphixScrollWindow::font5);
//wxFont(24,wxDECORATIVE,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
		bla=_T(wxString::Format(wxT("[%.2i:%.2i]"),(anarace->ga->maxTime-anarace->getTimer())/60,(anarace->ga->maxTime-anarace->getTimer())%60));
		if(anarace->isOptimizing())
			bla2=_T("Optimizing...");
		else bla2=_T("(paused)");
		int dx,dy,yy;
		dc->GetTextExtent(bla,&dx,&dy);
		yy=(getInnerHeight()-dy)/3;
		dc->DrawText(bla,getInnerLeftBound()+(getInnerWidth()-dx)/2,getInnerUpperBound()+yy);
		dc->GetTextExtent(bla2,&dx,&dy);
		dc->DrawText(bla2,getInnerLeftBound()+(getInnerWidth()-dx)/2,getInnerUpperBound()+2*yy);
	}
	}
	else if(isShown()==2)
	{
	        wxString bla;
        	wxString bla2;
//wxFont(24,wxDECORATIVE,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
	       	dc->SetTextForeground(wxColour(200,0,0));
                dc->SetFont(GraphixScrollWindow::font6);
//wxFont(12,wxDECORATIVE,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
       	        dc->DrawText(_T(wxString::Format(wxT("%i/%i/%i res./units/struc."),0,0,0)),getInnerLeftBound(),getInnerUpperBound());
               	dc->SetFont(GraphixScrollWindow::font5);
//wxFont(24,wxDECORATIVE,wxNORMAL,wxBOLD,false,_T(""),wxFONTENCODING_DEFAULT));
                bla=_T(wxString::Format(wxT("Total: %.2i"),123));
       	        if(anarace->isOptimizing())
               	        bla2=_T("Optimizing...");
                else bla2=_T("(paused)");
       	        int dx,dy,yy;
              	dc->GetTextExtent(bla,&dx,&dy);
                yy=(getInnerHeight()-dy)/3;
       	        dc->DrawText(bla,getInnerLeftBound()+(getInnerWidth()-dx)/2,getInnerUpperBound()+yy);
               	dc->GetTextExtent(bla2,&dx,&dy);
                dc->DrawText(bla2,getInnerLeftBound()+(getInnerWidth()-dx)/2,getInnerUpperBound()+2*yy);
       	}

	{
		wxRect edge=wxRect(getInnerLeftBound()+getInnerWidth()-180,getInnerUpperBound()+getInnerHeight()-20,160,FONT_SIZE+10);
		dc->SetPen(wxPen(wxColour(100,100,255),1,wxSOLID));
		if(!anarace->isOptimizing())
		{
			animation++;if(animation>50) animation=0;
		} else animation=49;
		dc->SetBrush(wxBrush(wxColour(100-(animation%25)*2,100-(animation%25)*2,200-(animation%50)*2),wxSOLID));
		dc->DrawRoundedRectangle(edge,3);
		dc->SetTextForeground(wxColour(255,255,255));
		dc->SetFont(GraphixScrollWindow::font2);
		if(!anarace->isOptimizing())
			dc->DrawText(_T("Click to continue"),edge.x+10,edge.y+3);
		else 
			dc->DrawText(_T("Click to pause"),edge.x+10,edge.y+3);
		continueButton=addButton(edge);
	}
}
