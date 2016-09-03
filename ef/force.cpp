#include "force.hpp"

ForceEntry::ForceEntry(UI_Object* parent, Rect rect, Rect maxRect, string unit):UI_Button(parent, rect, maxRect, unit, unit, FORCE_ENTRY_BUTTON, NO_TEXT_MODE, NO_BUTTON_MODE, DO_NOT_ADJUST, SMALL_NORMAL_BOLD_FONT, NO_AUTO_SIZE)
	// TODO!
{
	addUnit = new UI_Button(this, Rect(Point(getWidth()-117,2),Size(8,8)), Rect(Point(0,0), getSize()), ADD_BUTTON, PRESS_BUTTON_MODE);
	subUnit = new UI_Button(this, Rect(Point(getWidth()-107,2),Size(8,8)), Rect(Point(0,0), getSize()), SUB_BUTTON, PRESS_BUTTON_MODE);
	cancelUnit = new UI_Button(this, Rect(Point(getWidth()-97,2),Size(8,8)), Rect(Point(0,0), getSize()), CANCEL_BUTTON, PRESS_BUTTON_MODE);
	addUnit->updateToolTip(*theme.lookUpString(ADD_UNIT_TOOLTIP_STRING));
	subUnit->updateToolTip(*theme.lookUpString(REMOVE_UNIT_TOOLTIP_STRING));
	cancelUnit->updateToolTip(*theme.lookUpString(REMOVE_GOAL_TOOLTIP_STRING));

	startForce=20;
	currentForce=20;
	targetForce=20;
}

ForceEntry::~ForceEntry()
{
	delete(addUnit);
	delete(subUnit);
	delete(cancelUnit);
}

void ForceEntry::process()
{
	if(!shown)
		return;
	if(!isMouseInside()) {
		addUnit->Hide();
		subUnit->Hide();
		cancelUnit->Hide();
	} else {
		addUnit->Show();
		subUnit->Show();
		cancelUnit->Show();
	}
	move(currentForce, startForce, targetForce);
	UI_Button::process();
}

const eUnitType ForceEntry::getType() const
{
#ifdef _SCC_DEBUG
    if((type<0)||(type>=UNIT_TYPE_TYPES)) {
        toLog("WARNING: (ForceEntry::getType): Variable not initialized.");return(UNIT_TYPE_TYPES);
    }
#endif
	return(type);
}

void ForceEntry::setTargetForce(const int force)
{
	if(targetForce!=force)
	{
		startForce=currentForce;
		if(targetForce<20)
			targetForce=20;
		else targetForce=force;
	}
}

void ForceEntry::draw(DC* dc) const
{
	if(!shown) return;
	UI_Button::draw(dc);
	//TODO: Was ist mit Einheiten die zwar als force da sind, aber nicht buildable ( = kein Eintrag in phaenoToGenoType) sind?
	dc->SetBrush(*theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+getType())));
	dc->SetPen(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN+getType())));
					
	//									  if(isShown()==1)
	dc->DrawRoundedRectangle(Rect(getAbsolutePosition()+Point(getWidth()-currentForce, 0), Size(currentForce+1,FONT_SIZE+5)), 3);
	//									  else if(isShown()==2)
	//		dc->DrawRoundedRectangle(Rect(edge.GetPosition() + Point(edge.GetWidth() - oldForce[i], 0), Size(oldForce[i] + 1, FONT_SIZE + 4)), 4);
	if (total < goal)
		dc->SetTextForeground(dc->mixColor(theme.lookUpColor(BRIGHT_TEXT_COLOUR), theme.lookUpColor(FULFILLED_TEXT_COLOUR)));
	else
		dc->SetTextForeground(dc->mixColor(theme.lookUpColor(BRIGHT_TEXT_COLOUR), theme.lookUpColor(NOT_FULFILLED_TEXT_COLOUR)));
	
	ostringstream os;
	os << total << "/" << goal;
	int dx, dy;
	dc->GetTextExtent(os.str(), &dx, &dy);
	dc->DrawText(os.str(), getAbsolutePosition() + Point(getWidth() - dx - 2, 2));
//} // end oldForceList >0
}

