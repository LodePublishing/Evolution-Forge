#include "bgwindow.hpp"
#include "configuration.hpp"
#include <sstream>

const unsigned int MIN_HEIGHT = 2;

BoGraphWindow::BoGraphWindow(UI_Object* bograph_parent, const unsigned int game_number, const unsigned int game_max, const unsigned int player_number, const unsigned int player_max) :
	UI_Window(bograph_parent, BOGRAPH_WINDOW_TITLE_STRING, theme.lookUpPlayerRect(BUILD_ORDER_GRAPH_WINDOW, game_number, game_max, player_number, player_max), theme.lookUpPlayerMaxHeight(BUILD_ORDER_GRAPH_WINDOW, game_number, game_max, player_number, player_max), SCROLLED, AUTO_SIZE_ADJUST, Rect(0, 15, 1000, 1000)),
	markAni(0),
	anarace(NULL),
	selectedItems(),
	gameNumber(game_number),
	gameMax(game_max),
	playerNumber(player_number),
	playerMax(player_max),
	lastRace(TERRA),
	totalTime(1)
{
	addHelpButton(DESCRIPTION_BOGRAPH_WINDOW_CHAPTER);
}

BoGraphWindow::~BoGraphWindow()
{
	for(std::list<BoGraphLine*>::iterator i = boGraphLine.begin(); i != boGraphLine.end(); ++i)
		delete(*i);
}

void BoGraphWindow::resetData()
{
	for(std::list<BoGraphLine*>::iterator i = boGraphLine.begin(); i != boGraphLine.end(); ++i)
		(*i)->resetData();
	processList();
}

void BoGraphWindow::setMode(const unsigned int game_number, const unsigned int game_max, const unsigned int player_number, const unsigned int player_max)
{
	if((game_number == gameNumber) && (game_max == gameMax) && (player_number == playerNumber) && (player_max == playerMax))
		return;
	gameNumber = game_number;
	gameMax = game_max;
	playerNumber = player_number;
	playerMax = player_max;
}


void BoGraphWindow::reloadOriginalSize()
{
	setOriginalRect(UI_Object::theme.lookUpPlayerRect(BUILD_ORDER_GRAPH_WINDOW, gameNumber, gameMax, playerNumber, playerMax));
	setMaxHeight(UI_Object::theme.lookUpPlayerMaxHeight(BUILD_ORDER_GRAPH_WINDOW, gameNumber, gameMax, playerNumber, playerMax));

	UI_Window::reloadOriginalSize();
	processList();
}


void BoGraphWindow::setSelected(const std::list<unsigned int>& selected)
{
	for(std::list<unsigned int>::const_iterator j = selected.begin(); j != selected.end(); ++j)
		for(std::list<BoGraphLine*>::iterator i = boGraphLine.begin(); i != boGraphLine.end(); ++i)
			(*i)->checkSelected(*j);
}

