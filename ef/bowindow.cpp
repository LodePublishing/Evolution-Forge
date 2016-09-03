#include "bowindow.hpp"

BoEntry::BoEntry(UI_Object* parent, Rect rect, Rect maxRect, string unit):UI_Button(parent, rect, maxRect, unit, unit, FORCE_ENTRY_BUTTON, NO_TEXT_MODE, NO_BUTTON_MODE, DO_NOT_ADJUST, SMALL_NORMAL_BOLD_FONT, NO_AUTO_SIZE)
    // TODO!
{
/*    addUnit = new UI_Button(this, Rect(Point(getWidth()-117,2),Size(8,8)), Rect(Point(0,0),getSize()), ADD_BUTTON, PRESS_BUTTON_MODE);
    subUnit = new UI_Button(this, Rect(Point(getWidth()-107,2),Size(8,8)), Rect(Point(0,0),getSize()), SUB_BUTTON, PRESS_BUTTON_MODE);
    cancelUnit = new UI_Button(this, Rect(Point(getWidth()-97,2),Size(8,8)), Rect(Point(0,0),getSize()), CANCEL_BUTTON, PRESS_BUTTON_MODE);*/
};

                                                                                                                                                           
BoEntry::~BoEntry()
{
/*    if(addUnit)
        delete(addUnit);
    if(subUnit)
        delete(subUnit);
    if(cancelUnit)
        delete(cancelUnit);*/
};
                                                                                                                                                            
int BoEntry::changed()
{
//    if(addUnit->isPressed()) return(1);
  //  if(subUnit->isPressed()) return(2);
//    if(cancelUnit->isPressed()) return(3);
    return(0);
};
                                                                                                                                                            
void BoEntry::updateText(string utext)
{
    updateNormalText(utext);
    updatePressedText(utext);
};
                                                                                                                                                            
BoWindow::BoWindow(UI_Object* parent, ANARACE* anarace, InfoWindow* infoWindow, map <long, Order>* orderList, const int windowNumber):UI_Window(parent, BOWINDOW_TITLE_STRING, BUILD_ORDER_WINDOW, windowNumber, SCROLLED, AUTO_SIZE_ADJUST, NOT_TABBED, Rect(0,50,1000,1000))
{
//	addButton(Rect(getClientRectLeftBound()+getWidth()-48,getClientRectUpperBound()-30,12,12), 0, PERM_BUTTON);
//	addButton(Rect(getClientRectLeftBound()+getWidth()-48,getClientRectUpperBound()-15,12,12), 0, PERM_BUTTON);
	this->orderList=orderList;
	this->infoWindow=infoWindow;
	this->anarace=anarace;
	resetData();
    for(int i=MAX_LENGTH;i--;)
    {
        boEntry[i]=new BoEntry(this, Rect(getRelativeClientRectPosition()+Point(400,400), Size(getClientRectWidth(),FONT_SIZE+5)),
                Rect(getRelativeClientRectPosition(), getClientRectSize()+Size(0, getMaxRect().height)),  // max size -y? TODO
                        stats[(*anarace->getStartCondition())->getRace()][i].name);
        boEntry[i]->Hide();
    };
};

BoWindow::~BoWindow()
{
    for(int i=MAX_LENGTH;i--;)
        if(boEntry[i])
            delete boEntry[i]; 
};

void BoWindow::resetData()
{
//	int i;
	boInsertPoint=-1;
	boEndPoint=-1;
	boGoalListOpened=0;
	optimizeMode=0;
	markedUnit=0;
//	for(i=0;i<MAX_LENGTH;i++)
//		selection[i]=1;
};

