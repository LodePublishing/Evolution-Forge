#include "bodiagram.hpp"
#include "../core/configuration.hpp"
#include <sstream>

/*BoDiagramWindow::BoDiagramWindow(const BoDiagramWindow& object) :
	UI_Window((UI_Window)object),
	anarace(object.anarace),
	count(object.count),
	bold(object.bold),
	oldMouse()
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
}*/

BoDiagramWindow::BoDiagramWindow(UI_Object* bod_parent, const unsigned int game_number, const unsigned int game_max, const unsigned int player_number, const unsigned int player_max) :
	UI_Window(bod_parent, BODIAGRAM_WINDOW_TITLE_STRING, theme.lookUpPlayerRect(BUILD_ORDER_DIAGRAM_WINDOW, game_number, game_max, player_number, player_max), theme.lookUpPlayerMaxHeight(BUILD_ORDER_DIAGRAM_WINDOW, game_number, game_max, player_number, player_max), NOT_SCROLLED),
	anarace(NULL),
	count(0),
	bold(false),
	mouseTime(0),
	oldMouse(),
	gameNumber(game_number),
	gameMax(game_max),
	playerNumber(player_number),
	playerMax(player_max)
{
	resetData();
	addHelpButton(DESCRIPTION_BODIAGRAM_WINDOW_CHAPTER);
}


BoDiagramWindow::~BoDiagramWindow()
{ }

void BoDiagramWindow::setMode(const unsigned int game_number, const unsigned int game_max, const unsigned int player_number, const unsigned int player_max)
{
	if((game_number == gameNumber) && (game_max == gameMax) && (player_number == playerNumber) && (player_max == playerMax))
		return;
	gameNumber = game_number;
	gameMax = game_max;
	playerNumber = player_number;
	playerMax = player_max;
}


void BoDiagramWindow::reloadOriginalSize()
{
	setOriginalRect(UI_Object::theme.lookUpPlayerRect(BUILD_ORDER_DIAGRAM_WINDOW, gameNumber, gameMax, playerNumber, playerMax));
	setMaxHeight(UI_Object::theme.lookUpPlayerMaxHeight(BUILD_ORDER_DIAGRAM_WINDOW, gameNumber, gameMax, playerNumber, playerMax));

	UI_Window::reloadOriginalSize();
}

void BoDiagramWindow::assignAnarace(ANABUILDORDER* bod_anarace)
{
	if(anarace == bod_anarace)
		return;
	anarace = bod_anarace;
	resetData();
}

void BoDiagramWindow::resetData()
{ 
	for(unsigned int j = TOTAL_STATS; j--;)
		for(unsigned int i=MAX_LENGTH*2+2;i--;)
			current[j][i] = start[j][i] = target[j][i] = Point(0,0);
	processList();
}

void BoDiagramWindow::process()
{
	if(!isShown()) 
		return;

	UI_Window::process();

	for(unsigned int i=count;i--;)
		for(unsigned int j = TOTAL_STATS;j--;)
		{
			if(Point::mv(current[j][i].y, start[j][i].y, target[j][i].y));
				setNeedRedrawNotMoved();
			current[j][i] = target[j][i]; // ?
		}
	
	bold = false;
	selectedItems.clear();
	if((count>2)&&(getAbsoluteClientRect().Inside(mouse))&&(anarace->getRealTimer()>0))
	{
		bold=true;
		if(oldMouse!=mouse)
			setNeedRedrawNotMoved();
		mouseTime = anarace->getRealTimer() * (mouse.x - getAbsoluteClientRectLeftBound()) / getClientRectWidth();
	
		unsigned int number = 0;
		for(std::list<PROGRAM>::const_iterator k = anarace->getProgramList().begin(); k != anarace->getProgramList().end(); ++k, ++number)
		if((k->getTime() - k->getBT() - (anarace->getRealTimer()/20) < coreConfiguration.getMaxTime()-mouseTime)&&(k->getTime() + (anarace->getRealTimer()/20) > coreConfiguration.getMaxTime() - mouseTime))
			selectedItems.push_back(number);
	}
	oldMouse = mouse;
}

