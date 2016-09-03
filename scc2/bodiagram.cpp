#include "bodiagram.h"

BoDiagramWindow::BoDiagramWindow(UI_Object* parent, ANARACE* anarace, InfoWindow* infoWindow, const int windowNumber):UI_Window(parent, BODIAGRAM_WINDOW_TITLE_STRING, BO_DIAGRAM_WINDOW, windowNumber, NOT_SCROLLED)
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
	if(!shown) return;
	UI_Window::process();
};

void BoDiagramWindow::draw(DC* dc)
{
	if(!shown) return;
	UI_Window::draw(dc);
		Point mins[MAX_TIME];
		Point gas[MAX_TIME];
//	  Point fitness[MAX_LENGTH];
		Point hsupply[MAX_TIME];
		Point nsupply[MAX_TIME];
		// TODO evtl einen gefuellten Graphen zwischen maxSupply und supply machen... => anarace setStatisticsSupply aendern
		int time;
		if(anarace->ga->maxTime-anarace->getTimer()<2) return;
		int count=0;


		if(infoWindow->isShown())
  			dc->SetBrush(*theme.lookUpBrush(BODIAGRAM_BACK1));
		else
			  dc->SetBrush(*theme.lookUpBrush(BODIAGRAM_BACK2));
		dc->DrawRectangle(getAbsoluteClientRectPosition()+Point(0,10), getClientRectSize()+Size(0,-10));

		if(insideClientRect(controls.getCurrentPosition()))
		{				
			int x=controls.getX()-2;
			if(x<getAbsoluteClientRectLeftBound())
				x=getAbsoluteClientRectLeftBound();
			if(x+4>=getAbsoluteClientRectRightBound())
				x=getAbsoluteClientRectRightBound()-5;
			dc->SetBrush(*theme.lookUpBrush(BODIAGRAM_BAR));
			dc->DrawRectangle(x,getAbsoluteClientRectUpperBound()+10,4,getClientRectHeight()-10);
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
			mins[count]=Point(getAbsoluteClientRectLeftBound()+3+((count*(getClientRectWidth()-6))/(anarace->ga->maxTime-time)),getAbsoluteClientRectLowerBound()-y1-2);
																												                                            
			if(maxGas) y1=anarace->getStatisticsHaveGas(s)*75/maxGas; else y1=0;
			gas[count]=Point(getAbsoluteClientRectLeftBound()+3+((count*(getClientRectWidth()-6))/(anarace->ga->maxTime-time)),getAbsoluteClientRectLowerBound()-y1-2);
//TODO anarace->getMaxpFitness-getTimer kann auch 0 sein !!
																												                                            
/*		  y1=anarace->getStatisticsFitness(s)*75/(anarace->getMaxpFitness()-anarace->getTimer());
			fitness[count]=Point(getAbsoluteClientRectLeftBound()+3+((s*(getClientRectWidth()-6))/(anarace->ga->maxTime-anarace->getTimer())),getAbsoluteClientRectLowerBound()-y1);*/
			if(maxSupply) y1=anarace->getStatisticsHaveSupply(s)*75/maxSupply; else y1=0;
			hsupply[count]=Point(getAbsoluteClientRectLeftBound()+3+((count*(getClientRectWidth()-6))/(anarace->ga->maxTime-time)),getAbsoluteClientRectLowerBound()-y1-2);
			if(maxSupply) y1=anarace->getStatisticsNeedSupply(s)*75/maxSupply; else y1=0;
			nsupply[count]=Point(getAbsoluteClientRectLeftBound()+3+((count*(getClientRectWidth()-6))/(anarace->ga->maxTime-time)),getAbsoluteClientRectLowerBound()-y1-2);
			count++;
			s--;
		}

		if(count>0)
		{
				dc->SetFont(theme.lookUpFont(SMALL_ITALICS_BOLD_FONT));
/*			  if(infoWindowNumber)
				{
						dc->SetPen(Pen(dc->doColor(80,80,80),2,SOLID_PEN_STYLE));
						dc->DrawSpline(count,supply);
						dc->SetPen(Pen(dc->doColor(60,60,200),2,SOLID_PEN_STYLE));
						dc->DrawSpline(count,mins);
						dc->SetPen(Pen(dc->doColor(20,160,20),2,SOLID_PEN_STYLE));
						dc->DrawSpline(count,gas);
//					  dc->SetPen(Pen(dc->doColor(255,40,40),2,SOLID_PEN_STYLE));
//					  dc->DrawSpline(count,fitness);
//					  dc->SetTextForeground(dc->doColor(255,40,40));
//					  dc->DrawText(_T("Fitness"),getAbsoluteClientRectLeftBound()+1,getAbsoluteClientRectUpperBound()+8);
						dc->SetTextForeground(dc->doColor(60,60,200));
						dc->DrawText(_T("Minerals"),getAbsoluteClientRectLeftBound()+1,getAbsoluteClientRectUpperBound()+10);
						dc->SetTextForeground(dc->doColor(20,200,20));
						dc->DrawText(_T("Gas"),getAbsoluteClientRectLeftBound()+1,getAbsoluteClientRectUpperBound()+21);
						dc->SetTextForeground(dc->doColor(120,120,120));
						dc->DrawText(_T("Supply"),getAbsoluteClientRectLeftBound()+1,getAbsoluteClientRectUpperBound()+32);
				} else*/
//				{
//						dc->SetTextForeground(dc->doColor(255,160,160)); for fitness
						dc->SetPen(*theme.lookUpPen(BODIAGRAM_SUPPLY_PEN));
						for(int i=count;i--;)
						{
							if(i>0)
							{
								int k=hsupply[i].x;
								while(k<hsupply[i-1].x)
								{
									k++;
									dc->DrawLine(Point(k,hsupply[i].x),Point(k,nsupply[i].y));
								}
							}
							dc->DrawLine(hsupply[i],nsupply[i]);
						} // TODO
//						dc->DrawSpline(count,supply);
						dc->SetPen(*theme.lookUpPen(BODIAGRAM_MINERALS_PEN));
						dc->DrawSpline(count,mins);
						dc->SetPen(*theme.lookUpPen(BODIAGRAM_GAS_PEN));
						dc->DrawSpline(count,gas);
//					  dc->SetPen(Pen(dc->doColor(255,40,40),2,SOLID_PEN_STYLE));
//					  dc->DrawSpline(count,fitness);
//					  dc->SetTextForeground(dc->doColor(255,40,40));
//					  dc->DrawText(_T("Fitness"),getAbsoluteClientRectLeftBound()+1,getAbsoluteClientRectUpperBound()+8);
						dc->SetTextForeground(*theme.lookUpColor(MINERALS_TEXT_COLOUR));
						dc->DrawText("Minerals",getAbsoluteClientRectPosition()+Point(1,10));
						dc->SetTextForeground(*theme.lookUpColor(GAS_TEXT_COLOUR));
						dc->DrawText("Gas",getAbsoluteClientRectPosition()+Point(1,21));
						dc->SetTextForeground(*theme.lookUpColor(SUPPLY_TEXT_COLOUR));
						dc->DrawText("Supply",getAbsoluteClientRectPosition()+Point(1,32));

			if(count>5)
			if(insideClientRect(controls.getCurrentPosition()))
			{
				ostringstream os;
				dc->SetTextForeground(*theme.lookUpColor(BRIGHT_MINERALS_TEXT_COLOUR));
				os << anarace->getStatisticsHaveMinerals(anarace->ga->maxTime-(controls.getX()-getAbsoluteClientRectLeftBound())*(anarace->ga->maxTime-anarace->getTimer())/getClientRectWidth())/100;
				dc->DrawText(os.str(),getAbsoluteClientRectPosition()+Point(50,10));os.str("");
				
				dc->SetTextForeground(*theme.lookUpColor(BRIGHT_GAS_TEXT_COLOUR));
				
				os << anarace->getStatisticsHaveGas(anarace->ga->maxTime-(controls.getX()-getAbsoluteClientRectLeftBound())*(anarace->ga->maxTime-anarace->getTimer())/getClientRectWidth())/100;
				dc->DrawText(os.str(), getAbsoluteClientRectPosition()+Point(50,21));os.str("");
				
				dc->SetTextForeground(*theme.lookUpColor(BRIGHT_SUPPLY_TEXT_COLOUR));

				os << anarace->getStatisticsNeedSupply(anarace->ga->maxTime-(controls.getX()-getAbsoluteClientRectLeftBound())*(anarace->ga->maxTime-anarace->getTimer())/getClientRectWidth()) << ":" << anarace->getStatisticsHaveSupply(anarace->ga->maxTime-(controls.getX()-getAbsoluteClientRectLeftBound())*(anarace->ga->maxTime-anarace->getTimer())/getClientRectWidth());
				dc->DrawText(os.str(), getAbsoluteClientRectPosition()+Point(50,32));
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
						dc->DrawLine(getAbsoluteClientRectLeftBound()+infoWindow->getBx()+1,getAbsoluteClientRectLowerBound()-y1, getAbsoluteClientRectLeftBound()+infoWindow->getBx()+infoWindow->getBWidth()-1,getAbsoluteClientRectLowerBound()-y1);
				}

				if(stats[anarace->getPlayer()->getRace()][infoWindow->getUnit()].mins)
				{
						dc->SetPen(*theme.lookUpPen(DASHED_MINERALS_PEN));
						dc->DrawLine(getAbsoluteClientRectLeftBound()+infoWindow->getBx()+1,getAbsoluteClientRectLowerBound()-stats[anarace->getPlayer()->getRace()][infoWindow->getUnit()].mins/1000,getAbsoluteClientRectLeftBound()+infoWindow->getBx()+infoWindow->getBWidth()-1,getAbsoluteClientRectLowerBound()-stats[anarace->getPlayer()->getRace()][infoWindow->getUnit()].mins/1000);
				}

				if(stats[anarace->getPlayer()->getRace()][infoWindow->getUnit()].gas)
				{
						dc->SetPen(*theme.lookUpPen(DASHED_GAS_PEN));
						dc->DrawLine(getAbsoluteClientRectLeftBound()+infoWindow->getBx()+1,getAbsoluteClientRectLowerBound()-stats[anarace->getPlayer()->getRace()][infoWindow->getUnit()].gas/1000,getAbsoluteClientRectLeftBound()+infoWindow->getBx()+infoWindow->getBWidth()-1,getAbsoluteClientRectLowerBound()-stats[anarace->getPlayer()->getRace()][infoWindow->getUnit()].gas/1000);
				}
				dc->SetPen(*theme.lookUpPen(DASHED_MARKER_PEN));
				dc->DrawLine(getAbsoluteClientRectLeftBound()+infoWindow->getBx()+infoWindow->getBWidth(),getAbsoluteClientRectUpperBound()+10,getAbsoluteClientRectLeftBound()+infoWindow->getBx()+infoWindow->getBWidth(),getAbsoluteClientRectLowerBound());
				dc->DrawLine(getAbsoluteClientRectLeftBound()+infoWindow->getBx(),getAbsoluteClientRectUpperBound()+10,getAbsoluteClientRectLeftBound()+infoWindow->getBx(),getAbsoluteClientRectLowerBound());
		}
}

