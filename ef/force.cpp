#include "force.hpp"
#include "savebox.hpp"
#include "../core/configuration.hpp"

ForceWindow::ForceWindow(UI_Object* force_parent, const unsigned int game_number, const unsigned int game_max, const unsigned int player_number, const unsigned int player_max) :
	UI_Window(force_parent, FORCE_WINDOW_TITLE_STRING, theme.lookUpPlayerRect(FORCE_WINDOW, game_number, game_max, player_number, player_max), theme.lookUpPlayerMaxHeight(FORCE_WINDOW, game_number, game_max, player_number, player_max), NOT_SCROLLED),
	currentGoalUnit(0),
	startLine(0),
	menuRadio(new UI_Radio(this, Rect(getRelativeClientRectPosition() + Point(5, 10), Size(0,0)) )),
	nongoalsText(new UI_StaticText(this, NON_GOALS_STRING, Rect(Point(0, 0), Size(0,0)), Size(0,0), FORCE_TEXT_COLOR, SMALL_BOLD_FONT, DO_NOT_ADJUST)),
	goalsText(new UI_StaticText(this, UI_Object::theme.lookUpString(GOALS_STRING), Rect(Point(0, 0), Size(0,0)), Size(0,0), FORCE_TEXT_COLOR, SMALL_BOLD_FONT, DO_NOT_ADJUST)),
	legend(new UI_StaticText(this, TIME_LEGEND_STRING, Rect(Point(200, 0), Size(0,0)), Size(0,0), FORCE_TEXT_COLOR, SMALL_FONT, DO_NOT_ADJUST)), // TODO
	goalForceList(),
	nongoalForceList(),
	markedUnit(0),
	oldMarkedUnit(0),
	anarace(NULL),
	
	unitMenu(new UnitMenu(this, Rect(10, 40, 100, 0), Size(0,0), DO_NOT_ADJUST)),
	goalMenu(new GoalMenu(this, Rect(10, 40, 100, 0), Size(0,0), DO_NOT_ADJUST)),
	saveBox(NULL),
//	locationMenu(new LocationMenu(this, anarace, getRelativeClientRect())),
	gameNumber(game_number),
	gameMax(game_max),
	playerNumber(player_number),
	playerMax(player_max)

{	
//	menuButton[UNIT_MENU] = new UI_Button(menuRadio, Rect(Point(0, 0), Size(50,0)), Size(0,0), ADD_GOAL_STRING, MY_BUTTON, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
	menuButton[UNIT_MENU] = new UI_Button(menuRadio, Rect(Point(5, 5), Size(21,20)), Size(5,5), ADD_BUTTON, true, STATIC_BUTTON_MODE, NULL_STRING, ARRANGE_TOP_LEFT);
//	menuButton[GOAL_MENU] = new UI_Button(menuRadio, Rect(Point(0, 0), Size(50,0)), Size(0,0), GOAL_LIST_STRING, MY_BUTTON, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH);
	menuButton[GOAL_MENU] = new UI_Button(menuRadio, Rect(Point(5, 5), Size(21,20)), Size(5,5), LOAD_BUTTON, true, STATIC_BUTTON_MODE, NULL_STRING, ARRANGE_TOP_LEFT);
	menuButton[SAVE_GOALS] = new UI_Button(menuRadio, Rect(Point(5,5), Size(21,20)), Size(5,5), SAVE_BUTTON, true, STATIC_BUTTON_MODE, NULL_STRING, ARRANGE_TOP_LEFT),
	
	menuButton[UNIT_MENU]->updateToolTip(ADD_GOALS_TOOLTIP_STRING);
	menuButton[GOAL_MENU]->updateToolTip(CHOOSE_GOALS_TOOLTIP_STRING);
	menuButton[SAVE_GOALS]->updateToolTip(SAVE_GOAL_TOOLTIP_STRING);

	unitMenu->Hide();
	goalMenu->Hide();
	legend->Hide();

//	for(unsigned int i = 0; i<MAX_FORCE_MENUS; ++i)	
	menuRadio->addButton(menuButton[UNIT_MENU], 0);
	menuRadio->addButton(menuButton[GOAL_MENU], 1);
	menuRadio->addButton(menuButton[SAVE_GOALS], 2);
	menuRadio->calculateBoxSize(HORIZONTAL_GROUP);
/*	for (unsigned int j = 0; j < 2; ++j)	
		for (unsigned int loc = 0; loc < (*anarace->getMap())->getMaxLocations(); ++loc)
		{
			locationName[j][loc] = new UI_StaticText(this, (*anarace->getMap())->getLocation(loc)->getName(), Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), FORCE_TEXT_COLOR, SMALL_BOLD_FONT);
			locationName[j][loc]->Hide();
		}*/
	addHelpButton(DESCRIPTION_UNIT_LIST_WINDOW_CHAPTER);
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
	delete saveBox;

	for(unsigned int i=MAX_FORCE_MENUS;i--;)
		delete menuButton[i];
//	for(int j = 0; j < 2; ++j)
//		for (unsigned int loc = 0; loc < (*anarace->getMap())->getMaxLocations(); ++loc)
//			delete locationName[j][loc];
	
	delete unitMenu;
	delete goalMenu;
	
	delete menuRadio;
//	delete locationMenu;
}

