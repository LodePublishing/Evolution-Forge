#include "player.h"
#include "math.h"


Player::~Player()
{
};

Player::Player(UI_Object* parent, ANARACE** anarace, ePlayerMode mode, const int playerNumber):UI_Object(parent)
{
	geneAnimation=0;
	shown=0;
	this->anarace = anarace;
	window[INFO_WINDOW] = new InfoWindow(this, *anarace, playerNumber);
	window[FORCE_WINDOW] = new ForceWindow(this, *anarace, playerNumber);
	window[TIMER_WINDOW] = new TimerWindow(this, *anarace, playerNumber);
	window[STATISTICS_WINDOW]=new StatisticsWindow(this, *anarace, playerNumber);
	window[BUILD_ORDER_WINDOW]=new BoWindow(this, *anarace, (InfoWindow*)window[INFO_WINDOW], &orderList, playerNumber);
	window[BO_GRAPH_WINDOW] = new BoGraphWindow(this, *anarace, (InfoWindow*)window[INFO_WINDOW], &orderList, playerNumber);
	window[BO_DIAGRAM_WINDOW]=new BoDiagramWindow(this, *anarace, (InfoWindow*)window[INFO_WINDOW], playerNumber);
	setMode(mode);
	//Show(0);
};


void Player::drawGeneString(DC* dc, Rect position)
{
	//TODO
#if 0
	int stringheight=0;
	int currentType=-1;
	Point points1[200];
	Point points2[200];
	Point points3[200];
	Point points4[200];
	for(int i=0;i<200;i++)
	{
		points1[i].x=0;points1[i].y=0;
		points2[i].x=0;points2[i].y=0;
		points3[i].x=0;points3[i].y=0;
		points4[i].x=0;points4[i].y=0;
	}
	int boanzahl=0;
	int colors[MAX_LENGTH];												    
	for(int i=0;i<MAX_LENGTH;i++)
		colors[i]=0;
//	int boanzahl=orderList.GetCount(); //~~ TODO
	{
		NODE* node=orderList.GetFirst();
		while(node)
		{
			ORDER* order=node->GetData();
			if(order->blend)
			{
				boanzahl++;
				colors[order->row]=order->unit;
				node=node->GetNext();
			}
		}
	}
	currentType=-1;

	if((*anarace)->isOptimizing())
	{
		for(int i=0;i<MAX_LENGTH;i++)
			if(colors[i])
			{
				int k=1;
				while((i<MAX_LENGTH)&&((!colors[i])||((stats[(*anarace)->getPlayer()->getRace()][colors[i]].facilityType)&&((currentType==-1)||(stats[(*anarace)->getPlayer()->getRace()][colors[i]].facilityType==currentType)))))
				{
					i++;
					if(!colors[i]) continue;
					if(currentType==-1)
						currentType=stats[(*anarace)->getPlayer()->getRace()][colors[i]].facilityType;
					k++;
				}
				currentType=stats[(*anarace)->getPlayer()->getRace()][colors[i]].facilityType;
																			    
				for(int j=0;j<k+1;j++)
				{
					points1[j].x=10+(stringheight+j)*(position.width-20)/(boanzahl)+position.x-1;
					points2[j].x=10+(stringheight+j)*(position.width-20)/(boanzahl)+position.x-1;
					points1[j].y=(int)((cos((float)((stringheight+j)+geneAnimation)*10.0*3.1416/200.0)*0.9*position.height/2)+position.y+position.height/2.1);
					points2[j].y=(int)((sin(((float)((stringheight+j)+geneAnimation)+13.0)*10.0*3.1416/200.0)*0.9*position.height/2)+position.y+position.height/2.1);
					points3[j].x=10+(stringheight+j)*(position.width-20)/(boanzahl)+position.x-1;
					points4[j].x=10+(stringheight+j)*(position.width-20)/(boanzahl)+position.x-1;
					points3[j].y=(int)((cos(((float)((stringheight+j)+geneAnimation)+26.0)*10.0*3.1416/200.0)*0.9*position.height/2)+position.y+position.height/2.1);
					points4[j].y=(int)((sin(((float)((stringheight+j)+geneAnimation)+39.0)*10.0*3.1416/200.0)*0.9*position.height/2)+position.y+position.height/2.1);
				} //end for(j=0;j<k;j++)
				stringheight+=k;
				k++;
				if(k>=1)
				{
					Pen bla1=Pen(Colour(
3*(COLOR1R+2*BOcolor[stats[(*anarace)->getPlayer()->getRace()][colors[i-1]].facilityType].Red())/4,
3*(COLOR1G+2*BOcolor[stats[(*anarace)->getPlayer()->getRace()][colors[i-1]].facilityType].Green())/4,
3*(COLOR1B+2*BOcolor[stats[(*anarace)->getPlayer()->getRace()][colors[i-1]].facilityType].Blue())/4),3,SOLID);
					Pen bla2=Pen(Colour(
					3*(COLOR1R/2+BOcolor[stats[(*anarace)->getPlayer()->getRace()][colors[i-1]].facilityType].Red())/10,
					3*(COLOR1G/2+BOcolor[stats[(*anarace)->getPlayer()->getRace()][colors[i-1]].facilityType].Green())/10,
					3*(COLOR1B/2+BOcolor[stats[(*anarace)->getPlayer()->getRace()][colors[i-1]].facilityType].Blue())/10),1,SOLID);
																			    
					dc->SetPen(bla1);
					dc->DrawSpline(k,points1);
					for(int j=0;j<k;j++) points1[j].y-=2;
					dc->SetPen(bla2);
					dc->DrawSpline(k,points1);
					for(int j=0;j<k;j++) points1[j].y+=4;
					dc->DrawSpline(k,points1);
																			    
					dc->SetPen(bla1);
					dc->DrawSpline(k,points2);
					for(int j=0;j<k;j++) points2[j].y-=2;
					dc->SetPen(bla2);
					dc->DrawSpline(k,points2);
					for(int j=0;j<k;j++) points2[j].y+=4;
					dc->DrawSpline(k,points2);
																			    
					dc->SetPen(bla1);
					dc->DrawSpline(k,points3);
					for(int j=0;j<k;j++) points3[j].y-=2;
					dc->SetPen(bla2);
					dc->DrawSpline(k,points3);
					for(int j=0;j<k;j++) points3[j].y+=4;
					dc->DrawSpline(k,points3);
																			    
					dc->SetPen(bla1);
					dc->DrawSpline(k,points4);
					for(int j=0;j<k;j++) points4[j].y-=2;
					dc->SetPen(bla2);
					dc->DrawSpline(k,points4);
					for(int j=0;j<k;j++) points4[j].y+=4;
					dc->DrawSpline(k,points4);
				}
			} //end blend
		geneAnimation++;
	} //end if(isOptimizing)
#endif
};

