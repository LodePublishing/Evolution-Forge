#include "bowindow.h"
#include "util.h"
#include "UI_Theme.h"

BoWindow::BoWindow(UI_Object* parent, wxRect rahmen, wxRect maxSize, ANARACE* anarace, InfoWindow* infoWindow, OrderList* orderList):UI_Window(parent, BOWINDOW_TITLE_STRING, rahmen,maxSize, SCROLLED, AUTO_SIZE_ADJUST, NOT_TABBED, wxRect(0,50,1000,1000))
{
//	addButton(wxRect(getClientRectLeftBound()+getWidth()-48,getClientRectUpperBound()-30,12,12), 0, PERM_BUTTON);
//	addButton(wxRect(getClientRectLeftBound()+getWidth()-48,getClientRectUpperBound()-15,12,12), 0, PERM_BUTTON);
	this->orderList=orderList;
	this->infoWindow=infoWindow;
	this->anarace=anarace;
	resetData();
};

BoWindow::~BoWindow()
{
};

void BoWindow::resetData()
{
//	int i;
	boInsertPoint=-1;
	boEndPoint=-1;
	boGoalListOpened=0;
	optimizeMode=0;
	resetButtons();
	markedUnit=0;
//	for(i=0;i<MAX_LENGTH;i++)
//		selection[i]=1;
};

void BoWindow::resetButtons()
{
	for(int i=0;i<2*MAX_LENGTH;i++) //? TODO ???
	{
		optButton[i]=9999;
		unitButton[i]=9999;
		orderButton[i]=9999;	
	}
	makeSpaceButton=99999;
};

void BoWindow::draw(wxDC* dc)
{
	UI_Window::draw(dc);

	drawSelectionStuff(dc);
	
	int tempForceCount[UNIT_TYPE_COUNT];
	int oldTime=0;
	
	for(int i=UNIT_TYPE_COUNT;i--;)
		tempForceCount[i]=0;
	boEndPoint=0;
	resetButtons();

	if((orderList->getMakeSpace()>-1)&&(boGoalListOpened==0))
	{
		dc->SetTextForeground(wxColour(0,200,0));
		dc->DrawText(*theme.lookUpString(CLICK_TO_INSERT_ORDER_STRING),getClientRectPosition()+wxPoint(0,orderList->getMakeSpace()*(FONT_SIZE+5)-getScrollY()));
	}

	dc->SetPen(wxPen(wxColour(0,0,0),1,wxSOLID));
	if(boInsertPoint>-1)
	{
		int line=boInsertPoint;
		wxRect edge=wxRect(getRelativeClientRectPosition()+wxPoint(0,line*(FONT_SIZE+5)-getScrollY()),wxSize(270,FONT_SIZE+4));
		if(fitItemToClientRect(edge,1))
		{
			int bright=0;
					if(edge.Inside(controls.getCurrentPosition()))
				bright=50;
			//if(t%2==0)
				dc->SetBrush(theme.lookUpBrightenedBrush(FORCE_LIST_BUTTON_BRUSH, bright));
			//else
				//dc->SetBrush(wxBrush(wxColour(COLOR2R,COLOR2G,COLOR2B),wxSOLID));
																				
			dc->DrawRoundedRectangle(edge,4);
			dc->SetTextForeground(wxColour(0+bright,200+bright,0+bright)); //TODO
			if(boGoalListOpened==1)
			{
				dc->DrawBitmap(*theme.lookUpBitmap(ARROW_UP_BITMAP), edge.x+4,edge.y+2);
				dc->DrawText(*theme.lookUpString(CLICK_TO_CLOSE_STRING),edge.x+14,edge.y);
			}
			else
			{
				dc->DrawBitmap(*theme.lookUpBitmap(ARROW_UP_BITMAP), edge.x+4,edge.y+2);
				dc->DrawText(*theme.lookUpString(CLICK_TO_GO_BACK_STRING),edge.x+14,edge.y);
			}
//			unitButton[line]=addButton(edge); TODO
		}
		line++;
		if(boGoalListOpened==1)
		{
			for(int i=1;i<11;i++)
			{
				wxRect edge=wxRect(getRelativeClientRectPosition()+wxPoint(10,line*(FONT_SIZE+5)-getScrollY()),wxSize(270,FONT_SIZE+4));
				int bright=0;
								if(edge.Inside(controls.getCurrentPosition()))
										bright=50;
				lastBogoal=edge.GetY();
				if(fitItemToClientRect(edge,1))
				{
					dc->SetBrush(theme.lookUpBrightenedBrush((eBrush)(BRIGHT_UNIT_TYPE_1_BRUSH+i),100+bright));
					dc->DrawRoundedRectangle(edge,4);
					dc->SetTextForeground(theme.lookUpBrightenedColour(TEXT_COLOUR, bright));
					dc->DrawText(*theme.lookUpString((eString)(UNIT_TYPE_1_STRING+i)),edge.x+10,edge.y);
//					unitButton[line]=addButton(edge); TODO
				}
				line++;
			}
		}
		else if(boGoalListOpened>1)
		{
			int type=boGoalListOpened+1;
			for(int i=0;i<UNIT_TYPE_COUNT;i++)
			{
				if(stats[anarace->getPlayer()->getRace()][i].facilityType==type)
				{
					wxRect edge=wxRect(getRelativeClientRectPosition()+wxPoint(20,line*(FONT_SIZE+5)-getScrollY()),wxSize(270,FONT_SIZE+4));
					int bright=0;
					if(edge.Inside(controls.getCurrentPosition()))
						bright=50;

					lastBogoal=edge.y;
					if(fitItemToClientRect(edge,1))
					{
						dc->SetBrush(theme.lookUpBrightenedBrush((eBrush)(BRIGHT_UNIT_TYPE_1_BRUSH+type),100+bright));
		 				dc->DrawRoundedRectangle(edge,4);
						dc->SetTextForeground(*theme.lookUpColour(TEXT_COLOUR));
						dc->DrawText(_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][i].name)),edge.x+10,edge.y);
//						unitButton[line]=addButton(edge, "insert this unit"); TODO
					}
					line++;
				}
			}
		}
