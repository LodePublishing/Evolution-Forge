#include "bowindow.hpp"
#include "../core/settings.hpp"
#include "../ui/editfield.hpp"

BoWindow::BoWindow(const BoWindow& object) : 
	UI_Window((UI_Window)object),
	infoWindow(object.infoWindow),
	anarace(object.anarace),
	orderList(object.orderList),
	optimizeMode(object.optimizeMode),
	boInsertPoint(object.boInsertPoint),
	boEndPoint(object.boEndPoint),
	boGoalListOpened(object.boGoalListOpened),
	lastBogoal(object.lastBogoal),
	resetButton(new UI_Button(*(object.resetButton))),
	saveBuildOrderButton(new UI_Button(*(object.saveBuildOrderButton))),
	msgWindow(object.msgWindow)
//	fixed(object.fixed)
{ }

BoWindow& BoWindow::operator=(const BoWindow& object)
{
	((UI_Window)(*this)) = ((UI_Window)object);
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
	return(*this);
	msgWindow = object.msgWindow;
//	fixed = object.fixed;
}

BoWindow::BoWindow(UI_Object* bo_parent, ANARACE* bo_anarace, InfoWindow* bo_info_window, MessageWindow* message_window, std::list<Order*>* bo_order_list/*, bool* fixed_list*/, const unsigned int bo_window_number) :
	UI_Window(bo_parent, BOWINDOW_TITLE_STRING, BUILD_ORDER_WINDOW, bo_window_number, SCROLLED, AUTO_SIZE_ADJUST, NOT_TABBED, Rect(0, 25, 1000, 1000)),
	infoWindow(bo_info_window),
	anarace(bo_anarace),
	orderList(bo_order_list),
	optimizeMode(0),
	boInsertPoint(-1),
	boEndPoint(-1),
	boGoalListOpened(0),
	lastBogoal(0),
	resetButton(new UI_Button(this, Rect(getRelativeClientRectPosition(), getClientRectSize()), RESET_BUILD_ORDER_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, PRESS_BUTTON_MODE, ARRANGE_TOP_RIGHT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE)),
	saveBuildOrderButton(new UI_Button(this, Rect(getRelativeClientRectPosition(), getClientRectSize()), SAVE_BUILD_ORDER_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, PRESS_BUTTON_MODE, ARRANGE_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE)),
	msgWindow(message_window)
//	fixed(fixed_list)
{
	resetData();
	resetButton->updateToolTip(RESET_BUILD_ORDER_TOOLTIP_STRING);
	saveBuildOrderButton->updateToolTip(SAVE_BUILD_ORDER_TOOLTIP_STRING);
}

BoWindow::~BoWindow()
{
	std::list<BoEntry*>::iterator i = boList.begin();
	while(i!=boList.end())
	{
		delete(*i);
		i = boList.erase(i);
	}
	delete resetButton;
	delete saveBuildOrderButton;
}

void BoWindow::assignAnarace(ANARACE* bo_anarace)
{
	anarace = bo_anarace;
}

void BoWindow::resetData()
{
//	int i;
	boInsertPoint=-1;
	boEndPoint=-1;
	boGoalListOpened=0;
	optimizeMode=0;
	
        std::list<BoEntry*>::iterator entry = boList.begin();
        while(entry != boList.end())
	{
		if(UI_Object::currentButton == *entry) UI_Object::currentButton = NULL;
		delete(*entry);
		entry = boList.erase(entry);
	}
									
//	for(i=0;i<MAX_LENGTH;i++)
//		selection[i]=1;
}

void BoWindow::reloadStrings()
{
	processList();
	UI_Object::reloadStrings();
}

void BoWindow::processList()
{
        std::list<BoEntry*>::iterator entry = boList.begin();
        int row = 2;
        for(std::list<Order*>::const_iterator order = orderList->begin(); order != orderList->end(); ++order)
        {
		
		Rect edge=Rect(getRelativeClientRectPosition()+Point(0, row*(FONT_SIZE+6)), Size(200, FONT_SIZE+5));//(*order)->rect.GetSize());// TODO
		(fitItemToRelativeClientRect(edge, 1));
//		edge.SetTopLeft(edge.GetTopLeft() - Size(0, getScrollY()));
		row++;
		if(entry == boList.end())
		{
			BoEntry* t = new BoEntry(this, Rect(Point(max_x, getRelativeClientRectPosition().y+200), Size(getClientRectWidth(), FONT_SIZE+5)),
			// max size -y? TODO
				*UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+(*order)->getUnit()+UNIT_NULL_STRING))); // (*anarace->getStartCondition())->getRace()?
			t->setButton(eButton(UNIT_TYPE_0_BUTTON+stats[(*anarace->getStartCondition())->getRace()][(*order)->getUnit()].unitType));
			t->adjustRelativeRect(edge);
			t->setUnit((*order)->getUnit());
			t->setIP((*order)->getIP());
			boList.push_back(t);
		} else 
		if((*entry)->getUnit() != (*order)->getUnit())
		{
			std::list<BoEntry*>::iterator k = entry;
                        while(k!=boList.end())
                        {
                                if((*k)->getUnit() == (*order)->getUnit())
                                        break;
                                k++;
                        }
                        if(k!=boList.end()) // => Found, move the entry
			{
                                BoEntry* old = *k;
                                entry = boList.insert(entry, old);
				entry++;
                                boList.erase(k);
                                old->adjustRelativeRect(edge);
				old->resetGradient();
				old->setIP((*order)->getIP());
			} else // => not found, insert a new one
			{
				BoEntry* t = new BoEntry(this, Rect(Point(max_x,getRelativeClientRectPosition().y+200), Size(getClientRectWidth(), FONT_SIZE+5)), *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+(*order)->getUnit()+UNIT_NULL_STRING))); // (*anarace->getStartCondition())->getRace()?
	                        t->setButton(eButton(UNIT_TYPE_0_BUTTON+stats[(*anarace->getStartCondition())->getRace()][(*order)->getUnit()].unitType));
				t->adjustRelativeRect(edge);
				t->setUnit((*order)->getUnit());
				t->setIP((*order)->getIP());
        	                entry = boList.insert(entry, t);
				entry++;
			}
		} else // ok