void BoDiagramWindow::processList()
{
	if((anarace==NULL)||(anarace->getRealTimer()<2)) 
		return;


	unsigned int max[TOTAL_STATS];
	for(unsigned int i = TOTAL_STATS; i--;)
		max[i] = 0;
	max[HAVE_SUPPLY] = 10;
	max[NEED_SUPPLY] = 10;
	max[HAVE_MINERALS] = (*anarace->getStartCondition())->getMinerals();
	max[HAVE_GAS] = (*anarace->getStartCondition())->getGas();

	count = anarace->getProgramList().size()*2+2;
	
	unsigned int xpos[MAX_LENGTH*2+2];
	unsigned int temp[TOTAL_STATS][MAX_LENGTH*2+2];

	temp[HAVE_MINERALS][0] = (*anarace->getStartCondition())->getMinerals();
	temp[HAVE_GAS][0] = (*anarace->getStartCondition())->getGas();
	temp[HAVE_SUPPLY][0] = (*anarace->getStartCondition())->getHaveSupply();
	temp[NEED_SUPPLY][0] = (*anarace->getStartCondition())->getNeedSupply();
	xpos[0] = 1;

	temp[HAVE_MINERALS][count-1] = anarace->getMinerals();
	temp[HAVE_GAS][count-1] = anarace->getGas();
	temp[HAVE_SUPPLY][count-1] = anarace->getHaveSupply();
	temp[NEED_SUPPLY][count-1] = anarace->getNeedSupply();
	xpos[count-1] = 1 + getClientTargetWidth()-5;
		
	unsigned int k = 1;
	for(std::list<PROGRAM>::const_iterator order = anarace->getProgramList().begin(); order != anarace->getProgramList().end(); ++order)
	{
		// TODO Problem: Es kann auftauchen, dass Timer eines Eintrags groesser als vorangehender und nachfolgender Eintrag ist!
		// Wahrscheinlich wenn Orders vertauscht / neu eingesetzt werden :-o
		temp[HAVE_MINERALS][k] = order->getStatisticsBefore().getHaveMinerals();
		temp[HAVE_GAS][k] = order->getStatisticsBefore().getHaveGas();
		temp[HAVE_SUPPLY][k] = order->getStatisticsBefore().getHaveSupply();
		temp[NEED_SUPPLY][k] = order->getStatisticsBefore().getNeedSupply();
		xpos[k] = 1 + (int)(order->getRealTime() * (getClientTargetWidth()-5) / (float)(anarace->getRealTimer()));
		k++;

		temp[HAVE_MINERALS][k] = order->getStatisticsAfter().getHaveMinerals();
		temp[HAVE_GAS][k] = order->getStatisticsAfter().getHaveGas();
		temp[HAVE_SUPPLY][k] = order->getStatisticsAfter().getHaveSupply();
		temp[NEED_SUPPLY][k] = order->getStatisticsAfter().getNeedSupply();
		xpos[k] = xpos[k-1];
		k++;
	}


	for(unsigned int j = TOTAL_STATS; j--;)
		for(unsigned int i = count; i--;)
		{
			if(temp[j][i] > max[j])
				max[j] = temp[j][i];
		}
	// special rule for supply:
		if(max[HAVE_SUPPLY] > max[NEED_SUPPLY])
			max[NEED_SUPPLY] = max[HAVE_SUPPLY];
		else if(max[NEED_SUPPLY] > max[HAVE_SUPPLY])
			max[HAVE_SUPPLY] = max[NEED_SUPPLY];
		
// TODO nur machen wenns optimiert!?
//	if(anarace->getTimer()==coreConfiguration.getMaxTime()) time=0;
//		else 
//	time=anarace->getTimer()+1;

	unsigned int height = getClientRectHeight() - 20;
	

	for(unsigned int j = TOTAL_STATS; j--;)
		for(unsigned int i = count; i--;)
		{
			int y1;
			if(max[j])
				y1 = temp[j][i]*height/max[j];
			else y1 = 0;
			Point p = Point(xpos[i], -y1-2);
			if(target[j][i] == Point(0,0))
				target[j][i] = start[j][i] = current[j][i] = p;
			else if(p != target[j][i]) {
				target[j][i] = p;
				start[j][i] = current[j][i];
			}
		}
}