void ForceWindow::setMode(const unsigned int game_number, const unsigned int game_max, const unsigned int player_number, const unsigned int player_max)
{
	if((game_number == gameNumber) && (game_max == gameMax) && (player_number == playerNumber) && (player_max == playerMax))
		return;
	gameNumber = game_number;
	gameMax = game_max;
	playerNumber = player_number;
	playerMax = player_max;
}


void ForceWindow::reloadOriginalSize()
{
	setOriginalRect(UI_Object::theme.lookUpPlayerRect(FORCE_WINDOW, gameNumber, gameMax, playerNumber, playerMax));
	setMaxHeight(UI_Object::theme.lookUpPlayerMaxHeight(FORCE_WINDOW, gameNumber, gameMax, playerNumber, playerMax));

	UI_Window::reloadOriginalSize();
	setChangedFlag();
}

void ForceWindow::assignAnarace(ANABUILDORDER* force_anarace)
{
	anarace = force_anarace;
	unitMenu->assignAnarace(anarace);
	goalMenu->assignAnarace(anarace);
//	locationMenu->assignMap(anarace->getMap());
// resetData?
}

void ForceWindow::resetData()
{
	for(std::list<ForceEntry*>::iterator i = nongoalForceList.begin(); i!= nongoalForceList.end(); ++i)
		delete (*i);
	for(std::list<ForceEntry*>::iterator i = goalForceList.begin(); i!= goalForceList.end(); ++i)
		delete (*i);
	nongoalForceList.clear();
	goalForceList.clear();
	goalMenu->resetData();
//	locationMenu->assignMap(anarace->getMap());
//	locationMenu->resetData();
	processList();
}

void ForceWindow::closeMenus()
{
	unitMenu->close();
	goalMenu->close();
//	locationMenu->close();
}

