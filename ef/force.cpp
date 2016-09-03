#include "force.hpp"

ForceEntry::ForceEntry(UI_Object* parent, Rect rect, Rect maxRect, string unit):UI_Button(parent, rect, maxRect, unit, unit, FORCE_ENTRY_BUTTON, NO_TEXT_MODE, NO_BUTTON_MODE, DO_NOT_ADJUST, SMALL_NORMAL_BOLD_FONT, NO_AUTO_SIZE)
	// TODO!
{
	addUnit = new UI_Button(this, Rect(Point(getWidth()-117,2),Size(8,8)), Rect(Point(0,0), getSize()), ADD_BUTTON, PRESS_BUTTON_MODE);
	subUnit = new UI_Button(this, Rect(Point(getWidth()-107,2),Size(8,8)), Rect(Point(0,0), getSize()), SUB_BUTTON, PRESS_BUTTON_MODE);
	cancelUnit = new UI_Button(this, Rect(Point(getWidth()-97,2),Size(8,8)), Rect(Point(0,0), getSize()), CANCEL_BUTTON, PRESS_BUTTON_MODE);
	addUnit->updateToolTip("add a unit");
	subUnit->updateToolTip("remove a unit");
	cancelUnit->updateToolTip("remove a goal");
};

ForceEntry::~ForceEntry()
{
	delete(addUnit);
	delete(subUnit);
	delete(cancelUnit);
};

void ForceEntry::process()
{
	if(!shown)
		return;
	UI_Button::process();
};

void ForceEntry::draw(DC* dc) const
{
	if(!shown) return;
	UI_Button::draw(dc);
	if (oldForce)
	{
	//TODO: Was ist mit Einheiten die zwar als force da sind, aber nicht buildable ( = kein Eintrag in phaenoToGenoType) sind?
			dc->SetBrush(*theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+type)));
			dc->SetPen(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN+type)));
					
	//                                      if(isShown()==1)
			dc->DrawRoundedRectangle(Rect(getAbsolutePosition()+Point(getWidth()-oldForce, 0), Size(oldForce+1,FONT_SIZE+5)), 3);
	//                                      else if(isShown()==2)
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
		} // end oldForceList >0

};

const int ForceEntry::changed() const
{
	if(addUnit->isPressed()) return(1);
	if(subUnit->isPressed()) return(2);
	if(cancelUnit->isPressed()) return(3);
	return(0);
};

void ForceEntry::updateText(string utext)
{
	updateNormalText(utext);
	updatePressedText(utext);
};

ForceWindow::ForceWindow(UI_Object* parent, ANARACE* anarace, const int windowNumber):UI_Window(parent, FORCE_WINDOW_TITLE_STRING, FORCE_WINDOW, windowNumber, SCROLLED)
{
	goalButton[0] = new UI_Button(this, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), Rect(Point(0,0),getSize()), USE_MAP_SETTINGS_STRING, USE_MAP_SETTINGS_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
	goalButton[1] = new UI_Button(this, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), Rect(Point(0,0),getSize()), TERRA_STRING, TERRA_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
	goalButton[2] = new UI_Button(this, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), Rect(Point(0,0),getSize()), PROTOSS_STRING, PROTOSS_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
	goalButton[3] = new UI_Button(this, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), Rect(Point(0,0),getSize()), ZERG_STRING, ZERG_STRING, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, STATIC_BUTTON_MODE, ARRANGE_TOP, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);

	goalButton[1]->updateToolTip("Choose race terra");
	goalButton[2]->updateToolTip("Choose race protoss");
	goalButton[3]->updateToolTip("Choose race zerg");
	
	nongoals = new UI_StaticText(this, NON_GOALS_STRING, Rect(getRelativeClientRectPosition()+Point(0,20), getClientRectSize()), HORIZONTALLY_CENTERED_TEXT_MODE, FORCE_TEXT_COLOUR);
			
	radio = new UI_Radio(this);
	for(int i=0;i<4;i++)
		radio->addButton(goalButton[i]);
	goalButton[2]->forcePressed();

