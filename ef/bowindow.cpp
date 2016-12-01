#include "bowindow.hpp"
#include "configuration.hpp"
#include "savebox.hpp"
#include <sstream>

BoWindow::BoWindow(UI_Object* bo_parent, const unsigned int game_number, const unsigned int game_max, const unsigned int player_number, const unsigned int player_max) :
	UI_Window(bo_parent, BOWINDOW_TITLE_STRING, theme.lookUpPlayerRect(BUILD_ORDER_WINDOW, game_number, game_max, player_number, player_max), theme.lookUpPlayerMaxHeight(BUILD_ORDER_WINDOW, game_number, game_max, player_number, player_max), SCROLLED, AUTO_SIZE_ADJUST, Rect(0, 30, 1000, 1000)),
	geneAnimation(0),
	anarace(NULL),
//	moveTarget(),
	optimizeMode(0),
	addUnit(-1),
//	wasMovedByMouse(false),
//	boInsertPoint(-1),
//	boEndPoint(-1),
	boGoalListOpened(0),
	lastBogoal(0),
	startLine(0),
	selectedItems(),
	menuGroup(new UI_Group(this, Rect(Point(5, 5), Size(100,0)), Size(0, 5), HORIZONTAL_GROUP, false, TOP_LEFT, NULL_STRING)),
	restartBuildOrderButton(new UI_Button(menuGroup, Rect(Point(0,0), Size(0,0)), Size(5,5), MENU_BUTTON, REFRESH_BITMAP, PRESS_BUTTON_MODE, NULL_STRING, DO_NOT_ADJUST)),
	saveBuildOrderButton(new UI_Button(menuGroup, Rect(Point(0,0), Size(0,0)), Size(5,5), MENU_BUTTON, SAVE_BITMAP, STATIC_BUTTON_MODE, NULL_STRING, DO_NOT_ADJUST)),
	loadBuildOrderButton(new UI_Button(menuGroup, Rect(Point(0,0), Size(0,0)), Size(5,5), MENU_BUTTON, LOAD_BITMAP, STATIC_BUTTON_MODE, NULL_STRING, DO_NOT_ADJUST)),

	// TODO in OptionenMenue
//	alwaysBuildWorker(new UI_Button(this, Rect(5, 40, getWidth()-20, 10), Size(5, 5), CHECK_BUTTON, true, CHECK_BUTTON_MODE, SETTING_ALWAYS_BUILD_WORKERS_STRING, SETTING_ALWAYS_BUILD_WORKERS_TOOLTIP_STRING, SPECIAL_BUTTON_LEFT)),
//	onlySwapOrders(new UI_CheckButton(this, Rect(5, 40, getWidth()-20, 15), Size(5, 5), ARRANGE_LEFT, SETTING_ONLY_SWAP_ORDERS_STRING, SETTING_ONLY_SWAP_ORDERS_TOOLTIP_STRING, false)), // TODO
//	unitMenuButton(new UI_Button(this, Rect(Point(0, 0), Size(50,0)), Size(0,0), MY_BUTTON, true, STATIC_BUTTON_MODE, ADD_GOAL_STRING, DO_NOT_ADJUST, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH)),
//	unitMenu(new UnitMenu(unitMenuButton, Rect(100, 0, 100, 0), Size(0,0), DO_NOT_ADJUST)),
	boMenu(new BoMenu(this, Rect(10, 40, 100, 0), Size(0,0), DO_NOT_ADJUST)),
//	fixed(fixed_list)
	gameNumber(game_number),
	gameMax(game_max),
	playerNumber(player_number),
	playerMax(player_max),
	saveBox(false),
	boMenuOpenedExternally(false),
	loadedBuildOrder(-1)
{
//	for(unsigned int i = 60;i--;)
//		timeSlot[i]=NULL;
	resetData();
	restartBuildOrderButton->updateToolTip(RESET_BUILD_ORDER_TOOLTIP_STRING);
	saveBuildOrderButton->updateToolTip(SAVE_BUILD_ORDER_TOOLTIP_STRING);
	loadBuildOrderButton->updateToolTip(LOAD_BUILD_ORDER_TOOLTIP_STRING);
//	unitMenuButton->Hide();
	
	addHelpButton(DESCRIPTION_BUILD_ORDER_WINDOW_CHAPTER);
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
	delete menuGroup;
//	delete alwaysBuildWorker;
//	delete onlySwapOrders;
//	delete unitMenuButton;
//	delete unitMenu;
	delete boMenu;
}

const bool BoWindow::saveBuildOrder()
{
	needSaveBox();
	return(true);
}

