#include "force.hpp"

ForceEntry::ForceEntry(UI_Object* entry_parent, const Rect entry_rect, const Rect entry_max_rect, const string& entry_unit):
	UI_Button(entry_parent, entry_rect, entry_max_rect, entry_unit, entry_unit, FORCE_ENTRY_BUTTON, NO_TEXT_MODE, PRESS_BUTTON_MODE, DO_NOT_ADJUST, SMALL_NORMAL_BOLD_FONT, NO_AUTO_SIZE),
	startForce(20),
	targetForce(20),
	currentForce(20),
	type(WORKER_UNIT_TYPE),
	total(1),
	goal(1)
{
}

ForceEntry::~ForceEntry()
{
}

void ForceEntry::process()
{
	if(!isShown())
		return;
	Size::mv(currentForce, startForce, targetForce);
	UI_Button::process();
}

const eUnitType ForceEntry::getType() const
{
#ifdef _SCC_DEBUG
    if(type>=UNIT_TYPE_TYPES) {
        toLog("WARNING: (ForceEntry::getType): Variable not initialized.");return(UNIT_TYPE_TYPES);
    }
#endif
	return(type);
}

void ForceEntry::setTargetForce(const unsigned int force)
{
	if(targetForce!=force)
	{
		startForce=currentForce;
//		if(targetForce<20)
//			targetForce=20;
		//else 
		targetForce=force;
	}
}

void ForceEntry::draw(DC* dc) const
{
	if(!isShown()) return;
	UI_Button::draw(dc);
	//TODO: Was ist mit Einheiten die zwar als force da sind, aber nicht buildable ( = kein Eintrag in phaenoToGenoType) sind?
	dc->SetBrush(*theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+getType())));
	dc->SetPen(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN+getType())));
					
	//									  if(isShown()==1)
	dc->DrawRoundedRectangle(Rect(getAbsolutePosition()+Point(getWidth()-currentForce, 0), Size(currentForce+1,FONT_SIZE+5)), 3);
	//									  else if(isShown()==2)
	//		dc->DrawRoundedRectangle(Rect(edge.GetPosition() + Point(edge.GetWidth() - oldForce[i], 0), Size(oldForce[i] + 1, FONT_SIZE + 4)), 4);
	if (total < goal)
		dc->SetTextForeground(dc->mixColor(theme.lookUpColor(BRIGHT_TEXT_COLOR), theme.lookUpColor(FULFILLED_TEXT_COLOR)));
	else
		dc->SetTextForeground(dc->mixColor(theme.lookUpColor(BRIGHT_TEXT_COLOR), theme.lookUpColor(NOT_FULFILLED_TEXT_COLOR)));
	
	ostringstream os;
	if(total <= goal)
		os << total << "/" << goal;
	else
		os << "+" << total - goal << "    " << total << "/" << goal;
	Size s = dc->GetTextExtent(os.str());
	dc->DrawText(os.str(), getAbsolutePosition() + Point(getWidth() - s.GetWidth() - 2, 2));
//} // end oldForceList >0
}

void ForceEntry::HideIt()
{
	/*addUnit->frameReset();
	subUnit->frameReset();
	cancelUnit->frameReset();*/
	this->Hide();
}

void ForceEntry::updateText(const string& utext)
{
	updateNormalText(utext);
	updatePressedText(utext);
}

void ForceEntry::setType(const eUnitType type)
{
#ifdef _SCC_DEBUG
	if((type<0)&&(type>=UNIT_TYPE_TYPES)) {
        toLog("WARNING: (ForceEntry::setType): Value out of range.");return;
    }
#endif
	this->type=type;
}

void ForceEntry::setTotal(const unsigned int total)
{
	this->total=total;
}

void ForceEntry::setGoal(const unsigned int goal)
{
	this->goal=goal;
}

