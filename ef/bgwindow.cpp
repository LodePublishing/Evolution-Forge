#include "bgwindow.hpp"

#include <sstream>

BoGraphWindow::BoGraphWindow(const BoGraphWindow& object) :
	UI_Window((UI_Window)object),
//	boGraphList(),
	markedUnit(object.markedUnit),
	markedIP(object.markedIP),
	ownMarkedUnit(object.ownMarkedUnit),
	ownMarkedIP(object.ownMarkedIP),
	markAni(object.markAni),
	orderList(object.orderList),
	infoWindow(object.infoWindow),
	anarace(object.anarace)
{
	for(int i = BOGRAPH_MAX_LINES;i--;)
	{
		bograph[i].facility = object.bograph[i].facility;
		bograph[i].height = object.bograph[i].height;
		bograph[i].lines = object.bograph[i].lines;
		bograph[i].edge = object.bograph[i].edge;
	}
}

BoGraphWindow& BoGraphWindow::operator=(const BoGraphWindow& object)
{
	((UI_Window)(*this)) = ((UI_Window)object);
	markedUnit = object.markedUnit;
	markedIP = object.markedIP;
	ownMarkedUnit = object.ownMarkedUnit;
	ownMarkedIP = object.ownMarkedIP;
	markAni = object.markAni;
	orderList = object.orderList;
	infoWindow = object.infoWindow;
	anarace = object.anarace;
	for(int i = BOGRAPH_MAX_LINES;i--;)
	{
		bograph[i].facility = object.bograph[i].facility;
		bograph[i].height = object.bograph[i].height;
		bograph[i].lines = object.bograph[i].lines;
		bograph[i].edge = object.bograph[i].edge;
	}
	return(*this);
}

BoGraphWindow::BoGraphWindow(UI_Object* bograph_parent, ANARACE* bograph_anarace, InfoWindow* bograph_info_window, std::list<Order*>* bograph_order_list, const unsigned int bograph_window_number) :
	UI_Window(bograph_parent, BOGRAPH_WINDOW_TITLE_STRING, BO_GRAPH_WINDOW, bograph_window_number, SCROLLED),
//	boGraphList(),
	markedUnit(0),
	markedIP(0),
	ownMarkedUnit(0),
	ownMarkedIP(0),
	markAni(0),
	orderList(bograph_order_list),
	infoWindow(bograph_info_window),
	anarace(bograph_anarace)
{
//	resetData(); // TODO
}

BoGraphWindow::~BoGraphWindow()
{
	for(int j = 0; j < BOGRAPH_MAX_LINES; j++)
	{
		std::list<BoGraphEntry*>::iterator i = bograph[j].boGraphList.begin();
		while(i != bograph[j].boGraphList.end())
		{	
			delete(*i);
			i = bograph[j].boGraphList.erase(i);
		}
	}
}


void BoGraphWindow::checkForInfoWindow()
{
	return;
#if 0	
	for(std::list<BoGraphEntry*>::const_iterator bgEntry = boGraphList.begin(); bgEntry != boGraphList.end(); bgEntry++)
	{
		if((*bgEntry)->isCurrentlyHighlighted())
		{
//		  int row=((boInsertPoint>-1)&&((*order)->row>=boInsertPoint))*(boEndPoint-boInsertPoint);
//	  mouse on order in player reinschieben, da is ja auch orderList zuhause
//			Rect edge(getRelativeClientRectPosition()+(*bgEntry)->brect.GetTopLeft()/*-Point(0,getScrollY() TODO)*/,(*order)->brect.GetSize());
/*			if((fitItemToRelativeClientRect(edge)&& edge.Inside(controls.getCurrentPosition()-getAbsolutePosition())))
			{*/
				ownMarkedUnit = (*bgEntry)->getUnit();
				ownMarkedIP = (*bgEntry)->getIP();
				infoWindow->setUnit(ownMarkedUnit);
				infoWindow->setIP(ownMarkedIP);
				infoWindow->Show(1);
				infoWindow->setFreeMove();
				infoWindow->assignBg(*bgEntry);
				infoWindow->assignBo(NULL);
				if((*bgEntry)->getAbsolutePosition().x + 200 > getAbsolutePosition().x + getWidth())
					infoWindow->adjustRelativeRect(Rect(getRelativeRightBound() - 200, getRelativeLowerBound() + 5, 200, 110));
				else
					infoWindow->adjustRelativeRect(Rect((*bgEntry)->getAbsolutePosition().x, getRelativeLowerBound() + 5, 200, 110));
				return;
//			}*/
		}
	}
	#endif
}
	