#if 0
void Player::InitPositions(UI_Window* parentWindow)
{
//UI Object hier einrichten! :-)
//	und bei den boWindows etc. so machen, dass UI_Object und nicht UI_Window akzeptiert wird

	
// 1 Player: 1st player [BASIC]
	boWindowRect[0]=Rect(Point(mainWindow->getClientRectWidth()-THIRD_COLOUMN,SECOND_ROW),Size(THIRD_COLOUMN,6*(FONT_SIZE+5)+3));
	boWindowMax[0]=Rect(boWindowRect[0].GetPosition(),Size(boWindowRect[0].GetWidth(),mainWindow->getClientRectHeight()-30-120));
	forceWindowRect[0]=Rect(0,SECOND_ROW,FIRST_COLOUMN,14*(FONT_SIZE+5));
//TODO ne Art * Operator reinbringen, damit die Hoehe genommen wird, die die entsprechende Drawroutine berechnet
	forceWindowMax[0]=Rect(0,SECOND_ROW,FIRST_COLOUMN,mainWindow->getClientRectHeight()-250);
	timerWindowRect[0]=Rect(Point(mainWindow->getClientRectWidth()-THIRD_COLOUMN,0),Size(THIRD_COLOUMN,120));
	timerWindowMax[0]=timerWindowRect[0];
	statisticsWindowRect[0]=Rect(timerWindowRect[0].x-500,0,SECOND_COLOUMN,120);
	statisticsWindowMax[0]=Rect(timerWindowRect[0].x-500,0,SECOND_COLOUMN,120);

	boDiagramWindowRect[0]=Rect(Point(FIRST_COLOUMN,400/*theCore->getLowerBound()+5*/),Size(SECOND_COLOUMN,110));
	boDiagramWindowMax[0]=boDiagramWindowRect[0];

	boGraphWindowRect[0]=Rect(statisticsWindowRect[0].x,boDiagramWindowRect[0].y+boDiagramWindowRect[0].height+5,SECOND_COLOUMN,40);
	boGraphWindowMax[0]=Rect(statisticsWindowRect[0].x,boDiagramWindowRect[0].y+boDiagramWindowRect[0].height+5,SECOND_COLOUMN,380);

// 1 Player: 1st player [ADVANCED]
	boWindowRect[1]=Rect(Point(mainWindow->getClientRectWidth()-THIRD_COLOUMN,SECOND_ROW),Size(THIRD_COLOUMN,6*(FONT_SIZE+5)+3));
	boWindowMax[1]=Rect(boWindowRect[1].GetPosition(),Size(boWindowRect[1].GetWidth(),mainWindow->getClientRectHeight()-30-120));
	forceWindowRect[1]=Rect(0,SECOND_ROW,FIRST_COLOUMN,14*(FONT_SIZE+5));
	forceWindowMax[1]=Rect(0,SECOND_ROW,FIRST_COLOUMN,mainWindow->getClientRectHeight()-250);
	timerWindowRect[1]=Rect(Point(mainWindow->getClientRectWidth()-THIRD_COLOUMN,0),Size(THIRD_COLOUMN,120));
	timerWindowMax[1]=timerWindowRect[1];
	statisticsWindowRect[1]=Rect(timerWindowRect[1].x-500,0,SECOND_COLOUMN,120);
	statisticsWindowMax[1]=statisticsWindowRect[1];

	boDiagramWindowRect[1]=Rect(Point(FIRST_COLOUMN,400),Size(SECOND_COLOUMN,110));
	boDiagramWindowMax[1]=boDiagramWindowRect[1];

	boGraphWindowRect[1]=Rect(FIRST_COLOUMN,515,SECOND_COLOUMN,40);
	boGraphWindowMax[1]=Rect(boGraphWindowRect[1].GetPosition(),Size(boGraphWindowRect[1].GetWidth(),380));

// 2 Player; 1st player [EXPERT], the human
	boWindowRect[2]=Rect(Point(mainWindow->getClientRectWidth()-THIRD_COLOUMN,0),Size(THIRD_COLOUMN,6*(FONT_SIZE+5)+3));
	boWindowMax[2]=Rect(boWindowRect[2].GetPosition(),Size(boWindowRect[2].GetWidth(),mainWindow->getClientRectHeight()/2-5));

	forceWindowRect[2]=Rect(0,0,FIRST_COLOUMN,14*(FONT_SIZE+5));
	forceWindowMax[2]=Rect(forceWindowRect[2].GetPosition(),Size(FIRST_COLOUMN,mainWindow->getClientRectHeight()/2-5));

	timerWindowRect[2]=Rect(Point(FIRST_COLOUMN+THIRD_COLOUMN,0),Size(THIRD_COLOUMN,120));
	timerWindowMax[2]=timerWindowRect[2];

	statisticsWindowRect[2]=Rect(timerWindowRect[0].x-500,0,SECOND_COLOUMN,120);
	statisticsWindowMax[2]=statisticsWindowRect[2];

	boDiagramWindowRect[2]=Rect(Point(FIRST_COLOUMN,125),Size(SECOND_COLOUMN,110));
	boDiagramWindowMax[2]=boDiagramWindowRect[2];
																			    
	boGraphWindowRect[2]=Rect(FIRST_COLOUMN,240,SECOND_COLOUMN,40);
	boGraphWindowMax[2]=Rect(boGraphWindowRect[2].GetPosition(),Size(boGraphWindowRect[2].GetWidth(),200));


// 2 Player: 2nd player [EXPERT], the computer
	boWindowRect[3]=Rect(Point(mainWindow->getClientRectWidth()-/*2**/THIRD_COLOUMN,mainWindow->getClientRectHeight()/2+5),Size(THIRD_COLOUMN,6*(FONT_SIZE+5)+3));
	boWindowMax[3]=Rect(boWindowRect[3].GetPosition(),Size(boWindowRect[3].GetWidth(),mainWindow->getClientRectHeight()/2-5));

	forceWindowRect[3]=Rect(0,mainWindow->getClientRectHeight()/2+5,FIRST_COLOUMN,14*(FONT_SIZE+5));
	forceWindowMax[3]=Rect(forceWindowRect[3].GetPosition(),Size(FIRST_COLOUMN,mainWindow->getClientRectHeight()/2-5));

	timerWindowRect[3]=Rect(Point(FIRST_COLOUMN,0),Size(THIRD_COLOUMN,120));
	timerWindowMax[3]=timerWindowRect[3];

	boDiagramWindowRect[3]=Rect(Point(FIRST_COLOUMN,445),Size(SECOND_COLOUMN,110));
	boDiagramWindowMax[3]=boDiagramWindowRect[3];
																			    
	boGraphWindowRect[3]=Rect(FIRST_COLOUMN,555,SECOND_COLOUMN,40);
	boGraphWindowMax[3]=Rect(boGraphWindowRect[3].GetPosition(),Size(boGraphWindowRect[3].GetWidth(),200));



// 2 Player: 1st player [GOSU], the human - freestyle
	boWindowRect[4]=Rect(Point(mainWindow->getClientRectWidth()-THIRD_COLOUMN,0),Size(THIRD_COLOUMN,6*(FONT_SIZE+5)+3));
	boWindowMax[4]=Rect(boWindowRect[4].GetPosition(),Size(boWindowRect[4].GetWidth(),mainWindow->getClientRectHeight()*2/3));
																			    
	forceWindowRect[4]=Rect(0,SECOND_ROW,FIRST_COLOUMN,14*(FONT_SIZE+5));
	forceWindowMax[4]=Rect(forceWindowRect[4].GetPosition(),Size(FIRST_COLOUMN,mainWindow->getClientRectHeight()/3-5));
																			    
	timerWindowRect[4]=Rect(Point(mainWindow->getClientRectWidth()-3*THIRD_COLOUMN,0),Size(THIRD_COLOUMN,120));
	timerWindowMax[4]=timerWindowRect[4];

	statisticsWindowRect[4]=Rect(timerWindowRect[0].x-500,0,SECOND_COLOUMN,120);
	statisticsWindowMax[4]=statisticsWindowRect[4];

	boDiagramWindowRect[4]=Rect(Point(0,2*mainWindow->getClientRectHeight()/3+5),Size(SECOND_COLOUMN,110));
	boDiagramWindowMax[4]=boDiagramWindowRect[4];
																			    
	boGraphWindowRect[4]=Rect(0,2*mainWindow->getClientRectHeight()/3+115,SECOND_COLOUMN,40);
	boGraphWindowMax[4]=Rect(boGraphWindowRect[4].GetPosition(),Size(boGraphWindowRect[4].GetWidth(),200));



// 2 Player: 2nd player [GOSU/TRANSCEND], the computer - freestyle
	boWindowRect[5]=Rect(Point(mainWindow->getClientRectWidth()-2*THIRD_COLOUMN,0),Size(THIRD_COLOUMN,6*(FONT_SIZE+5)+3));
	boWindowMax[5]=Rect(boWindowRect[5].GetPosition(),Size(boWindowRect[5].GetWidth(),mainWindow->getClientRectHeight()*2/3));
																			    
	forceWindowRect[5]=Rect(FIRST_COLOUMN,SECOND_ROW,FIRST_COLOUMN,14*(FONT_SIZE+5));
	forceWindowMax[5]=Rect(forceWindowRect[5].GetPosition(),Size(FIRST_COLOUMN,mainWindow->getClientRectHeight()/3-5));
																			    
	timerWindowRect[5]=Rect(Point(mainWindow->getClientRectWidth()-4*THIRD_COLOUMN,0),Size(THIRD_COLOUMN,120));
	timerWindowMax[5]=timerWindowRect[5];

	boDiagramWindowRect[5]=Rect(Point(SECOND_COLOUMN,2*mainWindow->getClientRectHeight()/3+5),Size(SECOND_COLOUMN,110));
	boDiagramWindowMax[5]=boDiagramWindowRect[5];
																			    
	boGraphWindowRect[5]=Rect(SECOND_COLOUMN,2*mainWindow->getClientRectHeight()/3+115,SECOND_COLOUMN,40);
	boGraphWindowMax[5]=Rect(boGraphWindowRect[5].GetPosition(),Size(boGraphWindowRect[5].GetWidth(),200));



// 2 Player: 1st player [TRANSCEND], the computer - freestyle
	boWindowRect[6]=Rect(Point(mainWindow->getClientRectWidth()-THIRD_COLOUMN,0),Size(THIRD_COLOUMN,6*(FONT_SIZE+5)+3));
	boWindowMax[6]=Rect(boWindowRect[6].GetPosition(),Size(boWindowRect[6].GetWidth(),mainWindow->getClientRectHeight()*2/3));
																			    
	forceWindowRect[6]=Rect(0,SECOND_ROW,FIRST_COLOUMN,14*(FONT_SIZE+5));
	forceWindowMax[6]=Rect(forceWindowRect[6].GetPosition(),Size(FIRST_COLOUMN,mainWindow->getClientRectHeight()/3-5));
																			    
	timerWindowRect[6]=Rect(Point(mainWindow->getClientRectWidth()-3*THIRD_COLOUMN,0),Size(THIRD_COLOUMN,120));
	timerWindowMax[6]=timerWindowRect[6];

	boDiagramWindowRect[6]=Rect(Point(0,2*mainWindow->getClientRectHeight()/3+5),Size(SECOND_COLOUMN,110));
	boDiagramWindowMax[6]=boDiagramWindowRect[6];
																			    
	boGraphWindowRect[6]=Rect(0,2*mainWindow->getClientRectHeight()/3+115,SECOND_COLOUMN,40);
	boGraphWindowMax[6]=Rect(boGraphWindowRect[6].GetPosition(),Size(boGraphWindowRect[6].GetWidth(),200));

	infoWindowRect[0]=Rect(Point(boWindowRect[0].GetX()-200,0),Size(200,5+9*(FONT_SIZE+5)));
	infoWindowMax[0]=infoWindowRect[0];//Rect(Point(boWindowRect[0].x-200/*theCore->getLeftBound()+theCore->getWidth()-200*/,0),infoWindowRect[0].GetSize());

	for(int i=0;i<25;i++)
	{
		boWindowRect[i]=Rect(boWindowRect[i].GetPosition()+mainWindow->getClientRectPosition(),boWindowRect[i].GetSize());
		forceWindowRect[i]=Rect(forceWindowRect[i].GetPosition()+mainWindow->getClientRectPosition(),forceWindowRect[i].GetSize());
		timerWindowRect[i]=Rect(timerWindowRect[i].GetPosition()+mainWindow->getClientRectPosition(),timerWindowRect[i].GetSize());
		statisticsWindowRect[i]=Rect(statisticsWindowRect[i].GetPosition()+mainWindow->getClientRectPosition(),statisticsWindowRect[i].GetSize());
		boDiagramWindowRect[i]=Rect(boDiagramWindowRect[i].GetPosition()+mainWindow->getClientRectPosition(),boDiagramWindowRect[i].GetSize());
		boGraphWindowRect[i]=Rect(boGraphWindowRect[i].GetPosition()+mainWindow->getClientRectPosition(),boGraphWindowRect[i].GetSize());
		infoWindowRect[i]=Rect(infoWindowRect[i].GetPosition()+mainWindow->getClientRectPosition(),infoWindowRect[i].GetSize());

		boWindowMax[i]=Rect(boWindowMax[i].GetPosition()+mainWindow->getClientRectPosition(),boWindowMax[i].GetSize());
		forceWindowMax[i]=Rect(forceWindowMax[i].GetPosition()+mainWindow->getClientRectPosition(),forceWindowMax[i].GetSize());
		timerWindowMax[i]=Rect(timerWindowMax[i].GetPosition()+mainWindow->getClientRectPosition(),timerWindowMax[i].GetSize());
		statisticsWindowMax[i]=Rect(statisticsWindowMax[i].GetPosition()+mainWindow->getClientRectPosition(),statisticsWindowMax[i].GetSize());
		boDiagramWindowMax[i]=Rect(boDiagramWindowMax[i].GetPosition()+mainWindow->getClientRectPosition(),boDiagramWindowMax[i].GetSize());
		boGraphWindowMax[i]=Rect(boGraphWindowMax[i].GetPosition()+mainWindow->getClientRectPosition(),boGraphWindowMax[i].GetSize());
		infoWindowMax[i]=Rect(infoWindowMax[i].GetPosition()+mainWindow->getClientRectPosition(),infoWindowMax[i].GetSize());
	}
};
#endif 
																			    

