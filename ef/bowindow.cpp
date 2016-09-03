#include "bowindow.hpp"
#include "../core/database.hpp"
#include "savebox.hpp"

BoWindow::BoWindow(const BoWindow& object) : 
	UI_Window((UI_Window)object),
	anarace(object.anarace),
	moveTarget(),
	optimizeMode(object.optimizeMode),
	boInsertPoint(object.boInsertPoint),
	boEndPoint(object.boEndPoint),
	boGoalListOpened(object.boGoalListOpened),
	lastBogoal(object.lastBogoal),
	startLine(0),
	selectedItem(0),
	saveBuildOrderButton(new UI_Button(*(object.saveBuildOrderButton))),
	loadBuildOrderButton(new UI_Button(*(object.loadBuildOrderButton))),
	boMenu(new BoMenu(this, Rect(10, 40, 100, 0), Size(0,0), DO_NOT_ADJUST)),
	saveBox(NULL)
//	fixed(object.fixed),
{ }

BoWindow& BoWindow::operator=(const BoWindow& object)
{
	((UI_Window)(*this)) = ((UI_Window)object);
	anarace = object.anarace;
	moveTarget = Rect();
	optimizeMode = object.optimizeMode;
	boInsertPoint = object.boInsertPoint;
	boEndPoint = object.boEndPoint;
	boGoalListOpened = object.boGoalListOpened;
	lastBogoal = object.lastBogoal;
	startLine = 0;
	selectedItem = 0;
	delete saveBuildOrderButton;
	saveBuildOrderButton = new UI_Button(*(object.saveBuildOrderButton));
	delete loadBuildOrderButton;
	loadBuildOrderButton = new UI_Button(*(object.loadBuildOrderButton));
	delete boMenu;
	boMenu = new BoMenu(*(object.boMenu));
	saveBox = NULL;
//	fixed = object.fixed;
	return(*this);
}

BoWindow::BoWindow(UI_Object* bo_parent, const unsigned int game_number, const unsigned int max_games, const unsigned int player_number, const unsigned int max_players) :
	UI_Window(bo_parent, BOWINDOW_TITLE_STRING, theme.lookUpPlayerRect(BUILD_ORDER_WINDOW, game_number, max_games, player_number, max_players), theme.lookUpPlayerMaxHeight(BUILD_ORDER_WINDOW, game_number, max_games, player_number, max_players), SCROLLED, AUTO_SIZE_ADJUST, NOT_TABBED, Rect(0, 30, 1000, 1000)),
	anarace(NULL),
	moveTarget(),
	optimizeMode(0),
	addUnit(-1),
	wasMovedByMouse(false),
	boInsertPoint(-1),
	boEndPoint(-1),
	boGoalListOpened(0),
	lastBogoal(0),
	startLine(0),
	selectedItem(0),
	restartBuildOrderButton(new UI_Button(this, Rect(Point(10,15), Size(0,0)), Size(5,5), REFRESH_BUTTON, true, PRESS_BUTTON_MODE, NULL_STRING, ARRANGE_TOP_LEFT)),
	saveBuildOrderButton(new UI_Button(this, Rect(Point(10,15), Size(0,0)), Size(5,5), SAVE_BUTTON, true, STATIC_BUTTON_MODE, NULL_STRING, ARRANGE_TOP_LEFT)),
	loadBuildOrderButton(new UI_Button(this, Rect(Point(10,15), Size(0,0)), Size(5,5), LOAD_BUTTON, true, STATIC_BUTTON_MODE, NULL_STRING, ARRANGE_TOP_LEFT)),

	alwaysBuildWorker(new UI_CheckButton(this, Rect(5, 40, getWidth()-20, 15), Size(5, 5), ARRANGE_LEFT, SETTING_ALWAYS_BUILD_WORKER_STRING, SETTING_ALWAYS_BUILD_WORKER_TOOLTIP_STRING, false)), // TODO
	onlySwapOrders(new UI_CheckButton(this, Rect(5, 40, getWidth()-20, 15), Size(5, 5), ARRANGE_LEFT, SETTING_ONLY_SWAP_ORDERS_STRING, SETTING_ONLY_SWAP_ORDERS_TOOLTIP_STRING, false)), // TODO