//              if((*entry)->getUnit() == (*order)->getUnit())
               	{
			(*entry)->setIP((*order)->getIP());
     	                (*entry)->adjustRelativeRect(edge);
                        entry++;
       	        }
	}
	
	while(entry != boList.end())
        {
                if(UI_Object::currentButton == *entry) UI_Object::currentButton = NULL;
                delete(*entry);
                entry = boList.erase(entry);
        }
}


void BoWindow::checkForInfoWindow()
{
	for(std::list<BoEntry*>::iterator bo = boList.begin(); bo != boList.end(); ++bo)
	{
//		if(boEntry[line]->locked())
//			fixed[line]=true;
//		else fixed[line]=false;
		if((*bo)->isCurrentlyHighlighted())
		{
			(*bo)->forceHighlighted();
			infoWindow->assignBo(*bo);
			infoWindow->setUnit((*bo)->getUnit());
			if((*bo)->getIP()!=infoWindow->getIP())
				infoWindow->setIP((*bo)->getIP());
			infoWindow->Show(1);
			infoWindow->adjustRelativeRect(Rect(getRelativeLeftBound() - 205, (*bo)->getAbsolutePosition().y + getRelativeClientRectUpperBound() - 5, 200, 110));
			return;
		}
	} // end for ...
	
/*	for(std::list<BoEntry*>::iterator bo = boList.begin(); bo != boList.end(); ++bo)
	{
		if((*bo)->getIP() == infoWindow->getIP())
		{
			(*bo)->forceHighlighted();
			infoWindow->assignBo(*bo);
			infoWindow->setUnit((*bo)->getUnit());
			if((*bo)->getIP()!=infoWindow->getIP())
				infoWindow->setIP((*bo)->getIP());
			infoWindow->Show(1);
			infoWindow->adjustRelativeRect(Rect(getRelativeLeftBound() - 205, (*bo)->getAbsolutePosition().y + getRelativeClientRectUpperBound() - 5, 200, 110));
			return;
		}
	} // end for ...*/
	infoWindow->bo=NULL;
}

void BoWindow::process()
{
	if(!isShown()) 
		return;
	UI_Window::process();
       /*	std::list<BoEntry*>::iterator entry = boList.begin();
	if((*entry)->getHeight()==FONT_SIZE+5)
	{
		BoEntry* j = *entry;
		int k = 1;
	}*/


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
	unsigned int tempForceCount[UNIT_TYPE_COUNT];
	memset(tempForceCount, 0, UNIT_TYPE_COUNT * sizeof(int));

// hack for minimum size ~~
//  Rect edge=Rect(getRelativeClientRectPosition()+Point(0,100),Size(100,20));
//fitItemToRelativeClientRect(edge,1);

	checkForInfoWindow();
	
	if(resetButton->isLeftClicked())
	{
		setResetFlag();
		msgWindow->addMessage("Resetted build order...");
	}

	if(saveBuildOrderButton->isLeftClicked()&&(!UI_Object::editTextField))
		UI_Object::editTextField = new UI_EditField(getParent(), saveBuildOrderButton, SAVE_BUILD_ORDER_AS_STRING, GIVE_BO_A_NAME_STRING);

	if((UI_Object::editTextField)&&(UI_Object::editTextField->getCaller()==saveBuildOrderButton))
	{
		if(UI_Object::editTextField->isCanceled())
		{
			delete UI_Object::editTextField;
			UI_Object::resetButton();
			UI_Object::editTextField=NULL;
		} else
		if(UI_Object::editTextField->isDone())
		{
			if(UI_Object::editTextField->getString().length()>0)
			{
				settings.saveBuildOrder(UI_Object::editTextField->getString(), anarace);
				ostringstream os;
				os << "Saved build order.";  // TODO
				msgWindow->addMessage(os.str());
			}
			delete UI_Object::editTextField;
			UI_Object::resetButton();
			UI_Object::editTextField=NULL;
		}
	}
}

void BoWindow::draw(DC* dc) const
{
	if(!isShown()) 
		return;
	UI_Window::draw(dc);

/*	int k = 0;
	for(std::list<Order*>::const_iterator order = orderList->begin(); order != orderList->end(); ++order)
	{
		dc->DrawText(stats[(*anarace->getStartCondition())->getRace()][(*order)->getUnit()].name, getAbsolutePosition() + Size(0, k*10));
		k++;
	}
	k = 0;
	for(int s=MAX_LENGTH;s--;)
		if(anarace->getProgramIsBuilt(s)&&(anarace->getRealProgramTime(s) + stats[anarace->getRace()][anarace->getPhaenoCode(s)].BT<=anarace->getRealTimer()))
		{
			dc->DrawText(stats[(*anarace->getStartCondition())->getRace()][anarace->getPhaenoCode(s)].name, getAbsolutePosition() + Size(100, k*10));
			k++;
		}	 */

//	ostringstream os;
//	os << new_one << "/" << add_end << "/" << moved << "/" << same << "/" << deleted << " [" << size1 << "/" << size2 << "]";
//	os << PRERACE::situationHashMap.size();
//	dc->DrawText(os.str(), getAbsoluteClientRectPosition() - Size(0, 8));



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

#if 0
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
#endif