ForceWindow::ForceWindow(UI_Object* force_parent, ANARACE* force_anarace, MessageWindow* force_msg_window, const unsigned int force_window_number) :
	UI_Window(force_parent, FORCE_WINDOW_TITLE_STRING, FORCE_WINDOW, force_window_number, SCROLLED),
	msgWindow(force_msg_window),
	changed(false),
	menuRadio(new UI_Radio(this)),
	nongoals(new UI_StaticText(this, NON_GOALS_STRING, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), NO_TEXT_MODE, FORCE_TEXT_COLOR)),
	goals(new UI_StaticText(this, GOALS_STRING, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), NO_TEXT_MODE, FORCE_TEXT_COLOR)),
	legend(new UI_StaticText(this, LEGEND_STRING, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR)),
	markedUnit(0),
	oldMarkedUnit(0),
	anarace(force_anarace),
	currentUnitType(0),
	totalUnits(1),
	unitMenu(new UnitMenu(this, anarace, Rect(10, 10, getWidth()-10, 0))),
	goalMenu(new GoalMenu(this, anarace, Rect(10, 10, getWidth()-100, 0))),
	forceMenu(new ForceMenu(this, anarace, Rect(10, 10, getWidth()-200, 0))),
	raceMenu(new RaceMenu(this, anarace, Rect(10, 10, getWidth()-200, 0))) // TODO
{	
	resetData();

	for(int i=GAS_SCV+1;i--;)
	{
		forceEntry[i]=new ForceEntry(this, Rect(getRelativeClientRectPosition()+Point(0,0), Size(getClientRectWidth(), FONT_SIZE+5)), 
		Rect(getRelativeClientRectPosition(), getClientRectSize()+Size(0, getMaxRect().GetHeight())),  // max size -y?
						stats[anarace->getRace()][i].name);
		forceEntry[i]->Hide();
	}

	menuButton[RACE_MENU] = new UI_Button(this, Rect(getRelativeClientRectPosition()+Point(0,10), getClientRectSize()), Rect(Point(0,0),getSize()), CHOOSE_RACE_STRING, CHOOSE_RACE_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
	menuButton[UNIT_MENU] = new UI_Button(this, Rect(getRelativeClientRectPosition()+Point(0,10), getClientRectSize()), Rect(Point(0,0),getSize()), ADD_GOAL_STRING, ADD_GOAL_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
	menuButton[GOAL_MENU] = new UI_Button(this, Rect(getRelativeClientRectPosition()+Point(0,10), getClientRectSize()), Rect(Point(0,0),getSize()), GOAL_LIST_STRING, GOAL_LIST_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
	menuButton[FORCE_MENU] = new UI_Button(this, Rect(getRelativeClientRectPosition()+Point(0,10), getClientRectSize()), Rect(Point(0,0),getSize()), STARTFORCE_STRING, STARTFORCE_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);

	menuButton[RACE_MENU]->updateToolTip(*theme.lookUpString(CHOOSE_RACE_TOOLTIP_STRING));
	menuButton[UNIT_MENU]->updateToolTip(*theme.lookUpString(ADD_GOALS_TOOLTIP_STRING));
	menuButton[GOAL_MENU]->updateToolTip(*theme.lookUpString(CHOOSE_GOALS_TOOLTIP_STRING));
	menuButton[FORCE_MENU]->updateToolTip(*theme.lookUpString(CHOOSE_STARTING_FORCE_TOOLTIP_STRING));
	

	raceMenu->Hide();
	unitMenu->Hide();
	goalMenu->Hide();
	forceMenu->Hide();

	for(int i = 0; i<MAX_MENUS; i++)	
		menuRadio->addButton(menuButton[i]);
	
	for(int i = UNIT_TYPE_TYPES;i--;)
		unitTypeCount[i]=0;
	
/*	addUnit = new UI_Button(this, Rect(getRelativeClientRectPosition() + Point(getClientRectWidth()+5,2),Size(8,8)), Rect(Point(0,0), getSize()), ADD_BUTTON, PRESS_BUTTON_MODE);
	subUnit = new UI_Button(this, Rect(getRelativeClientRectPosition() + Point(getClientRectWidth()+15,2),Size(8,8)), Rect(Point(0,0), getSize()), SUB_BUTTON, PRESS_BUTTON_MODE);
	cancelUnit = new UI_Button(this, Rect(getRelativeClientRectPosition() + Point(getClientRectWidth()+25,2),Size(8,8)), Rect(Point(0,0), getSize()), CANCEL_BUTTON, PRESS_BUTTON_MODE);
	addUnit->updateToolTip(*theme.lookUpString(ADD_UNIT_TOOLTIP_STRING));
	subUnit->updateToolTip(*theme.lookUpString(REMOVE_UNIT_TOOLTIP_STRING));
	cancelUnit->updateToolTip(*theme.lookUpString(REMOVE_GOAL_TOOLTIP_STRING));*/
	
}

ForceWindow::~ForceWindow()
{
	for(int i=GAS_SCV+1;i--;)
		delete forceEntry[i];

	delete nongoals;
	delete goals;
	delete legend;

	for(int i=MAX_MENUS;i--;)
		delete menuButton[i];
	
	delete unitMenu;
	delete goalMenu;
	delete forceMenu;
	delete raceMenu; 
	
	delete menuRadio;
}

void ForceWindow::closeMenus()
{
	unitMenu->close();
	goalMenu->close();
	forceMenu->close();
	raceMenu->close();
}

void ForceWindow::setMarkedUnit(const unsigned int markedUnit)
{
	this->markedUnit=markedUnit;
}
	
const unsigned int ForceWindow::getMarkedUnit() const
{
	return (markedUnit);
}

void ForceWindow::process()
{
	if(!isShown())
		return;
	UI_Window::process();
	totalUnits=0;
	for(int i = UNIT_TYPE_TYPES;i--;)
		unitTypeCount[i]=0;

	currentUnitType = 0;
	oldMarkedUnit = markedUnit;
	markedUnit = 0;



	if(unitMenu->getPressedItem()>0)
	{
		(*anarace->getCurrentGoal())->addGoal(unitMenu->getPressedItem(), 1, 0, 0);
		ostringstream os;
		os << "Added " << stats[anarace->getRace()][unitMenu->getPressedItem()].name << " to the goal list.";
		msgWindow->addMessage(os.str());
		setChangedFlag();
	}

	if(goalMenu->getPressedItem()>=0)
	{
		settings.assignGoal(anarace->getPlayerNum(), goalMenu->getPressedItem());
		menuRadio->forceUnpressAll();
		ostringstream os;
		os << "Set " << (*anarace->getCurrentGoal())->getName() << " as new goal list."; // TODO
		msgWindow->addMessage(os.str());
		setChangedFlag();
		menuButton[GOAL_MENU]->forceUnpress();
	}
	
	if(forceMenu->getPressedItem()>=0)
	{
		settings.assignStartcondition(anarace->getPlayerNum(), forceMenu->getPressedItem());
		menuRadio->forceUnpressAll();
		settings.fillGroups();
		setChangedFlag();
		menuButton[FORCE_MENU]->forceUnpress();
	}

	eRace racepressed = (eRace)(raceMenu->getPressedItem());
	if(racepressed >= 0)
	{
		settings.assignStartRace(anarace->getPlayerNum(), racepressed);
//		ostringstream os;
//		os << "Set " << theme.lookUp(*anarace->getCurrentGoal())->getName() << " as new goal list."; // TODO
//		msgWindow->addMessage(os.str()); TODO
	
		settings.assignStartcondition(anarace->getPlayerNum(), 0);
		settings.fillGroups();
		settings.assignGoal(anarace->getPlayerNum(), 0);
		setChangedFlag();
    	menuButton[RACE_MENU]->forceUnpress();

		unitMenu->resetData();
		/// TODO hier wird mehrfach adjustgoals aufgerufen!!
	}

	if(menuRadio->hasChanged())
	{
		switch(menuRadio->getMarked())
		{
			// TODO in ne eigene Klasse oder so
			case RACE_MENU:
			{
				raceMenu->Show();
                raceMenu->open();
               	if(!raceMenu->isOpen()) {
		            menuRadio->forceUnpressAll();
                    closeMenus();
                } else {
                    closeMenus();
                    raceMenu->open();
                }
			}
			break;
		
			case UNIT_MENU:
			{
			// unit menu has a menu depth of 2!
			// thus a repeated press on the menu item will not cause the item to flip back
				bool maybePushAgain=false;
//				if(unitMenu->isOpen()) 
//					maybePushAgain=true;
				unitMenu->Show();
                unitMenu->open();
                if(!unitMenu->isOpen()) {
                    menuRadio->forceUnpressAll();
                    closeMenus();
                } else {
//					if(maybePushAgain)
//						menuRadio->forcePress(UNIT_MENU);
                    closeMenus();
                    unitMenu->open();
                }
			}
            break;
			
			case GOAL_MENU:
			{
					goalMenu->Show();
					goalMenu->open();
				if(!goalMenu->isOpen()) {
					menuRadio->forceUnpressAll();
					closeMenus();
				} else {
					closeMenus();
					goalMenu->open();
				}
			}
			break;
			
			case FORCE_MENU:
			{
				forceMenu->Show();
				forceMenu->open();
				if(!forceMenu->isOpen()) {
					menuRadio->forceUnpressAll();
					closeMenus();
				} else {
					closeMenus();
					forceMenu->open();
				}
			}
			break;
			default:break;
		}
	}

	if(getChangedFlag())
	{
		resetData();
//		unitMenu->resetData(); CHANGED
	}
		
	
	if(!isMouseInside()) {
		menuRadio->forceUnpressAll();
		closeMenus();
	}
	
//	if((addUnit->isCurrentlyHighlighted()||subUnit->isCurrentlyHighlighted()||cancelUnit->isCurrentlyHighlighted()))
//	{
//		forceEntry[markedUnit]->forceHighlighted();
//		while(true);
//	}
	
// ------------------- GOALS
	unsigned int startLine = 3;
	if(raceMenu->getHeight() > startLine)
		startLine = raceMenu->getHeight();
	if(unitMenu->getHeight()/2 > startLine)
		startLine = unitMenu->getHeight()/2;
	if(goalMenu->getHeight() > startLine)
		startLine = goalMenu->getHeight();
	if(forceMenu->getHeight() > startLine)
		startLine = forceMenu->getHeight();
	unsigned int line = 1;
	
	anarace->countUnitsTotal();

	goals->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5)));
	legend->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5)));
	line++;
