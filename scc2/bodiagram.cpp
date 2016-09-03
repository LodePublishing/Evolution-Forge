#include "bodiagram.h"

BoDiagramWindow::BoDiagramWindow(UI_Object* parent, wxRect rahmen, wxRect maxSize, ANARACE* anarace, InfoWindow* infoWindow):UI_Window(parent, BODIAGRAM_WINDOW_TITLE_STRING, rahmen,maxSize,NOT_SCROLLED)
{
	resetData();
	this->anarace=anarace;
	this->infoWindow=infoWindow;
};

BoDiagramWindow::~BoDiagramWindow()
{
};

void BoDiagramWindow::resetData()
{
};

void BoDiagramWindow::process()
{
	UI_Window::process();
};

void BoDiagramWindow::draw(wxDC* dc)
{
	UI_Window::draw(dc);
		wxPoint mins[MAX_TIME];
		wxPoint gas[MAX_TIME];
//	  wxPoint fitness[MAX_LENGTH];
		wxPoint hsupply[MAX_TIME];
		wxPoint nsupply[MAX_TIME];
		// TODO evtl einen gefuellten Graphen zwischen maxSupply und supply machen... => anarace setStatisticsSupply aendern
		int time;
		if(anarace->ga->maxTime-anarace->getTimer()<2) return;
		int count=0;


		if(infoWindow->isShown())
  			dc->SetBrush(wxBrush(wxColour(30,30,30),wxCROSS_HATCH));
		else
			  dc->SetBrush(wxBrush(wxColour(50,50,50),wxCROSS_HATCH));
		dc->DrawRectangle(getClientRectLeftBound(),getClientRectUpperBound()+10,getClientRectWidth(),getClientRectHeight()-10);

		if(insideClientRect(controls.getCurrentPosition()))
		{				
			int x=controls.getX()-2;
			if(x<getClientRectLeftBound())
				x=getClientRectLeftBound();
			if(x+4>=getClientRectLeftBound()+getClientRectWidth())
				x=getClientRectLeftBound()+getClientRectWidth()-5;
			dc->SetBrush(wxBrush(wxColour(40,40,40),wxSOLID));
			dc->DrawRectangle(x,getClientRectUpperBound()+10,4,getClientRectHeight()-10);
		}
																												                                            
		if(anarace->getTimer()==anarace->ga->maxTime) time=0;
				else time=anarace->getTimer()+1;
		int maxSupply=0;
		int maxMins=0;
		int maxGas=0;
		for(int i=time;i<anarace->ga->maxTime;i++)
		{
			if(anarace->getStatisticsHaveMinerals(i)>maxMins) maxMins=anarace->getStatisticsHaveMinerals(i);
			if(anarace->getStatisticsHaveGas(i)>maxGas) maxGas=anarace->getStatisticsHaveGas(i);
			if(anarace->getStatisticsNeedSupply(i)>maxSupply) maxSupply=anarace->getStatisticsNeedSupply(i);
			if(anarace->getStatisticsHaveSupply(i)>maxSupply) maxSupply=anarace->getStatisticsHaveSupply(i);
		};
			
		int s=anarace->ga->maxTime-1;
		while(s>=time)
		{
			int y1;
			if(maxMins) y1=anarace->getStatisticsHaveMinerals(s)*75/maxMins; else y1=0;
			mins[count]=wxPoint(getClientRectLeftBound()+3+((count*(getClientRectWidth()-6))/(anarace->ga->maxTime-time)),getClientRectUpperBound()+getClientRectHeight()-y1-2);
																												                                            
			if(maxGas) y1=anarace->getStatisticsHaveGas(s)*75/maxGas; else y1=0;
			gas[count]=wxPoint(getClientRectLeftBound()+3+((count*(getClientRectWidth()-6))/(anarace->ga->maxTime-time)),getClientRectUpperBound()+getClientRectHeight()-y1-2);
//TODO anarace->getMaxpFitness-getTimer kann auch 0 sein !!
																												                                            
/*		  y1=anarace->getStatisticsFitness(s)*75/(anarace->getMaxpFitness()-anarace->getTimer());
			fitness[count]=wxPoint(getClientRectLeftBound()+3+((s*(getClientRectWidth()-6))/(anarace->ga->maxTime-anarace->getTimer())),getClientRectUpperBound()+getClientRectHeight()-y1);*/
			if(maxSupply) y1=anarace->getStatisticsHaveSupply(s)*75/maxSupply; else y1=0;
			hsupply[count]=wxPoint(getClientRectLeftBound()+3+((count*(getClientRectWidth()-6))/(anarace->ga->maxTime-time)),getClientRectUpperBound()+getClientRectHeight()-y1-2);
			if(maxSupply) y1=anarace->getStatisticsNeedSupply(s)*75/maxSupply; else y1=0;
			nsupply[count]=wxPoint(getClientRectLeftBound()+3+((count*(getClientRectWidth()-6))/(anarace->ga->maxTime-time)),getClientRectUpperBound()+getClientRectHeight()-y1-2);
			count++;
			s--;
		}

		if(count>0)
		{
				dc->SetFont(*theme.lookUpFont(SMALL_ITALICS_BOLD_FONT));
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
//					  dc->DrawText(_T("Fitness"),getClientRectLeftBound()+1,getClientRectUpperBound()+8);
						dc->SetTextForeground(wxColour(60,60,200));
						dc->DrawText(_T("Minerals"),getClientRectLeftBound()+1,getClientRectUpperBound()+10);
						dc->SetTextForeground(wxColour(20,200,20));
						dc->DrawText(_T("Gas"),getClientRectLeftBound()+1,getClientRectUpperBound()+21);
						dc->SetTextForeground(wxColour(120,120,120));
						dc->DrawText(_T("Supply"),getClientRectLeftBound()+1,getClientRectUpperBound()+32);
				} else*/
//				{
						dc->SetTextForeground(wxColour(255,160,160));

						dc->SetPen(wxPen(wxColour(120,120,120),1,wxSOLID));
						for(int i=count;i--;)
						{
							if(i>0)
							{
								int k=hsupply[i].x;
								while(k<hsupply[i-1].x)
								{
									k++;
									dc->DrawLine(wxPoint(k,hsupply[i].x),wxPoint(k,nsupply[i].y));
								}
							}
							dc->DrawLine(hsupply[i],nsupply[i]);
						} // TODO
//						dc->DrawSpline(count,supply);
						dc->SetPen(wxPen(wxColour(80,80,255),2,wxSOLID));
						dc->DrawSpline(count,mins);
						dc->SetPen(wxPen(wxColour(40,255,40),2,wxSOLID));
						dc->DrawSpline(count,gas);
//					  dc->SetPen(wxPen(wxColour(255,40,40),2,wxSOLID));
//					  dc->DrawSpline(count,fitness);
//					  dc->SetTextForeground(wxColour(255,40,40));
//					  dc->DrawText(_T("Fitness"),getClientRectLeftBound()+1,getClientRectUpperBound()+8);
						dc->SetTextForeground(*theme.lookUpColour(MINERALS_TEXT_COLOUR));
						dc->DrawText(_T("Minerals"),getClientRectLeftBound()+1,getClientRectUpperBound()+10);
						dc->SetTextForeground(*theme.lookUpColour(GAS_TEXT_COLOUR));
						dc->DrawText(_T("Gas"),getClientRectLeftBound()+1,getClientRectUpperBound()+21);
						dc->SetTextForeground(*theme.lookUpColour(SUPPLY_TEXT_COLOUR));
						dc->DrawText(_T("Supply"),getClientRectLeftBound()+1,getClientRectUpperBound()+32);

			if(count>5)
			if(insideClientRect(controls.getCurrentPosition()))
			{

				dc->SetTextForeground(*theme.lookUpColour(BRIGHT_MINERALS_TEXT_COLOUR));
				dc->DrawText(_T(wxString::Format(wxT("%i"),

				anarace->getStatisticsHaveMinerals(anarace->ga->maxTime-(controls.getX()-getClientRectLeftBound())*(anarace->ga->maxTime-anarace->getTimer())/getClientRectWidth())/100)),
					getClientRectLeftBound()+50,getClientRectUpperBound()+10);
				
				dc->SetTextForeground(*theme.lookUpColour(BRIGHT_GAS_TEXT_COLOUR));
				dc->DrawText(_T(wxString::Format(wxT("%i"),
				
				anarace->getStatisticsHaveGas(anarace->ga->maxTime-(controls.getX()-getClientRectLeftBound())*(anarace->ga->maxTime-anarace->getTimer())/getClientRectWidth())/100)),
					getClientRectLeftBound()+50,getClientRectUpperBound()+21);
				
				dc->SetTextForeground(*theme.lookUpColour(BRIGHT_SUPPLY_TEXT_COLOUR));
				dc->DrawText(_T(wxString::Format(wxT("%i/%i"),
							
				anarace->getStatisticsNeedSupply(anarace->ga->maxTime-(controls.getX()-getClientRectLeftBound())*(anarace->ga->maxTime-anarace->getTimer())/getClientRectWidth()),anarace->getStatisticsHaveSupply(anarace->ga->maxTime-(controls.getX()-getClientRectLeftBound())*(anarace->ga->maxTime-anarace->getTimer())/getClientRectWidth()))),
				getClientRectLeftBound()+50,getClientRectUpperBound()+32);
			}
			dc->SetPen(*theme.lookUpPen(RECTANGLE_PEN));
		}
		if(infoWindow->isShown())
		{
				if(stats[anarace->getPlayer()->getRace()][infoWindow->getUnit()].supply)
				{
						dc->SetPen(*theme.lookUpPen(DASHED_SUPPLY_PEN));
						int y1=stats[anarace->getPlayer()->getRace()][infoWindow->getUnit()].supply*5;
						if(y1<0) y1=-y1;
						dc->DrawLine(getClientRectLeftBound()+infoWindow->getBx()+1,getClientRectUpperBound()+getClientRectHeight()-y1,getClientRectLeftBound()+infoWindow->getBx()+infoWindow->getBWidth()-1,getClientRectUpperBound()+getClientRectHeight()-y1);
				}

				if(stats[anarace->getPlayer()->getRace()][infoWindow->getUnit()].mins)
				{
						dc->SetPen(*theme.lookUpPen(DASHED_MINERALS_PEN));
						dc->DrawLine(getClientRectLeftBound()+infoWindow->getBx()+1,getClientRectUpperBound()+getClientRectHeight()-stats[anarace->getPlayer()->getRace()][infoWindow->getUnit()].mins/1000,getClientRectLeftBound()+infoWindow->getBx()+infoWindow->getBWidth()-1,getClientRectUpperBound()+getClientRectHeight()-stats[anarace->getPlayer()->getRace()][infoWindow->getUnit()].mins/1000);
				}

				if(stats[anarace->getPlayer()->getRace()][infoWindow->getUnit()].gas)
				{
						dc->SetPen(*theme.lookUpPen(DASHED_GAS_PEN));
						dc->DrawLine(getClientRectLeftBound()+infoWindow->getBx()+1,getClientRectUpperBound()+getClientRectHeight()-stats[anarace->getPlayer()->getRace()][infoWindow->getUnit()].gas/1000,getClientRectLeftBound()+infoWindow->getBx()+infoWindow->getBWidth()-1,getClientRectUpperBound()+getClientRectHeight()-stats[anarace->getPlayer()->getRace()][infoWindow->getUnit()].gas/1000);
				}
				dc->SetPen(*theme.lookUpPen(DASHED_MARKER_PEN));
				dc->DrawLine(getClientRectLeftBound()+infoWindow->getBx()+infoWindow->getBWidth(),getClientRectUpperBound()+10,getClientRectLeftBound()+infoWindow->getBx()+infoWindow->getBWidth(),getClientRectUpperBound()+getClientRectHeight());
				dc->DrawLine(getClientRectLeftBound()+infoWindow->getBx(),getClientRectUpperBound()+10,getClientRectLeftBound()+infoWindow->getBx(),getClientRectUpperBound()+getClientRectHeight());
		}
}

