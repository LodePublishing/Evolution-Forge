#include "bowindow.hpp"
#include "../core/settings.hpp"


BoEntry::BoEntry(const BoEntry& object) :
	UI_Button((UI_Button)object)
{ }
	
BoEntry& BoEntry::operator=(const BoEntry& object)
{
	((UI_Button)(*this)) = ((UI_Button)object);
	return(*this);
}

BoEntry::BoEntry(UI_Object* bo_parent, Rect bo_rect, Rect bo_max_rect, const string& bo_unit):
	UI_Button(bo_parent, bo_rect, bo_max_rect, bo_unit, bo_unit, FORCE_ENTRY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, NO_BUTTON_MODE, DO_NOT_ADJUST, SMALL_NORMAL_BOLD_FONT, FULL_WIDTH)
	// TODO!
{
/*	addUnit = new UI_Button(this, Rect(Point(getWidth()-117,2),Size(8,8)), Rect(Point(0,0),getSize()), ADD_BUTTON, PRESS_BUTTON_MODE);
	subUnit = new UI_Button(this, Rect(Point(getWidth()-107,2),Size(8,8)), Rect(Point(0,0),getSize()), SUB_BUTTON, PRESS_BUTTON_MODE);
	cancelUnit = new UI_Button(this, Rect(Point(getWidth()-97,2),Size(8,8)), Rect(Point(0,0),getSize()), CANCEL_BUTTON, PRESS_BUTTON_MODE);*/
}

BoEntry::~BoEntry()
{
/*	delete(addUnit);
	delete(subUnit);
	delete(cancelUnit);*/
}

void BoEntry::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Button::draw(dc);	
}

void BoEntry::process()
{
	if(!isShown())
		return;
	UI_Button::process();
}

const unsigned int BoEntry::changed()
{
//	if(addUnit->isLeftClicked()) return(1);
  //  if(subUnit->isLeftClicked()) return(2);
//	if(cancelUnit->isLeftClicked()) return(3);
	return(0);
}

void BoEntry::updateText(const string& utext)
{
	updateNormalText(utext);
	updatePressedText(utext);
}



BoWindow::BoWindow(const BoWindow& object) : 
    UI_Window((UI_Window)object),
    markedUnit(object.markedUnit),
    ownMarkedUnit(object.ownMarkedUnit),
    markedIP(object.markedIP),
    ownMarkedIP(object.ownMarkedIP),
    infoWindow(object.infoWindow),
    anarace(object.anarace),
    orderList(object.orderList),
    optimizeMode(object.optimizeMode),
    boInsertPoint(object.boInsertPoint),
    boEndPoint(object.boEndPoint),
    boGoalListOpened(object.boGoalListOpened),
    lastBogoal(object.lastBogoal),
    resetButton(new UI_Button(*(object.resetButton))),
    saveBuildOrderButton(new UI_Button(*(object.saveBuildOrderButton)))
//    speed(new NumberField(*(object.speed)))
{ }

BoWindow& BoWindow::operator=(const BoWindow& object)
{
	((UI_Window)(*this)) = ((UI_Window)object);
	markedUnit = object.markedUnit;
	ownMarkedUnit = object.ownMarkedUnit;
	markedIP = object.markedIP;
	ownMarkedIP = object.ownMarkedIP;
	infoWindow = object.infoWindow;
	anarace = object.anarace;
	orderList = object.orderList;
	optimizeMode = object.optimizeMode;
	boInsertPoint = object.boInsertPoint;
	boEndPoint = object.boEndPoint;
	boGoalListOpened = object.boGoalListOpened;
	lastBogoal = object.lastBogoal;
	delete resetButton;
	resetButton = new UI_Button(*(object.resetButton));
	delete saveBuildOrderButton;
	saveBuildOrderButton = new UI_Button(*(object.saveBuildOrderButton));
//	delete speed;
//	speed = new NumberField(*(object.speed));
	return(*this);
}