void BoGraphWindow::resetData()
{
	markedUnit = 0;markedIP = 0;
	ownMarkedUnit = 0;ownMarkedIP = 0;
	for(int i=BOGRAPH_MAX_LINES;i--;)
	{
		bograph[i].facility = 0;
		bograph[i].height = 0;
		bograph[i].lines = 0;
		bograph[i].edge = Rect();
                std::list<BoGraphEntry*>::iterator j = bograph[i].boGraphList.begin();
                while(j != bograph[i].boGraphList.end())
              {
                        delete(*j);
                      j = bograph[i].boGraphList.erase(j);
                }
	}
}

void BoGraphWindow::processList()
{
	resetData();
	//markedUnit=0;markedIP=0;
	
	unsigned int unitCounter[UNIT_TYPE_COUNT][MAX_LENGTH];
	unsigned int height[UNIT_TYPE_COUNT];
	unsigned int lines[UNIT_TYPE_COUNT];
	unsigned int fac[BOGRAPH_MAX_LINES];
	unsigned int faccount = 0;

	for(int i=UNIT_TYPE_COUNT;i--;)
	{
		for(int j = 0; j < MAX_LENGTH; j++)
			unitCounter[i][j] = 0;
		height[i] = 0;
		lines[i] = 0;
	}
	for(int i=BOGRAPH_MAX_LINES;i--;)
		fac[i] = 0;

// ------ CALCULATE NUMBER OF ENTRIES FOR EACH FACILITY ------ 
// = maximum of force - availible for each facility
	for(list<Order*>::const_iterator order=orderList->begin(); order!=orderList->end(); ++order)
	{
		unsigned int IP = (*order)->getIP();
// falls facility benoetigt wird und Zahl der zu dem Zeitpunkt vorhandenen Einheiten minus der verfuegbaren Einheiten > hoehe => setze hoehe auf diesen Wert	    
		if(anarace->getProgramFacility(IP)&&
		// TODO: needed once / is lost etc. beachten!!!
		  (anarace->getProgramTotalCount(IP, anarace->getProgramFacility(IP))-anarace->getProgramAvailibleCount(IP,anarace->getProgramFacility(IP))>height[anarace->getProgramFacility(IP)]))
			 height[anarace->getProgramFacility(IP)]=anarace->getProgramTotalCount(IP,anarace->getProgramFacility(IP))-anarace->getProgramAvailibleCount(IP,anarace->getProgramFacility(IP));
		// total - availible at that time equals used facilities
	}
	//=>  height[i] = max used i-facilities


//calculate number of lines per facility and adjust the height
	for(int i=UNIT_TYPE_COUNT;i--;)
	{
// at maximum MIN_HEIGHT items in one row
		while(height[i]>MIN_HEIGHT) {
			height[i]-=MIN_HEIGHT;
			lines[i]++;
		}
		if(height[i] > 0)
			lines[i]++;
		if(lines[i] > 1)
			height[i] = MIN_HEIGHT;
		if(height[i] == 0) // TODO!
			height[i] = 1;
	}

// ------ CREATE A LIST OF NEEDED FACILITIES
// TODO WARUM = 1 und nicht = 0?
	faccount = 1;
	for(list<Order*>::const_iterator order=orderList->begin(); order!=orderList->end(); ++order)
	{
		if(anarace->getProgramFacility((*order)->getIP()))
		{
			unsigned int i;
			// search for 'untaken' facilities
			for(i = 1;(i < faccount)&&(fac[i] != anarace->getProgramFacility((*order)->getIP())); i++);

			if(i==faccount)
			{
				fac[i]=anarace->getProgramFacility((*order)->getIP());
				faccount++;
			}
		}
	}

// ...and sort them (just an optical issue, scvs last)
	std::sort(fac, fac+BOGRAPH_MAX_LINES); // Warnung?!
// now put all together
	int position = 0;
	for(int i = 0;i < BOGRAPH_MAX_LINES; i++)
	{
		bograph[position].facility = fac[i];
		bograph[position].lines = lines[fac[i]];
		bograph[position].height = height[fac[i]];
		position += lines[fac[i]];
	}

// ... and finally the orders
	std::list<BoGraphEntry*>::iterator entry[BOGRAPH_MAX_LINES];
	for(int i = BOGRAPH_MAX_LINES; i--;)
		entry[i] = bograph[i].boGraphList.begin();
		
	for(std::list<Order*>::const_iterator order = orderList->begin(); order != orderList->end(); ++order)
	{
		if(!anarace->getProgramFacility((*order)->getIP()))
			continue;
		Rect edge;
		int i;
		for(i = 0; i < BOGRAPH_MAX_LINES; i++)
			if(bograph[i].facility == anarace->getProgramFacility((*order)->getIP()))
			{
				int k;
				for(k = 0; k < MAX_LENGTH; k++)
					if(unitCounter[bograph[i].facility][k] <= anarace->getRealProgramTime((*order)->getIP()))
					{
						unitCounter[bograph[i].facility][k] = anarace->getRealProgramTime((*order)->getIP())+stats[anarace->getRace()][(*order)->getUnit()].BT;
						break;
					}
			       edge = Rect(getRelativeClientRectPosition() + Point( ( anarace->getRealProgramTime((*order)->getIP())*getClientRectWidth()) / (anarace->getRealTimer()), FONT_SIZE+11+(i+k/MIN_HEIGHT)*(FONT_SIZE+9)+(k%MIN_HEIGHT)*(FONT_SIZE+9)/bograph[i].height), Size(  (stats[anarace->getRace()][(*order)->getUnit()].BT/*anarace->getProgramBT(s)*/*getClientRectWidth())/(anarace->getRealTimer()), (FONT_SIZE+8)/(bograph[i].height)));
				break;
			}
 		
		if(entry[i] == bograph[i].boGraphList.end())
		{
			BoGraphEntry* t = new BoGraphEntry(this, edge, getRelativeClientRect(), (*order)->getUnit());
			t->setButton(eButton(UNIT_TYPE_0_BUTTON+stats[(*anarace->getStartCondition())->getRace()][(*order)->getUnit()].unitType));
			t->setIP((*order)->getIP());
			bograph[i].boGraphList.push_back(t);
		} else
		if((*entry[i])->getUnit() != (*order)->getUnit())
		{
			std::list<BoGraphEntry*>::iterator k = entry[i];
			while(k != bograph[i].boGraphList.end())
			{
				if((*k)->getUnit() == (*order)->getUnit())
					break;
				k++;
			}
			if(k != bograph[i].boGraphList.end()) // => Found, move the entry
			{
				BoGraphEntry* old = *k;
				entry[i] = bograph[i].boGraphList.insert(entry[i], old);
				entry[i]++;
				bograph[i].boGraphList.erase(k);
				old->adjustRelativeRect(edge);
				old->setIP((*order)->getIP());
			} else // => not found, insert a new one
			{
				BoGraphEntry* t = new BoGraphEntry(this, edge, getRelativeClientRect(), (*order)->getUnit());
				t->setButton(eButton(UNIT_TYPE_0_BUTTON+stats[(*anarace->getStartCondition())->getRace()][(*order)->getUnit()].unitType));
				t->setIP((*order)->getIP());
				entry[i] = bograph[i].boGraphList.insert(entry[i], t);
				entry[i]++;
			}
		} else // ok
//	      if((*entry)->getUnit() == (*order)->getUnit())
		{
			(*entry[i])->buttonPlacementArea = edge;
//			(*entry[i])->adjustRelativeRect(edge);
//			(*entry[i])->adjustButtonPlacementPosition();
//			(*entry[i])->adjustButtonPlacementSize();
			(*entry[i])->setIP((*order)->getIP());
			entry[i]++;
		}
	}

	for(int i = 0; i < BOGRAPH_MAX_LINES; i++)
	while(entry[i] != bograph[i].boGraphList.end())
	{
		if(UI_Object::currentButton == *entry[i]) UI_Object::currentButton = NULL;
		delete(*entry[i]);
		entry[i] = bograph[i].boGraphList.erase(entry[i]);
	}



#if 0
// ... and finally the orders
	std::list<BoGraphEntry*>::iterator entry = boGraphList.begin();
	for(std::list<Order*>::const_iterator order = orderList->begin(); order != orderList->end(); ++order)
	{
		if(!anarace->getProgramFacility((*order)->getIP()))
			continue;
		Rect edge;

		for(int i = 0; i < BOGRAPH_MAX_LINES; i++)
			if(bograph[i].facility == anarace->getProgramFacility((*order)->getIP()))
			{
				int k;
				for(k = 0; k < MAX_LENGTH; k++)
					if(unitCounter[bograph[i].facility][k] <= anarace->getRealProgramTime((*order)->getIP()))
					{
						unitCounter[bograph[i].facility][k] = anarace->getRealProgramTime((*order)->getIP())+stats[anarace->getRace()][(*order)->getUnit()].BT;
						break;
					}
			       edge = Rect(getRelativeClientRectPosition() + Point( ( anarace->getRealProgramTime((*order)->getIP())*getClientRectWidth()) / (anarace->getRealTimer()), FONT_SIZE+11+(i+k/MIN_HEIGHT)*(FONT_SIZE+9)+(k%MIN_HEIGHT)*(FONT_SIZE+9)/bograph[i].height), Size(  (stats[anarace->getRace()][(*order)->getUnit()].BT/*anarace->getProgramBT(s)*/*getClientRectWidth())/(anarace->getRealTimer()), (FONT_SIZE+8)/(bograph[i].height)));
				break;
			}
 		
		if(entry == boGraphList.end())
		{
			BoGraphEntry* t = new BoGraphEntry(this, edge, getRelativeClientRect(), (*order)->getUnit());
			t->setButton(eButton(UNIT_TYPE_0_BUTTON+stats[(*anarace->getStartCondition())->getRace()][(*order)->getUnit()].unitType));
			t->setIP((*order)->getIP());
			boGraphList.push_back(t);
		} else
		if((*entry)->getUnit() != (*order)->getUnit())
		{
			std::list<BoGraphEntry*>::iterator k = entry;
			while(k!=boGraphList.end())
			{
				if((*k)->getUnit() == (*order)->getUnit())
					break;
				k++;
			}
			if(k!=boGraphList.end()) // => Found, move the entry
			{
				BoGraphEntry* old = *k;
				entry = boGraphList.insert(entry, old);
				entry++;
				boGraphList.erase(k);
				old->adjustRelativeRect(edge);
				old->setIP((*order)->getIP());
			} else // => not found, insert a new one
			{
				BoGraphEntry* t = new BoGraphEntry(this, edge, getRelativeClientRect(), (*order)->getUnit());
				t->setButton(eButton(UNIT_TYPE_0_BUTTON+stats[(*anarace->getStartCondition())->getRace()][(*order)->getUnit()].unitType));
				t->setIP((*order)->getIP());
				entry = boGraphList.insert(entry, t);
				entry++;
			}
		} else // ok
//	      if((*entry)->getUnit() == (*order)->getUnit())
		{
			(*entry)->buttonPlacementArea=edge;
			(*entry)->adjustRelativeRect(edge);
			(*entry)->setIP((*order)->getIP());
			entry++;
		}
	}

	while(entry != boGraphList.end())
	{
		if(UI_Object::currentButton == *entry) UI_Object::currentButton = NULL;
		delete(*entry);
		entry = boGraphList.erase(entry);
	}
#endif
/*	
			




			if((*order)->second.bonew)
			{
				BoGraphEntry* t = new BoGraphEntry(this, edge, getRelativeClientRect(), (*order)->second.getUnit());
				entry = boGraphList.insert(entry, t);
				(*entry)->setButton(eButton(UNIT_TYPE_0_BUTTON+stats[(*anarace->getStartCondition())->getRace()][(*order)->second.getUnit()].unitType));
			} else if((*order)->second.getUnit() != (*entry)->getUnit())
			{
				delete *entry;
				entry = boGraphList.erase(entry);
				wasDeleted=true;
			} else
			{
				(*entry)->adjustRelativeRect(edge);
				(*entry)->setButton(eButton(UNIT_TYPE_0_BUTTON+stats[(*anarace->getStartCondition())->getRace()][(*order)->second.getUnit()].unitType));
				entry++;
			}
		
//			if(insideRelativeClientRect(edge))
			{
//				if(((markedUnit>0)&&((*order)->second.getUnit()==markedUnit))||
//				   ((markedIP>0)&&((*order)->second.getIP()==markedIP))||
//				   ((ownMarkedUnit>0)&&((*order)->second.getUnit()==ownMarkedUnit))||
//				   ((ownMarkedIP>0)&&((*order)->second.getIP()==ownMarkedIP)))
//					dc->SetBrush(*theme.lookUpBrush(BRIGHT_CONTINUE_BUTTON_BRUSH));
//				else
//					dc->SetBrush(*theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[anarace->getRace()][(*order)->second.getUnit()].unitType)));
				
//				dc->SetPen(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN+stats[anarace->getRace()][(*order)->second.getUnit()].unitType)));
			//	(*entry)->setButton(eButton(UNIT_TYPE_0_BUTTON+stats[(*anarace->getStartCondition())->getRace()][(*order)->second.getUnit()].unitType));
//				dc->DrawEdgedRoundedRectangle(Rect(getAbsolutePosition()+edge.GetTopLeft(), edge.GetSize()),3);
			}
		}
	}*/
// assign the coordinates for the lines where the orders are printed on
	int j=0;
	for(int i=0;i<BOGRAPH_MAX_LINES;i++)
		if(bograph[i].facility>0)
		{
			bograph[i].edge = Rect(Point(0, j*(FONT_SIZE+9)), Size(10, FONT_SIZE+10+(bograph[i].lines-1) * (FONT_SIZE+9)));
			j += bograph[i].lines;
		}


	Rect edge_lastline = Rect(Point(1, j*(FONT_SIZE+9)), Size(10, FONT_SIZE+10));
// let the window adjust to the last line
	fitItemToRelativeClientRect(edge_lastline, 1);
}