void BoWindow::setMode(const unsigned int game_number, const unsigned int game_max, const unsigned int player_number, const unsigned int player_max)
{
	if((game_number == gameNumber) && (game_max == gameMax) && (player_number == playerNumber) && (player_max == playerMax))
		return;
	gameNumber = game_number;
	gameMax = game_max;
	playerNumber = player_number;
	playerMax = player_max;
}


void BoWindow::reloadOriginalSize()
{
	setOriginalRect(UI_Object::theme.lookUpPlayerRect(BUILD_ORDER_WINDOW, gameNumber, gameMax, playerNumber, playerMax));
	setMaxHeight(UI_Object::theme.lookUpPlayerMaxHeight(BUILD_ORDER_WINDOW, gameNumber, gameMax, playerNumber, playerMax));
	UI_Window::reloadOriginalSize();
	
//	alwaysBuildWorker->setOriginalSize(Size(getWidth()-20, 10));
//	onlySwapOrders->setOriginalSize(Size(getWidth()-20, 0));
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
//	boInsertPoint=-1;
//	boEndPoint=-1;
	boGoalListOpened=0;
	optimizeMode=0;
	
	std::list<BoEntry*>::iterator entry = boList.begin();
	while(entry != boList.end())
	{
		delete(*entry);
		entry = boList.erase(entry);
	}
	processList();
	recheckSomeDataAfterChange();
}

/*
void BoWindow::reloadStrings()
{
	processList();
	for(std::list<BoEntry*>::iterator entry = boList.begin(); entry!=boList.end(); ++entry)
		(*entry)->reloadStrings();
	
	UI_Window::reloadStrings();
}*/

void BoWindow::closeMenus()
{
//	unitMenu->close();
	boMenu->close();
}

const bool BoWindow::areBosStillMoving() const 
{
	for(std::list<BoEntry*>::const_iterator entry = boList.begin(); entry != boList.end(); ++entry)
		if((*entry)->isMoving())
			return(true);
	return(false);
}