BoWindow::BoWindow(UI_Object* bo_parent, ANARACE* bo_anarace, InfoWindow* bo_info_window, std::map <long, Order>* bo_order_list, const unsigned int bo_window_number):
	UI_Window(bo_parent, BOWINDOW_TITLE_STRING, BUILD_ORDER_WINDOW, bo_window_number, SCROLLED, AUTO_SIZE_ADJUST, NOT_TABBED, Rect(0,25,1000,1000)),
	markedUnit(0),
	ownMarkedUnit(0),
	markedIP(0),
	ownMarkedIP(0),
	infoWindow(bo_info_window),
	anarace(bo_anarace),
	orderList(bo_order_list),
	optimizeMode(0),
	boInsertPoint(-1),
	boEndPoint(-1),
    boGoalListOpened(0),
	lastBogoal(0),
	resetButton(new UI_Button(this, Rect(getRelativeClientRectPosition(), getClientRectSize()), Rect(Point(0,0),getSize()), RESET_BUILD_ORDER_STRING, RESET_BUILD_ORDER_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, PRESS_BUTTON_MODE, ARRANGE_RIGHT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE)),
	saveBuildOrderButton(new UI_Button(this, Rect(getRelativeClientRectPosition(), getClientRectSize()), Rect(Point(0,0),getSize()), SAVE_BUILD_ORDER_STRING, SAVE_BUILD_ORDER_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, PRESS_BUTTON_MODE, ARRANGE_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE))
	//speed(new NumberField(this, Rect(getRelativeClientRectPosition(), Size(getClientRectWidth() - 100, 0)), SPEED_STRING, SETTING_FRAMERATE_TOOLTIP_STRING, 25, PERCENT_NUMBER_TYPE))
{
	resetData();
	for(int i=MAX_LENGTH;i--;)
	{
		boEntry[i]=new BoEntry(this, Rect(getRelativeClientRectPosition()+Point(200,200), Size(getClientRectWidth(), FONT_SIZE+5)),
				Rect(getRelativeClientRectPosition(), getMaxRect().GetSize()/*getClientRectSize()*/),  // max size -y? TODO
						stats[(*anarace->getStartCondition())->getRace()][i].name);
		boEntry[i]->Hide();
	}
	resetButton->updateToolTip("Reset and restart from the scratch");
	saveBuildOrderButton->updateToolTip("Save build order as bitmap or html");
}

BoWindow::~BoWindow()
{
	for(int i=MAX_LENGTH;i--;)
		delete boEntry[i]; 
	delete resetButton;
	delete saveBuildOrderButton;
//	delete speed;
}

void BoWindow::resetData()
{
//	int i;
	boInsertPoint=-1;
	boEndPoint=-1;
	boGoalListOpened=0;
	optimizeMode=0;
	markedUnit=0;markedIP=0;
	ownMarkedUnit=0;ownMarkedIP=0;
//	for(i=0;i<MAX_LENGTH;i++)
//		selection[i]=1;
}