// all units above GAS_SCV are pretty uninteresting (mostly temporary variables)
	for (int i = 0; i <= GAS_SCV; i++)
		if((*anarace->getCurrentGoal())->getAllGoal(i) > 0)
		{
			Rect edge = Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5));

// NEW
			if(anarace->getLocationTotal(GLOBAL, i) > (*anarace->getCurrentGoal())->getAllGoal(i))
				forceEntry[i]->setTargetForce(getClientRectWidth() / 3);
			else
				forceEntry[i]->setTargetForce(anarace->getLocationTotal(GLOBAL, i) * getClientRectWidth() / (3*(*anarace->getCurrentGoal())->getAllGoal(i)));
//			forceEntry[i]->setTargetForce(anarace->getLocationTotal(GLOBAL, i) * getClientRectWidth() / (3*anarace->getUnitsTotalMax()));
	
			totalUnits+=anarace->getLocationTotal(GLOBAL, i);
			unitTypeCount[stats[anarace->getRace()][i].unitType]+=anarace->getLocationTotal(GLOBAL, i);
		// TODO 2. Modus machen um Produktionsgebaeude ueber 'facility' zu bestimmen (fuer hotkeys geradezu praedestiniert)
			forceEntry[i]->setType(stats[anarace->getRace()][i].unitType);
			forceEntry[i]->setTotal(anarace->getLocationTotal(GLOBAL, i));
			forceEntry[i]->setGoal((*anarace->getCurrentGoal())->getAllGoal(i));
