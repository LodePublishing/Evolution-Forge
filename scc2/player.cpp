#include "player.h"
#include "math.h"

void Player::drawGeneString(wxDC* dc, wxRect position)
{
	int i,k,j;
	int stringheight=0;
	int currentType=0;
	k=0;
	wxPoint points1[200];
	wxPoint points2[200];
	wxPoint points3[200];
	wxPoint points4[200];
	for(i=0;i<200;i++)
	{
		points1[i].x=0;points1[i].y=0;
		points2[i].x=0;points2[i].y=0;
		points3[i].x=0;points3[i].y=0;
		points4[i].x=0;points4[i].y=0;
	}
																			    
	int boanzahl=boWindow->orderList.GetCount(); //~~ TODO
	if((*anarace)->isOptimizing())
	{
		NODE* node=boWindow->orderList.GetFirst(); //~~ TODO
		while(node)
		{
			ORDER* order=node->GetData();
			if(order->blend)
			{
				k=1;
				while((node)&&((!(order=node->GetData())->blend)||((stats[(*anarace)->getPlayer()->getRace()][order->unit].type)&&(stats[(*anarace)->getPlayer()->getRace()][order->unit].type==currentType))))
				{
					node=node->GetNext();
					if(!order->blend) continue;
					k++;
				}
				currentType=stats[(*anarace)->getPlayer()->getRace()][order->unit].type;
																			    
				for(j=0;j<k+1;j++)
				{
					points1[j].x=10+(stringheight+j)*(position.width-20)/(boanzahl)+position.x-1;
					points2[j].x=10+(stringheight+j)*(position.width-20)/(boanzahl)+position.x-1;
					points1[j].y=(int)((cos((float)((stringheight+j)+(*anarace)->getGeneration())*10.0*3.1416/200.0)*0.9*position.height/2)+position.y+position.height/2.1);
					points2[j].y=(int)((sin(((float)((stringheight+j)+(*anarace)->getGeneration())+13.0)*10.0*3.1416/200.0)*0.9*position.height/2)+position.y+position.height/2.1);
																			    
					points3[j].x=10+(stringheight+j)*(position.width-20)/(boanzahl)+position.x-1;
					points4[j].x=10+(stringheight+j)*(position.width-20)/(boanzahl)+position.x-1;
					points3[j].y=(int)((cos(((float)((stringheight+j)+(*anarace)->getGeneration())+26.0)*10.0*3.1416/200.0)*0.9*position.height/2)+position.y+position.height/2.1);
					points4[j].y=(int)((sin(((float)((stringheight+j)+(*anarace)->getGeneration())+39.0)*10.0*3.1416/200.0)*0.9*position.height/2)+position.y+position.height/2.1);
				} //end for(j=0;j<k;j++)
				stringheight+=k;
//			      points1[stringheight].x=boDiagramWindow->getRightBound()+60+(int)(cos((float)((stringheight+j)+(*anarace)->getGeneration())*5.0*3.1416/200.0)*50.0);
				k++;
				if(k>=1)
				{
				wxPen bla1=wxPen(wxColour(
3*(COLOR1R+2*BOcolor[stats[(*anarace)->getPlayer()->getRace()][order->unit].type].Red())/10,
3*(COLOR1G+2*BOcolor[stats[(*anarace)->getPlayer()->getRace()][order->unit].type].Green())/10,
3*(COLOR1B+2*BOcolor[stats[(*anarace)->getPlayer()->getRace()][order->unit].type].Blue())/10),3,wxSOLID);
				wxPen bla2=wxPen(wxColour(
				3*(COLOR1R/2+BOcolor[stats[(*anarace)->getPlayer()->getRace()][order->unit].type].Red())/10,
				3*(COLOR1G/2+BOcolor[stats[(*anarace)->getPlayer()->getRace()][order->unit].type].Green())/10,
				3*(COLOR1B/2+BOcolor[stats[(*anarace)->getPlayer()->getRace()][order->unit].type].Blue())/10),1,wxSOLID);
																			    
				dc->SetPen(bla1);
				dc->DrawSpline(k,points1);
				for(j=0;j<k;j++) points1[j].y-=2;
				dc->SetPen(bla2);
				dc->DrawSpline(k,points1);
				for(j=0;j<k;j++) points1[j].y+=4;
				dc->DrawSpline(k,points1);
																			    
				dc->SetPen(bla1);
				dc->DrawSpline(k,points2);
				for(j=0;j<k;j++) points2[j].y-=2;
				dc->SetPen(bla2);
				dc->DrawSpline(k,points2);
				for(j=0;j<k;j++) points2[j].y+=4;
				dc->DrawSpline(k,points2);
																			    
				dc->SetPen(bla1);
				dc->DrawSpline(k,points3);
				for(j=0;j<k;j++) points3[j].y-=2;
				dc->SetPen(bla2);
				dc->DrawSpline(k,points3);
				for(j=0;j<k;j++) points3[j].y+=4;
				dc->DrawSpline(k,points3);
																			    
				dc->SetPen(bla1);
				dc->DrawSpline(k,points4);
				for(j=0;j<k;j++) points4[j].y-=2;
				dc->SetPen(bla2);
				dc->DrawSpline(k,points4);
				for(j=0;j<k;j++) points4[j].y+=4;
				dc->DrawSpline(k,points4);
				}
			} //end blend
			if(node)
				node=node->GetNext();
																			    
		} //end while(node)
	} //end if(isOptimizing)
};