void ForceWindow::process()
{
	if(!isShown())
		return;
	ForceEntry::changed = NO_MESSAGE;

	bool line_has_changed = false;
	
	signed int oldStartLine = -1;
	if(unitMenu->getHeight()/2 > oldStartLine)
		oldStartLine = unitMenu->getHeight()/2;
	if(goalMenu->getHeight() > oldStartLine)
		oldStartLine = goalMenu->getHeight();

	UI_Window::process();
	
	menuRadio->calculateBoxSize(HORIZONTAL_GROUP);

	oldMarkedUnit = markedUnit;
	markedUnit = 0;

	int assignGoal = -1;


	if(ForceEntry::changed)
	{
		if(ForceEntry::forceEntryIsGoal)
		{
			if(ForceEntry::changed == NUMBER_HAS_CHANGED)
			{
				const std::list<GOAL> error_list = anarace->getGoal()->tryToAddGoal(ForceEntry::forceEntryUnit, ForceEntry::forceEntryTime, ForceEntry::forceEntryLocation, ForceEntry::forceEntryCount);

				// TODO in addGoal rein
//				addCount = -1  - ((stats[anarace->getRace()][addUnit].create>0)&&(stats[anarace->getRace()][addUnit].create==addUnit)); 
				
				if(error_list.size()>0)
					for(std::list<GOAL>::const_iterator blink = error_list.begin(); blink != error_list.end(); ++blink)
					{
						UI_Object::theme.playSound(ERROR_SOUND, (getAbsolutePosition() + getSize()/2).x);
						for(std::list<ForceEntry*>::iterator f = goalForceList.begin(); f != goalForceList.end(); ++f)
							if(((*f)->getUnit() == blink->getUnit())&&((*f)->getTime() == blink->getTime()))//&&((*f)->getLocation() == blink->getLocation()))
							{
								(*f)->resetGradient();
								break;
							}
						for(std::list<ForceEntry*>::iterator f = nongoalForceList.begin(); f != nongoalForceList.end(); ++f)
							if((*f)->getUnit() == blink->getUnit())
							{
								(*f)->resetGradient();
								break;
							}
					}					
				else
				{
					if(ForceEntry::forceEntryCount >0)
						UI_Object::msgList.push_back(UI_Object::theme.lookUpFormattedString(ADDED_ONE_GOAL_STRING, UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+ForceEntry::forceEntryUnit))));
					else if(ForceEntry::forceEntryCount <0)
						UI_Object::msgList.push_back(UI_Object::theme.lookUpFormattedString(REMOVED_ONE_GOAL_STRING, UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+ForceEntry::forceEntryUnit))));
					setChangedFlag();
				}
			} else 
			if(ForceEntry::changed == GOAL_TIME_HAS_CHANGED)
			{
				setChangedFlag();
			}
			
		} else if((!ForceEntry::forceEntryIsGoal)&&(ForceEntry::changed == NUMBER_HAS_CHANGED))
		{
			anarace->getGoal()->tryToAddGoal(ForceEntry::forceEntryUnit, ForceEntry::forceEntryTime, ForceEntry::forceEntryLocation, ForceEntry::forceEntryCount);
			UI_Object::msgList.push_back(UI_Object::theme.lookUpFormattedString(MOVED_NON_GOAL_STRING, UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+ForceEntry::forceEntryUnit))));
			setChangedFlag();
		} 
	}

// TODO rechtsklick -> entfernen, auch im unitmenu!

// ------ TRANSLATE THE CHOSEN MENU ITEM
	currentGoalUnit = unitMenu->getMarkedItem()+1;
	if((!unitMenu->secondLevel())&&(currentGoalUnit==2))
		currentGoalUnit = GAS_SCV;


// ------ CHECK FOR PRESSED UNIT ITEM
	if(unitMenu->getPressedItem()>=0)
	{
		const std::list<GOAL> error_list = anarace->getGoal()->tryToAddGoal(unitMenu->getPressedItem(), 0, 0, 1);
		if(error_list.size()>0)
			for(std::list<GOAL>::const_iterator blink = error_list.begin(); blink != error_list.end(); ++blink)
			{
				UI_Object::theme.playSound(ERROR_SOUND, (getAbsolutePosition() + getSize()/2).x);
				for(std::list<ForceEntry*>::iterator f = goalForceList.begin(); f!=goalForceList.end(); ++f)
					if(((*f)->getUnit() == blink->getUnit())&&((*f)->getTime() == blink->getTime()))//&&((*f)->getLocation() == blink->getLocation()))
					{
						(*f)->resetGradient();
						break;
					}
				for(std::list<ForceEntry*>::iterator f = nongoalForceList.begin(); f!=nongoalForceList.end(); ++f)
					if((*f)->getUnit() == blink->getUnit())
					{
						(*f)->resetGradient();
						break;
					}
			}
		{
			UI_Object::msgList.push_back(UI_Object::theme.lookUpFormattedString(ADDED_GOAL_STRING, UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+unitMenu->getPressedItem()))));
			setChangedFlag();
		}
	}

	if(goalMenu->getPressedItem()>=0)
	{
		assignGoal = goalMenu->getPressedItem();
		menuRadio->forceUnpressAll();
		menuButton[GOAL_MENU]->forceUnpress();
	}
	
