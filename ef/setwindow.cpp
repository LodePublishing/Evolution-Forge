#include "setwindow.hpp"

SettingsWindow::SettingsWindow(UI_Object* parent):
	UI_Window(parent, SETTINGS_WINDOW_TITLE_STRING, SETTINGS_WINDOW, 0, SCROLLED)
{

/*	allowGoalAdaptionText = new UI_StaticText(this, SETTING_ALLOW_GOAL_ADAPTION_STRING, Rect(getRelativeClientRectPosition()+Point(0,95), Size(200, 15)), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR);
    allowGoalAdaptionText->updateToolTip(*theme.lookUpString(SETTING_ALLOW_GOAL_ADAPTION_TOOLTIP_STRING));
	preprocessText = new UI_StaticText(this, SETTING_PREPROCESS_BUILDORDER_STRING, Rect(getRelativeClientRectPosition()+Point(0,35), Size(200, 15)), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR);
    preprocessText->updateToolTip(*theme.lookUpString(SETTING_PREPROCESS_BUILDORDER_TOOLTIP_STRING));
	minimalistText = new UI_StaticText(this, SETTING_MINIMALIST_STRING, Rect(getRelativeClientRectPosition()+Point(250,20), Size(200, 15)), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR);
    minimalistText->updateToolTip(*theme.lookUpString(SETTING_MINIMALIST_TOOLTIP_STRING));
	fullText = new UI_StaticText(this, SETTING_FULL_STRING, Rect(getRelativeClientRectPosition()+Point(250,35), Size(200, 15)), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR);
    fullText->updateToolTip(*theme.lookUpString(SETTING_FULL_TOOLTIP_STRING));
	customText = new UI_StaticText(this, SETTING_CUSTOM_STRING, Rect(getRelativeClientRectPosition()+Point(250,50), Size(200, 15)), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR);
    customText->updateToolTip(*theme.lookUpString(SETTING_CUSTOM_TOOLTIP_STRING));
	staticFramerateText = new UI_StaticText(this, SETTING_STATIC_FRAMERATE_STRING, Rect(getRelativeClientRectPosition()+Point(250,95), Size(200, 15)), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR);
    staticFramerateText->updateToolTip(*theme.lookUpString(SETTING_STATIC_FRAMERATE_TOOLTIP_STRING));

	smoothMovementText = new UI_StaticText(this, SETTING_SMOOTH_MOVEMENT_STRING, Rect(getRelativeClientRectPosition()+Point(250,110), Size(200, 15)), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR);
    smoothMovementText->updateToolTip(*theme.lookUpString(SETTING_SMOOTH_MOVEMENT_TOOLTIP_STRING));
	
	glowingText = new UI_StaticText(this, SETTING_GLOWING_BUTTONS_STRING, Rect(getRelativeClientRectPosition()+Point(250, 125), Size(200, 15)), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR);

	backgroundBitmapText = new UI_StaticText(this, SETTING_BACKGROUND_BITMAP_STRING, Rect(getRelativeClientRectPosition()+Point(250, 140), Size(200, 15)), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR);

	transparencyText = new UI_StaticText(this, SETTING_TRANSPARENCY_STRING, Rect(getRelativeClientRectPosition()+Point(250, 155), Size(200, 15)), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR);
    transparencyText->updateToolTip(*theme.lookUpString(SETTING_TRANSPARENCY_TOOLTIP_STRING));

	resolutionText = new UI_StaticText(this, SETTING_RESOLUTION_STRING, Rect(getRelativeClientRectPosition()+Point(250,170), Size(200, 15)), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR);

	fullscreenText = new UI_StaticText(this, SETTING_FULLSCREEN_STRING, Rect(getRelativeClientRectPosition()+Point(250, 185), Size(200, 15)), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR);

	languageText = new UI_StaticText(this, SETTING_LANGUAGE_STRING, Rect(getRelativeClientRectPosition()+Point(250,200), Size(200, 15)), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR);


	maxTimeText = new UI_StaticText(this, SETTING_MAX_TIME_STRING, Rect(getRelativeClientRectPosition()+Point(0,20), Size(200, 15)), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR);
    maxTimeText->updateToolTip(*theme.lookUpString(SETTING_MAX_TIME_TOOLTIP_STRING));
	maxLengthText = new UI_StaticText(this, SETTING_MAX_LENGTH_STRING, Rect(getRelativeClientRectPosition()+Point(0,50), Size(200, 15)), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR);
    maxLengthText->updateToolTip(*theme.lookUpString(SETTING_MAX_LENGTH_TOOLTIP_STRING));
	maxRunsText = new UI_StaticText(this, SETTING_MAX_RUNS_STRING, Rect(getRelativeClientRectPosition()+Point(0,65), Size(200, 15)), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR);
    maxRunsText->updateToolTip(*theme.lookUpString(SETTING_MAX_RUNS_TOOLTIP_STRING));
	maxTimeOutText = new UI_StaticText(this, SETTING_MAX_TIMEOUT_STRING, Rect(getRelativeClientRectPosition()+Point(0,80), Size(200, 15)), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR);
    maxTimeOutText->updateToolTip(*theme.lookUpString(SETTING_MAX_TIMEOUT_TOOLTIP_STRING));
	breedFactorText = new UI_StaticText(this, SETTING_BREED_FACTOR_STRING, Rect(getRelativeClientRectPosition()+Point(0,110), Size(200, 15)), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR);
    breedFactorText->updateToolTip(*theme.lookUpString(SETTING_BREED_FACTOR_TOOLTIP_STRING));
	crossingOverText = new UI_StaticText(this, SETTING_CROSSING_OVER_STRING, Rect(getRelativeClientRectPosition()+Point(0,125), Size(200, 15)), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR);
    crossingOverText->updateToolTip(*theme.lookUpString(SETTING_CROSSING_OVER_TOOLTIP_STRING));
	framerateText = new UI_StaticText(this, SETTING_FRAMERATE_STRING, Rect(getRelativeClientRectPosition()+Point(250,65), Size(200, 15)), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR);
    framerateText->updateToolTip(*theme.lookUpString(SETTING_FRAMERATE_TOOLTIP_STRING));
	//dynamicFramerateText = new UI_StaticText(this, SETTING_DYNAMIC_FRAMERATE_STRING, Rect(getRelativeClientRectPosition()+Point(250,80), Size(200, 15)), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR);
//    dynamicFramerateText->updateToolTip(*theme.lookUpString(SETTING_DYNAMIC_FRAMERATE_TOOLTIP_STRING));

	
	languageMenu = new languageMenu(this);
	resolutionMenu = new resolutionMenu(this);

	englishButton = new UI_Button(this, Rect(getRelativeClientRectPosition()+Point(250,215), Size(200, 15)), Rect(getRelativeClientRectPosition()+Point(250,215), Size(200, 15)), SETTING_ENGLISH_STRING, SETTING_ENGLISH_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);

	germanButton = new UI_Button(this, Rect(getRelativeClientRectPosition()+Point(250,215), Size(200, 15)), Rect(getRelativeClientRectPosition()+Point(250,215), Size(200, 15)), SETTING_GERMAN_STRING, SETTING_GERMAN_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);

	languageRadio->addButton(englishButton);
	languageRadio->addButton(germanButton);*/
	
	
	//this->anarace = anarace;
	
	resetData();
/*
	for(int i=GAS_SCV+1;i--;)
	{
		forceEntry[i]=new ForceEntry(this, Rect(getRelativeClientRectPosition()+Point(0,0), Size(getClientRectWidth(),FONT_SIZE+5)), 
					Rect(getRelativeClientRectPosition(), getClientRectSize()+Size(0, getMaxRect().height)),  // max size -y?
						stats[anarace->getRace()][i].name);
		forceEntry[i]->Hide();
	}

	menuRadio = new UI_Radio(this);
	menuButton[RACE_MENU] = new UI_Button(this, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), Rect(Point(0,0),getSize()), CHOOSE_RACE_STRING, CHOOSE_RACE_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
	menuButton[UNIT_MENU] = new UI_Button(this, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), Rect(Point(0,0),getSize()), ADD_GOAL_STRING, ADD_GOAL_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
	menuButton[GOAL_MENU] = new UI_Button(this, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), Rect(Point(0,0),getSize()), GOAL_LIST_STRING, GOAL_LIST_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
	menuButton[FORCE_MENU] = new UI_Button(this, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), Rect(Point(0,0),getSize()), STARTFORCE_STRING, STARTFORCE_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP_LEFT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);

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
		unitTypeCount[i]=0;*/
}