// TODO wenn Settings geaendert werden... geht total hinueber...
//		if (fitItemToRelativeClientRect(edge, 1))
			{
				forceEntry[i]->Show();
				
/*				if( ((i==markedUnit)&&(!forceEntry[i]->isCurrentlyHighlighted())) ||((markedUnit!=i)&&(forceEntry[i]->isCurrentlyHighlighted())))
				{
					forceEntry[i]->forceHighlighted();
					markedUnit = i;
					edge.SetSize(edge.GetSize() - Size(35,0));
					addUnit->Show();
					subUnit->Show();
					cancelUnit->Show();
				}
				if((oldMarkedUnit != markedUnit)&&(forceEntry[i]->isCurrentlyHighlighted()))
				{
					addUnit->setPosition(Point(getClientRectWidth()+5,edge.y+2));
					addUnit->adjustRelativeRect(Rect(Point(getRelativeClientRectLeftBound() + getClientRectWidth()-30,edge.y+2),Size(8,8)));
					subUnit->setPosition(Point(getClientRectWidth()+15,edge.y+2));
					subUnit->adjustRelativeRect(Rect(Point(getRelativeClientRectLeftBound() + getClientRectWidth()-20,edge.y+2),Size(8,8)));
					cancelUnit->setPosition(Point(getClientRectWidth()+25,edge.y+2));
					cancelUnit->adjustRelativeRect(Rect(Point(getRelativeClientRectLeftBound() + getClientRectWidth()-10,edge.y+2),Size(8,8)));
			
					// adjustRect, setPosition
				}*/
				
				forceEntry[i]->updateText("   " + stats[anarace->getRace()][i].name);
				forceEntry[i]->setFreeMove();
				forceEntry[i]->adjustRelativeRect(edge);
	
				if(forceEntry[i]->isLeftClicked())
				{
        			ostringstream os;
					(*anarace->getCurrentGoal())->addGoal(i, 1, 0, 0);
        			os << "Added one " << stats[anarace->getRace()][i].name << " to the goal list.";         
					msgWindow->addMessage(os.str());
		
				   setChangedFlag();
				}//<- Bei (+) kann sich nichts an den goals veraendern! trotzdem changed, da sich die maximale Fitness veraendert hat
				if(forceEntry[i]->isRightClicked())
				{
        			ostringstream os;
					(*anarace->getCurrentGoal())->addGoal(i, -1, 0, 0);
                    os << "Removed one " << stats[anarace->getRace()][i].name << " from the goal list.";
                    msgWindow->addMessage(os.str());
					
					setChangedFlag();		   // <- Bei (-) sehr wohl
				}
//					case 3:(*anarace->getCurrentGoal())->addGoal(i,-(*anarace->getCurrentGoal())->getGlobalGoal(0, i), 0, 0);
//							setChangedFlag();break;		   // <- und auch bei X natuerlich


			}