const int ForceEntry::changed() const
{
	if(addUnit->isPressed()) return(1);
	if(subUnit->isPressed()) return(2);
	if(cancelUnit->isPressed()) return(3);
	return(0);
}

void ForceEntry::updateText(string utext)
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

void ForceEntry::setTotal(const int total)
{
	this->total=total;
}

void ForceEntry::setGoal(const int goal)
{
/*#ifdef _SCC_DEBUG
	if((type<0)&&(type>=UNIT_TYPE_TYPES)) {
        toLog("WARNING: (ForceEntry::setType): Value out of range.");return;
    }
#endif*/
	this->goal=goal;
}

ForceWindow::ForceWindow(UI_Object* parent, ANARACE* anarace, const int windowNumber):UI_Window(parent, FORCE_WINDOW_TITLE_STRING, FORCE_WINDOW, windowNumber, SCROLLED)
{
	nongoals = new UI_StaticText(this, NON_GOALS_STRING, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), HORIZONTALLY_CENTERED_TEXT_MODE, FORCE_TEXT_COLOUR);
	goals = new UI_StaticText(this, GOALS_STRING, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), HORIZONTALLY_CENTERED_TEXT_MODE, FORCE_TEXT_COLOUR);
	
	this->anarace = anarace;
	
	resetData();

	for(int i=GAS_SCV+1;i--;)
	{
		forceEntry[i]=new ForceEntry(this, Rect(getRelativeClientRectPosition()+Point(0,0), Size(getClientRectWidth(),FONT_SIZE+5)), 
				Rect(getRelativeClientRectPosition(), getClientRectSize()+Size(0, getMaxRect().height)),  // max size -y?
						stats[anarace->getRace()][i].name);
		forceEntry[i]->Hide();
	}

	menuRadio = new UI_Radio(this);
	menuButton[RACE_MENU] = new UI_Button(this, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), Rect(Point(0,0),getSize()), CHOOSE_RACE_STRING, CHOOSE_RACE_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
	menuButton[UNIT_MENU] = new UI_Button(this, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), Rect(Point(0,0),getSize()), ADD_GOAL_STRING, ADD_GOAL_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
	menuButton[GOAL_MENU] = new UI_Button(this, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), Rect(Point(0,0),getSize()), GOAL_LIST_STRING, GOAL_LIST_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
	menuButton[FORCE_MENU] = new UI_Button(this, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), Rect(Point(0,0),getSize()), STARTFORCE_STRING, STARTFORCE_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);

	menuButton[RACE_MENU]->updateToolTip(*theme.lookUpString(CHOOSE_RACE_TOOLTIP_STRING));
	menuButton[UNIT_MENU]->updateToolTip(*theme.lookUpString(ADD_GOALS_TOOLTIP_STRING));
	menuButton[GOAL_MENU]->updateToolTip(*theme.lookUpString(CHOOSE_GOALS_TOOLTIP_STRING));
	menuButton[FORCE_MENU]->updateToolTip(*theme.lookUpString(CHOOSE_STARTING_FORCE_TOOLTIP_STRING));
	
	raceMenu=new RaceMenu(this, anarace, Rect(10, 20, getWidth()-200, 0));
	unitMenu=new UnitMenu(this, anarace, Rect(10, 20, getWidth()-10, 0));
	goalMenu=new GoalMenu(this, anarace, Rect(10, 20, getWidth()-100, 0));
	forceMenu=new ForceMenu(this, anarace, Rect(10, 20, getWidth()-200, 0));

	for(int i = 0; i<MAX_MENUS; i++)	
		menuRadio->addButton(menuButton[i]);
	
	totalUnits=0;
	for(int i = UNIT_TYPE_TYPES;i--;)
		unitTypeCount[i]=0;
}