SettingsWindow::~SettingsWindow()
{
	/*delete maxTimeText;
	delete preprocessText;
	delete maxLengthText;
	delete maxRunsText;
	delete maxTimeOutText;
	delete allowGoalAdaptionText;
	delete breedFactorText;
	delete crossingOverText;
	delete minimalistText;
	delete fullText;
	delete customText;
	delete framerateText;
	delete dynamicFramerateText;
	delete staticFramerateText;
	delete smoothMovementText;
	delete glowingText;
	delete backgroundBitmapText;
	delete transparencyText;
	delete resolutionText;
	delete fullscreenText;
	delete languageText;
	delete englishButton; 
	delete germanButton;
	delete languageRadio;*/
}

void SettingsWindow::process()
{
//	if(!shown)
//		return;
	UI_Window::process();
#if 0
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
//				settings.initSoup();
			};break;
			case ZERG:
			{
				settings.setStartRace(anarace->getPlayerNum(), ZERG); // race setzen NACH setMap, da setMap die default race von geladener Map laedt
				settings.setStartcondition(anarace->getPlayerNum(), ZERG);
				settings.fillGroups();
				settings.setGoal(anarace->getPlayerNum(), ZERG);
				setChangedFlag();					
//				settings.initSoup();	
			};break;
			default:break;
		}
	}

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
	{
		resetData();
		unitMenu->resetData();
	}
		
	
	if(!isMouseInside()) {
		menuRadio->forceUnpressAll();
		closeMenus();
	}