/*		   	else
			{
				forceEntry[i]->Hide();
				forceEntry[i]->adjustRelativeRect(Rect(getRelativeClientRectPosition()+Point(-100,200), Size(getClientRectWidth(),FONT_SIZE+10)));
			}*/
			line++;
		} // goal > 0

	nongoals->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5)));
//  ---------- NON GOALS
	line++;
	for (unsigned int i = 0; i <= GAS_SCV; i++)
		if (((*anarace->getCurrentGoal())->getAllGoal(i) == 0) && ((*anarace->getCurrentGoal())->getIsBuildable(i)))
		{
			Rect edge = Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5));

			forceEntry[i]->setTargetForce(anarace->getLocationTotal(GLOBAL, i) * getClientRectWidth() / (3*anarace->getUnitsTotalMax()));

			totalUnits+=anarace->getLocationTotal(GLOBAL, i);
			unitTypeCount[stats[anarace->getRace()][i].unitType]+=anarace->getLocationTotal(GLOBAL, i);
			
			forceEntry[i]->setType(stats[anarace->getRace()][i].unitType);
			forceEntry[i]->setTotal(anarace->getLocationTotal(GLOBAL, i));
			
			forceEntry[i]->setGoal((*anarace->getCurrentGoal())->getAllGoal(i));

		//	if (fitItemToRelativeClientRect(edge, 1))
			{
				forceEntry[i]->Show();
	/*			if( ((i==markedUnit)&&(!forceEntry[i]->isCurrentlyHighlighted())) ||((markedUnit!=i)&&(forceEntry[i]->isCurrentlyHighlighted())))
				{
					forceEntry[i]->forceHighlighted();
					markedUnit = i;
					edge.SetSize(edge.GetSize() - Size(35,0));
					addUnit->Show();
					subUnit->Show();
					cancelUnit->Show();
				}
				if((oldMarkedUnit != markedUnit)&&(forceEntry[i]->isCurrentlyHighlighted()))
				{
                   addUnit->setPosition(Point(getClientRectWidth()+5,edge.y+2));
                    addUnit->adjustRelativeRect(Rect(Point(getRelativeClientRectLeftBound() + getClientRectWidth()-30,edge.y+2),Size(8,8)));
					
					subUnit->setPosition(Point(getClientRectWidth()+15,edge.y+2));
    				subUnit->adjustRelativeRect(Rect(Point(getRelativeClientRectLeftBound() + getClientRectWidth()-20,edge.y+2),Size(8,8)));
					cancelUnit->setPosition(Point(getClientRectWidth()+25,edge.y+2));
    				cancelUnit->adjustRelativeRect(Rect(Point(getRelativeClientRectLeftBound() + getClientRectWidth()-10,edge.y+2),Size(8,8)));
				
					// adjustRect, setPosition
				}*/
			
				ostringstream os;
				//os << anarace->getLocationTotal(0, i) << " " << stats[anarace->getRace()][i].name;
				//if (anarace->getLocationTotal(0, i) == 1);
				//else os << "s";
				//forceEntry[i]->updateText(os.str());
				forceEntry[i]->updateText("   " + stats[anarace->getRace()][i].name);
				forceEntry[i]->setFreeMove();
				forceEntry[i]->adjustRelativeRect(edge);

				if(forceEntry[i]->isLeftClicked())
				{
					(*anarace->getCurrentGoal())->addGoal(i, 1, 0, 0);
				    setChangedFlag();
                    ostringstream os;
                    os << "Moved non-goal " << stats[anarace->getRace()][i].name << " to the goal list.";
                    msgWindow->addMessage(os.str());
				}//<- Bei (+) kann sich nichts an den goals veraendern! trotzdem changed, da sich die maximale Fitness veraendert hat
				/*if(forceEntry[i]->isRightClicked())
				{
					(*anarace->getCurrentGoal())->addGoal(i, -1, 0, 0);
					setChangedFlag();		   // <- Bei (-) sehr wohl
					
				}*/ // TODO geht doch garnicht!? ist ja schon nongoal...

				
			fitItemToRelativeClientRect(edge, 1);
		}
		 /*   else
			{
				forceEntry[i]->Hide();
				forceEntry[i]->adjustRelativeRect(Rect(getRelativeClientRectPosition()+Point(-100,200), Size(getClientRectWidth(),FONT_SIZE+10)));
			}*/
			line++;
		} // goal > 0
		else if((*anarace->getCurrentGoal())->getAllGoal(i) == 0)
		{
			forceEntry[i]->HideIt();
			forceEntry[i]->adjustRelativeRect(Rect(getRelativeClientRectPosition()+Point(0,0), Size(getClientRectWidth(),FONT_SIZE+10)));
		}
		Rect edge=Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5));
		fitItemToRelativeClientRect(edge, 1);