void BoWindow::process()
{
	if(!isShown()) 
		return;
	UI_Window::process();

//	speed->updateNumber((int)(100/(settings.getSpeed()+1)));

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

/*		if(controls.isShiftPressed()&&insideClientRect(controls.getCurrentPosition()-Point(0,(FONT_SIZE+5))))
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
		} else*/ // TODO
		{
				orderList->setMakeSpace(-1);
				boGoalListOpened=0;
				boInsertPoint=-1;
				boEndPoint=-1;
		}
#endif

	unsigned int line=0;
	boEndPoint=0;
	unsigned int oldTime=MAX_TIME+1;
	ownMarkedUnit=0;
	ownMarkedIP=0;
	unsigned int tempForceCount[UNIT_TYPE_COUNT];
	for(int i=UNIT_TYPE_COUNT;i--;)
		tempForceCount[i]=0;

	// hack for minimum size ~~
  //  Rect edge=Rect(getRelativeClientRectPosition()+Point(0,100),Size(100,20));
	//fitItemToRelativeClientRect(edge,1);

	for(std::map<long, Order>::iterator order=orderList->begin(); order!=orderList->end(); ++order)
	{
		unsigned int row=0;//((boInsertPoint>-1)&&(order->second.getRow()>=boInsertPoint))*(boEndPoint-boInsertPoint);
		Rect edge=Rect(getRelativeClientRectPosition()+order->second.rect.GetTopLeft()+Point(0,row*(FONT_SIZE+5)-getScrollY()), order->second.rect.GetSize());
//		if(fitItemToRelativeClientRect(edge,1))
		{
			boEntry[line]->Show();
			if(boEntry[line]->isCurrentlyHighlighted())
			{
				ownMarkedUnit=order->second.getUnit();
				ownMarkedIP=order->second.getIP();
	            infoWindow->setUnit(order->second.getUnit());
    			infoWindow->setIP(order->second.getIP());
	            infoWindow->setBx(order->second.brect.GetLeft());
    	        infoWindow->setBWidth(order->second.brect.GetWidth());
        	    infoWindow->Show(1);
				// TODO 
				infoWindow->setFreeMove();
				infoWindow->adjustRelativeRect(Rect(getRelativeLeftBound() - 205, edge.GetTop() + getRelativeClientRectUpperBound() + 30, 200, 110));
			}
			Rect temp = boEntry[line]->startRect;
			boEntry[line]->adjustRelativeRect(edge);
			if(temp.GetLeft()!=boEntry[line]->startRect.GetLeft())
				boEntry[line]->resetGradient();
//				if((infoWindow->isShown())&&(order->second.getIP()==infoWindow->getIP()) // TODO evtl
			if(((markedUnit>0)&&(markedUnit==order->second.getUnit()))||
			   ((markedIP>0)&&(markedIP==order->second.getIP())))
					boEntry[line]->forceHighlighted();
//				else
//			   		boEntry[line]->forceDelighted();

			boEntry[line]->setButton(eButton(UNIT_TYPE_0_BUTTON+stats[(*anarace->getStartCondition())->getRace()][order->second.getUnit()].unitType));
			if(anarace->getProgramTime(order->second.getIP())!=oldTime)
				for(int i=UNIT_TYPE_COUNT;i--;)
					tempForceCount[i]=0;
					if((boEntry[line]->getWidth()<110)||(boEntry[line]->getRelativeLeftBound()>50))
					{
						boEntry[line]->updateText(" ");
					}
					else	
					{
						std::ostringstream os;
//						os << tempForceCount[order->second.getUnit()]+anarace->getProgramTotalCount(order->second.getIP(), order->second.getUnit())+1 << ".";
						os <<  stats[(*anarace->getStartCondition())->getRace()][order->second.getUnit()].name;
//						dc->DrawText(os.str(),edge.GetPosition()+Point(5,0));os.str("");
//						dc->DrawText(stats[(*anarace->getStartCondition())->getRace()][anarace->getPhaenoCode(order->second.getIP())].name,edge.GetPosition()+Point(20,0));
						boEntry[line]->updateText(os.str());
	//					if(row+order->second.row==boEndPoint) //~~
	//						dc->DrawBitmap(bmpArrowDown,edge.x+edge.width-12,edge.y+1);
	//					else
	//						dc->DrawBitmap(bmpArrowUp,edge.x+edge.width-12,edge.y+1);
//						if(optimizeMode)
//							optButton[row+t]=addButton(Rect(getClientRectLeftBound()+edge.width,edge.y,getWidth()-edge.width,FONT_SIZE+5));
//						orderButton[order->second.getIP()]=addButton(edge, "click right to remove order"); //~~~ TODO
					} 
					//else boEntry[line]->updateText("");*/
				
					if(anarace->getProgramTime(order->second.getIP())==oldTime)
						tempForceCount[order->second.getUnit()]++;
					else					
						oldTime=anarace->getProgramTime(order->second.getIP());
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
//		else 
//		{
//			boEntry[line]->Hide();
//			boEntry[line]->adjustRelativeRect(Rect(getRelativeClientRectPosition()+Point(200,200), Size(getClientRectWidth(),FONT_SIZE+5)));			
//		}
		line++;
	(fitItemToRelativeClientRect(edge,1));
	} // end for ...
	for(;line<MAX_LENGTH;line++)
	{
    	boEntry[line]->adjustRelativeRect(Rect(Point(max_x+10,getRelativeClientRectPosition().y+200), Size(getClientRectWidth(), FONT_SIZE+5)));
//		if(boEntry[line]->getAbsoluteRect().GetTopLeft() == getRelativeClientRectPosition()+Point(200,200))
//			boEntry[line]->Hide(); TODO
	}
	
		
//		boEntry[line]->Hide();
	

	for(int i=0;i<MAX_LENGTH;i++)
		if((boEntry[i]->getRelativeLeftBound()>300)&&(boEntry[i]->targetRect.GetLeft()>100))
			boEntry[i]->Hide();
	
	if(resetButton->isLeftClicked())
	{
		settings.assignRunParametersToSoup();
		setChangedFlag();
	}

	if(saveBuildOrderButton->isLeftClicked())
	{
		settings.saveBuildOrder(anarace);
	}

//	if(speed->addClicked() && (settings.getSpeed()>0))
//		settings.setSpeed(settings.getSpeed()-1);
//	if(speed->subClicked() && (settings.getSpeed()<100))
//		settings.setSpeed(settings.getSpeed()+1);

//	setMarkedUnit(ownMarkedUnit);
//	setMarkedIP(ownMarkedIP);
}

void BoWindow::draw(DC* dc) const
{
	if(!isShown()) 
		return;
	UI_Window::draw(dc);

//	drawSelectionStuff(dc);

//	if(infoWindow->isShown())
//		dc->DrawLine(
	
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
//			brightenColor(FORCE_ADD_GOAL_TEXT_COLOR,bright); TODO
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
					dc->SetTextForeground(dc->brightenColor(theme.lookUpColor(TEXT_COLOR),bright));
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
						dc->SetTextForeground(*theme.lookUpColor(TEXT_COLOR));
						dc->DrawText(stats[(*anarace->getStartCondition())->getRace()][i].name, edge.GetPosition()+Point(10,0));
//						unitButton[line]=addButton(edge, "insert this unit"); TODO
					}
					line++;
				}
			}
		}