// ------------------- GOALS
	int startLine = raceMenu->getHeight();
	if(unitMenu->getHeight()/2 > startLine)
		startLine = unitMenu->getHeight()/2;
	if(goalMenu->getHeight() > startLine)
		startLine = goalMenu->getHeight();
	if(forceMenu->getHeight() > startLine)
		startLine = forceMenu->getHeight();

	int line = 1;
	
	anarace->countUnitsTotal();

	goals->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5)));
	line++;
// all units above GAS_SCV are pretty uninteresting (mostly temporary variables)
	for (int i = 0; i <= GAS_SCV; i++)
		if((*anarace->getCurrentGoal())->getAllGoal(i) > 0)
		{
			Rect edge = Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5));

			forceEntry[i]->setTargetForce(anarace->getLocationTotal(GLOBAL, i) * getClientRectWidth() / (3*anarace->getUnitsTotalMax()));
	
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

	nongoals->adjustRelativeRect(Rect(getRelativeClientRectPosition() + Point(0, line * (FONT_SIZE+7) + startLine*(FONT_SIZE+9)), Size(getClientRectWidth(), FONT_SIZE+5)));
//  ---------- NON GOALS
	line++;
	for (int i = 0; i <= GAS_SCV; i++)
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
		int d = unitTypeCount[i] * 360 / totalUnits;
		if(d!=targetUnitTypeCount[i])
		{
			startUnitTypeCount[i]=currentUnitTypeCount[i];
			targetUnitTypeCount[i]=d;
		}		
		move(currentUnitTypeCount[i], startUnitTypeCount[i], targetUnitTypeCount[i]);
	}

		#endif
}

void SettingsWindow::resetData()
{
//	currentUnitType = 0;
/*	for (int i = GAS_SCV+1; i--;)	// muss global in der Klasse sein und nicht lokal in drawGoalList, weil die Balken vergroessern/verkleinern sich ja 
	{
		if ((!anarace) || ((*anarace->getCurrentGoal())->allGoal[i] == 0))
			forceEntry[i]->oldForce = 0;
		else if ((anarace) && ((*anarace->getCurrentGoal())->allGoal[i]))
			forceEntry[i]->oldForce = anarace->getLocationTotal(0, i) * 75 / anarace->getUnitsTotalMax();
	}*/ //TODO
//	markedUnit = 0;
}

void SettingsWindow::draw(DC* dc) const
{
//	if(!shown)
//		return;
	UI_Window::draw(dc);
#if 0
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

	dc->SetTextForeground(*theme.lookUpColor(FORCE_TEXT_COLOR));
	dc->DrawText(*theme.lookUpString(STARTING_FORCE_STRING) , getAbsoluteClientRectPosition());

//		goalButton[i] = addButton(edge, "change race"); TODO tooltip
	line += 2;
	Rect edge;
/*
 * if((percent[i]/counter[i]<100)&&(percent[i]/counter[i]>0))
 * * {
 * * //			 dc->DrawText(_T(string::Format(T("[%i]"),anarace->getLocationTotal(0,i))),edge.GetPosition()+Point(edge.GetWidth()-80-oldForceList[i],0));
 * * dc->DrawText(_T(string::Format(T("%i%%"),percent[i]/counter[i])),edge.GetPosition()+Point(edge.GetWidth()-120-oldForceList[i],0));
 * * unsigned int dx,dy;
 * * string bla=_T(string::Format(T("%2i/%2i"),anarace->getLocationTotal(0,i),(*anarace->getCurrentGoal())->allGoal[i]));
 * * dc->GetTextExtent(bla,&dx,&dy);
 * * dc->DrawText(bla,edge.GetPosition()+Point(edge.GetWidth()-45-dx,0));
 * * 
 * * } else
 * * {
 * * unsigned int dx,dy;
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

	int grad=0;
	for(int i=UNIT_TYPE_TYPES;i--;)
	{
		int x = getAbsolutePosition().x+getWidth()-40;
		int y = getAbsolutePosition().y+40;
	
//		pieColor(dc->GetSurface(), x, y, 30, grad, grad+d, (Uint32)(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN+i))->GetColor()));
//		filledpieColor(dc->GetSurface(), x, y, 30, grad, grad+currentUnitTypeCount[i], (Uint32)(*theme.lookUpBrush((eBrush)(BRIGHT_UNIT_TYPE_0_BRUSH+i))->GetColor()));
		grad+=currentUnitTypeCount[i];
	}
	#endif
}