/*		edge.x=20;
		edge.y=line*(FONT_SIZE+5)+9-getScrollY();
		edge.width=getClientRectWidth()-40;
		edge.height=2;
		if(fitItemToClientRect(edge))
		{
			dc->SetPen(wxPen(wxColour(200,0,0),2,wxSOLID));
			dc->DrawLine(edge.x+getClientRectLeftBound(),edge.y+getClientRectUpperBound(),edge.x+edge.width+getClientRectLeftBound(),edge.y+getClientRectUpperBound());
			dc->SetPen(wxPen(wxColour(0,0,0),1,wxSOLID));
		}*/
		boEndPoint=line+1;
		//end
	}
	
	if(markedUnit==0)
		markAni=0;
	else
	{		
		if(markAni>1)
			markAni-=4;
		else markAni=50;
	}

	int line=0;
	NODE* node=orderList->GetFirst();
	while(node)
	{
		ORDER* order=node->GetData();
		int row=((boInsertPoint>-1)&&(order->row>=boInsertPoint))*(boEndPoint-boInsertPoint);
		wxRect edge=wxRect(getRelativeClientRectPosition()+order->rect.GetPosition()+wxPoint(0,row*(FONT_SIZE+5)-getScrollY()),order->rect.GetSize());
		if(fitItemToClientRect(edge,1))
		{
			{
				int bright=0;
				if((infoWindow->isShown())&&(node->GetKey()==infoWindow->getKey())||(markedUnit==order->unit))
					bright=50+markAni;

                dc->SetBrush(theme.lookUpBrightenedBrush((eBrush)(UNIT_TYPE_1_BRUSH+stats[anarace->getPlayer()->getRace()][anarace->getPhaenoCode(order->IP)].facilityType),(100+bright)*order->blend/50));
// optimizeMode...						

//else dc->SetBrush(wxBrush(wxColour(
//(COLOR2R+BOcolor[stats[anarace->getPlayer()->getRace()][order->unit].facilityType].Red())*order->blend/50,
//(COLOR2G+BOcolor[stats[anarace->getPlayer()->getRace()][order->unit].facilityType].Green())*order->blend/50,
//(COLOR2B+BOcolor[stats[anarace->getPlayer()->getRace()][order->unit].facilityType].Blue())*order->blend/50),wxSOLID));

					dc->DrawRoundedRectangle(edge,4);
					dc->SetTextForeground(theme.lookUpBrush((eBrush)(UNIT_TYPE_1_BRUSH+stats[anarace->getPlayer()->getRace()][anarace->getPhaenoCode(order->IP)].facilityType))->GetColour());

					if(anarace->getProgramTime(order->IP)!=oldTime)
						for(int i=UNIT_TYPE_COUNT;i--;)
								tempForceCount[i]=0;
					if(edge.width>=110)
					{
						dc->DrawText(_T(wxString::Format(wxT("%i."),tempForceCount[anarace->getPhaenoCode(order->IP)]+anarace->getProgramForceCount(order->IP,anarace->getPhaenoCode(order->IP))+1)),edge.GetPosition()+wxPoint(5,0));
						dc->DrawText(_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][anarace->getPhaenoCode(order->IP)].name)),edge.GetPosition()+wxPoint(20,0));
	//					if(row+order->row==boEndPoint) //~~
	//						dc->DrawBitmap(bmpArrowDown,edge.x+edge.width-12,edge.y+1);
	//					else
	//						dc->DrawBitmap(bmpArrowUp,edge.x+edge.width-12,edge.y+1);
//						if(optimizeMode)
//							optButton[row+t]=addButton(wxRect(getClientRectLeftBound()+edge.width,edge.y,getWidth()-edge.width,FONT_SIZE+5));
//						orderButton[order->IP]=addButton(edge, "click right to remove order"); //~~~ TODO
					}
					
					if(anarace->getProgramTime(order->IP)==oldTime)
						tempForceCount[anarace->getPhaenoCode(order->IP)]++;
					else					
						oldTime=anarace->getProgramTime(order->IP);