// eigene processList machen falls anarace sich sicher nicht veraendert hat (optimieren = aus)
void BoWindow::processList()
{
	if(anarace==NULL) return;
	makePufferInvalid();
	
	bool new_item = false;
	bool deleted_item = false;
//	unsigned int last_time = 0;

/*	std::list<PROGRAM> temp_program_list = anarace->getProgramList();

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
				if((current_position_rect.getBottom() >= edge.getTop()) && (current_position_rect.getTop() <= edge.getBottom() ))
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
			if((next_my_unit != order_start_point->getUnit())||(previous_order_point->getUnit() != order_start_point->getUnit()))
			{
				anarace->getProgramList().insert(order_target_point, *order_start_point);
				anarace->getProgramList().erase(order_start_point);
				boList.insert(entry_target_point, *entry_start_point);
				boList.erase(entry_start_point);
			
			}
		} 
//		else  // insert at the back
//		{
//			std::list<PROGRAM>::iterator next_order = order_start_point; 
//			next_order++;
//			
//			unsigned int unit1 = next_my_unit;
//			unsigned int unit2 = order_start_point->getUnit();
//			unsigned int unit3 = anarace->getProgramList().back().getUnit();				
//				
//			if((unit1 != unit2) || (unit3 != unit2))
//			{
//				boList.push_back(*entry_start_point);
//				boList.erase(entry_start_point);
//				anarace->getProgramList().push_back(*order_start_point);
//				anarace->getProgramList().erase(order_start_point);
//			}
//		
//		}
	}*/

	
//	Rect r2 = unitMenuButton->getRelativeRect();
//	r2.setSize(Size(10, (unitMenu->getHeight()/2+1) * (FONT_SIZE+9)));

	
	std::list<BoEntry*>::iterator entry = boList.begin();
	unsigned int row = startLine;
	unsigned int id = 0;
	for(std::list<PROGRAM>::const_iterator order = anarace->getProgramList().begin(); order != anarace->getProgramList().end(); ++order)
	if(anarace->getTimer() + order->getBT() <= order->getTime())
	{
		int count = 1;
		std::list<PROGRAM>::const_iterator old_order = order;
		unsigned int my_unit = order->getUnit();
		unsigned int my_race = anarace->getRace();

		if(efConfiguration.isCompactDisplayMode())
		{
			std::list<PROGRAM>::const_iterator previous_order = order;
			order++;			
			while((order != anarace->getProgramList().end())&&(my_unit == order->getUnit()))
			{
				previous_order = order;
				order++;
				count++;
			}
			order = previous_order;
		}

		std::ostringstream os;
		{
			os << "$" << formatTime(old_order->getRealTime(), efConfiguration.getGameSpeed()) << " - " << formatTime(order->getRealTime() + order->getBT(), efConfiguration.getGameSpeed()) << "$: " << GAME::lookUpUnitString(my_race, my_unit) << "#" <<
		"  " << UI_Object::theme.lookUpFormattedString(BOWINDOW_BUILD_TIME_STRING, GAME::race[my_race].stats[my_unit].BT/(efConfiguration.getGameSpeed()*3+6)) << "#";
			for(unsigned int i = RACE::MAX_RESOURCE_TYPES; i--;)
				if(GAME::race[my_race].stats[my_unit].resource[i] > 0)
				{
					os << "  $" << GAME::race[my_race].stats[my_unit].resource[i]/100 << "$ [ currently " << order->before.getHaveResource(i)/100 << "] " << GAME::lookUpGameString(GAME::FIRST_RESOURCE_STRING + i);
					if(count > 1)
						os << " " << UI_Object::theme.lookUpFormattedString(BOWINDOW_EACH_TOTAL_STRING, GAME::race[my_race].stats[my_unit].resource[i] * count/100);
			}
			os << "##Current building queue:#";
			std::priority_queue<Building, std::vector<Building> > building_queue = order->buildingQueue;
			while(!building_queue.empty())
			{
				if(building_queue.top().getBuildFinishedTime() >= anarace->getTimer())
					os << "$" << formatTime(order->getTime() - building_queue.top().getBuildFinishedTime(), efConfiguration.getGameSpeed()) << "/" <<formatTime(building_queue.top().getTotalBuildTime(), efConfiguration.getGameSpeed()) << "$: " << GAME::lookUpUnitString(my_race, building_queue.top().getType()) << "#";
				building_queue.pop();
			}
		}
		
		Rect edge = Rect(Point(2 + getRelativeClientRectLeftBound(), row*(FONT_SIZE+6)), Size(getWidth()-20, FONT_SIZE+5));// TODO

/*		if((UI_Button::getCurrentButton()!=NULL)&&(UI_Button::isMoveByMouse()))
		{
			Rect r = UI_Button::getCurrentButton()->getRelativeRect();
			if((r.getBottom() >= edge.getTop()) && (r.getTop() <= edge.getBottom()))
			{
				row++;
				moveTarget = Rect(edge.getTopLeft() + ((UI_Object*)getScrollBar())->getAbsolutePosition(), edge.getSize());
			}
		}*/

/*		if((unitMenuButton->isShown())&&(r2.getTop()<=edge.getBottom()))
		{
			if(addUnit>=0)
			{
				PROGRAM o;
				o.setUnit(addUnit);
				order = anarace->getProgramList().insert(order, o);
				
				BoEntry* t = new BoEntry((UI_Object*)getScrollBar(), Point(getParent()->getWidth(), getRelativeClientRectPosition().y+205), Size(5,5),
				UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*my_race+my_unit)), *order); // (*anarace->getStartCondition())->getRace()?
				entry = boList.insert(entry, t);
				addUnit=-1;
				unitMenu->Hide();
			} else
			
			while(r2.getBottom() >= edge.getTop())
			{
				row++;
				edge = Rect(getRelativeClientRectPosition()+Point(4, row*(FONT_SIZE+6)), Size(getWidth()-20, FONT_SIZE+5));//(*order)->rect.getSize());// TODO
			}
			
		}*/

//		toErrorLog("check time entries");
		// TODO manchmal kanns hier zu Problemen kommen :/ TODO Absturz... + falsche Position...
/*		if((old_order->getRealTime()/60) > (last_time/60))
		{
			++row;
			// TODO: nicht dauernd neu erstellen sondern gleiche suchen!
			for(unsigned int i = (last_time/60)+1;i < (old_order->getRealTime()/60);i++)
			{
				delete timeSlot[i];
				timeSlot[i]=NULL;
			}
			last_time = ((order->getRealTime() + order->getBT())/60)*60;
			std::ostringstream os;
			os << " ----- " << formatTime(last_time) << " ----- ";
			if(timeSlot[last_time/60]==NULL)
				timeSlot[last_time/60] = new UI_StaticText((UI_Object*)getScrollBar(), os.str(), Rect(Point(7, 5+row*(FONT_SIZE+6)), Size(0,FONT_SIZE+5)), Size(5,5), FORCE_TEXT_COLOR, SMALL_BOLD_FONT, DO_NOT_ADJUST);
			else 
				timeSlot[last_time/60]->updateText(os.str());
			timeSlot[last_time/60]->adjustRelativeRect(edge);
			edge = Rect(getRelativeClientRectPosition()+Point(2, row*(FONT_SIZE+6)), Size(getWidth()-20, FONT_SIZE+5));//(*order)->rect.getSize());// TODO
		}*/
//		fitItemToRelativeClientRect(edge, true);
					
//		toErrorLog("check this bo");
		if(entry == boList.end())
		{
			signed int x_pos = getParent()->getWidth();
			if((gameMax>1)&&(gameNumber==0)) x_pos = - getWidth() - getParent()->getWidth()/2;
			// wenn nicht optimieren -> anaraceliste loeschen TODO
			BoEntry* t = new BoEntry((UI_Object*)getScrollBar(), Point(x_pos, 205), Size(5,5),
			// max size -y? TODO
				GAME::lookUpUnitString(my_race, my_unit), *order, count, id);
			new_item = true;
			t->setAllowMoveByMouse();
			t->setButtonColorsType(eButtonColorsType(UNIT_TYPE_0_BUTTON + GAME::race[my_race].stats[my_unit].unitType));
			t->adjustRelativeRect(edge);
			t->updateToolTip(os.str());
			boList.push_back(t);
		} 
		else 
		if(((*entry)->program.getUnit() != my_unit)||((*entry)->getCount() != count))
		{
			std::list<BoEntry*>::iterator k = entry;
			while(k != boList.end())
			{
				if(((*k)->program.getUnit() == my_unit)&&((*k)->getCount() == count))  //oder direkt adressen vergleichen? mmmh... TODO!!! was wenn zwei getauscht haben? mmmh...
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
					new_item = true;
					old->resetGradient();
				} else
					deleted_item = true;
				old->updateToolTip(os.str());
				old->setId(id);
				old->program = *order;
			} else // => not found, insert a new one
			{
				signed int x_pos = getParent()->getWidth()/2;
				if((gameMax>1)&&(gameNumber==0)) x_pos = - getWidth() - getParent()->getWidth()/2;
				BoEntry* t = new BoEntry((UI_Object*)getScrollBar(), Point(x_pos, 205), Size(5,5), GAME::lookUpUnitString(my_race, my_unit), *order, count, id);
				new_item = true;
				t->setAllowMoveByMouse();
				t->setButtonColorsType(eButtonColorsType(UNIT_TYPE_0_BUTTON + GAME::race[my_race].stats[my_unit].unitType));
				if((edge != t->getTargetRect()) && ((UI_Button::getCurrentButton()!=t) || (!UI_Button::isMoveByMouse())))
				{
					t->adjustRelativeRect(edge);
					new_item = true;
				}
				t->updateToolTip(os.str());
				entry = boList.insert(entry, t);
				++entry;
			} 
		} else // ok
//		if((*entry)->getUnit() == my_unit)
	       	{
			(*entry)->program = *order;
			if(edge != (*entry)->getTargetRect())
			{
				if((edge != (*entry)->getTargetRect())&&((UI_Button::getCurrentButton()!=(*entry))||(!UI_Button::isMoveByMouse())))
				{
	     				(*entry)->adjustRelativeRect(edge);
					(*entry)->resetGradient();
				}
			}
			(*entry)->setId(id);
			(*entry)->updateToolTip(os.str());
						
			++entry;
       		}
		++row;
		id += count;
	}// end for
	fitItemToRelativeClientRect(Rect(getRelativeClientRectPosition() + Point(2, row*(FONT_SIZE+6)), Size(1,1)), true);//getWidth()-20, FONT_SIZE+5)), true);