ForceWindow::~ForceWindow()
{
//	for(int i=3;i--;)
//		delete goalButton[i];
	for(int i=GAS_SCV+1;i--;)
		delete forceEntry[i];
//	delete radio;

	delete nongoals;
	delete goals;

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

const int ForceWindow::getMarkedUnit() const
{
	return (markedUnit);
}

void ForceWindow::process()
{
	if(!shown)
		return;
	UI_Window::process();

	totalUnits=0;
	for(int i = UNIT_TYPE_TYPES;i--;)
		unitTypeCount[i]=0;

	currentUnitType = 0;
	markedUnit = 0;

	if(unitMenu->getPressedItem()>0)
	{
		(*anarace->getCurrentGoal())->addGoal(unitMenu->getPressedItem(), 1, 0, 0);
//		msgWindow->addMessage("Added %s to the goal list."),stats[anarace->getRace()][SCV].name)));
		// TODO connection zum mainwindow machen
		setChangedFlag();
	}
	if(goalMenu->getPressedItem()>=0)
	{
		settings.setGoal(anarace->getPlayerNum(), goalMenu->getPressedItem());
//		msgWindow->addMessage("Added %s to the goal list."),stats[anarace->getRace()][SCV].name)));
		// TODO connection zum mainwindow machen
		setChangedFlag();
	}
	if(forceMenu->getPressedItem()>=0)
	{
		settings.setStartcondition(anarace->getPlayerNum(), forceMenu->getPressedItem());
		settings.fillGroups();
		setChangedFlag();
	}

	if(raceMenu->getPressedItem()>=0)
	{
		switch(raceMenu->getPressedItem())
		{
			  case TERRA:
			  {
				  settings.setStartRace(anarace->getPlayerNum(), TERRA);
				  settings.setStartcondition(anarace->getPlayerNum(), TERRA);
				  settings.fillGroups();
				  settings.setGoal(anarace->getPlayerNum(), TERRA);
				  setChangedFlag();					
				  /// TODO hier wird mehrfach adjustgoals aufgerufen!!
			  };break;
			  case PROTOSS:
			  {
				  settings.setStartRace(anarace->getPlayerNum(), PROTOSS);
				  settings.setStartcondition(anarace->getPlayerNum(), PROTOSS);
				  settings.fillGroups();
				  settings.setGoal(anarace->getPlayerNum(), PROTOSS);
				setChangedFlag();					
//				  settings.initSoup();
			  };break;
			  case ZERG:
			  {
				  settings.setStartRace(anarace->getPlayerNum(), ZERG); // race setzen NACH setMap, da setMap die default race von geladener Map laedt
				  settings.setStartcondition(anarace->getPlayerNum(), ZERG);
				  settings.fillGroups();
				  settings.setGoal(anarace->getPlayerNum(), ZERG);
				  setChangedFlag();					
//				  settings.initSoup();	
			  };break;
			  default:break;
		}
	}
/*	if(radio->hasChanged())
	{
		switch (radio->getMarked())
			{
			  case TERRA:
			  {
				  settings.setStartRace(anarace->getPlayerNum(), TERRA);
				  settings.setStartcondition(anarace->getPlayerNum(), TERRA);
				  settings.fillGroups();
				  settings.setGoal(anarace->getPlayerNum(), TERRA);
				  setChangedFlag();					
				  /// TODO hier wird mehrfach adjustgoals aufgerufen!!
			  };break;
			  case PROTOSS:
			  {
				  settings.setStartRace(anarace->getPlayerNum(), PROTOSS);
				  settings.setStartcondition(anarace->getPlayerNum(), PROTOSS);
				  settings.fillGroups();
				  settings.setGoal(anarace->getPlayerNum(), PROTOSS);
				setChangedFlag();					
//				  settings.initSoup();
			  };break;
			  case ZERG:
			  {
				  settings.setStartRace(anarace->getPlayerNum(), ZERG); // race setzen NACH setMap, da setMap die default race von geladener Map laedt
				  settings.setStartcondition(anarace->getPlayerNum(), ZERG);
				  settings.fillGroups();
				  settings.setGoal(anarace->getPlayerNum(), ZERG);
				  setChangedFlag();					
//				  settings.initSoup();
			  };break;
			}
	} // race darf nur indirekt geaendert werden!*/

	if(menuRadio->hasChanged())
	{
		switch(menuRadio->getMarked())
		{
			// TODO in ne eigene Klasse oder so
			case RACE_MENU:
                    raceMenu->open();
                if(!raceMenu->isOpen()) {
                    menuRadio->forceUnpressAll();
                    closeMenus();
                } else {
                    closeMenus();
                    raceMenu->open();
                }
                break;
		
			case UNIT_MENU:
                    unitMenu->open();
                if(!unitMenu->isOpen()) {
                    menuRadio->forceUnpressAll();
                    closeMenus();
                } else {
                    closeMenus();
                    unitMenu->open();
                }
                break;
			case GOAL_MENU:
					goalMenu->open();
				if(!goalMenu->isOpen()) {
					menuRadio->forceUnpressAll();
					closeMenus();
				} else {
					closeMenus();
					goalMenu->open();
				}
				break;
			case FORCE_MENU:
				forceMenu->open();
				if(!forceMenu->isOpen()) {
					menuRadio->forceUnpressAll();
					closeMenus();
				} else {
					closeMenus();
					forceMenu->open();
				}
				break;
			default:break;
		}
	}

	if(getChangedFlag())
		resetData();
	
	if (!Rect(getAbsolutePosition(), getSize()).Inside(controls.getCurrentPosition())) {
		menuRadio->forceUnpressAll();
		closeMenus();
	}

// ------------------- GOALS
	int line = raceMenu->getHeight();
	if(unitMenu->getHeight() > line)
		line = unitMenu->getHeight();
	if(goalMenu->getHeight() > line)
		line = goalMenu->getHeight();
	if(forceMenu->getHeight() > line)
		line = forceMenu->getHeight();
	
	line++;

	goals->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line*(FONT_SIZE+6)), Size(getClientRectWidth(), FONT_SIZE+5)));
	line++;
