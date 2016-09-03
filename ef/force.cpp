#include "force.hpp"
#include "../ui/editfield.hpp"

ForceWindow::ForceWindow(UI_Object* force_parent, ANARACE* force_anarace, MessageWindow* force_msg_window, const unsigned int force_window_number) :
	UI_Window(force_parent, FORCE_WINDOW_TITLE_STRING, FORCE_WINDOW, force_window_number, SCROLLED),
	addUnit(0),
	addTime(0),
	addLocation(0),
	addCount(0),
	currentGoalUnit(0),
	msgWindow(force_msg_window),
	menuRadio(new UI_Radio(this, getRelativeClientRectPosition() + Point(0,10) )),
	saveGoalButton(new UI_Button(this, Rect(getRelativeClientRectPosition() + Size(0, 10), getClientRectSize()), Rect(Point(0,0), getSize()), SAVE_GOAL_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_RIGHT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE)),
	nongoalsText(new UI_StaticText(this, NON_GOALS_STRING, Rect(Point(0,20), getClientRectSize()), FORCE_TEXT_COLOR, SMALL_NORMAL_BOLD_FONT)),
	goalsText(new UI_StaticText(this, GOALS_STRING, Rect(Point(0,20), getClientRectSize()), FORCE_TEXT_COLOR, SMALL_NORMAL_BOLD_FONT)),
	legend(new UI_StaticText(this, TIME_LEGEND_STRING, Rect(Point(0,20), getClientRectSize()), FORCE_TEXT_COLOR, SMALL_NORMAL_BOLD_FONT, RIGHT_BOUNDED_TEXT_MODE)),
	goalForceList(),
	nongoalForceList(),
	markedUnit(0),
	oldMarkedUnit(0),
	anarace(force_anarace),
	unitMenu(new UnitMenu(this, anarace, Rect(10, 10, getWidth()-10, 0))),
	goalMenu(new GoalMenu(this, anarace, Rect(10, 10, getWidth()-100, 0))),
	raceMenu(new RaceMenu(this, Rect(10, 10, getWidth()-200, 0))), // TODO
	locationMenu(new LocationMenu(this, anarace, getRelativeClientRect())),
	startLine(0)
{	
	menuButton[RACE_MENU] = new UI_Button(this, Rect(Point(0, 0), getClientRectSize()), Rect(Point(0,0), getSize()), CHOOSE_RACE_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE_ONCE);
	menuButton[UNIT_MENU] = new UI_Button(this, Rect(Point(0, 0), getClientRectSize()), Rect(Point(0,0), getSize()), ADD_GOAL_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE_ONCE);
	menuButton[GOAL_MENU] = new UI_Button(this, Rect(Point(0, 0), getClientRectSize()), Rect(Point(0,0), getSize()), GOAL_LIST_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE_ONCE);
	menuButton[RACE_MENU]->updateToolTip(CHOOSE_RACE_TOOLTIP_STRING);
	menuButton[UNIT_MENU]->updateToolTip(ADD_GOALS_TOOLTIP_STRING);
	menuButton[GOAL_MENU]->updateToolTip(CHOOSE_GOALS_TOOLTIP_STRING);
	
	raceMenu->Hide();
	unitMenu->Hide();
	goalMenu->Hide();

	for(int i = 0; i<MAX_MENUS; i++)	
		menuRadio->addButton(menuButton[i]);
	menuRadio->calculateSameWidthOfButtons(true);


	for (unsigned int j = 0; j < 2; j++)	
		for (unsigned int loc = 0; loc < MAX_LOCATIONS; loc++)
		{
			locationName[j][loc] = new UI_StaticText(this, (*anarace->getMap())->getLocation(loc)->getName(), Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), FORCE_TEXT_COLOR, SMALL_NORMAL_BOLD_FONT);
			locationName[j][loc]->Hide();
		}
//	processList();
}

ForceWindow::~ForceWindow()
{
	for(std::list<ForceEntry*>::iterator i = nongoalForceList.begin(); i!= nongoalForceList.end(); ++i)
		delete (*i);
	for(std::list<ForceEntry*>::iterator i = goalForceList.begin(); i!= goalForceList.end(); ++i)
		delete (*i);
	
	delete nongoalsText;
	delete goalsText;
	delete legend;

	for(int i=MAX_MENUS;i--;)
		delete menuButton[i];
	for(int j = 0; j < 2; j++)
		for(int i=MAX_LOCATIONS;i--;)
			delete locationName[j][i];
	
	delete unitMenu;
	delete goalMenu;
	delete raceMenu; 
	
	delete menuRadio;
	delete locationMenu;
}

void ForceWindow::assignAnarace(ANARACE* force_anarace)
{
	anarace = force_anarace;
	unitMenu->assignAnarace(anarace);
	goalMenu->assignAnarace(anarace);
	locationMenu->assignAnarace(anarace);
}

void ForceWindow::closeMenus()
{
	unitMenu->close();
	goalMenu->close();
	raceMenu->close();
	locationMenu->close();
}