void BoGraphWindow::process()
{
	if(!isShown()) 
		return;
		
	if(!orderList) 
		return;

	UI_Window::process();
	
	checkForInfoWindow();
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

	if(!orderList) 
		return;
	// now print the lines...
	UI_Window::draw(dc);
	dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
	for(int i = 0; i<BOGRAPH_MAX_LINES; i++)
		if(bograph[i].facility>0)
	//		for(int j=0;j<bograph[i].lines;j++)
			{
				Rect rec=Rect(bograph[i].edge.GetTopLeft() + getAbsoluteClientRectPosition() + Size(0,(FONT_SIZE+10)), Size(getClientRectWidth(), bograph[i].edge.GetHeight()));
//				if(insideAbsoluteClientRect(rec)) // TODO
				// TODO BUTTONS drausmachen...
				{
				//	if(j%2==0)
				//		dc->SetBrush(*theme.lookUpBrush(BO_DARK_BRUSH));
				//	else 
					dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
					dc->DrawRectangle(rec);
				}
			}
//	if((signed int)lastbographY > getAbsoluteClientRectUpperBound()+(signed int)(FONT_SIZE+10)) // TODO
	{
// and the time steps on the top
		dc->SetTextForeground(*theme.lookUpColor(TIMESTEPS_TEXT_COLOR));
// dc->SetPen(*BLACK_PEN); TODO
		unsigned int timesteps=((anarace->getRealTimer())/30)/10+1; // TODO <- wird 0? bei Protoss? :-/
		dc->SetPen(*theme.lookUpPen(GREEN_TIMESTEPS_PEN));
		for(unsigned int i=0;i<(anarace->getRealTimer())/30;i++)
			if(i%timesteps==0)
			{
				if(i>0) 
				{
					dc->DrawVerticalLine(getAbsoluteClientRectLeftBound()+5+i*((getClientRectWidth()-20)/(anarace->getRealTimer()/30)),
							       getAbsoluteClientRectUpperBound()+(FONT_SIZE+5), getAbsoluteClientRectLowerBound()-5);
					dc->DrawHorizontalLine(getAbsoluteClientRectLeftBound()+5+i*((getClientRectWidth()-20)/(anarace->getRealTimer()/30)), getAbsoluteClientRectUpperBound()+(FONT_SIZE+5), getAbsoluteClientRectLeftBound()+5+i*((getClientRectWidth()-20)/(anarace->getRealTimer()/30)) + 5);
				}
				
				std::ostringstream os;
				os << i/2 << ":" << 3*(i%2) << "0";
				dc->DrawText(os.str(), getAbsoluteClientRectPosition()+Point(5+i*((getClientRectWidth()-20)/((anarace->getRealTimer())/30)), 0));
			}

// --------------------------------- END BUILD ORDER GRAPH ------------------------------
	}
#if 0	
// ... and finally the orders
	for(std::map<long, Order>::const_iterator order=orderList->begin(); order!=orderList->end(); ++order)
	{
		if(anarace->getProgramFacility((*order)->second.getIP()))
		{
			Rect edge=Rect(getRelativeClientRectPosition()+(*order)->second.brect.GetTopLeft(), (*order)->second.brect.GetSize());
//			if(insideRelativeClientRect(edge))
			{
				if(((markedUnit>0)&&((*order)->second.getUnit()==markedUnit))||
				   ((markedIP>0)&&((*order)->second.getIP()==markedIP))||
//				   ((ownMarkedUnit>0)&&((*order)->second.getUnit()==ownMarkedUnit))||
				   ((ownMarkedIP>0)&&((*order)->second.getIP()==ownMarkedIP)))
					dc->SetBrush(*theme.lookUpBrush(BRIGHT_CONTINUE_BUTTON_BRUSH));
				else
					dc->SetBrush(*theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[anarace->getRace()][(*order)->second.getUnit()].unitType)));
				
				dc->SetPen(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN+stats[anarace->getRace()][(*order)->second.getUnit()].unitType)));
				dc->DrawEdgedRoundedRectangle(Rect(getAbsolutePosition()+edge.GetTopLeft(), edge.GetSize()),3);
			}
		}
	}
	#endif
	//finally print the legend
	
	dc->SetFont(UI_Object::theme.lookUpFont(SMALL_MIDDLE_NORMAL_FONT));
	dc->SetTextForeground(*theme.lookUpColor(WINDOW_TEXT_COLOR));
	for(int i=0; i<BOGRAPH_MAX_LINES; i++)
		if(bograph[i].facility>0)
			dc->DrawText(" "+*UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+bograph[i].facility+UNIT_NULL_STRING)), getAbsoluteClientRectPosition()+Point(0, 4+(i+1)*(FONT_SIZE+10)));
}