//	goalFileListOpened = 0;
//	goalListOpened = 0;
	this->anarace = anarace;
	
	resetData();

	for(int i=GAS_SCV+1;i--;)
	{
		forceEntry[i]=new ForceEntry(this, Rect(getRelativeClientRectPosition()+Point(-100,200), Size(getClientRectWidth(),FONT_SIZE+5)), 
				Rect(getRelativeClientRectPosition(), getClientRectSize()+Size(0, getMaxRect().height)),  // max size -y?
						stats[(*anarace->getStartCondition())->getRace()][i].name);
		forceEntry[i]->Hide();
	};
	unitMenu=new UnitMenu(this, anarace, Rect(10, 50, 0, 0));
};

ForceWindow::~ForceWindow()
{
	for(int i=4;i--;)
		delete goalButton[i];
	for(int i=GAS_SCV+1;i--;)
		delete forceEntry[i];
	delete radio;
	delete unitMenu;
};

const int ForceWindow::getMarkedUnit() const
{
	return (markedUnit);
};

void ForceWindow::process()
{
	if(!shown)
		return;
	UI_Window::process();

	int a = unitMenu->pressedUnit;
	if(a>0)
	{
		(*anarace->getCurrentGoal())->addGoal(a, 1, 0, 0);
		setChangedFlag();
	};
	
	currentUnitType = 0;
	markedUnit = 0;
	if(radio->hasChanged())
	{
            switch (radio->getMarked())
            {
              case 0:
//                  settings.setMap(USE_MAP_SETTINGS);
                  break;
              case (TERRA + 1):
              {
                  settings.setStartRace(anarace->getPlayerNum(), TERRA);
				  settings.setStartcondition(anarace->getPlayerNum(), TERRA);
				  settings.fillGroups();
				  settings.setGoal(anarace->getPlayerNum(), TERRA);
				setChangedFlag();					
//				  settings.initSoup();
				  /// TODO hier wird mehrfach adjustgoals aufgerufen!!
              };break;
              case (PROTOSS + 1):
              {
                  settings.setStartRace(anarace->getPlayerNum(), PROTOSS);
				  settings.setStartcondition(anarace->getPlayerNum(), PROTOSS);
				  settings.fillGroups();
				  settings.setGoal(anarace->getPlayerNum(), PROTOSS);
				setChangedFlag();					
//				  settings.initSoup();
              };break;
              case (ZERG + 1):
              {
                  settings.setStartRace(anarace->getPlayerNum(), ZERG); // race setzen NACH setMap, da setMap die default race von geladener Map laedt
				  settings.setStartcondition(anarace->getPlayerNum(), ZERG);
				  settings.fillGroups();
				  settings.setGoal(anarace->getPlayerNum(), ZERG);
				setChangedFlag();					
//				  settings.initSoup();
              };break;
            }
	} // race darf nur indirekt geaendert werden!

	if(getChangedFlag())
        resetData();
	
//	if (!Rect(getAbsolutePosition(), getSize()).Inside(controls.getCurrentPosition()))
//	{
//		goalListOpened = 0;
//		goalFileListOpened = 0;
//	}

// ------------------- GOALS
	int line = 2+unitMenu->getHeight();
// all units above GAS_SCV are pretty uninteresting (mostly temporary variables)
	for (int i = 0; i <= GAS_SCV; i++)
		if((*anarace->getCurrentGoal())->allGoal[i] > 0)
		{
			Rect edge = Rect(getRelativeClientRectPosition() + Point(0, line*(FONT_SIZE+6)), Size(getClientRectWidth(), FONT_SIZE+5));

			if (forceEntry[i]->oldForce < anarace->getLocationTotal(0, i) * 75 / anarace->getUnitsTotalMax())
				forceEntry[i]->oldForce += (anarace->getLocationTotal(0, i) * 75 / anarace->getUnitsTotalMax() /*- oldForceList[i]*/) / 5 + 1;
			else if (forceEntry[i]->oldForce > anarace->getLocationTotal(0, i) * 75 / anarace->getUnitsTotalMax())
				forceEntry[i]->oldForce -= (forceEntry[i]->oldForce - anarace->getLocationTotal(0, i) * 75 / anarace->getUnitsTotalMax()) / 5 + 1;
			if(forceEntry[i]->oldForce < 20) forceEntry[i]->oldForce=20;
			forceEntry[i]->type=stats[(*anarace->getStartCondition())->getRace()][i].unitType;
			forceEntry[i]->total=anarace->getLocationTotal(0, i);
			forceEntry[i]->goal=(*anarace->getCurrentGoal())->allGoal[i];

			if (fitItemToRelativeClientRect(edge, 1)) //clientRect veraendert sich net ;/
			{
				forceEntry[i]->Show();
				if (forceEntry[i]->isCurrentlyHighlighted())
					markedUnit = i;
				ostringstream os;
//				os << anarace->getLocationTotal(0, i) << " " << stats[(*anarace->getStartCondition())->getRace()][i].name;
//				if (anarace->getLocationTotal(0, i) == 1);
//				else os << "s";
//				forceEntry[i]->updateText(os.str());
				forceEntry[i]->updateText("   " + stats[(*anarace->getStartCondition())->getRace()][i].name);
				forceEntry[i]->adjustRelativeRect(edge);
				switch(forceEntry[i]->changed())
				{
					case 0:break;
					case 1:(*anarace->getCurrentGoal())->addGoal(i, 1, 0, 0);break;  //<- Bei (+) kann sich nichts an den goals veraendern!
					case 2:(*anarace->getCurrentGoal())->addGoal(i, -1, 0, 0);
							setChangedFlag();break;           // <- Bei (-) sehr wohl
					case 3:(*anarace->getCurrentGoal())->addGoal(i,-(*anarace->getCurrentGoal())->globalGoal[0][i], 0, 0);
					        setChangedFlag();break;           // <- und auch bei X natuerlich
				};
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
        if (((*anarace->getCurrentGoal())->allGoal[i] == 0) && ((*anarace->getCurrentGoal())->isBuildable[i]))
        {
            Rect edge = Rect(getRelativeClientRectPosition() + Point(0, line*(FONT_SIZE+6)), Size(getClientRectWidth(), FONT_SIZE+5));
                                                                                                                                                            
            if (forceEntry[i]->oldForce < anarace->getLocationTotal(0, i) * 75 / anarace->getUnitsTotalMax())
                forceEntry[i]->oldForce += (anarace->getLocationTotal(0, i) * 75 / anarace->getUnitsTotalMax()/* - oldForceList[i]*/) / 5 + 1;
            else if (forceEntry[i]->oldForce > anarace->getLocationTotal(0, i) * 75 / anarace->getUnitsTotalMax())
                forceEntry[i]->oldForce -= (forceEntry[i]->oldForce - anarace->getLocationTotal(0, i) * 75 / anarace->getUnitsTotalMax()) / 5 + 1;
            forceEntry[i]->type=stats[(*anarace->getStartCondition())->getRace()][i].unitType;
            forceEntry[i]->total=anarace->getLocationTotal(0, i);
            forceEntry[i]->goal=(*anarace->getCurrentGoal())->allGoal[i];
                                                                                                                                                            
            if (fitItemToRelativeClientRect(edge, 1)) //clientRect veraendert sich net ;/
            {
                forceEntry[i]->Show();
                if (forceEntry[i]->isCurrentlyHighlighted())
                    markedUnit = i;
                ostringstream os;
                //os << anarace->getLocationTotal(0, i) << " " << stats[(*anarace->getStartCondition())->getRace()][i].name;
                //if (anarace->getLocationTotal(0, i) == 1);
                //else os << "s";
                //forceEntry[i]->updateText(os.str());
                forceEntry[i]->updateText("   " + stats[(*anarace->getStartCondition())->getRace()][i].name);
                forceEntry[i]->adjustRelativeRect(edge);
                switch(forceEntry[i]->changed())
                {
                    case 0:break;
                    case 1:(*anarace->getCurrentGoal())->addGoal(i, 1, 0, 0);break;  //<- Bei (+) kann sich nichts an den goals veraendern!
                    case 2:(*anarace->getCurrentGoal())->addGoal(i, -1, 0, 0);
                            setChangedFlag();break;           // <- Bei (-) sehr wohl
                    case 3:(*anarace->getCurrentGoal())->addGoal(i,-(*anarace->getCurrentGoal())->globalGoal[0][i], 0, 0);
                            setChangedFlag();break;           // <- und auch bei X natuerlich
                };
            }
         /*   else
            {
                forceEntry[i]->Hide();
                forceEntry[i]->adjustRelativeRect(Rect(getRelativeClientRectPosition()+Point(-100,200), Size(getClientRectWidth(),FONT_SIZE+10)));
            }*/
            line++;
        } // goal > 0
		else if((*anarace->getCurrentGoal())->allGoal[i] == 0)
		{
    	    forceEntry[i]->Hide();
        //	forceEntry[i]->adjustRelativeRect(Rect(getRelativeClientRectPosition()+Point(-100,200), Size(getClientRectWidth(),FONT_SIZE+10)));
	    };

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
										 stats[(*anarace->getStartCondition())->getRace()][i].name)), edge.GetPosition());
				else
					dc->DrawText(_T
								 (string::
								  Format(T("%2i %ss"), anarace->getLocationTotal(0, i),
										 stats[(*anarace->getStartCondition())->getRace()][i].name)), edge.GetPosition());
