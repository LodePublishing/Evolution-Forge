#include "force.h"

void ForceWindow::setAnarace(ANARACE* anarace)
{
	this->anarace=anarace;
};																			    

ForceWindow::ForceWindow(wxRect rahmen, wxRect maxSize):GraphixScrollWindow(0,rahmen,maxSize,SCROLLED)
{
	resetData();
};

void ForceWindow::processButtons()
{
	checkButtons();
	for(int i=0;i<UNIT_TYPE_COUNT;i++)
	{
		if(isActivated(buttonType[i*3]))
		{
			anarace->getPlayer()->goal->addGoal(i,1,0,0);
//			msgWindow->addMessage(_T(wxString::Format(wxT("Increased number of goals of %s."),stats[anarace->getPlayer()->getRace()][i].name)));
			wasChanged();
		}
		if(isActivated(buttonType[i*3+1]))
		{
			anarace->getPlayer()->goal->addGoal(i,-1,0,0);
  //		      msgWindow->addMessage(_T(wxString::Format(wxT("Decreased number of goals of %s."),stats[anarace->getPlayer()->getRace()][i].name)));
			wasChanged();
		}
		if(isActivated(buttonType[i*3+2]))
		{
			anarace->getPlayer()->goal->addGoal(i,-anarace->getPlayer()->goal->globalGoal[0][i],0,0);
    //		    msgWindow->addMessage(_T(wxString::Format(wxT("Removed %s from the goal list."),stats[anarace->getPlayer()->getRace()][i].name)));
			wasChanged();
		}
	}
							    
	if(!mouseInside())
  	      goalListOpened=0;

	if(isActivated(resetButton))
	{
		goalReset=1;
		wasChanged();
	}																		    
	if(isActivated(addGoalButton))
	{
		if(!goalListOpened)
			goalListOpened=1;
		else if(goalListOpened==1)
			goalListOpened=0;
		else if(goalListOpened>1)
			goalListOpened=1;
	} else
	for(int i=0;i<UNIT_TYPE_COUNT;i++)
		if(isActivated(addUnitButton[i]))
		{
			if((goalListOpened==1)&&(i==0)) //scv
			{
				anarace->getPlayer()->goal->addGoal(SCV,1,0,0);					 
//				msgWindow->addMessage(_T(wxString::Format(wxT("Added %s to the goal list."),stats[anarace->getPlayer()->getRace()][SCV].name)));
				wasChanged();
			}
		       else
			if((goalListOpened==1)&&(i==1)) //gasscv
			{
				anarace->getPlayer()->goal->addGoal(GAS_SCV,1,0,0);
//				msgWindow->addMessage(_T(wxString::Format(wxT("Added %s to the goal list."),stats[anarace->getPlayer()->getRace()][GAS_SCV].name)));
				wasChanged();
			}
			else if(goalListOpened==1)
				goalListOpened=i;
			else if(goalListOpened)
			{
				int l=0;
				for(int j=0;j<UNIT_TYPE_COUNT;j++)
				{
					if(stats[anarace->getPlayer()->getRace()][j].type==goalListOpened+1)
					{
						if(i==l)
						{
							anarace->getPlayer()->goal->addGoal(j,1,0,0);
							wasChanged();
  //						      msgWindow->addMessage(_T(wxString::Format(wxT("Added %s to the goal list."),stats[anarace->getPlayer()->getRace()][j].name)));
//						      anarace->getPlayer()->goal->adjustGoals(0); //~~
							j=UNIT_TYPE_COUNT;
						}
						l++;
					}
				}
			}
		}
};
																			    
void ForceWindow::resetData()
{
	for(int i=0;i<UNIT_TYPE_COUNT;i++)
		addUnitButton[i]=99999;
	for(int i=0;i<UNIT_TYPE_COUNT;i++)
		oldForceList[i]=0;
	for(int i=0;i<UNIT_TYPE_COUNT*3;i++)
		buttonType[i]=99999;
	goalListOpened=0;
 	addGoalButton=99999;
	maxUnitForce=1;
};
																			    
