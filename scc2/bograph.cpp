#include "bograph.h"

BoGraphWindow::BoGraphWindow(UI_Object* parent, ANARACE* anarace, InfoWindow* infoWindow, map<long, Order>* orderList, const int windowNumber):UI_Window(parent, BOGRAPH_WINDOW_TITLE_STRING, BO_GRAPH_WINDOW, windowNumber, SCROLLED)
{
	this->orderList=orderList;
	this->infoWindow=infoWindow;
	this->anarace=anarace;
	resetData();
};

BoGraphWindow::~BoGraphWindow()
{
};


void BoGraphWindow::setMarkedUnit(int unit)
{
	markedUnit=unit;
};

void BoGraphWindow::checkForInfoWindow()
{
 //pair<string, int> Enumerator(string("down"), down); //create a pair
// mi.insert(Enumerator); //insert the pair
	for(map<long, Order>::const_iterator order=orderList->begin(); order!=orderList->end(); ++order)
	{
//      int row=((boInsertPoint>-1)&&(order->row>=boInsertPoint))*(boEndPoint-boInsertPoint);
//      mouse on order in player reinschieben, da is ja auch orderList zuhause
		Rect edge(getRelativeClientRectPosition()+order->second.brect.GetPosition()/*-Point(0,getScrollY() TODO)*/,order->second.brect.GetSize());
        if((fitItemToRelativeClientRect(edge)&&edge.Inside(controls.getCurrentPosition()-getAbsolutePosition())))
        {
//window[INFO_WINDOW]->adjustWindow(Rect(Point(window[INFO_WINDOW]->getPosition().x,500)/*order->rect.GetY()+getClientRectUpperBound()-getScrollY()+((boInsertPoint>-1)&&(order->row>=boInsertPoint))*(boEndPoint-boInsertPoint)*(FONT_SIZE+5)-1)*/,window[INFO_WINDOW]->getSize()));
            infoWindow->setUnit(order->second.unit);
            infoWindow->setIP(order->second.IP);
            infoWindow->setBx(order->second.brect.x);
            infoWindow->setBWidth(order->second.brect.width);
            infoWindow->Show(1);
            return;
        }
    }
};
	
#if 0
//TODO evtl irgendwie doch mit bowindow zusammenfassen
void BoGraphWindow::CheckOrders()
{
	for(int s=MAX_LENGTH;s--;)
// /home/clawg/work/sc1041/sc/exe/../scc2/scc2.cpp:843: undefined reference to `ANARACE::getProgramIsBuilt(int)' <- WTF? nur bei exe...
		if(anarace->getProgramIsBuilt2(s)/*&&(anarace->getProgramTime(s)<=ga->maxTime-anarace->getTimer())*/)
		{
			if(NODE* node=orderList->Find(anarace->getMarker(s)))
			{
				ORDER* order=node->GetData();
				order->blendTarget=50;
				order->unit=anarace->phaenoCode[s];
				order->IP=s;
				order->checked=1;
			} // => aktualisieren
			else // => neues erstellen
			{
				//TODO: testen ob anderes item da ist, das aber die gleiche Unit besitzt + an die gleiche Position kommt
				int found=0;
/*			      NODE *node=orderList->GetFirst();
				while(node&&(!found))
				{
					if((node->GetData()->unit!=anarace->phaenoCode[s])||(node->GetData()->row!=k+1)||(node->GetData()->target.x>0))
//TODO... damits net rein und raushaut...
						node=node->GetNext();
					else //=> ueberschreiben
					{
						ORDER* order=node->GetData();
						order->blendTarget=50;
						order->bonew=1;
						order->IP=s;
						order->unit=anarace->phaenoCode[s];
						order->checked=1;
						found=1;
					}
				}*/
				if(!found)
				{
					ORDER* order=new ORDER;
					order->blend=1;order->blendTarget=50;
					order->bonew=1;
					order->unit=anarace->phaenoCode[s];
					order->IP=s;
					order->checked=1;
					orderList->Append((long)anarace->getMarker(s),order);
				}
			}
		}
																			    
		NODE* node=orderList->GetFirst();
		while(node)
		{
			if(!node->GetData()->checked)
				node=orderList->DeleteNode(node);
			else
			{
				node->GetData()->checked=0;
				node=node->GetNext();
			}
		}
};
#endif
       
