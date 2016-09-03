#include "force.hpp"

ForceWindow::ForceWindow(UI_Object* force_parent, ANARACE* force_anarace, MessageWindow* force_msg_window, const unsigned int force_window_number) :
	UI_Window(force_parent, FORCE_WINDOW_TITLE_STRING, FORCE_WINDOW, force_window_number, SCROLLED),
	msgWindow(force_msg_window),
	changed(false),
	menuRadio(new UI_Radio(this)),
	saveGoalButton(new UI_Button(this, Rect(getRelativeClientRectPosition()+Point(0,10), getClientRectSize()), Rect(Point(0,0),getSize()), SAVE_GOAL_STRING, SAVE_GOAL_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, PRESS_BUTTON_MODE, ARRANGE_TOP_RIGHT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE)),
	nongoals(new UI_StaticText(this, NON_GOALS_STRING, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), NO_TEXT_MODE, FORCE_TEXT_COLOR)),
	goals(new UI_StaticText(this, GOALS_STRING, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), NO_TEXT_MODE, FORCE_TEXT_COLOR)),
	legend(new UI_StaticText(this, LEGEND_STRING, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR)),
	markedUnit(0),
	oldMarkedUnit(0),
	anarace(force_anarace),
	unitMenu(new UnitMenu(this, anarace, Rect(10, 10, getWidth()-10, 0))),
	goalMenu(new GoalMenu(this, anarace, Rect(10, 10, getWidth()-100, 0))),
	forceMenu(new ForceMenu(this, anarace, Rect(10, 10, getWidth()-200, 0))),
	raceMenu(new RaceMenu(this, anarace, Rect(10, 10, getWidth()-200, 0))) // TODO
{	
	resetData();
	
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
	oldMarkedUnit = markedUnit;
	markedUnit = 0;

	int addUnit = 0;
	int addTime = 0;
	int addLocation = 0;
	int addCount = 0;
	
	int assignGoal = -1;
	int assignForce = -1;
	int assignRace = -1;

	currentGoalUnit = unitMenu->getMarkedUnit();

	if(saveGoalButton->isLeftClicked())
		settings.saveGoal(*anarace->getCurrentGoal());

	if(unitMenu->getPressedItem()>0)
	{
		addUnit = unitMenu->getPressedItem(); addCount = 1; addTime = 0; addLocation = 0;
		ostringstream os;
		os << "Added " << stats[anarace->getRace()][unitMenu->getPressedItem()].name << " to the goal list.";
		msgWindow->addMessage(os.str());
	}

	if(goalMenu->getPressedItem()>=0)
	{
		assignGoal = goalMenu->getPressedItem();
		menuRadio->forceUnpressAll();
		ostringstream os;
		os << "Set " << (*anarace->getCurrentGoal())->getName() << " as new goal list."; // TODO
		msgWindow->addMessage(os.str());
		menuButton[GOAL_MENU]->forceUnpress();
	}
	
	if(forceMenu->getPressedItem()>=0)
	{
		assignForce = forceMenu->getPressedItem();
		menuRadio->forceUnpressAll();
		menuButton[FORCE_MENU]->forceUnpress();
	}

	eRace racepressed = (eRace)(raceMenu->getPressedItem());
	if(racepressed >= 0)
	{
		assignRace = racepressed;
		ostringstream os;
		os << "Set " << (*anarace->getCurrentGoal())->getName() << " as new goal list."; // TODO
		msgWindow->addMessage(os.str()); 
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
	if(forceMenu->getHeight() > startLine)
		startLine = forceMenu->getHeight();
	unsigned int line = 1;
	
	anarace->countUnitsTotal();

	goals->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5)));
	legend->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5)));
	line++;

    std::list<ForceEntry*>::iterator forceEntry = forceList.begin();

	if(forceList.size() < (*anarace->getCurrentGoal())->goal.size())
	{
		ostringstream os;
		os << "DEBUG: " << (*anarace->getCurrentGoal())->goal.size() - forceList.size();
		toLog(os.str());
	}

	for(std::list<GOAL>::const_iterator i = (*anarace->getCurrentGoal())->goal.begin(); i!= (*anarace->getCurrentGoal())->goal.end(); ++i)
	{
	// TODO 2. Modus machen um Produktionsgebaeude ueber 'facility' zu bestimmen (fuer hotkeys geradezu praedestiniert)
		if(anarace->getLocationTotal(i->getLocation(), i->getUnit()) >= i->getCount())
			(*forceEntry)->setTargetForce(getClientRectWidth() / 3);
		else
			(*forceEntry)->setTargetForce(anarace->getLocationTotal(i->getLocation(), i->getUnit()) * getClientRectWidth() / (3*i->getCount()));
		(*forceEntry)->setTotalNumber(anarace->getLocationTotal(i->getLocation(), i->getUnit()));
				
		Rect edge = Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), 
						Size((*forceEntry)->getWidth(), FONT_SIZE+5));
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
	   			ostringstream os;
				addUnit = i->getUnit();	addCount = 1; addTime = i->getTime();	addLocation = i->getLocation();
   				os << "Added one " << stats[anarace->getRace()][i->getUnit()].name << " to the goal list.";        
				msgWindow->addMessage(os.str());
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
	    		ostringstream os;
				addUnit = i->getUnit();	addCount = -1; addTime = i->getTime();	addLocation = i->getLocation();
				os << "Removed one " << stats[anarace->getRace()][i->getUnit()].name << " from the goal list.";
				msgWindow->addMessage(os.str());
			}
		}
		line++;
		forceEntry++;
	} // end of for

	nongoals->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5)));