void ForceWindow::drawGoalList(wxDC* dc)
{
	int percent[UNIT_TYPE_COUNT];
	int counter[UNIT_TYPE_COUNT]; //number of different goals
	for(int i=0;i<UNIT_TYPE_COUNT;i++) {percent[i]=0;counter[i]=0;}
	for(int i=0;i<MAX_GOALS;i++)
	{
		percent[anarace->getPlayer()->goal->goal[i].unit]+=anarace->fitnessCode[i];
		counter[anarace->getPlayer()->goal->goal[i].unit]++;
	}
																			    
// goallist
	int worstTime[UNIT_TYPE_COUNT];
	for(int i=0;i<MAX_GOALS;i++)
		worstTime[i]=0;

	for(int i=0;i<MAX_GOALS;i++)
		if(anarace->getPlayer()->goal->goal[i].count>0)
			if(worstTime[anarace->getPlayer()->goal->goal[i].unit]<anarace->getFinalTime(i))
				worstTime[anarace->getPlayer()->goal->goal[i].unit]=anarace->getFinalTime(i);
	dc->SetFont(GraphixScrollWindow::font);
	dc->SetTextForeground(wxColour(TEXT1R,TEXT1G,TEXT1B));
	dc->SetPen(wxPen(wxColour(0,0,0),1,wxSOLID));
       	int line=1;
	if(isShown()==1)
	{
		wxString bla=_T("reset goals");
		int dx,dy;
		dc->SetFont(GraphixScrollWindow::font2);
		dc->GetTextExtent(bla,&dx,&dy);
		wxRect edge=wxRect(getInnerLeftBound()+getInnerWidth()-dx-10,getInnerUpperBound()+5,dx+10,FONT_SIZE+10);
		dc->SetPen(wxPen(wxColour(100,100,255),2,wxSOLID));
		dc->SetBrush(wxBrush(wxColour(50,50,100),wxSOLID));
		dc->DrawRoundedRectangle(edge,3);
		dc->SetTextForeground(wxColour(255,255,255));
		dc->DrawText(bla,edge.x+5,edge.y+3);
		resetButton=addButton(edge);
		line+=2;
		dc->SetPen(wxPen(wxColour(0,0,0),1,wxSOLID));
		dc->SetFont(GraphixScrollWindow::font);
		dc->SetTextForeground(wxColour(TEXT1R,TEXT1G,TEXT1B));
		clearButtons();
		edge=wxRect(getInnerPosition()+wxPoint(0,line*(FONT_SIZE+5)-getScrollY()),wxSize(270,FONT_SIZE+5));
		if(fitToClientArea(edge,1))
		{
			dc->SetBrush(wxBrush(wxColour(50,50,50),wxSOLID));
			dc->DrawRoundedRectangle(edge,4);
			dc->SetTextForeground(wxColour(0,200,0));
			if(!goalListOpened)
			{
//      			dc->DrawBitmap(bmpArrowDown,edge.x+4,edge.y+2);
				dc->DrawText(_T("Click here to add a goal"),edge.GetPosition()+wxPoint(14,0));
			}
			else if(goalListOpened==1)
			{
//      			dc->DrawBitmap(bmpArrowUp,edge.x+4,edge.y+2);
				dc->DrawText(_T("(Click here to close)"),edge.GetPosition()+wxPoint(14,0));
			}
			else
			{
//			      dc->DrawBitmap(bmpArrowUp,edge.x+4,edge.y+2);
				dc->DrawText(_T("(Click here to go back)"),edge.GetPosition()+wxPoint(14,0));
			}
			addGoalButton=addButton(edge);
		}
		line++;
		if(goalListOpened==1)
		{
			int l=0;
			for(int i=1;i<11;i++)
			{
				int bright=0;
				edge=wxRect(getInnerPosition()+wxPoint(10,line*(FONT_SIZE+5)-getScrollY()),wxSize(270,FONT_SIZE+5));
				if(edge.Inside(controls.getCurrentPosition()))
					bright=50;
				if(fitToClientArea(edge,1))
				{
					dc->SetBrush(wxBrush(wxColour((bright+COLOR1R+2*BOcolor[i].Red()),bright+(COLOR1G+2*BOcolor[i].Green()),bright+(COLOR1B+2*BOcolor[i].Blue())),wxSOLID));
					dc->DrawRoundedRectangle(edge,4);
					dc->SetTextForeground(wxColour(TEXT1R,TEXT1G,TEXT1B));
					dc->DrawText(BOnames[i],edge.GetPosition()+wxPoint(10,0));
					addUnitButton[l]=addButton(edge);
					l++;
				}
				line++; // !!!
			}
		}
		else if(goalListOpened>1)
		{
			int l=0;
			int type=goalListOpened+1;
			for(int i=0;i<UNIT_TYPE_COUNT;i++)
			{
				if(stats[anarace->getPlayer()->getRace()][i].type==type)
				{
					int bright=0;
					edge=wxRect(getInnerPosition()+wxPoint(10,line*(FONT_SIZE+5)-getScrollY()),wxSize(270,FONT_SIZE+5));
					if(edge.Inside(controls.getCurrentPosition()))
						bright=50;
					if(fitToClientArea(edge,1))
					{
						dc->SetBrush(wxBrush(wxColour(bright+(COLOR1R+2*BOcolor[type].Red()),bright+(COLOR1G+2*BOcolor[type].Green()),bright+(COLOR1B+2*BOcolor[type].Blue())),wxSOLID));
						dc->DrawRoundedRectangle(edge,4);
						dc->SetTextForeground(wxColour(TEXT1R,TEXT1G,TEXT1B));
						dc->DrawText(_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][i].name)),edge.GetPosition()+wxPoint(10,0));
						addUnitButton[l]=addButton(edge);
						l++;
					}
					line++; //!!!
				}
			}
		}
		line++;
	}

