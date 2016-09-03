#include "bowindow.h"
#include "util.h"

void BoWindow::setAnarace(ANARACE* anarace)
{
	this->anarace=anarace;
}

BoWindow::BoWindow(wxRect rahmen, wxRect maxSize):GraphixScrollWindow(0,rahmen,maxSize,SCROLLED,NOT_TABBED,wxRect(0,50,1000,1000))
{
	addButton(wxRect(getInnerLeftBound()+getWidth()-48,getInnerUpperBound()-30,12,12),PERM_BUTTON);
	addButton(wxRect(getInnerLeftBound()+getWidth()-48,getInnerUpperBound()-15,12,12),PERM_BUTTON);
	resetData();
};

void BoWindow::resetData()
{
//	int i;
	boInsertPoint=-1;
	boEndPoint=-1;
	boGoalListOpened=0;
	optimizeMode=0;
//	for(i=0;i<MAX_LENGTH;i++)
//		selection[i]=1;
	orderList.Clear();
};

void BoWindow::drawBuildOrder(wxDC* dc)
{
	boEndPoint=0;
	dc->SetPen(wxPen(wxColour(0,0,0),1,wxSOLID));
	if(boInsertPoint>-1)
	{
		int line=boInsertPoint;
		wxRect edge=wxRect(getInnerPosition()+wxPoint(0,line*(FONT_SIZE+5)-getScrollY()),wxSize(270,FONT_SIZE+4));
		if(fitToClientArea(edge,1))
		{
			//if(t%2==0)
				dc->SetBrush(wxBrush(wxColour(50,50,50),wxSOLID));
			//else
				//dc->SetBrush(wxBrush(wxColour(COLOR2R,COLOR2G,COLOR2B),wxSOLID));
																			    
			dc->DrawRoundedRectangle(edge,4);
			dc->SetTextForeground(wxColour(0,200,0));
			if(boGoalListOpened==1)
			{
//				dc->DrawBitmap(bmpArrowUp,edge.x+4,edge.y+2);
				dc->DrawText(_T("(Click here to close)"),edge.x+14,edge.y);
			}
			else
			{
//				dc->DrawBitmap(bmpArrowUp,edge.x+4,edge.y+2);
				dc->DrawText(_T("(Click here to go back)"),edge.x+14,edge.y);
			}
			unitButton[line]=addButton(edge);
		}
		line++;
		if(boGoalListOpened==1)
		{
			for(int i=1;i<11;i++)
			{
				wxRect edge=wxRect(getInnerPosition()+wxPoint(10,line*(FONT_SIZE+5)-getScrollY()),wxSize(270,FONT_SIZE+4));
				lastBogoal=edge.GetY();
				if(fitToClientArea(edge,1))
				{
					dc->SetBrush(wxBrush(wxColour((COLOR1R+2*BOcolor[i].Red()),(COLOR1G+2*BOcolor[i].Green()),(COLOR1B+2*BOcolor[i].Blue())),wxSOLID));
					dc->DrawRoundedRectangle(edge,4);
					dc->SetTextForeground(wxColour(TEXT1R,TEXT1G,TEXT1B));
					dc->DrawText(BOnames[i],edge.x+10,edge.y);
					unitButton[line]=addButton(edge);
				}
				line++;
			}
		}
		else if(boGoalListOpened>1)
		{
			int type=boGoalListOpened+1;
			for(int i=0;i<UNIT_TYPE_COUNT;i++)
			{
				if(stats[anarace->getPlayer()->getRace()][i].type==type)
				{
					wxRect edge=wxRect(getInnerPosition()+wxPoint(20,line*(FONT_SIZE+5)-getScrollY()),wxSize(270,FONT_SIZE+4));
					lastBogoal=edge.y;
					if(fitToClientArea(edge,1))
					{
						dc->SetBrush(wxBrush(wxColour((COLOR1R+2*BOcolor[type].Red()),(COLOR1G+2*BOcolor[type].Green()),(COLOR1B+2*BOcolor[type].Blue())),wxSOLID));
		 				dc->DrawRoundedRectangle(edge,4);
						dc->SetTextForeground(wxColour(TEXT1R,TEXT1G,TEXT1B));
						dc->DrawText(_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][i].name)),edge.x+10,edge.y);
						unitButton[line]=addButton(edge);
					}
					line++;
				}
			}
		}