void Player::InitPositions(GraphixScrollWindow* mainWindow)
{
// 1 Player: 1st player [BASIC]
	boWindowRect[0]=wxRect(wxPoint(mainWindow->getInnerWidth()-THIRD_COLOUMN,SECOND_ROW),wxSize(THIRD_COLOUMN,6*(FONT_SIZE+5)+3));
	boWindowMax[0]=wxRect(boWindowRect[0].GetPosition(),wxSize(boWindowRect[0].GetWidth(),mainWindow->getInnerHeight()-30-120));
	forceWindowRect[0]=wxRect(0,SECOND_ROW,FIRST_COLOUMN,14*(FONT_SIZE+5));
	forceWindowMax[0]=wxRect(0,SECOND_ROW,FIRST_COLOUMN,mainWindow->getInnerHeight()-250);
	timerWindowRect[0]=wxRect(wxPoint(mainWindow->getInnerWidth()-THIRD_COLOUMN,0),wxSize(THIRD_COLOUMN,120));
	timerWindowMax[0]=timerWindowRect[0];
	statisticsWindowRect[0]=wxRect(timerWindowRect[0].x-500,0,SECOND_COLOUMN,120);
	statisticsWindowMax[0]=wxRect(timerWindowRect[0].x-500,0,SECOND_COLOUMN,120);

	boDiagramWindowRect[0]=wxRect(wxPoint(FIRST_COLOUMN,400/*theCore->getLowerBound()+5*/),wxSize(SECOND_COLOUMN,110));
	boDiagramWindowMax[0]=boDiagramWindowRect[0];

	boGraphWindowRect[0]=wxRect(statisticsWindowRect[0].x,boDiagramWindowRect[0].y+boDiagramWindowRect[0].height+5,SECOND_COLOUMN,40);
	boGraphWindowMax[0]=wxRect(statisticsWindowRect[0].x,boDiagramWindowRect[0].y+boDiagramWindowRect[0].height+5,SECOND_COLOUMN,380);

// 1 Player: 1st player [ADVANCED]
        boWindowRect[1]=wxRect(wxPoint(mainWindow->getInnerWidth()-THIRD_COLOUMN,SECOND_ROW),wxSize(THIRD_COLOUMN,6*(FONT_SIZE+5)+3));
        boWindowMax[1]=wxRect(boWindowRect[1].GetPosition(),wxSize(boWindowRect[1].GetWidth(),mainWindow->getInnerHeight()-30-120));
        forceWindowRect[1]=wxRect(0,SECOND_ROW,FIRST_COLOUMN,14*(FONT_SIZE+5));
        forceWindowMax[1]=wxRect(0,SECOND_ROW,FIRST_COLOUMN,mainWindow->getInnerHeight()-250);
        timerWindowRect[1]=wxRect(wxPoint(mainWindow->getInnerWidth()-THIRD_COLOUMN,0),wxSize(THIRD_COLOUMN,120));
        timerWindowMax[1]=timerWindowRect[1];
        statisticsWindowRect[1]=wxRect(timerWindowRect[1].x-500,0,SECOND_COLOUMN,120);
        statisticsWindowMax[1]=statisticsWindowRect[1];

	boDiagramWindowRect[1]=wxRect(wxPoint(FIRST_COLOUMN,400),wxSize(SECOND_COLOUMN,110));
	boDiagramWindowMax[1]=boDiagramWindowRect[1];

	boGraphWindowRect[1]=wxRect(FIRST_COLOUMN,515,SECOND_COLOUMN,40);
	boGraphWindowMax[1]=wxRect(boGraphWindowRect[1].GetPosition(),wxSize(boGraphWindowRect[1].GetWidth(),380));

// 2 Player; 1st player [EXPERT], the human
	boWindowRect[2]=wxRect(wxPoint(mainWindow->getInnerWidth()-THIRD_COLOUMN,0),wxSize(THIRD_COLOUMN,6*(FONT_SIZE+5)+3));
	boWindowMax[2]=wxRect(boWindowRect[2].GetPosition(),wxSize(boWindowRect[2].GetWidth(),mainWindow->getInnerHeight()/2-5));

	forceWindowRect[2]=wxRect(0,0,FIRST_COLOUMN,14*(FONT_SIZE+5));
	forceWindowMax[2]=wxRect(forceWindowRect[2].GetPosition(),wxSize(FIRST_COLOUMN,mainWindow->getInnerHeight()/2-5));

	timerWindowRect[2]=wxRect(wxPoint(FIRST_COLOUMN+THIRD_COLOUMN,0),wxSize(THIRD_COLOUMN,120));
	timerWindowMax[2]=timerWindowRect[2];

	statisticsWindowRect[2]=wxRect(timerWindowRect[0].x-500,0,SECOND_COLOUMN,120);
	statisticsWindowMax[2]=statisticsWindowRect[2];

        boDiagramWindowRect[2]=wxRect(wxPoint(FIRST_COLOUMN,125),wxSize(SECOND_COLOUMN,110));
        boDiagramWindowMax[2]=boDiagramWindowRect[2];
                                                                                                                                                            
        boGraphWindowRect[2]=wxRect(FIRST_COLOUMN,240,SECOND_COLOUMN,40);
        boGraphWindowMax[2]=wxRect(boGraphWindowRect[2].GetPosition(),wxSize(boGraphWindowRect[2].GetWidth(),200));


// 2 Player: 2nd player [EXPERT], the computer
	boWindowRect[3]=wxRect(wxPoint(mainWindow->getInnerWidth()-/*2**/THIRD_COLOUMN,mainWindow->getInnerHeight()/2+5),wxSize(THIRD_COLOUMN,6*(FONT_SIZE+5)+3));
	boWindowMax[3]=wxRect(boWindowRect[3].GetPosition(),wxSize(boWindowRect[3].GetWidth(),mainWindow->getInnerHeight()/2-5));

	forceWindowRect[3]=wxRect(0,mainWindow->getInnerHeight()/2+5,FIRST_COLOUMN,14*(FONT_SIZE+5));
	forceWindowMax[3]=wxRect(forceWindowRect[3].GetPosition(),wxSize(FIRST_COLOUMN,mainWindow->getInnerHeight()/2-5));

	timerWindowRect[3]=wxRect(wxPoint(FIRST_COLOUMN,0),wxSize(THIRD_COLOUMN,120));
	timerWindowMax[3]=timerWindowRect[3];

        boDiagramWindowRect[3]=wxRect(wxPoint(FIRST_COLOUMN,445),wxSize(SECOND_COLOUMN,110));
        boDiagramWindowMax[3]=boDiagramWindowRect[3];
                                                                                                                                                            
        boGraphWindowRect[3]=wxRect(FIRST_COLOUMN,550,SECOND_COLOUMN,40);
        boGraphWindowMax[3]=wxRect(boGraphWindowRect[3].GetPosition(),wxSize(boGraphWindowRect[3].GetWidth(),200));



// 2 Player: 1st player [GOSU], the human - freestyle
        boWindowRect[4]=wxRect(wxPoint(mainWindow->getInnerWidth()-THIRD_COLOUMN,0),wxSize(THIRD_COLOUMN,6*(FONT_SIZE+5)+3));
        boWindowMax[4]=wxRect(boWindowRect[4].GetPosition(),wxSize(boWindowRect[4].GetWidth(),mainWindow->getInnerHeight()));
                                                                                                                                                            
        forceWindowRect[4]=wxRect(0,SECOND_ROW,FIRST_COLOUMN,14*(FONT_SIZE+5));
        forceWindowMax[4]=wxRect(forceWindowRect[4].GetPosition(),wxSize(FIRST_COLOUMN,mainWindow->getInnerHeight()/3-5));
                                                                                                                                                            
        timerWindowRect[4]=wxRect(wxPoint(mainWindow->getInnerWidth()-3*THIRD_COLOUMN,0),wxSize(THIRD_COLOUMN,120));
        timerWindowMax[4]=timerWindowRect[4];

        statisticsWindowRect[4]=wxRect(timerWindowRect[0].x-500,0,SECOND_COLOUMN,120);
        statisticsWindowMax[4]=statisticsWindowRect[4];

        boDiagramWindowRect[4]=wxRect(wxPoint(0,2*mainWindow->getInnerHeight()/3+5),wxSize(SECOND_COLOUMN,110));
        boDiagramWindowMax[4]=boDiagramWindowRect[4];
                                                                                                                                                            
        boGraphWindowRect[4]=wxRect(0,2*mainWindow->getInnerHeight()/3+115,SECOND_COLOUMN,40);
        boGraphWindowMax[4]=wxRect(boGraphWindowRect[4].GetPosition(),wxSize(boGraphWindowRect[4].GetWidth(),200));



// 2 Player: 2nd player [GOSU/TRANSCEND], the computer - freestyle
        boWindowRect[5]=wxRect(wxPoint(mainWindow->getInnerWidth()-2*THIRD_COLOUMN,0),wxSize(THIRD_COLOUMN,6*(FONT_SIZE+5)+3));
        boWindowMax[5]=wxRect(boWindowRect[5].GetPosition(),wxSize(boWindowRect[5].GetWidth(),mainWindow->getInnerHeight()));
                                                                                                                                                            
        forceWindowRect[5]=wxRect(FIRST_COLOUMN,SECOND_ROW,FIRST_COLOUMN,14*(FONT_SIZE+5));
        forceWindowMax[5]=wxRect(forceWindowRect[5].GetPosition(),wxSize(FIRST_COLOUMN,mainWindow->getInnerHeight()/3-5));
                                                                                                                                                            
        timerWindowRect[5]=wxRect(wxPoint(mainWindow->getInnerWidth()-4*THIRD_COLOUMN,0),wxSize(THIRD_COLOUMN,120));
        timerWindowMax[5]=timerWindowRect[5];

        boDiagramWindowRect[5]=wxRect(wxPoint(SECOND_COLOUMN,2*mainWindow->getInnerHeight()/3+5),wxSize(SECOND_COLOUMN,110));
        boDiagramWindowMax[5]=boDiagramWindowRect[5];
                                                                                                                                                            
        boGraphWindowRect[5]=wxRect(SECOND_COLOUMN,2*mainWindow->getInnerHeight()/3+115,SECOND_COLOUMN,40);
        boGraphWindowMax[5]=wxRect(boGraphWindowRect[5].GetPosition(),wxSize(boGraphWindowRect[5].GetWidth(),200));



// 2 Player: 1st player [TRANSCEND], the computer - freestyle
        boWindowRect[6]=wxRect(wxPoint(mainWindow->getInnerWidth()-THIRD_COLOUMN,0),wxSize(THIRD_COLOUMN,6*(FONT_SIZE+5)+3));
        boWindowMax[6]=wxRect(boWindowRect[6].GetPosition(),wxSize(boWindowRect[6].GetWidth(),mainWindow->getInnerHeight()));
                                                                                                                                                            
        forceWindowRect[6]=wxRect(0,SECOND_ROW,FIRST_COLOUMN,14*(FONT_SIZE+5));
        forceWindowMax[6]=wxRect(forceWindowRect[6].GetPosition(),wxSize(FIRST_COLOUMN,mainWindow->getInnerHeight()/3-5));
                                                                                                                                                            
        timerWindowRect[6]=wxRect(wxPoint(mainWindow->getInnerWidth()-3*THIRD_COLOUMN,0),wxSize(THIRD_COLOUMN,120));
        timerWindowMax[6]=timerWindowRect[6];

        boDiagramWindowRect[6]=wxRect(wxPoint(0,2*mainWindow->getInnerHeight()/3+5),wxSize(SECOND_COLOUMN,110));
        boDiagramWindowMax[6]=boDiagramWindowRect[6];
                                                                                                                                                            
        boGraphWindowRect[6]=wxRect(0,2*mainWindow->getInnerHeight()/3+115,SECOND_COLOUMN,40);
        boGraphWindowMax[6]=wxRect(boGraphWindowRect[6].GetPosition(),wxSize(boGraphWindowRect[6].GetWidth(),200));


	infoWindowRect[0]=wxRect(boWindowRect[0].GetPosition()+wxPoint(-200,0),wxSize(200,5+9*(FONT_SIZE+5)));
	infoWindowMax[0]=wxRect(boWindowRect[0].x-205/*theCore->getLeftBound()+theCore->getWidth()-200*/,boWindowRect[0].y,200,boGraphWindowRect[0].y-boWindowRect[0].y-5);

	for(int i=0;i<25;i++)
	{
		boWindowRect[i]=wxRect(boWindowRect[i].GetPosition()+mainWindow->getInnerPosition(),boWindowRect[i].GetSize());
		forceWindowRect[i]=wxRect(forceWindowRect[i].GetPosition()+mainWindow->getInnerPosition(),forceWindowRect[i].GetSize());
		timerWindowRect[i]=wxRect(timerWindowRect[i].GetPosition()+mainWindow->getInnerPosition(),timerWindowRect[i].GetSize());
		statisticsWindowRect[i]=wxRect(statisticsWindowRect[i].GetPosition()+mainWindow->getInnerPosition(),statisticsWindowRect[i].GetSize());
		boDiagramWindowRect[i]=wxRect(boDiagramWindowRect[i].GetPosition()+mainWindow->getInnerPosition(),boDiagramWindowRect[i].GetSize());
		boGraphWindowRect[i]=wxRect(boGraphWindowRect[i].GetPosition()+mainWindow->getInnerPosition(),boGraphWindowRect[i].GetSize());
		infoWindowRect[i]=wxRect(infoWindowRect[i].GetPosition()+mainWindow->getInnerPosition(),infoWindowRect[i].GetSize());

		boWindowMax[i]=wxRect(boWindowMax[i].GetPosition()+mainWindow->getInnerPosition(),boWindowMax[i].GetSize());
		forceWindowMax[i]=wxRect(forceWindowMax[i].GetPosition()+mainWindow->getInnerPosition(),forceWindowMax[i].GetSize());
		timerWindowMax[i]=wxRect(timerWindowMax[i].GetPosition()+mainWindow->getInnerPosition(),timerWindowMax[i].GetSize());
		statisticsWindowMax[i]=wxRect(statisticsWindowMax[i].GetPosition()+mainWindow->getInnerPosition(),statisticsWindowMax[i].GetSize());
		boDiagramWindowMax[i]=wxRect(boDiagramWindowMax[i].GetPosition()+mainWindow->getInnerPosition(),boDiagramWindowMax[i].GetSize());
		boGraphWindowMax[i]=wxRect(boGraphWindowMax[i].GetPosition()+mainWindow->getInnerPosition(),boGraphWindowMax[i].GetSize());
		infoWindowMax[i]=wxRect(infoWindowMax[i].GetPosition()+mainWindow->getInnerPosition(),infoWindowMax[i].GetSize());
	}
};
																			    