// ------------------- GOALS

// all units above GAS_SCV are pretty uninteresting (mostly temporary variables)
	for(int i=0;i<=GAS_SCV;i++)
		if(anarace->getPlayer()->goal->allGoal[i]>0)
		{
			if(globalForcelog[i].order==anarace->getLocationForce(0,i))
			{
				if(globalForcelog[i].count<160)
					globalForcelog[i].count+=3;
			}
			else
			{
				globalForcelog[i].count=0;
				globalForcelog[i].order=anarace->getLocationForce(0,i);
			}
			wxRect edge=wxRect(getInnerPosition()+wxPoint(0,line*(FONT_SIZE+5)-getScrollY()),wxSize(270,FONT_SIZE+5));
																			    
			if(oldForceList[i]<anarace->getLocationForce(0,i)*100/maxUnitForce)
				oldForceList[i]+=(anarace->getLocationForce(0,i)*100/maxUnitForce-oldForceList[i])/5+1;
			else if(oldForceList[i]>anarace->getLocationForce(0,i)*100/maxUnitForce)
				oldForceList[i]-=(oldForceList[i]-anarace->getLocationForce(0,i)*100/maxUnitForce)/5+1;
			if(fitToClientArea(edge,1))
			{
				if(line%2==0)
					dc->SetBrush(wxBrush(wxColour(COLOR1R,COLOR1G,COLOR1B),wxSOLID));
				else dc->SetBrush(wxBrush(wxColour(COLOR2R,COLOR2G,COLOR2B),wxSOLID));
																			    
				dc->DrawRoundedRectangle(edge,4);
																			    
				dc->SetTextForeground(wxColour(TEXT1R,TEXT1G,TEXT1B));
				dc->DrawText(_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][i].name)),edge.GetPosition()+wxPoint(12,0));
																			    
				if(oldForceList[i]>0)
				{
dc->SetBrush(wxBrush(wxColour(((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/2))%256,
					((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/4))%256,
					((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/8))%256),wxSOLID));																			    
					dc->DrawRoundedRectangle(wxRect(edge.GetPosition()+wxPoint(edge.GetWidth()-41-oldForceList[i],0),wxSize(oldForceList[i]+1,FONT_SIZE+4)),4);
				}
				dc->SetBrush(wxBrush(wxColour(200,200,200),wxTRANSPARENT)); //TODO gruen rot  evtl
				dc->DrawRoundedRectangle(edge.GetPosition()+wxPoint(edge.GetWidth()-41-anarace->getPlayer()->goal->allGoal[i]*100/maxUnitForce,0),wxSize(anarace->getPlayer()->goal->allGoal[i]*100/maxUnitForce+1,FONT_SIZE+4),4);
																			    
				if((percent[i]/counter[i]<100)&&(percent[i]/counter[i]>0))
				{
			//	      dc->DrawText(_T(wxString::Format(wxT("[%i]"),anarace->getLocationForce(0,i))),edge.GetPosition()+wxPoint(edge.GetWidth()-80-oldForceList[i],0));
					dc->DrawText(_T(wxString::Format(wxT("%i%%"),percent[i]/counter[i])),edge.GetPosition()+wxPoint(edge.GetWidth()-120-oldForceList[i],0));
                                        int dx,dy;
                                        wxString bla=_T(wxString::Format(wxT("%2i/%2i"),anarace->getLocationForce(0,i),anarace->getPlayer()->goal->allGoal[i]));
                                        dc->GetTextExtent(bla,&dx,&dy);
                                        dc->DrawText(bla,edge.GetPosition()+wxPoint(edge.GetWidth()-45-dx,0));

				} else
				{
					int dx,dy;
					wxString bla=_T(wxString::Format(wxT("%2i"),anarace->getLocationForce(0,i)));
					dc->GetTextExtent(bla,&dx,&dy);
					dc->DrawText(bla,edge.GetPosition()+wxPoint(edge.GetWidth()-45-dx,0));
				}
																			    
																			    