void ForceWindow::process()
{
	if(!isShown())
		return;
	ForceEntry::changed = NO_MESSAGE;
	
	int oldStartLine = 3;
	if(raceMenu->getHeight() > oldStartLine)
		oldStartLine = raceMenu->getHeight();
	if(unitMenu->getHeight()/2 > oldStartLine)
		oldStartLine = unitMenu->getHeight()/2;
	if(goalMenu->getHeight() > oldStartLine)
		oldStartLine = goalMenu->getHeight();

	UI_Window::process();

	oldMarkedUnit = markedUnit;
	markedUnit = 0;

	addUnit = 0;
	addTime = 0;
	addLocation = 0;
	addCount = 0;
	
	int assignGoal = -1;
	int assignRace = -1;

	if(ForceEntry::changed)
	{
		if(ForceEntry::forceEntryIsGoal)
		{
			if(ForceEntry::changed == LEFT_CLICKED)
			{
				bool allow = true;
				const std::list<unsigned int> errorList = (*anarace->getCurrentGoal())->allowAddition(ForceEntry::forceEntryUnit);
				if(errorList.size()>0)
				{
					for(std::list<unsigned int>::const_iterator blink = errorList.begin(); blink != errorList.end(); ++blink)
					{
						for(std::list<ForceEntry*>::iterator f = goalForceList.begin(); f != goalForceList.end(); ++f)
							if((*f)->getUnit() == (*blink))
							{
								(*f)->resetGradient();
								break;
							}
						for(std::list<ForceEntry*>::iterator f = nongoalForceList.begin(); f != nongoalForceList.end(); ++f)
							if((*f)->getUnit() == (*blink))
							{
								(*f)->resetGradient();
								break;
							}
					}					
					allow=false;
				}			
				if(allow)
				{
					addUnit = ForceEntry::forceEntryUnit;	addCount = 1  + ((stats[anarace->getRace()][addUnit].create>0)&&(stats[anarace->getRace()][addUnit].create==addUnit)); addTime = ForceEntry::forceEntryTime;	addLocation = ForceEntry::forceEntryLocation;
					msgWindow->addMessage(UI_Object::theme.lookUpFormattedString(ADDED_ONE_GOAL_STRING, *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+ForceEntry::forceEntryUnit+UNIT_NULL_STRING))));
					setChangedFlag();
				}
			}
		
			if(ForceEntry::changed == RIGHT_CLICKED)
			{
				bool allow=true;
				if((*anarace->getCurrentGoal())->getAllGoal(ForceEntry::forceEntryUnit)==1)
				{
					const std::list<unsigned int> errorList = (*anarace->getCurrentGoal())->allowDeletion(ForceEntry::forceEntryUnit);
					if(errorList.size()>0)
					{
						for(std::list<unsigned int>::const_iterator blink = errorList.begin(); blink != errorList.end(); ++blink)
						{
							for(std::list<ForceEntry*>::iterator f = goalForceList.begin(); f != goalForceList.end(); ++f)
								if((*f)->getUnit() == (*blink))
								{
									(*f)->resetGradient();
									break;
								}
							for(std::list<ForceEntry*>::iterator f = nongoalForceList.begin(); f != nongoalForceList.end(); ++f)
								if((*f)->getUnit() == (*blink))
								{
									(*f)->resetGradient();
									break;
								}
						}						
						allow=false;
					}
				}
				if(allow)
				{
					addUnit = ForceEntry::forceEntryUnit;	addCount = -1  - ((stats[anarace->getRace()][addUnit].create>0)&&(stats[anarace->getRace()][addUnit].create==addUnit)); addTime = ForceEntry::forceEntryTime;	addLocation = ForceEntry::forceEntryLocation;
					msgWindow->addMessage(UI_Object::theme.lookUpFormattedString(REMOVED_ONE_GOAL_STRING, *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+ForceEntry::forceEntryUnit+UNIT_NULL_STRING))));
					setChangedFlag();
				}
			}
		} else if(!ForceEntry::forceEntryIsGoal)
		{
			if(ForceEntry::changed == LEFT_CLICKED)
			{
				addUnit = ForceEntry::forceEntryUnit;	addCount = 1 + ((stats[anarace->getRace()][addUnit].create>0)&&(stats[anarace->getRace()][addUnit].create==addUnit)); addTime = 0;	addLocation = 0;
				msgWindow->addMessage(UI_Object::theme.lookUpFormattedString(MOVED_NON_GOAL_STRING, *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+ForceEntry::forceEntryUnit+UNIT_NULL_STRING))));
				setChangedFlag();
//				movedForceEntry=*forceEntry;
			} else 
			if(ForceEntry::changed == RIGHT_CLICKED)
			{} // do nothing
		}

	}

// TODO rechtsklick -> entfernen, auch im unitmenu!

// ------ TRANSLATE THE CHOSEN MENU ITEM
	currentGoalUnit = unitMenu->getMarkedItem()+1;
	if((!unitMenu->secondLevel())&&(currentGoalUnit==2))
		currentGoalUnit = GAS_SCV;

// ------ CREATE EDITBOX FOR SAVING BUILD ORDER
	if(saveGoalButton->isLeftClicked()&&(!UI_Object::editTextField))
		UI_Object::editTextField = new UI_EditField(getParent(), saveGoalButton, SAVE_GOALS_AS_STRING, GIVE_GOAL_A_NAME_STRING);

// ------ PROCESS TEXT BOX
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
				goalMenu->resetData();
				msgWindow->addMessage(UI_Object::theme.lookUpFormattedString(SAVED_GOAL_STRING, (*anarace->getCurrentGoal())->getName()));
			}
			delete UI_Object::editTextField;
			UI_Object::resetButton();
			UI_Object::editTextField=NULL;
		}
	}

