#include "unitmenu.hpp"

UnitMenuEntry::UnitMenuEntry(UI_Object* parent, Rect rect, Rect maxRect, string unit):UI_Button(parent, rect, maxRect, unit, unit, ADD_GOAL_BUTTON, NO_TEXT_MODE, PRESS_BUTTON_MODE, DO_NOT_ADJUST, SMALL_NORMAL_BOLD_FONT, AUTO_HEIGHT_FULL_WIDTH)
	// TODO!
{
};

UnitMenuEntry::~UnitMenuEntry()
{
};

void UnitMenuEntry::process()
{
	UI_Button::process();
};

void UnitMenuEntry::draw(DC* dc) const
{
	if(!shown) return;
	UI_Button::draw(dc);
};


void UnitMenuEntry::updateText(string utext)
{
	updateNormalText(utext);
	updatePressedText(utext);
};

const int UnitMenu::getHeight() const
{
	return(height);
};


UnitMenu::UnitMenu(UI_Object* parent, ANARACE* anarace, Rect rect):UI_Object(parent, rect)
{
	addGoalButton = new UI_Button(this, Rect(0,0,120,15), Rect(0,0,120,15), CLICK_TO_ADD_GOAL_STRING, CLICK_TO_CLOSE_STRING, ADD_GOAL_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, PRESS_BUTTON_MODE, DO_NOT_ADJUST, SMALL_NORMAL_BOLD_FONT, AUTO_HEIGHT_FULL_WIDTH);
			
	goalFileListOpened = 0;
	goalListOpened = 0;
	height=0;
	this->anarace = anarace;
	pressedUnit=0;
	resetData();

	for(int i=UNIT_TYPE_COUNT;i--;)
	{
		unitEntry[i]=new UnitMenuEntry(this, Rect(0,0,200,200), Rect(0,0,200,200),
				
				//Rect(getRelativePosition()+Point(100,200), Size(getWidth(),FONT_SIZE+10)), 
				//Rect(getRelativePosition(), getSize()+Size(0, getMaxRect().height)),  // max size -y?
				stats[(*anarace->getStartCondition())->getRace()][i].name);
		unitEntry[i]->Hide();
	};
};

UnitMenu::~UnitMenu()
{
	delete addGoalButton;
	for(int i=UNIT_TYPE_COUNT;i--;)
		delete unitEntry[i];
};

const int UnitMenu::getMarkedUnit() const
{
	return (markedUnit);
};