/*	line++;
	for (int i = 0; i <= GAS_SCV; i++)
		if (((*anarace->getCurrentGoal())->allGoal[i] == 0) && (anarace->getLocationTotal(0, i)))
		{
			Rect edge = Rect(getClientRectPosition() + Point(0,
															  line * (FONT_SIZE + 5) + 3 - getScrollY()),
								 Size(270, FONT_SIZE + 5));
			if (oldForceList[i] < anarace->getLocationTotal(0, i) * 100 / anarace->getUnitsTotalMax())
				oldForceList[i] += (anarace->getLocationTotal(0, i) * 100 / anarace->getUnitsTotalMax() - oldForceList[i]) / 5 + 1;
			else if (oldForceList[i] > anarace->getLocationTotal(0, i) * 100 / anarace->getUnitsTotalMax())
				oldForceList[i] -= (oldForceList[i] - anarace->getLocationTotal(0, i) * 100 / anarace->getUnitsTotalMax()) / 5 + 1;
			if (fitItemToClientRect(edge, 1))
			{
				if (line % 2 == 0)
					dc->SetBrush(*theme.lookUpBrush(BO_DARK_BRUSH));
				else
					dc->SetBrush(*theme.lookUpBrush(BO_BRIGHT_BRUSH));

				dc->DrawRoundedRectangle(edge, 4);
				if (anarace->getLocationTotal(0, i) == 1)
					dc->DrawText(_T
								 (string::
								  Format(T("%2i %s"), anarace->getLocationTotal(0, i),
										 stats[anarace->getRace()][i].name)), edge.GetPosition());
				else
					dc->DrawText(_T
								 (string::
								  Format(T("%2i %ss"), anarace->getLocationTotal(0, i),
										 stats[anarace->getRace()][i].name)), edge.GetPosition());
//							  dc->SetBrush(Brush(Color(   ( (anarace->getProgramCode(order->IP)+1)*155/(1+(*anarace->getCurrentGoal())->getMaxBuildTypes()/2))%156,
//											  ((anarace->getProgramCode(order->IP)+1)*155/(1+(*anarace->getCurrentGoal())->getMaxBuildTypes()/4))%156,
//											  ((anarace->getProgramCode(order->IP)+1)*155/(1+(*anarace->getCurrentGoal())->getMaxBuildTypes()/8))%156),SOLID));

				dc->SetBrush(theme.lookUpMixedBrush(BO_DARK_BRUSH, (eBrush)(UNIT_TYPE_1_BRUSH+stats[anarace->getRace()][i].facilityType)));

				dc->DrawRoundedRectangle(Rect
										 (edge.GetPosition() + Point(edge.width - 1 - oldForceList[i], 0),
										  Size(oldForceList[i] + 1, FONT_SIZE + 4)), 4);

				if ((isShown() == 1) && (edge.Inside(controls.getCurrentPosition())))
				{
					dc->DrawText(*theme.lookUpString(ADD_AS_GOAL_STRING), edge.GetPosition() + Point(edge.width - 100, 0));	//TODO
				}
			}
			line++;
		}
//		setMaxScrollY(t*(FONT_SIZE+5));*/

	for(int i=UNIT_TYPE_TYPES;i--;)
	{
		unsigned int d = unitTypeCount[i] * 360 / totalUnits;
		if(d!=targetUnitTypeCount[i])
		{
			startUnitTypeCount[i]=currentUnitTypeCount[i];
			targetUnitTypeCount[i]=d;
		}		
		Size::mv(currentUnitTypeCount[i], startUnitTypeCount[i], targetUnitTypeCount[i]);
	}

	/*	
	const int ForceEntry::changed() const
{
	if(addUnit->isClicked()) return(1);
	if(subUnit->isClicked()) return(2);
	if(cancelUnit->isClicked()) return(3);
	return(0);
}

				
				switch(forceEntry[i]->changed())
				{
					case 0:break;
					case 1:(*anarace->getCurrentGoal())->addGoal(i, 1, 0, 0);
						   setChangedFlag();break;  //<- Bei (+) kann sich nichts an den goals veraendern! trotzdem changed, da sich die maximale Fitness veraendert hat
					case 2:(*anarace->getCurrentGoal())->addGoal(i, -1, 0, 0);
							setChangedFlag();break;		   // <- Bei (-) sehr wohl
					case 3:(*anarace->getCurrentGoal())->addGoal(i,-(*anarace->getCurrentGoal())->getGlobalGoal(0, i), 0, 0);
							setChangedFlag();break;		   // <- und auch bei X natuerlich
				}

*/
		
}