/*		edge.x=20;
		edge.y=line*(FONT_SIZE+5)+9-getScrollY();
		edge.width=getInnerWidth()-40;
		edge.height=2;
		if(fitToClientArea(edge))
		{
			dc->SetPen(wxPen(wxColour(200,0,0),2,wxSOLID));
			dc->DrawLine(edge.x+getInnerLeftBound(),edge.y+getInnerUpperBound(),edge.x+edge.width+getInnerLeftBound(),edge.y+getInnerUpperBound());
			dc->SetPen(wxPen(wxColour(0,0,0),1,wxSOLID));
		}*/
		boEndPoint=line+1;
		//end
	}
	
	int line=0;
	NODE* node=orderList.GetFirst();
	while(node)
	{
		ORDER* order=node->GetData();
		wxRect edge=wxRect(getInnerPosition()+order->rect.GetPosition()+wxPoint(0,((boInsertPoint>-1)&&(order->row>=boInsertPoint))*(boEndPoint-boInsertPoint)*(FONT_SIZE+5)-getScrollY()),order->rect.GetSize());
		if(fitToClientArea(edge,1))
		{
			{
//					if(((order->targety-3)/(FONT_SIZE+5))%2)
				dc->SetBrush(wxBrush(wxColour(
(/*100*(infoWindowNumber>0)*(node->GetKey()==infoWindowNumber-1)+*/COLOR1R+BOcolor[stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[order->IP]].type].Red())*order->blend/50,
(/*100*(infoWindowNumber>0)*(node->GetKey()==infoWindowNumber-1)*//*+(optimizeMode*anarace->isConstant(order->IP))*255+(1-(optimizeMode*anarace->isConstant(order->IP)))**/(COLOR1G+BOcolor[stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[order->IP]].type].Green())*order->blend/50),
(/*100*(infoWindowNumber>0)*(node->GetKey()==infoWindowNumber-1)+*/COLOR1B+BOcolor[stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[order->IP]].type].Blue())*order->blend/50),wxSOLID));

//else dc->SetBrush(wxBrush(wxColour(
//(COLOR2R+BOcolor[stats[anarace->getPlayer()->getRace()][order->unit].type].Red())*order->blend/50,
//(COLOR2G+BOcolor[stats[anarace->getPlayer()->getRace()][order->unit].type].Green())*order->blend/50,
//(COLOR2B+BOcolor[stats[anarace->getPlayer()->getRace()][order->unit].type].Blue())*order->blend/50),wxSOLID));

					dc->DrawRoundedRectangle(edge,4);
					dc->SetTextForeground(BOcolor[stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[order->IP]].type]);

					if(edge.width>=110)
					{
//						unitButton[row+t]=addButton(wxRect(edge.x+edge.width-12,edge.y+1,8,8));
						dc->DrawText(_T(wxString::Format(wxT("%i."),anarace->getProgramForceCount(order->IP,anarace->phaenoCode[order->IP])+1)),edge.GetPosition()+wxPoint(5,0));
						dc->DrawText(_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][anarace->phaenoCode[order->IP]].name)),edge.GetPosition()+wxPoint(20,0));
//						if(row+t+1==boEndPoint)
//							dc->DrawBitmap(bmpArrowDown,edge.x+edge.width-12,edge.y+1);
//						else
//							dc->DrawBitmap(bmpArrowUp,edge.x+edge.width-12,edge.y+1);
//						if(optimizeMode)
//							optButton[row+t]=addButton(wxRect(getInnerLeftBound()+edge.width,edge.y,getWidth()-edge.width,FONT_SIZE+5));
					}
/*					if(edge.width>=140)
						dc->DrawText(_T(wxString::Format(wxT("%i"),order->mins)),110+edge.x,edge.y);
					if(edge.width>=180)
						dc->DrawText(_T(wxString::Format(wxT("%i"),order->gas)),150+edge.x,edge.y);
					if(edge.width>=230)
						dc->DrawText(_T(wxString::Format(wxT("%i/%i"),order->needSupply,order->haveSupply)),190+edge.x,edge.y);
					if(edge.width>=260)
						dc->DrawText(_T(wxString::Format(wxT("%.2i:%.2i"),order->time/60,order->time%60)),240+edge.x,edge.y);
	//      			dc->DrawText(_T(wxString::Format(wxT("%i"),order->successType)),190+order->x,order->y);*/
//					}
			}
		}
		line++;
		node=node->GetNext();
	} //order blend>0
	
};

