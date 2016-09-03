#include "force.h"

ForceEntry::ForceEntry(UI_Object* parent, wxRect rect, wxRect maxRect, wxString unit):UI_Button(parent, rect, maxRect, unit, unit, FORCE_ENTRY_BUTTON /* ~~ */, NO_BUTTON_MODE, DO_NOT_ADJUST, SMALL_NORMAL_BOLD_FONT, NO_AUTO_SIZE)
	// TODO!
{
	addUnit = new UI_Button(this, wxRect(wxPoint(getWidth()-117,2),wxSize(8,8)), wxRect(wxPoint(0,0),getSize()), ADD_BUTTON, PRESS_BUTTON_MODE);
	subUnit = new UI_Button(this, wxRect(wxPoint(getWidth()-107,2),wxSize(8,8)), wxRect(wxPoint(0,0),getSize()), SUB_BUTTON, PRESS_BUTTON_MODE);
	cancelUnit = new UI_Button(this, wxRect(wxPoint(getWidth()-97,2),wxSize(8,8)), wxRect(wxPoint(0,0),getSize()), CANCEL_BUTTON, PRESS_BUTTON_MODE);
};

ForceEntry::~ForceEntry()
{
	if(addUnit)
		delete(addUnit);
	if(subUnit)
		delete(subUnit);
	if(cancelUnit)
		delete(cancelUnit);
};

int ForceEntry::changed()
{
	if(addUnit->isPressed()) return(1);
	if(subUnit->isPressed()) return(2);
	if(cancelUnit->isPressed()) return(3);
	return(0);
};

void ForceEntry::updateText(wxString utext)
{
	updateNormalText(utext);
	updatePressedText(utext);
};