void Player::draw(DC* dc)
{
//	drawGeneString(dc,Rect(mainWindow->getPosition()+Point(0,20+i*(mainWindow->getClientRectHeight()/(UI_Object::settings.getMap(0)->getMaxPlayer()-1))),Size(mainWindow->getWidth(),mainWindow->getClientRectHeight()/(UI_Object::settings.getMap(0)->getMaxPlayer()-1)-40))); TODO
	
	UI_Object::draw(dc);
};

void Player::setWindowTitles(ePlayerMode mode)
{
	for(int i=BUILD_ORDER_WINDOW;i<=INFO_WINDOW;i++)
		window[(eWindow)i]->setTitleParameter(*theme.lookUpString((eString)(mode+HIDE_MODE_STRING)));
};


void Player::setMode(ePlayerMode mode)//, int player1, int player2)
{
	this->mode=mode;
	setWindowTitles(mode);
#if 0
	// hide: 0
	// basic mode [PLAYER]: 1     // 1 - 2 player trying to reach the goal
	// advanced mode [PLAYER]: 2  // 1 - 2 player trying to reach the goal
	// expert mode [PLAYER]: 3    // 2 - 4 player, 1-2 'human', 1-3 computers trying to stop him
	// expert mode [COMPUTER]: 4 
	// gosu mode [PLAYER]: 5      // freeplay for 1-2 human vs 1-2 computers
	// gosu mode [COMPUTER]: 6  
	// transcendend mode [COMPUTER]: 7 // freeplay for 1-2 computers vs 1-2 computers

//TODO check ob wir den Type nicht schon vorliegen haben
	if(type)
	{
		shown=1;
		window[BUILD_ORDER_WINDOW]->setFreeMove(1);
		window[BUILD_ORDER_WINDOW]->setMaxWindow(boWindowMax[type-1]);
		window[BUILD_ORDER_WINDOW]->adjustWindow(boWindowRect[type-1]);

		window[FORCE_WINDOW]->setFreeMove(1);
		window[FORCE_WINDOW]->setMaxWindow(forceWindowMax[type-1]);
		window[FORCE_WINDOW]->adjustWindow(forceWindowRect[type-1]);

		window[BO_DIAGRAM_WINDOW]->setFreeMove(1);
		window[BO_DIAGRAM_WINDOW]->setMaxWindow(boDiagramWindowMax[type-1]);
		window[BO_DIAGRAM_WINDOW]->adjustWindow(boDiagramWindowRect[type-1]);

		window[BO_GRAPH_WINDOW]->setFreeMove(1);
		window[BO_GRAPH_WINDOW]->setMaxWindow(boGraphWindowMax[type-1]);
		window[BO_GRAPH_WINDOW]->adjustWindow(boGraphWindowRect[type-1]);

		window[TIMER_WINDOW]->setFreeMove(1);
		window[TIMER_WINDOW]->setMaxWindow(timerWindowMax[type-1]);
		window[TIMER_WINDOW]->adjustWindow(timerWindowRect[type-1]);

		window[STATISTICS_WINDOW]->setFreeMove(1);
		window[STATISTICS_WINDOW]->setMaxWindow(statisticsWindowMax[type-1]);
		window[STATISTICS_WINDOW]->adjustWindow(statisticsWindowRect[type-1]);
// erstmal nur 1vs1 max
	int show[9][8]={
		{0,0,0,0,0,0,0,0},
		{1,1,0,0,0,1,0,0}, // basic, goal based
		{1,1,0,1,1,1,1,0}, // advanced functions, goal based
		{1,1,1,1,1,1,0,0},{1,2,1,1,1,2,0,1}, // 1 human vs 1 comp - goal based
		{1,2,1,1,1,2,0,2},{1,2,1,1,1,2,0,2}, // 1 human vs 1 comp free GOSU
		{1,2,1,1,1,2,0,2},{1,2,1,1,1,2,0,2}}; // 1 comp vs 1 comp free TRANSCEND

		window[BUILD_ORDER_WINDOW]->Show(show[type][0]);
		window[FORCE_WINDOW]->Show(show[type][1]);
//		window[INFO_WINDOW]->Show(show[type][2]);
		window[BO_DIAGRAM_WINDOW]->Show(show[type][3]);
		window[BO_GRAPH_WINDOW]->Show(show[type][4]);
       	window[TIMER_WINDOW]->Show(show[type][5]);
		window[STATISTICS_WINDOW]->Show(show[type][6]);
		(*anarace)->getPlayer()->getGoal()->setMode(show[type][7]);
	} else 
	{
		shown=0; //?
//		(*anarace)->setActive(1);
	}

	switch(type)
	{
		case 0:
		case 1:
		case 2:
		case 3:setWindowTitles(0);break;
		case 4:setWindowTitles(1);break;
		case 5:setWindowTitles(0);break;
		case 6:setWindowTitles(1);break;
		case 7:setWindowTitles(2);break;
		default:break;
	}
//	(*anarace)->setActive(1);
#endif
};

