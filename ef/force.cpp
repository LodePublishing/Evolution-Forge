#include "force.hpp"
#include "../ui/editfield.hpp"

ForceWindow::ForceWindow(UI_Object* force_parent, ANARACE* force_anarace, MessageWindow* force_msg_window, const unsigned int force_window_number) :
	UI_Window(force_parent, FORCE_WINDOW_TITLE_STRING, FORCE_WINDOW, force_window_number, SCROLLED),
	currentGoalUnit(0),
	msgWindow(force_msg_window),
	menuRadio(new UI_Radio(this, getRelativeClientRectPosition() + Point(0,10) )),
	saveGoalButton(new UI_Button(this, Rect(getRelativeClientRectPosition() + Size(0, 10), getClientRectSize()), Rect(Point(0,0), getSize()), SAVE_GOAL_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_RIGHT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE)),
	nongoals(new UI_StaticText(this, NON_GOALS_STRING, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), FORCE_TEXT_COLOR, SMALL_NORMAL_BOLD_FONT)),
	goals(new UI_StaticText(this, GOALS_STRING, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), FORCE_TEXT_COLOR, SMALL_NORMAL_BOLD_FONT)),
	legend(new UI_StaticText(this, TIME_LEGEND_STRING, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), FORCE_TEXT_COLOR, SMALL_NORMAL_BOLD_FONT, RIGHT_BOUNDED_TEXT_MODE)),
	forceList(),
	markedUnit(0),
	oldMarkedUnit(0),
	anarace(force_anarace),
	unitMenu(new UnitMenu(this, anarace, Rect(10, 10, getWidth()-10, 0))),
	goalMenu(new GoalMenu(this, anarace, Rect(10, 10, getWidth()-100, 0))),
//	forceMenu(new ForceMenu(this, anarace, Rect(10, 10, getWidth()-200, 0))), TODO in map tab verschieben
	raceMenu(new RaceMenu(this, Rect(10, 10, getWidth()-200, 0))) // TODO
{	
	resetData();
	
	menuButton[RACE_MENU] = new UI_Button(this, Rect(Point(0, 0), getClientRectSize()), Rect(Point(0,0), getSize()), CHOOSE_RACE_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE_ONCE);
	menuButton[UNIT_MENU] = new UI_Button(this, Rect(Point(0, 0), getClientRectSize()), Rect(Point(0,0), getSize()), ADD_GOAL_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE_ONCE);
	menuButton[GOAL_MENU] = new UI_Button(this, Rect(Point(0, 0), getClientRectSize()), Rect(Point(0,0), getSize()), GOAL_LIST_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE_ONCE);
//	menuButton[FORCE_MENU] = new UI_Button(this, Rect(getRelativeClientRectPosition()+Point(0,10), getClientRectSize()), Rect(Point(0,0),getSize()), STARTFORCE_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);

	menuButton[RACE_MENU]->updateToolTip(CHOOSE_RACE_TOOLTIP_STRING);
	menuButton[UNIT_MENU]->updateToolTip(ADD_GOALS_TOOLTIP_STRING);
	menuButton[GOAL_MENU]->updateToolTip(CHOOSE_GOALS_TOOLTIP_STRING);
//	menuButton[FORCE_MENU]->updateToolTip(CHOOSE_STARTING_FORCE_TOOLTIP_STRING);
	
	raceMenu->Hide();
	unitMenu->Hide();
	goalMenu->Hide();
//	forceMenu->Hide();

	for(int i = 0; i<MAX_MENUS; i++)	
		menuRadio->addButton(menuButton[i]);
	menuRadio->calculateSameWidthOfButtons(true);
}

ForceWindow::~ForceWindow()
{

	for(std::list<ForceEntry*>::iterator i = forceList.begin(); i!= forceList.end(); ++i)
		delete (*i);
	
		
	delete nongoals;
	delete goals;
	delete legend;

	for(int i=MAX_MENUS;i--;)
		delete menuButton[i];
	
	delete unitMenu;
	delete goalMenu;
//	delete forceMenu;
	delete raceMenu; 
	
	delete menuRadio;
}