void UnitMenu::process()
{
	UI_Object::process();
	currentUnitType = 0;
	markedUnit = 0;

/*	if (!Rect(parent->getAbsolutePosition(), parent->getSize()).Inside(controls.getCurrentPosition()))
	{
		goalListOpened = 0;
		goalFileListOpened = 0;
	}*/

	if(addGoalButton->isPressed())
	{
		if(goalListOpened==0)
			goalListOpened=1;
		else if(goalListOpened==1)
			goalListOpened=0;
		else goalListOpened=1;
	};
	
	// check for Pressed Units
	pressedUnit = -1;
	if(goalListOpened)
	for (int i = 1; i <= GAS_SCV; i++)
	{
		if (unitEntry[i]->isPressed())
		{
			if ((goalListOpened == 1) && (i == 1))  //scv
            	pressedUnit = i;
    // TODO: pruefen ob das Goal schon vorhanden ist, wenn ja => hasChanged nicht aufrufen
                //    (*anarace->getCurrentGoal())->addGoal(SCV, 1, 0, 0);
    //                              msgWindow->addMessage(_T(string::Format(T("Added %s to the goal list."),stats[(*anarace->getStartCondition())->getRace()][SCV].name)));
			else if ((goalListOpened == 1) && (i == 2))   //gasscv
				pressedUnit = i;
//                 {
  //                  (*anarace->getCurrentGoal())->addGoal(GAS_SCV, 1, 0, 0);
    //                              msgWindow->addMessage(_T(string::Format(T("Added %s to the goal list."),stats[(*anarace->getStartCondition())->getRace()][GAS_SCV].name)));
    //                setChangedFlag();
                //} 
			else if (goalListOpened == 1)
				goalListOpened = i;
            else if (goalListOpened > 1)
			{
				int l = 2;
				for (int j = 2; j < UNIT_TYPE_COUNT; j++)
					if (stats[(*anarace->getStartCondition())->getRace()][j].unitType == goalListOpened)
					{
						if (i == l)
						{
						
							pressedUnit = j;
//                                (*anarace->getCurrentGoal())->addGoal(j, 1, 0, 0);
//                              setChangedFlag();
			//                                                      msgWindow->addMessage(_T(string::Format(T("Added %s to the goal list."),stats[(*anarace->getStartCondition())->getRace()][j].name)));
							j = UNIT_TYPE_COUNT;
						}
						l++;
					}
			}
		}
	}
		
	// ------------------- unit entries
	int line = 2;
	for(int i=UNIT_TYPE_COUNT;i--;)
	{
		if(!unitEntry[i]->isShown())
		{
			unitEntry[i]->setPosition(Point(0,0));
			unitEntry[i]->targetRect=unitEntry[i]->getRelativeRect();
			unitEntry[i]->startRect=unitEntry[i]->getRelativeRect();
			unitEntry[i]->buttonPlacementArea=unitEntry[i]->getRelativeRect();
		}
		else
			unitEntry[i]->Hide();
	}

// all units above GAS_SCV are pretty uninteresting (mostly temporary variables)

	if(goalListOpened)
	{
		if(goalListOpened==1)
		{
            for (int i = 1; i < 10; i++)
            {
                Rect edge = Rect(Point(10,
                                                                  line * (FONT_SIZE + 6) /*- getScrollY()*/),
                                     Size(getParent()->getWidth(), FONT_SIZE + 5));
                //if (fitItemToClientRect(edge, 1))
                {
					unitEntry[i]->Show();
	                if (unitEntry[i]->isCurrentlyHighlighted())
    	                markedUnit = i;
                    unitEntry[i]->setButton(eButton(UNIT_TYPE_0_BUTTON+i));
					unitEntry[i]->updateText(*theme.lookUpString((eString)(UNIT_TYPE_0_STRING+i)));
					unitEntry[i]->adjustRelativeRect(edge);
                }
                line++;         // !!!
			}
			for(int i=11;i<=GAS_SCV;i++)
				unitEntry[i]->Hide();
			unitEntry[0]->Hide();
		}
		else if(goalListOpened>1)
		for(int i = 0; i<=GAS_SCV; i ++)
		if (stats[(*anarace->getStartCondition())->getRace()][i].unitType == goalListOpened)
		{
			Rect edge = Rect(Point(10, line*(FONT_SIZE+6)), Size(getParent()->getWidth(), FONT_SIZE+5));
			unitEntry[i]->Show();
			unitEntry[i]->type=(eUnitType)(goalListOpened);
	//		if (parent->fitItemToRelativeRect(edge, 1)) 
			{
				unitEntry[i]->Show();
				if (unitEntry[i]->isCurrentlyHighlighted())
					markedUnit = i;
				unitEntry[i]->setButton(eButton(UNIT_TYPE_0_BUTTON+goalListOpened));
				unitEntry[i]->updateText(stats[(*anarace->getStartCondition())->getRace()][i].name);
				unitEntry[i]->adjustRelativeRect(edge);
			//	if(unitEntry[i]->isPressed())
			//			pressedUnit = i;
	//					(*anarace->getCurrentGoal())->addGoal(i, 1, 0, 0);/*setChangedFlag();*///break;
			}
	/*		   	else
				{
					unitEntry[i]->Hide();
					unitEntry[i]->adjustRelativeRect(Rect(getRelativePosition()+Point(-100,200), Size(getWidth(),FONT_SIZE+10)));
			}*/
			line++;
		}
	}
	height=line+1;
};
/*
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
};*/
//#endif

void UnitMenu::resetData()
{
	currentUnitType = 0;
	markedUnit = 0;
};

void UnitMenu::draw(DC* dc) const
{
	if(!shown)
		return;
	UI_Object::draw(dc);

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
//		goalButton[i] = addButton(edge, "change race"); TODO tooltip
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
};