//                              dc->SetBrush(Brush(Color(   ( (anarace->getProgramCode(order->IP)+1)*155/(1+(*anarace->getCurrentGoal())->getMaxBuildTypes()/2))%156,
//                                              ((anarace->getProgramCode(order->IP)+1)*155/(1+(*anarace->getCurrentGoal())->getMaxBuildTypes()/4))%156,
//                                              ((anarace->getProgramCode(order->IP)+1)*155/(1+(*anarace->getCurrentGoal())->getMaxBuildTypes()/8))%156),SOLID));

				dc->SetBrush(theme.lookUpMixedBrush(BO_DARK_BRUSH, (eBrush)(UNIT_TYPE_1_BRUSH+stats[(*anarace->getStartCondition())->getRace()][i].facilityType)));

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
//        setMaxScrollY(t*(FONT_SIZE+5));*/

};
#if 0
void ForceWindow::processButtons()
{

	if (isActivated(goalFileListButton))
	{
		if (!goalFileListOpened)
			goalFileListOpened = 1;
		else
			goalFileListOpened = 0;
	}
	for (int i = 0; i < 4; i++)
	{

	}

	for (int i = 0; i < 100; i++)
		if (isActivated(selectGoalButton[i]))
		{
			int k = 0;
			int j = 0;
			while (k < i)
			{
				if (settings.getCurrentGoal(j)->getRace() == (*anarace->getStartCondition())->getRace())
					k++;
				j++;
			}
			settings.setGoal(k, anarace->getPlayerNum());
			settings.setStartRace(anarace->getPlayerNum(), (*anarace->getStartCondition())->getRace());
			setChangedFlag();
//            settings.setMap(MELEE);
			break;
		}
	if (isActivated(addGoalButton))
	{
		if (!goalListOpened)
			goalListOpened = 1;
		else if (goalListOpened == 1)
			goalListOpened = 0;
		else if (goalListOpened > 1)
			goalListOpened = 1;
	} else
		for (int i = 0; i < UNIT_TYPE_COUNT; i++)
			if (isActivated(addUnitButton[i]))
			{
				if ((goalListOpened == 1) && (i == 0))	//scv
				{
	// TODO: pruefen ob das Goal schon vorhanden ist, wenn ja => hasChanged nicht aufrufen
					(*anarace->getCurrentGoal())->addGoal(SCV, 1, 0, 0);
	//                              msgWindow->addMessage(_T(string::Format(T("Added %s to the goal list."),stats[(*anarace->getStartCondition())->getRace()][SCV].name)));
					setChangedFlag();
				} else if ((goalListOpened == 1) && (i == 1))	//gasscv
				{
					(*anarace->getCurrentGoal())->addGoal(GAS_SCV, 1, 0, 0);
	//                              msgWindow->addMessage(_T(string::Format(T("Added %s to the goal list."),stats[(*anarace->getStartCondition())->getRace()][GAS_SCV].name)));
					setChangedFlag();
				} else if (goalListOpened == 1)
					goalListOpened = i;
				else if (goalListOpened)
				{
					int l = 0;
					for (int j = 0; j < UNIT_TYPE_COUNT; j++)
					{
						if (stats[(*anarace->getStartCondition())->getRace()][j].facilityType == goalListOpened + 1)
						{
							if (i == l)
							{
								(*anarace->getCurrentGoal())->addGoal(j, 1, 0, 0);
								setChangedFlag();
				//                                                      msgWindow->addMessage(_T(string::Format(T("Added %s to the goal list."),stats[(*anarace->getStartCondition())->getRace()][j].name)));
								j = UNIT_TYPE_COUNT;
							}
							l++;
						}
					}
				}
			}
};
#endif