//	unitMenuButton(new UI_Button(this, Rect(Point(0, 0), Size(50,0)), Size(0,0), MY_BUTTON, true, STATIC_BUTTON_MODE, ADD_GOAL_STRING, DO_NOT_ADJUST, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
//	unitMenu(new UnitMenu(unitMenuButton, Rect(100, 0, 100, 0), Size(0,0), DO_NOT_ADJUST)),
	boMenu(new BoMenu(this, Rect(10, 40, 100, 0), Size(0,0), DO_NOT_ADJUST)),
	saveBox(NULL)
//	fixed(fixed_list)
{
	for(unsigned int i = MAX_TIME/60;i--;)
		timeSlot[i]=NULL;
	boMenu->Hide();
	resetData();
	restartBuildOrderButton->updateToolTip(RESET_BUILD_ORDER_TOOLTIP_STRING);
	saveBuildOrderButton->updateToolTip(SAVE_BUILD_ORDER_TOOLTIP_STRING);
	loadBuildOrderButton->updateToolTip(LOAD_BUILD_ORDER_TOOLTIP_STRING);
	//	unitMenu->Hide();
//	unitMenuButton->Hide();
//	addHelpButton(DESCRIPTION_BUILD_ORDER_WINDOW_CHAPTER);
	
}

BoWindow::~BoWindow()
{
	std::list<BoEntry*>::iterator i = boList.begin();
	while(i!=boList.end())
	{
		delete(*i);
		i = boList.erase(i);
	}
	delete restartBuildOrderButton;
	delete saveBuildOrderButton;
	delete loadBuildOrderButton;
	delete alwaysBuildWorker;
	delete onlySwapOrders;
//	delete unitMenuButton;
//	delete unitMenu;
	delete saveBox;
	delete boMenu;
}

void BoWindow::reloadOriginalSize()
{
	UI_Window::reloadOriginalSize();
	
	alwaysBuildWorker->setOriginalSize(Size(getWidth()-20, 0));
	onlySwapOrders->setOriginalSize(Size(getWidth()-20, 0));
//	unitMenuButton->setOriginalSize(Size(theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), 0));
}

void BoWindow::assignAnarace(ANABUILDORDER* bo_anarace)
{
	anarace = bo_anarace;
//	unitMenu->assignAnarace(bo_anarace);
	boMenu->assignAnarace(bo_anarace);
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
		delete(*entry);
		entry = boList.erase(entry);
	}
									
//	for(i=0;i<MAX_LENGTH;++i)
//		selection[i]=1;
	Rect edge = Rect(getRelativeClientRectPosition()+Point(4, 3*(FONT_SIZE+6)), Size(getWidth()-20, FONT_SIZE+5));
	fitItemToRelativeClientRect(edge, 1);
	processList();

	recheckSomeDataAfterChange();
}

void BoWindow::reloadStrings()
{
	processList();
	for(std::list<BoEntry*>::iterator entry = boList.begin(); entry!=boList.end(); ++entry)
		(*entry)->reloadStrings();
	
	UI_Window::reloadStrings();
}

void BoWindow::closeMenus()
{
//	unitMenu->close();
	boMenu->close();
}