Player::Player(ANARACE** anarace, int position)
{
	this->anarace=anarace;
	boWindow=new BoWindow(boWindowRect[0],boWindowMax[0]);
//,SCROLLED,NOT_TABBED,wxRect(0,50,1000,1000));
	boWindow->setTitle(0,_T(wxString::Format(wxT("Build Order player %i"),position+1)));
//      boWindow->addDescription(0,"Name");boWindow->addDescription(110,"Mins");boWindow->addDescription(150,"Gas");boWindow->addDescription(190,"Supply");boWindow->addDescription(243,"Time");
																			    
	forceWindow=new ForceWindow(forceWindowRect[0],forceWindowMax[0]);
	forceWindow->setTitle(0,_T(wxString::Format(wxT("Units list player %i"),position+1)));
																			    
	timerWindow=new TimerWindow(timerWindowRect[0],timerWindowMax[0]);
	timerWindow->setTitle(0,_T(wxString::Format(wxT("Best of breed player %i"),position+1)));
																			    
	statisticsWindow=new StatisticsWindow(statisticsWindowRect[0],statisticsWindowMax[0]);
	statisticsWindow->setTitle(0,_T(wxString::Format(wxT("Statistics player %i"),position+1)));
																			    
	boDiagramWindow=new BoDiagramWindow(boDiagramWindowRect[0],boDiagramWindowMax[0]);
	boDiagramWindow->setTitle(0,_T(wxString::Format(wxT("Overview player %i"),position+1)));
																			    
	boGraphWindow=new BoGraphWindow(boGraphWindowRect[0],boGraphWindowMax[0]);
	boGraphWindow->setTitle(0,_T(wxString::Format(wxT("Graphical build order player %i"),position+1)));
																			    
	infoWindow=new InfoWindow(infoWindowRect[0],infoWindowMax[0]);
	infoWindow->setTitle(0,"Order description");

	assignAnarace(anarace);

	Show(0);
	resetData();
};