void ForceWindow::closeMenus()
{
	unitMenu->close();
	goalMenu->close();
//	forceMenu->close();
	raceMenu->close();
}

void ForceWindow::setMarkedUnit(const unsigned int marked_unit)
{
#ifdef _SCC_DEBUG
    if(marked_unit >= UNIT_TYPE_COUNT) {
        toLog("DEBUG: (ForceWindow::setMarkedUnit): Value marked_unit out of range.");return;
    }
#endif
	markedUnit = marked_unit;
}
	
const unsigned int ForceWindow::getMarkedUnit() const
{
#ifdef _SCC_DEBUG
    if(markedUnit >= UNIT_TYPE_COUNT) {
        toLog("DEBUG: (ForceWindow::getMarkedUnit): Value markedUnit out of range.");return(0);
    }
#endif
	return (markedUnit);
}

void ForceWindow::process()
{
	if(!isShown())
		return;
	ForceEntry::changed=false;
	UI_Window::process();
	if(ForceEntry::changed)
		setChangedFlag();
	oldMarkedUnit = markedUnit;
	markedUnit = 0;

	int addUnit = 0;
	int addTime = 0;
	int addLocation = 0;
	int addCount = 0;
	
	int assignGoal = -1;
//	int assignForce = -1;
	int assignRace = -1;

	currentGoalUnit = unitMenu->getMarkedItem();

	if(saveGoalButton->isLeftClicked()&&(!UI_Object::editTextField))
		UI_Object::editTextField = new UI_EditField(getParent(), saveGoalButton, SAVE_GOALS_AS_STRING, GIVE_GOAL_A_NAME_STRING);

	if((UI_Object::editTextField)&&(UI_Object::editTextField->getCaller()==saveGoalButton))
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
		        settings.saveGoal(editTextField->getString(), *anarace->getCurrentGoal());
	        	msgWindow->addMessage(UI_Object::theme.lookUpFormattedString(SAVED_GOAL_STRING, (*anarace->getCurrentGoal())->getName()));
			}
			delete UI_Object::editTextField;
			UI_Object::resetButton();
			UI_Object::editTextField=NULL;
		}
	}
	
	if(unitMenu->getPressedItem()>=0)
	{
		bool allow = true;
		const std::list<unsigned int> errorList = (*anarace->getCurrentGoal())->allowAddition(unitMenu->getPressedItem());
		if(errorList.size()>0)
		{
			for(std::list<unsigned int>::const_iterator blink = errorList.begin(); blink != errorList.end(); ++blink)
			{
				for(std::list<ForceEntry*>::iterator f = forceList.begin(); f!=forceList.end(); ++f)
				{
					if((*f)->getUnit() == (*blink))
					{
						(*f)->resetGradient();
						break;
					}
				}
			}
			allow=false;
		}
		if(allow)
		{
			addUnit = unitMenu->getPressedItem(); addCount = 1; addTime = 0; addLocation = 0;
			msgWindow->addMessage(UI_Object::theme.lookUpFormattedString(ADDED_GOAL_STRING, *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+unitMenu->getPressedItem()+UNIT_NULL_STRING))));
		}
	}

	if(goalMenu->getPressedItem()>=0)
	{
		assignGoal = goalMenu->getPressedItem();
		menuRadio->forceUnpressAll();
		msgWindow->addMessage(UI_Object::theme.lookUpFormattedString(SET_NEW_GOAL_LIST_STRING, (*anarace->getCurrentGoal())->getName()));
		menuButton[GOAL_MENU]->forceUnpress();
	}
	
