#include "bodiagram.h"

BoDiagramWindow::BoDiagramWindow(wxRect rahmen, wxRect maxSize):GraphixScrollWindow(0,rahmen,maxSize,NOT_SCROLLED)
{
};

void BoDiagramWindow::setAnarace(ANARACE* anarace)
{
	this->anarace=anarace;
	resetData();
}

void BoDiagramWindow::resetData()
{
};

void BoDiagramWindow::assignInfoWindow(InfoWindow* infoWindow)
{
	this->infoWindow=infoWindow;
};
																												                                            
void BoDiagramWindow::showProgramGraph(wxDC* dc)
{
		wxPoint mins[MAX_TIME];
		wxPoint gas[MAX_TIME];
//	  wxPoint fitness[MAX_LENGTH];
		wxPoint supply[MAX_TIME];
		// TODO evtl einen gefuellten Graphen zwischen maxSupply und supply machen... => anarace setStatisticsSupply aendern
		int time;
		if(anarace->ga->maxTime-anarace->getTimer()<2) return;
		int count=0;


//		if(infoWindow->isShown())
  //			dc->SetBrush(wxBrush(wxColour(30,30,30),wxCROSS_HATCH));
	//	else
			  dc->SetBrush(wxBrush(wxColour(50,50,50),wxCROSS_HATCH));
		dc->DrawRectangle(getInnerLeftBound(),getInnerUpperBound()+10,getInnerWidth(),getInnerHeight()-10);

		if(insideClientArea(controls.getCurrentPosition()))
		{				
			int x=controls.getX()-2;
			if(x<getInnerLeftBound())
				x=getInnerLeftBound();
			if(x+4>=getInnerLeftBound()+getInnerWidth())
				x=getInnerLeftBound()+getInnerWidth()-5;
			dc->SetBrush(wxBrush(wxColour(40,40,40),wxSOLID));
			dc->DrawRectangle(x,getInnerUpperBound()+10,4,getInnerHeight()-10);
		}
		
																												                                            
		if(anarace->getTimer()==anarace->ga->maxTime) time=0;
				else time=anarace->getTimer()+1;
		int s=anarace->ga->maxTime-1;
		while(s>=time)
		{
		int y1;
				if(anarace->getStatisticsHaveMinerals(s)>75000) y1=75; else y1=anarace->getStatisticsHaveMinerals(s)/1000;
				mins[count]=wxPoint(getInnerLeftBound()+3+((count*(getInnerWidth()-6))/(anarace->ga->maxTime-time)),getInnerUpperBound()+getInnerHeight()-y1-2);
																												                                            
				if(anarace->getStatisticsHaveGas(s)>75000) y1=75; else y1=anarace->getStatisticsHaveGas(s)/1000;
				gas[count]=wxPoint(getInnerLeftBound()+3+((count*(getInnerWidth()-6))/(anarace->ga->maxTime-time)),getInnerUpperBound()+getInnerHeight()-y1-2);
//TODO anarace->getMaxpFitness-getTimer kann auch 0 sein !!
																												                                            
/*			  y1=anarace->getStatisticsFitness(s)*75/(anarace->getMaxpFitness()-anarace->getTimer());
				fitness[count]=wxPoint(getInnerLeftBound()+3+((s*(getInnerWidth()-6))/(anarace->ga->maxTime-anarace->getTimer())),getInnerUpperBound()+getInnerHeight()-y1);*/
																												                                            
				if(anarace->getStatisticsHaveSupply(s)-anarace->getStatisticsNeedSupply(s)>25) y1=75; else
				if(anarace->getStatisticsHaveSupply(s)<anarace->getStatisticsNeedSupply(s)) y1=0; else
				y1=(anarace->getStatisticsHaveSupply(s)-anarace->getStatisticsNeedSupply(s))*3;
																												                                            
				supply[count]=wxPoint(getInnerLeftBound()+3+((count*(getInnerWidth()-6))/(anarace->ga->maxTime-time)),getInnerUpperBound()+getInnerHeight()-y1-2);
				count++;
				s--;
		}

		if(count>0)
		{
				dc->SetFont(GraphixScrollWindow::font2);
/*			  if(infoWindowNumber)
				{
						dc->SetPen(wxPen(wxColour(80,80,80),2,wxSOLID));
						dc->DrawSpline(count,supply);
						dc->SetPen(wxPen(wxColour(60,60,200),2,wxSOLID));
						dc->DrawSpline(count,mins);
						dc->SetPen(wxPen(wxColour(20,160,20),2,wxSOLID));
						dc->DrawSpline(count,gas);
//					  dc->SetPen(wxPen(wxColour(255,40,40),2,wxSOLID));
//					  dc->DrawSpline(count,fitness);
//					  dc->SetTextForeground(wxColour(255,40,40));
//					  dc->DrawText(_T("Fitness"),getInnerLeftBound()+1,getInnerUpperBound()+8);
						dc->SetTextForeground(wxColour(60,60,200));
						dc->DrawText(_T("Minerals"),getInnerLeftBound()+1,getInnerUpperBound()+10);
						dc->SetTextForeground(wxColour(20,200,20));
						dc->DrawText(_T("Gas"),getInnerLeftBound()+1,getInnerUpperBound()+21);
						dc->SetTextForeground(wxColour(120,120,120));
						dc->DrawText(_T("Supply"),getInnerLeftBound()+1,getInnerUpperBound()+32);
				} else*/
//				{
						dc->SetTextForeground(wxColour(255,160,160));

						dc->SetPen(wxPen(wxColour(120,120,120),2,wxSOLID));
						dc->DrawSpline(count,supply);
						dc->SetPen(wxPen(wxColour(80,80,255),2,wxSOLID));
						dc->DrawSpline(count,mins);
						dc->SetPen(wxPen(wxColour(40,255,40),2,wxSOLID));
						dc->DrawSpline(count,gas);
//					  dc->SetPen(wxPen(wxColour(255,40,40),2,wxSOLID));
//					  dc->DrawSpline(count,fitness);
//					  dc->SetTextForeground(wxColour(255,40,40));
//					  dc->DrawText(_T("Fitness"),getInnerLeftBound()+1,getInnerUpperBound()+8);
						dc->SetTextForeground(wxColour(80,80,255));
						dc->DrawText(_T("Minerals"),getInnerLeftBound()+1,getInnerUpperBound()+10);
						dc->SetTextForeground(wxColour(40,255,40));
						dc->DrawText(_T("Gas"),getInnerLeftBound()+1,getInnerUpperBound()+21);
						dc->SetTextForeground(wxColour(160,160,160));
						dc->DrawText(_T("Supply"),getInnerLeftBound()+1,getInnerUpperBound()+32);

			if(count>5)
			if(insideClientArea(controls.getCurrentPosition()))
			{

				dc->SetTextForeground(wxColour(180,180,255));
				dc->DrawText(_T(wxString::Format(wxT("%i"),

				anarace->getStatisticsHaveMinerals(anarace->ga->maxTime-(controls.getX()-getInnerLeftBound())*(anarace->ga->maxTime-anarace->getTimer())/getInnerWidth())/100)),
					getInnerLeftBound()+50,getInnerUpperBound()+10);
				
				dc->SetTextForeground(wxColour(140,255,140));
				dc->DrawText(_T(wxString::Format(wxT("%i"),
				
				anarace->getStatisticsHaveGas(anarace->ga->maxTime-(controls.getX()-getInnerLeftBound())*(anarace->ga->maxTime-anarace->getTimer())/getInnerWidth())/100)),
					getInnerLeftBound()+50,getInnerUpperBound()+21);
				
				dc->SetTextForeground(wxColour(220,220,220));
				dc->DrawText(_T(wxString::Format(wxT("%i/%i"),
							
				anarace->getStatisticsNeedSupply(anarace->ga->maxTime-(controls.getX()-getInnerLeftBound())*(anarace->ga->maxTime-anarace->getTimer())/getInnerWidth()),anarace->getStatisticsHaveSupply(anarace->ga->maxTime-(controls.getX()-getInnerLeftBound())*(anarace->ga->maxTime-anarace->getTimer())/getInnerWidth()))),
				getInnerLeftBound()+50,getInnerUpperBound()+32);
			}
			dc->SetPen(wxPen(wxColour(PEN1R,PEN1G,PEN1B),1,wxSOLID));
		}
		if(infoWindow->isShown())
		{
				if(stats[anarace->getPlayer()->getRace()][infoWindow->getUnit()].supply)
				{
						dc->SetPen(wxPen(wxColour(160,160,160),2,wxSHORT_DASH));
			int y1=stats[anarace->getPlayer()->getRace()][infoWindow->getUnit()].supply*5;
			if(y1<0) y1=-y1;
						dc->DrawLine(getInnerLeftBound()+infoWindow->getBx()+1,getInnerUpperBound()+getInnerHeight()-y1,getInnerLeftBound()+infoWindow->getBx()+infoWindow->getBWidth()-1,getInnerUpperBound()+getInnerHeight()-y1);
				}

				if(stats[anarace->getPlayer()->getRace()][infoWindow->getUnit()].mins)
				{
						dc->SetPen(wxPen(wxColour(120,120,255),2,wxSHORT_DASH));
						dc->DrawLine(getInnerLeftBound()+infoWindow->getBx()+1,getInnerUpperBound()+getInnerHeight()-stats[anarace->getPlayer()->getRace()][infoWindow->getUnit()].mins/1000,getInnerLeftBound()+infoWindow->getBx()+infoWindow->getBWidth()-1,getInnerUpperBound()+getInnerHeight()-stats[anarace->getPlayer()->getRace()][infoWindow->getUnit()].mins/1000);
				}

				if(stats[anarace->getPlayer()->getRace()][infoWindow->getUnit()].gas)
				{
						dc->SetPen(wxPen(wxColour(80,255,80),2,wxSHORT_DASH));
						dc->DrawLine(getInnerLeftBound()+infoWindow->getBx()+1,getInnerUpperBound()+getInnerHeight()-stats[anarace->getPlayer()->getRace()][infoWindow->getUnit()].gas/1000,getInnerLeftBound()+infoWindow->getBx()+infoWindow->getBWidth()-1,getInnerUpperBound()+getInnerHeight()-stats[anarace->getPlayer()->getRace()][infoWindow->getUnit()].gas/1000);
				}

				dc->SetPen(wxPen(wxColour(150,0,0),2,wxSHORT_DASH));
				dc->DrawLine(getInnerLeftBound()+infoWindow->getBx()+infoWindow->getBWidth(),getInnerUpperBound()+10,getInnerLeftBound()+infoWindow->getBx()+infoWindow->getBWidth(),getInnerUpperBound()+getInnerHeight());
				dc->DrawLine(getInnerLeftBound()+infoWindow->getBx(),getInnerUpperBound()+10,getInnerLeftBound()+infoWindow->getBx(),getInnerUpperBound()+getInnerHeight());
		}
}

