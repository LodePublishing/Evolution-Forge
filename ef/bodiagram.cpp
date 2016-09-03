#include "bodiagram.hpp"
#include "../core/configuration.hpp"
#include <sstream>

BoDiagramWindow::BoDiagramWindow(const BoDiagramWindow& object) :
	UI_Window((UI_Window)object),
	anarace(object.anarace),
	count(object.count),
	bold(object.bold)
{
// TODO arrays?
}

BoDiagramWindow& BoDiagramWindow::operator=(const BoDiagramWindow& object)
{
	((UI_Window)(*this)) = ((UI_Window)object);
	anarace = object.anarace;
	count = object.count;
	bold = object.bold;
	return(*this);
}

BoDiagramWindow::BoDiagramWindow(UI_Object* bod_parent, const unsigned int game_number, const unsigned int max_games, const unsigned int player_number, const unsigned int max_players) :
	UI_Window(bod_parent, BODIAGRAM_WINDOW_TITLE_STRING, theme.lookUpPlayerRect(BUILD_ORDER_DIAGRAM_WINDOW, game_number, max_games, player_number, max_players), theme.lookUpPlayerMaxHeight(BUILD_ORDER_DIAGRAM_WINDOW, game_number, max_games, player_number, max_players), NOT_SCROLLED),
	anarace(NULL),
	count(0),
	bold(false)
{
	resetData();
}


BoDiagramWindow::~BoDiagramWindow()
{ }

void BoDiagramWindow::reloadOriginalSize()
{
//	setOriginalRect(UI_Object::theme.lookUpGameRect(BUILD_ORDER_DIAGRAM_WINDOW, gameNumber, gameMax));
//	setMaxHeight(UI_Object::theme.lookUpGameMaxHeight(BUILD_ORDER_DIAGRAM_WINDOW, gameNumber, gameMax));

	
	UI_Window::reloadOriginalSize();
}

void BoDiagramWindow::assignAnarace(ANABUILDORDER* bod_anarace)
{
	anarace = bod_anarace;
	resetData();
}