//			      buttonType[i*3]=addBitmapButton(wxRect(edge.x+edge.width-36,edge.y+2,8,8),bmpAdd);
//			      buttonType[i*3+1]=addBitmapButton(wxRect(edge.x+edge.width-26,edge.y+2,8,8),bmpSub);
//			      buttonType[i*3+2]=addBitmapButton(wxRect(edge.x+edge.width-16,edge.y+2,8,8),bmpCancel);
																			    
			}
		line++;
	}
	DrawButtons(dc);
																			    
//      setMaxScrollY((t+1)*(FONT_SIZE+5));


// Trennungslinie
	{
		wxRect edge=wxRect(getInnerPosition()+wxPoint(20,line*(FONT_SIZE+5)+9-getScrollY()),wxSize(getInnerWidth()-40,2));
		if(fitToClientArea(edge,1))
		{
			dc->SetTextForeground(wxColour(50,150,50));
			dc->DrawText(_T("non-goals:"),getInnerPosition()+wxPoint(20,line*(FONT_SIZE+5)-getScrollY()));
//			dc->SetPen(wxPen(wxColour(200,0,0),2,wxSOLID));
//			dc->DrawLine(edge.GetPosition(),edge.GetPosition()+wxPoint(edge.width,0));
//			dc->SetPen(wxPen(wxColour(0,0,0),1,wxSOLID));
		}
	}
	dc->SetFont(GraphixScrollWindow::font);
	dc->SetTextForeground(wxColour(TEXT1R,TEXT1G,TEXT1B));

//  ---------- NON GOALS
	line++;
	for(int i=0;i<=GAS_SCV;i++)
		if((anarace->getPlayer()->goal->allGoal[i]==0)&&(anarace->getLocationForce(0,i)))
		{
			if(globalForcelog[i].order==anarace->getLocationForce(0,i))
			{
			       if(globalForcelog[i].count<160)
				       globalForcelog[i].count+=5;
			}
			else
			{
				globalForcelog[i].count=0;
				globalForcelog[i].order=anarace->getLocationForce(0,i);
			}
			wxRect edge=wxRect(getInnerPosition()+wxPoint(0,line*(FONT_SIZE+5)+3-getScrollY()),wxSize(270,FONT_SIZE+5));
			if(oldForceList[i]<anarace->getLocationForce(0,i)*100/maxUnitForce)
				oldForceList[i]+=(anarace->getLocationForce(0,i)*100/maxUnitForce-oldForceList[i])/5+1;
			else if(oldForceList[i]>anarace->getLocationForce(0,i)*100/maxUnitForce)
				oldForceList[i]-=(oldForceList[i]-anarace->getLocationForce(0,i)*100/maxUnitForce)/5+1;
			if(fitToClientArea(edge,1))
			{
				if(line%2==0)
					dc->SetBrush(wxBrush(wxColour(COLOR1R,COLOR1G,COLOR1B),wxSOLID));
				else dc->SetBrush(wxBrush(wxColour(COLOR2R,COLOR2G,COLOR2B),wxSOLID));
				dc->DrawRoundedRectangle(edge,4);
				dc->DrawText(_T(wxString::Format(wxT("%s"),stats[anarace->getPlayer()->getRace()][i].name)),edge.GetPosition()+wxPoint(10,0));
				dc->SetBrush(wxBrush(wxColour(   ((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/2))%256,
						((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/4))%256,
						((anarace->getPlayer()->goal->toGeno(i)+1)*255/(1+anarace->getPlayer()->goal->getMaxBuildTypes()/8))%256),wxSOLID));
				dc->DrawRoundedRectangle(wxRect(edge.GetPosition()+wxPoint(edge.width-1-oldForceList[i],0),wxSize(oldForceList[i]+1,FONT_SIZE+4)),4);
				dc->DrawText(_T(wxString::Format(wxT("%2i"),anarace->getLocationForce(0,i))),edge.GetPosition());
			}
			line++;
		}
//      setMaxScrollY(t*(FONT_SIZE+5));*/
};