void BoGraphWindow::assignAnarace(ANARACE* bograph_anarace)
{
	anarace = bograph_anarace;
}

const unsigned int BoGraphWindow::getMarkedIP() const 
{
#ifdef _SCC_DEBUG
    if(ownMarkedIP > MAX_LENGTH) {
	toLog("DEBUG: (BoGraphWindow::getMarkedIP): Value ownMarkedIP out of range.");return(0);
    }
#endif
	return(ownMarkedIP);
}

void BoGraphWindow::setMarkedIP(const unsigned int marked_ip) 
{
#ifdef _SCC_DEBUG
    if(marked_ip > MAX_LENGTH) {
	toLog("DEBUG: (BoGraphWindow::setMarkedIP): Value marked_ip out of range.");return;
	}
#endif
	markedIP = marked_ip;
}

const unsigned int BoGraphWindow::getMarkedUnit() const 
{
#ifdef _SCC_DEBUG
    if(ownMarkedUnit >= UNIT_TYPE_COUNT) {
	toLog("DEBUG: (BoGraphWindow::getMarkedUnit): Value ownMarkedUnit out of range.");return(0);
    }
#endif
	return(ownMarkedUnit);
}

void BoGraphWindow::setMarkedUnit(const unsigned int marked_unit) {
#ifdef _SCC_DEBUG
    if(marked_unit >= UNIT_TYPE_COUNT) {
	toLog("DEBUG: (BoGraphWindow::setMarkedUnit): Value marked_unit out of range.");return;
    }
#endif
	markedUnit = marked_unit;
}