void BoDiagramWindow::resetData()
{ 
	for(unsigned int i=MAX_TIME;i--;)
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

void BoDiagramWindow::process()
{
	if(!isShown()) 
		return;

	UI_Window::process();

	for(unsigned int i=count;i--;)
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
	
	bold = false;
	if((count>2)&&(getAbsoluteClientRect().Inside(mouse))&&(anarace->getRealTimer()>0))
	{
		bold=true;
		setNeedRedrawNotMoved();
	}
}

void BoDiagramWindow::processList()
{
	if(anarace->getRealTimer()<2) return;
	
// TODO nur machen wenns optimiert!

//	if(anarace->getTimer()==coreConfiguration.getMaxTime()) time=0;
//		else 
//time=anarace->getTimer()+1;
	float time = (getClientTargetWidth()-5) / (float)(anarace->getRealTimer());

	unsigned int haveSupply = 10; // TODO
	unsigned int maxMins = (*anarace->getStartCondition())->getMinerals();
	unsigned int maxGas = (*anarace->getStartCondition())->getGas();
	
	for(std::list<PROGRAM>::const_iterator order = anarace->getProgramList().begin(); order != anarace->getProgramList().end(); ++order)
	{
		if(order->getStatisticsBefore().getHaveMinerals() > maxMins) maxMins = order->getStatisticsBefore().getHaveMinerals();
		if(order->getStatisticsBefore().getHaveGas() > maxGas) maxGas = order->getStatisticsBefore().getHaveGas();
		if(order->getStatisticsBefore().getNeedSupply() > haveSupply) haveSupply = order->getStatisticsBefore().getNeedSupply();
		if(order->getStatisticsBefore().getHaveSupply() > haveSupply) haveSupply = order->getStatisticsBefore().getHaveSupply();
	}
	if(anarace->getMinerals()>maxMins) maxMins=anarace->getMinerals();
	if(anarace->getGas()>maxGas) maxGas=anarace->getGas();
	if(anarace->getNeedSupply()>haveSupply)	haveSupply=anarace->getNeedSupply();
	if(anarace->getHaveSupply()>haveSupply) haveSupply=anarace->getHaveSupply();

	count=0;

	{
		int y1;
		if(maxMins) y1=(*anarace->getStartCondition())->getMinerals()*(getClientRectHeight()-20)/maxMins; else y1=0;
		Point p = Point( 1, -y1-2);
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

		if(maxGas) y1=(*anarace->getStartCondition())->getGas()*(getClientRectHeight()-20)/maxGas; else y1=0;
		p = Point( 1, -y1-2);
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
																																							
/*	  if(anarace->getMaxpFitness()) y1=anarace->getIPStatisticsFitness(2*s)*(getClientRectHeight()-20)/(anarace->getMaxpFitness()-anarace->getTimer()); else y1=0;
		p = Point(((count*getTargetClientRectWidth())/(anarace->getRealTimer())), -y1);
		if(p!=targetFitness[count]) {
			targetFitness[count]=p;
			startFitness[count]=fitness[count];
		}	   */
																																							
		if(haveSupply) y1=(*anarace->getStartCondition())->getHaveSupply()*(getClientRectHeight()-20)/haveSupply; else y1=0;
		p = Point(1, -y1-2);
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

		if(haveSupply) y1=(*anarace->getStartCondition())->getNeedSupply()*(getClientRectHeight()-20)/haveSupply; else y1=0;
		p = Point(1, -y1-2);
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
	}
	++count;

	for(std::list<PROGRAM>::const_iterator order = anarace->getProgramList().begin(); order != anarace->getProgramList().end(); ++order)
	{
		int y1;
		if(maxMins) y1 = order->getStatisticsBefore().getHaveMinerals()*(getClientRectHeight()-20)/maxMins; else y1 = 0;
		Point p = Point(1+(int)(order->getRealTime()*time), -y1-2);
		if(targetMinerals[count]==Point(0,0))
			targetMinerals[count] = startMinerals[count] = minerals[count] = p;
		else
		if(p != targetMinerals[count]) {
			targetMinerals[count] = p;
			startMinerals[count] = minerals[count];
		}
		

		if(maxGas) y1 = order->getStatisticsBefore().getHaveGas()*(getClientRectHeight()-20)/maxGas; else y1 = 0;
		p = Point(1+(int)(order->getRealTime()*time), -y1-2);
		if(targetGas[count] == Point(0, 0))
			targetGas[count] = startGas[count] = gas[count] = p;
		else
		if(p != targetGas[count]) {
			targetGas[count] = p;
			startGas[count] = gas[count];
		}
	
		if(haveSupply) y1 = order->getStatisticsBefore().getHaveSupply()*(getClientRectHeight()-20)/haveSupply; else y1=0;
		p = Point(1+(int)(order->getRealTime()*time), -y1-2);
		if(targetHneedSupply[count] == Point(0, 0))
			targetHneedSupply[count] = startHneedSupply[count] = hneedSupply[count] = p;
		else
		if(p != targetHneedSupply[count]) {
			targetHneedSupply[count] = p;
			startHneedSupply[count] = hneedSupply[count];
		}
		
		if(haveSupply) y1 = order->getStatisticsBefore().getNeedSupply()*(getClientRectHeight()-20)/haveSupply; else y1=0;
		p = Point(1+(int)(order->getRealTime()*time), -y1-2);
		if(targetNneedSupply[count] == Point(0, 0))
			targetNneedSupply[count] = startNneedSupply[count] = nneedSupply[count] = p;
		else
		if(p != targetNneedSupply[count]) {
			targetNneedSupply[count] = p;
			startNneedSupply[count] = nneedSupply[count];
		}
		++count;

		if(maxMins) y1 = order->getStatisticsAfter().getHaveMinerals()*(getClientRectHeight()-20)/maxMins; else y1 = 0;
		p = Point(1+(int)(order->getRealTime()*time), -y1-2);
		if(targetMinerals[count]==Point(0,0))
			targetMinerals[count] = startMinerals[count] = minerals[count] = p;
		else
		if(p != targetMinerals[count]) {
			targetMinerals[count] = p;
			startMinerals[count] = minerals[count];
		}
		

		if(maxGas) y1 = order->getStatisticsAfter().getHaveGas()*(getClientRectHeight()-20)/maxGas; else y1 = 0;
		p = Point(1+(int)(order->getRealTime()*time), -y1-2);
		if(targetGas[count] == Point(0, 0))
			targetGas[count] = startGas[count] = gas[count] = p;
		else
		if(p != targetGas[count]) {
			targetGas[count] = p;
			startGas[count] = gas[count];
		}
	
		if(haveSupply) y1 = order->getStatisticsAfter().getHaveSupply()*(getClientRectHeight()-20)/haveSupply; else y1=0;
		p = Point(1+(int)(order->getRealTime()*time), -y1-2);
		if(targetHneedSupply[count] == Point(0, 0))
			targetHneedSupply[count] = startHneedSupply[count] = hneedSupply[count] = p;
		else
		if(p != targetHneedSupply[count]) {
			targetHneedSupply[count] = p;
			startHneedSupply[count] = hneedSupply[count];
		}
		
		if(haveSupply) y1 = order->getStatisticsAfter().getNeedSupply()*(getClientRectHeight()-20)/haveSupply; else y1=0;
		p = Point(1+(int)(order->getRealTime()*time), -y1-2);
		if(targetNneedSupply[count] == Point(0, 0))
			targetNneedSupply[count] = startNneedSupply[count] = nneedSupply[count] = p;
		else
		if(p != targetNneedSupply[count]) {
			targetNneedSupply[count] = p;
			startNneedSupply[count] = nneedSupply[count];
		}
		++count;
	
	}

	{
		int y1;
		if(maxMins) y1=anarace->getMinerals()*(getClientRectHeight()-20)/maxMins; else y1=0;
		Point p = Point( getClientTargetWidth()-2, -y1-2);
		if(targetMinerals[count]==Point(0,0))
			targetMinerals[count] = startMinerals[count] = minerals[count] = p;
		else
		if(p!=targetMinerals[count]) {
			targetMinerals[count]=p;
			startMinerals[count]=minerals[count];
		}

		if(maxGas) y1=anarace->getGas()*(getClientRectHeight()-20)/maxGas; else y1=0;
		p = Point(  getClientTargetWidth()-2, -y1-2);
		// first round
		if(targetGas[count]==Point(0,0))
			targetGas[count] = startGas[count] = gas[count] = p;
		else
		if(p!=targetGas[count]) {
			targetGas[count] = p;
			startGas[count] = gas[count];
		}
		
		if(haveSupply) y1 = anarace->getHaveSupply()*(getClientRectHeight()-20)/haveSupply; else y1=0;
		p = Point( getClientTargetWidth()-2, -y1-2);
		if(targetHneedSupply[count] == Point(0,0))
			targetHneedSupply[count] = startHneedSupply[count] = hneedSupply[count] = p;
		else
		if(p!=targetHneedSupply[count]) {
			targetHneedSupply[count] = p;
			startHneedSupply[count] = hneedSupply[count];
		}

		if(haveSupply) y1=anarace->getNeedSupply()*(getClientRectHeight()-20)/haveSupply; else y1=0;
		p = Point( getClientTargetWidth()-2, -y1-2);
		if(targetNneedSupply[count]==Point(0,0))
			targetNneedSupply[count] = startNneedSupply[count] = nneedSupply[count] = p;
		else
		if(p!=targetNneedSupply[count]) {
			targetNneedSupply[count] = p;
			startNneedSupply[count] = nneedSupply[count];
		}
	}

//TODO letztes item
	++count;
/*	for(int i = 0; i < count; ++i) ???
		for(int j = 0; j < i; ++j)???
		{
			if(minerals[i].x < minerals[j].x)
			{
				Point temp = minerals[i];
				minerals[i]=minerals[j];
				minerals[j]=temp;
				temp = startMinerals[i];
				startMinerals[i]=startMinerals[j];
				startMinerals[j]=temp;
				temp = targetMinerals[i];
				targetMinerals[i]=targetMinerals[j];
				targetMinerals[j]=temp;			
			}
		}*/

}

void BoDiagramWindow::draw(DC* dc) const
{
	if(!isShown()) 
		return;
	UI_Window::draw(dc);
	if(!checkForNeedRedraw())
		return;
//	if(infoWindow->isShown())
		dc->SetBrush(*theme.lookUpBrush(BODIAGRAM_BACK1));
//	else
//		dc->SetBrush(*theme.lookUpBrush(BODIAGRAM_BACK2));
	dc->DrawRectangle(getAbsoluteClientRectPosition()+Point(0,10), getClientRectSize() - Size(0,10));

	if(count>0)
	{
		dc->SetFont(theme.lookUpFont(SMALL_BOLD_FONT));
		dc->SetBrush(*theme.lookUpBrush(BODIAGRAM_SUPPLY_BRUSH));
		dc->SetPen(*theme.lookUpPen(BODIAGRAM_SUPPLY_PEN));
			for(unsigned int i = 0;i<(count-1);++i)
			{
				if((hneedSupply[i].y > nneedSupply[i].y)&&(hneedSupply[i].x < (signed int)(getClientTargetWidth()-2)))
					dc->DrawRectangle(getAbsoluteClientRectPosition() + Point(0, getClientRectHeight()) + hneedSupply[i], Size(hneedSupply[i+1].x - hneedSupply[i].x, hneedSupply[i].y - nneedSupply[i].y));
				else if((hneedSupply[i].y < nneedSupply[i].y)&&(hneedSupply[i].x < (signed int)(getClientTargetWidth()-2)))
					dc->DrawRectangle(getAbsoluteClientRectPosition() + Point(0, getClientRectHeight()) + hneedSupply[i], Size(hneedSupply[i+1].x - hneedSupply[i].x, nneedSupply[i].y - hneedSupply[i].y));
			}
				
			
			if(bold)
				dc->SetPen(*theme.lookUpPen(BODIAGRAM_MINERALS_BOLD_PEN));
			else
				dc->SetPen(*theme.lookUpPen(BODIAGRAM_MINERALS_PEN));
			dc->DrawSpline(count, &(minerals[0]), getAbsoluteClientRectPosition()+Point(0,getClientRectHeight()));

			if(bold)
				dc->SetPen(*theme.lookUpPen(BODIAGRAM_GAS_BOLD_PEN));
			else
				dc->SetPen(*theme.lookUpPen(BODIAGRAM_GAS_PEN));
			dc->DrawSpline(count, &(gas[0]), getAbsoluteClientRectPosition()+Point(0,getClientRectHeight()));
//			dc->SetPen(*theme.lookUpPen(BODIAGRAM_FITNESS_PEN));
//			dc->DrawSpline(count, &(fitness[0]), getAbsoluteClientRectPosition()+Point(0,getClientRectHeight()));*/
					
			dc->SetTextForeground(*theme.lookUpColor(MINERALS_TEXT_COLOR));
			dc->DrawText("Minerals",getAbsoluteClientRectPosition()+Point(1,15));
			dc->SetTextForeground(*theme.lookUpColor(GAS_TEXT_COLOR));
			dc->DrawText("Gas",getAbsoluteClientRectPosition()+Point(1,26));
			dc->SetTextForeground(*theme.lookUpColor(SUPPLY_TEXT_COLOR));
			dc->DrawText("Supply",getAbsoluteClientRectPosition()+Point(1,37));
			dc->SetTextForeground(*theme.lookUpColor(FITNESS_TEXT_COLOR));
			dc->DrawText("Time",getAbsoluteClientRectPosition()+Point(1,48));
// Das in process, nur updaten wenn bohasChanged
			if(bold)
			{
				// TODO this anarace values are one iteration too old compared to mouse position...
				std::ostringstream os;
				os.str("");
				dc->SetTextForeground(*theme.lookUpColor(BRIGHT_MINERALS_TEXT_COLOR));
				unsigned int time = anarace->getRealTimer() * (mouse.x - getAbsoluteClientRectLeftBound()) / getClientRectWidth();
				if(mouse.x < getAbsoluteClientRectLeftBound())
					while(true);
				os << anarace->getTimeStatistics()[coreConfiguration.getMaxTime()-time].getHaveMinerals()/100;
				dc->DrawText(os.str(),getAbsoluteClientRectPosition()+Point(50,15));os.str("");
				
				dc->SetTextForeground(*theme.lookUpColor(BRIGHT_GAS_TEXT_COLOR));
				os << anarace->getTimeStatistics()[coreConfiguration.getMaxTime()-time].getHaveGas()/100;
				dc->DrawText(os.str(), getAbsoluteClientRectPosition()+Point(50,26));os.str("");
				
				dc->SetTextForeground(*theme.lookUpColor(BRIGHT_SUPPLY_TEXT_COLOR));
				int ns = anarace->getTimeStatistics()[coreConfiguration.getMaxTime()-time].getNeedSupply();
				int hs = anarace->getTimeStatistics()[coreConfiguration.getMaxTime()-time].getHaveSupply();
				
				os << ns << ":" << hs;
				dc->DrawText(os.str(), getAbsoluteClientRectPosition()+Point(50,37));
				
				dc->SetTextForeground(*theme.lookUpColor(BRIGHT_FITNESS_TEXT_COLOR));
				dc->DrawText(formatTime(time), getAbsoluteClientRectPosition()+Point(50,48));
				
				dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
				dc->SetPen(*theme.lookUpPen(SELECT_PEN));
				dc->DrawEmptyRectangle( getAbsoluteClientRectLeftBound() + (time * getClientRectWidth()) / anarace->getRealTimer() ,getAbsoluteClientRectUpperBound()+10, 4, getClientRectHeight()-10);

				
	// ------ (soon transparent) BAR WHICH SHOWS WHICH TIME INTERVALL IS LOOKED UPON -----
/*	if(insideClientRect(controls.getCurrentPosition()))
	{				
		int x=controls.getX()-2;
		if(x<getAbsoluteClientRectLeftBound())
			x=getAbsoluteClientRectLeftBound();
		if(x+4>=getAbsoluteClientRectRightBound())
			x=getAbsoluteClientRectRightBound()-5;
	}*/
// ----- END BAR -----			
			}
//			#endif
			dc->SetPen(*theme.lookUpPen(RECTANGLE_PEN));
		}
		#if 0 
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
		#endif

}

