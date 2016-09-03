#include "bodiagram.hpp"

BoDiagramWindow::BoDiagramWindow(UI_Object* parent, ANARACE* anarace, InfoWindow* infoWindow, const int windowNumber):UI_Window(parent, BODIAGRAM_WINDOW_TITLE_STRING, BO_DIAGRAM_WINDOW, windowNumber, NOT_SCROLLED)
{
	resetData();
	this->anarace=anarace;
	this->infoWindow=infoWindow;
	for(int i=MAX_TIME;i--;)
	{
		minerals[i]=Point(0,0);
		gas[i]=Point(0,0);
		hneedSupply[i]=Point(0,0);
		nneedSupply[i]=Point(0,0);

		startMinerals[i]=Point(0,0);
		startGas[i]=Point(0,0);
		startHneedSupply[i]=Point(0,0);
		startNneedSupply[i]=Point(0,0);

		targetMinerals[i]=Point(0,0);
		targetGas[i]=Point(0,0);
		targetHneedSupply[i]=Point(0,0);
		targetNneedSupply[i]=Point(0,0);
	}
}

BoDiagramWindow::~BoDiagramWindow()
{
}

void BoDiagramWindow::resetData()
{
}

void BoDiagramWindow::process()
{
	if(!shown) return;
	UI_Window::process();

	int time;
	count=0;
	if(anarace->getRealTimer()<2) return;
	
	if(anarace->getTimer()==settings.getGa()->maxTime) time=0;
		else time=anarace->getTimer()+1;

	int haveSupply=0;
	int maxMins=0;
	int maxGas=0;
	for(int i=time;i<settings.getGa()->maxTime;i++)
	{
		if(anarace->getStatisticsHaveMinerals(i)>maxMins) maxMins=anarace->getStatisticsHaveMinerals(i);
		if(anarace->getStatisticsHaveGas(i)>maxGas) maxGas=anarace->getStatisticsHaveGas(i);
		if(anarace->getStatisticsNeedSupply(i)>haveSupply) haveSupply=anarace->getStatisticsNeedSupply(i);
		if(anarace->getStatisticsHaveSupply(i)>haveSupply) haveSupply=anarace->getStatisticsHaveSupply(i);
	}
		
	int s=settings.getGa()->maxTime-1;
	while(s>=time)
	{
		int y1;
		Point p;
		if(maxMins) y1=anarace->getStatisticsHaveMinerals(s)*75/maxMins; else y1=0;
		p=Point(getAbsoluteClientRectLeftBound()+3+((count*(getClientRectWidth()-6))/(settings.getGa()->maxTime-time)), getAbsoluteClientRectLowerBound()-y1-2);
		if(p!=targetMinerals[count]) {
			targetMinerals[count]=p;
			startMinerals[count]=minerals[count];
		}
																																						
		if(maxGas) y1=anarace->getStatisticsHaveGas(s)*75/maxGas; else y1=0;
		p=Point(getAbsoluteClientRectLeftBound()+3+((count*(getClientRectWidth()-6))/(settings.getGa()->maxTime-time)),getAbsoluteClientRectLowerBound()-y1-2);
		if(p!=targetGas[count]) {
			targetGas[count]=p;
			startGas[count]=gas[count];
		}
	
//TODO anarace->getMaxpFitness-getTimer kann auch 0 sein !!
																																						
/*		  y1=anarace->getStatisticsFitness(s)*75/(anarace->getMaxpFitness()-anarace->getTimer());
		fitness[count]=Point(getAbsoluteClientRectLeftBound()+3+((s*(getClientRectWidth()-6))/(anarace->getRealTimer())),getAbsoluteClientRectLowerBound()-y1);*/
		if(haveSupply) y1=anarace->getStatisticsHaveSupply(s)*75/haveSupply; else y1=0;
		p=Point(getAbsoluteClientRectLeftBound()+3+((count*(getClientRectWidth()-6))/(settings.getGa()->maxTime-time)),getAbsoluteClientRectLowerBound()-y1-2);
		if(p!=targetHneedSupply[count]) {
			targetHneedSupply[count]=p;
			startHneedSupply[count]=hneedSupply[count];
		}
		
		if(haveSupply) y1=anarace->getStatisticsNeedSupply(s)*75/haveSupply; else y1=0;
		p=Point(getAbsoluteClientRectLeftBound()+3+((count*(getClientRectWidth()-6))/(settings.getGa()->maxTime-time)),getAbsoluteClientRectLowerBound()-y1-2);
		if(p!=targetNneedSupply[count]) {
			targetNneedSupply[count]=p;
			startNneedSupply[count]=nneedSupply[count];
		}
	
		count++;
		s--;
	}

	for(int i=count;i--;)
	{
		move(minerals[i].y, startMinerals[i].y, targetMinerals[i].y);
		move(gas[i].y, startGas[i].y, targetGas[i].y);
		move(hneedSupply[i].y, startHneedSupply[i].y, targetHneedSupply[i].y);
		move(nneedSupply[i].y, startNneedSupply[i].y, targetNneedSupply[i].y);
		minerals[i].x=targetMinerals[i].x;
		gas[i].x=targetGas[i].x;
		hneedSupply[i].x=targetHneedSupply[i].x;
		nneedSupply[i].x=targetNneedSupply[i].x;
	}
}