#include <sstream>
// eigene processList machen falls anarace sich sicher nicht veraendert hat (optimieren = aus)
void BoWindow::processList()
{
//	if(!isShown())
//		return;
	if(anarace==NULL) return;
	setNeedRedrawNotMoved();
	
	unsigned int last_time = 0;

	std::list<PROGRAM> temp_program_list = anarace->getProgramList();

	// TODO irgendwie nur erlauben wenn programlist == bolist ist... also nicht waehrend dem optimieren

	if((UI_Button::getCurrentButton()!=NULL)&&(UI_Button::isMoveByMouse()))
	{
		Rect current_position_rect = UI_Button::getCurrentButton()->getRelativeRect();
		std::list<PROGRAM>::iterator order_start_point = anarace->getProgramList().begin();
		std::list<BoEntry*>::iterator entry_start_point = boList.begin();
		std::list<PROGRAM>::iterator order_target_point = anarace->getProgramList().begin();
		std::list<BoEntry*>::iterator entry_target_point = boList.begin();
	
		UI_Button::getCurrentButton()->resetGradient();
	
		bool found_one = false;
		bool move_forward = false;
		{
		std::list<PROGRAM>::iterator order = anarace->getProgramList().begin();
		std::list<BoEntry*>::iterator entry = boList.begin(); 
	
		for(unsigned int row = startLine+1;
			entry!=boList.end(); 
			 ++entry, ++order, ++row)
		{
			if(UI_Button::getCurrentButton() == (*entry))
			{
				order_start_point = order;
				entry_start_point = entry;
				if(!found_one)
					move_forward = true;
			}
			else if(!found_one)
			{
				Rect edge = (*entry)->getRelativeRect();
				if((current_position_rect.GetBottom() >= edge.GetTop()) && (current_position_rect.GetTop() <= edge.GetBottom() ))
				{
					order_target_point = order;
					entry_target_point = entry;
					found_one = true;
				}
			} 
		}
		}
		
		if(found_one)
		{
			std::list<PROGRAM>::iterator next_order = order_start_point; 
			std::list<PROGRAM>::iterator previous_order_point = order_target_point; 
			if(move_forward)
			{
				next_order++;
				previous_order_point--;
			} else
			{
				next_order--;
				previous_order_point++;
			}
			if((next_order->getUnit() != order_start_point->getUnit())||(previous_order_point->getUnit() != order_start_point->getUnit()))
			{
				anarace->getProgramList().insert(order_target_point, *order_start_point);
				anarace->getProgramList().erase(order_start_point);
				boList.insert(entry_target_point, *entry_start_point);
				boList.erase(entry_start_point);
			
			}
		} 
/*		else  // insert at the back
		{
			std::list<PROGRAM>::iterator next_order = order_start_point; 
			next_order++;
			
			unsigned int unit1 = next_order->getUnit();
			unsigned int unit2 = order_start_point->getUnit();
			unsigned int unit3 = anarace->getProgramList().back().getUnit();				
				
			if((unit1 != unit2) || (unit3 != unit2))
			{
				boList.push_back(*entry_start_point);
				boList.erase(entry_start_point);
				anarace->getProgramList().push_back(*order_start_point);
				anarace->getProgramList().erase(order_start_point);
			}
		
		}*/
	}

	
//	Rect r2 = unitMenuButton->getRelativeRect();
//	r2.SetSize(Size(10, (unitMenu->getHeight()/2+1) * (FONT_SIZE+9)));

	
	std::list<BoEntry*>::iterator entry = boList.begin();
	unsigned int row = startLine+1;
	unsigned int number = 0;
	moveTarget = Rect(0, 0, 0, 0);
	for(std::list<PROGRAM>::iterator order = anarace->getProgramList().begin(); order != anarace->getProgramList().end(); ++order, ++number)
	{
		Rect edge = Rect(getRelativeClientRectPosition()+Point(4, row*(FONT_SIZE+6)), Size(getWidth()-20, FONT_SIZE+5));// TODO

		if((UI_Button::getCurrentButton()!=NULL)&&(UI_Button::isMoveByMouse()))
		{
			Rect r = UI_Button::getCurrentButton()->getRelativeRect();
			if((r.GetBottom() >= edge.GetTop()) && (r.GetTop() <= edge.GetBottom()))
			{
				row++;
				moveTarget = Rect(edge.GetTopLeft() + ((UI_Object*)getScrollBar())->getAbsolutePosition(), edge.GetSize());
			}
		}

/*		if((unitMenuButton->isShown())&&(r2.GetTop()<=edge.GetBottom()))
		{
			if(addUnit>=0)
			{
				PROGRAM o;
				o.setUnit(addUnit);
				order = anarace->getProgramList().insert(order, o);
				
				BoEntry* t = new BoEntry((UI_Object*)getScrollBar(), Point(UI_Object::max_x, getRelativeClientRectPosition().y+205), Size(5,5),
				UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+order->getUnit()+UNIT_NULL_STRING)), *order); // (*anarace->getStartCondition())->getRace()?
				entry = boList.insert(entry, t);
				addUnit=-1;
				unitMenu->Hide();
			} else
			
			while(r2.GetBottom() >= edge.GetTop())
			{
				row++;
				edge = Rect(getRelativeClientRectPosition()+Point(4, row*(FONT_SIZE+6)), Size(getWidth()-20, FONT_SIZE+5));//(*order)->rect.GetSize());// TODO
			}
			
		}*/


		if((order->getRealTime()/60) > (last_time/60))
		{
			++row;
			// TODO: nicht dauernd neu erstellen sondern gleiche suchen!
			for(unsigned int i = (last_time/60)+1;i < (order->getRealTime()/60);i++)
			{
				delete timeSlot[i];
				timeSlot[i]=NULL;
			}
			last_time = (order->getRealTime()/60)*60;
			std::ostringstream os;
			os << " ----- " << formatTime(last_time) << " ----- ";
			if(timeSlot[last_time/60]==NULL)
				timeSlot[last_time/60] = new UI_StaticText((UI_Object*)getScrollBar(), os.str(), Rect(Point(7, 5+row*(FONT_SIZE+6)), Size(0,FONT_SIZE+5)), Size(5,5), FORCE_TEXT_COLOR, SMALL_BOLD_FONT, DO_NOT_ADJUST);
			else 
				timeSlot[last_time/60]->updateText(os.str());
			timeSlot[last_time/60]->adjustRelativeRect(edge);
			edge = Rect(getRelativeClientRectPosition()+Point(4, row*(FONT_SIZE+6)), Size(getWidth()-20, FONT_SIZE+5));//(*order)->rect.GetSize());// TODO
		}
		fitItemToRelativeClientRect(edge, 1);
					

		if(entry == boList.end())
		{
			// wenn nicht optimieren -> anaraceliste loeschen TODO
			BoEntry* t = new BoEntry((UI_Object*)getScrollBar(), Point(UI_Object::max_x, getRelativeClientRectPosition().y+205), Size(5,5),
			// max size -y? TODO
				(eString)(UNIT_TYPE_COUNT*anarace->getRace()+order->getUnit()+UNIT_NULL_STRING), *order, number); // (*anarace->getStartCondition())->getRace()?
			t->setAllowMoveByMouse();
			t->setButtonColorsType(eButtonColorsType(UNIT_TYPE_0_BUTTON+stats[(*anarace->getStartCondition())->getRace()][order->getUnit()].unitType));
			t->adjustRelativeRect(edge);
			boList.push_back(t);

		} 
		else 
		if((*entry)->program.getUnit() != order->getUnit())
		{
		//	if(anarace->isOptimizing())
			{
			std::list<BoEntry*>::iterator k = entry;
			while(k != boList.end())
			{
				if((*k)->program.getUnit() == order->getUnit())  //oder direkt adressen vergleichen? mmmh... TODO!!! was wenn zwei getauscht haben? mmmh...
				{
					break;
				}
				++k;
			}
			if(k != boList.end()) // => Found, move the entry
			{
				BoEntry* old = *k;
				entry = boList.insert(entry, old);
				++entry;
				boList.erase(k);
				if((edge != old->getTargetRect())&&((UI_Button::getCurrentButton()!=old)||(!UI_Button::isMoveByMouse())))
				{
					old->adjustRelativeRect(edge);
					old->resetGradient();
				}
				old->setNumber(number);
				old->program = *order;
			} else // => not found, insert a new one
			{
				BoEntry* t = new BoEntry((UI_Object*)getScrollBar(), Point(UI_Object::max_x, getRelativeClientRectPosition().y+205), Size(5,5), (eString)(UNIT_TYPE_COUNT*anarace->getRace()+order->getUnit()+UNIT_NULL_STRING), *order, number); // (*anarace->getStartCondition())->getRace()?
				t->setAllowMoveByMouse();
				t->setButtonColorsType(eButtonColorsType(UNIT_TYPE_0_BUTTON+stats[(*anarace->getStartCondition())->getRace()][order->getUnit()].unitType));
				if((edge != t->getTargetRect())&&((UI_Button::getCurrentButton()!=t)||(!UI_Button::isMoveByMouse())))
					t->adjustRelativeRect(edge);
				entry = boList.insert(entry, t);
				++entry;

			}
			} 
		} else // ok
//		if((*entry)->getUnit() == order->getUnit())
	       	{
			(*entry)->program = *order;
			if(edge != (*entry)->getTargetRect())
			{
				if((*entry)->getAbsoluteRightBound() > getAbsoluteClientRectRightBound())
					setNeedRedrawMoved();
				if((edge != (*entry)->getTargetRect())&&((UI_Button::getCurrentButton()!=(*entry))||(!UI_Button::isMoveByMouse())))
				{
	     				(*entry)->adjustRelativeRect(edge);
					(*entry)->resetGradient();
				}
			}
			(*entry)->setNumber(number);
						
			++entry;
       		}
		++row;
	}

	++row;
	Rect edge = Rect(getRelativeClientRectPosition()+Point(4, row*(FONT_SIZE+6)), Size(getWidth()-20, FONT_SIZE+5));// TODO
	fitItemToRelativeClientRect(edge, 1);

	for(unsigned int i = (last_time/60)+1;i<MAX_TIME/60;i++)
	{
		delete timeSlot[i];
		timeSlot[i]=NULL;
	}
	
	while(entry != boList.end())
	{
		delete(*entry);
		entry = boList.erase(entry);
		
	}
}