void Player::assignAnarace(ANARACE** anarace)
{
	boWindow->setAnarace(*anarace);
	forceWindow->setAnarace(*anarace);
	timerWindow->setAnarace(*anarace);
	statisticsWindow->setAnarace(*anarace);
	boDiagramWindow->setAnarace(*anarace);
	boGraphWindow->setAnarace(*anarace);
	infoWindow->setAnarace(*anarace);
};

void Player::Show(int type)//, int player1, int player2)
{
	// hide: 0
	// basic mode: 1     // 1 - 2 player trying to reach the goal
	// advanced mode: 2  // 1 - 2 player trying to reach the goal
	// expert mode: 3    // 2 - 4 player, 1-2 'human', 1-3 computers trying to stop him
	// gosu mode: 4      // freeplay for 1-2 human vs 1-2 computers
	// transcendend mode: 5 // freeplay for 1-2 computers vs 1-2 computers

	if(type)
	{
		shown=1;
		boWindow->setFreeMove(1);
		boWindow->setMaxWindow(boWindowMax[type-1]);
		boWindow->adjustWindow(boWindowRect[type-1]);

		forceWindow->setFreeMove(1);
		forceWindow->setMaxWindow(forceWindowMax[type-1]);
		forceWindow->adjustWindow(forceWindowRect[type-1]);

		boDiagramWindow->setFreeMove(1);
		boDiagramWindow->setMaxWindow(boDiagramWindowMax[type-1]);
		boDiagramWindow->adjustWindow(boDiagramWindowRect[type-1]);

		boGraphWindow->setFreeMove(1);
		boGraphWindow->setMaxWindow(boGraphWindowMax[type-1]);
		boGraphWindow->adjustWindow(boGraphWindowRect[type-1]);

		timerWindow->setFreeMove(1);
		timerWindow->setMaxWindow(timerWindowMax[type-1]);
		timerWindow->adjustWindow(timerWindowRect[type-1]);

		statisticsWindow->setFreeMove(1);
		statisticsWindow->setMaxWindow(statisticsWindowMax[type-1]);
		statisticsWindow->adjustWindow(statisticsWindowRect[type-1]);
// erstmal nur 1vs1 max
	int show[25][7]={
		{0,0,0,0,0,0,0},
		{1,1,0,0,0,1,0}, // basic, goal based
		{1,1,0,1,1,1,1}, // advanced functions, goal based
		{1,1,1,1,1,1,0},{1,2,1,1,1,2,0}, // 1 human vs 1 comp - goal based
		{1,2,1,1,1,2,0},{1,2,1,1,1,2,0}, // 1 human vs 1 comp free GOSU
		{1,2,1,1,1,2,0},{1,2,1,1,1,2,0}, // 1 comp vs 1 comp free TRANSCEND
		{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0}};

	boWindow->Show(show[type][0]);
	forceWindow->Show(show[type][1]);
	infoWindow->Show(show[type][2]);
	boDiagramWindow->Show(show[type][3]);
	boGraphWindow->Show(show[type][4]);
       	timerWindow->Show(show[type][5]);
	statisticsWindow->Show(show[type][6]);
		(*anarace)->setActive(1);
	} else 
	{
		shown=0; //?
		(*anarace)->setActive(0);
	}
};