// all units above GAS_SCV are pretty uninteresting (mostly temporary variables)
	for (int i = 0; i <= GAS_SCV; i++)
		if((*anarace->getCurrentGoal())->getAllGoal(i) > 0)
		{
			Rect edge = Rect(getRelativeClientRectPosition() + Point(0, line*(FONT_SIZE+6)), Size(getClientRectWidth(), FONT_SIZE+5));

			forceEntry[i]->setTargetForce(anarace->getLocationTotal(GLOBAL, i) * 75 / anarace->getUnitsTotalMax());
	
			totalUnits+=anarace->getLocationTotal(GLOBAL, i);
			unitTypeCount[stats[anarace->getRace()][i].unitType]+=anarace->getLocationTotal(GLOBAL, i);
		
			forceEntry[i]->setType(stats[anarace->getRace()][i].unitType);
			forceEntry[i]->setTotal(anarace->getLocationTotal(GLOBAL, i));
			forceEntry[i]->setGoal((*anarace->getCurrentGoal())->getAllGoal(i));
// TODO wenn Settings geaendert werden... geht total hinueber...
//		if (fitItemToRelativeClientRect(edge, 1))
			{
				forceEntry[i]->Show();
				if (forceEntry[i]->isCurrentlyHighlighted())
					markedUnit = i;
				forceEntry[i]->updateText("   " + stats[anarace->getRace()][i].name);
				forceEntry[i]->adjustRelativeRect(edge);
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
			}
/*		   	else
			{
				forceEntry[i]->Hide();
				forceEntry[i]->adjustRelativeRect(Rect(getRelativeClientRectPosition()+Point(-100,200), Size(getClientRectWidth(),FONT_SIZE+10)));
			}*/
			line++;
		} // goal > 0

	nongoals->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line*(FONT_SIZE+6)), Size(getClientRectWidth(), FONT_SIZE+5)));