void BoGraphWindow::processList()
{
	if(!isShown())
		return;
	if(anarace==NULL)
		return;

	totalTime = anarace->getRealTimer();
	if(totalTime == coreConfiguration.getMaxTime())
	{
		totalTime = 0;
		for(std::list<PROGRAM>::const_iterator t = anarace->getProgramList().begin(); t != anarace->getProgramList().end(); ++t)
			if(t->getRealTime() + t->getBT() > totalTime)
				totalTime = t->getRealTime() + t->getBT();
	}
	
	unsigned int height[LAST_UNIT];
	unsigned int lines[LAST_UNIT];
	unsigned int unitCounter[LAST_UNIT][MAX_LENGTH];
	memset(unitCounter, 0, MAX_LENGTH * LAST_UNIT * sizeof(int));

	std::priority_queue<unsigned int, std::vector<unsigned int> > endOfBuild;

	memset(height, 0, LAST_UNIT * sizeof(int));
	memset(lines, 0, LAST_UNIT * sizeof(int));
	eRace my_race = anarace->getRace();

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
				if((my_race == PROTOSS)&&(stats[PROTOSS][order->getUnit()].facility[0] == PROBE))
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
	for(unsigned int i=LAST_UNIT; i--;)
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

// not yet availible times:
//	std::list<std::list<Not_Availible> > not_availible_list;
	std::list<unsigned int > first_availible;
	{
		for(std::list<unsigned int>::iterator i = fac.begin(); i!=fac.end(); ++i)
		{
			unsigned int first_availible_item;
			if((*anarace->getStartCondition())->getLocationTotal(GLOBAL, *i)>0)
			{
				first_availible.push_back(0);
				continue;
			}
			else first_availible_item = anarace->getRealTimer();
			
			for(std::list<PROGRAM>::const_iterator order = anarace->getProgramList().begin(); order != anarace->getProgramList().end(); ++order)
				if((order->getUnit() == *i) || (stats[my_race][order->getUnit()].create == *i))
				{
					if(order->getRealTime() + order->getBT() < first_availible_item)
						first_availible_item = order->getRealTime() + order->getBT();
				}
			first_availible.push_back(1+( first_availible_item*(getClientRectWidth()-10)) / totalTime);
		}
		
	}

					
				/*	unsigned int begin = 1 + (order->getRealTime()*(getClientRectWidth()-10)) / totalTime;
					unsigned int end = begin + (order->getBT()*(getClientRectWidth()-10)) / totalTime;
					bool found_one = false;
					for(std::list<Not_Availible>::iterator j = not_availible.begin(); j != not_availible.end(); ++j)
					{
						if((j->begin < begin) && (j->end > end))
						{
							found_one = true;
							break;// => ignore;
						}
						if((j->begin < begin) && (j->end < end))
						{
							found_one = true;
							j->end = end;
							break;
						}
						if((j->begin > begin) && (j->end > end))
						{
							found_one = true;
							j->begin = begin;
							break;
						}
						if((j->begin > begin) && (j->end < end))
						{
							found_one = true;
							j->begin = begin;
							j->end = end;
							break;
						}
					}
					if(!found_one)
					{
						Not_Availible not_availible_item;
						not_availible_item.begin = begin;
						not_availible_item.end = end;
						not_availible.push_back(not_availible_item);
					}
				}
			not_availible_list.push_back(not_availible);
		}
	}*/


	



// now put all together
	{
	unsigned int position = 0; 
	Rect edge;
	std::list<BoGraphLine*>::iterator j = boGraphLine.begin();
//	std::list<std::list<Not_Availible> >::const_iterator k = not_availible_list.begin();
	std::list<unsigned int>::const_iterator k = first_availible.begin();
	for(std::list<unsigned int>::iterator i = fac.begin(); i!=fac.end(); ++i, ++j, ++k)
	{
		edge = Rect(Point(5, 10+position * (FONT_SIZE+11)), Size(getClientRectWidth()-10, lines[*i] * (FONT_SIZE+11)));
		
		if(j == boGraphLine.end())
		{
			BoGraphLine* t = new BoGraphLine(getScrollBar(), edge, my_race, *i, lines[*i], height[*i]);
			t->firstAvailible = *k;
			boGraphLine.push_back(t);
			j = --boGraphLine.end();
		}
		else
		{
			(*j)->setOriginalRect(edge);
			(*j)->setFacility(*i);
			(*j)->setLines(lines[*i]);
			(*j)->setRace(my_race);
			(*j)->setLineHeight(height[*i]);
			(*j)->firstAvailible = *k;
		}
		position += (*j)->getLines();
	}

	// let the window adjust to the last line

	if(boGraphLine.size())
		fitItemToRelativeClientRect(Rect(Point(5, position * (FONT_SIZE+11)), Size(getClientRectWidth()-10, (FONT_SIZE+11))), true);
	else
		fitItemToRelativeClientRect(Rect(), true);
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
			
			unsigned int my_unit = order->getUnit();

			std::ostringstream os;
			{
				os << "$" << formatTime(order->getRealTime(), efConfiguration.getGameSpeed()) << " - " << formatTime(order->getRealTime() + order->getBT(), efConfiguration.getGameSpeed()) << "$: " << UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*my_race+my_unit)) << "#" << "  " << UI_Object::theme.lookUpFormattedString(BOWINDOW_BUILD_TIME_STRING, stats[my_race][my_unit].BT/(efConfiguration.getGameSpeed()*3+6)) << "#";
				if(stats[my_race][my_unit].minerals>0)
					os << "  $" << stats[my_race][my_unit].minerals/100 << "$ " << UI_Object::theme.lookUpString(BOWINDOW_MINERALS_STRING) << "#";
				if(stats[my_race][my_unit].gas>0)
					os << "  $" << stats[my_race][my_unit].gas/100 << "$ " << UI_Object::theme.lookUpString(BOWINDOW_GAS_STRING) << "#";
			}
			
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
						1+( order->getRealTime()*(getClientRectWidth()-10)) / totalTime, 
							1+(k/MIN_HEIGHT)*(FONT_SIZE+11)+(k%MIN_HEIGHT)*(FONT_SIZE+11)/((*i)->getLineHeight())), 
						Size((order->getBT()*(getClientRectWidth()-10)) / totalTime,
							(FONT_SIZE+10)/((*i)->getLineHeight())));
					break;
				}

#ifdef _SCC_DEBUG
			// no facility found!?
			if(i == boGraphLine.end())
				toErrorLog("ERROR (BoGraphWindow::processList()): Facility not found");