/*					if(edge.width>=140)
						dc->DrawText(_T(wxString::Format(wxT("%i"),order->mins)),110+edge.x,edge.y);
					if(edge.width>=180)
						dc->DrawText(_T(wxString::Format(wxT("%i"),order->gas)),150+edge.x,edge.y);
					if(edge.width>=230)
						dc->DrawText(_T(wxString::Format(wxT("%i/%i"),order->needSupply,order->haveSupply)),190+edge.x,edge.y);
					if(edge.width>=260)
						dc->DrawText(_T(wxString::Format(wxT("%.2i:%.2i"),order->time/60,order->time%60)),240+edge.x,edge.y);
	//	  			dc->DrawText(_T(wxString::Format(wxT("%i"),order->successType)),190+order->x,order->y);*/
//					}
	//				unitButton[row+order->row]=addButton(wxRect(edge.x+edge.width-12,edge.y+1,8,8));
			}
		}
		line++;
		node=node->GetNext();
	} //order blend>0
//	if(makeSpace!=99999) TODO
//		makeSpaceButton=addButton(wxRect(getClientRectPosition()+wxPoint(0,makeSpace*(FONT_SIZE+5)-getScrollY()),wxSize(SECOND_COLOUMN-8,FONT_SIZE+4)));
//	else makeSpaceButton=99999;
};