void BoWindow::setSelected(const unsigned int selected)
{
	std::list<BoEntry*>::iterator entry = boList.begin();
	while(entry != boList.end())
	{
		if((*entry)->getNumber() == selected)
		{
			(*entry)->resetGradient();
			return;
		}
		++entry;
	}

}

void BoWindow::checkForInfoWindow()
{
	selectedItem = -1;
	std::list<BoEntry*>::iterator entry = boList.begin();
	while(entry != boList.end())
	{
		if((*entry)->isCurrentlyHighlighted())
		{
			selectedItem = (*entry)->getNumber();
			return;
		}
		++entry;
	}
#if 0
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
	#endif
}

void BoWindow::process()
{
	if(!isShown()) 
		return;
	
	unsigned int oldStartLine = 0;
//	if(unitMenu->getHeight()/2 > oldStartLine)
//		oldStartLine = unitMenu->getHeight()/2;
	if(boMenu->getHeight() > oldStartLine)
		oldStartLine = boMenu->getHeight();

	UI_Window::process();
	if(anarace==NULL) return;

	bool reloadList = false;
	// TODO evtl auf move befehl warten

/*	if(isMouseInside())
	{
		if(!unitMenuButton->isShown())
		{
			unitMenuButton->Show();
			unitMenuButton->setPosition(10, mouse.y - getAbsolutePosition().y);
		}
	}
	else unitMenuButton->Hide();

	
	addUnit=-1;
		if(unitMenuButton->isLeftClicked())
		{
			unitMenu->open();
			if(!unitMenu->isOpen())
			{
				unitMenu->close();
			} else
			{
				unitMenuButton->forcePress();
				unitMenu->close();
				unitMenu->open();
			}
		}
		
	if(unitMenu->isShown())
	{
		if(unitMenu->getPressedItem()>=0)
			addUnit = unitMenu->getPressedItem();
	// TODO: Hier nur jeweils Einheiten in das Menue, welche baubar sind!

        if(unitMenu->menuHasChanged())
        {
                if(unitMenu->secondLevel())
                        unitMenuButton->updateText(BACK_STRING);
                else if(unitMenu->isOpen())
                        unitMenuButton->updateText(CLOSE_STRING);
                else
                        unitMenuButton->updateText(ADD_GOAL_STRING);
		reloadList=true;
        }


	}*/
	
	

	if(wasMovedByMouse)
	{
//		setChangedFlag(); ?
		wasMovedByMouse = false;
	}
	
	std::list<BoEntry*>::iterator entry = boList.begin();
	std::list<PROGRAM>::iterator order = anarace->getProgramList().begin(); 

	while(entry != boList.end())
	{
		if((*entry)->getAbsoluteRightBound() > getAbsoluteClientRectRightBound())
			setNeedRedrawMoved();
		
		if((*entry)->isRightClicked())
		{
			delete(*entry);
			entry = boList.erase(entry);
			order = anarace->getProgramList().erase(order);
		} else
		if((UI_Button::getCurrentButton() == (UI_Button*)*entry)&&(UI_Button::isMoveByMouse()))
		{
			(*entry)->setPosition((*entry)->getRelativeLeftBound(), ((*entry)->getRelativePosition() + mouse + UI_Button::getMouseMovePoint() * (-1)).y);
			UI_Button::setMouseMovePoint(mouse);
			reloadList = true;
			wasMovedByMouse = true;
		}
		
		++entry;
		++order;
	}

	if((reloadList)||(startLine!=oldStartLine))
	{
		startLine = oldStartLine;
		processList();
	}

	if(!isMouseInside()) 
	{
		loadBuildOrderButton->forceUnpress();
		closeMenus();
	}

	if(boMenu->getPressedItem()>=0)
	{
		anarace->copyProgramList(database.getBuildOrder(anarace->getRace(), anarace->getGoal(), boMenu->getPressedItem())->getProgramList());
		anarace->setTimer(100);//database.getBuildOrder(anarace->getRace(), anarace->getGoal(), boMenu->getPressedItem())->getTime());
//		processList();
		loadBuildOrderButton->forceUnpress();
		setNeedRedrawMoved();
	}

	if(loadBuildOrderButton->isLeftClicked())
	{
		boMenu->open();
		if(!boMenu->isOpen())
		{
			loadBuildOrderButton->forceUnpress();
			closeMenus();
		} else
		{
			closeMenus();
			boMenu->open();
		}
		
	}
	if(boMenu->isOpen())
	{
		alwaysBuildWorker->Hide();
		onlySwapOrders->Hide();
	} else
	{
		alwaysBuildWorker->Show();
		onlySwapOrders->Show();
	}


/*	if(restartBuildOrderButton->isLeftClicked())
	{
		std::list<BoEntry*>::iterator entry2 = boList.begin();
		while(entry2!=boList.end())
		{
			delete(*entry2);
			entry2 = boList.erase(entry2);
		}*/
/*		std::list<PROGRAM>::iterator order = anarace->getProgramList().begin();
		while(order!=anarace->getProgramList().end())
		{
			order = anarace->getProgramList().erase(order);
		}
		anarace->resetGeneCode();*/
//	}


	
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
	for(int i=0;i<MAX_LENGTH;++i)
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
						for(j=0;(j<UNIT_TYPE_COUNT)&&(l<i-boInsertPoint);++j)
							if(stats[(*anarace->getStartCondition())->getRace()][j].facilityType==boGoalListOpened+1) ++l;
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
				int temp=(controls.getCurrentPosition().y-getClientRectUpperBound())/(FONT_SIZE+5);
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
//				orderList->setMakeSpace(-1);
				boGoalListOpened=0;
				boInsertPoint=-1;
				boEndPoint=-1;
		}