// ------ OPEN/CLOSE MENUES

	switch(menuRadio->getMarked())
	{
		case -1:break;
		case UNIT_MENU:
		{
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
		}break;
			
		case GOAL_MENU:
		{
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
// ------ CREATE EDITBOX FOR SAVING BUILD ORDER
		case SAVE_GOALS:
		{
			if(!saveBox)
				saveBox = new SaveBox(getParent(), SAVE_GOALS_AS_STRING, GIVE_GOAL_A_NAME_STRING);
		}
		break;			
		default:break;
	} // end of menuRadio has changed

// ------ PROCESS TEXT BOX
	if(saveBox)
	{
		if(saveBox->isCanceled())
		{
			delete saveBox;
			saveBox=0;
			menuButton[SAVE_GOALS]->forceUnpress();
			setNeedRedrawMoved();
				UI_Object::focus = NULL;
		} else
		if(saveBox->isDone())
		{
			if(saveBox->getString().length()>0)
			{
				UI_Object::focus = NULL;
				database.saveGoal(saveBox->getString(), anarace->getGoal());
				goalMenu->resetData();
				UI_Object::msgList.push_back(UI_Object::theme.lookUpFormattedString(SAVED_GOAL_STRING, anarace->getGoal()->getName()));
				delete saveBox;
				saveBox = NULL;
				menuButton[SAVE_GOALS]->forceUnpress();
				setNeedRedrawMoved();
			}
		}
	}


		
// ------ CLOSE MENUES IF MOUSE HAS LEFT FORCE WINDOW	
	if(!isMouseInside()) 
	{
		menuRadio->forceUnpressAll();
		closeMenus();
	}
	
	if(assignGoal>=0)
	{
		anarace->assignGoal(database.getGoal(anarace->getRace(), goalMenu->getPressedItem()));
		UI_Object::msgList.push_back(UI_Object::theme.lookUpFormattedString(SET_NEW_GOAL_LIST_STRING, anarace->getGoal()->getName()));
		std::list<ForceEntry*>::iterator a = goalForceList.begin();
		while(a!=goalForceList.end())
		{
			delete(*a);
			a = goalForceList.erase(a);
		}
		a = nongoalForceList.begin();
		while(a!=nongoalForceList.end())
		{
			delete(*a);
			a = nongoalForceList.erase(a);
		}
		setChangedFlag(); // TODO, evtl resetten irgendwie...
	}

	startLine = 0;
	if(unitMenu->getHeight()/2 > startLine)
		startLine = unitMenu->getHeight()/2;
	if(goalMenu->getHeight() > startLine)
		startLine = goalMenu->getHeight();


	if(unitMenu->menuHasChanged())
	{
		if(unitMenu->secondLevel())
			menuButton[UNIT_MENU]->setButtonColorsType(BACK_BUTTON);
//				updateText(BACK_STRING);
		else if(unitMenu->isOpen())
			menuButton[UNIT_MENU]->setButtonColorsType(DELETE_BUTTON);
//				updateText(CLOSE_STRING);
		else	
			menuButton[UNIT_MENU]->setButtonColorsType(ADD_BUTTON);
//				updateText(ADD_GOAL_STRING);
	}

	
	if((startLine!=oldStartLine)||(getChangedFlag()))
	{
		if(getChangedFlag())
			anarace->getGoal()->adjustGoals(true, (*anarace->getStartCondition())->getUnit(0) );
		processList();
	}

	if(unitMenu->isOpen())
	{
		techTreeWindow->assignAnarace(anarace);
		techTreeWindow->setCurrentGoalUnit(currentGoalUnit);
		techTreeWindow->Show();
	}
	else 
	{
		techTreeWindow->Hide();
	}
}

void ForceWindow::reloadStrings()
{
// TODO Problem wenn reloadStrings aufgerufen wird ohne dass je initialisiert wurde?
	processList();
	UI_Window::reloadStrings();
}