void BoWindow::drawSelectionStuff(wxDC* dc)
{
		dc->SetPen(wxPen(wxColour(INFOWINDOWPENR,INFOWINDOWPENG,INFOWINDOWPENB),1,wxSOLID));
		dc->SetTextForeground(wxColour(100,100,255));
		dc->SetFont(GraphixScrollWindow::font2);
		dc->DrawText(_T("Optimize everything"),getInnerLeftBound(),getInnerUpperBound()-32);
		dc->DrawText(_T("Optimize selected"),getInnerLeftBound(),getInnerUpperBound()-18);
		dc->SetBrush(getBackground());
		wxRect edge=wxRect(getInnerLeftBound()+getWidth()-48,getInnerUpperBound()-32,12,12);
		wxRect edge2=wxRect(getInnerLeftBound()+getWidth()-48,getInnerUpperBound()-18,12,12);
																			    
		dc->DrawRoundedRectangle(edge,3);
		dc->DrawRoundedRectangle(edge2,3);
																			    
		dc->SetTextForeground(wxColour(100,200,100));
		if(!optimizeMode) dc->DrawCheckMark(edge);
		else
		if(optimizeMode)
		{
//			dc->DrawBitmap(bmpArrowDown,getInnerLeftBound()+getWidth()-46,getInnerUpperBound()-4);
			dc->DrawCheckMark(edge2);
		}
																			    
/*      int i;
//      boanzahl=orderList.GetCount();
																			    
	if(optimizeMode)
	{
	int start=0;
	int inrow=0;
	int l=0;
	for(i=0;i<orderList.getCount();i++)
	{
		if((!inrow)&&(selection[i])&&(i!=boInsertPoint))
		{
			inrow=1;
			start=l;
		} else if((inrow)&&((!selection[i])||(i==boInsertPoint)))
		{
			inrow=0;
			wxPoint pointsKlammer[9];
			int h1=start*(FONT_SIZE+5)-getScrollY()+11+getInnerUpperBound();
			int h2=(l+1)*(FONT_SIZE+5)-getScrollY()-2+getInnerUpperBound();
																			    
			if((h1<getInnerUpperBound())&&(h2>getInnerUpperBound()))
				h1=getInnerUpperBound(); //~~
			if((h1<getInnerUpperBound()+getInnerHeight())&&(h2>getInnerUpperBound()+getInnerHeight()))
				h2=getInnerUpperBound()+getInnerHeight();
																			    
			if((h2<getInnerUpperBound())||(h1>getInnerUpperBound()+getInnerHeight()))
				i=orderLength;
			else
			{
      int x1=getInnerLeftBound()+getInnerWidth();
																			    
				pointsKlammer[0].x=x1+2;
				pointsKlammer[8].x=x1+2;
																			    
				pointsKlammer[1].x=x1+4;
				pointsKlammer[7].x=x1+4;
																			    
				pointsKlammer[2].x=x1+5;
				pointsKlammer[6].x=x1+5;
																			    
				pointsKlammer[3].x=x1+6;
				pointsKlammer[5].x=x1+6;
																			    
				pointsKlammer[4].x=x1+8;
																			    
																			    
				pointsKlammer[0].y=h1;
				pointsKlammer[8].y=h2;
																			    
				pointsKlammer[1].y=h1+2;
				pointsKlammer[7].y=h2-2;
																			    
				pointsKlammer[2].y=h1+6;
				pointsKlammer[6].y=h2-6;
																			    
				pointsKlammer[3].y=(h1+h2)/2-3;
				pointsKlammer[5].y=(h1+h2)/2+3;
																			    
				pointsKlammer[4].y=(h1+h2)/2;
//TODO gruen/rot
				dc->SetPen(wxPen(wxColour(50,200,50),1,wxSOLID));
				dc->DrawSpline(9,pointsKlammer);
			}
			}
			l++;
			if(i==boInsertPoint)
				l=boEndPoint;
		}
}
*/
};