//  ---------- NON GOALS
	line++;
	for (int i = 0; i <= GAS_SCV; i++)
		if (((*anarace->getCurrentGoal())->getAllGoal(i) == 0) && ((*anarace->getCurrentGoal())->getIsBuildable(i)))
		{
			Rect edge = Rect(getRelativeClientRectPosition() + Point(0, line*(FONT_SIZE+6)), Size(getClientRectWidth(), FONT_SIZE+5));

			forceEntry[i]->setTargetForce(anarace->getLocationTotal(GLOBAL, i) * 75 / anarace->getUnitsTotalMax());

			totalUnits+=anarace->getLocationTotal(GLOBAL, i);
			unitTypeCount[stats[anarace->getRace()][i].unitType]+=anarace->getLocationTotal(GLOBAL, i);
			
			forceEntry[i]->setType(stats[anarace->getRace()][i].unitType);
			forceEntry[i]->setTotal(anarace->getLocationTotal(GLOBAL, i));
			
			forceEntry[i]->setGoal((*anarace->getCurrentGoal())->getAllGoal(i));

		//	if (fitItemToRelativeClientRect(edge, 1))
			{
				forceEntry[i]->Show();
				if (forceEntry[i]->isCurrentlyHighlighted())
					markedUnit = i;
				ostringstream os;
				//os << anarace->getLocationTotal(0, i) << " " << stats[anarace->getRace()][i].name;
				//if (anarace->getLocationTotal(0, i) == 1);
				//else os << "s";
				//forceEntry[i]->updateText(os.str());
				forceEntry[i]->updateText("   " + stats[anarace->getRace()][i].name);
				forceEntry[i]->adjustRelativeRect(edge);
				switch(forceEntry[i]->changed())
				{
					case 0:break;
					case 1:(*anarace->getCurrentGoal())->addGoal(i, 1, 0, 0);break;  //<- Bei (+) kann sich nichts an den goals veraendern!
					case 2:(*anarace->getCurrentGoal())->addGoal(i, -1, 0, 0);
							setChangedFlag();break;		   // <- Bei (-) sehr wohl
					case 3:(*anarace->getCurrentGoal())->addGoal(i,-(*anarace->getCurrentGoal())->getGlobalGoal(0, i), 0, 0);
							setChangedFlag();break;		   // <- und auch bei X natuerlich
				};
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
			forceEntry[i]->Hide();
			forceEntry[i]->adjustRelativeRect(Rect(getRelativeClientRectPosition()+Point(0,0), Size(getClientRectWidth(),FONT_SIZE+10)));
		};
		Rect edge=Rect(getRelativeClientRectPosition() + Point(0, line*(FONT_SIZE+6)), Size(getClientRectWidth(), FONT_SIZE+5));
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
		int d = unitTypeCount[i] * 360 / totalUnits;
		if(d!=targetUnitTypeCount[i])
		{
			startUnitTypeCount[i]=currentUnitTypeCount[i];
			targetUnitTypeCount[i]=d;
		}		
		move(currentUnitTypeCount[i], startUnitTypeCount[i], targetUnitTypeCount[i]);
	}

		
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
	if(!shown)
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
	int line = 1;
//	int t = 0;

	dc->SetTextForeground(*theme.lookUpColor(FORCE_TEXT_COLOUR));
	dc->DrawText(*theme.lookUpString(STARTING_FORCE_STRING) , getAbsoluteClientRectPosition());

//		goalButton[i] = addButton(edge, "change race"); TODO tooltip
	line += 2;
	Rect edge;