void ForceWindow::processList()
{
//	if(!isShown())
//		return;

//	filledHeight = 0; ? TODO
	if(anarace==NULL)
		return;
	std::list<ForceEntry*>::iterator goal_entry = goalForceList.begin();

	for(std::list<GOAL>::iterator i = anarace->getGoal()->goal.begin(); i!= anarace->getGoal()->goal.end(); ++i)
	{
		if(goal_entry == goalForceList.end())
		{
		// Suche gleichen Nongoal Eintrag
			std::list<ForceEntry*>::iterator nongoal_entry = nongoalForceList.begin();
			while(nongoal_entry != nongoalForceList.end())
			{
				if((*nongoal_entry)->getUnit() == i->getUnit())
					break;
				++nongoal_entry;
			}
			if(nongoal_entry == nongoalForceList.end()) // =>     Not found? => Insert new goal at the end
			{
				ForceEntry* forceEntry = new ForceEntry(this, Rect(getRelativeClientRectPosition() + Point(5, 15), Size(getClientRectWidth() - 15, FONT_SIZE + 6)), "null");
				forceEntry->setUnit(i->getUnit());
				forceEntry->setType(stats[anarace->getRace()][i->getUnit()].unitType);
				forceEntry->updateText("   " + UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i->getUnit())));
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
				++k;
			}
			if(k != goalForceList.end()) // => Found, move the entry
			{
				ForceEntry* old = *k;
				goal_entry = goalForceList.insert(goal_entry, old);
				++goal_entry;
				goalForceList.erase(k);
			} else // => not found, insert a new one
			{
				ForceEntry* forceEntry = new ForceEntry(this, Rect(getRelativeClientRectPosition()+Point(5, 15), Size(getClientRectWidth()-15, FONT_SIZE+6)), "null");
				forceEntry->setUnit(i->getUnit());
				forceEntry->setType(stats[anarace->getRace()][i->getUnit()].unitType);
				forceEntry->updateText("   " + UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i->getUnit())));
				forceEntry->assignGoal(&*i);
				goal_entry = goalForceList.insert(goal_entry, forceEntry);
				++goal_entry;
			}
		} else // ok
//	      if((*entry)->getUnit() == (*order)->getUnit())
		{
			(*goal_entry)->updateText("   " + UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i->getUnit())));
			
			++goal_entry;
		}
	}

	while(goal_entry != goalForceList.end())
	{
	// Suche gleichen Nongoal Eintrag
		if(anarace->getGoal()->getIsBuildable((*goal_entry)->getUnit()))
		{
		// => is a non-goal! move it down!
			(*goal_entry)->assignGoal(NULL);
			nongoalForceList.push_front(*goal_entry);
			goal_entry = goalForceList.erase(goal_entry);
		} else
// not buildable? => delete
		{
			delete(*goal_entry);
			goal_entry = goalForceList.erase(goal_entry);
		}
	}

	
	bool checkedUnits[GAS_SCV+1];
	for(unsigned int i=GAS_SCV+1;i--;)
		checkedUnits[i]=false;

	std::list<ForceEntry*>::iterator nongoal_entry = nongoalForceList.begin();

	while(nongoal_entry != nongoalForceList.end())
	{
		if(anarace->getGoal()->getIsHaveable((*nongoal_entry)->getUnit()))
		{
			checkedUnits[(*nongoal_entry)->getUnit()] = true;
			(*nongoal_entry)->updateText("   " + UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+(*nongoal_entry)->getUnit())));
			++nongoal_entry;
		} else
		{
			delete(*nongoal_entry);
			nongoal_entry = nongoalForceList.erase(nongoal_entry);
		}
	}


		for (unsigned int i = GAS_SCV+1; i--;)
//		if ( (!anarace->getGoal()->isGoal(i)) && ((anarace->getGoal()->getIsBuildable(i))||(anarace->getLocationTotal(0,i))))
//	if (anarace->getGoal()->getIsHaveable(i)) 
			if ((anarace->getGoal()->getIsHaveable(i))&&(!checkedUnits[i])) //&&(anarace->getLocationTotal(loc, i)))

//			nonGoalList[i]=true;
			{
				checkedUnits[i]=true;
				ForceEntry* forceEntry;
//				if(n>=size) // add new at the end
				{
					forceEntry = new ForceEntry(this, Rect(getRelativeClientRectPosition()+Point(5, 15), Size(getClientRectWidth() - 15, FONT_SIZE+6)), "null");
					nongoalForceList.push_back(forceEntry);
				//	++nongoal_size;
					forceEntry->setUnit(i);
					forceEntry->setType(stats[anarace->getRace()][i].unitType);
					forceEntry->updateText("   " + UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i)));
					forceEntry->assignGoal(NULL);
				//	++nongoal_n;
				} 
				/*else // adapt forceentry
				{
					forceEntry = *j;
					++j;
					forceEntry->setUnit(i);
					forceEntry->setType(stats[anarace->getRace()][i].unitType);
					forceEntry->updateText("   " + UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i)));
					forceEntry->assignGoal(NULL);
					++nongoal_n;
				}*/
			}