void ForceWindow::resetData()
{
	currentUnitType = 0;
/*	for (int i = GAS_SCV+1; i--;)	// muss global in der Klasse sein und nicht lokal in drawGoalList, weil die Balken vergroessern/verkleinern sich ja 
	{
		if ((!anarace) || ((*anarace->getCurrentGoal())->allGoal[i] == 0))
			forceEntry[i]->oldForce = 0;
		else if ((anarace) && ((*anarace->getCurrentGoal())->allGoal[i]))
			forceEntry[i]->oldForce = anarace->getLocationTotal(0, i) * 75 / anarace->getUnitsTotalMax();
	}*/ //TODO
	markedUnit = 0;
}

void ForceWindow::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Window::draw(dc);

//	  int percent[UNIT_TYPE_COUNT];
//	  int counter[UNIT_TYPE_COUNT]; //number of different goals
//	  for(int i=0;i<UNIT_TYPE_COUNT;i++) {percent[i]=0;counter[i]=0;}
/*
 * for(int i=0;i<MAX_GOALS;i++) // ???? WTF??
 * * {
 * * percent[(*anarace->getCurrentGoal())->goal[i].unit]+=anarace->fitnessCode[i];
 * * counter[(*anarace->getCurrentGoal())->goal[i].unit]++;
 * * } 
 */

// goallist
/*	int worstTime[UNIT_TYPE_COUNT];
	for (int i = UNIT_TYPE_COUNT; i--;)
		worstTime[i] = 0;

	for(list<GOAL>::const_iterator i = (*anarace->getCurrentGoal())->goal.begin(); i!=(*anarace->getCurrentGoal())->goal.end();++i)
		if (i->count >0)
			if (worstTime[i->unit] < i->finalTime)
				worstTime[i->unit] = i->finalTime;*/ // ???
	dc->SetFont(theme.lookUpFont(SMALL_NORMAL_BOLD_FONT));
	dc->SetPen(*theme.lookUpPen(RECTANGLE_PEN));
	unsigned int line = 1;
//	int t = 0;

//dc->SetTextForeground(*theme.lookUpColor(FORCE_TEXT_COLOR));
//	dc->DrawText(*theme.lookUpString(STARTING_FORCE_STRING) , getAbsoluteClientRectPosition());

	Rect edge;