//  ---------- NON GOALS
	line++;
	for (unsigned int i = 0; i <= GAS_SCV; i++)
		if (((*anarace->getCurrentGoal())->getAllGoal(i) == 0) && ((*anarace->getCurrentGoal())->getIsBuildable(i)))// ( anarace->getLocationTotal(0,i)))
		{
			(*forceEntry)->setTargetForce(anarace->getLocationTotal(GLOBAL, i) * getClientRectWidth() / (3*anarace->getUnitsTotalMax()));
			(*forceEntry)->setTotalNumber(anarace->getLocationTotal(GLOBAL, i));
		
			Rect edge = Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), 
							Size((*forceEntry)->getWidth(), FONT_SIZE+5));
			(*forceEntry)->setFreeMove();
			(*forceEntry)->adjustRelativeRect(edge);
			

			if((*forceEntry)->isLeftClicked())
			{
				addUnit = i;	addCount = 1; addTime = 0;	addLocation = 0;
				ostringstream os;
				os << "Moved non-goal " << stats[anarace->getRace()][i].name << " to the goal list.";
				msgWindow->addMessage(os.str());
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

	Rect edge=Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5));
	fitItemToRelativeClientRect(edge, 1);
	
	if(addCount!=0)
	{
		(*anarace->getCurrentGoal())->addGoal(addUnit, addCount, addTime, addLocation);
		setChangedFlag();
	}

	if(assignGoal>=0)
	{
		settings.assignGoal(anarace->getPlayerNum(), goalMenu->getPressedItem());
		setChangedFlag();		
	}
	if(assignForce>=0)
	{
		settings.assignStartcondition(anarace->getPlayerNum(), forceMenu->getPressedItem());
		settings.fillGroups();
		setChangedFlag();
	}
	if(assignRace>=0)
	{
		settings.assignStartRace(anarace->getPlayerNum(), racepressed);
		settings.assignStartcondition(anarace->getPlayerNum(), 0);
		settings.fillGroups();
		settings.assignGoal(anarace->getPlayerNum(), 0);
		unitMenu->resetData();
		setChangedFlag();
	}

	if(getChangedFlag())
	{
		(*anarace->getCurrentGoal())->adjustGoals(true, (*anarace->getStartCondition())->getUnit(0) );
		resetData();
//		unitMenu->resetData(); CHANGED
	}
	
	
}

void ForceWindow::resetData()
{
	int size = forceList.size();
	int n = 0;
	std::list<ForceEntry*>::iterator j = forceList.begin();

    for(std::list<GOAL>::iterator i = (*anarace->getCurrentGoal())->goal.begin(); i!= (*anarace->getCurrentGoal())->goal.end(); ++i)
	{
		ForceEntry* forceEntry; 
		if(n>=size)
		{
			forceEntry = new ForceEntry(this, Rect(getRelativeClientRectPosition()+Point(0,0), Size(getClientRectWidth(), FONT_SIZE+5)),
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
		forceEntry->updateText("   " + stats[anarace->getRace()][i->getUnit()].name);
		forceEntry->assignGoal(&*i);

		n++;
	}

//	bool nonGoalList[GAS_SCV+1];
//	for(int l = 0;l<GAS_SCV+1;l++)
//		nonGoalList[l]=false;
// TODO die raussuchen die schon bestehen, unit vergleichen etc.		
	int k = 0;
    for (unsigned int i = 0; i <= GAS_SCV; i++)
        if (((*anarace->getCurrentGoal())->getAllGoal(i) == 0) && ((*anarace->getCurrentGoal())->getIsBuildable(i))) // (	anarace->getLocationTotal(0,i)))
//			nonGoalList[i]=true;
	    {
			k++;
        	ForceEntry* forceEntry;
    	    if(n>=size)
	        {
        	    forceEntry = new ForceEntry(this, Rect(getRelativeClientRectPosition()+Point(0,0), Size(getClientRectWidth(), FONT_SIZE+5)),
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
            forceEntry->updateText("   " + stats[anarace->getRace()][i].name);
            forceEntry->assignGoal(NULL);
			
			n++;
		}

		while(j!=forceList.end())
		{
			delete(*j);
			j = forceList.erase(j);
		}
			
	int z = forceList.size();
	int zz = (*anarace->getCurrentGoal())->goal.size(); 
	
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
					dc->DrawText(stats[anarace->getRace()][*i].name, Point(px+4- (s.GetWidth())/2, p.y + k*s2.GetHeight()+3));
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
    for(std::list<ForceEntry*>::const_iterator j = forceList.begin(); (!mouseType)&&(j!= forceList.end()); ++j)
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

	drawTechTree(dc);


}