void BoWindow::processButtons()
{
	checkButtons();
	for(int i=0;i<MAX_LENGTH;i++)
	{
		if(isActivated(optButton[i]))
		{
//			if((boInsertPoint==-1)||(i<boInsertPoint))
//				selection[i]=!selection[i];
//			else if(i>boEndPoint)
//			       selection[boInsertPoint-boEndPoint+i]=!selection[boInsertPoint-boEndPoint+i]; TODO
		}
		if(isActivated(unitButton[i]))
		{
//			msgWindow->addMessage(_T(wxString::Format(wxT("Button %i pressed"),i)));
			if(boInsertPoint==-1) //all closed
			{
//				msgWindow->addMessage(_T(wxString::Format(wxT("Opening menu at %i"),i)));
				boInsertPoint=i+1;
				anarace->setOptimizing(0);
				boGoalListOpened=1; //scheint zu funzen
			}
			else if(i<boInsertPoint)
			{
//				msgWindow->addMessage(_T(wxString::Format(wxT("Closing menu at %i and opening menu at %i"),boInsertPoint,i)));
				boInsertPoint=i+1;
				anarace->setOptimizing(0);
				boGoalListOpened=1;
			} 
			else if(i>=boEndPoint)
			{
//				msgWindow->addMessage(_T(wxString::Format(wxT("Closing menu at %i and opening menu at %i"),boInsertPoint,boInsertPoint-boEndPoint+i)));
				boInsertPoint=boInsertPoint-boEndPoint+i+1;
				anarace->setOptimizing(0);
				boGoalListOpened=1;
			} else
			{
				if(/*(i+1==boEndPoint)||*/(i==boInsertPoint)) //oeffne/schliesse Knopf <- aha...
				{
					if(boGoalListOpened==1) //=> schliesse
					{
//						msgWindow->addMessage(_T(wxString::Format(wxT("Closing menu at %i"),boInsertPoint)));
						boInsertPoint=-1;
						boEndPoint=0;
						boGoalListOpened=0;
					} // => schliessen
					else if(boGoalListOpened>1)
					{
//						msgWindow->addMessage(_T(wxString::Format(wxT("Returning to 1. menu at %i"),boInsertPoint)));
						boGoalListOpened=1;
					} // => schliessen
				} else // aha ein menueeintrag
				{
//						msgWindow->addMessage(_T(wxString::Format(wxT("%i %i %i"),boInsertPoint,boEndPoint,i)));
					if(boGoalListOpened==1) // => 1. Untermenue
					{
						if(i==boInsertPoint+1)
						{
							anarace->insertOrder(SCV,boInsertPoint);
//							msgWindow->addMessage(_T(wxString::Format(wxT("Item 1 was chosen")))); // => scv
//							update=2; TODO
						}
						else if(i==boInsertPoint+2)
						{
							anarace->insertOrder(GAS_SCV,boInsertPoint);
//							msgWindow->addMessage(_T(wxString::Format(wxT("Item 2 was chosen")))); // => gas scv
//							update=2; TODO
						}
						else 
						{
//							msgWindow->addMessage(_T(wxString::Format(wxT("Opening 2. menu [%i] at %i"),i-boInsertPoint-1,boInsertPoint)));
							boGoalListOpened=i-boInsertPoint-1;
						}
					} else if(boGoalListOpened>1)
					{
						int l=0;
						int j;
						for(j=0;(j<UNIT_TYPE_COUNT)&&(l<=i-boInsertPoint);j++)
							if(stats[anarace->getPlayer()->getRace()][j].type==boGoalListOpened)
								l++;
						anarace->insertOrder(j,i-boInsertPoint);
//						msgWindow->addMessage(_T(wxString::Format(wxT("Item %i was chosen"),i-boInsertPoint-1)));
//						update=2; TODO
						//actually add an entry to the build order
					}
				}
				anarace->setOptimizing(0);
			}
		}
	}

	if((optimizeMode)&&(isActivated(0))) // Modus: alle
	{
		transformClientWidth(24);
		optimizeMode=0;
//		msgWindow->addMessage(_T("Mutations will now affect the whole build order."));
	} else if((!optimizeMode)&&(isActivated(1)))
	{
		transformClientWidth(-24);
		optimizeMode=1;
//		msgWindow->addMessage(_T("Mutations will now affect only the selected parts."));
//		msgWindow->addMessage(_T("Press continue as soon as you are done selecting."));
	}

	clearButtons();
	for(int i=0;i<MAX_LENGTH;i++) //? TODO ???
	{
		optButton[i]=9999;
		unitButton[i]=9999;
	}
};