/*	}
	for (unsigned int loc = 0; loc < 1; ++loc)
	{
		bool unitsthere = false;
		for (unsigned int i = GAS_SCV+1; i--;)
			if ((anarace->getGoal()->getIsHaveable(i))&&(anarace->getLocationTotal(loc, i)))
				unitsthere=true;
		if(!unitsthere)
		{
			locationName[1][loc]->Hide();
			continue;
		}	   

		for (unsigned int i = GAS_SCV+1; i--;)
//		if ( (!anarace->getGoal()->isGoal(i)) && ((anarace->getGoal()->getIsBuildable(i))||(anarace->getLocationTotal(0,i))))
//	if (anarace->getGoal()->getIsHaveable(i)) 
			if ((anarace->getGoal()->getIsHaveable(i))&&(anarace->getLocationTotal(loc, i))&&(!checkedUnits[i]))
//			nonGoalList[i]=true;
			{
				ForceEntry* forceEntry;
				if(n>=size)
				{
					forceEntry = new ForceEntry(this, Rect(getRelativeClientRectPosition()+Point(0,0), Size(getClientRectWidth(), FONT_SIZE+6)),
					Rect(getRelativeClientRectPosition(), getClientRectSize()+Size(0, getMaxRect().GetHeight())), "");
					forceList.push_back(forceEntry);
					++size;
				} else
				{
					forceEntry = *j;
					++j;
				}
				forceEntry->setUnit(i);
				forceEntry->setType(stats[anarace->getRace()][i].unitType);
				forceEntry->updateText("   " + UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i)));
				forceEntry->assignGoal(NULL);
				++n;
			}
	}*/

/*	while(a!=goalForceList.end())
	{
		delete(*a);
		a = goalForceList.erase(a);
	}

	while(b!=nongoalForceList.end())
	{
		delete(*b);
		b = nongoalForceList.erase(b);
	}*/
		
//	int z = forceList.size();
//	int zz = anarace->getGoal()->goal.size(); 
// ------------------- GOALS ----------------------------
// ------ START WHERE THE MENUES END
	startLine = 0;
	if(unitMenu->getHeight()/2 > startLine)
		startLine = unitMenu->getHeight()/2;
	if(goalMenu->getHeight() > startLine)
		startLine = goalMenu->getHeight();
	unsigned int line = 0;
	
	anarace->countUnitsTotal();
	goalsText->updateText(anarace->getGoal()->getName());
	goalsText->adjustRelativeRect(Rect(Point(7, 45 + line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+6)));
//	legend->adjustRelativeRect(Rect(Point(100, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5)));
	++line;

	std::list<ForceEntry*>::iterator forceEntry = goalForceList.begin();
/*	unsigned int current_location = 99;
	for(int i = MAX_LOCATIONS;i--;)
		locationName[0][i]->Hide();*/



		
