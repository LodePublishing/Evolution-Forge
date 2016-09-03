#include "bgwindow.hpp"

#include <sstream>

const unsigned int MIN_HEIGHT = 2;


/*
BoGraphWindow::BoGraphWindow(const BoGraphWindow& object) :
	UI_Window((UI_Window)object),
	markAni(object.markAni),
	anarace(object.anarace),
	selectedItem(-1)
{
	for(unsigned int i = BOGRAPH_MAX_LINES;i--;)
		boGraphLine[i] = object.boGraphLine[i];
}

BoGraphWindow& BoGraphWindow::operator=(const BoGraphWindow& object)
{
	((UI_Window)(*this)) = ((UI_Window)object);
	markAni = object.markAni;
	anarace = object.anarace;
	selectedItem = -1;
	for(unsigned int i = BOGRAPH_MAX_LINES;i--;)
		boGraphLine[i] = object.boGraphLine[i];
	return(*this);
}*/

BoGraphWindow::BoGraphWindow(UI_Object* bograph_parent, const unsigned int game_number, const unsigned int max_games, const unsigned int player_number, const unsigned int max_players) :
	UI_Window(bograph_parent, BOGRAPH_WINDOW_TITLE_STRING, theme.lookUpPlayerRect(BUILD_ORDER_GRAPH_WINDOW, game_number, max_games, player_number, max_players), theme.lookUpPlayerMaxHeight(BUILD_ORDER_GRAPH_WINDOW, game_number, max_games, player_number, max_players), SCROLLED, AUTO_SIZE_ADJUST, NOT_TABBED, Rect(0, 15, 1000, 1000)),
	markAni(0),
	anarace(NULL),
	selectedItem(-1)
{
	addHelpButton(DESCRIPTION_BOGRAPH_WINDOW_CHAPTER);
}

BoGraphWindow::~BoGraphWindow()
{
	for(std::list<BoGraphLine*>::iterator i = boGraphLine.begin(); i != boGraphLine.end(); ++i)
		delete(*i);
//	for(std::list<UI_StaticText*>::iterator i = legend.begin(); i != legend.end(); ++i)
//		delete(*i);

}

void BoGraphWindow::resetData()
{
	for(std::list<BoGraphLine*>::iterator i = boGraphLine.begin(); i != boGraphLine.end(); ++i)
		(*i)->resetData();
	processList();
}

void BoGraphWindow::reloadOriginalSize()
{
	UI_Window::reloadOriginalSize();
	processList();
}


void BoGraphWindow::setSelected(const unsigned int selected)
{
	for(std::list<BoGraphLine*>::iterator i = boGraphLine.begin(); i != boGraphLine.end(); ++i)
		(*i)->checkSelected(selected);
}