int BoWindow::CheckForInfoWindow()
{
	NODE *node=orderList.GetFirst();
	while(node)
	{
		ORDER* order=node->GetData();
		int row=((boInsertPoint>-1)&&(order->row>=boInsertPoint))*(boEndPoint-boInsertPoint);
		wxRect edge(order->rect.GetPosition()+getInnerPosition()+wxPoint(0,-getScrollY()+((boInsertPoint>-1)&&(order->row>=boInsertPoint))*(boEndPoint-boInsertPoint)*(FONT_SIZE+5)-1),wxSize(order->rect.GetWidth(),FONT_SIZE+6));
		if((fitToClientArea(edge)&&edge.Inside(controls.getCurrentPosition())))
		{
			//infoWindow->adjustWindow(wxRect(infoWindow->getLeftBound(),edge.y+getInnerUpperBound()-50,infoWindow->getWidth(),infoWindow->getHeight()));
			return(node->GetKey()+1);
		}
		node=node->GetNext();
	}
	return(0);
};

void BoWindow::CheckOrders()
{
	int k=0;
	for(int s=MAX_LENGTH;s--;)
// /home/clawg/work/sc1041/sc/exe/../scc2/scc2.cpp:843: undefined reference to `ANARACE::getProgramIsBuilt(int)' <- WTF? nur bei exe...
		if(anarace->getProgramIsBuilt2(s)/*&&(anarace->getProgramTime(s)<=ga->maxTime-anarace->getTimer())*/)
		{
			if(NODE* node=orderList.Find(anarace->getMarker(s))) // => found old one -> update the data!
			{
				ORDER* order=node->GetData();
				wxRect t=wxRect(0,(k+1)*(FONT_SIZE+5),SECOND_COLOUMN-8,FONT_SIZE+4);
				if(order->target!=t)
					order->start=order->rect;
				order->target=t;
				order->row=k+1;
				order->blendTarget=50;
				order->blendStart=order->blend;
		   	 	order->unit=anarace->phaenoCode[s];
				order->IP=s;
				order->checked=1;
			} // => aktualisieren
			else // => neues erstellen
			{
				//TODO: testen ob anderes item da ist, das aber die gleiche Unit besitzt + an die gleiche Position kommt
				int found=0;
				NODE *node=orderList.GetFirst();
				while(node&&(!found))
				{
					if((node->GetData()->unit!=anarace->phaenoCode[s])||(node->GetData()->row!=k+1)||(node->GetData()->target.x>0))
						node=node->GetNext();
					else //=> ueberschreiben
					{
						ORDER* order=node->GetData();
						order->blendTarget=50;
						order->blendStart=order->blend;
						//order->dx=0;order->dy=0;
						//order->y=(k+1)*(FONT_SIZE+5);
						order->row=k+1; //?
						wxRect t=wxRect(0,(k+1)*(FONT_SIZE+5),SECOND_COLOUMN-8,FONT_SIZE+4);
						if(order->target!=t)
							order->start=order->rect;
						order->target=t; //?
						order->bonew=1;
						order->IP=s;
						order->unit=anarace->phaenoCode[s];
						order->checked=1;
						found=1;
					}
				}
				if(!found)
				{
					ORDER* order=new ORDER;
					order->rect=wxRect(170,(k+1)*(FONT_SIZE+5),0,FONT_SIZE+4);
					wxRect t=wxRect(0,(k+1)*(FONT_SIZE+5),SECOND_COLOUMN-8,FONT_SIZE+4);
					if(order->target!=t)
						order->start=order->rect;
					order->target=t;
					order->blend=1;order->blendTarget=50;
					order->blendStart=order->blend;
					order->bonew=1;
					order->unit=anarace->phaenoCode[s];
					order->row=k+1;
					order->IP=s;
					order->checked=1;
					orderList.Append((long)anarace->getMarker(s),order);
				}
			}
			k++;
		}
																			    
		NODE* node=orderList.GetFirst();
																			    
		while(node)
		{
			if(!node->GetData()->checked)
				node=orderList.DeleteNode(node);
			else
			{
				node->GetData()->checked=0;
				node=node->GetNext();
			}
		}
};
																			    
																			  
void BoWindow::MoveOrders()
{
	NODE *node=orderList.GetFirst();
	while(node)
	{
		ORDER* order=node->GetData();
		move(order->rect.x,order->start.x,order->target.x);
		move(order->rect.y,order->start.y,order->target.y);
		move(order->rect.width,order->start.width,order->target.width);
		move(order->rect.height,order->start.height,order->target.height);
		move(order->blend, order->blendStart, order->blendTarget);
		node=node->GetNext();
	}
};