//...		edge.x=20;
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
}

void BoWindow::drawSelectionStuff(DC* dc) const
{
		dc->SetPen(*theme.lookUpPen(SELECT_PEN));
		dc->SetTextForeground(*theme.lookUpColor(WINDOW_TEXT_COLOR));
		dc->SetFont(theme.lookUpFont(SMALL_ITALICS_BOLD_FONT));
		dc->DrawText(*theme.lookUpString(OPTIMIZE_EVERYTHING_STRING), Point(0,-32)+getAbsoluteClientRectPosition());
		dc->DrawText(*theme.lookUpString(OPTIMIZE_SELECTED_STRING), getAbsoluteClientRectPosition()+Point(0,-18));
		dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
		Rect edge=Rect(getAbsoluteClientRectPosition()+Point(getClientRectWidth()-32,-32),Size(12,12));
		Rect edge2=Rect(getAbsoluteClientRectPosition()+Point(getClientRectWidth()-32,-18),Size(12,12));
																				
		dc->DrawRoundedRectangle(edge,3);
		dc->DrawRoundedRectangle(edge2,3);
																				
		dc->SetTextForeground(*theme.lookUpColor(FORCE_TEXT_COLOR));
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
}

const unsigned int BoWindow::getMarkedIP() const
{
	return(ownMarkedIP);
}

void BoWindow::setMarkedIP(const unsigned int markedIP)
{
	this->markedIP=markedIP;
}

const unsigned int BoWindow::getMarkedUnit() const
{
	return(ownMarkedUnit);
}

void BoWindow::setMarkedUnit(const unsigned int markedUnit)
{
	this->markedUnit=markedUnit;
}