#endif

			if(*j == (*i)->boGraphList.end())
			{
				BoGraphEntry* t = new BoGraphEntry(*i, edge, Size(0,0), *order, number); // TODO
				t->adjustRelativeRect(edge);
				t->setButtonColorsType(eButtonColorsType(BRIGHT_UNIT_TYPE_0_BUTTON+stats[my_race][my_unit].unitType));
				t->updateToolTip(os.str());
				(*i)->boGraphList.push_back(t);
			} else
			if((**j)->program.getUnit() != my_unit)
			{
				std::list<BoGraphEntry*>::iterator k = *j;
				while(k != (*i)->boGraphList.end())
				{
					if((*k)->program.getUnit() == my_unit)
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
					old->updateToolTip(os.str());
					old->setNumber(number);
					old->program = *order;
				} else // => not found, insert a new one
				{
					BoGraphEntry* t = new BoGraphEntry(*i, edge, Size(0,0), *order, number); // TODO
					t->adjustRelativeRect(edge);
					t->setButtonColorsType(eButtonColorsType(BRIGHT_UNIT_TYPE_0_BUTTON+stats[my_race][my_unit].unitType));
					t->updateToolTip(os.str());
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
				(**j)->updateToolTip(os.str()); // ?
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
	selectedItems.clear();
	for(std::list<BoGraphLine*>::iterator i = boGraphLine.begin(); i!=boGraphLine.end(); ++i)
		if((*i)->getFacility()>0)
		{
			for(std::list<BoGraphEntry*>::iterator entry = (*i)->boGraphList.begin(); entry!=(*i)->boGraphList.end(); ++entry)
				if((*entry)->isCurrentlyHighlighted())
				{
					selectedItems.push_back( (*entry)->getNumber() );
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
//	fitItemToRelativeClientRect(boGraphLine.back()->getRelativeRect(),1);//Rect(Point(5, 35+10 * (FONT_SIZE+10)), Size(getClientRectWidth()-10, (FONT_SIZE+10))), true);
	
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
			Rect edge=Rect(getAbsolutePosition()+getRelativeClientRectPosition()+(*order)->brect.getTopLeft(), (*order)->brect.getSize());
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
	
		dc->setFont(UI_Object::theme.lookUpFont(SMALL_FONT));
// and the time steps on the top
		dc->setTextForeground(*theme.lookUpColor(TIMESTEPS_TEXT_COLOR));
// dc->setPen(*BLACK_PEN); TODO
		unsigned int time = totalTime / (6+efConfiguration.getGameSpeed()*3);
		unsigned int timesteps=(time/30)/10+1; // TODO <- wird 0? bei Protoss? :-/ ?
		dc->setPen(*theme.lookUpPen(DASHED_MINERALS_PEN));
		unsigned int y = getAbsoluteClientRectLowerBound() - 5;
		std::list<BoGraphLine*>::const_iterator j = boGraphLine.end();
		if(j!=boGraphLine.begin())
		{
			--j;
			if(y > (*j)->getAbsoluteLowerBound() - 2)
				y = (*j)->getAbsoluteLowerBound() - 2;
		}
		for(unsigned int i=0;i<time/30;++i)
			if(i%timesteps==0)
			{
				if(i>0) 
				{
					dc->DrawVerticalLine(getAbsoluteClientRectLeftBound()+30*i*(getClientRectWidth()-10)/time,
								   getAbsoluteClientRectUpperBound(), 
								   y);
					dc->DrawHorizontalLine(getAbsoluteClientRectLeftBound()+30*i*(getClientRectWidth()-10)/time, 
							getAbsoluteClientRectUpperBound(), 
							getAbsoluteClientRectLeftBound()+30*i*(getClientRectWidth()-10)/time + 5);
				}
				
				std::ostringstream os;
				os.str("");
				os << i/2 << ":" << 3*(i%2) << "0";
				dc->DrawText(os.str(), getAbsoluteClientRectPosition()+Point(2+30*i*(getClientRectWidth()-10)/time, 1));
			}

// --------------------------------- END BUILD ORDER GRAPH ------------------------------
	}
	UI_Object::draw(dc);
	
}

void BoGraphWindow::assignAnarace(ANABUILDORDER* bograph_anarace) 
{
	bool race_has_changed = false;
	if((anarace==NULL)||(lastRace != bograph_anarace->getRace()))
		race_has_changed = true;
	anarace = bograph_anarace;
	if(race_has_changed)
	{
		lastRace = anarace->getRace();
		for(std::list<BoGraphLine*>::iterator i = boGraphLine.begin(); i != boGraphLine.end(); ++i)
			delete(*i);
		boGraphLine.clear();
	}
}

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