void Player::processButtons()
{
	if(boWindow->isShown()) boWindow->processButtons();
	if(forceWindow->isShown()) forceWindow->processButtons();
//	if(infoWindow->isShown()) infoWindow->processButtons();
//	if(boDiagramWindow->isShown()) boDiagramWindow->processButtons();
//	if(boGraphWindow->isShown()) boGraphWindow->processButtons();
	if(timerWindow->isShown()) timerWindow->processButtons();
//	if(statisticsWindow->isShown()) statisticsWindow->processButtons();	
};

int Player::hasChanged()
{
	if(boWindow->hasChanged()||forceWindow->hasChanged()||infoWindow->hasChanged()||boDiagramWindow->hasChanged()||boGraphWindow->hasChanged()||timerWindow->hasChanged()||statisticsWindow->hasChanged())
		return(1);
	else return(0);
};

int Player::isOptimizing()
{
	return((*anarace)->isOptimizing());
};

void Player::setOptimizing(int opt)
{
	(*anarace)->setOptimizing(opt);
};

void Player::changeAccepted()
{
	boWindow->changeAccepted();
	forceWindow->changeAccepted();
	infoWindow->changeAccepted();
	boDiagramWindow->changeAccepted();
	boGraphWindow->changeAccepted();
	timerWindow->changeAccepted();
	statisticsWindow->changeAccepted();
};