void BoGraphWindow::processList()
{
	if(!isShown())
		return;
	if(anarace==NULL) return;
	
// the legend
// and the time steps on the top

/*	for(std::list<UI_StaticText*>::iterator i = legend.begin(); i != legend.end(); ++i)
		delete(*i);
	legend.clear();
	// TODO adjust machen

	unsigned int timesteps=((anarace->getRealTimer())/30)/10+1;
	for(unsigned int i=0;i<(anarace->getRealTimer())/30;++i)
		if((i%timesteps==0)&&(i>0))
		{
			std::ostringstream os;
			os << i/2 << ":" << 3*(i%2) << "0";
//			legend.push_back(new UI_StaticText(this, os.str(), Rect(Point(5+i*((getClientRectWidth()-20)/((anarace->getRealTimer())/30)), 6), Size(0,0)), Size(0,0), TIMESTEPS_TEXT_COLOR, SMALL_FONT, DO_NOT_ADJUST));
		}*/
	
	unsigned int unitCounter[UNIT_TYPE_COUNT][MAX_TIME];
	unsigned int height[UNIT_TYPE_COUNT];
	unsigned int lines[UNIT_TYPE_COUNT];
	
	std::priority_queue<unsigned int, std::vector<unsigned int> > endOfBuild;

	memset(unitCounter, 0, MAX_TIME * UNIT_TYPE_COUNT * sizeof(int));
	memset(height, 0, UNIT_TYPE_COUNT * sizeof(int));
	memset(lines, 0, UNIT_TYPE_COUNT * sizeof(int));

// ------ CALCULATE NUMBER OF ENTRIES FOR EACH FACILITY ------ 
// = maximum of force - availible for each facility
	for(std::list<PROGRAM>::const_iterator order = anarace->getProgramList().begin(); order != anarace->getProgramList().end(); ++order)
		if(anarace->getTimer() + order->getBT() <= order->getTime())
		{
// falls facility benoetigt wird und Zahl der zu dem Zeitpunkt vorhandenen Einheiten minus der verfuegbaren Einheiten > hoehe => setze hoehe auf diesen Wert		
			while((!endOfBuild.empty())&&(endOfBuild.top() > order->getTime()))
				endOfBuild.pop();
			if(order->getFacility())
			{
				if((anarace->getRace() == PROTOSS)&&(stats[anarace->getRace()][order->getUnit()].facility[0] == PROBE))
				{
					endOfBuild.push(order->getTime() - order->getBT());
					if(endOfBuild.size() > height[PROBE])
						height[PROBE] = endOfBuild.size();
				}
				else
				if(order->getUsedFacilityCount() > height[order->getFacility()])
					height[order->getFacility()] = order->getUsedFacilityCount();
			}
		// total - availible at that time equals used facilities
		}
	//=>  height[i] = max used i-facilities


//calculate number of lines per facility and adjust the height
	for(unsigned int i=UNIT_TYPE_COUNT;i--;)
	{
// at maximum MIN_HEIGHT items in one row
		while(height[i]>MIN_HEIGHT) {
			height[i]-=MIN_HEIGHT;
			++lines[i];
		}
		if(height[i] > 0)
			++lines[i];
		if(lines[i] > 1)
			height[i] = MIN_HEIGHT;
		if(height[i] == 0)
			height[i] = 1;
	}

// ------ CREATE A LIST OF NEEDED FACILITIES
	std::list<unsigned int> fac;
	for(std::list<PROGRAM>::const_iterator order = anarace->getProgramList().begin(); order != anarace->getProgramList().end(); ++order)
		if((anarace->getTimer() + order->getBT() <= order->getTime())&&(order->getFacility()>0))
		{
// search for 'untaken' facilities
			std::list<unsigned int>::iterator i;
			for(i = fac.begin(); (i!=fac.end())&&(*i != order->getFacility()); ++i);
// none found, create new
			if(i == fac.end())
				fac.push_back(order->getFacility());
		}

// ...and sort them (just an optical issue, scvs last)
	fac.sort();
// now put all together
	{
	unsigned int position = 0; 
	Rect edge;
	std::list<BoGraphLine*>::iterator j = boGraphLine.begin();
	for(std::list<unsigned int>::iterator i = fac.begin(); i!=fac.end(); ++i, ++j)
	{
		edge = Rect(Point(5, 10+position * (FONT_SIZE+10)), Size(getClientRectWidth()-10, lines[*i] * (FONT_SIZE+10)));
		
		if(j == boGraphLine.end())
		{
			BoGraphLine* t = new BoGraphLine(getScrollBar(), edge, anarace->getRace(), *i, lines[*i], height[*i]);
			boGraphLine.push_back(t);
			j = --boGraphLine.end();
		}
		else
		{
			(*j)->setOriginalRect(edge);
			(*j)->setFacility(*i);
			(*j)->setLines(lines[*i]);
			(*j)->setRace(anarace->getRace());
			(*j)->setLineHeight(height[*i]);
		}
		position += (*j)->getLines();
	}

	// let the window adjust to the last line

	if(boGraphLine.size())
		fitItemToRelativeClientRect(Rect(Point(5, position * (FONT_SIZE+10)), Size(getClientRectWidth()-10, (FONT_SIZE+10))),1);
	else
		fitItemToRelativeClientRect(Rect(), 1);
	}

	
// ... and finally the orders
	std::list<std::list<BoGraphEntry*>::iterator> entry;
	for(std::list<BoGraphLine*>::iterator i = boGraphLine.begin(); i!=boGraphLine.end(); ++i)
		entry.push_back((*i)->boGraphList.begin());
	
// go through the rectangles	
	unsigned int number = 0;
	for(std::list<PROGRAM>::const_iterator order = anarace->getProgramList().begin(); order != anarace->getProgramList().end(); ++order, ++number)
		if(anarace->getTimer() + order->getBT() <= order->getTime())
		{
			if(!order->getFacility())
				continue;
			Rect edge;
			
			// search through the boGraphList, result is an iterator on 'end()' or on an entry :o :/
			// TODO
			std::list<std::list<BoGraphEntry*>::iterator>::iterator j = entry.begin();
			std::list<BoGraphLine*>::iterator i = boGraphLine.begin();
			for(; i!=boGraphLine.end(); ++i, ++j)
				if((*i)->getFacility() == order->getFacility())
				{
					unsigned int k;
					for(k = 0; k < MAX_TIME; ++k)
						if(unitCounter[(*i)->getFacility()][k] <= order->getRealTime())
						{
							unitCounter[(*i)->getFacility()][k] = order->getRealTime() + order->getBT();
							break;
						}
					// k hat mit MAX_TIME im Grunde nichts zu tun, MAX_TIME waere nur das oberste Limit an simultanen Befehlen!
					edge = Rect(Point(
						1+( order->getRealTime()*(getClientRectWidth()-10)) / (anarace->getRealTimer()), 
							1+(k/MIN_HEIGHT)*(FONT_SIZE+10)+(k%MIN_HEIGHT)*(FONT_SIZE+10)/((*i)->getLineHeight())), 
						Size((order->getBT()*(getClientRectWidth()-10))/(anarace->getRealTimer()),
							(FONT_SIZE+10)/((*i)->getLineHeight())));
					break;
				}

#ifdef _SCC_DEBUG
			// no facility found!?
			if(i == boGraphLine.end())
				toLog("ERROR: GraphWindow::processList(), Facility not found");
#endif

			if(*j == (*i)->boGraphList.end())
			{
				BoGraphEntry* t = new BoGraphEntry(*i, edge, Size(0,0), *order, number); // TODO
				t->adjustRelativeRect(edge);
				t->setButtonColorsType(eButtonColorsType(BRIGHT_UNIT_TYPE_0_BUTTON+stats[(*anarace->getStartCondition())->getRace()][order->getUnit()].unitType));
				(*i)->boGraphList.push_back(t);
			} else
			if((**j)->program.getUnit() != order->getUnit())
			{
				std::list<BoGraphEntry*>::iterator k = *j;
				while(k != (*i)->boGraphList.end())
				{
					if((*k)->program.getUnit() == order->getUnit())
						break;
					++k;
				}
				if(k != (*i)->boGraphList.end()) // => Found, move the entry
				{
					BoGraphEntry* old = *k;
					*j = (*i)->boGraphList.insert(*j, old);
					++(*j);
					(*i)->boGraphList.erase(k);
					if(edge != old->getTargetRect())
					{
						old->adjustRelativeRect(edge);
						old->resetGradient();
					}
					old->setNumber(number);
					old->program = *order;
				} else // => not found, insert a new one
				{
					BoGraphEntry* t = new BoGraphEntry(*i, edge, Size(0,0), *order, number); // TODO
					t->adjustRelativeRect(edge);
					t->setButtonColorsType(eButtonColorsType(BRIGHT_UNIT_TYPE_0_BUTTON+stats[(*anarace->getStartCondition())->getRace()][order->getUnit()].unitType));
					*j = (*i)->boGraphList.insert(*j, t);
					++(*j);
				}
			} else // ok
//			  if((*entry)->getUnit() == (*order)->getUnit())
			{
				(**j)->program = *order;
				if(edge!=(**j)->getTargetRect())
				{
					(**j)->adjustRelativeRect(edge);
					(**j)->resetGradient();
				}
				(**j)->setNumber(number);
				++(*j);
			}
		}


// clear the remaining buttons
	std::list<std::list<BoGraphEntry*>::iterator>::iterator j = entry.begin();
	for(std::list<BoGraphLine*>::iterator i = boGraphLine.begin();  i!=boGraphLine.end(); ++i, ++j)
		while(*j != (*i)->boGraphList.end())
		{
			if(UI_Button::getCurrentButton() == (**j)) 
				UI_Button::resetButton();
			delete(**j);
			*j = (*i)->boGraphList.erase(*j);
		}


// --------------------------------- END BUILD ORDER GRAPH ------------------------------
}