void BoGraphWindow::resetData()
{
	markedUnit=0;
};
/*
void BoGraphWindow::MoveOrders()
{
	NODE *node=orderList->GetFirst();
	while(node)
	{
		ORDER* order=node->GetData();
		move(order->rect.x,order->target.x);
		move(order->rect.y,order->target.y);
		move(order->rect.width,order->target.width);
		move(order->rect.height,order->target.height);
		move(order->blend,order->blendTarget);
		node=node->GetNext();
	}
};*/

void BoGraphWindow::process()
{
	if(!shown) return;
	checkForInfoWindow();
	UI_Window::process();
};

void BoGraphWindow::draw(DC* dc)
{
	if(!shown) return;
	UI_Window::draw(dc);
	
	if(!orderList) return;
	struct BOGRAPH
	{
		int type;
		int position;
		int height;
		int lines;
	} bograph[20];
	int fac[20];
	int unitCounter[UNIT_TYPE_COUNT][MAX_LENGTH];
	int height[UNIT_TYPE_COUNT];
	int lines[UNIT_TYPE_COUNT];
	int faccount=0;
																			    
																			    
	for(int i=0;i<UNIT_TYPE_COUNT;i++)
	{
		for(int j=0;j<MAX_LENGTH;j++)
			unitCounter[i][j]=0;
		height[i]=0;
		lines[i]=0;
	}
	for(int i=0;i<20;i++)
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
		int IP=order->second.IP;
// falls facility benoetigt wird und Zahl der zu dem Zeitpunkt vorhandenen Einheiten minus der verfuegbaren Einheiten > hoehe => setze hoehe auf diesen Wert
		if(anarace->getProgramFacility(IP)&&(anarace->getProgramTotalCount(IP,anarace->getProgramFacility(IP))-anarace->getProgramAvailibleCount(IP,anarace->getProgramFacility(IP))>height[anarace->getProgramFacility(IP)]))
			 height[anarace->getProgramFacility(IP)]=anarace->getProgramTotalCount(IP,anarace->getProgramFacility(IP))-anarace->getProgramAvailibleCount(IP,anarace->getProgramFacility(IP));
	}
																			    
//calculate number of lines per facility and adjust the height
	for(int i=UNIT_TYPE_COUNT;i--;)
	{
		while(height[i]>MIN_HEIGHT)
		{
			height[i]-=MIN_HEIGHT;
			lines[i]++;
		}
		if(height[i]>0) lines[i]++;
		if(lines[i]>1)
			height[i]=MIN_HEIGHT;
       }
																			    
//make a list of facilities that are needed...
	faccount=1;
    for(map<long, Order>::const_iterator order=orderList->begin(); order!=orderList->end(); ++order)
	{
		if(anarace->getProgramFacility(order->second.IP))
		{
			int i=-1;
			for(i=1;i<faccount;i++)
				if(fac[i]==anarace->getProgramFacility(order->second.IP)) break;
			if(i==faccount)
			{
				fac[i]=anarace->getProgramFacility(order->second.IP);
				faccount++;
			}
		}
	}
																			    
// ...and sort them
	for(int i=0;i<20;i++)
		if(fac[i])
			for(int j=0;j<i;j++)