#endif

//	unsigned int line=0;
	boEndPoint=0;
//	unsigned int oldTime=MAX_TIME+1;
	unsigned int tempForceCount[UNIT_TYPE_COUNT];
	memset(tempForceCount, 0, UNIT_TYPE_COUNT * sizeof(int));

// hack for minimum size ~~
//  Rect edge=Rect(getRelativeClientRectPosition()+Point(0,100),Size(180,20));
//fitItemToRelativeClientRect(edge,1);

	checkForInfoWindow();
	
	if(saveBuildOrderButton->isLeftClicked()&&(!saveBox))
		saveBox = new SaveBox(getParent(), SAVE_BUILD_ORDER_AS_STRING, GIVE_BO_A_NAME_STRING);

	if(saveBox)
	{
		if(saveBox->isCanceled())
		{
			delete saveBox;
			saveBox = NULL;
			UI_Object::focus = NULL;
			saveBuildOrderButton->forceUnpress();
		} else
		if(saveBox->isDone())
		{
			if(saveBox->getString().length()>0)
			{
				UI_Object::focus = NULL;
				BUILD_ORDER my_bo_list(anarace->getRace(), *anarace->getGoal(), saveBox->getString(), anarace->getRealTimer(), anarace->getProgramList());
				
				database.saveBuildOrder(saveBox->getString(), my_bo_list);
				boMenu->resetData();
				UI_Object::msgList.push_back("Saved build order.");
				delete saveBox;
				saveBox = NULL;
				saveBuildOrderButton->forceUnpress();
			}
		}
	}
	
	if(anarace->setAlwaysBuildWorkers( alwaysBuildWorker->isChecked() ));
