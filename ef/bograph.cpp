#include "bograph.hpp"

BoGraphWindow::BoGraphWindow(UI_Object* parent, ANARACE* anarace, InfoWindow* infoWindow, map<long, Order>* orderList, const int windowNumber):UI_Window(parent, BOGRAPH_WINDOW_TITLE_STRING, BO_GRAPH_WINDOW, windowNumber, SCROLLED)
{
	this->orderList=orderList;
	this->infoWindow=infoWindow;
	this->anarace=anarace;
	resetData();
}

BoGraphWindow::~BoGraphWindow()
{
}


void BoGraphWindow::setMarkedUnit(int unit)
{
	markedUnit=unit;
}

void BoGraphWindow::checkForInfoWindow()
{
	for(map<long, Order>::const_iterator order=orderList->begin(); order!=orderList->end(); ++order)
	{
//	  int row=((boInsertPoint>-1)&&(order->row>=boInsertPoint))*(boEndPoint-boInsertPoint);
//	  mouse on order in player reinschieben, da is ja auch orderList zuhause
		Rect edge(getRelativeClientRectPosition()+order->second.brect.GetPosition()/*-Point(0,getScrollY() TODO)*/,order->second.brect.GetSize());
		if((fitItemToRelativeClientRect(edge)&&edge.Inside(controls.getCurrentPosition()-getAbsolutePosition())))
		{
//window[INFO_WINDOW]->adjustWindow(Rect(Point(window[INFO_WINDOW]->getPosition().x,500)/*order->rect.GetY()+getClientRectUpperBound()-getScrollY()+((boInsertPoint>-1)&&(order->row>=boInsertPoint))*(boEndPoint-boInsertPoint)*(FONT_SIZE+5)-1)*/,window[INFO_WINDOW]->getSize()));
			infoWindow->setUnit(order->second.getUnit());
			infoWindow->setIP(order->second.getIP());
			infoWindow->setBx(order->second.brect.x);
			infoWindow->setBWidth(order->second.brect.width);
			infoWindow->Show(1);
			return;
		}
	}
}
	
void BoGraphWindow::resetData()
{
	markedUnit=0;
	lastbographY=getAbsoluteClientRectUpperBound();
	for(int i=20;i--;)
	{
		bograph[i].type=0;
		bograph[i].position=0;
		bograph[i].height=0;
		bograph[i].lines=0;
		bograph[i].edge=Rect(0,0,0,0);
	}
}