// ------ CHECK FOR PRESSED UNIT ITEM
	if(unitMenu->getPressedItem()>=0)
	{
		bool allow = true;
		const std::list<unsigned int> errorList = (*anarace->getCurrentGoal())->allowAddition(unitMenu->getPressedItem());
		if(errorList.size()>0)
		{
			for(std::list<unsigned int>::const_iterator blink = errorList.begin(); blink != errorList.end(); ++blink)
			{
				for(std::list<ForceEntry*>::iterator f = goalForceList.begin(); f!=goalForceList.end(); ++f)
					if((*f)->getUnit() == (*blink))
					{
						(*f)->resetGradient();
						break;
					}
				for(std::list<ForceEntry*>::iterator f = nongoalForceList.begin(); f!=nongoalForceList.end(); ++f)
					if((*f)->getUnit() == (*blink))
					{
						(*f)->resetGradient();
						break;
					}
			}
			allow=false;
		}
		if(allow)
		{
			addUnit = unitMenu->getPressedItem(); addCount = 1 + ((stats[anarace->getRace()][addUnit].create>0)&&(stats[anarace->getRace()][addUnit].create==addUnit)); addTime = 0; addLocation = 0;
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
	
	if(raceMenu->getPressedItem()>=0)
	{
		assignRace = raceMenu->getPressedItem();
		msgWindow->addMessage(UI_Object::theme.lookUpFormattedString(SET_RACE_STRING, *theme.lookUpString((eString)(TERRA_STRING + assignRace)))); 
		menuButton[RACE_MENU]->forceUnpress();
	}

// ------ OPEN/CLOSE MENUES
	if(menuRadio->hasChanged())
	{
		switch(menuRadio->getMarked())
		{
			// TODO in ne eigene Klasse oder so
			case RACE_MENU:
			{
				raceMenu->Show();
				raceMenu->open();
			   	if(!raceMenu->isOpen()) 
				{
					menuRadio->forceUnpressAll();
					closeMenus();
				} else 
				{
					closeMenus();
					raceMenu->open();
				}
			}break;
		
			case UNIT_MENU:
			{
				unitMenu->Show();
				unitMenu->open();
				if(!unitMenu->isOpen()) 
				{
					menuRadio->forceUnpressAll();
					closeMenus();
				} else 
				{
					menuButton[UNIT_MENU]->forcePress();
					closeMenus();
					unitMenu->open();
				}
				unitMenu->process();
			}break;
			
			case GOAL_MENU:
			{
				goalMenu->Show();
				goalMenu->open();
				if(!goalMenu->isOpen()) 
				{
					menuRadio->forceUnpressAll();
					closeMenus();
				} else 
				{
					closeMenus();
					goalMenu->open();
				}
			}
			break;
			
			default:break;
		} // end of menuRadio has changed
		setChangedFlag();
	}

		
// ------ CLOSE MENUES IF MOUSE HAS LEFT FORCE WINDOW	
	if(!isMouseInside()) 
	{
		menuRadio->forceUnpressAll();
		closeMenus();
	}
	
	
// ------------------- GOALS ----------------------------

#if 0
// ------ START WHERE THE MENUES END
	unsigned int startLine = 3;
	if(raceMenu->getHeight() > startLine)
		startLine = raceMenu->getHeight();
	if(unitMenu->getHeight()/2 > startLine)
		startLine = unitMenu->getHeight()/2;
	if(goalMenu->getHeight() > startLine)
		startLine = goalMenu->getHeight();
	unsigned int line = 1;
	
	anarace->countUnitsTotal();
	goalsText->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5)));
	legend->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5)));
	line++;

	std::list<ForceEntry*>::iterator forceEntry = goalForceList.begin();
/*	unsigned int current_location = 99;
	for(int i = MAX_LOCATIONS;i--;)
		locationName[0][i]->Hide();*/
	bool menuIsOpen=false;
	for(std::list<GOAL>::const_iterator i = (*anarace->getCurrentGoal())->goal.begin(); i!= (*anarace->getCurrentGoal())->goal.end(); ++i)
	{
/*		if(i->getLocation()!=current_location)
		{
			current_location = i->getLocation();
			locationName[0][current_location]->Show();
			locationName[0][current_location]->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)-getScrollY()), Size(getClientRectWidth(), FONT_SIZE+5)));
			line++;
		}*/
	
	// TODO 2. Modus machen um Produktionsgebaeude ueber 'facility' zu bestimmen (fuer hotkeys geradezu praedestiniert)
		if(anarace->getLocationTotal(i->getLocation(), i->getUnit()) >= i->getCount())
			(*forceEntry)->setTargetForce(getClientRectWidth() / 3);
		else
			(*forceEntry)->setTargetForce(anarace->getLocationTotal(i->getLocation(), i->getUnit()) * getClientRectWidth() / (3*i->getCount()));
		(*forceEntry)->setTotalNumber(anarace->getLocationTotal(i->getLocation(), i->getUnit()));
				
		Rect edge = Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)-getScrollY()), Size((*forceEntry)->getWidth(), FONT_SIZE+6));
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
					for(std::list<ForceEntry*>::iterator f = goalForceList.begin(); f != goalForceList.end(); ++f)
						if((*f)->getUnit() == (*blink))
						{
							(*f)->resetGradient();
							break;
						}
					for(std::list<ForceEntry*>::iterator f = nongoalForceList.begin(); f != nongoalForceList.end(); ++f)
						if((*f)->getUnit() == (*blink))
						{
							(*f)->resetGradient();
							break;
						}
				}					
				allow=false;
			}			
			if(allow)
			{
				addUnit = i->getUnit();	addCount = 1  + ((stats[anarace->getRace()][addUnit].create>0)&&(stats[anarace->getRace()][addUnit].create==addUnit)); addTime = i->getTime();	addLocation = i->getLocation();
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
						for(std::list<ForceEntry*>::iterator f = goalForceList.begin(); f != goalForceList.end(); ++f)
							if((*f)->getUnit() == (*blink))
							{
								(*f)->resetGradient();
								break;
							}
						for(std::list<ForceEntry*>::iterator f = nongoalForceList.begin(); f != nongoalForceList.end(); ++f)
							if((*f)->getUnit() == (*blink))
							{
								(*f)->resetGradient();
								break;
							}
					}						
					allow=false;
				}
			}
			if(allow)
			{
				addUnit = i->getUnit();	addCount = -1  - ((stats[anarace->getRace()][addUnit].create>0)&&(stats[anarace->getRace()][addUnit].create==addUnit)); addTime = i->getTime();	addLocation = i->getLocation();
				msgWindow->addMessage(UI_Object::theme.lookUpFormattedString(REMOVED_ONE_GOAL_STRING, *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i->getUnit()+UNIT_NULL_STRING))));
			}
		}
/*		if((*forceEntry)->isLocationGoalClicked())
		{
			locationMenu->setParent(*forceEntry);
			locationMenu->open();
			menuIsOpen=true;
		}
	
		if((locationMenu->isOpen()) && (locationMenu->getParent() == *forceEntry))
			line+=locationMenu->getHeight();
		else*/
		line++;
		forceEntry++;
	} // end of for
//	if(!menuIsOpen)
//		locationMenu->close();
	nongoalsText->adjustRelativeRect(Rect(Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+6)));

//  ---------- NON GOALS
	line++;
//	bool checkedUnits[GAS_SCV+1];
//	for(int i=GAS_SCV+1;i--;)
//		checkedUnits[i]=false;
	int loc=0;