void Player::process()
{
	UI_Object::process();
//	if(window[BUILD_ORDER_WINDOW]->isShown()) window[BUILD_ORDER_WINDOW]->processButtons();
//	if(window[FORCE_WINDOW]->isShown()) window[FORCE_WINDOW]->processButtons();
//	if(window[INFO_WINDOW]->isShown()) window[INFO_WINDOW]->processButtons();
//	if(window[BO_DIAGRAM_WINDOW]->isShown()) window[BO_DIAGRAM_WINDOW]->processButtons();
//	if(window[BO_GRAPH_WINDOW]->isShown()) window[BO_GRAPH_WINDOW]->processButtons();
//	if(window[TIMER_WINDOW]->isShown()) window[TIMER_WINDOW]->processButtons();
//	if(window[STATISTICS_WINDOW]->isShown()) window[STATISTICS_WINDOW]->processButtons();	


// ------ COMMUNICATION BETWEEN THE WINDOWS ------ TODO
//	if(window[FORCE_WINDOW]->isShown())
//	{
//		window[BO_GRAPH_WINDOW]->setMarkedUnit(window[FORCE_WINDOW]->getMarkedUnit());
//	    window[BUILD_ORDER_WINDOW]->setMarkedUnit(window[FORCE_WINDOW]->getMarkedUnit());
//	}
// ------ END COMMUNICATION BETWEEN THE WINDOWS ------

	
// ------ PROCESS MEMBER VARIABLES ------	
	MoveOrders();
// ------ END PROCESSING MEMBER VARIABLES ------
};