void BoWindow::drawSelectionStuff(wxDC* dc)
{
		dc->SetPen(*theme.lookUpPen(SELECT_PEN));
		dc->SetTextForeground(*theme.lookUpColour(WINDOW_TEXT_COLOUR));
		dc->SetFont(*theme.lookUpFont(SMALL_ITALICS_BOLD_FONT));
		dc->DrawText(*theme.lookUpString(OPTIMIZE_EVERYTHING_STRING),getClientRectLeftBound(),getClientRectUpperBound()-32);
		dc->DrawText(*theme.lookUpString(OPTIMIZE_SELECTED_STRING),getClientRectLeftBound(),getClientRectUpperBound()-18);
		dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
		wxRect edge=wxRect(getClientRectLeftBound()+getWidth()-48,getClientRectUpperBound()-32,12,12);
		wxRect edge2=wxRect(getClientRectLeftBound()+getWidth()-48,getClientRectUpperBound()-18,12,12);
																				
		dc->DrawRoundedRectangle(edge,3);
		dc->DrawRoundedRectangle(edge2,3);
																				
		dc->SetTextForeground(*theme.lookUpColour(FORCE_TEXT_COLOUR));
		if(!optimizeMode) dc->DrawCheckMark(edge);
		else
		if(optimizeMode)
		{
			dc->DrawBitmap(*theme.lookUpBitmap(ARROW_DOWN_BITMAP), getClientRectLeftBound()+getWidth()-46,getClientRectUpperBound()-4);
			dc->DrawCheckMark(edge2);
		}
																				
/*	  int i;
//	  boanzahl=orderList->GetCount();
																				
	if(optimizeMode)
	{
	int start=0;
	int inrow=0;
	int l=0;
	for(i=0;i<orderList->getCount();i++)
	{
		if((!inrow)&&(selection[i])&&(i!=boInsertPoint))
		{
			inrow=1;
			start=l;
		} else if((inrow)&&((!selection[i])||(i==boInsertPoint)))
		{
			inrow=0;
			wxPoint pointsKlammer[9];
			int h1=start*(FONT_SIZE+5)-getScrollY()+11+getClientRectUpperBound();
			int h2=(l+1)*(FONT_SIZE+5)-getScrollY()-2+getClientRectUpperBound();
																				
			if((h1<getClientRectUpperBound())&&(h2>getClientRectUpperBound()))
				h1=getClientRectUpperBound(); //~~
			if((h1<getClientRectUpperBound()+getClientRectHeight())&&(h2>getClientRectUpperBound()+getClientRectHeight()))
				h2=getClientRectUpperBound()+getClientRectHeight();
																				
			if((h2<getClientRectUpperBound())||(h1>getClientRectUpperBound()+getClientRectHeight()))
				i=orderLength;
			else
			{
	  int x1=getClientRectLeftBound()+getClientRectWidth();
																				
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

void BoWindow::process()
{
	UI_Window::process();

	
	checkForInfoWindow();
#if 0
	if(isActivated(makeSpaceButton))
	{
		boInsertPoint=makeSpace;
		anarace->setOptimizing(false);
		boGoalListOpened=1;
	} else
	for(int i=0;i<MAX_LENGTH;i++)
	{
		if(getPressCondition(orderButton[i])==7)
		{
			anarace->removeOrder(i);
		}
		if(isActivated(optButton[i]))
		{
//			if((boInsertPoint==-1)||(i<boInsertPoint))
//				selection[i]=!selection[i];
//			else if(i>boEndPoint)
//				   selection[boInsertPoint-boEndPoint+i]=!selection[boInsertPoint-boEndPoint+i]; TODO
		}
		if(isActivated(unitButton[i]))
		{
//			msgWindow->addMessage(_T(wxString::Format(wxT("Button %i pressed"),i)));
//			if(boInsertPoint==-1) //all closed
//			{
//				msgWindow->addMessage(_T(wxString::Format(wxT("Opening menu at %i"),i)));
//				boInsertPoint=i+1;
//				anarace->setOptimizing(false);
//				boGoalListOpened=1; //scheint zu funzen
//			}
//			else 
			if(i<boInsertPoint)
			{
//				msgWindow->addMessage(_T(wxString::Format(wxT("Closing menu at %i and opening menu at %i"),boInsertPoint,i)));
				boInsertPoint=i+1;
				anarace->setOptimizing(false);
				boGoalListOpened=1;
			} 
			else if(i>=boEndPoint)
			{
//				msgWindow->addMessage(_T(wxString::Format(wxT("Closing menu at %i and opening menu at %i"),boInsertPoint,boInsertPoint-boEndPoint+i)));
				boInsertPoint=boInsertPoint-boEndPoint+i+1;
				anarace->setOptimizing(false);
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
						int j=0;
						for(j=0;(j<UNIT_TYPE_COUNT)&&(l<i-boInsertPoint);j++)
							if(stats[anarace->getPlayer()->getRace()][j].facilityType==boGoalListOpened+1) l++;
						anarace->insertOrder(j-1,boInsertPoint);
//						msgWindow->addMessage(_T(wxString::Format(wxT("Item %i was chosen"),i-boInsertPoint-1)));
//						update=2; TODO
						//actually add an entry to the build order
					}
				}
				anarace->setOptimizing(false);
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
		if(controls.isShiftPressed()&&insideClientRect(controls.getCurrentPosition()-wxPoint(0,(FONT_SIZE+5))))
		{
				int temp=(controls.getCurrentPosition().y-getClientRectUpperBound()-getScrollY())/(FONT_SIZE+5);
				if((boInsertPoint>-1)&&(temp>=boInsertPoint)&&(temp<=boEndPoint))
						temp=-1;
				if((orderList->getMakeSpace()!=temp)&&(temp>-1)&&(boGoalListOpened==1))
				{
						boGoalListOpened=0;
						boInsertPoint=-1;
						boEndPoint=-1;
				}
				orderList->setMakeSpace(temp);
		} else
		{
				orderList->setMakeSpace(-1);
				boGoalListOpened=0;
				boInsertPoint=-1;
				boEndPoint=-1;
		}
#endif

};


void BoWindow::setMarkedUnit(int unit)
{
	markedUnit=unit;
};

void BoWindow::checkForInfoWindow()
{
    NODE *node=orderList->GetFirst();
    while(node)
    {
        ORDER* order=node->GetData();
//        int row=((boInsertPoint>-1)&&(order->row>=boInsertPoint))*(boEndPoint-boInsertPoint);
//      mouse on order in player reinschieben, da is ja auch orderList zuhause
		wxRect edge(order->rect.GetPosition()+getRelativeClientRectPosition()+wxPoint(0,-getScrollY()+((boInsertPoint>-1)&&(order->row>=boInsertPoint))*(boEndPoint-boInsertPoint)*(FONT_SIZE+5)-1),wxSize(order->rect.GetWidth(),FONT_SIZE+6));

		if((fitItemToClientRect(edge)&&edge.Inside(controls.getCurrentPosition())))
        {
//window[INFO_WINDOW]->adjustWindow(wxRect(wxPoint(window[INFO_WINDOW]->getPosition().x,500)/*order->rect.GetY()+getClientRectUpperBound()-getScrollY()+((boInsertPoint>-1)&&(order->row>=boInsertPoint))*(boEndPoint-boInsertPoint)*(FONT_SIZE+5)-1)*/,window[INFO_WINDOW]->getSize()));
            infoWindow->setUnit(order->unit);
            infoWindow->setKey(node->GetKey());
            infoWindow->setOrder(order);
            infoWindow->setBx(order->brect.x);
            infoWindow->setBWidth(order->brect.width);
            infoWindow->Show(1);
            return;
        }
        node=node->GetNext();
    }
};