/*	for (unsigned int loc = 1; loc < MAX_LOCATIONS; loc++)
	{
		bool unitsthere = false;
		for (unsigned int i = GAS_SCV+1; i--;)
			if (((*anarace->getCurrentGoal())->getIsHaveable(i))&&(anarace->getLocationTotal(loc, i)))
				unitsthere=true;
		if(!unitsthere)
		{
			locationName[1][loc]->Hide();
			continue;
		}
		locationName[1][loc]->Show();
		locationName[1][loc]->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)-getScrollY()), Size(getClientRectWidth(), FONT_SIZE+5)));
		line++;*/

		forceEntry = nongoalForceList.begin();
		while(forceEntry!=nongoalForceList.end())
		{
//			if (((*anarace->getCurrentGoal())->getIsHaveable(i)))//&&(anarace->getLocationTotal(loc, i)))
			{
//				checkedUnits[i]=true;
				(*forceEntry)->setTargetForce(anarace->getLocationTotal(loc, (*forceEntry)->getUnit()) * getClientRectWidth() / (3*anarace->getNonGoalsUnitsTotalMax()));
				(*forceEntry)->setTotalNumber(anarace->getLocationTotal(loc, (*forceEntry)->getUnit()));
				Rect edge = Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)/*-getScrollY()*/), 
							Size((*forceEntry)->getWidth(), FONT_SIZE+6));
				(*forceEntry)->setFreeMove();
				(*forceEntry)->adjustRelativeRect(edge);

				if((*forceEntry)->isLeftClicked())
				{
					addUnit = (*forceEntry)->getUnit();	addCount = 1 + ((stats[anarace->getRace()][addUnit].create>0)&&(stats[anarace->getRace()][addUnit].create==addUnit)); addTime = 0;	addLocation = 0;
					msgWindow->addMessage(UI_Object::theme.lookUpFormattedString(MOVED_NON_GOAL_STRING, *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+(*forceEntry)->getUnit()+UNIT_NULL_STRING))));
//					movedForceEntry=*forceEntry;
				} else if((*forceEntry)->isRightClicked())
					{} // do nothing
				fitItemToRelativeClientRect(edge, 1);
				line++;
				forceEntry++;
			} // goal > 0*/
		}
	
/*		for (unsigned int i = GAS_SCV+1; i--;)
//			if (((*anarace->getCurrentGoal())->getAllGoal(i) == 0) && (((*anarace->getCurrentGoal())->getIsBuildable(i))|| ( anarace->getLocationTotal(0,i))))
			if (((*anarace->getCurrentGoal())->getIsHaveable(i)))//&&(anarace->getLocationTotal(loc, i)))
			{
				checkedUnits[i]=true;
				(*forceEntry)->setTargetForce(anarace->getLocationTotal(loc, i) * getClientRectWidth() / (3*anarace->getNonGoalsUnitsTotalMax()));
				(*forceEntry)->setTotalNumber(anarace->getLocationTotal(loc, i));
				Rect edge = Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)-getScrollY()), 
							Size((*forceEntry)->getWidth(), FONT_SIZE+6));
				(*forceEntry)->setFreeMove();
				(*forceEntry)->adjustRelativeRect(edge);

				if((*forceEntry)->isLeftClicked())
				{
					addUnit = i;	addCount = 1; addTime = 0;	addLocation = 0;
					msgWindow->addMessage(UI_Object::theme.lookUpFormattedString(MOVED_NON_GOAL_STRING, *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i+UNIT_NULL_STRING))));
					movedForceEntry=*forceEntry;
				}
				fitItemToRelativeClientRect(edge, 1);
				line++;
				forceEntry++;
			} // goal > 0*/
/*	}
	for(int loc=0;loc<1;loc++)
	{
		bool unitsthere = false;
		for (unsigned int i = GAS_SCV+1; i--;)
			if (((*anarace->getCurrentGoal())->getIsHaveable(i))&&(anarace->getLocationTotal(loc, i)))
				unitsthere=true;
		if(!unitsthere)
		{
			locationName[1][loc]->Hide();
			continue;
		}
		locationName[1][loc]->Show();
		locationName[1][loc]->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)-getScrollY()), Size(getClientRectWidth(), FONT_SIZE+5)))
;
		line++;

		for (unsigned int i = GAS_SCV+1; i--;)
//			if (((*anarace->getCurrentGoal())->getAllGoal(i) == 0) && (((*anarace->getCurrentGoal())->getIsBuildable(i))|| ( anarace->getLocationTotal(0,i))))
			if (((*anarace->getCurrentGoal())->getIsHaveable(i))&&(anarace->getLocationTotal(loc, i))&&(!checkedUnits[i]))
			{
				checkedUnits[i]=true;
				(*forceEntry)->setTargetForce(anarace->getLocationTotal(loc, i) * getClientRectWidth() / (3*anarace->getNonGoalsUnitsTotalMax()));
				(*forceEntry)->setTotalNumber(anarace->getLocationTotal(loc, i));
				Rect edge = Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)-getScrollY()),
				Size((*forceEntry)->getWidth(), FONT_SIZE+6));
				(*forceEntry)->setFreeMove();
				(*forceEntry)->adjustRelativeRect(edge);

				if((*forceEntry)->isLeftClicked())
				{
					addUnit = i;
					addCount = 1;
					addTime = 0;
					addLocation = 0;
					msgWindow->addMessage(UI_Object::theme.lookUpFormattedString(MOVED_NON_GOAL_STRING, *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i+UNIT_NULL_STRING))));
				}
				fitItemToRelativeClientRect(edge, 1);
				line++;
				forceEntry++;
			} // goal > 0
	}*/

	Rect edge=Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+6));
	fitItemToRelativeClientRect(edge, 1);
	