//	bool menuIsOpen=false;
	for(std::list<GOAL>::const_iterator i = anarace->getGoal()->goal.begin(); i!= anarace->getGoal()->goal.end(); ++i)
	{
/*		if(i->getLocation()!=current_location)
		{
			current_location = i->getLocation();
			locationName[0][current_location]->Show();
			locationName[0][current_location]->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5)));
			++line;
		}*/
	
	// TODO 2. Modus machen um Produktionsgebaeude ueber 'facility' zu bestimmen (fuer hotkeys geradezu praedestiniert)
		if(anarace->getLocationTotal(i->getLocation(), i->getUnit()) >= i->getCount())
			(*forceEntry)->setTargetForce(getClientRectWidth() /3);
		else
			(*forceEntry)->setTargetForce(anarace->getLocationTotal(i->getLocation(), i->getUnit()) * getClientRectWidth() / (3*i->getCount()));
		(*forceEntry)->setTotalNumber(anarace->getLocationTotal(i->getLocation(), i->getUnit()));
				
		Rect edge = Rect(getRelativeClientRectPosition() + Point(0, 45 + line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size((*forceEntry)->getWidth()/*getClientRectWidth()*/, FONT_SIZE+6));
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
		++line;
		++forceEntry;
	} // end of for
//	if(!menuIsOpen)
//		locationMenu->close();
	nongoalsText->adjustRelativeRect(Rect(Point(7, 45 + (line+1) * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+6)));

//  ---------- NON GOALS
	line+=2;
//	bool checkedUnits[GAS_SCV+1];
//	for(int i=GAS_SCV+1;i--;)
//		checkedUnits[i]=false;
	unsigned int loc=0;
/*	for (unsigned int loc = 1; loc < MAX_LOCATIONS; ++loc)
	{
		bool unitsthere = false;
		for (unsigned int i = GAS_SCV+1; i--;)
			if ((anarace->getGoal()->getIsHaveable(i))&&(anarace->getLocationTotal(loc, i)))
				unitsthere=true;
		if(!unitsthere)
		{
			locationName[1][loc]->Hide();
			continue;
		}
		locationName[1][loc]->Show();
		locationName[1][loc]->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5)));
		++line;*/

		forceEntry = nongoalForceList.begin();
		while(forceEntry!=nongoalForceList.end())
		{
//			if ((anarace->getGoal()->getIsHaveable(i)))//&&(anarace->getLocationTotal(loc, i)))
			{
//				checkedUnits[i]=true;
				(*forceEntry)->setTargetForce(anarace->getLocationTotal(loc, (*forceEntry)->getUnit()) * getClientRectWidth() / (3*anarace->getNonGoalsUnitsTotalMax()));
				(*forceEntry)->setTotalNumber(anarace->getLocationTotal(loc, (*forceEntry)->getUnit()));
				Rect edge = Rect(getRelativeClientRectPosition() + Point(0, 45 + line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), 
							Size((*forceEntry)->getWidth(), FONT_SIZE+6));
				(*forceEntry)->adjustRelativeRect(edge);

				fitItemToRelativeClientRect(edge, 1);
				++line;
				++forceEntry;
			} // goal > 0*/
		}
	
/*		for (unsigned int i = GAS_SCV+1; i--;)
//			if ((!anarace->getGoal()->isGoal(i)) && ((anarace->getGoal()->getIsBuildable(i))|| ( anarace->getLocationTotal(0,i))))
			if ((anarace->getGoal()->getIsHaveable(i)))//&&(anarace->getLocationTotal(loc, i)))
			{
				checkedUnits[i]=true;
				(*forceEntry)->setTargetForce(anarace->getLocationTotal(loc, i) * getClientRectWidth() / (3*anarace->getNonGoalsUnitsTotalMax()));
				(*forceEntry)->setTotalNumber(anarace->getLocationTotal(loc, i));
				Rect edge = Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), 
							Size((*forceEntry)->getWidth(), FONT_SIZE+6));
				(*forceEntry)->adjustRelativeRect(edge);

				if((*forceEntry)->isLeftClicked())
				{
					addUnit = i;	addCount = 1; addTime = 0;	addLocation = 0;
					msgWindow->addMessage(UI_Object::theme.lookUpFormattedString(MOVED_NON_GOAL_STRING, UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i))));
					movedForceEntry=*forceEntry;
				}
				fitItemToRelativeClientRect(edge, 1);
				++line;
				++forceEntry;
			} // goal > 0*/
/*	}
	for(int loc=0;loc<1;++loc)
	{
		bool unitsthere = false;
		for (unsigned int i = GAS_SCV+1; i--;)
			if ((anarace->getGoal()->getIsHaveable(i))&&(anarace->getLocationTotal(loc, i)))
				unitsthere=true;
		if(!unitsthere)
		{
			locationName[1][loc]->Hide();
			continue;
		}
		locationName[1][loc]->Show();
		locationName[1][loc]->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5)))
;
		++line;

		for (unsigned int i = GAS_SCV+1; i--;)
//			if ((!anarace->getGoal()->isGoal(i)) && ((anarace->getGoal()->getIsBuildable(i))|| ( anarace->getLocationTotal(0,i))))
			if ((anarace->getGoal()->getIsHaveable(i))&&(anarace->getLocationTotal(loc, i))&&(!checkedUnits[i]))
			{
				checkedUnits[i]=true;
				(*forceEntry)->setTargetForce(anarace->getLocationTotal(loc, i) * getClientRectWidth() / (3*anarace->getNonGoalsUnitsTotalMax()));
				(*forceEntry)->setTotalNumber(anarace->getLocationTotal(loc, i));
				Rect edge = Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)),
				Size((*forceEntry)->getWidth(), FONT_SIZE+6));
				(*forceEntry)->adjustRelativeRect(edge);

				if((*forceEntry)->isLeftClicked())
				{
					addUnit = i;
					addCount = 1;
					addTime = 0;
					addLocation = 0;
					msgWindow->addMessage(UI_Object::theme.lookUpFormattedString(MOVED_NON_GOAL_STRING, UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i))));
				}
				fitItemToRelativeClientRect(edge, 1);
				++line;
				++forceEntry;
			} // goal > 0
	}*/

	--line;
	Rect edge=Rect(getRelativeClientRectPosition() + Point(0, 45 + line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+6));
	fitItemToRelativeClientRect(edge, 1);
	

	markedUnit = 0;
}

