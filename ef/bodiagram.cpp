#include "bodiagram.hpp"

BoDiagramWindow::BoDiagramWindow(const BoDiagramWindow& object) :
    UI_Window((UI_Window)object),
    anarace(object.anarace),
    infoWindow(object.infoWindow),
    count(object.count)
{
// TODO arrays?
}

BoDiagramWindow& BoDiagramWindow::operator=(const BoDiagramWindow& object)
{
	((UI_Window)(*this)) = ((UI_Window)object);
	anarace = object.anarace;
	infoWindow = object.infoWindow;
	count = object.count;
	return(*this);
}

BoDiagramWindow::BoDiagramWindow(UI_Object* bod_parent, ANARACE* bod_anarace, InfoWindow* bod_info_window, const unsigned int bod_window_number):
	UI_Window(bod_parent, BODIAGRAM_WINDOW_TITLE_STRING, BO_DIAGRAM_WINDOW, bod_window_number, NOT_SCROLLED),
	anarace(bod_anarace),
	infoWindow(bod_info_window),	
	count(0)
{
	resetData();
	for(int i=MAX_TIME;i--;)
	{
		minerals[i]=Point(0,0);
		gas[i]=Point(0,0);
		hneedSupply[i]=Point(0,0);
		nneedSupply[i]=Point(0,0);
//		fitness[i]=Point(0,0);

		startMinerals[i]=Point(0,0);
		startGas[i]=Point(0,0);
		startHneedSupply[i]=Point(0,0);
		startNneedSupply[i]=Point(0,0);
		startFitness[i]=Point(0,0);

		targetMinerals[i]=Point(0,0);
		targetGas[i]=Point(0,0);
		targetHneedSupply[i]=Point(0,0);
		targetNneedSupply[i]=Point(0,0);
		targetFitness[i]=Point(0,0);
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
	if(!isShown()) 
		return;

	UI_Window::process();

	count=0;
	if(anarace->getRealTimer()<2) return;
	
//	if(anarace->getTimer()==settings.getGa()->maxTime) time=0;
//		else 
//time=anarace->getTimer()+1;
	float time = (getClientRectWidth()-6) / (float)(anarace->getRealTimer());

	unsigned int haveSupply=0;
	unsigned int maxMins=0;
	unsigned int maxGas=0;
	for(int i = settings.getGa()->maxLength - anarace->getLength(); i<settings.getGa()->maxLength; i++)
	{
		if(anarace->getStatisticsHaveMinerals(i)>maxMins) 
			maxMins=anarace->getStatisticsHaveMinerals(i);
		if(anarace->getStatisticsHaveGas(i)>maxGas) 
			maxGas=anarace->getStatisticsHaveGas(i);
		if(anarace->getStatisticsNeedSupply(i)>haveSupply) 
			haveSupply=anarace->getStatisticsNeedSupply(i);
		if(anarace->getStatisticsHaveSupply(i)>haveSupply) 
			haveSupply=anarace->getStatisticsHaveSupply(i);
	}
	unsigned int s=MAX_LENGTH/*settings.getGa()->maxLength*/ - anarace->getLength();
	while(s < MAX_LENGTH)//settings.getGa()->maxLength)
	{
		if(!anarace->getProgramIsBuilt(s))
		{
			s++;
			continue;
		}
		int y1;
		if(maxMins) y1=anarace->getStatisticsHaveMinerals(s)*75/maxMins; else y1=0;
		Point p=Point( 3+anarace->getProgramTime(s)*time, -y1-2);
        if(targetMinerals[count]==Point(0,0))
        {
            targetMinerals[count]=p;
            startMinerals[count]=p;
            minerals[count]=p;
        } else
		if(p!=targetMinerals[count]) {
			targetMinerals[count]=p;
			startMinerals[count]=minerals[count];
		}
																																						
		if(maxGas) y1=anarace->getStatisticsHaveGas(s)*75/maxGas; else y1=0;
		p=Point( 3+anarace->getProgramTime(s)*time, -y1-2);
		// first round
		if(targetGas[count]==Point(0,0))
		{
			targetGas[count]=p;
			startGas[count]=p;
			gas[count]=p;
		} else
		if(p!=targetGas[count]) {
			targetGas[count]=p;
			startGas[count]=gas[count];
		}
	
//TODO anarace->getMaxpFitness-getTimer kann auch 0 sein !! kann es??
																																						
/*		if(anarace->getMaxpFitness()) y1=anarace->getStatisticsFitness(s)*75/(anarace->getMaxpFitness()-anarace->getTimer()); else y1=0;
		p=Point( 3+((count*(getClientRectWidth()-6))/(anarace->getRealTimer())), -y1);
		if(p!=targetFitness[count]) {
			targetFitness[count]=p;
			startFitness[count]=fitness[count];
		}		*/
		
		if(haveSupply) y1=anarace->getStatisticsHaveSupply(s)*75/haveSupply; else y1=0;
		p=Point(3+anarace->getRealProgramTime(s)*time, -y1-2);
        if(targetHneedSupply[count]==Point(0,0))
        {
            targetHneedSupply[count]=p;
            startHneedSupply[count]=p;
            hneedSupply[count]=p;
        } else
		if(p!=targetHneedSupply[count]) {
			targetHneedSupply[count]=p;
			startHneedSupply[count]=hneedSupply[count];
		}
		
		if(haveSupply) y1=anarace->getStatisticsNeedSupply(s)*75/haveSupply; else y1=0;
		p=Point(3+anarace->getRealProgramTime(s)*time, -y1-2);
        if(targetNneedSupply[count]==Point(0,0))
        {
            targetNneedSupply[count]=p;
            startNneedSupply[count]=p;
            nneedSupply[count]=p;
        } else
		if(p!=targetNneedSupply[count]) {
			targetNneedSupply[count]=p;
			startNneedSupply[count]=nneedSupply[count];
		}
	
		count++;
//		s--;
		s++;
	}

//TODO letztes item
//	count++;

	for(int i=count;i--;)
	{
	// TODO in unsigned umwandeln...
		Point::mv(minerals[i].y, startMinerals[i].y, targetMinerals[i].y);
		Point::mv(gas[i].y, startGas[i].y, targetGas[i].y);
		Point::mv(hneedSupply[i].y, startHneedSupply[i].y, targetHneedSupply[i].y);
		Point::mv(nneedSupply[i].y, startNneedSupply[i].y, targetNneedSupply[i].y);
//		Point::mv(fitness[i].y, startFitness[i].y, targetFitness[i].y);
		
		minerals[i].x=targetMinerals[i].x;
		gas[i].x=targetGas[i].x;
		hneedSupply[i].x=targetHneedSupply[i].x;
		nneedSupply[i].x=targetNneedSupply[i].x;
//		fitness[i].x=targetFitness[i].x;
	}
}

void BoDiagramWindow::draw(DC* dc) const
{
	if(!isShown()) 
		return;
	UI_Window::draw(dc);
	if(infoWindow->isShown())
		dc->SetBrush(*theme.lookUpBrush(BODIAGRAM_BACK1));
	else
		dc->SetBrush(*theme.lookUpBrush(BODIAGRAM_BACK2));
	dc->DrawRectangle(getAbsoluteClientRectPosition()+Point(0,10), getClientRectSize() - Size(0,10));

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
		
			dc->SetBrush(*theme.lookUpBrush(BODIAGRAM_SUPPLY_BRUSH));
			dc->SetPen(*theme.lookUpPen(BODIAGRAM_SUPPLY_PEN));
			for(unsigned int i=0;i<getClientRectWidth();i++)
			{
				int yy1 = getAbsoluteClientRectPosition().y + getClientRectHeight() + hneedSupply[count*i/getClientRectWidth()].y;
				int yy2 = getAbsoluteClientRectPosition().y + getClientRectHeight() + nneedSupply[count*i/getClientRectWidth()].y;
/*				if(yy1 < yy2)
				{
            		dc->SetPen(*theme.lookUpPen(BODIAGRAM_SUPPLY_PEN));
					dc->DrawVerticalLine(getAbsoluteClientRectPosition().x+i, yy1, yy2);
				} else
				if(yy1 > yy2)
				{
                    dc->SetPen(*theme.lookUpPen(MARKED_BITMAP_RED_PEN));
                    dc->DrawVerticalLine(getAbsoluteClientRectPosition().x+i, yy2, yy1);
				} else
                {
                    dc->SetPen(*theme.lookUpPen(MARKED_BITMAP_RED_PEN));
                    dc->DrawVerticalLine(getAbsoluteClientRectPosition().x+i, yy1, yy1+1);
                }*/
                {
                    dc->SetPen(*theme.lookUpPen(MARKED_BITMAP_RED_PEN));
                    dc->DrawVerticalLine(getAbsoluteClientRectPosition().x+i, yy1, getAbsoluteClientRectPosition().y + getClientRectHeight());
            		dc->SetPen(*theme.lookUpPen(BODIAGRAM_SUPPLY_PEN));
					dc->DrawVerticalLine(getAbsoluteClientRectPosition().x+i, yy2, getAbsoluteClientRectPosition().y + getClientRectHeight());
                } 				
				
			}
			
			dc->SetPen(*theme.lookUpPen(BODIAGRAM_MINERALS_PEN));
			dc->DrawSpline(count, &(minerals[0]), getAbsoluteClientRectPosition()+Point(0,getClientRectHeight()));
			dc->SetPen(*theme.lookUpPen(BODIAGRAM_GAS_PEN));
			dc->DrawSpline(count, &(gas[0]), getAbsoluteClientRectPosition()+Point(0,getClientRectHeight()));
//			dc->SetPen(*theme.lookUpPen(BODIAGRAM_FITNESS_PEN));
//			dc->DrawSpline(count, &(fitness[0]), getAbsoluteClientRectPosition()+Point(0,getClientRectHeight()));*/
					
			dc->SetTextForeground(*theme.lookUpColor(MINERALS_TEXT_COLOR));
			dc->DrawText("Minerals",getAbsoluteClientRectPosition()+Point(1,10));
			dc->SetTextForeground(*theme.lookUpColor(GAS_TEXT_COLOR));
			dc->DrawText("Gas",getAbsoluteClientRectPosition()+Point(1,21));
			dc->SetTextForeground(*theme.lookUpColor(SUPPLY_TEXT_COLOR));
			dc->DrawText("Supply",getAbsoluteClientRectPosition()+Point(1,32));
			dc->SetTextForeground(*theme.lookUpColor(FITNESS_TEXT_COLOR));
			dc->DrawText("Fitness",getAbsoluteClientRectPosition()+Point(1,43));
					
			if(count>5)
			if(isMouseInside())
			{
				// TODO this anarace values are one iteration too old compared to mouse position...
/*				ostringstream os;
				dc->SetTextForeground(*theme.lookUpColor(BRIGHT_MINERALS_TEXT_COLOR));
				os << anarace->getStatisticsHaveMinerals(settings.getGa()->maxTime-(controls.getX()-getAbsoluteClientRectLeftBound())*(anarace->getRealTimer())/getClientRectWidth())/100;
				dc->DrawText(os.str(),getAbsoluteClientRectPosition()+Point(50,10));os.str("");
				
				dc->SetTextForeground(*theme.lookUpColor(BRIGHT_GAS_TEXT_COLOR));
				
				os << anarace->getStatisticsHaveGas(settings.getGa()->maxTime-(controls.getX()-getAbsoluteClientRectLeftBound())*(anarace->getRealTimer())/getClientRectWidth())/100;
				dc->DrawText(os.str(), getAbsoluteClientRectPosition()+Point(50,21));os.str("");
				
				dc->SetTextForeground(*theme.lookUpColor(BRIGHT_SUPPLY_TEXT_COLOR));

				int ns=anarace->getStatisticsNeedSupply(settings.getGa()->maxTime-(controls.getX()-getAbsoluteClientRectLeftBound())*(anarace->getRealTimer())/getClientRectWidth());
				int hs=anarace->getStatisticsHaveSupply(settings.getGa()->maxTime-(controls.getX()-getAbsoluteClientRectLeftBound())*(anarace->getRealTimer())/getClientRectWidth());
				
				os << ns << ":" << hs;
				dc->DrawText(os.str(), getAbsoluteClientRectPosition()+Point(50,32));*/
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
					dc->DrawHorizontalLine(getAbsoluteClientRectLeftBound()+infoWindow->getBx()+1,
								 getAbsoluteClientRectLowerBound()-y1, 
								 getAbsoluteClientRectLeftBound()+infoWindow->getBx()+infoWindow->getBWidth()-1);
			}

			if(stats[(*anarace->getStartCondition())->getRace()][infoWindow->getUnit()].minerals)
			{
				dc->SetPen(*theme.lookUpPen(DASHED_MINERALS_PEN));
				dc->DrawHorizontalLine(getAbsoluteClientRectLeftBound()+infoWindow->getBx()+1,
							 getAbsoluteClientRectLowerBound()-stats[(*anarace->getStartCondition())->getRace()][infoWindow->getUnit()].minerals/1000,
							 getAbsoluteClientRectLeftBound()+infoWindow->getBx()+infoWindow->getBWidth()-1);
			}

			if(stats[(*anarace->getStartCondition())->getRace()][infoWindow->getUnit()].gas)
			{
				dc->SetPen(*theme.lookUpPen(DASHED_GAS_PEN));
				dc->DrawHorizontalLine(getAbsoluteClientRectLeftBound()+infoWindow->getBx()+1,
							 getAbsoluteClientRectLowerBound()-stats[(*anarace->getStartCondition())->getRace()][infoWindow->getUnit()].gas/1000,
							 getAbsoluteClientRectLeftBound()+infoWindow->getBx()+infoWindow->getBWidth()-1);
			}
			dc->SetPen(*theme.lookUpPen(DASHED_MARKER_PEN));
			dc->DrawVerticalLine(getAbsoluteClientRectLeftBound()+infoWindow->getBx()+infoWindow->getBWidth(),
						 getAbsoluteClientRectUpperBound()+10,
						 getAbsoluteClientRectLowerBound());
						 
			dc->DrawVerticalLine(getAbsoluteClientRectLeftBound()+infoWindow->getBx(),
						 getAbsoluteClientRectUpperBound()+10,
						 getAbsoluteClientRectLowerBound());
		}

// ------ (soon transparent) BAR WHICH SHOWS WHICH TIME INTERVALL IS LOOKED UPON -----
/*	if(insideClientRect(controls.getCurrentPosition()))
	{				
		int x=controls.getX()-2;
		if(x<getAbsoluteClientRectLeftBound())
			x=getAbsoluteClientRectLeftBound();
		if(x+4>=getAbsoluteClientRectRightBound())
			x=getAbsoluteClientRectRightBound()-5;
//		dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
		dc->SetPen(*theme.lookUpPen(SELECT_PEN));
		dc->DrawEmptyRectangle(x,getAbsoluteClientRectUpperBound()+10,4,getClientRectHeight()-10);
	}*/
// ----- END BAR -----
}

