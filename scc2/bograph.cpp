#include "bograph.h"

void BoGraphWindow::setMarkedUnit(int unit)
{
	markedUnit=unit;
};

void BoGraphWindow::assignInfoWindow(InfoWindow* infoWindow)
{
	this->infoWindow=infoWindow;
};

bool BoGraphWindow::mouseOnOrder(ORDER* order)
{
	wxRect edge(getInnerPosition()+order->brect.GetPosition()-wxPoint(0,getScrollY()),order->brect.GetSize());
        if((fitToClientArea(edge)&&edge.Inside(controls.getCurrentPosition())))
		return true;
	else return(false);	
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
       
void BoGraphWindow::setAnarace(ANARACE* anarace)
{
	this->anarace=anarace;
}																		    
																			   
BoGraphWindow::BoGraphWindow(wxRect rahmen, wxRect maxSize):GraphixScrollWindow(0,rahmen,maxSize,SCROLLED)
{
	resetData();
};
																			    
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
																			    
void BoGraphWindow::showBoGraph(wxDC* dc, OrderList* orderList)
{
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
	NODE* node=orderList->GetFirst();
	while(node)
	{
		int IP=node->GetData()->IP;
// falls facility benoetigt wird und Zahl der zu dem Zeitpunkt vorhandenen Einheiten minus der verfuegbaren Einheiten > hoehe => setze hoehe auf diesen Wert
		if(anarace->getProgramFacility(IP)&&(anarace->getProgramForceCount(IP,anarace->getProgramFacility(IP))-anarace->getProgramAvailibleCount(IP,anarace->getProgramFacility(IP))>height[anarace->getProgramFacility(IP)]))
			 height[anarace->getProgramFacility(IP)]=anarace->getProgramForceCount(IP,anarace->getProgramFacility(IP))-anarace->getProgramAvailibleCount(IP,anarace->getProgramFacility(IP));
		node=node->GetNext();
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
	node=orderList->GetFirst();
	faccount=1;
	while(node)
	{
		if(anarace->getProgramFacility(node->GetData()->IP))
		{
			int i=-1;
			for(i=1;i<faccount;i++)
				if(fac[i]==anarace->getProgramFacility(node->GetData()->IP)) break;
			if(i==faccount)
			{
				fac[i]=anarace->getProgramFacility(node->GetData()->IP);
				faccount++;
			}
		}
		node=node->GetNext();
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
	node=orderList->GetFirst();
	int hoehe=0;
	while(node)
	{
		ORDER* order=node->GetData();
		if(anarace->getProgramFacility(order->IP))
			for(int i=0;i<20;i++)
				if(bograph[i].type==anarace->getProgramFacility(order->IP))
				{
//order->time muesste vorsortiert sein
					for(int j=0;j<MAX_LENGTH;j++)
						if(unitCounter[bograph[i].type][j]<=anarace->getProgramTime(order->IP))
						{
							unitCounter[bograph[i].type][j]=anarace->getProgramTime(order->IP)+stats[anarace->getPlayer()->getRace()][anarace->getPhaenoCode(order->IP)].BT;
							hoehe=j;
							j=MAX_LENGTH;
					       }
					wxRect t=wxRect(
wxPoint( (anarace->getProgramTime(order->IP)*getInnerWidth())/(anarace->ga->maxTime-anarace->getTimer()),
	 (1+i+hoehe/MIN_HEIGHT)*(FONT_SIZE+5)+(hoehe%MIN_HEIGHT)*(FONT_SIZE+4)/bograph[i].height),
wxSize(  (stats[anarace->getPlayer()->getRace()][anarace->getPhaenoCode(order->IP)].BT/*anarace->getProgramBT(s)*/*getInnerWidth())/(anarace->ga->maxTime-anarace->getTimer()),
	 (FONT_SIZE+4)/(bograph[i].height)));
					if(t!=order->btarget)
						order->bstart=order->brect;
					order->btarget=t;
					if(order->bonew)
					{
						order->bonew=0;
						order->brect=order->btarget;
					}
				}
		node=node->GetNext();
	}
	// now print the lines...
	int lastbograph=-1;
	dc->SetPen(wxPen(wxColour(PEN1R,PEN1G,PEN1B),1,wxSOLID));
	{
		wxRect edge=wxRect(getInnerPosition(),wxSize(10000,FONT_SIZE+4));
		for(int i=0;i<20;i++)
			if(bograph[i].type>0)
				for(int j=i;j<i+bograph[i].lines;j++)
				{
					edge.SetY(getInnerUpperBound()+(j+1)*(FONT_SIZE+5));
					if(fitToClientArea(edge,1))
					{
						if(j%2==0)
							dc->SetBrush(wxBrush(wxColour(COLOR1G,COLOR1G,COLOR1B),wxSOLID));
						else dc->SetBrush(wxBrush(wxColour(COLOR2R,COLOR2G,COLOR2B),wxSOLID));
						dc->DrawRoundedRectangle(edge,4);
						lastbograph=edge.y+edge.height;
					}
				}
	}
																		    

// and the time steps on the top
	dc->SetTextForeground(wxColour(50,200,50));
	dc->SetPen(*wxBLACK_PEN);
	int timesteps=((anarace->ga->maxTime-anarace->getTimer())/30)/10+1; // TODO <- wird 0? bei Protoss? :-/
	for(int i=0;i<(anarace->ga->maxTime-anarace->getTimer())/30;i++)
		if(i%timesteps==0)
		{
		      dc->DrawLine(getInnerLeftBound()+(i+timesteps)*(getInnerWidth()/((anarace->ga->maxTime-anarace->getTimer())/30)),getInnerUpperBound(),
			      getInnerLeftBound()+(i+timesteps)*(getInnerWidth()/((anarace->ga->maxTime-anarace->getTimer())/30)),lastbograph);
			dc->DrawText(_T(wxString::Format(wxT("%i:%i0"),i/2,3*(i%2))),getInnerLeftBound()+5+i*((getInnerWidth()-20)/((anarace->ga->maxTime-anarace->getTimer())/30)),getInnerUpperBound());
			
		}
	for(int i=0;i<(anarace->ga->maxTime-anarace->getTimer())/30;i++)
		if(i%timesteps==0)
		{
  //		  dc->DrawLine(getInnerLeftBound()+(i+timesteps)*(getInnerWidth()/((anarace->ga->maxTime-anarace->getTimer())/30)),getInnerUpperBound(),    //		      getInnerLeftBound()+(i+timesteps)*(getInnerWidth()/((anarace->ga->maxTime-anarace->getTimer())/30)),getInnerUpperBound()+FONT_SIZE+4);
			dc->DrawText(_T(wxString::Format(wxT("%i:%i0"),i/2,3*(i%2))),getInnerLeftBound()+5+i*((getInnerWidth()-20)/((anarace->ga->maxTime-anarace->getTimer())/30)),getInnerUpperBound());
		}
//TODO nochmal timesteps checken
																			    
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
	node=orderList->GetFirst();
	while(node)
	{
		ORDER* order=node->GetData();
		if(anarace->getProgramFacility(order->IP))
		{
			wxRect edge=wxRect(getInnerPosition()+order->brect.GetPosition(),order->brect.GetSize());
			if(fitToClientArea(edge))
			{
#if 0
				dc->SetBrush(wxBrush(wxColour(
(100*(infoWindow.isShown())*(node->GetKey()==infoWindow.getKey())+COLOR1R+BOcolor[stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[order->IP]].type].Red())*order->blend/50,
(100*(infoWindow.isShown())*(node->GetKey()==infoWindow.getKey())/*+(optimizeMode*anarace->isConstant(order->IP))*255+(1-(optimizeMode*anarace->isConstant(order->IP)))**/(COLOR1G+BOcolor[stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[order->IP]].type].Green())*order->blend/50),
(100*(infoWindow.isShown())*(node->GetKey()==infoWindow.getKey())+COLOR1B+BOcolor[stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[order->IP]].type].Blue())*order->blend/50),wxSOLID));
#endif
				int bright=0;
				if(( (infoWindow->isShown())&&(node->GetKey()==infoWindow->getKey()) )||((markedUnit>0)&&(order->unit==markedUnit)))
						bright=50+markAni;
				dc->SetBrush(wxBrush(wxColour(
		((anarace->getProgramCode(order->IP)+1)*155/(1+anarace->getPlayer()->getGoal()->getMaxBuildTypes()/2))%156+bright,
		((anarace->getProgramCode(order->IP)+1)*155/(1+anarace->getPlayer()->getGoal()->getMaxBuildTypes()/4))%156+bright,
		((anarace->getProgramCode(order->IP)+1)*155/(1+anarace->getPlayer()->getGoal()->getMaxBuildTypes()/8))%156+bright),wxSOLID));
				dc->DrawRoundedRectangle(edge,4);
			}
		}
		node=node->GetNext();
	}
	//finally print the legend
	dc->SetTextForeground(wxColour(0,0,0));
	for(int i=0;i<20;i++)
		if(bograph[i].type>0)
			dc->DrawText(_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][bograph[i].type].name)),getInnerPosition()+wxPoint(0,(i+1)*(FONT_SIZE+5)));
	markedUnit=0;
};