// scvs last
				if(fac[i]>fac[j])
				{
					int temp=fac[i];
					fac[i]=fac[j];
					fac[j]=temp;
//					xchg(fac[i],fac[j]);
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
//sort the order after IPs
// now calculate the rectangles :)
	//orderList->Sort();
	int hoehe=0;
    for(map<long, Order>::iterator order=orderList->begin(); order!=orderList->end(); ++order)
	{
		if(anarace->getProgramFacility(order->second.IP))
			for(int i=0;i<20;i++)
				if(bograph[i].type==anarace->getProgramFacility(order->second.IP))
				{
//order->time muesste vorsortiert sein
					for(int j=0;j<MAX_LENGTH;j++)
						if(unitCounter[bograph[i].type][j]<=anarace->getProgramTime(order->second.IP))
						{
							unitCounter[bograph[i].type][j]=anarace->getProgramTime(order->second.IP)+stats[anarace->getPlayer()->getRace()][anarace->getPhaenoCode(order->second.IP)].BT;
							hoehe=j;
							j=MAX_LENGTH;
					       }
					Rect t=Rect(
Point( (anarace->getProgramTime(order->second.IP)*getClientRectWidth())/(anarace->ga->maxTime-anarace->getTimer()),
	 (1+i+hoehe/MIN_HEIGHT)*(FONT_SIZE+5)+(hoehe%MIN_HEIGHT)*(FONT_SIZE+4)/bograph[i].height),
Size(  (stats[anarace->getPlayer()->getRace()][anarace->getPhaenoCode(order->second.IP)].BT/*anarace->getProgramBT(s)*/*getClientRectWidth())/(anarace->ga->maxTime-anarace->getTimer()),
	 (FONT_SIZE+4)/(bograph[i].height)));
					if(t!=order->second.btarget)
						order->second.bstart=order->second.brect;
					order->second.btarget=t;
					if(order->second.bonew)
					{
						order->second.bonew=0;
						order->second.brect=order->second.btarget;
					}
				}
	}
	// now print the lines...
	int lastbograph=-1;
	dc->SetPen(*theme.lookUpPen(RECTANGLE_PEN));
	{
		Rect edge=Rect(getRelativeClientRectPosition(),Size(10000,FONT_SIZE+4));
		for(int i=0;i<20;i++)
			if(bograph[i].type>0)
				for(int j=i;j<i+bograph[i].lines;j++)
				{
					edge.SetTop(getRelativeClientRectUpperBound()+(j+1)*(FONT_SIZE+5));
					if(fitItemToRelativeClientRect(edge,1))
					{
						if(j%2==0)
							dc->SetBrush(*theme.lookUpBrush(BO_DARK_BRUSH));
						else dc->SetBrush(*theme.lookUpBrush(BO_BRIGHT_BRUSH));
						dc->DrawRoundedRectangle(Rect(getAbsolutePosition()+edge.GetPosition(), edge.GetSize()),4);
						lastbograph=edge.y+edge.height;
					}
				}
	}
																		    

// and the time steps on the top
	dc->SetTextForeground(*theme.lookUpColor(TIMESTEPS_TEXT_COLOUR));
//	dc->SetPen(*BLACK_PEN); TODO
	int timesteps=((anarace->ga->maxTime-anarace->getTimer())/30)/10+1; // TODO <- wird 0? bei Protoss? :-/
	for(int i=0;i<(anarace->ga->maxTime-anarace->getTimer())/30;i++)
		if(i%timesteps==0)
		{
		      dc->DrawLine(getAbsoluteClientRectPosition()+Point(30*(i+timesteps)*getClientRectWidth()/(anarace->ga->maxTime-anarace->getTimer()),0),
			      getAbsoluteClientRectPosition()+Point(30*(i+timesteps)*getClientRectWidth()/(anarace->ga->maxTime-anarace->getTimer()),lastbograph));
			ostringstream os;
			os << i/2 << ":" << 3*(i%2) << "0";
			dc->DrawText(os.str(),getAbsoluteClientRectPosition()+Point(5+i*((getClientRectWidth()-20)/((anarace->ga->maxTime-anarace->getTimer())/30)),0));
		}
#if 0
		for(int i=0;i<(anarace->ga->maxTime-anarace->getTimer())/30;i++)
		if(i%timesteps==0)
		{
  //		  dc->DrawLine(getAbsoluteClientRectLeftBound()+(i+timesteps)*(getClientRectWidth()/((anarace->ga->maxTime-anarace->getTimer())/30)),getClientRectUpperBound(),    //		      getAbsoluteClientRectLeftBound()+(i+timesteps)*(getClientRectWidth()/((anarace->ga->maxTime-anarace->getTimer())/30)),getClientRectUpperBound()+FONT_SIZE+4);
  			ostringstream os;
			os << i/2 << ":" << 3*(i%2) << "0";
  			dc->DrawText(_T(String::Format(T("%i:%i0"),i/2,3*(i%2))),getAbsoluteClientRectLeftBound()+5+i*((getClientRectWidth()-20)/((anarace->ga->maxTime-anarace->getTimer())/30)),getClientRectUpperBound());
		}
//TODO nochmal timesteps checken
#endif																		    
// --------------------------------- END BUILD ORDER GRAPH ------------------------------
																			    
																			    


	if(markedUnit==0)
		markAni=0;
	else
	{
		if(markAni>1)
			markAni-=4;
		else markAni=50;
	};
																		    
// ... and finally the orders
    for(map<long, Order>::const_iterator order=orderList->begin(); order!=orderList->end(); ++order)
	{
		if(anarace->getProgramFacility(order->second.IP))
		{
			Rect edge=Rect(getRelativeClientRectPosition()+order->second.brect.GetPosition(),order->second.brect.GetSize());
			if(fitItemToRelativeClientRect(edge))
			{
				dc->SetBrush(*theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[anarace->getPlayer()->getRace()][anarace->getPhaenoCode(order->second.IP)].unitType)));
#if 0
							Color(dc->GetSurface(),
(100*(infoWindow.isShown())*(order->first==infoWindow.getKey())+COLOR1R+BOcolor[stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[order->IP]].type].Red())*order->blend/50,
(100*(infoWindow.isShown())*(node->fist==infoWindow.getKey())/*+(optimizeMode*anarace->isConstant(order->IP))*255+(1-(optimizeMode*anarace->isConstant(order->IP)))**/(COLOR1G+BOcolor[stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[order->IP]].type].Green())*order->blend/50),
(100*(infoWindow.isShown())*(node->first==infoWindow.getKey())+COLOR1B+BOcolor[stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[order->IP]].type].Blue())*order->blend/50),SOLID_BRUSH_STYLE));
#endif
				int bright=0;
				if(( (infoWindow->isShown())&&(order->second.IP==infoWindow->getIP()) )||((markedUnit>0)&&(order->second.unit==markedUnit)))
						bright=50+markAni;