/*	for(unsigned int i = (last_time/60)+1;i<MAX_TIME/60;i++)
	{
		delete timeSlot[i];
		timeSlot[i]=NULL;
	} TODO*/
	
	while(entry != boList.end())
	{
		delete(*entry);
		entry = boList.erase(entry);
		deleted_item = true;
	}
	if(new_item)
		UI_Object::sound.playSound(SWISHIN_SOUND, (getAbsolutePosition() + getSize()/2).x);
	if(deleted_item)
		UI_Object::sound.playSound(SWISHOUT_SOUND, (getAbsolutePosition() + getSize()/2).x);
}

void BoWindow::setSelected(const std::list<unsigned int>& selected)
{
	std::list<BoEntry*>::iterator entry = boList.begin();
	while(entry != boList.end())
	{
		for(std::list<unsigned int>::const_iterator j = selected.begin(); j != selected.end(); ++j)
			if(((*entry)->getId() <= *j)&&((*entry)->getCount() + (*entry)->getId() - 1 >= *j ))
			{
				(*entry)->resetGradient();
//				return;
			}
		++entry;
	}

}

const bool BoWindow::openBoMenu()
{
	if(boMenu->isOpen())
	{
		loadBuildOrderButton->forceUnpress();
		boMenu->close();
		boMenuOpenedExternally = false;
		UI_Object::focus = NULL;
		return(false);
	} else 
	{
		loadBuildOrderButton->forcePress();
		boMenuOpenedExternally = true;
		UI_Object::focus = boMenu;
		return(true);
	}
}