void BoGraphWindow::process()
{
	if(!shown) return;
	if(!orderList) return;

	markedUnit=0;
	
	UI_Window::process();
	
	checkForInfoWindow();

	int fac[20];
	int unitCounter[UNIT_TYPE_COUNT][MAX_LENGTH];
	int height[UNIT_TYPE_COUNT];
	int lines[UNIT_TYPE_COUNT];
	int faccount=0;
																				
																				
	for(int i=UNIT_TYPE_COUNT;i--;) 
	{
		for(int j=0;j<MAX_LENGTH;j++)
			unitCounter[i][j]=0;
		height[i]=0;
		lines[i]=0;
	}
	
	for(int i=20;i--;)
	{
		bograph[i].type=0;
		bograph[i].position=0;
		bograph[i].height=0;
		bograph[i].lines=0;
		fac[i]=0;
	}
																				
//calculate maximum height per facility => height is after that the maximum of force - availible for each facility
	for(map<long, Order>::const_iterator order=orderList->begin(); order!=orderList->end(); ++order)
	{
		int IP=order->second.getIP();
// falls facility benoetigt wird und Zahl der zu dem Zeitpunkt vorhandenen Einheiten minus der verfuegbaren Einheiten > hoehe => setze hoehe auf diesen Wert
		if(anarace->getProgramFacility(IP)&&
		  (anarace->getProgramTotalCount(IP, anarace->getProgramFacility(IP))-anarace->getProgramAvailibleCount(IP,anarace->getProgramFacility(IP))>height[anarace->getProgramFacility(IP)]))
			 height[anarace->getProgramFacility(IP)]=anarace->getProgramTotalCount(IP,anarace->getProgramFacility(IP))-anarace->getProgramAvailibleCount(IP,anarace->getProgramFacility(IP));
		// total - availible at that time equals used facilities
	}
	//=>  height[i] = max used i-facilities
																				
//calculate number of lines per facility and adjust the height
	for(int i=UNIT_TYPE_COUNT;i--;)
	{
// at maximum MIN_HEIGHT items in one row
		while(height[i]>MIN_HEIGHT)	{
			height[i]-=MIN_HEIGHT;
			lines[i]++;
		}
		if(height[i]>0) 
			lines[i]++;
		if(lines[i]>1)
			height[i]=MIN_HEIGHT;
	}
																				
//make a list of facilities that are needed...
// TODO WARUM = 1 und nicht = 0?
	faccount=1;
	for(map<long, Order>::const_iterator order=orderList->begin(); order!=orderList->end(); ++order)
	{
		if(anarace->getProgramFacility(order->second.getIP()))
		{
			int i;
			// search for 'untaken' facilities
			for(i=1;(i<faccount)&&(fac[i]!=anarace->getProgramFacility(order->second.getIP()));i++);
					
			if(i==faccount)
			{
				fac[i]=anarace->getProgramFacility(order->second.getIP());
				faccount++;
			}
		}
	}
																				
// ...and sort them (just an optical issue, scvs last)
	for(int i=0;i<20;i++)
		if(fac[i])
			for(int j=0;j<i;j++)
				if((fac[j]>0)&&(fac[i]>fac[j])) {
						int temp=fac[i];fac[i]=fac[j];fac[j]=temp;
					}
																				
// now put all together
	int position=0;
	for(int i=0;i<20;i++)
	{
		bograph[position].type=fac[i];
		bograph[position].lines=lines[fac[i]];
		bograph[position].height=height[fac[i]];
		position+=lines[fac[i]];
	}

// create a sorted by IP - list
	orderCount=0;
	for(map<long, Order>::iterator order=orderList->begin(); order!=orderList->end(); ++order) {
		sortedList[orderCount]=&(order->second);
		orderCount++;
	}

	for(int i=0;i<orderCount;i++)
		for(int j=0;j<i;j++)
			if(sortedList[i]->getIP() > sortedList[j]->getIP())
				// > =>  earlier! first IP is MAX_LENGTH
			{
				Order* temp = sortedList[i];
				sortedList[i]=sortedList[j];
				sortedList[j]=temp;	
			}
	
	int hoehe=0;
//	for(map<long, Order>::iterator order=orderList->begin(); order!=orderList->end(); ++order)
	for(int k=0;k<orderCount;k++)
	{
		if(anarace->getProgramFacility(sortedList[k]->getIP()))
			for(int i=0;i<20;i++)
				if(bograph[i].type==anarace->getProgramFacility(sortedList[k]->getIP()))
				{
//order->time muesste vorsortiert sein
// hoehen! Positionen anordnen damits keine ueberschneidungen gibt

		// calculate the y-position of this specific order, THIS HAS TO BEGIN WITH 0 -> MAX_LENGTH, DO NOT USE -- OPTIMIZATION!
					for(int j=0;j<MAX_LENGTH;j++)
						if(unitCounter[bograph[i].type][j]<=anarace->getProgramTime(sortedList[k]->getIP()))
						{
// if we have found a unitCounter which has a lower time (all unitCounter start with 0) we set the unit time to starttime + buildtime
// at the end all unitCounter have the time at which programTime ... TODO
// the items are in REAL TIME, i.e. time 0 is the beginning, ga->maxTime is the end
							
							unitCounter[bograph[i].type][j]=anarace->getProgramTime(sortedList[k]->getIP())+stats[anarace->getRace()][sortedList[k]->getUnit()/*anarace->getPhaenoCode(sortedList[k]->getIP())*/].BT;
							hoehe=j;
							j=MAX_LENGTH;
						}

					
					Rect t=Rect(
Point( ( anarace->getProgramTime(sortedList[k]->getIP())*getClientRectWidth())/(anarace->getRealTimer()),
 	   (1+i+hoehe/MIN_HEIGHT)*(FONT_SIZE+5)+(hoehe%MIN_HEIGHT)*(FONT_SIZE+4)/bograph[i].height),

Size(  (stats[anarace->getRace()][sortedList[k]->getUnit()/*anarace->getPhaenoCode(sortedList[k]->getIP())*/].BT/*anarace->getProgramBT(s)*/*getClientRectWidth())/(anarace->getRealTimer()),
	 (FONT_SIZE+4)/(bograph[i].height)));
					if(t!=sortedList[k]->btarget)
						sortedList[k]->bstart=sortedList[k]->brect;
					sortedList[k]->btarget=t;
					if(sortedList[k]->bonew)
					{
						sortedList[k]->brect=sortedList[k]->btarget;
						sortedList[k]->bstart=sortedList[k]->btarget;
					}
				}
	}


// assign the coordinates for the lines where the orders are printed on
	int j=0;
	for(int i=0;i<20;i++)
		if(bograph[i].type>0)
		{
			bograph[i].edge=Rect(getAbsoluteClientRectPosition(),Size(10000,bograph[i].lines*(FONT_SIZE+5)));
			bograph[i].edge.SetTop(getAbsoluteClientRectUpperBound()+(j+1)*(FONT_SIZE+5));
			j+=bograph[i].lines;
			if(fitItemToAbsoluteClientRect(bograph[i].edge,1))
				lastbographY=bograph[i].edge.y+bograph[i].edge.height;
		}
}