/*	if(forceMenu->getPressedItem()>=0)
	{
		assignForce = forceMenu->getPressedItem();
		menuRadio->forceUnpressAll();
		menuButton[FORCE_MENU]->forceUnpress();
	} TODO in maptab verschieben */

	if(raceMenu->getPressedItem()>=0)
	{
		assignRace = raceMenu->getPressedItem();
		msgWindow->addMessage(UI_Object::theme.lookUpFormattedString(SET_RACE_STRING, *theme.lookUpString((eString)(TERRA_STRING + assignRace)))); 
    	menuButton[RACE_MENU]->forceUnpress();
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
//				bool maybePushAgain=false;
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
			
/*			case FORCE_MENU:
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
			break;*/
			default:break;
		}
	} // end of menuRadio has changed

		
	
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
//	if(forceMenu->getHeight() > startLine)
//		startLine = forceMenu->getHeight();
	unsigned int line = 1;
	
	anarace->countUnitsTotal();

	goals->setFreeMove();
	goals->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5)));
	legend->setFreeMove();
	legend->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5)));

	line++;

    std::list<ForceEntry*>::iterator forceEntry = forceList.begin();

	for(std::list<GOAL>::const_iterator i = (*anarace->getCurrentGoal())->goal.begin(); i!= (*anarace->getCurrentGoal())->goal.end(); ++i)
	{
	// TODO 2. Modus machen um Produktionsgebaeude ueber 'facility' zu bestimmen (fuer hotkeys geradezu praedestiniert)
		if(anarace->getLocationTotal(i->getLocation(), i->getUnit()) >= i->getCount())
			(*forceEntry)->setTargetForce(getClientRectWidth() / 3);
		else
			(*forceEntry)->setTargetForce(anarace->getLocationTotal(i->getLocation(), i->getUnit()) * getClientRectWidth() / (3*i->getCount()));
		(*forceEntry)->setTotalNumber(anarace->getLocationTotal(i->getLocation(), i->getUnit()));
				
		Rect edge = Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)-getScrollY()), 
						Size((*forceEntry)->getWidth(), FONT_SIZE+6));
		(*forceEntry)->setFreeMove();
		(*forceEntry)->adjustRelativeRect(edge);
	
		if((*forceEntry)->isLeftClicked())
		{
			bool allow = true;
			const std::list<unsigned int> errorList = (*anarace->getCurrentGoal())->allowAddition(i->getUnit());
			if(errorList.size()>0)
			{
				for(std::list<unsigned int>::const_iterator blink = errorList.begin(); blink != errorList.end(); ++blink)
				{
					for(std::list<ForceEntry*>::iterator f = forceList.begin(); f!=forceList.end(); ++f)
					{
						if((*f)->getUnit() == (*blink))
						{
							(*f)->resetGradient();
							break;
						}
					}
				}
				allow=false;
			}			
			if(allow)
			{
				addUnit = i->getUnit();	addCount = 1; addTime = i->getTime();	addLocation = i->getLocation();
				msgWindow->addMessage(UI_Object::theme.lookUpFormattedString(ADDED_ONE_GOAL_STRING, *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i->getUnit()+UNIT_NULL_STRING))));
			}
		}
		if((*forceEntry)->isRightClicked())
		{
			bool allow=true;
			if(i->getCount()==1)
			{
				const std::list<unsigned int> errorList = (*anarace->getCurrentGoal())->allowDeletion(i->getUnit());
				if(errorList.size()>0)
				{
					for(std::list<unsigned int>::const_iterator blink = errorList.begin(); blink != errorList.end(); ++blink)
					{
						for(std::list<ForceEntry*>::iterator f = forceList.begin(); f!=forceList.end(); ++f)
						{
							if((*f)->getUnit() == (*blink))
							{
								(*f)->resetGradient();
								break;
							}
						}
					}
					allow=false;
				}
			}
			if(allow)
			{
				addUnit = i->getUnit();	addCount = -1; addTime = i->getTime();	addLocation = i->getLocation();
				msgWindow->addMessage(UI_Object::theme.lookUpFormattedString(REMOVED_ONE_GOAL_STRING, *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i->getUnit()+UNIT_NULL_STRING))));
			}
		}
		line++;
		forceEntry++;
	} // end of for

	nongoals->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5)));
//  ---------- NON GOALS
	line++;
	for (unsigned int i = 0; i <= GAS_SCV; i++)