/*
 * if((percent[i]/counter[i]<100)&&(percent[i]/counter[i]>0))
 * * {
 * * //			 dc->DrawText(_T(string::Format(T("[%i]"),anarace->getLocationTotal(0,i))),edge.GetPosition()+Point(edge.GetWidth()-80-oldForceList[i],0));
 * * dc->DrawText(_T(string::Format(T("%i%%"),percent[i]/counter[i])),edge.GetPosition()+Point(edge.GetWidth()-120-oldForceList[i],0));
 * * int dx,dy;
 * * string bla=_T(string::Format(T("%2i/%2i"),anarace->getLocationTotal(0,i),(*anarace->getCurrentGoal())->allGoal[i]));
 * * dc->GetTextExtent(bla,&dx,&dy);
 * * dc->DrawText(bla,edge.GetPosition()+Point(edge.GetWidth()-45-dx,0));
 * * 
 * * } else
 * * {
 * * int dx,dy;
 * * string bla=_T(string::Format(T("%2i"),anarace->getLocationTotal(0,i)));
 * * dc->GetTextExtent(bla,&dx,&dy);
 * * dc->DrawText(bla,edge.GetPosition()+Point(edge.GetWidth()-45-dx,0));
 * * } 
 */
/*
				if ((isShown() == 1) && (edge.Inside(controls.getCurrentPosition())))
				{
		
				dc->SetBrush(*TRANSPARENT_BRUSH);
					dc->SetPen(*theme.lookUpPen(BITMAP_MARKED_PEN));
					Rect tedge[11];
					for (int j = 0; j < 11; j++)
					{
						tedge[j] = Rect(edge.x + edge.width - 117 + j * 10, edge.y + 1, 10, 10);
						if (tedge[j].Inside(controls.getCurrentPosition()))
							dc->DrawRectangle(tedge[j]);
					}

					buttonType[3] =
						addBitmapButton(Rect(tedge[3].GetPosition() + Point(1, 1), tedge[3].GetSize()), bmpArrowLeft,
										"increase priority");
					buttonType[4] =
						addBitmapButton(Rect(tedge[4].GetPosition() + Point(1, 1), tedge[4].GetSize()), bmpImp6,
										"set priority extremely high");
					buttonType[5] =
						addBitmapButton(Rect(tedge[5].GetPosition() + Point(1, 1), tedge[5].GetSize()), bmpImp5,
										"set priority very high");
					buttonType[6] =
						addBitmapButton(Rect(tedge[6].GetPosition() + Point(1, 1), tedge[6].GetSize()), bmpImp4,
										"set priority high");
					buttonType[7] =
						addBitmapButton(Rect(tedge[7].GetPosition() + Point(1, 1), tedge[7].GetSize()), bmpImp3,
										"set priority medium");
					buttonType[8] =
						addBitmapButton(Rect(tedge[8].GetPosition() + Point(1, 1), tedge[8].GetSize()), bmpImp2,
										"set priority low");
					buttonType[9] =
						addBitmapButton(Rect(tedge[9].GetPosition() + Point(1, 1), tedge[9].GetSize()), bmpImp1,
										"set priority very low");
					buttonType[10] =
						addBitmapButton(Rect(tedge[10].GetPosition() + Point(1, 1), tedge[10].GetSize()), bmpArrowRight,
										"decrease priority");*/
					// TODO
		//			currentUnitType = i;
	// TODO: evtl Option um alle Controls anzeigen zu lassen! (Taste oder so)
/*				} //end is shown
				dc->SetPen(*theme.lookUpPen(RECTANGLE_PEN));
			} // end  allgoal>0?
			line++;
		}
//		setMaxScrollY((t+1)*(FONT_SIZE+5));*/

	unsigned int grad=0;
	for(int i=UNIT_TYPE_TYPES;i--;)
	{
//		int x = getAbsoluteRightBound() - 40;
//		int y = getAbsoluteUpperBound() + 40;
	
//		pieColor(dc->GetSurface(), x, y, 30, grad, grad+d, (Uint32)(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN+i))->GetColor()));
//		filledpieColor(dc->GetSurface(), x, y, 30, grad, grad+currentUnitTypeCount[i], (Uint32)(*theme.lookUpBrush((eBrush)(BRIGHT_UNIT_TYPE_0_BRUSH+i))->GetColor()));
		grad+=currentUnitTypeCount[i];
	}
	
}