void BoWindow::process()
{
	if(!shown) return;
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
//			msgWindow->addMessage(_T(string::Format(T("Button %i pressed"),i)));
//			if(boInsertPoint==-1) //all closed
//			{
//				msgWindow->addMessage(_T(string::Format(T("Opening menu at %i"),i)));
//				boInsertPoint=i+1;
//				anarace->setOptimizing(false);
//				boGoalListOpened=1; //scheint zu funzen
//			}
//			else 
			if(i<boInsertPoint)
			{
//				msgWindow->addMessage(_T(string::Format(T("Closing menu at %i and opening menu at %i"),boInsertPoint,i)));
				boInsertPoint=i+1;
				anarace->setOptimizing(false);
				boGoalListOpened=1;
			} 
			else if(i>=boEndPoint)
			{
//				msgWindow->addMessage(_T(string::Format(T("Closing menu at %i and opening menu at %i"),boInsertPoint,boInsertPoint-boEndPoint+i)));
				boInsertPoint=boInsertPoint-boEndPoint+i+1;
				anarace->setOptimizing(false);
				boGoalListOpened=1;
			} else
			{
				if(/*(i+1==boEndPoint)||*/(i==boInsertPoint)) //oeffne/schliesse Knopf <- aha...
				{
					if(boGoalListOpened==1) //=> schliesse
					{
//						msgWindow->addMessage(_T(string::Format(T("Closing menu at %i"),boInsertPoint)));
						boInsertPoint=-1;
						boEndPoint=0;
						boGoalListOpened=0;
					} // => schliessen
					else if(boGoalListOpened>1)
					{
//						msgWindow->addMessage(_T(string::Format(T("Returning to 1. menu at %i"),boInsertPoint)));
						boGoalListOpened=1;
					} // => schliessen
				} else // aha ein menueeintrag
				{
//						msgWindow->addMessage(_T(string::Format(T("%i %i %i"),boInsertPoint,boEndPoint,i)));
					if(boGoalListOpened==1) // => 1. Untermenue
					{
						if(i==boInsertPoint+1)
						{
							anarace->insertOrder(SCV,boInsertPoint);
//							msgWindow->addMessage(_T(string::Format(T("Item 1 was chosen")))); // => scv
//							update=2; TODO
						}
						else if(i==boInsertPoint+2)
						{
							anarace->insertOrder(GAS_SCV,boInsertPoint);
//							msgWindow->addMessage(_T(string::Format(T("Item 2 was chosen")))); // => gas scv
//							update=2; TODO
						}
						else 
						{
//							msgWindow->addMessage(_T(string::Format(T("Opening 2. menu [%i] at %i"),i-boInsertPoint-1,boInsertPoint)));
							boGoalListOpened=i-boInsertPoint-1;
						}
					} else if(boGoalListOpened>1)
					{
						int l=0;
						int j=0;
						for(j=0;(j<UNIT_TYPE_COUNT)&&(l<i-boInsertPoint);j++)
							if(stats[(*anarace->getStartCondition())->getRace()][j].facilityType==boGoalListOpened+1) l++;
						anarace->insertOrder(j-1,boInsertPoint);
//						msgWindow->addMessage(_T(string::Format(T("Item %i was chosen"),i-boInsertPoint-1)));
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

		if(controls.isShiftPressed()&&insideClientRect(controls.getCurrentPosition()-Point(0,(FONT_SIZE+5))))
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

		int line=0;
	boEndPoint=0;
	int oldTime=0;
	int tempForceCount[UNIT_TYPE_COUNT];
	for(int i=UNIT_TYPE_COUNT;i--;)
		tempForceCount[i]=0;

	// hack for minimum size ~~
  //  Rect edge=Rect(getRelativeClientRectPosition()+Point(0,100),Size(100,20));
    //fitItemToRelativeClientRect(edge,1);

	for(map<long, Order>::iterator order=orderList->begin(); order!=orderList->end(); ++order)
	{
		int row=((boInsertPoint>-1)&&(order->second.row>=boInsertPoint))*(boEndPoint-boInsertPoint);
		Rect edge=Rect(getRelativeClientRectPosition()+order->second.rect.GetPosition()+Point(0,row*(FONT_SIZE+5)-getScrollY()),order->second.rect.GetSize());
		if(fitItemToRelativeClientRect(edge,1))
		{
			boEntry[line]->Show();
			if(boEntry[line]->isCurrentlyHighlighted()) markedUnit = line;
            boEntry[line]->adjustRelativeRect(edge);
			boEntry[line]->setButton(eButton(UNIT_TYPE_0_BUTTON+stats[(*anarace->getStartCondition())->getRace()][order->second.unit].unitType));

//			int bright=0;
//				if((infoWindow->isShown())&&(order->second.IP==infoWindow->getIP())||(markedUnit==order->second.unit)) // TODO
//					bright=50+markAni;

			/*	dc->SetBrush(
							Brush(
							dc->brightenColor(
								theme.lookUpBrush((eBrush)(BRIGHT_UNIT_TYPE_1_BRUSH+stats[(*anarace->getStartCondition())->getRace()][anarace->getPhaenoCode(order->second.IP)].facilityType))->GetColor(), (100+bright)*order->second.blend/50
								             ), 
							    theme.lookUpBrush((eBrush)(BRIGHT_UNIT_TYPE_1_BRUSH+stats[(*anarace->getStartCondition())->getRace()][anarace->getPhaenoCode(order->second.IP)].facilityType))->GetStyle()));*/
// optimizeMode...						

//else dc->SetBrush(Brush(dc->doColor(
//(COLOR2R+BOcolor[stats[(*anarace->getStartCondition())->getRace()][order->second.unit].facilityType].Red())*order->blend/50,
//(COLOR2G+BOcolor[stats[(*anarace->getStartCondition())->getRace()][order->second.unit].facilityType].Green())*order->blend/50,
//(COLOR2B+BOcolor[stats[(*anarace->getStartCondition())->getRace()][order->second.unit].facilityType].Blue())*order->blend/50),SOLID));

//					dc->DrawRoundedRectangle(edge,4);
//					dc->SetTextForeground(*theme.lookUpBrush((eBrush)(UNIT_TYPE_1_BRUSH+stats[(*anarace->getStartCondition())->getRace()][anarace->getPhaenoCode(order->second.IP)].facilityType))->GetColor());

					if(anarace->getProgramTime(order->second.IP)!=oldTime)
						for(int i=UNIT_TYPE_COUNT;i--;)
								tempForceCount[i]=0;
					if(edge.width>=110)
					{
						ostringstream os;
						os << tempForceCount[anarace->getPhaenoCode(order->second.IP)]+anarace->getProgramTotalCount(order->second.IP, anarace->getPhaenoCode(order->second.IP))+1 << ".";
						os <<  stats[(*anarace->getStartCondition())->getRace()][anarace->getPhaenoCode(order->second.IP)].name;
//						dc->DrawText(os.str(),edge.GetPosition()+Point(5,0));os.str("");
//						dc->DrawText(stats[(*anarace->getStartCondition())->getRace()][anarace->getPhaenoCode(order->second.IP)].name,edge.GetPosition()+Point(20,0));
						boEntry[line]->updateText(os.str());
	//					if(row+order->second.row==boEndPoint) //~~
	//						dc->DrawBitmap(bmpArrowDown,edge.x+edge.width-12,edge.y+1);
	//					else
	//						dc->DrawBitmap(bmpArrowUp,edge.x+edge.width-12,edge.y+1);
//						if(optimizeMode)
//							optButton[row+t]=addButton(Rect(getClientRectLeftBound()+edge.width,edge.y,getWidth()-edge.width,FONT_SIZE+5));
//						orderButton[order->second.IP]=addButton(edge, "click right to remove order"); //~~~ TODO
					} else boEntry[line]->updateText("");
				
					if(anarace->getProgramTime(order->second.IP)==oldTime)
						tempForceCount[anarace->getPhaenoCode(order->second.IP)]++;
					else					
						oldTime=anarace->getProgramTime(order->second.IP);
/*					if(edge.width>=140)
						dc->DrawText(_T(string::Format(T("%i"),order->second.mins)),110+edge.x,edge.y);
					if(edge.width>=180)
						dc->DrawText(_T(string::Format(T("%i"),order->second.gas)),150+edge.x,edge.y);
					if(edge.width>=230)
						dc->DrawText(_T(string::Format(T("%i/%i"),order->second.needSupply,order->haveSupply)),190+edge.x,edge.y);
					if(edge.width>=260)
						dc->DrawText(_T(string::Format(T("%.2i:%.2i"),order->second.time/60,order->time%60)),240+edge.x,edge.y);
	//	  			dc->DrawText(_T(string::Format(T("%i"),order->second.successType)),190+order->x,order->y);*/
//					}
		} // end fit item
		else 
		{
            boEntry[line]->Hide();
			boEntry[line]->adjustRelativeRect(Rect(getRelativeClientRectPosition()+Point(400,400), Size(getClientRectWidth(),FONT_SIZE+5)));			
		}
		line++;
	} // end for ...
	

};


void BoWindow::draw(DC* dc) const
{
	if(!shown) return;
	UI_Window::draw(dc);

	drawSelectionStuff(dc);
	
	
	/*if((orderList->getMakeSpace()>-1)&&(boGoalListOpened==0))
	{
		dc->SetTextForeground(dc->doColor(0,200,0));
		dc->DrawText(*theme.lookUpString(CLICK_TO_INSERT_ORDER_STRING),getClientRectPosition()+Point(0,orderList->getMakeSpace()*(FONT_SIZE+5)-getScrollY()));
	}*/ // TODO

/*	dc->SetPen(*theme.lookUpPen(RECTANGLE_PEN));
	if(boInsertPoint>-1)
	{
		int line=boInsertPoint;
		Rect edge=Rect(getAbsoluteClientRectPosition()+Point(0,line*(FONT_SIZE+5)-getScrollY()),Size(270,FONT_SIZE+4));
		if(fitItemToAbsoluteClientRect(edge,1))
		{
			int bright=0;
					if(edge.Inside(controls.getCurrentPosition()))
				bright=50;
			//if(t%2==0)
				dc->SetBrush(Brush(dc->brightenColor(theme.lookUpBrush(FORCE_LIST_BUTTON_BRUSH)->GetColor(), bright), theme.lookUpBrush(FORCE_LIST_BUTTON_BRUSH)->GetStyle()));
			//else
			//dc->SetBrush(Brush(dc->doColor(COLOR2R,COLOR2G,COLOR2B),SOLID));
			dc->DrawRoundedRectangle(edge,4);
//			brightenColor(FORCE_ADD_GOAL_TEXT_COLOUR,bright); TODO
//			dc->SetTextForeground(dc->doColor(0+bright,200+bright,0+bright)); //TODO
			if(boGoalListOpened==1)
			{
				dc->DrawBitmap(*theme.lookUpBitmap(ARROW_UP_BITMAP), edge.GetPosition()+Point(4,2));
				dc->DrawText(*theme.lookUpString(CLICK_TO_CLOSE_STRING), edge.GetPosition()+Point(14,0));
			}
			else
			{
				dc->DrawBitmap(*theme.lookUpBitmap(ARROW_UP_BITMAP), edge.GetPosition()+Point(4,2));
				dc->DrawText(*theme.lookUpString(CLICK_TO_GO_BACK_STRING),edge.GetPosition()+Point(4,2));
			}
//			unitButton[line]=addButton(edge); TODO
		}
		line++;
		if(boGoalListOpened==1)
		{
			for(int i=1;i<11;i++)
			{
				Rect edge=Rect(getAbsoluteClientRectPosition()+Point(10,line*(FONT_SIZE+5)-getScrollY()),Size(270,FONT_SIZE+4));
				int bright=0;
				if(edge.Inside(controls.getCurrentPosition()))
						bright=50;
				lastBogoal=edge.GetBottom();
				if(fitItemToAbsoluteClientRect(edge,1))
				{
					dc->SetBrush(Brush(dc->brightenColor(theme.lookUpBrush((eBrush)(BRIGHT_UNIT_TYPE_1_BRUSH+i))->GetColor(), 100+bright), theme.lookUpBrush((eBrush)(BRIGHT_UNIT_TYPE_1_BRUSH+i))->GetStyle()));
					dc->DrawRoundedRectangle(edge,4);
					dc->SetTextForeground(dc->brightenColor(theme.lookUpColor(TEXT_COLOUR),bright));
					dc->DrawText(*theme.lookUpString((eString)(UNIT_TYPE_1_STRING+i)),edge.GetPosition()+Point(10,0));
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
				if(stats[(*anarace->getStartCondition())->getRace()][i].facilityType==type)
				{
					Rect edge=Rect(getAbsoluteClientRectPosition()+Point(20,line*(FONT_SIZE+5)-getScrollY()),Size(270,FONT_SIZE+4));
					int bright=0;
					if(edge.Inside(controls.getCurrentPosition()))
						bright=50;

					lastBogoal=edge.y;
					if(fitItemToAbsoluteClientRect(edge, 1))
					{
	                    dc->SetBrush(Brush(dc->brightenColor(theme.lookUpBrush((eBrush)(BRIGHT_UNIT_TYPE_1_BRUSH+type))->GetColor(), 100+bright), theme.lookUpBrush((eBrush)(BRIGHT_UNIT_TYPE_1_BRUSH+type))->GetStyle()));
		 				dc->DrawRoundedRectangle(edge,4);
						dc->SetTextForeground(*theme.lookUpColor(TEXT_COLOUR));
						dc->DrawText(stats[(*anarace->getStartCondition())->getRace()][i].name, edge.GetPosition()+Point(10,0));
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
			dc->SetPen(Pen(dc->doColor(200,0,0),2,SOLID));
			dc->DrawLine(edge.x+getClientRectLeftBound(),edge.y+getClientRectUpperBound(),edge.x+edge.width+getClientRectLeftBound(),edge.y+getClientRectUpperBound());
			dc->SetPen(Pen(dc->doColor(0,0,0),1,SOLID));
		}*/
//		boEndPoint=line+1;
		//end
//	}*/
	

	
//	if(makeSpace!=99999) TODO
//		makeSpaceButton=addButton(Rect(getClientRectPosition()+Point(0,makeSpace*(FONT_SIZE+5)-getScrollY()),Size(SECOND_COLOUMN-8,FONT_SIZE+4)));
//	else makeSpaceButton=99999;
};

void BoWindow::drawSelectionStuff(DC* dc) const
{
		dc->SetPen(*theme.lookUpPen(SELECT_PEN));
		dc->SetTextForeground(*theme.lookUpColor(WINDOW_TEXT_COLOUR));
		dc->SetFont(theme.lookUpFont(SMALL_ITALICS_BOLD_FONT));
		dc->DrawText(*theme.lookUpString(OPTIMIZE_EVERYTHING_STRING), Point(0,-32)+getAbsoluteClientRectPosition());
		dc->DrawText(*theme.lookUpString(OPTIMIZE_SELECTED_STRING), getAbsoluteClientRectPosition()+Point(0,-18));
		dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
		Rect edge=Rect(getAbsoluteClientRectPosition()+Point(getClientRectWidth()-32,-32),Size(12,12));
		Rect edge2=Rect(getAbsoluteClientRectPosition()+Point(getClientRectWidth()-32,-18),Size(12,12));
																				
		dc->DrawRoundedRectangle(edge,3);
		dc->DrawRoundedRectangle(edge2,3);
																				
		dc->SetTextForeground(*theme.lookUpColor(FORCE_TEXT_COLOUR));
//		if(!optimizeMode) dc->DrawCheckMark(edge);
//		else
//		if(optimizeMode)
//		{
//			dc->DrawBitmap(*theme.lookUpBitmap(ARROW_DOWN_BITMAP), getClientRectLeftBound()+getWidth()-46,getClientRectUpperBound()-4);
//			dc->DrawCheckMark(edge2);
//		}
																				
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
			Point pointsKlammer[9];
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
				dc->SetPen(Pen(dc->doColor(50,200,50),1,SOLID));
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




void BoWindow::setMarkedUnit(int unit)
{
	markedUnit=unit;
};

void BoWindow::checkForInfoWindow()
{
    for(map<long, Order>::iterator order=orderList->begin(); order!=orderList->end(); ++order)
    {
//        int row=((boInsertPoint>-1)&&(order->row>=boInsertPoint))*(boEndPoint-boInsertPoint);
//      mouse on order in player reinschieben, da is ja auch orderList zuhause
		Rect edge(order->second.rect.GetPosition()+getRelativeClientRectPosition()+Point(0,-getScrollY()+((boInsertPoint>-1)&&(order->second.row>=boInsertPoint))*(boEndPoint-boInsertPoint)*(FONT_SIZE+5)-1),Size(order->second.rect.GetWidth(),FONT_SIZE+6));

		if((fitItemToRelativeClientRect(edge)&&edge.Inside(controls.getCurrentPosition())))
        {
//window[INFO_WINDOW]->adjustWindow(Rect(Point(window[INFO_WINDOW]->getPosition().x,500)/*order->second.rect.GetBottom()+getClientRectUpperBound()-getScrollY()+((boInsertPoint>-1)&&(order->row>=boInsertPoint))*(boEndPoint-boInsertPoint)*(FONT_SIZE+5)-1)*/,window[INFO_WINDOW]->getSize()));
            infoWindow->setUnit(order->second.unit);
            infoWindow->setIP(order->second.IP);
            infoWindow->setBx(order->second.brect.x);
            infoWindow->setBWidth(order->second.brect.width);
            infoWindow->Show(1);
            return;
        }
    }
};