#endif
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
                std::list<ForceEntry*>::iterator a = goalForceList.begin();
                while(a!=goalForceList.end())
                {
			if(UI_Object::currentButton==*a)
                        	UI_Object::currentButton=NULL;
                        delete(*a);
                        a = goalForceList.erase(a);
                }
                a = nongoalForceList.begin();
                while(a!=nongoalForceList.end())
                {
			if(UI_Object::currentButton==*a)
                        	UI_Object::currentButton=NULL;
                        delete(*a);
                        a = nongoalForceList.erase(a);
                }
		setChangedFlag();		
	}
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
		std::list<ForceEntry*>::iterator a = goalForceList.begin();
		while(a!=goalForceList.end())
		{
                        if(UI_Object::currentButton==*a)
				UI_Object::currentButton=NULL;
			delete(*a);
			a = goalForceList.erase(a);
		}
		a = nongoalForceList.begin();
                while(a!=nongoalForceList.end())
                {
                        if(UI_Object::currentButton==*a)
				UI_Object::currentButton=NULL;
                        delete(*a);
                        a = nongoalForceList.erase(a);
                }		
		
		setResetFlag(); //!
	}

	startLine = 3;
	if(raceMenu->getHeight() > startLine)
		startLine = raceMenu->getHeight();
	if(unitMenu->getHeight()/2 > startLine)
		startLine = unitMenu->getHeight()/2;
	if(goalMenu->getHeight() > startLine)
		startLine = goalMenu->getHeight();

	if(unitMenu->secondLevel())
		menuButton[UNIT_MENU]->updateText(BACK_STRING);
	else if(unitMenu->isOpen())
		menuButton[UNIT_MENU]->updateText(CLOSE_STRING);
	else	
		menuButton[UNIT_MENU]->updateText(ADD_GOAL_STRING);

	if((startLine!=oldStartLine)||(getChangedFlag()))
	{
		(*anarace->getCurrentGoal())->adjustGoals(true, (*anarace->getStartCondition())->getUnit(0) );
		processList();
	}

}

void ForceWindow::reloadStrings()
{
	processList();
	UI_Object::reloadStrings();
}


void ForceWindow::processList()
{
	std::list<ForceEntry*>::iterator goal_entry = goalForceList.begin();

	for(std::list<GOAL>::iterator i = (*anarace->getCurrentGoal())->goal.begin(); i!= (*anarace->getCurrentGoal())->goal.end(); ++i)
        {
                if(goal_entry == goalForceList.end())
                {
                // Suche gleichen Nongoal Eintrag
                        std::list<ForceEntry*>::iterator nongoal_entry = nongoalForceList.begin();
                        while(nongoal_entry != nongoalForceList.end())
                        {
                                if((*nongoal_entry)->getUnit() == i->getUnit())
                                        break;
                                nongoal_entry++;
                        }
                        if(nongoal_entry == nongoalForceList.end()) // =>     Not found? => Insert new goal at the end
                        {
                                ForceEntry* forceEntry = new ForceEntry(this, Rect(getRelativeClientRectPosition()+Point(0,0), Size(getClientRectWidth(), FONT_SIZE+6)),
                                                                        Rect(getRelativeClientRectPosition(), getClientRectSize()+Size(0, getMaxRect().GetHeight())),"");
                                forceEntry->setUnit(i->getUnit());
                                forceEntry->setType(stats[anarace->getRace()][i->getUnit()].unitType);
                                forceEntry->updateText("   " + *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i->getUnit()+UNIT_NULL_STRING)));
                                forceEntry->assignGoal(&*i);
                                goalForceList.push_back(forceEntry);
			} //=> Found? => move
			else 
			{
				(*nongoal_entry)->assignGoal(&*i);
       				goalForceList.push_back(*nongoal_entry);
				nongoalForceList.erase(nongoal_entry);
			}
		} else 
                if((*goal_entry)->getUnit() != i->getUnit())
                {
                        std::list<ForceEntry*>::iterator k = goal_entry;
                        while(k != goalForceList.end())
                        {
                                if((*k)->getUnit() == i->getUnit())
                                        break;
                                k++;
                        }
                        if(k != goalForceList.end()) // => Found, move the entry
                        {
                                ForceEntry* old = *k;
                                goal_entry = goalForceList.insert(goal_entry, old);
                                goal_entry++;
                                goalForceList.erase(k);
                        } else // => not found, insert a new one
                        {
                                ForceEntry* forceEntry = new ForceEntry(this, Rect(getRelativeClientRectPosition()+Point(0,0), Size(getClientRectWidth(), FONT_SIZE+6)),
                                                                        Rect(getRelativeClientRectPosition(), getClientRectSize()+Size(0, getMaxRect().GetHeight())),"");
                                forceEntry->setUnit(i->getUnit());
                                forceEntry->setType(stats[anarace->getRace()][i->getUnit()].unitType);
                                forceEntry->updateText("   " + *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i->getUnit()+UNIT_NULL_STRING)));
                                forceEntry->assignGoal(&*i);
                                goal_entry = goalForceList.insert(goal_entry, forceEntry);
				goal_entry++;
                        }
                } else // ok
//              if((*entry)->getUnit() == (*order)->getUnit())
                {
			(*goal_entry)->updateText("   " + *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i->getUnit()+UNIT_NULL_STRING)));
                        goal_entry++;
                }
        }

	while(goal_entry != goalForceList.end())
	{
	// Suche gleichen Nongoal Eintrag
		if((*anarace->getCurrentGoal())->getIsBuildable((*goal_entry)->getUnit()))
		{
		// => is a non-goal! move it down!
			(*goal_entry)->assignGoal(NULL);
			nongoalForceList.push_front(*goal_entry);
			goal_entry = goalForceList.erase(goal_entry);
		} else
// not buildable? => delete
		{
			if(UI_Object::currentButton == *goal_entry)
				UI_Object::currentButton=NULL;
			delete(*goal_entry);
			goal_entry = goalForceList.erase(goal_entry);
		}
	}

	
	bool checkedUnits[GAS_SCV+1];
	for(int i=GAS_SCV+1;i--;)
		checkedUnits[i]=false;

	std::list<ForceEntry*>::iterator nongoal_entry = nongoalForceList.begin();

	while(nongoal_entry != nongoalForceList.end())
	{
		if((*anarace->getCurrentGoal())->getIsHaveable((*nongoal_entry)->getUnit()))
		{
			checkedUnits[(*nongoal_entry)->getUnit()] = true;
			(*nongoal_entry)->updateText("   " + *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+(*nongoal_entry)->getUnit()+UNIT_NULL_STRING)));
			nongoal_entry++;
		} else
		{
			if(UI_Object::currentButton == *nongoal_entry)
				UI_Object::currentButton = NULL;
			delete(*nongoal_entry);
			nongoal_entry = nongoalForceList.erase(nongoal_entry);
		}
	}