void BoWindow::checkForInfoWindow()
{
	selectedItems.clear();
	std::list<BoEntry*>::iterator entry = boList.begin();
	while(entry != boList.end())
	{
		if((*entry)->isCurrentlyHighlighted())
		{
			for(unsigned int i = 0; i < (*entry)->getCount(); ++i)
				selectedItems.push_back((*entry)->getId()+i);
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

void BoWindow::mouseHasLeft()
{
	if(!boMenuOpenedExternally)
	{
		if(loadBuildOrderButton->isCurrentlyActivated())
		{
			loadBuildOrderButton->forceUnpress();	
			closeMenus();
		}
	}
}
void BoWindow::process()
{
	if((!isShown()) || (anarace==NULL))
		return;

	if(anarace->isOptimizing())
		geneAnimation+=0.43;
	
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
		if(pressed = etc. unitMenu->getPressedItem()>=0)
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
	
	

/*	if(wasMovedByMouse)
	{
//		setChangedFlag(); ?
		wasMovedByMouse = false;
	}*/
	
	std::list<BoEntry*>::iterator entry = boList.begin();
//	std::list<PROGRAM>::iterator order = anarace->getProgramList().begin(); 

	while(entry != boList.end())
	{
/*		if((*entry)->isRightClicked())
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
		}*/
		
		++entry;
//		++order;
	}

	if((reloadList) || (startLine != oldStartLine) || (getChangedFlag()))
	{

//		todo... 
		startLine = oldStartLine;
		processList();
	}

	if(!isMouseInside()) 
		mouseHasLeft();

	loadedBuildOrder = -1;
	if((loadedBuildOrder = boMenu->getPressedItem())>=0)
	{
		loadBuildOrderButton->forceUnpress();
	}

	if(loadBuildOrderButton->isLeftClicked())
	{
		boMenu->open();
		if(!boMenu->isOpen())
		{
			loadBuildOrderButton->forceUnpress();
			closeMenus();
			boMenuOpenedExternally = false;
		} else
		{
			closeMenus();
			boMenu->open();
		}
	}
	if(boMenu->isOpen())
	{
//		alwaysBuildWorker->Hide();
//		onlySwapOrders->Hide();
	} else
	{
//		alwaysBuildWorker->Show();
//		onlySwapOrders->Show();
	}


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
						for(j=0;(j<LAST_UNIT)&&(l<i-boInsertPoint);++j)
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
//	boEndPoint=0;
//	unsigned int oldTime=MAX_TIME+1;
	std::vector<unsigned int> tempForceCount(RACE::UNIT_TYPE_COUNT);
	tempForceCount.assign(RACE::UNIT_TYPE_COUNT, 0);

	checkForInfoWindow();
// TODO savebox von guimain regeln lassen, bowindow (und forcewindow) soll nur eine bool und eine string variable setzen und auf einen Aufruf ('isDone' bzw. 'isCanceled') von oben warten
	if(saveBuildOrderButton->isLeftClicked())
		needSaveBox();
	else
	if(saveBox)
	{
		if(UI_Window::saveBoxCanceled)
		{
			saveBoxIsCanceled();
			saveBox = false;
			UI_Window::saveBoxCanceled = false;
		}
		else if(UI_Window::saveBoxDone)
		{
			saveBoxIsDone(UI_Window::saveBoxString);
			UI_Window::saveBoxString = "";
			saveBox = false;
			UI_Window::saveBoxDone = false;
		}
	}

//	if(anarace->setAlwaysBuildWorkers( alwaysBuildWorker->isCurrentlyActivated() ));
//		setChangedFlag();
//	if(anarace->setOnlySwapOrders( onlySwapOrders->isCurrentlyActivated() ));
//		setChangedFlag();
	
	if((efConfiguration.isDnaSpiral()) && (!boMenu->isOpen()))
		setRelativeClientRectPosition(Point(5, menuGroup->getRelativeLowerBound() + 2 + 2*(FONT_SIZE+8)));
	else
		setRelativeClientRectPosition(Point(5, menuGroup->getRelativeLowerBound()));
	getScrollBar()->checkBoundsOfChildren(getAbsoluteClientRectUpperBound(), getAbsoluteClientRectLowerBound());
}

void BoWindow::drawGene(unsigned int k, const Point* points, const Point position, Pen& bla1, Pen& bla2) const
{
	if(points[0].y<points[1].y) dc->setPen(bla1);else dc->setPen(bla2);
	dc->DrawSpline(k, points, position);
	dc->DrawSpline(k, points, position - Size(0,1));
	dc->DrawSpline(k, points, position + Size(0,1));
}

void BoWindow::drawGeneString() const
{
	unsigned int orderCount = anarace->getProgramList().size();
	if(orderCount<2)
		return;

	Rect position = Rect(Point(5, menuGroup->getHeight() + 1), Size(getClientRectWidth()-5, 2*(FONT_SIZE+8)));
//		Rect(Point(210, 200), Size(256, 128));
	dc->setBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
	dc->setPen(*theme.lookUpPen(NULL_PEN));
	dc->DrawRectangle(Rect(position.getTopLeft() - Size(1,2), position.getSize() + Size(2,4)));
//	DC::addRectangle(Rect(position.getTopLeft() - Size(1,2), position.getSize() + Size(2,4))); TODO

	unsigned int stringheight=0;
	Point points1[2];
	Point points2[2];
	Point points3[2];
	Point points4[2];
	std::vector<unsigned int> colors;
	for(std::list<PROGRAM>::const_iterator i = anarace->getProgramList().begin();i!=anarace->getProgramList().end();++i)
		colors.push_back(i->getUnit());

	unsigned int current_type = GAME::race[anarace->getRace()].stats[colors[0]].facilityType;
	for(unsigned int i=0;i<2*orderCount;++i)
	{
		current_type = GAME::race[anarace->getRace()].stats[colors[i/2]].facilityType;
		for(unsigned int j=0;j<2;++j)
		{
			points1[j] = Point(5+(stringheight+j)*(position.getWidth()-8)/(orderCount*2)-1, (int)((cos((float)(4*(stringheight+j)+geneAnimation)*5.0*M_PI/200.0)*0.9*position.getHeight()/2)+position.getHeight()/2.1));
				
			points2[j] = Point(5+(stringheight+j)*(position.getWidth()-8)/(orderCount*2)-1, (int)((sin(((float)(4*(stringheight+j)+geneAnimation)+13.0)*5.0*M_PI/200.0)*0.9*position.getHeight()/2)+position.getHeight()/2.1));
			
			points3[j] = Point(5+(stringheight+j)*(position.getWidth()-8)/(orderCount*2)-1, (int)((cos(((float)(4*(stringheight+j)+geneAnimation)+26.0)*5.0*M_PI/200.0)*0.9*position.getHeight()/2)+position.getHeight()/2.1));
					
			points4[j] = Point(5+(stringheight+j)*(position.getWidth()-8)/(orderCount*2)-1, (int)((sin(((float)(4*(stringheight+j)+geneAnimation)+39.0)*5.0*M_PI/200.0)*0.9*position.getHeight()/2)+position.getHeight()/2.1));
		}
		stringheight+=1;

		Pen bla1 = Pen(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN + GAME::race[anarace->getRace()].stats[colors[i/2]].unitType)));
		Pen bla2 = Pen(*theme.lookUpPen((ePen)(UNIT_TYPE_0_PEN + GAME::race[anarace->getRace()].stats[colors[i/2]].unitType)));
		unsigned int k=2;
		if(points1[0].y>points1[1].y) // faellt -> hinten
		{
			if(points1[0].y>points2[0].y)
			{
				drawGene(k, points1, position.getTopLeft(), bla1, bla2);
				drawGene(k, points2, position.getTopLeft(), bla1, bla2);
			}
			else
			{
				drawGene(k, points2, position.getTopLeft(), bla1, bla2);
				drawGene(k, points1, position.getTopLeft(), bla1, bla2);
			}
			if(points3[0].y>points4[0].y)
			{
				drawGene(k, points3, position.getTopLeft(), bla1, bla2);
				drawGene(k, points4, position.getTopLeft(), bla1, bla2);
			} else
			{
				drawGene(k, points4, position.getTopLeft(), bla1, bla2);
				drawGene(k, points3, position.getTopLeft(), bla1, bla2);
			}
		} else
		{
			if(points3[0].y>points4[0].y)
			{
				drawGene(k, points3, position.getTopLeft(), bla1, bla2);
				drawGene(k, points4, position.getTopLeft(), bla1, bla2);
			} else
			{
				drawGene(k, points4, position.getTopLeft(), bla1, bla2);
				drawGene(k, points3, position.getTopLeft(), bla1, bla2);
			}
			if(points1[0].y>points2[0].y)
			{
				drawGene(k, points1, position.getTopLeft(), bla1, bla2);
				drawGene(k, points2, position.getTopLeft(), bla1, bla2);
			}
			else
			{
				drawGene(k, points2, position.getTopLeft(), bla1, bla2);
				drawGene(k, points1, position.getTopLeft(), bla1, bla2);
			}
		}
	}
}
																			


void BoWindow::draw() const
{
	UI_Window::draw();
	if((efConfiguration.isDnaSpiral())&&(!boMenu->isOpen()))
		drawGeneString();

/*	if((UI_Button::getCurrentButton()!=NULL)&&(UI_Button::isMoveByMouse())&&(moveTarget!=Rect(0,0,0,0)))
	{
		dc->DrawRectangle(moveTarget);
	}*/

//	int k = 0;
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
		dc->setTextForeground(dc->doColor(0,200,0));
		dc->DrawText(*theme.lookUpString(CLICK_TO_INSERT_ORDER_STRING),getClientRectPosition()+Point(0,orderList->getMakeSpace()*(FONT_SIZE+5)));
	}*/ // TODO

/*	dc->setPen(*theme.lookUpPen(RECTANGLE_PEN));
	if(boInsertPoint>-1)
	{
		int line=boInsertPoint;
		Rect edge=Rect(getAbsoluteClientRectPosition()+Point(0,line*(FONT_SIZE+5)),Size(270,FONT_SIZE+4));
		if(fitItemToAbsoluteClientRect(edge,true))
		{
			int bright=0;
					if(edge.Inside(controls.getCurrentPosition()))
				bright=50;
			//if(t%2==0)
				dc->setBrush(Brush(dc->brightenColor(theme.lookUpBrush(FORCE_LIST_BUTTON_BRUSH)->getColor(), bright), theme.lookUpBrush(FORCE_LIST_BUTTON_BRUSH)->getStyle()));
			//else
			//dc->setBrush(Brush(dc->doColor(COLOR2R,COLOR2G,COLOR2B),SOLID));
			dc->DrawRoundedRectangle(edge,4);
//			brightenColor(FORCE_ADD_GOAL_TEXT_COLOR,bright); TODO
//			dc->setTextForeground(dc->doColor(0+bright,200+bright,0+bright)); //TODO
			if(boGoalListOpened==1)
			{
				dc->DrawBitmap(*theme.lookUpBitmap(ARROW_UP_BITMAP), edge.getPosition()+Point(4,2));
				dc->DrawText(*theme.lookUpString(CLICK_TO_CLOSE_STRING), edge.getPosition()+Point(14,0));
			}
			else
			{
				dc->DrawBitmap(*theme.lookUpBitmap(ARROW_UP_BITMAP), edge.getPosition()+Point(4,2));
				dc->DrawText(*theme.lookUpString(CLICK_TO_GO_BACK_STRING),edge.getPosition()+Point(4,2));
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
				lastBogoal=edge.getBottom();
				if(fitItemToAbsoluteClientRect(edge,true))
				{
					dc->setBrush(Brush(dc->brightenColor(theme.lookUpBrush((eBrush)(BRIGHT_UNIT_TYPE_1_BRUSH+i))->getColor(), 100+bright), theme.lookUpBrush((eBrush)(BRIGHT_UNIT_TYPE_1_BRUSH+i))->getStyle()));
					dc->DrawRoundedRectangle(edge,4);
					dc->setTextForeground(dc->brightenColor(theme.lookUpColor(TEXT_COLOR),bright));
					dc->DrawText(*theme.lookUpString((eString)(UNIT_TYPE_1_STRING+i)),edge.getPosition()+Point(10,0));
//					unitButton[line]=addButton(edge); TODO
				}
				++line;
			}
		}
		else if(boGoalListOpened>1)
		{
			int type=boGoalListOpened+1;
			for(int i=0;i<LAST_UNIT;++i)
			{
				if(stats[(*anarace->getStartCondition())->getRace()][i].facilityType==type)
				{
					Rect edge=Rect(getAbsoluteClientRectPosition()+Point(20,line*(FONT_SIZE+5)),Size(270,FONT_SIZE+4));
					int bright=0;
					if(edge.Inside(controls.getCurrentPosition()))
						bright=50;

					lastBogoal=edge.y;
					if(fitItemToAbsoluteClientRect(edge, true))
					{
						dc->setBrush(Brush(dc->brightenColor(theme.lookUpBrush((eBrush)(BRIGHT_UNIT_TYPE_1_BRUSH+type))->getColor(), 100+bright), theme.lookUpBrush((eBrush)(BRIGHT_UNIT_TYPE_1_BRUSH+type))->getStyle()));
		 				dc->DrawRoundedRectangle(edge,4);
						dc->setTextForeground(*theme.lookUpColor(TEXT_COLOR));
						dc->DrawText(stats[(*anarace->getStartCondition())->getRace()][i].name, edge.getPosition()+Point(10,0));
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
			dc->setPen(Pen(dc->doColor(200,0,0),2,SOLID));
			dc->DrawLine(edge.x+getClientRectLeftBound(),edge.y+getClientRectUpperBound(),edge.x+edge.width+getClientRectLeftBound(),edge.y+getClientRectUpperBound());
			dc->setPen(Pen(dc->doColor(0,0,0),1,SOLID));
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
		dc->setPen(*theme.lookUpPen(SELECT_PEN));
		dc->setTextForeground(*theme.lookUpColor(WINDOW_TEXT_COLOR));
		dc->setFont(theme.lookUpFont(SMALL_BOLD_FONT));
		dc->DrawText(*theme.lookUpString(OPTIMIZE_EVERYTHING_STRING), Point(0,-32)+getAbsoluteClientRectPosition());
		dc->DrawText(*theme.lookUpString(OPTIMIZE_SELECTED_STRING), getAbsoluteClientRectPosition()+Point(0,-18));
		dc->setBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
		Rect edge=Rect(getAbsoluteClientRectPosition()+Point(getClientRectWidth()-32,-32),Size(12,12));
		Rect edge2=Rect(getAbsoluteClientRectPosition()+Point(getClientRectWidth()-32,-18),Size(12,12));
																				
		dc->DrawRoundedRectangle(edge,3);
		dc->DrawRoundedRectangle(edge2,3);
																				
		dc->setTextForeground(*theme.lookUpColor(FORCE_TEXT_COLOR));
//		if(!optimizeMode) dc->DrawCheckMark(edge);
//		else
//		if(optimizeMode)
//		{
//			dc->DrawBitmap(*theme.lookUpBitmap(ARROW_DOWN_BITMAP), getClientRectLeftBound()+getWidth()-46,getClientRectUpperBound()-4);
//			dc->DrawCheckMark(edge2);
//		}
																				
/*	  int i;
//	  boanzahl=orderList->getCount();
																				
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
				dc->setPen(Pen(dc->doColor(50,200,50),1,SOLID));
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
	geneAnimation=0;

	// TODO buttons in eine group und in group so etwas wie ein 'addItem' schreiben
//	restartBuildOrderButton->adjustPositionAndSize(ADJUST_ONLY_POSITION);
//	saveBuildOrderButton->adjustPositionAndSize(ADJUST_ONLY_POSITION);
//	loadBuildOrderButton->adjustPositionAndSize(ADJUST_ONLY_POSITION);
}


const bool BoWindow::wasResetted() const
{
	if(!isShown())
		return(false);
	return(restartBuildOrderButton->isLeftClicked());
}


void BoWindow::needSaveBox()
{
	UI_Window::needSaveBox = true;
	UI_Window::saveBoxParameter.saveboxText = SAVE_BUILD_ORDER_AS_STRING;
	UI_Window::saveBoxParameter.descriptionText = GIVE_BO_A_NAME_STRING;
	UI_Window::saveBoxParameter.okString = SAVE_BOX_OK_STRING;
	UI_Window::saveBoxParameter.cancelString = SAVE_BOX_CANCEL_STRING;
	UI_Window::saveBoxParameter.inputProposal = "";
	saveBox = true;
}

void BoWindow::saveBoxIsCanceled()
{
	saveBuildOrderButton->forceUnpress();
	makePufferInvalid();
}

void BoWindow::saveBoxIsDone(std::string input_string)
{
	BO_HEADER bo_header;
	bo_header.setRace(anarace->getRace());
	bo_header.setName(input_string);
	bo_header.setTime(anarace->getRealTimer());
	BUILD_ORDER my_bo_list(bo_header, *anarace->getGoal(),  anarace->getProgramList());
				
//	database.saveBuildOrder(input_string, my_bo_list, anarace); ?
	boMenu->resetData();
	UI_Object::msgList.push_back(UI_Object::theme.lookUpFormattedString(SAVED_BUILD_ORDER_STRING, input_string));
// TODO datawindow benachrichtigen!	
	saveBuildOrderButton->forceUnpress();
	makePufferInvalid(); // ?
}