#if 0
void BoWindow::CheckOrders()
{
	int k=0;
	if(controls.isShiftPressed()&&insideClientRect(controls.getCurrentPosition()-wxPoint(0,(FONT_SIZE+5))))
	{
		int temp=(controls.getCurrentPosition().y-getClientRectUpperBound()-getScrollY())/(FONT_SIZE+5);
				if((boInsertPoint>-1)&&(temp>=boInsertPoint)&&(temp<=boEndPoint))
			temp=-1;
		if((makeSpace!=temp)&&(temp>-1)&&(boGoalListOpened==1))
		{
			boGoalListOpened=0;
			boInsertPoint=-1;
			boEndPoint=-1;
		}
		makeSpace=temp;
	} else 
	{
		makeSpace=-1;
				boGoalListOpened=0;
				boInsertPoint=-1;
				boEndPoint=-1;
	}
	

	for(int s=MAX_LENGTH;s--;)
// /home/clawg/work/sc1041/sc/exe/../scc2/scc2.cpp:843: undefined reference to `ANARACE::getProgramIsBuilt(int)' <- WTF? nur bei exe...
		if(anarace->getProgramIsBuilt2(s)/*&&(anarace->getProgramTime(s)<=ga->maxTime-anarace->getTimer())*/)
		{
			if(NODE* node=orderList->Find(anarace->getMarker(s))) // => found old one -> update the data!
			{
				ORDER* order=node->GetData();
				order->row=k+1+((makeSpace>-1)*(k+1>=makeSpace));
				wxRect t=wxRect(0,order->row*(FONT_SIZE+5),SECOND_COLOUMN-8,FONT_SIZE+4);
				if(order->target!=t)
					order->start=order->rect;
				order->target=t;
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
/*				NODE *node=orderList->GetFirst();
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
						order->row=k+1+((makeSpace>-1)*(k+1>=makeSpace)); //?
						wxRect t=wxRect(0,order->row*(FONT_SIZE+5),SECOND_COLOUMN-8,FONT_SIZE+4);
						if(order->target!=t)
							order->start=order->rect;
						order->target=t; //?
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
					order->row=k+1+((makeSpace>-1)*(k+1>=makeSpace));
					order->rect=wxRect(170,order->row*(FONT_SIZE+5),0,FONT_SIZE+4);
					wxRect t=wxRect(0,order->row*(FONT_SIZE+5),SECOND_COLOUMN-8,FONT_SIZE+4);
					if(order->target!=t)
						order->start=order->rect;
					order->target=t;
					order->blend=1;order->blendTarget=50;
					order->blendStart=order->blend;
					order->bonew=1;
					order->unit=anarace->phaenoCode[s];
					order->IP=s;
					order->checked=1;
					orderList->Append((long)anarace->getMarker(s),order);
				}
			}
			k++;
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
																				
#if 0																			  
void BoWindow::MoveOrders()
{
	NODE *node=orderList->GetFirst();
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
#endif