//		if (((*anarace->getCurrentGoal())->getAllGoal(i) == 0) && (((*anarace->getCurrentGoal())->getIsBuildable(i))|| ( anarace->getLocationTotal(0,i))))
		if ((*anarace->getCurrentGoal())->getIsHaveable(i))
		{
			(*forceEntry)->setTargetForce(anarace->getLocationTotal(GLOBAL, i) * getClientRectWidth() / (3*anarace->getNonGoalsUnitsTotalMax()));
			(*forceEntry)->setTotalNumber(anarace->getLocationTotal(GLOBAL, i));
		
			Rect edge = Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)-getScrollY()), 
							Size((*forceEntry)->getWidth(), FONT_SIZE+6));
			(*forceEntry)->setFreeMove();
			(*forceEntry)->adjustRelativeRect(edge);

			if((*forceEntry)->isLeftClicked())
			{
				addUnit = i;	addCount = 1; addTime = 0;	addLocation = 0;
				msgWindow->addMessage(UI_Object::theme.lookUpFormattedString(MOVED_NON_GOAL_STRING, *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i+UNIT_NULL_STRING))));
			}
			fitItemToRelativeClientRect(edge, 1);

			line++;
			forceEntry++;
		} // goal > 0
//		else if((*anarace->getCurrentGoal())->getAllGoal(i) == 0)
//		{
//			forceEntry[j]->HideIt();
//			forceEntry[j]->adjustRelativeRect(Rect(getRelativeClientRectPosition()+Point(0,0), Size(getClientRectWidth(),FONT_SIZE+10)));
//			j++;
//		}

	Rect edge=Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+6));
	fitItemToRelativeClientRect(edge, 1);
	
	if(addCount!=0)
	{
//		bool new_unit = ((*anarace->getCurrentGoal())->getAllGoal(addUnit)==0);
		(*anarace->getCurrentGoal())->addGoal(addUnit, addCount, addTime, addLocation);
//		if(((*anarace->getCurrentGoal())->getAllGoal(addUnit)==0)||new_unit) TODO: 3. Status fuer ChangedFlag machen, damit zwar BO resetted aber die Goals nicht neuberechnet werden muessen
			setChangedFlag();
	}

	if(assignGoal>=0)
	{
		settings.assignGoal(anarace->getPlayerNumber(), goalMenu->getPressedItem());
		setChangedFlag();		
	}
/*	if(assignForce>=0)
	{
		settings.assignStartcondition(anarace->getPlayerNumber(), forceMenu->getPressedItem());
		settings.fillGroups();
		setChangedFlag();
    //  ostringstream os; TODO
//      os << "Set " << settings.getStartcondition(anarace->getPlayerNumber(), fo)->getName() << " as starting force.";
  //    msgWindow->addMessage(os.str())
	} TODO in Map-Tab verschieben*/
	if(assignRace>=0)
	{
		settings.assignStartRace(anarace->getPlayerNumber(), (eRace)assignRace);
		settings.assignStartcondition(anarace->getPlayerNumber(), 0);
		settings.fillGroups();
		settings.assignGoal(anarace->getPlayerNumber(), 0);
		unitMenu->resetData();
		goalMenu->resetData();
		switch(assignRace)
		{
			case TERRA:UI_Object::theme.setColorTheme(DARK_BLUE_THEME);break;
			case PROTOSS:UI_Object::theme.setColorTheme(YELLOW_THEME);break;
			case ZERG:UI_Object::theme.setColorTheme(DARK_RED_THEME);break;
			default:break;
		}
		setResetFlag(); //!
	}

/*	if(getChangedFlag())
	{
//		(*anarace->getCurrentGoal())->adjustGoals(true, (*anarace->getStartCondition())->getUnit(0) );
		resetData();
//		unitMenu->resetData(); CHANGED
	}*/
	
	
}

void ForceWindow::resetData()
{
//	forceList.clear();	
	int size = forceList.size();
	int n = 0;
	std::list<ForceEntry*>::iterator j = forceList.begin();

    for(std::list<GOAL>::iterator i = (*anarace->getCurrentGoal())->goal.begin(); i!= (*anarace->getCurrentGoal())->goal.end(); ++i)
	{
		ForceEntry* forceEntry; 
		if(n>=size)
		{
			forceEntry = new ForceEntry(this, Rect(getRelativeClientRectPosition()+Point(0,0), Size(getClientRectWidth(), FONT_SIZE+6)),
			        Rect(getRelativeClientRectPosition(), getClientRectSize()+Size(0, getMaxRect().GetHeight())),"");
			forceList.push_back(forceEntry);
			size++;
		} else 
		{
			forceEntry = *j;
			j++;
		}

		forceEntry->setUnit(i->getUnit());
		forceEntry->setType(stats[anarace->getRace()][i->getUnit()].unitType);
		forceEntry->updateText("   " + *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i->getUnit()+UNIT_NULL_STRING)));
		forceEntry->assignGoal(&*i);

		n++;
	}