void BoGraphWindow::draw(DC* dc) const
{
	if(!shown) return;
	UI_Window::draw(dc);

	if(!orderList) return;

	// now print the lines...
	dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
	for(int i=0;i<20;i++)
		if(bograph[i].type>0)
	//		for(int j=0;j<bograph[i].lines;j++)
			{
				Rect rec=Rect(bograph[i].edge.GetPosition()/*+Point(0,j*(FONT_SIZE+5))*/, bograph[i].edge.GetSize());
				if(insideAbsoluteClientRect(rec))
				// TODO BUTTONS drausmachen...
				{
				//	if(j%2==0)
				//		dc->SetBrush(*theme.lookUpBrush(BO_DARK_BRUSH));
				//	else 
					dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
					dc->DrawRoundedRectangle(rec,2);
				}
			}

// and the time steps on the top
	dc->SetTextForeground(*theme.lookUpColor(TIMESTEPS_TEXT_COLOUR));
// dc->SetPen(*BLACK_PEN); TODO
	int timesteps=((anarace->getRealTimer())/30)/10+1; // TODO <- wird 0? bei Protoss? :-/
	dc->SetPen(*theme.lookUpPen(GREEN_TIMESTEPS_PEN));
	for(int i=0;i<(anarace->getRealTimer())/30;i++)
		if(i%timesteps==0)
		{
			if(i>0) 
				dc->DrawLine(getAbsoluteClientRectPosition()+Point(5+i*((getClientRectWidth()-20)/((anarace->getRealTimer())/30)),(FONT_SIZE+5)), getAbsoluteClientRectPosition()+Point(5+i*((getClientRectWidth()-20)/((anarace->getRealTimer())/30))+1,lastbographY-getAbsoluteClientRectPosition().y));
			ostringstream os;
			os << i/2 << ":" << 3*(i%2) << "0";
			dc->DrawText(os.str(),getAbsoluteClientRectPosition()+Point(5+i*((getClientRectWidth()-20)/((anarace->getRealTimer())/30)),0));
		}

// --------------------------------- END BUILD ORDER GRAPH ------------------------------
																				
	/*if(markedUnit==0)
		markAni=0;
	else
	{
		if(markAni>1)
			markAni-=4;
		else markAni=50;
	};*/
																			
// ... and finally the orders
//	for(map<long, Order>::const_iterator order=orderList->begin(); order!=orderList->end(); ++order)
	for(int i=orderCount;i--;)
	{
		if(anarace->getProgramFacility(sortedList[i]->getIP()))
		{
			Rect edge=Rect(getRelativeClientRectPosition()+sortedList[i]->brect.GetPosition(), sortedList[i]->brect.GetSize());
//			if(insideRelativeClientRect(edge))
			{
				dc->SetBrush(*theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[anarace->getRace()][anarace->getPhaenoCode(sortedList[i]->getIP())].unitType)));
#if 0
							Color(dc->GetSurface(),
(100*(infoWindow.isShown())*(order->first==infoWindow.getKey())+COLOR1R+BOcolor[stats[anarace->getRace()][anarace->phaenoCode[order->IP]].type].Red())*order->blend/50,
(100*(infoWindow.isShown())*(node->fist==infoWindow.getKey())/*+(optimizeMode*anarace->isConstant(order->IP))*255+(1-(optimizeMode*anarace->isConstant(order->IP)))**/(COLOR1G+BOcolor[stats[anarace->getRace()][anarace->phaenoCode[order->IP]].type].Green())*order->blend/50),
(100*(infoWindow.isShown())*(node->first==infoWindow.getKey())+COLOR1B+BOcolor[stats[anarace->getRace()][anarace->phaenoCode[order->IP]].type].Blue())*order->blend/50),SOLID_BRUSH_STYLE));
#endif
	//			int bright=0;
	///			if(( (infoWindow->isShown())&&(order->second.getIP()==infoWindow->getIP()) )||((markedUnit>0)&&(order->second.getUnit()==markedUnit)))
	//					bright=50;//+markAni; TODO
//				dc->SetBrush(*theme.lookUpBrush(BO_DARK_BRUSH));				//TODO
//				dc->SetBrush(Brush(dc->doColor(
//		((anarace->getProgramCode(order->second.IP)+1)*155/(1+anarace->getPlayer()->getGoal()->getMaxBuildTypes()/2))%156+bright,
//		((anarace->getProgramCode(order->second.IP)+1)*155/(1+anarace->getPlayer()->getGoal()->getMaxBuildTypes()/4))%156+bright,
//		((anarace->getProgramCode(order->second.IP)+1)*155/(1+anarace->getPlayer()->getGoal()->getMaxBuildTypes()/8))%156+bright),SOLID_BRUSH_STYLE));
				dc->SetPen(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN+stats[anarace->getRace()][anarace->getPhaenoCode(sortedList[i]->getIP())].unitType)));
				dc->DrawRoundedRectangle(Rect(getAbsolutePosition()+edge.GetPosition(), edge.GetSize()),2);
			}
		}
	}
	//finally print the legend
	dc->SetTextForeground(*theme.lookUpColor(BRIGHT_TEXT_COLOUR));
	for(int i=0;i<20;i++)
		if(bograph[i].type>0)
			dc->DrawText(" "+stats[anarace->getRace()][bograph[i].type].name,getAbsoluteClientRectPosition()+Point(0,1+(i+1)*(FONT_SIZE+5)));
}