//	int goal_n = 0;
//	int nongoal_n = 0;
//	int goal_size = goalForceList.size();
//	int nongoal_size = nongoalForceList.size();
//	bool wasDeleted=false;
#if 0
	for(std::list<GOAL>::iterator i = (*anarace->getCurrentGoal())->goal.begin(); i!= (*anarace->getCurrentGoal())->goal.end(); ++i)
	{
		if(wasDeleted)
			i--;
		wasDeleted=false;

		if((goal_n >= goal_size) || ((*a)->getUnit() != i->getUnit()))
		{
		// Suche gleichen Nongoal Eintrag
			std::list<ForceEntry*>::iterator k = nongoalForceList.begin();
			while(k!=nongoalForceList.end())
			{
				if((*k)->getUnit()==i->getUnit())
					break;
				k++;
			}
			if(k==nongoalForceList.end()) // =>	Not found? => Insert new goal at the end
			{
				ForceEntry* forceEntry = new ForceEntry(this, Rect(getRelativeClientRectPosition()+Point(0,0), Size(getClientRectWidth(), FONT_SIZE+6)),
									Rect(getRelativeClientRectPosition(), getClientRectSize()+Size(0, getMaxRect().GetHeight())),"");
				forceEntry->setUnit(i->getUnit());
				forceEntry->setType(stats[anarace->getRace()][i->getUnit()].unitType);
				forceEntry->updateText("   " + *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i->getUnit()+UNIT_NULL_STRING)));
				forceEntry->assignGoal(&*i);
				goalForceList.push_back(forceEntry);
				goal_size++;
				goal_n++;
				if(goal_n<goal_size)
				{
					std::list<GOAL>::iterator l = i;
					while(l!=(*anarace->getCurrentGoal())->goal.end())
					{
						if(l->getUnit() == (*a)->getUnit())
							break;
						l++;
					}
					if(l == (*anarace->getCurrentGoal())->goal.end())
					// goal in goalforcelist does not fit to any goal in the goallist -> erase or move down!
					{
						if((*anarace->getCurrentGoal())->getIsBuildable((*a)->getUnit()))
						{
						// => is a non-goal! move it down!
							(*a)->assignGoal(NULL);
							nongoalForceList.push_front(*a);
							
							a = goalForceList.erase(a);
							goal_n++;
							goal_size--;
							nongoal_size++;
							wasDeleted=true;;
						} else
						{
							if(UI_Object::currentButton == *a) UI_Object::currentButton = NULL;
							delete(*a);
							a = goalForceList.erase(a);
							goal_size--;
							wasDeleted=true;
						}
					} 
				}
				//else wayne
			} else
			// found one, move it up
			{
				(*k)->assignGoal(&*i);
       				goalForceList.insert(a, *k);
				nongoalForceList.erase(k);
				goal_n++;
				goal_size++;
				nongoal_size--;
			}

		} else
//		gleicher Eitnrag gefunden - > ok
		if((*a)->getUnit()==i->getUnit()) // TODO TIME
		{
			a++; // ok, ignore
			goal_n++;
		} 
	} 
	
	while(a!=goalForceList.end())
	{
	// Suche gleichen Nongoal Eintrag
		if((*anarace->getCurrentGoal())->getIsBuildable((*a)->getUnit()))
		{
		// => is a non-goal! move it down!
			(*a)->assignGoal(NULL);
			nongoalForceList.push_front(*a);
			a = goalForceList.erase(a);
			goal_n++;
			goal_size--;
			nongoal_size++;
			wasDeleted=true;;
		} else
		{
			if(UI_Object::currentButton == *a)
				UI_Object::currentButton=NULL;
			delete(*a);
			a = goalForceList.erase(a);
			goal_size--;
			wasDeleted=true;
		}
	}
	
	bool checkedUnits[GAS_SCV+1];
	for(int i=GAS_SCV+1;i--;)
		checkedUnits[i]=false;

	int loc=0;
	std::list<ForceEntry*>::iterator b = nongoalForceList.begin();

/*	for (unsigned int loc = 1; loc < MAX_LOCATIONS; loc++)
	{
		bool unitsthere = false;
		for (unsigned int i = GAS_SCV+1; i--;)
			if (((*anarace->getCurrentGoal())->getIsHaveable(i))&&(anarace->getLocationTotal(loc, i)))
				unitsthere=true;
		if(!unitsthere)
		{
			locationName[1][loc]->Hide();
			continue;
		}	   */

		while(b!=nongoalForceList.end())
		{
			if((*anarace->getCurrentGoal())->getIsHaveable((*b)->getUnit()))
			{
				checkedUnits[(*b)->getUnit()]=true;
				nongoal_n++;
				b++;
			} else
			{
				if(UI_Object::currentButton == *b)
					UI_Object::currentButton=NULL;
				delete(*b);
				b = nongoalForceList.erase(b);
				nongoal_size--;
			}
		}
		
#endif
		for (unsigned int i = GAS_SCV+1; i--;)
//		if ( ((*anarace->getCurrentGoal())->getAllGoal(i) == 0) && (((*anarace->getCurrentGoal())->getIsBuildable(i))||(anarace->getLocationTotal(0,i))))
//	if ((*anarace->getCurrentGoal())->getIsHaveable(i)) 
			if (((*anarace->getCurrentGoal())->getIsHaveable(i))&&(!checkedUnits[i])) //&&(anarace->getLocationTotal(loc, i)))

//			nonGoalList[i]=true;
			{
				checkedUnits[i]=true;
				ForceEntry* forceEntry;
//				if(n>=size) // add new at the end
				{
					forceEntry = new ForceEntry(this, Rect(getRelativeClientRectPosition()+Point(0,0), Size(getClientRectWidth(), FONT_SIZE+6)),
					Rect(getRelativeClientRectPosition(), getClientRectSize()+Size(0, getMaxRect().GetHeight())), "");
					nongoalForceList.push_back(forceEntry);
				//	nongoal_size++;
					forceEntry->setUnit(i);
					forceEntry->setType(stats[anarace->getRace()][i].unitType);
					forceEntry->updateText("   " + *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i+UNIT_NULL_STRING)));
					forceEntry->assignGoal(NULL);
				//	nongoal_n++;
				} 
				/*else // adapt forceentry
				{
					forceEntry = *j;
					j++;
					forceEntry->setUnit(i);
					forceEntry->setType(stats[anarace->getRace()][i].unitType);
					forceEntry->updateText("   " + *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i+UNIT_NULL_STRING)));
					forceEntry->assignGoal(NULL);
					nongoal_n++;
				}*/
			}