//	bool nonGoalList[GAS_SCV+1];
//	for(int l = 0;l<GAS_SCV+1;l++)
//		nonGoalList[l]=false;
// TODO die raussuchen die schon bestehen, unit vergleichen etc.		
	int k = 0;
    for (unsigned int i = 0; i <= GAS_SCV; i++)
//        if ( ((*anarace->getCurrentGoal())->getAllGoal(i) == 0) && (((*anarace->getCurrentGoal())->getIsBuildable(i))||(anarace->getLocationTotal(0,i))))
        if ((*anarace->getCurrentGoal())->getIsHaveable(i)) 
//			nonGoalList[i]=true;
	    {
			k++;
        	ForceEntry* forceEntry;
    	    if(n>=size)
	        {
        	    forceEntry = new ForceEntry(this, Rect(getRelativeClientRectPosition()+Point(0,0), Size(getClientRectWidth(), FONT_SIZE+6)),
    	                Rect(getRelativeClientRectPosition(), getClientRectSize()+Size(0, getMaxRect().GetHeight())), "");
            	forceList.push_back(forceEntry);
        	    size++;
    	    } else
	        {
        	    forceEntry = *j;
    	        j++;
	        }

			forceEntry->setUnit(i);
			forceEntry->setType(stats[anarace->getRace()][i].unitType);
            forceEntry->updateText("   " + *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i+UNIT_NULL_STRING)));
            forceEntry->assignGoal(NULL);
			
			n++;
		}

		while(j!=forceList.end())
		{
			delete(*j);
			j = forceList.erase(j);
		}
			
//	int z = forceList.size();
//	int zz = (*anarace->getCurrentGoal())->goal.size(); 
	
	markedUnit = 0;
}