void BoGraphWindow::checkForInfoWindow()
{
	selectedItem = -1;
	for(std::list<BoGraphLine*>::iterator i = boGraphLine.begin(); i!=boGraphLine.end(); ++i)
		if((*i)->getFacility()>0)
		{
			for(std::list<BoGraphEntry*>::iterator entry = (*i)->boGraphList.begin(); entry!=(*i)->boGraphList.end(); ++entry)
				if((*entry)->isCurrentlyHighlighted())
				{
					selectedItem = (*entry)->getNumber();
					return;
				}
		}
}



void BoGraphWindow::process()
{
	if(!isShown()) 
		return;
		
	UI_Window::process();
	
//	if(boGraphLine.size())
//	fitItemToRelativeClientRect(boGraphLine.back()->getRelativeRect(),1);//Rect(Point(5, 35+10 * (FONT_SIZE+10)), Size(getClientRectWidth()-10, (FONT_SIZE+10))),1);
	
	checkForInfoWindow();
	getScrollBar()->checkBoundsOfChildren(getAbsoluteClientRectUpperBound(), getAbsoluteClientRectLowerBound());
}

void BoGraphWindow::mouseHasMoved()
{
#if 0
	if(!getAbsoluteRect().Inside(mouse))
		return;
	ownMarkedUnit = 0;ownMarkedIP = 0;
	for(std::list<Order*>::const_iterator order = orderList->begin(); order != orderList->end(); ++order)
		if(anarace->getProgramFacility((*order)->getIP()))
		{
			Rect edge=Rect(getAbsolutePosition()+getRelativeClientRectPosition()+(*order)->brect.GetTopLeft(), (*order)->brect.GetSize());
			if(edge.Inside(mouse))
			{
				ownMarkedUnit=(*order)->getUnit();
				ownMarkedIP=(*order)->getIP();
				return;
			}
		}
#endif
}