/*	}
	for (unsigned int loc = 0; loc < 1; loc++)
	{
		bool unitsthere = false;
		for (unsigned int i = GAS_SCV+1; i--;)
			if (((*anarace->getCurrentGoal())->getIsHaveable(i))&&(anarace->getLocationTotal(loc, i)))
				unitsthere=true;
		if(!unitsthere)
		{
			locationName[1][loc]->Hide();
			continue;
		}	   

		for (unsigned int i = GAS_SCV+1; i--;)
//		if ( ((*anarace->getCurrentGoal())->getAllGoal(i) == 0) && (((*anarace->getCurrentGoal())->getIsBuildable(i))||(anarace->getLocationTotal(0,i))))
//	if ((*anarace->getCurrentGoal())->getIsHaveable(i)) 
			if (((*anarace->getCurrentGoal())->getIsHaveable(i))&&(anarace->getLocationTotal(loc, i))&&(!checkedUnits[i]))
//			nonGoalList[i]=true;
			{
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
	}*/

/*	while(a!=goalForceList.end())
	{
		delete(*a);
		a = goalForceList.erase(a);
		UI_Object::currentButton=NULL;
	}

	while(b!=nongoalForceList.end())
	{
		if(UI_Object::currentButton == *b)
			UI_Object::currentButton = NULL;
		delete(*b);
		b = nongoalForceList.erase(b);
	}*/
		
//	int z = forceList.size();
//	int zz = (*anarace->getCurrentGoal())->goal.size(); 
// ------------------- GOALS ----------------------------
// ------ START WHERE THE MENUES END
	unsigned int startLine = 3;
	if(raceMenu->getHeight() > startLine)
		startLine = raceMenu->getHeight();
	if(unitMenu->getHeight()/2 > startLine)
		startLine = unitMenu->getHeight()/2;
	if(goalMenu->getHeight() > startLine)
		startLine = goalMenu->getHeight();
	unsigned int line = 2;
	
	anarace->countUnitsTotal();
	goalsText->adjustRelativeRect(Rect(Point(7, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5)));
	legend->adjustRelativeRect(Rect(Point(7, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5)));
//	line++;

	std::list<ForceEntry*>::iterator forceEntry = goalForceList.begin();
/*	unsigned int current_location = 99;
	for(int i = MAX_LOCATIONS;i--;)
		locationName[0][i]->Hide();*/



		
	bool menuIsOpen=false;
	for(std::list<GOAL>::const_iterator i = (*anarace->getCurrentGoal())->goal.begin(); i!= (*anarace->getCurrentGoal())->goal.end(); ++i)
	{
/*		if(i->getLocation()!=current_location)
		{
			current_location = i->getLocation();
			locationName[0][current_location]->Show();
			locationName[0][current_location]->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)-getScrollY()), Size(getClientRectWidth(), FONT_SIZE+5)));
			line++;
		}*/
	
	// TODO 2. Modus machen um Produktionsgebaeude ueber 'facility' zu bestimmen (fuer hotkeys geradezu praedestiniert)
		if(anarace->getLocationTotal(i->getLocation(), i->getUnit()) >= i->getCount())
			(*forceEntry)->setTargetForce(getClientRectWidth() / 3);
		else
			(*forceEntry)->setTargetForce(anarace->getLocationTotal(i->getLocation(), i->getUnit()) * getClientRectWidth() / (3*i->getCount()));
		(*forceEntry)->setTotalNumber(anarace->getLocationTotal(i->getLocation(), i->getUnit()));
				
		Rect edge = Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)-getScrollY()), Size((*forceEntry)->getWidth(), FONT_SIZE+6));
		(*forceEntry)->setFreeMove();
		(*forceEntry)->adjustRelativeRect(edge);
	
/*		if((*forceEntry)->isLocationGoalClicked())
		{
			locationMenu->setParent(*forceEntry);
			locationMenu->open();
			menuIsOpen=true;
		}
	
		if((locationMenu->isOpen()) && (locationMenu->getParent() == *forceEntry))
			line+=locationMenu->getHeight();
		else*/
		line++;
		forceEntry++;
	} // end of for
//	if(!menuIsOpen)
//		locationMenu->close();
	nongoalsText->adjustRelativeRect(Rect(Point(7, (line+1) * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+6)));

//  ---------- NON GOALS
	line++;
//	bool checkedUnits[GAS_SCV+1];
//	for(int i=GAS_SCV+1;i--;)
//		checkedUnits[i]=false;
	int loc=0;
/*	for (unsigned int loc = 1; loc < MAX_LOCATIONS; loc++)
	{
		bool unitsthere = false;
		for (unsigned int i = GAS_SCV+1; i--;)
			if (((*anarace->getCurrentGoal())->getIsHaveable(i))&&(anarace->getLocationTotal(loc, i)))
				unitsthere=true;
		if(!unitsthere)
		{
			locationName[1][loc]->Hide();
			continue;
		}
		locationName[1][loc]->Show();
		locationName[1][loc]->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)-getScrollY()), Size(getClientRectWidth(), FONT_SIZE+5)));
		line++;*/

		forceEntry = nongoalForceList.begin();
		while(forceEntry!=nongoalForceList.end())
		{
//			if (((*anarace->getCurrentGoal())->getIsHaveable(i)))//&&(anarace->getLocationTotal(loc, i)))
			{
//				checkedUnits[i]=true;
				(*forceEntry)->setTargetForce(anarace->getLocationTotal(loc, (*forceEntry)->getUnit()) * getClientRectWidth() / (3*anarace->getNonGoalsUnitsTotalMax()));
				(*forceEntry)->setTotalNumber(anarace->getLocationTotal(loc, (*forceEntry)->getUnit()));
				Rect edge = Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)/*-getScrollY()*/), 
							Size((*forceEntry)->getWidth(), FONT_SIZE+6));
				(*forceEntry)->setFreeMove();
				(*forceEntry)->adjustRelativeRect(edge);

				fitItemToRelativeClientRect(edge, 1);
				line++;
				forceEntry++;
			} // goal > 0*/
		}
	