/*	
		int dx, dy;
		string bla;
		if (!goalFileListOpened)
				bla = *theme.lookUpString(LOAD_GOAL_LIST_STRING);
		else
			bla = *theme.lookUpString(CLOSE_GOAL_LIST_STRING);
		dc->GetTextExtent(bla, &dx, &dy);
		Rect edge;
		int bright = goalFileListButtonAni;
		if (goalFileListOpened)
			bright += 100;
		edge = Rect(getClientRectLeftBound() + 5, getClientRectUpperBound() + 35, dx + 20, FONT_SIZE + 10);
		
		dc->SetBrush(theme.lookUpBrightenedBrush(FORCE_LIST_BRUSH, bright));
		dc->DrawRoundedRectangle(edge, 3);
		dc->DrawText(bla, edge.x + 5, edge.y + 3);
		dc->DrawBitmap(*theme.lookUpBitmap(ARROW_DOWN_BITMAP), edge.x + edge.width - 15, edge.y + 5);
//		goalFileListButton = addButton(edge); TODO
		dc->SetPen(*theme.lookUpPen(RECTANGLE_PEN));

		line += 2;
		if (goalFileListOpened)
		{
			for (int i = 0; i < settings.getCurrentGoalCount(); i++)
				if (settings.getCurrentGoal(i)->getRace() == anarace->getRace())
				{
					GOAL_ENTRY	 *goal = settings.getCurrentGoal(i);
					int bright = 0;
					Rect edge = Rect(getClientRectPosition() + Point(10,
																	  line * (FONT_SIZE + 5) - getScrollY()),
										 Size(270,
												FONT_SIZE + 5));
					if (edge.Inside(controls.getCurrentPosition()))
						bright = 50;
					if (fitItemToClientRect(edge, 1))
					{
						dc->SetBrush(theme.lookUpBrightenedBrush(BO_DARK_BRUSH, bright));
						dc->DrawRoundedRectangle(edge, 4);
						dc->SetTextForeground(*theme.lookUpColor(TEXT_COLOUR));
						dc->DrawText(_T(string::Format(T("%s"), goal->getName())), edge.GetPosition() + Point(10, 0));
//						selectGoalButton[i] = addButton(edge, "load this goal into that player"); TODO
					}
					line++;
				}
		}
		dc->SetFont(theme.lookUpFont(SMALL_NORMAL_BOLD_FONT));
		dc->SetTextForeground(*theme.lookUpColor(TEXT_COLOUR));
		edge = Rect(getClientRectPosition() + Point(0, line * (FONT_SIZE + 5) - getScrollY()), Size(270, FONT_SIZE + 5));
		if (fitItemToClientRect(edge, 1))
		{
			dc->SetBrush(*theme.lookUpBrush(FORCE_LIST_BUTTON_BRUSH));
			dc->DrawRoundedRectangle(edge, 4);
			
			dc->SetTextForeground(*theme.lookUpColor(FORCE_TEXT_COLOUR));
			if (!goalListOpened)
			{
				dc->DrawBitmap(*theme.lookUpBitmap(ARROW_DOWN_BITMAP), edge.x + 4, edge.y + 2);
				dc->DrawText(*theme.lookUpString(CLICK_TO_ADD_GOAL_STRING), edge.GetPosition() + Point(14, 0));
			} else if (goalListOpened == 1)
			{
				dc->DrawBitmap(*theme.lookUpBitmap(ARROW_UP_BITMAP), edge.x + 4, edge.y + 2);
				dc->DrawText(*theme.lookUpString(CLICK_TO_CLOSE_STRING), edge.GetPosition() + Point(14, 0));
			} else
			{
				dc->DrawBitmap(*theme.lookUpBitmap(ARROW_UP_BITMAP), edge.x + 4, edge.y + 2);
				dc->DrawText(*theme.lookUpString(CLICK_TO_GO_BACK_STRING), edge.GetPosition() + Point(14, 0));
			}
//			addGoalButton = addButton(edge); TODO
			line++;
		}

		dc->SetPen(*theme.lookUpPen(RECTANGLE_PEN));
		if (goalListOpened == 1)
		{
			int l = 0;
			for (int i = 1; i < 11; i++)
			{
				int bright = 0;
				Rect edge = Rect(getClientRectPosition() + Point(10,
																  line * (FONT_SIZE + 5) - getScrollY()),
									 Size(270, FONT_SIZE + 5));
				if (edge.Inside(controls.getCurrentPosition()))
					bright = 50;
				if (fitItemToClientRect(edge, 1))
				{
					dc->SetBrush(theme.lookUpBrightenedBrush((eBrush)(BRIGHT_UNIT_TYPE_1_BRUSH+i), bright+100));
					dc->DrawRoundedRectangle(edge, 4);
					dc->SetTextForeground(*theme.lookUpColor(TEXT_COLOUR));
					dc->DrawText(*theme.lookUpString((eString)(UNIT_TYPE_0_STRING+i)), edge.GetPosition() + Point(10, 0));
//					addUnitButton[l] = addButton(edge); TODO
					l++;
				}
				line++;			// !!!
			}
		} else if (goalListOpened > 1)
		{
			int l = 0;
			int type = goalListOpened + 1;
			for (int i = 0; i < UNIT_TYPE_COUNT; i++)
			{
				if (stats[anarace->getRace()][i].facilityType == type)
				{
					int bright = 0;
					Rect edge = Rect(getClientRectPosition() + Point(10,
																	  line * (FONT_SIZE + 5) - getScrollY()),
										 Size(270,
												FONT_SIZE + 5));
					if (edge.Inside(controls.getCurrentPosition()))
						bright = 50;
					if (fitItemToClientRect(edge, 1))
					{
						dc->SetBrush(theme.lookUpBrightenedBrush((eBrush)(BRIGHT_UNIT_TYPE_1_BRUSH+i), bright+100));
						dc->DrawRoundedRectangle(edge, 4);
						dc->SetTextForeground(*theme.lookUpColor(TEXT_COLOUR));
						dc->DrawText(_T(string::Format(T("%s"), stats[anarace->getRace()][i].name)),
									 edge.GetPosition() + Point(10, 0));
//						addUnitButton[l] = addButton(edge); TODO
						l++;
					}
					line++;		//!!!
				}
			}
		}

	};*/