int Player::isShown()
{
	return(shown);
};
																			    
Player::~Player()
{
};

void Player::resetData()
{
	shown=0;
};
																			    
void Player::DrawMe(wxDC* dc)
{
	if(timerWindow->isShown())
	{
		timerWindow->Draw(dc);
		timerWindow->drawTimer(dc);
	}
																			    
	if(statisticsWindow->isShown())
	{
		statisticsWindow->Draw(dc);
		statisticsWindow->drawStatistics(dc);
	}
																			    
/*	if(boDiagramWindow->isShown())
	{
		boDiagramWindow->Draw(dc);
		boDiagramWindow->showProgramGraph(dc);
	}
																			    
	if(boGraphWindow->isShown())
	{
		boGraphWindow->Draw(dc);
		boGraphWindow->showBoGraph(dc);
	}*/
																			    
	if(boWindow->isShown())
	{
		boWindow->MoveOrders();
		dc->SetTextForeground(wxColour(255,0,0));
		boWindow->Draw(dc);
		boWindow->drawSelectionStuff(dc);
		boWindow->drawBuildOrder(dc);
	}

	if(forceWindow->isShown())
	{
		forceWindow->Draw(dc);
		forceWindow->drawGoalList(dc);
	}
};

void Player::update()
{
	int t=0;
	forceWindow->maxUnitForce=4;
	for(int i=0;i<=GAS_SCV;i++)
	{
		if((*anarace)->getLocationForce(0,i)>forceWindow->maxUnitForce)
			forceWindow->maxUnitForce=(*anarace)->getLocationForce(0,i);
		if((*anarace)->getPlayer()->goal->allGoal[i]>forceWindow->maxUnitForce)
			forceWindow->maxUnitForce=(*anarace)->getPlayer()->goal->allGoal[i];
		t+=(*anarace)->getLocationForce(0,i);;
	}
	if(t>statisticsWindow->maxForce) statisticsWindow->maxForce=t;
	statisticsWindow->currentForce=t;
	if(statisticsWindow->isShown()) //they are sorted there, otherwise we get an infinite loop
		statisticsWindow->ProgressGraph();
	if(boWindow->isShown())
		boWindow->CheckOrders();
	if(boGraphWindow->isShown())
		boGraphWindow->CheckOrders();
//infoWindowNumber=0;
//		if(infoWindow->isShown())
  //		      infoWindowNumber=CheckForInfoWindow();

};

wxRect Player::statisticsWindowRect[25];
wxRect Player::statisticsWindowMax[25];
wxRect Player::boWindowRect[25];
wxRect Player::boWindowMax[25];
wxRect Player::timerWindowRect[25];
wxRect Player::timerWindowMax[25];
wxRect Player::boDiagramWindowRect[25];
wxRect Player::boDiagramWindowMax[25];
wxRect Player::boGraphWindowRect[25];
wxRect Player::boGraphWindowMax[25];
wxRect Player::forceWindowRect[25];
wxRect Player::forceWindowMax[25];
wxRect Player::infoWindowRect[25];
wxRect Player::infoWindowMax[25];