//				dc->SetBrush(*theme.lookUpBrush(BO_DARK_BRUSH));				//TODO
//				dc->SetBrush(Brush(dc->doColor(
//		((anarace->getProgramCode(order->second.IP)+1)*155/(1+anarace->getPlayer()->getGoal()->getMaxBuildTypes()/2))%156+bright,
//		((anarace->getProgramCode(order->second.IP)+1)*155/(1+anarace->getPlayer()->getGoal()->getMaxBuildTypes()/4))%156+bright,
//		((anarace->getProgramCode(order->second.IP)+1)*155/(1+anarace->getPlayer()->getGoal()->getMaxBuildTypes()/8))%156+bright),SOLID_BRUSH_STYLE));
				dc->DrawRoundedRectangle(Rect(getAbsolutePosition()+edge.GetPosition(), edge.GetSize()),4);
			}
		}
	}
	//finally print the legend
	dc->SetTextForeground(*theme.lookUpColor(TEXT_COLOUR));
	for(int i=0;i<20;i++)
		if(bograph[i].type>0)
			dc->DrawText(stats[anarace->getPlayer()->getRace()][bograph[i].type].name,getAbsoluteClientRectPosition()+Point(0,(i+1)*(FONT_SIZE+5)));
	markedUnit=0;
};