void BoGraphWindow::draw(DC* dc) const
{
	if(!isShown()) 
		return;
	if(anarace==NULL) 
		return;

	if(checkForNeedRedraw())
	{
		UI_Window::drawWindow(dc);
	
		dc->SetFont(UI_Object::theme.lookUpFont(SMALL_FONT));
// and the time steps on the top
		dc->SetTextForeground(*theme.lookUpColor(TIMESTEPS_TEXT_COLOR));
// dc->SetPen(*BLACK_PEN); TODO
		unsigned int timesteps=((anarace->getRealTimer())/30)/10+1; // TODO <- wird 0? bei Protoss? :-/
		dc->SetPen(*theme.lookUpPen(GREEN_TIMESTEPS_PEN));
		for(unsigned int i=0;i<(anarace->getRealTimer())/30;++i)
			if(i%timesteps==0)
			{
				if(i>0) 
				{
					dc->DrawVerticalLine(getAbsoluteClientRectLeftBound()+5+i*((getClientRectWidth()-20)/(anarace->getRealTimer()/30)),
								   getAbsoluteClientRectUpperBound(), 
								   getAbsoluteClientRectLowerBound()-5);
					dc->DrawHorizontalLine(getAbsoluteClientRectLeftBound()+5+i*((getClientRectWidth()-20)/(anarace->getRealTimer()/30)), 
							getAbsoluteClientRectUpperBound(), 
							getAbsoluteClientRectLeftBound()+5+i*((getClientRectWidth()-20)/(anarace->getRealTimer()/30)) + 5);
				}
				
				std::ostringstream os;
				os.str("");
				os << i/2 << ":" << 3*(i%2) << "0";
				dc->DrawText(os.str(), getAbsoluteClientRectPosition()+Point(7+i*((getClientRectWidth()-20)/((anarace->getRealTimer())/30)), 1));
			}

// --------------------------------- END BUILD ORDER GRAPH ------------------------------
	}
	UI_Object::draw(dc);
	
}

void BoGraphWindow::assignAnarace(ANABUILDORDER* bograph_anarace) {
	anarace = bograph_anarace;
}