//end isShown==1
//	  line++;








/*				dc->SetBrush(*TRANSPARENT_BRUSH);	//TODO gruen rot  evtl
				if (isShown() == 1)
					dc->DrawRoundedRectangle(edge.GetPosition() +
											 Point(edge.GetWidth() -
													 (*anarace->getCurrentGoal())->allGoal[i] * 100 / anarace->getUnitsTotalMax(), 0),
											 Size((*anarace->getCurrentGoal())->allGoal[i] * 100 / anarace->getUnitsTotalMax() + 1,
													FONT_SIZE + 4), 4);
				else if (isShown() == 2)
					dc->DrawRoundedRectangle(edge.GetPosition() +
											 Point(edge.GetWidth() -
													 (*anarace->getCurrentGoal())->allGoal[i] * 100 / anarace->getUnitsTotalMax(), 0),
											 Size((*anarace->getCurrentGoal())->allGoal[i] * 100 / anarace->getUnitsTotalMax() + 1,
													FONT_SIZE + 4), 4); TODO ... mode oder so */

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

					buttonType[0] =
						addBitmapButton(Rect(tedge[0].GetPosition() + Point(1, 1), tedge[0].GetSize()), bmpAdd,
										"add one");
					buttonType[1] =
						addBitmapButton(Rect(tedge[1].GetPosition() + Point(1, 1), tedge[1].GetSize()), bmpSub,
										"remove one");
					buttonType[2] =
						addBitmapButton(Rect(tedge[2].GetPosition() + Point(1, 1), tedge[2].GetSize()), bmpCancel,
										"remove goal");
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

// Trennungslinie
/*	{
		Rect edge = Rect(getRelativeClientRectPosition() + Point(20, line * (FONT_SIZE + 5) + 9 - getScrollY()),
							 Size(getClientRectWidth() - 40, 2));
		if (fitItemToRelativeClientRect(edge, 1)) // TODO
		{
			dc->SetTextForeground(*theme.lookUpColor(FORCE_TEXT_COLOUR));
			dc->DrawText(*theme.lookUpString(NON_GOALS_STRING), getAbsoluteClientRectPosition() + Point(20, line * (FONT_SIZE + 5) - getScrollY()));
//					  dc->SetPen(Pen(Color(200,0,0),2,SOLID));
//					  dc->DrawLine(edge.GetPosition(),edge.GetPosition()+Point(edge.width,0));
//					  dc->SetPen(Pen(Color(0,0,0),1,SOLID));
		}
	}*/
//	dc->SetFont(theme.lookUpFont(SMALL_NORMAL_BOLD_FONT));
//	dc->SetTextForeground(*theme.lookUpColor(BRIGHT_TEXT_COLOUR));

		
	int grad=0;
	for(int i=UNIT_TYPE_TYPES;i--;)
	{
		int x = getAbsolutePosition().x+getWidth()-40;
		int y = getAbsolutePosition().y+40;
	
//		pieColor(dc->GetSurface(), x, y, 30, grad, grad+d, (Uint32)(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN+i))->GetColor()));
		filledpieColor(dc->GetSurface(), x, y, 30, grad, grad+currentUnitTypeCount[i], (Uint32)(*theme.lookUpBrush((eBrush)(BRIGHT_UNIT_TYPE_0_BRUSH+i))->GetColor()));
		grad+=currentUnitTypeCount[i];
	}
	
}