//		setChangedFlag();
	if(anarace->setOnlySwapOrders( onlySwapOrders->isChecked() ));
//		setChangedFlag();
	
	getScrollBar()->checkBoundsOfChildren(getAbsoluteClientRectUpperBound()+35, getAbsoluteClientRectLowerBound());
}

#include <sstream>
void BoWindow::draw(DC* dc) const
{
	if(!isShown()) 
		return;
	UI_Window::draw(dc);


	if((UI_Button::getCurrentButton()!=NULL)&&(UI_Button::isMoveByMouse())&&(moveTarget!=Rect(0,0,0,0)))
	{
		dc->DrawRectangle(moveTarget);
	}

//	int k = 0;
  //      for(std::list<PROGRAM>::const_iterator order = anarace->getProgramList().begin(); order != anarace->getProgramList().end(); ++order)
//	{
//		dc->DrawText(stats[(*anarace->getStartCondition())->getRace()][order->getUnit()].name, getAbsolutePosition() + Size(0, k*10));
//		++k;
//	}
/*	k = 0;
	for(int s=MAX_LENGTH;s--;)
		if(anarace->getProgramIsBuilt(s)&&(anarace->getRealProgramTime(s) + stats[anarace->getRace()][anarace->getPhaenoCode(s)].BT<=anarace->getRealTimer()))
		{
			dc->DrawText(stats[(*anarace->getStartCondition())->getRace()][anarace->getPhaenoCode(s)].name, getAbsolutePosition() + Size(180, k*10));
			++k;
		}	 */

//	std::ostringstream os;
//	os.str("");
//	os << "new: " << new_one << "/ add end: " << add_end << "/ moved: " << moved << "/ same: " << same << "/ deleted: " << deleted;   // << " [" << size1 << "/" << size2 << "]";
//	os << PRERACE::situationHashMap.size();
//	dc->DrawText(os.str(), getAbsoluteClientRectPosition() - Size(0, 8));

	
//	drawSelectionStuff(dc);

//	if(infoWindow->isShown())
//		dc->DrawLine(
	
	/*if((orderList->getMakeSpace()>-1)&&(boGoalListOpened==0))
	{
		dc->SetTextForeground(dc->doColor(0,200,0));
		dc->DrawText(*theme.lookUpString(CLICK_TO_INSERT_ORDER_STRING),getClientRectPosition()+Point(0,orderList->getMakeSpace()*(FONT_SIZE+5)));
	}*/ // TODO

/*	dc->SetPen(*theme.lookUpPen(RECTANGLE_PEN));
	if(boInsertPoint>-1)
	{
		int line=boInsertPoint;
		Rect edge=Rect(getAbsoluteClientRectPosition()+Point(0,line*(FONT_SIZE+5)),Size(270,FONT_SIZE+4));
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
		++line;
		if(boGoalListOpened==1)
		{
			for(int i=1;i<11;++i)
			{
				Rect edge=Rect(getAbsoluteClientRectPosition()+Point(10,line*(FONT_SIZE+5)),Size(270,FONT_SIZE+4));
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
				++line;
			}
		}
		else if(boGoalListOpened>1)
		{
			int type=boGoalListOpened+1;
			for(int i=0;i<UNIT_TYPE_COUNT;++i)
			{
				if(stats[(*anarace->getStartCondition())->getRace()][i].facilityType==type)
				{
					Rect edge=Rect(getAbsoluteClientRectPosition()+Point(20,line*(FONT_SIZE+5)),Size(270,FONT_SIZE+4));
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
					++line;
				}
			}
		}
//...		edge.x=20;
		edge.y=line*(FONT_SIZE+5)+9;
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
//		makeSpaceButton=addButton(Rect(getClientRectPosition()+Point(0,makeSpace*(FONT_SIZE+5)),Size(SECOND_COLOUMN-8,FONT_SIZE+4)));
//	else makeSpaceButton=99999;
}

#if 0
void BoWindow::drawSelectionStuff(DC* dc) const
{
		dc->SetPen(*theme.lookUpPen(SELECT_PEN));
		dc->SetTextForeground(*theme.lookUpColor(WINDOW_TEXT_COLOR));
		dc->SetFont(theme.lookUpFont(SMALL_BOLD_FONT));
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
	for(i=0;i<orderList->getCount();++i)
	{
		if((!inrow)&&(selection[i])&&(i!=boInsertPoint))
		{
			inrow=1;
			start=l;
		} else if((inrow)&&((!selection[i])||(i==boInsertPoint)))
		{
			inrow=0;
			Point pointsKlammer[9];
			int h1=start*(FONT_SIZE+5)+11+getClientRectUpperBound();
			int h2=(l+1)*(FONT_SIZE+5)-2+getClientRectUpperBound();
																				
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
			++l;
			if(i==boInsertPoint)
				l=boEndPoint;
		}
}
*/
}
#endif

void BoWindow::recheckSomeDataAfterChange()
{
	boMenu->resetData();
	if(boMenu->getNumber() > 0)
		loadBuildOrderButton->Show();
	else loadBuildOrderButton->Hide();
	resetMinXY();
	// TODO buttons in eine group und in group so etwas wie ein 'addItem' schreiben
	restartBuildOrderButton->adjustPositionAndSize(ADJUST_ONLY_POSITION);
	saveBuildOrderButton->adjustPositionAndSize(ADJUST_ONLY_POSITION);
	loadBuildOrderButton->adjustPositionAndSize(ADJUST_ONLY_POSITION);
}


const bool BoWindow::wasResetted() const
{
	if(!isShown())
		return(false);
	return(restartBuildOrderButton->isLeftClicked());
}