void BoDiagramWindow::draw(DC* dc) const
{
	if(!shown) return;
	UI_Window::draw(dc);
	if(infoWindow->isShown())
		dc->SetBrush(*theme.lookUpBrush(BODIAGRAM_BACK1));
	else
		dc->SetBrush(*theme.lookUpBrush(BODIAGRAM_BACK2));
	dc->DrawRectangle(getAbsoluteClientRectPosition()+Point(0,10), getClientRectSize()+Size(0,-10));

	if(count>0)
	{
		dc->SetFont(theme.lookUpFont(SMALL_ITALICS_BOLD_FONT));
/*		if(infoWindowNumber)
		{
			dc->SetPen(Pen(dc->doColor(80,80,80),2,SOLID_PEN_STYLE));
			dc->DrawSpline(count,needSupply);
			dc->SetPen(Pen(dc->doColor(60,60,200),2,SOLID_PEN_STYLE));
			dc->DrawSpline(count,minerals);
			dc->SetPen(Pen(dc->doColor(20,160,20),2,SOLID_PEN_STYLE));
			dc->DrawSpline(count,gas);
//			dc->SetPen(Pen(dc->doColor(255,40,40),2,SOLID_PEN_STYLE));
//			dc->DrawSpline(count,fitness);
//			dc->SetTextForeground(dc->doColor(255,40,40));
//			dc->DrawText(_T("Fitness"),getAbsoluteClientRectLeftBound()+1,getAbsoluteClientRectUpperBound()+8);
			dc->SetTextForeground(dc->doColor(60,60,200));
			dc->DrawText(_T("Minerals"),getAbsoluteClientRectLeftBound()+1,getAbsoluteClientRectUpperBound()+10);
			dc->SetTextForeground(dc->doColor(20,200,20));
			dc->DrawText(_T("Gas"),getAbsoluteClientRectLeftBound()+1,getAbsoluteClientRectUpperBound()+21);
			dc->SetTextForeground(dc->doColor(120,120,120));
			dc->DrawText(_T("Supply"),getAbsoluteClientRectLeftBound()+1,getAbsoluteClientRectUpperBound()+32);
		} else*/
//		{
//			dc->SetTextForeground(dc->doColor(255,160,160)); for fitness
			dc->SetBrush(*theme.lookUpBrush(BODIAGRAM_SUPPLY_BRUSH));
			dc->SetPen(*theme.lookUpPen(BODIAGRAM_SUPPLY_PEN));
/*			for(int i=count;i--;)
			{
				if(i>0)
					dc->DrawRectangle(hneedSupply[i].x, hneedSupply[i].y, hneedSupply[i].x - hneedSupply[i-1].x, nneedSupply[i].y-hneedSupply[i].y+1);
//				dc->DrawLine(hneedSupply[i],nneedSupply[i]);
			} // TODO*/
			dc->SetPen(*theme.lookUpPen(BODIAGRAM_SUPPLY_PEN));
//			dc->DrawSpline(count,nneedSupply);
//			dc->DrawSpline(count,hneedSupply);
			for(int i=0;i<getClientRectWidth();i++)
				dc->DrawLine(getAbsoluteClientRectLeftBound()+i, hneedSupply[count*i/getClientRectWidth()].y, getAbsoluteClientRectLeftBound()+i+1, nneedSupply[count*i/getClientRectWidth()].y+1);
					
			dc->SetPen(*theme.lookUpPen(BODIAGRAM_MINERALS_PEN));
			dc->DrawSpline(count, &(minerals[0]));
			dc->SetPen(*theme.lookUpPen(BODIAGRAM_GAS_PEN));
			dc->DrawSpline(count, &(gas[0]));
//			dc->SetPen(Pen(dc->doColor(255,40,40),2,SOLID_PEN_STYLE));
//			dc->DrawSpline(count,fitness);
//			dc->SetTextForeground(dc->doColor(255,40,40));
//			dc->DrawText(_T("Fitness"),getAbsoluteClientRectLeftBound()+1,getAbsoluteClientRectUpperBound()+8);
			dc->SetTextForeground(*theme.lookUpColor(MINERALS_TEXT_COLOUR));
			dc->DrawText("Minerals",getAbsoluteClientRectPosition()+Point(1,10));
			dc->SetTextForeground(*theme.lookUpColor(GAS_TEXT_COLOUR));
			dc->DrawText("Gas",getAbsoluteClientRectPosition()+Point(1,21));
			dc->SetTextForeground(*theme.lookUpColor(SUPPLY_TEXT_COLOUR));
			dc->DrawText("Supply",getAbsoluteClientRectPosition()+Point(1,32));

			if(count>5)
			if(insideClientRect(controls.getCurrentPosition()))
			{
				// TODO this anarace values are one iteration too old compared to mouse position...
				ostringstream os;
				dc->SetTextForeground(*theme.lookUpColor(BRIGHT_MINERALS_TEXT_COLOUR));
				os << anarace->getStatisticsHaveMinerals(settings.getGa()->maxTime-(controls.getX()-getAbsoluteClientRectLeftBound())*(anarace->getRealTimer())/getClientRectWidth())/100;
				dc->DrawText(os.str(),getAbsoluteClientRectPosition()+Point(50,10));os.str("");
				
				dc->SetTextForeground(*theme.lookUpColor(BRIGHT_GAS_TEXT_COLOUR));
				
				os << anarace->getStatisticsHaveGas(settings.getGa()->maxTime-(controls.getX()-getAbsoluteClientRectLeftBound())*(anarace->getRealTimer())/getClientRectWidth())/100;
				dc->DrawText(os.str(), getAbsoluteClientRectPosition()+Point(50,21));os.str("");
				
				dc->SetTextForeground(*theme.lookUpColor(BRIGHT_SUPPLY_TEXT_COLOUR));

				int ns=anarace->getStatisticsNeedSupply(settings.getGa()->maxTime-(controls.getX()-getAbsoluteClientRectLeftBound())*(anarace->getRealTimer())/getClientRectWidth());
				int hs=anarace->getStatisticsHaveSupply(settings.getGa()->maxTime-(controls.getX()-getAbsoluteClientRectLeftBound())*(anarace->getRealTimer())/getClientRectWidth());
				
				os << ns << ":" << hs;
				dc->DrawText(os.str(), getAbsoluteClientRectPosition()+Point(50,32));
			}
			dc->SetPen(*theme.lookUpPen(RECTANGLE_PEN));
		}
		if(infoWindow->isShown())
		{
				if(stats[(*anarace->getStartCondition())->getRace()][infoWindow->getUnit()].needSupply)
				{
						dc->SetPen(*theme.lookUpPen(DASHED_SUPPLY_PEN));
						int y1=stats[(*anarace->getStartCondition())->getRace()][infoWindow->getUnit()].needSupply*5;
						if(y1<0) y1=-y1;
						dc->DrawLine(getAbsoluteClientRectLeftBound()+infoWindow->getBx()+1,getAbsoluteClientRectLowerBound()-y1, getAbsoluteClientRectLeftBound()+infoWindow->getBx()+infoWindow->getBWidth()-1,getAbsoluteClientRectLowerBound()-y1);
				}

				if(stats[(*anarace->getStartCondition())->getRace()][infoWindow->getUnit()].minerals)
				{
						dc->SetPen(*theme.lookUpPen(DASHED_MINERALS_PEN));
						dc->DrawLine(getAbsoluteClientRectLeftBound()+infoWindow->getBx()+1,getAbsoluteClientRectLowerBound()-stats[(*anarace->getStartCondition())->getRace()][infoWindow->getUnit()].minerals/1000,getAbsoluteClientRectLeftBound()+infoWindow->getBx()+infoWindow->getBWidth()-1,getAbsoluteClientRectLowerBound()-stats[(*anarace->getStartCondition())->getRace()][infoWindow->getUnit()].minerals/1000);
				}

				if(stats[(*anarace->getStartCondition())->getRace()][infoWindow->getUnit()].gas)
				{
						dc->SetPen(*theme.lookUpPen(DASHED_GAS_PEN));
						dc->DrawLine(getAbsoluteClientRectLeftBound()+infoWindow->getBx()+1,getAbsoluteClientRectLowerBound()-stats[(*anarace->getStartCondition())->getRace()][infoWindow->getUnit()].gas/1000,getAbsoluteClientRectLeftBound()+infoWindow->getBx()+infoWindow->getBWidth()-1,getAbsoluteClientRectLowerBound()-stats[(*anarace->getStartCondition())->getRace()][infoWindow->getUnit()].gas/1000);
				}
				dc->SetPen(*theme.lookUpPen(DASHED_MARKER_PEN));
				dc->DrawLine(getAbsoluteClientRectLeftBound()+infoWindow->getBx()+infoWindow->getBWidth(),getAbsoluteClientRectUpperBound()+10,getAbsoluteClientRectLeftBound()+infoWindow->getBx()+infoWindow->getBWidth(),getAbsoluteClientRectLowerBound());
				dc->DrawLine(getAbsoluteClientRectLeftBound()+infoWindow->getBx(),getAbsoluteClientRectUpperBound()+10,getAbsoluteClientRectLeftBound()+infoWindow->getBx(),getAbsoluteClientRectLowerBound());
		}

// ------ (soon transparent) BAR WHICH SHOWS WHICH TIME INTERVALL IS LOOKED UPON -----
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
// ----- END BAR -----
}