void ForceWindow::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Window::draw(dc);
/*	mouseType=0;
	for(std::list<ForceEntry*>::const_iterator j = forceList.begin(); (!mouseType)&&(j!= forceList.end()); ++j)
		if((*j)->isCurrentlyHighlighted())
		{
			const std::list<unsigned int> subErrorList = anarace->getGoal()->allowDeletion((*j)->getUnit());
			const std::list<unsigned int> addErrorList = anarace->getGoal()->allowAddition((*j)->getUnit());
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
			const std::list<unsigned int> subErrorList = anarace->getGoal()->allowDeletion((*j)->getUnit());
			const std::list<unsigned int> addErrorList = anarace->getGoal()->allowAddition((*j)->getUnit());
			if((subErrorList.size()>0)&&(!addErrorList.size()))
				mouseType = 1;
			else if((!subErrorList.size())&&(!addErrorList.size()))
				mouseType = 3;
			else if((!subErrorList.size())&&(addErrorList.size()>0))
				mouseType = 2;
			break;
		}*/
	
}
/*
void ForceWindow::drawTechTree(DC* dc) const
{
	{
		Point p(270, 550);
		Size s(100, 15);
		Size s2(120, 25);

		int x1 = p.x;
		int x2 = p.x + 5*s2.GetWidth();
		int y2 = 10 * s2.GetHeight();

		const GOAL_TREE& tree = anarace->getGoalTree(currentGoalUnit);

		x1=9999;
		for(int k = 0; k<10; ++k)
			if(tree.width[k]>0)
			{
				bool foundOne = false;
				unsigned int x=0;
				int px=0;
				for(std::list<unsigned int>::const_iterator i = tree.unit[k].begin(); i!=tree.unit[k].end(); ++i)
				{
					px=p.x + x*5*s2.GetWidth()/tree.width[k] + (5*s2.GetWidth()/tree.width[k])/2;
					foundOne=true;
					if(x1 > px - (signed int)(s.GetWidth())/2)
						x1 = px - (s.GetWidth())/2;
					++x;
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

		for(unsigned int k = 0; k<10; ++k)
			if(tree.width[k]>0)
			{
				unsigned int x=0;
				for(std::list<unsigned int>::const_iterator i = tree.unit[k].begin(); i!=tree.unit[k].end(); ++i)
				{
					dc->SetBrush(*theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[anarace->getRace()][*i].unitType)));
					dc->SetPen(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_BOLD_PEN+stats[anarace->getRace()][*i].unitType)));
					
					for(std::list<unsigned int>::const_iterator j = tree.con[*i].begin(); j!= tree.con[*i].end(); ++j)
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
					++x;
				}
			}
			
		dc->SetTextForeground(*UI_Object::theme.lookUpColor(BRIGHT_TEXT_COLOR));
		for(unsigned int k = 0; k<10; ++k)
			if(tree.width[k]>0)
			{
				unsigned int x=0;
				int px=0;
				for(std::list<unsigned int>::const_iterator i = tree.unit[k].begin(); i!=tree.unit[k].end(); ++i)
				{
					px=p.x + x*5*s2.GetWidth()/tree.width[k] + (5*s2.GetWidth()/tree.width[k])/2;
					dc->SetBrush(*theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[anarace->getRace()][*i].unitType)));
					dc->SetPen(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN+stats[anarace->getRace()][*i].unitType)));
					dc->DrawEdgedRoundedRectangle(Rect(Point(px - (s.GetWidth())/2, p.y + k*s2.GetHeight()), s), 4);
					std::string text = UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+(*i)));
					Size text_size = dc->GetTextExtent(text);
					dc->DrawText(text, Point(px+4- (text_size.GetWidth())/2, p.y + k*s2.GetHeight()+4));
					++x;
				}
			}
	}
}
*/

TechTreeWindow* ForceWindow::techTreeWindow = NULL;