ForceWindow::ForceWindow(UI_Object* parent, wxRect rahmen, wxRect maxSize, ANARACE* anarace):UI_Window(parent, FORCE_WINDOW_TITLE_STRING, rahmen, maxSize, SCROLLED)
{
	goalButton[0] = new UI_Button(this, wxRect(getRelativeClientRectPosition()+wxPoint(0,20), getClientRectSize()), wxRect(wxPoint(0,0),getSize()), USE_MAP_SETTINGS_STRING, USE_MAP_SETTINGS_STRING, MY_BUTTON, STATIC_BUTTON_MODE, ARRANGE_TOP, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
	goalButton[1] = new UI_Button(this, wxRect(getRelativeClientRectPosition()+wxPoint(0,20), getClientRectSize()), wxRect(wxPoint(0,0),getSize()), TERRA_STRING, TERRA_STRING, MY_BUTTON, STATIC_BUTTON_MODE, ARRANGE_TOP, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
//	goalButton[1]->forcePressed();
	goalButton[2] = new UI_Button(this, wxRect(getRelativeClientRectPosition()+wxPoint(0,20), getClientRectSize()), wxRect(wxPoint(0,0),getSize()), PROTOSS_STRING, PROTOSS_STRING, MY_BUTTON, STATIC_BUTTON_MODE, ARRANGE_TOP, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);
	goalButton[3] = new UI_Button(this, wxRect(getRelativeClientRectPosition()+wxPoint(0,20), getClientRectSize()), wxRect(wxPoint(0,0),getSize()), ZERG_STRING, ZERG_STRING, MY_BUTTON, STATIC_BUTTON_MODE, ARRANGE_TOP, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE);

	radio = new UI_Radio(this);
	for(int i=0;i<4;i++)
		radio->addButton(goalButton[i]);

	goalFileListOpened = 0;
	goalListOpened = 0;
	this->anarace = anarace;
	
	resetData();

	for(int i=GAS_SCV+1;i--;)
	{
		forceEntry[i]=new ForceEntry(this, wxRect(getRelativeClientRectPosition()+wxPoint(-50,400), wxSize(getClientRectWidth(),FONT_SIZE+10)), 
				wxRect(getRelativeClientRectPosition(), getClientRectSize()+wxSize(0, maxSize.height-maxSize.y)),  // max size
						stats[anarace->getPlayer()->getGoal()->getRace()][i].name);
		forceEntry[i]->Hide();
	};
};

ForceWindow::~ForceWindow()
{
	for(int i=4;i--;)
		if(goalButton[i])
			delete goalButton[i];
	for(int i=GAS_SCV+1;i--;)
		if(forceEntry[i])
			delete forceEntry[i];
	if(radio)
		delete radio;
};

int ForceWindow::getMarkedUnit()
{
	return (markedUnit);
};

void ForceWindow::process()
{
	UI_Window::process();
	if(radio->hasChanged())
	{
            switch (radio->getMarked())
            {
              case 0:
                  settings.setMap(USE_MAP_SETTINGS);
                  break;
              case (TERRA + 1):
              {
                  settings.setStartRace(anarace->getPlayerNum(), TERRA);
                  settings.setMap(MELEE);
              };
                  break;
              case (PROTOSS + 1):
              {
                  settings.setStartRace(anarace->getPlayerNum(), PROTOSS);
                  settings.setMap(MELEE);
              };
                  break;
              case (ZERG + 1):
              {
                  settings.setStartRace(anarace->getPlayerNum(), ZERG); // race setzen NACH setMap, da setMap die default race von geladener Map laedt
                  settings.setMap(MELEE);
              };
                  break;
            }
	}
    if(getChangedFlag())
        resetData();
	
	if (!wxRect(getAbsolutePosition(), getSize()).Inside(controls.getCurrentPosition()))
	{
		goalListOpened = 0;
		goalFileListOpened = 0;
	}

// ------------------- GOALS
int line = 4;
// all units above GAS_SCV are pretty uninteresting (mostly temporary variables)
	for (int i = 0; i <= GAS_SCV; i++)
		if(anarace->getPlayer()->getGoal()->allGoal[i] > 0)
		{
			wxRect edge = wxRect(getRelativeClientRectPosition() + wxPoint(0, line*(FONT_SIZE+6)), wxSize(getClientRectWidth(), FONT_SIZE+5));

	//		if (oldForceList[i] < anarace->getLocationForce(0, i) * 100 / anarace->getMaxUnitForce())
	//			oldForceList[i] += (anarace->getLocationForce(0, i) * 100 / anarace->getMaxUnitForce() - oldForceList[i]) / 5 + 1;
	//		else if (oldForceList[i] > anarace->getLocationForce(0, i) * 100 / anarace->getMaxUnitForce())
	//			oldForceList[i] -= (oldForceList[i] - anarace->getLocationForce(0, i) * 100 / anarace->getMaxUnitForce()) / 5 + 1;
			if (fitItemToClientRect(edge, 1)) //clientRect veraendert sich net ;/
			{
				forceEntry[i]->Show();
				if (forceEntry[i]->isCurrentlyHighlighted())
					markedUnit = i;

				if (anarace->getLocationForce(0, i) == 1)
					forceEntry[i]->updateText(_T(wxString::Format(wxT("%2i %s"), anarace->getLocationForce(0, i),stats[anarace->getPlayer()->getRace()][i].name)));
				else
					forceEntry[i]->updateText(_T(wxString::Format(wxT("%2i %ss"), anarace->getLocationForce(0, i),stats[anarace->getPlayer()->getRace()][i].name)));
				forceEntry[i]->adjustRelativeRect(edge);
				switch(forceEntry[i]->changed())
				{
					case 0:break;
					case 1:anarace->getPlayer()->getGoal()->addGoal(i, 1, 0, 0);break;  //<- Bei (+) kann sich nichts an den goals veraendern!
					case 2:anarace->getPlayer()->getGoal()->addGoal(i, -1, 0, 0);
							setChangedFlag();break;           // <- Bei (-) sehr wohl
					case 3:anarace->getPlayer()->getGoal()->addGoal(i,-anarace->getPlayer()->getGoal()->globalGoal[0][i], 0, 0);
					        setChangedFlag();break;           // <- und auch bei X natuerlich
				};
			}
		   	else 
			{
//				forceEntry[i]->Hide();
				forceEntry[i]->adjustRelativeRect(wxRect(getRelativeClientRectPosition()+wxPoint(-50,400), wxSize(getClientRectWidth(),FONT_SIZE+10)));
			}
			line++;
		} // goal > 0
   	else 
	{
//		forceEntry[i]->Hide();
		forceEntry[i]->adjustRelativeRect(wxRect(getRelativeClientRectPosition()+wxPoint(-50,400), wxSize(getClientRectWidth(),FONT_SIZE+10)));
	};

};
#if 0
void ForceWindow::processButtons()
{

	if (getPressCondition(goalFileListButton) == 1)
	{
		if (goalFileListButtonAni == 0)
			goalFileListButtonAni = 25;
		else
			goalFileListButtonAni++;
		if (goalFileListButtonAni > 50)
			goalFileListButtonAni = 25;
	} else
		goalFileListButtonAni = 0;
	if (isActivated(goalFileListButton))
	{
		if (!goalFileListOpened)
			goalFileListOpened = 1;
		else
			goalFileListOpened = 0;
	}
	for (int i = 0; i < 4; i++)
	{

		if (isActivated(goalButton[i]))
		{
//                      anarace->getPlayer()->getGoal()->resetData();
			switch (i)
			{
			  case 0:
				  settings.setMap(USE_MAP_SETTINGS);
				  break;
			  case (TERRA + 1):
			  {
				  settings.setStartRace(anarace->getPlayerNum(), TERRA);
				  settings.setMap(MELEE);
			  };
				  break;
			  case (PROTOSS + 1):
			  {
				  settings.setStartRace(anarace->getPlayerNum(), PROTOSS);
				  settings.setMap(MELEE);
			  };
				  break;
			  case (ZERG + 1):
			  {
				  settings.setStartRace(anarace->getPlayerNum(), ZERG);	// race setzen NACH setMap, da setMap die default race von geladener Map laedt
				  settings.setMap(MELEE);
			  };
				  break;
			}
//                      setChangedFlag(); TODO DA IST EIN FETTER FEHLER
		}
	}

	for (int i = 0; i < 100; i++)
		if (isActivated(selectGoalButton[i]))
		{
			int k = 0;
			int j = 0;
			while (k < i)
			{
				if (settings.getGoal(j)->getRace() == anarace->getPlayer()->getGoal()->getRace())
					k++;
				j++;
			}
			settings.setGoal(k, anarace->getPlayerNum());
			settings.setStartRace(anarace->getPlayerNum(), anarace->getPlayer()->getGoal()->getRace());
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
					anarace->getPlayer()->getGoal()->addGoal(SCV, 1, 0, 0);
	//                              msgWindow->addMessage(_T(wxString::Format(wxT("Added %s to the goal list."),stats[anarace->getPlayer()->getRace()][SCV].name)));
					setChangedFlag();
				} else if ((goalListOpened == 1) && (i == 1))	//gasscv
				{
					anarace->getPlayer()->getGoal()->addGoal(GAS_SCV, 1, 0, 0);
	//                              msgWindow->addMessage(_T(wxString::Format(wxT("Added %s to the goal list."),stats[anarace->getPlayer()->getRace()][GAS_SCV].name)));
					setChangedFlag();
				} else if (goalListOpened == 1)
					goalListOpened = i;
				else if (goalListOpened)
				{
					int l = 0;
					for (int j = 0; j < UNIT_TYPE_COUNT; j++)
					{
						if (stats[anarace->getPlayer()->getRace()][j].facilityType == goalListOpened + 1)
						{
							if (i == l)
							{
								anarace->getPlayer()->getGoal()->addGoal(j, 1, 0, 0);
								setChangedFlag();
				//                                                      msgWindow->addMessage(_T(wxString::Format(wxT("Added %s to the goal list."),stats[anarace->getPlayer()->getRace()][j].name)));
				//                                                        anarace->getPlayer()->getGoal()->adjustGoals(0); //~~
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
	for (int i = 0; i < UNIT_TYPE_COUNT; i++)	// muss global in der Klasse sein und nicht lokal in drawGoalList, weil die Balken vergroessern/verkleinern sich ja 
	{
		if ((!anarace) || (anarace->getPlayer()->getGoal()->allGoal[i] == 0))
			oldForceList[i] = 0;
		else if ((anarace) && (anarace->getPlayer()->getGoal()->allGoal[i]))
			oldForceList[i] = anarace->getLocationForce(0, i) * 100 / anarace->getMaxUnitForce();
	}
	markedUnit = 0;
};

void ForceWindow::draw(wxDC* dc)
{
	UI_Window::draw(dc);

	currentUnitType = 0;
	markedUnit = 0;
//      int percent[UNIT_TYPE_COUNT];
//      int counter[UNIT_TYPE_COUNT]; //number of different goals
//      for(int i=0;i<UNIT_TYPE_COUNT;i++) {percent[i]=0;counter[i]=0;}
/*
 * for(int i=0;i<MAX_GOALS;i++) // ???? WTF??
 * * {
 * * percent[anarace->getPlayer()->getGoal()->goal[i].unit]+=anarace->fitnessCode[i];
 * * counter[anarace->getPlayer()->getGoal()->goal[i].unit]++;
 * * } 
 */

// goallist
	int worstTime[UNIT_TYPE_COUNT];
	for (int i = 0; i < MAX_GOALS; i++)
		worstTime[i] = 0;

	for (int i = 0; i < MAX_GOALS; i++)
		if (anarace->getPlayer()->getGoal()->goal[i].count > 0)
			if (worstTime[anarace->getPlayer()->getGoal()->goal[i].unit] < anarace->getFinalTime(i))
				worstTime[anarace->getPlayer()->getGoal()->goal[i].unit] = anarace->getFinalTime(i);
	dc->SetFont(*theme.lookUpFont(SMALL_NORMAL_BOLD_FONT));
	dc->SetPen(*theme.lookUpPen(RECTANGLE_PEN));
	int line = 1;
	int t = 0;

	dc->SetTextForeground(*theme.lookUpColour(FORCE_TEXT_COLOUR));
	dc->DrawText(*theme.lookUpString(STARTING_FORCE_STRING) , getClientRectPosition());

//		goalButton[i] = addButton(edge, "change race"); TODO tooltip
	line += 2;
	wxRect edge;
/*	if (isShown() == 1)
	{
		int dx, dy;
		wxString bla;
		if (!goalFileListOpened)
				bla = *theme.lookUpString(LOAD_GOAL_LIST_STRING);
		else
			bla = *theme.lookUpString(CLOSE_GOAL_LIST_STRING);
		dc->GetTextExtent(bla, &dx, &dy);
		wxRect edge;
		int bright = goalFileListButtonAni;
		if (goalFileListOpened)
			bright += 100;
		edge = wxRect(getClientRectLeftBound() + 5, getClientRectUpperBound() + 35, dx + 20, FONT_SIZE + 10);
		
		dc->SetBrush(theme.lookUpBrightenedBrush(FORCE_LIST_BRUSH, bright));
		dc->DrawRoundedRectangle(edge, 3);
		dc->DrawText(bla, edge.x + 5, edge.y + 3);
		dc->DrawBitmap(*theme.lookUpBitmap(ARROW_DOWN_BITMAP), edge.x + edge.width - 15, edge.y + 5);
//		goalFileListButton = addButton(edge); TODO
		dc->SetPen(*theme.lookUpPen(RECTANGLE_PEN));

		line += 2;
		if (goalFileListOpened)
		{
			for (int i = 0; i < settings.getGoalCount(); i++)
				if (settings.getGoal(i)->getRace() == anarace->getPlayer()->getGoal()->getRace())
				{
					GOAL_ENTRY     *goal = settings.getGoal(i);
					int bright = 0;
					wxRect edge = wxRect(getClientRectPosition() + wxPoint(10,
																	  line * (FONT_SIZE + 5) - getScrollY()),
										 wxSize(270,
												FONT_SIZE + 5));
					if (edge.Inside(controls.getCurrentPosition()))
						bright = 50;
					if (fitItemToClientRect(edge, 1))
					{
						dc->SetBrush(theme.lookUpBrightenedBrush(BO_DARK_BRUSH, bright));
						dc->DrawRoundedRectangle(edge, 4);
						dc->SetTextForeground(*theme.lookUpColour(TEXT_COLOUR));
						dc->DrawText(_T(wxString::Format(wxT("%s"), goal->getName())), edge.GetPosition() + wxPoint(10, 0));
//						selectGoalButton[i] = addButton(edge, "load this goal into that player"); TODO
					}
					line++;
				}
		}
		dc->SetFont(*theme.lookUpFont(SMALL_NORMAL_BOLD_FONT));
		dc->SetTextForeground(*theme.lookUpColour(TEXT_COLOUR));
		edge = wxRect(getClientRectPosition() + wxPoint(0, line * (FONT_SIZE + 5) - getScrollY()), wxSize(270, FONT_SIZE + 5));
		if (fitItemToClientRect(edge, 1))
		{
			dc->SetBrush(*theme.lookUpBrush(FORCE_LIST_BUTTON_BRUSH));
			dc->DrawRoundedRectangle(edge, 4);
			
			dc->SetTextForeground(*theme.lookUpColour(FORCE_TEXT_COLOUR));
			if (!goalListOpened)
			{
				dc->DrawBitmap(*theme.lookUpBitmap(ARROW_DOWN_BITMAP), edge.x + 4, edge.y + 2);
				dc->DrawText(*theme.lookUpString(CLICK_TO_ADD_GOAL_STRING), edge.GetPosition() + wxPoint(14, 0));
			} else if (goalListOpened == 1)
			{
				dc->DrawBitmap(*theme.lookUpBitmap(ARROW_UP_BITMAP), edge.x + 4, edge.y + 2);
				dc->DrawText(*theme.lookUpString(CLICK_TO_CLOSE_STRING), edge.GetPosition() + wxPoint(14, 0));
			} else
			{
				dc->DrawBitmap(*theme.lookUpBitmap(ARROW_UP_BITMAP), edge.x + 4, edge.y + 2);
				dc->DrawText(*theme.lookUpString(CLICK_TO_GO_BACK_STRING), edge.GetPosition() + wxPoint(14, 0));
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
				wxRect edge = wxRect(getClientRectPosition() + wxPoint(10,
																  line * (FONT_SIZE + 5) - getScrollY()),
									 wxSize(270, FONT_SIZE + 5));
				if (edge.Inside(controls.getCurrentPosition()))
					bright = 50;
				if (fitItemToClientRect(edge, 1))
				{
					dc->SetBrush(theme.lookUpBrightenedBrush((eBrush)(BRIGHT_UNIT_TYPE_1_BRUSH+i), bright+100));
					dc->DrawRoundedRectangle(edge, 4);
					dc->SetTextForeground(*theme.lookUpColour(TEXT_COLOUR));
					dc->DrawText(*theme.lookUpString((eString)(UNIT_TYPE_0_STRING+i)), edge.GetPosition() + wxPoint(10, 0));
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
				if (stats[anarace->getPlayer()->getRace()][i].facilityType == type)
				{
					int bright = 0;
					wxRect edge = wxRect(getClientRectPosition() + wxPoint(10,
																	  line * (FONT_SIZE + 5) - getScrollY()),
										 wxSize(270,
												FONT_SIZE + 5));
					if (edge.Inside(controls.getCurrentPosition()))
						bright = 50;
					if (fitItemToClientRect(edge, 1))
					{
						dc->SetBrush(theme.lookUpBrightenedBrush((eBrush)(BRIGHT_UNIT_TYPE_1_BRUSH+i), bright+100));
						dc->DrawRoundedRectangle(edge, 4);
						dc->SetTextForeground(*theme.lookUpColour(TEXT_COLOUR));
						dc->DrawText(_T(wxString::Format(wxT("%s"), stats[anarace->getPlayer()->getRace()][i].name)),
									 edge.GetPosition() + wxPoint(10, 0));
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







/*				if (oldForceList[i] > 0)
				{
	//TODO: Was ist mit Einheiten die zwar als force da sind, aber nicht buildable ( = kein Eintrag in phaenoToGenoType) sind?
					dc->SetBrush(wxBrush
								 (wxColour
								  (((anarace->getPlayer()->getGoal()->toGeno(i) + 1) * 255 / (1 +
																							  anarace->getPlayer()->
																							  getGoal()->getMaxBuildTypes() /
																							  2)) % 256,
								   ((anarace->getPlayer()->getGoal()->toGeno(i) + 1) * 255 / (1 +
																							  anarace->getPlayer()->
																							  getGoal()->getMaxBuildTypes() /
																							  4)) % 256,
								   ((anarace->getPlayer()->getGoal()->toGeno(i) + 1) * 255 / (1 +
																							  anarace->getPlayer()->
																							  getGoal()->getMaxBuildTypes() /
																							  8)) % 256), wxSOLID));
	//                                      if(isShown()==1)
	//                                              dc->DrawRoundedRectangle(wxRect(edge.GetPosition()+wxPoint(edge.GetWidth()-oldForceList[i],0),wxSize(oldForceList[i]+1,FONT_SIZE+4)),4);
	//                                      else if(isShown()==2)
					dc->DrawRoundedRectangle(wxRect
											 (edge.GetPosition() + wxPoint(edge.GetWidth() - oldForceList[i], 0),
											  wxSize(oldForceList[i] + 1, FONT_SIZE + 4)), 4);
					if (anarace->getLocationForce(0, i) < anarace->getPlayer()->getGoal()->allGoal[i])
						dc->SetTextForeground(theme.lookUpMixedColour(TEXT_COLOUR, FULFILLED_TEXT_COLOUR));
					else
						dc->SetTextForeground(theme.lookUpMixedColour(TEXT_COLOUR, NOT_FULFILLED_TEXT_COLOUR));
					
					wxString bla = _T(wxString::Format(wxT("%i/%i"),
													   anarace->getLocationForce(0,
																				 i),
													   anarace->getPlayer()->getGoal()->allGoal[i]));
					int dx, dy;
					dc->GetTextExtent(bla, &dx, &dy);
					dc->DrawText(bla, edge.GetPosition() + wxPoint(edge.GetWidth() - dx - 2, 0));
					dc->SetTextForeground(*theme.lookUpColour(TEXT_COLOUR));
				} // end oldForceList >0

				dc->SetBrush(*wxTRANSPARENT_BRUSH);	//TODO gruen rot  evtl
				if (isShown() == 1)
					dc->DrawRoundedRectangle(edge.GetPosition() +
											 wxPoint(edge.GetWidth() -
													 anarace->getPlayer()->getGoal()->allGoal[i] * 100 / anarace->getMaxUnitForce(), 0),
											 wxSize(anarace->getPlayer()->getGoal()->allGoal[i] * 100 / anarace->getMaxUnitForce() + 1,
													FONT_SIZE + 4), 4);
/*				else if (isShown() == 2)
					dc->DrawRoundedRectangle(edge.GetPosition() +
											 wxPoint(edge.GetWidth() -
													 anarace->getPlayer()->getGoal()->allGoal[i] * 100 / anarace->getMaxUnitForce(), 0),
											 wxSize(anarace->getPlayer()->getGoal()->allGoal[i] * 100 / anarace->getMaxUnitForce() + 1,
													FONT_SIZE + 4), 4); TODO ... mode oder so */

/*
 * if((percent[i]/counter[i]<100)&&(percent[i]/counter[i]>0))
 * * {
 * * //             dc->DrawText(_T(wxString::Format(wxT("[%i]"),anarace->getLocationForce(0,i))),edge.GetPosition()+wxPoint(edge.GetWidth()-80-oldForceList[i],0));
 * * dc->DrawText(_T(wxString::Format(wxT("%i%%"),percent[i]/counter[i])),edge.GetPosition()+wxPoint(edge.GetWidth()-120-oldForceList[i],0));
 * * int dx,dy;
 * * wxString bla=_T(wxString::Format(wxT("%2i/%2i"),anarace->getLocationForce(0,i),anarace->getPlayer()->getGoal()->allGoal[i]));
 * * dc->GetTextExtent(bla,&dx,&dy);
 * * dc->DrawText(bla,edge.GetPosition()+wxPoint(edge.GetWidth()-45-dx,0));
 * * 
 * * } else
 * * {
 * * int dx,dy;
 * * wxString bla=_T(wxString::Format(wxT("%2i"),anarace->getLocationForce(0,i)));
 * * dc->GetTextExtent(bla,&dx,&dy);
 * * dc->DrawText(bla,edge.GetPosition()+wxPoint(edge.GetWidth()-45-dx,0));
 * * } 
 */
/*
				if ((isShown() == 1) && (edge.Inside(controls.getCurrentPosition())))
				{
					dc->SetBrush(*wxTRANSPARENT_BRUSH);
					dc->SetPen(*theme.lookUpPen(BITMAP_MARKED_PEN));
					wxRect tedge[11];
					for (int j = 0; j < 11; j++)
					{
						tedge[j] = wxRect(edge.x + edge.width - 117 + j * 10, edge.y + 1, 10, 10);
						if (tedge[j].Inside(controls.getCurrentPosition()))
							dc->DrawRectangle(tedge[j]);
					}
/*
					buttonType[0] =
						addBitmapButton(wxRect(tedge[0].GetPosition() + wxPoint(1, 1), tedge[0].GetSize()), bmpAdd,
										"add one");
					buttonType[1] =
						addBitmapButton(wxRect(tedge[1].GetPosition() + wxPoint(1, 1), tedge[1].GetSize()), bmpSub,
										"remove one");
					buttonType[2] =
						addBitmapButton(wxRect(tedge[2].GetPosition() + wxPoint(1, 1), tedge[2].GetSize()), bmpCancel,
										"remove goal");
					buttonType[3] =
						addBitmapButton(wxRect(tedge[3].GetPosition() + wxPoint(1, 1), tedge[3].GetSize()), bmpArrowLeft,
										"increase priority");
					buttonType[4] =
						addBitmapButton(wxRect(tedge[4].GetPosition() + wxPoint(1, 1), tedge[4].GetSize()), bmpImp6,
										"set priority extremely high");
					buttonType[5] =
						addBitmapButton(wxRect(tedge[5].GetPosition() + wxPoint(1, 1), tedge[5].GetSize()), bmpImp5,
										"set priority very high");
					buttonType[6] =
						addBitmapButton(wxRect(tedge[6].GetPosition() + wxPoint(1, 1), tedge[6].GetSize()), bmpImp4,
										"set priority high");
					buttonType[7] =
						addBitmapButton(wxRect(tedge[7].GetPosition() + wxPoint(1, 1), tedge[7].GetSize()), bmpImp3,
										"set priority medium");
					buttonType[8] =
						addBitmapButton(wxRect(tedge[8].GetPosition() + wxPoint(1, 1), tedge[8].GetSize()), bmpImp2,
										"set priority low");
					buttonType[9] =
						addBitmapButton(wxRect(tedge[9].GetPosition() + wxPoint(1, 1), tedge[9].GetSize()), bmpImp1,
										"set priority very low");
					buttonType[10] =
						addBitmapButton(wxRect(tedge[10].GetPosition() + wxPoint(1, 1), tedge[10].GetSize()), bmpArrowRight,
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
	{
		wxRect edge = wxRect(getClientRectPosition() + wxPoint(20, line * (FONT_SIZE + 5) + 9 - getScrollY()),
							 wxSize(getClientRectWidth() - 40, 2));
		if (fitItemToClientRect(edge, 1))
		{
			dc->SetTextForeground(*theme.lookUpColour(FORCE_TEXT_COLOUR));
			dc->DrawText(*theme.lookUpString(NON_GOALS_STRING), getClientRectPosition() + wxPoint(20, line * (FONT_SIZE + 5) - getScrollY()));
//                      dc->SetPen(wxPen(wxColour(200,0,0),2,wxSOLID));
//                      dc->DrawLine(edge.GetPosition(),edge.GetPosition()+wxPoint(edge.width,0));
//                      dc->SetPen(wxPen(wxColour(0,0,0),1,wxSOLID));
		}
	}
	dc->SetFont(*theme.lookUpFont(SMALL_NORMAL_BOLD_FONT));
	dc->SetTextForeground(*theme.lookUpColour(TEXT_COLOUR));
/*
//  ---------- NON GOALS
	line++;
	for (int i = 0; i <= GAS_SCV; i++)
		if ((anarace->getPlayer()->getGoal()->allGoal[i] == 0) && (anarace->getLocationForce(0, i)))
		{
			wxRect edge = wxRect(getClientRectPosition() + wxPoint(0,
															  line * (FONT_SIZE + 5) + 3 - getScrollY()),
								 wxSize(270, FONT_SIZE + 5));
			if (oldForceList[i] < anarace->getLocationForce(0, i) * 100 / anarace->getMaxUnitForce())
				oldForceList[i] += (anarace->getLocationForce(0, i) * 100 / anarace->getMaxUnitForce() - oldForceList[i]) / 5 + 1;
			else if (oldForceList[i] > anarace->getLocationForce(0, i) * 100 / anarace->getMaxUnitForce())
				oldForceList[i] -= (oldForceList[i] - anarace->getLocationForce(0, i) * 100 / anarace->getMaxUnitForce()) / 5 + 1;
			if (fitItemToClientRect(edge, 1))
			{
				if (line % 2 == 0)
					dc->SetBrush(*theme.lookUpBrush(BO_DARK_BRUSH));
				else
					dc->SetBrush(*theme.lookUpBrush(BO_BRIGHT_BRUSH));

				dc->DrawRoundedRectangle(edge, 4);
				if (anarace->getLocationForce(0, i) == 1)
					dc->DrawText(_T
								 (wxString::
								  Format(wxT("%2i %s"), anarace->getLocationForce(0, i),
										 stats[anarace->getPlayer()->getRace()][i].name)), edge.GetPosition());
				else
					dc->DrawText(_T
								 (wxString::
								  Format(wxT("%2i %ss"), anarace->getLocationForce(0, i),
										 stats[anarace->getPlayer()->getRace()][i].name)), edge.GetPosition());
//                              dc->SetBrush(wxBrush(wxColour(   ( (anarace->getProgramCode(order->IP)+1)*155/(1+anarace->getPlayer()->getGoal()->getMaxBuildTypes()/2))%156,
//                                              ((anarace->getProgramCode(order->IP)+1)*155/(1+anarace->getPlayer()->getGoal()->getMaxBuildTypes()/4))%156,
//                                              ((anarace->getProgramCode(order->IP)+1)*155/(1+anarace->getPlayer()->getGoal()->getMaxBuildTypes()/8))%156),wxSOLID));

				dc->SetBrush(theme.lookUpMixedBrush(BO_DARK_BRUSH, (eBrush)(UNIT_TYPE_1_BRUSH+stats[anarace->getPlayer()->getRace()][i].facilityType)));

				dc->DrawRoundedRectangle(wxRect
										 (edge.GetPosition() + wxPoint(edge.width - 1 - oldForceList[i], 0),
										  wxSize(oldForceList[i] + 1, FONT_SIZE + 4)), 4);

				if ((isShown() == 1) && (edge.Inside(controls.getCurrentPosition())))
				{
					dc->DrawText(*theme.lookUpString(ADD_AS_GOAL_STRING), edge.GetPosition() + wxPoint(edge.width - 100, 0));	//TODO
				}
			}
			line++;
		}
//        setMaxScrollY(t*(FONT_SIZE+5));*/
};