/*		for (unsigned int i = GAS_SCV+1; i--;)
//			if (((*anarace->getCurrentGoal())->getAllGoal(i) == 0) && (((*anarace->getCurrentGoal())->getIsBuildable(i))|| ( anarace->getLocationTotal(0,i))))
			if (((*anarace->getCurrentGoal())->getIsHaveable(i)))//&&(anarace->getLocationTotal(loc, i)))
			{
				checkedUnits[i]=true;
				(*forceEntry)->setTargetForce(anarace->getLocationTotal(loc, i) * getClientRectWidth() / (3*anarace->getNonGoalsUnitsTotalMax()));
				(*forceEntry)->setTotalNumber(anarace->getLocationTotal(loc, i));
				Rect edge = Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)-getScrollY()), 
							Size((*forceEntry)->getWidth(), FONT_SIZE+6));
				(*forceEntry)->setFreeMove();
				(*forceEntry)->adjustRelativeRect(edge);

				if((*forceEntry)->isLeftClicked())
				{
					addUnit = i;	addCount = 1; addTime = 0;	addLocation = 0;
					msgWindow->addMessage(UI_Object::theme.lookUpFormattedString(MOVED_NON_GOAL_STRING, *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i+UNIT_NULL_STRING))));
					movedForceEntry=*forceEntry;
				}
				fitItemToRelativeClientRect(edge, 1);
				line++;
				forceEntry++;
			} // goal > 0*/
/*	}
	for(int loc=0;loc<1;loc++)
	{
		bool unitsthere = false;
		for (unsigned int i = GAS_SCV+1; i--;)
			if (((*anarace->getCurrentGoal())->getIsHaveable(i))&&(anarace->getLocationTotal(loc, i)))
				unitsthere=true;
		if(!unitsthere)
		{
			locationName[1][loc]->Hide();
			continue;
		}
		locationName[1][loc]->Show();
		locationName[1][loc]->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)-getScrollY()), Size(getClientRectWidth(), FONT_SIZE+5)))
;
		line++;

		for (unsigned int i = GAS_SCV+1; i--;)
//			if (((*anarace->getCurrentGoal())->getAllGoal(i) == 0) && (((*anarace->getCurrentGoal())->getIsBuildable(i))|| ( anarace->getLocationTotal(0,i))))
			if (((*anarace->getCurrentGoal())->getIsHaveable(i))&&(anarace->getLocationTotal(loc, i))&&(!checkedUnits[i]))
			{
				checkedUnits[i]=true;
				(*forceEntry)->setTargetForce(anarace->getLocationTotal(loc, i) * getClientRectWidth() / (3*anarace->getNonGoalsUnitsTotalMax()));
				(*forceEntry)->setTotalNumber(anarace->getLocationTotal(loc, i));
				Rect edge = Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)-getScrollY()),
				Size((*forceEntry)->getWidth(), FONT_SIZE+6));
				(*forceEntry)->setFreeMove();
				(*forceEntry)->adjustRelativeRect(edge);

				if((*forceEntry)->isLeftClicked())
				{
					addUnit = i;
					addCount = 1;
					addTime = 0;
					addLocation = 0;
					msgWindow->addMessage(UI_Object::theme.lookUpFormattedString(MOVED_NON_GOAL_STRING, *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i+UNIT_NULL_STRING))));
				}
				fitItemToRelativeClientRect(edge, 1);
				line++;
				forceEntry++;
			} // goal > 0
	}*/

	Rect edge=Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+6));
	fitItemToRelativeClientRect(edge, 1);
	

	markedUnit = 0;
}

void ForceWindow::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Window::draw(dc);
	mouseType=0;
/*	for(std::list<ForceEntry*>::const_iterator j = forceList.begin(); (!mouseType)&&(j!= forceList.end()); ++j)
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
			break;
		}
	for(std::list<ForceEntry*>::const_iterator j = forceList.begin(); (!mouseType)&&(j!= forceList.end()); ++j)
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
			break;
		}*/
	
}

void ForceWindow::drawTechTree(DC* dc) const
{
	if(unitMenu->isOpen())
	{
		Point p(270, 550);
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

		// draw outer border:
		dc->SetPen(*theme.lookUpPen(OUTER_BORDER_PEN));
		dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
		dc->DrawEdgedRoundedRectangle(x1-8, p.y-8, (unsigned int)(x2 - x1 + 6), (unsigned int)(y2 - p.y + 16), 6);

		// draw inner border:
		dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
		dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
		dc->DrawEdgedRoundedRectangle(x1-5, p.y-5, (unsigned int)(x2 - x1), (unsigned int)(y2 - p.y + 10), 6);

		for(unsigned int k = 0; k<10; k++)
			if(tree.width[k]>0)
			{
				unsigned int x=0;
				for(std::list<unsigned int>::const_iterator i = tree.unit[k].begin(); i!=tree.unit[k].end();i++)
				{
					dc->SetBrush(*theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[anarace->getRace()][*i].unitType)));
					dc->SetPen(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_BOLD_PEN+stats[anarace->getRace()][*i].unitType)));
					
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
			
		dc->SetTextForeground(*UI_Object::theme.lookUpColor(BRIGHT_TEXT_COLOR));
		for(unsigned int k = 0; k<10; k++)
			if(tree.width[k]>0)
			{
				unsigned int x=0;
				int px=0;
				for(std::list<unsigned int>::const_iterator i = tree.unit[k].begin(); i!=tree.unit[k].end(); i++)
				{
					px=p.x + x*5*s2.GetWidth()/tree.width[k] + (5*s2.GetWidth()/tree.width[k])/2;
					dc->SetBrush(*theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[anarace->getRace()][*i].unitType)));
					dc->SetPen(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN+stats[anarace->getRace()][*i].unitType)));
					dc->DrawEdgedRoundedRectangle(Rect(Point(px - (s.GetWidth())/2, p.y + k*s2.GetHeight()), s), 4);
					string text = *UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+(*i)+UNIT_NULL_STRING));
					Size text_size = dc->GetTextExtent(text);
					dc->DrawText(text, Point(px+4- (text_size.GetWidth())/2, p.y + k*s2.GetHeight()+4));
					x++;
				}
			}
	}
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