void ForceWindow::resetData()
{
	currentUnitType = 0;
	for (int i = UNIT_TYPE_COUNT; i--;)	// muss global in der Klasse sein und nicht lokal in drawGoalList, weil die Balken vergroessern/verkleinern sich ja 
	{
		if ((!anarace) || ((*anarace->getCurrentGoal())->allGoal[i] == 0))
			oldForceList[i] = 0;
		else if ((anarace) && ((*anarace->getCurrentGoal())->allGoal[i]))
			oldForceList[i] = anarace->getLocationTotal(0, i) * 75 / anarace->getUnitsTotalMax();
	}
	markedUnit = 0;
};

void ForceWindow::draw(DC* dc) const
{
	if(!shown)
		return;
	UI_Window::draw(dc);

//      int percent[UNIT_TYPE_COUNT];
//      int counter[UNIT_TYPE_COUNT]; //number of different goals
//      for(int i=0;i<UNIT_TYPE_COUNT;i++) {percent[i]=0;counter[i]=0;}
/*
 * for(int i=0;i<MAX_GOALS;i++) // ???? WTF??
 * * {
 * * percent[(*anarace->getCurrentGoal())->goal[i].unit]+=anarace->fitnessCode[i];
 * * counter[(*anarace->getCurrentGoal())->goal[i].unit]++;
 * * } 
 */

// goallist
	int worstTime[UNIT_TYPE_COUNT];
	for (int i = MAX_GOALS; i--;)
		worstTime[i] = 0;

	for (int i = MAX_GOALS; i--;)
		if ((*anarace->getCurrentGoal())->goal[i].count > 0)
			if (worstTime[(*anarace->getCurrentGoal())->goal[i].unit] < anarace->getFinalTime(i))
				worstTime[(*anarace->getCurrentGoal())->goal[i].unit] = anarace->getFinalTime(i);
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
				if (settings.getCurrentGoal(i)->getRace() == (*anarace->getStartCondition())->getRace())
				{
					GOAL_ENTRY     *goal = settings.getCurrentGoal(i);
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
				if (stats[(*anarace->getStartCondition())->getRace()][i].facilityType == type)
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
						dc->DrawText(_T(string::Format(T("%s"), stats[(*anarace->getStartCondition())->getRace()][i].name)),
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
//      line++;








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
 * * //             dc->DrawText(_T(string::Format(T("[%i]"),anarace->getLocationTotal(0,i))),edge.GetPosition()+Point(edge.GetWidth()-80-oldForceList[i],0));
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
//        setMaxScrollY((t+1)*(FONT_SIZE+5));*/

// Trennungslinie
/*	{
		Rect edge = Rect(getRelativeClientRectPosition() + Point(20, line * (FONT_SIZE + 5) + 9 - getScrollY()),
							 Size(getClientRectWidth() - 40, 2));
		if (fitItemToRelativeClientRect(edge, 1)) // TODO
		{
			dc->SetTextForeground(*theme.lookUpColor(FORCE_TEXT_COLOUR));
			dc->DrawText(*theme.lookUpString(NON_GOALS_STRING), getAbsoluteClientRectPosition() + Point(20, line * (FONT_SIZE + 5) - getScrollY()));
//                      dc->SetPen(Pen(Color(200,0,0),2,SOLID));
//                      dc->DrawLine(edge.GetPosition(),edge.GetPosition()+Point(edge.width,0));
//                      dc->SetPen(Pen(Color(0,0,0),1,SOLID));
		}
	}*/
	dc->SetFont(theme.lookUpFont(SMALL_NORMAL_BOLD_FONT));
	dc->SetTextForeground(*theme.lookUpColor(TEXT_COLOUR));

};