bool Player::getChangedFlag()
{
    for(int i=BUILD_ORDER_WINDOW;i<MAX_WINDOWS;i++)
		if(window[(eWindow)i]->getChangedFlag())
			return(1);
	return(0);
};

void Player::updateRectangles()
{
    for(int i=BUILD_ORDER_WINDOW;i<MAX_WINDOWS;i++)
        window[(eWindow)i]->updateRectangles();
};

bool Player::isOptimizing()
{
	return((*anarace)->isOptimizing());
};

void Player::setOptimizing(bool opt)
{
	(*anarace)->setOptimizing(opt);
};

void Player::changeAccepted()
{
	for(int i=BUILD_ORDER_WINDOW;i<=INFO_WINDOW;i++)
		window[(eWindow)i]->changeAccepted();
	//if((*anarace)->getPlayer()->isChanged())  //TODO, Informationsfluss ist sehr unsauber!
	//	(*anarace)->getPlayer()->changeAccepted();
};


/*void Player::resetData()
{
	window[BUILD_ORDER_WINDOW]->resetData();
	window[FORCE_WINDOW]->resetData();
	window[BO_GRAPH_WINDOW]->resetData();
	window[STATISTICS_WINDOW]->resetData();
	window[BO_DIAGRAM_WINDOW]->resetData();
	window[TIMER_WINDOW]->resetData();
	window[INFO_WINDOW]->resetData();
	geneAnimation=0;
	shown=0;
	orderList.Clear();  TODO
};*/