void BoDiagramWindow::draw(DC* dc) const
{
	if(!isShown()) 
		return;
	if(!checkForNeedRedraw())
		return;
	UI_Window::drawWindow(dc);
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
				if((current[HAVE_SUPPLY][i].y > current[NEED_SUPPLY][i].y)&&(current[HAVE_SUPPLY][i].x < (signed int)(getClientTargetWidth()-2)))
					dc->DrawRectangle(getAbsoluteClientRectPosition() + Point(0, getClientRectHeight()) + current[HAVE_SUPPLY][i], Size(current[HAVE_SUPPLY][i+1].x - current[HAVE_SUPPLY][i].x, current[HAVE_SUPPLY][i].y - current[NEED_SUPPLY][i].y));
				else if((current[HAVE_SUPPLY][i].y < current[NEED_SUPPLY][i].y)&&(current[HAVE_SUPPLY][i].x < (signed int)(getClientTargetWidth()-2)))
					dc->DrawRectangle(getAbsoluteClientRectPosition() + Point(0, getClientRectHeight()) + current[HAVE_SUPPLY][i], Size(current[HAVE_SUPPLY][i+1].x - current[HAVE_SUPPLY][i].x, current[NEED_SUPPLY][i].y - current[HAVE_SUPPLY][i].y));
			}
	
			{
				Point p = getAbsoluteClientRectPosition()+Point(0,getClientRectHeight());
				for(unsigned int i = 0; i < count-1;i++)
				{
					if(current[HAVE_GAS][i].y + 10 < current[HAVE_GAS][i+1].y)
						dc->SetPen(*theme.lookUpPen(DASHED_GAS_PEN));
					else
					if(current[HAVE_GAS][i].y < current[HAVE_GAS][i+1].y)
						dc->SetPen(*theme.lookUpPen(BODIAGRAM_GAS_PEN));
					else
						dc->SetPen(*theme.lookUpPen((ePen)(BODIAGRAM_GAS_PEN+(bold?3:0))));
					dc->DrawLine(p.x + current[HAVE_GAS][i].x, p.y + current[HAVE_GAS][i].y, p.x + current[HAVE_GAS][i+1].x, p.y + current[HAVE_GAS][i+1].y);
				}
			}
			
			
			{
				Point p = getAbsoluteClientRectPosition()+Point(0,getClientRectHeight());
				for(unsigned int i = 0; i < count-1; ++i)
				{
					if(current[HAVE_MINERALS][i].y /*+ 10*/ < current[HAVE_MINERALS][i+1].y)
					{
						dc->SetPen(*theme.lookUpPen(DASHED_MINERALS_PEN));
						dc->DrawLine(p.x + current[HAVE_MINERALS][i].x, p.y + current[HAVE_MINERALS][i].y, p.x + current[HAVE_MINERALS][i+1].x, p.y + current[HAVE_MINERALS][i+1].y);
					}
//					else ?
//					if(current[HAVE_MINERALS][i].y < current[HAVE_MINERALS][i+1].y)
//						dc->SetPen(*theme.lookUpPen(BODIAGRAM_MINERALS_PEN));
					else
					{
						dc->SetPen(*theme.lookUpPen((ePen)(BODIAGRAM_MINERALS_PEN+(bold?3:0))));
						dc->DrawLine(p.x + current[HAVE_MINERALS][i].x, p.y + current[HAVE_MINERALS][i].y, p.x + current[HAVE_MINERALS][i+1].x, p.y + current[HAVE_MINERALS][i+1].y);
					}
					
				}
				
				std::list<PROGRAM>::const_iterator k = anarace->getProgramList().begin();
				for(unsigned int i = 0; i < count-1; ++i)
					if((bold)&&(current[HAVE_MINERALS][i].y /*+ 10*/ < current[HAVE_MINERALS][i+1].y))
					{
						dc->SetTextForeground(*theme.lookUpColor(BRIGHT_TEXT_COLOR));
						dc->SetFont(theme.lookUpFont(SMALL_SHADOW_BOLD_FONT));
						dc->DrawText(stats[(*anarace->getStartCondition())->getRace()][k->getUnit()].name, Point(p.x + current[HAVE_MINERALS][i].x, p.y + current[HAVE_MINERALS][i].y + (current[HAVE_MINERALS][i+1].y - current[HAVE_MINERALS][i].y)/2));
						++k;		
					}
			}

//			dc->DrawSpline(count, &(current[HAVE_MINERALS][0]), getAbsoluteClientRectPosition()+Point(0,getClientRectHeight()));
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
				
				os << anarace->getTimeStatistics()[coreConfiguration.getMaxTime()-mouseTime].getHaveMinerals()/100;
				dc->DrawText(os.str(),getAbsoluteClientRectPosition()+Point(50,15));os.str("");
				
				dc->SetTextForeground(*theme.lookUpColor(BRIGHT_GAS_TEXT_COLOR));
				os << anarace->getTimeStatistics()[coreConfiguration.getMaxTime()-mouseTime].getHaveGas()/100;
				dc->DrawText(os.str(), getAbsoluteClientRectPosition()+Point(50,26));os.str("");
				
				dc->SetTextForeground(*theme.lookUpColor(BRIGHT_SUPPLY_TEXT_COLOR));
				int ns = anarace->getTimeStatistics()[coreConfiguration.getMaxTime()-mouseTime].getNeedSupply();
				int hs = anarace->getTimeStatistics()[coreConfiguration.getMaxTime()-mouseTime].getHaveSupply();
				
				os << ns << "/" << hs;
				dc->DrawText(os.str(), getAbsoluteClientRectPosition()+Point(50,37));
				
				dc->SetTextForeground(*theme.lookUpColor(BRIGHT_FITNESS_TEXT_COLOR));
				dc->DrawText(formatTime(mouseTime), getAbsoluteClientRectPosition()+Point(50,48));
				
				dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
				dc->SetPen(*theme.lookUpPen(SELECT_PEN));
				dc->DrawEmptyRectangle( getAbsoluteClientRectLeftBound() + (mouseTime * getClientRectWidth()) / anarace->getRealTimer() ,getAbsoluteClientRectUpperBound()+10, 4, getClientRectHeight()-10);

				
				
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

			if(stats[(*anarace->getStartCondition())->getRace()][infoWindow->getUnit()].current[HAVE_GAS])
			{
				dc->SetPen(*theme.lookUpPen(DASHED_GAS_PEN));
				dc->DrawHorizontalLine(getAbsoluteClientRectLeftBound()+infoWindow->getBx()+1,
							 getAbsoluteClientRectLowerBound()-stats[(*anarace->getStartCondition())->getRace()][infoWindow->getUnit()].current[HAVE_GAS]/1000,
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
	UI_Object::draw(dc);

}