void ForceWindow::drawTechTree(DC* dc) const
{
	if(unitMenu->isOpen())
	{
		Point p(300,500);
		Size s(100, 15);
		Size s2(120, 25);

		int x1 = p.x;
		int x2 = p.x + 5*s2.GetWidth();
		int y2 = 10 * s2.GetHeight();

		const GOAL_TREE& tree = anarace->getGoalTree(currentGoalUnit);

		x1=9999;
		for(int k = 0; k<10; k++)
			if(tree.width[k]>0)
			{
				bool foundOne = false;
				unsigned int x=0;
				int px=0;
				for(std::list<unsigned int>::const_iterator i = tree.unit[k].begin(); i!=tree.unit[k].end();i++)
				{
					px=p.x + x*5*s2.GetWidth()/tree.width[k] + (5*s2.GetWidth()/tree.width[k])/2;
					foundOne=true;
					if(x1 > px - (signed int)(s.GetWidth())/2)
						x1 = px - (s.GetWidth())/2;
					x++;
				}
				if((px > x2))// /+ (s.GetWidth())/2)>x2)
					x2 = px;// + (s.GetWidth())/2;
				if(foundOne)
					y2 = p.y + k*s2.GetHeight()+3+s.GetHeight();
			}
			
		dc->DrawEdgedRoundedRectangle(x1-5, p.y-5, (unsigned int)(x2 - x1), (unsigned int)(y2 - p.y + 10), 4);

		for(unsigned int k = 0; k<10; k++)
			if(tree.width[k]>0)
			{
				unsigned int x=0;
				for(std::list<unsigned int>::const_iterator i = tree.unit[k].begin(); i!=tree.unit[k].end();i++)
				{
					dc->SetBrush(*theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[anarace->getRace()][*i].unitType)));
					dc->SetPen(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN+stats[anarace->getRace()][*i].unitType)));
					
					for(std::list<unsigned int>::const_iterator j = tree.con[*i].begin(); j!= tree.con[*i].end(); j++)
					{
						if(tree.level[*j] > k)
							dc->DrawLine(p + Point(x*5*s2.GetWidth()/tree.width[k]+s.GetWidth()/2 + (5*s2.GetWidth()/tree.width[k] - s.GetWidth())/2, s.GetHeight()+k*s2.GetHeight()), p+Point(tree.coloumn[*j]*5*s2.GetWidth()/tree.width[tree.level[*j]]+s.GetWidth()/2 + (5*s2.GetWidth()/tree.width[tree.level[*j]] - s.GetWidth())/2, tree.level[*j]*s2.GetHeight()));
						else if(tree.level[*j] == k)
						{
							if(tree.coloumn[*j]<x)
								dc->DrawLine(p + Point(x*5*s2.GetWidth()/tree.width[k]+(5*s2.GetWidth()/tree.width[k] - s.GetWidth())/2, s.GetHeight()/2+k*s2.GetHeight()), p+Point(tree.coloumn[*j]*5*s2.GetWidth()/tree.width[k]+s.GetWidth() + (5*s2.GetWidth()/tree.width[k] - s.GetWidth())/2, s.GetHeight()/2+k*s2.GetHeight()));
							else 
								dc->DrawLine(p + Point(x*5*s2.GetWidth()/tree.width[k]+s.GetWidth()+(5*s2.GetWidth()/tree.width[k] - s.GetWidth())/2, s.GetHeight()/2+k*s2.GetHeight()), p+Point(tree.coloumn[*j]*5*s2.GetWidth()/tree.width[k] + (5*s2.GetWidth()/tree.width[k] - s.GetWidth())/2, s.GetHeight()/2+k*s2.GetHeight()));
						}
						else
							dc->DrawLine(p + Point(x*5*s2.GetWidth()/tree.width[k]+s.GetWidth()/2+(5*s2.GetWidth()/tree.width[k] - s.GetWidth())/2, k*s2.GetHeight()), p + Point(tree.coloumn[*j]*5*s2.GetWidth()/tree.width[tree.level[*j]]+s.GetWidth()/2+(5*s2.GetWidth()/tree.width[tree.level[*j]] - s.GetWidth())/2, s.GetHeight()+tree.level[*j]*s2.GetHeight()));
					}
					x++;
				}
			}
		for(unsigned int k = 0; k<10; k++)
			if(tree.width[k]>0)
			{
				unsigned int x=0;
				int px=0;
				for(std::list<unsigned int>::const_iterator i = tree.unit[k].begin(); i!=tree.unit[k].end();i++)
				{
					px=p.x + x*5*s2.GetWidth()/tree.width[k] + (5*s2.GetWidth()/tree.width[k])/2;
					dc->SetBrush(*theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[anarace->getRace()][*i].unitType)));
					dc->SetPen(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN+stats[anarace->getRace()][*i].unitType)));
					dc->DrawEdgedRoundedRectangle(Rect(Point(px - (s.GetWidth())/2, p.y + k*s2.GetHeight()), s), 4);
					dc->DrawText(*UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+(*i)+UNIT_NULL_STRING)), Point(px+4- (s.GetWidth())/2, p.y + k*s2.GetHeight()+3));
					x++;
				}
			}
	}
}

void ForceWindow::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Window::draw(dc);
	mouseType=0;
/*    for(std::list<ForceEntry*>::const_iterator j = forceList.begin(); (!mouseType)&&(j!= forceList.end()); ++j)
	{
		if((*j)->isCurrentlyHighlighted())
		{
			const std::list<unsigned int> subErrorList = (*anarace->getCurrentGoal())->allowDeletion((*j)->getUnit());
			const std::list<unsigned int> addErrorList = (*anarace->getCurrentGoal())->allowAddition((*j)->getUnit());
			if((subErrorList.size()>0)&&(!addErrorList.size()))
				mouseType = 1;
			else if((!subErrorList.size())&&(!addErrorList.size()))
				mouseType = 3;
			else if((!subErrorList.size())&&(addErrorList.size()>0))
				mouseType = 2;
		}
	}
	drawTechTree(dc);*/
}