void Player::CheckOrders()
{
	int k=0;
																			   
	for(int s=MAX_LENGTH;s--;)
// /home/clawg/work/sc1041/sc/exe/../scc2/scc2.cpp:843: undefined reference to `ANARACE::getProgramIsBuilt(int)' <- WTF? nur bei exe...
		if((*anarace)->getProgramIsBuilt2(s)/*&&(anarace->getProgramTime(s)<=ga->maxTime-anarace->getTimer())*/)
		{
			map<long, Order>::iterator order=orderList.find((*anarace)->getMarker(s)) ;// => found old one -> update the data!
			if(order!=orderList.end())
			{
				order->second.row=k+1;//+((orderList.getMakeSpace()>-1)*(k+1>=orderList.getMakeSpace())); TODO
				Rect t=Rect(0,order->second.row*(FONT_SIZE+5),500-8,FONT_SIZE+4);
				if(order->second.target!=t)
					order->second.start=order->second.rect;
				order->second.target=t;
				order->second.blendTarget=50;
				order->second.blendStart=order->second.blend;
				order->second.unit=(*anarace)->getPhaenoCode(s);
				order->second.IP=s;
				order->second.checked=1;
				order->second.bonew=0;
			} // => aktualisieren
			else
			// => neues erstellen
			{
				//TODO: testen ob anderes item da ist, das aber die gleiche Unit besitzt + an die gleiche Position kommt
				bool found=false;
				map<long, Order>::iterator order=orderList.begin();
				while((!found) && (order!=orderList.end()))
				{
					if((order->second.unit!=(*anarace)->getPhaenoCode(s))||(order->second.row!=k+1)||(order->second.target.x>0))
						order++;
					else //=> ueberschreiben
					{
						found=true;
						Order neuorder;
	                    neuorder.row=k+1;//+((orderList.getMakeSpace()>-1)*(k+1>=orderList.getMakeSpace())); TODO
                    	Rect t=Rect(0,order->second.row*(FONT_SIZE+5),500-8,FONT_SIZE+4);
                	    if(order->second.target!=t)
            	            neuorder.start=order->second.rect;
        	            else neuorder.start=order->second.start;
    	                neuorder.target=t;
	                    neuorder.blendTarget=50;
                    	neuorder.blendStart=order->second.blend;
                	    neuorder.unit=(*anarace)->getPhaenoCode(s);
            	        neuorder.marker=order->second.marker;
        	            neuorder.checked=1;
    	                neuorder.bonew=1;
						orderList.insert(pair<long, Order>((*anarace)->getMarker(s), neuorder));
						map<long, Order>::iterator temp=order;
						temp++;										
						orderList.erase(order);
						order=temp;
					}
				}
				if(!found) // neues erstellen!
				{
					Order neuorder;
					neuorder.row=k+1;//+((orderList.getMakeSpace()>-1)*(k+1>=orderList.getMakeSpace()));
					neuorder.rect=Rect(170,neuorder.row*(FONT_SIZE+5),0,FONT_SIZE+4);
					Rect t=Rect(0,neuorder.row*(FONT_SIZE+5),500-8,FONT_SIZE+4);
					neuorder.start=neuorder.rect;
					neuorder.target=t;
					neuorder.blend=1;neuorder.blendTarget=50;
					neuorder.blendStart=neuorder.blend;
					neuorder.bonew=1;
					neuorder.unit=(*anarace)->getPhaenoCode(s);
					neuorder.IP=s;
					neuorder.checked=1;
					orderList.insert(pair<long, Order>((*anarace)->getMarker(s), neuorder));
				}
			}
			k++;
		}

		map<long, Order>::iterator order=orderList.begin();
	    while(order!=orderList.end())
		{
			if(!order->second.checked)
			{
				map<long, Order>::iterator temp=order;
				temp++;
				orderList.erase(order);
				order=temp;
			}
			else
			{
				order->second.checked=0;
				order++;
			}
		};
};

void Player::MoveOrders() 
{
    for(map<long, Order>::iterator order=orderList.begin(); order!=orderList.end(); ++order)
	{
		move(order->second.rect.x,order->second.start.x,order->second.target.x);
		move(order->second.rect.y,order->second.start.y,order->second.target.y);
		move(order->second.rect.width,order->second.start.width,order->second.target.width);
		move(order->second.rect.height,order->second.start.height,order->second.target.height);

		move(order->second.brect.x,order->second.bstart.x,order->second.btarget.x);
		move(order->second.brect.y,order->second.bstart.y,order->second.btarget.y);
		move(order->second.brect.width,order->second.bstart.width,order->second.btarget.width);
		move(order->second.brect.height,order->second.bstart.height,order->second.btarget.height);

		move(order->second.blend, order->second.blendStart, order->second.blendTarget);
	}
};


