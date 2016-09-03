#include "player.h"
#include "math.h"


Player::~Player()
{
};

Player::Player(UI_Object* parent, ANARACE** anarace, ePlayerMode mode):UI_Object(parent)
{
	geneAnimation=0;
	shown=0;
	orderList.Clear();

	this->anarace = anarace;

// TODO ueberlegen wie ich die TAB Steuerung durchfuehre
	
	InfoWindow* infoWindow= new InfoWindow(this, *(UI_Object::theme.lookUpRect(INFO_WINDOW)), *(UI_Object::theme.lookUpMaxRect(INFO_WINDOW)), *anarace);
	window[INFO_WINDOW] = infoWindow;

	window[FORCE_WINDOW] = new ForceWindow(this, *theme.lookUpRect(FORCE_WINDOW), *theme.lookUpMaxRect(FORCE_WINDOW), *anarace);

	window[TIMER_WINDOW] = new TimerWindow(this, *theme.lookUpRect(TIMER_WINDOW), *theme.lookUpMaxRect(TIMER_WINDOW), *anarace);
	
	window[STATISTICS_WINDOW]=new StatisticsWindow(this, *theme.lookUpRect(STATISTICS_WINDOW), *theme.lookUpMaxRect(STATISTICS_WINDOW), *anarace);
	
	window[BUILD_ORDER_WINDOW]=new BoWindow(this, *theme.lookUpRect(BUILD_ORDER_WINDOW), *theme.lookUpMaxRect(BUILD_ORDER_WINDOW), *anarace, infoWindow, &orderList);
	
	window[BO_GRAPH_WINDOW] = new BoGraphWindow(this, *theme.lookUpRect(BO_GRAPH_WINDOW), *theme.lookUpMaxRect(BO_GRAPH_WINDOW), *anarace, infoWindow, &orderList);
	
	window[BO_DIAGRAM_WINDOW]=new BoDiagramWindow(this, *theme.lookUpRect(BO_DIAGRAM_WINDOW), *theme.lookUpMaxRect(BO_DIAGRAM_WINDOW), *anarace, infoWindow);
	

	setMode(mode);

	//Show(0);
};


void Player::drawGeneString(wxDC* dc, wxRect position)
{
	//TODO
#if 0
	int stringheight=0;
	int currentType=-1;
	wxPoint points1[200];
	wxPoint points2[200];
	wxPoint points3[200];
	wxPoint points4[200];
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
					wxPen bla1=wxPen(wxColour(
3*(COLOR1R+2*BOcolor[stats[(*anarace)->getPlayer()->getRace()][colors[i-1]].facilityType].Red())/4,
3*(COLOR1G+2*BOcolor[stats[(*anarace)->getPlayer()->getRace()][colors[i-1]].facilityType].Green())/4,
3*(COLOR1B+2*BOcolor[stats[(*anarace)->getPlayer()->getRace()][colors[i-1]].facilityType].Blue())/4),3,wxSOLID);
					wxPen bla2=wxPen(wxColour(
					3*(COLOR1R/2+BOcolor[stats[(*anarace)->getPlayer()->getRace()][colors[i-1]].facilityType].Red())/10,
					3*(COLOR1G/2+BOcolor[stats[(*anarace)->getPlayer()->getRace()][colors[i-1]].facilityType].Green())/10,
					3*(COLOR1B/2+BOcolor[stats[(*anarace)->getPlayer()->getRace()][colors[i-1]].facilityType].Blue())/10),1,wxSOLID);
																			    
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
	boWindowRect[0]=wxRect(wxPoint(mainWindow->getClientRectWidth()-THIRD_COLOUMN,SECOND_ROW),wxSize(THIRD_COLOUMN,6*(FONT_SIZE+5)+3));
	boWindowMax[0]=wxRect(boWindowRect[0].GetPosition(),wxSize(boWindowRect[0].GetWidth(),mainWindow->getClientRectHeight()-30-120));
	forceWindowRect[0]=wxRect(0,SECOND_ROW,FIRST_COLOUMN,14*(FONT_SIZE+5));
//TODO ne Art * Operator reinbringen, damit die Hoehe genommen wird, die die entsprechende Drawroutine berechnet
	forceWindowMax[0]=wxRect(0,SECOND_ROW,FIRST_COLOUMN,mainWindow->getClientRectHeight()-250);
	timerWindowRect[0]=wxRect(wxPoint(mainWindow->getClientRectWidth()-THIRD_COLOUMN,0),wxSize(THIRD_COLOUMN,120));
	timerWindowMax[0]=timerWindowRect[0];
	statisticsWindowRect[0]=wxRect(timerWindowRect[0].x-500,0,SECOND_COLOUMN,120);
	statisticsWindowMax[0]=wxRect(timerWindowRect[0].x-500,0,SECOND_COLOUMN,120);

	boDiagramWindowRect[0]=wxRect(wxPoint(FIRST_COLOUMN,400/*theCore->getLowerBound()+5*/),wxSize(SECOND_COLOUMN,110));
	boDiagramWindowMax[0]=boDiagramWindowRect[0];

	boGraphWindowRect[0]=wxRect(statisticsWindowRect[0].x,boDiagramWindowRect[0].y+boDiagramWindowRect[0].height+5,SECOND_COLOUMN,40);
	boGraphWindowMax[0]=wxRect(statisticsWindowRect[0].x,boDiagramWindowRect[0].y+boDiagramWindowRect[0].height+5,SECOND_COLOUMN,380);

// 1 Player: 1st player [ADVANCED]
	boWindowRect[1]=wxRect(wxPoint(mainWindow->getClientRectWidth()-THIRD_COLOUMN,SECOND_ROW),wxSize(THIRD_COLOUMN,6*(FONT_SIZE+5)+3));
	boWindowMax[1]=wxRect(boWindowRect[1].GetPosition(),wxSize(boWindowRect[1].GetWidth(),mainWindow->getClientRectHeight()-30-120));
	forceWindowRect[1]=wxRect(0,SECOND_ROW,FIRST_COLOUMN,14*(FONT_SIZE+5));
	forceWindowMax[1]=wxRect(0,SECOND_ROW,FIRST_COLOUMN,mainWindow->getClientRectHeight()-250);
	timerWindowRect[1]=wxRect(wxPoint(mainWindow->getClientRectWidth()-THIRD_COLOUMN,0),wxSize(THIRD_COLOUMN,120));
	timerWindowMax[1]=timerWindowRect[1];
	statisticsWindowRect[1]=wxRect(timerWindowRect[1].x-500,0,SECOND_COLOUMN,120);
	statisticsWindowMax[1]=statisticsWindowRect[1];

	boDiagramWindowRect[1]=wxRect(wxPoint(FIRST_COLOUMN,400),wxSize(SECOND_COLOUMN,110));
	boDiagramWindowMax[1]=boDiagramWindowRect[1];

	boGraphWindowRect[1]=wxRect(FIRST_COLOUMN,515,SECOND_COLOUMN,40);
	boGraphWindowMax[1]=wxRect(boGraphWindowRect[1].GetPosition(),wxSize(boGraphWindowRect[1].GetWidth(),380));

// 2 Player; 1st player [EXPERT], the human
	boWindowRect[2]=wxRect(wxPoint(mainWindow->getClientRectWidth()-THIRD_COLOUMN,0),wxSize(THIRD_COLOUMN,6*(FONT_SIZE+5)+3));
	boWindowMax[2]=wxRect(boWindowRect[2].GetPosition(),wxSize(boWindowRect[2].GetWidth(),mainWindow->getClientRectHeight()/2-5));

	forceWindowRect[2]=wxRect(0,0,FIRST_COLOUMN,14*(FONT_SIZE+5));
	forceWindowMax[2]=wxRect(forceWindowRect[2].GetPosition(),wxSize(FIRST_COLOUMN,mainWindow->getClientRectHeight()/2-5));

	timerWindowRect[2]=wxRect(wxPoint(FIRST_COLOUMN+THIRD_COLOUMN,0),wxSize(THIRD_COLOUMN,120));
	timerWindowMax[2]=timerWindowRect[2];

	statisticsWindowRect[2]=wxRect(timerWindowRect[0].x-500,0,SECOND_COLOUMN,120);
	statisticsWindowMax[2]=statisticsWindowRect[2];

	boDiagramWindowRect[2]=wxRect(wxPoint(FIRST_COLOUMN,125),wxSize(SECOND_COLOUMN,110));
	boDiagramWindowMax[2]=boDiagramWindowRect[2];
																			    
	boGraphWindowRect[2]=wxRect(FIRST_COLOUMN,240,SECOND_COLOUMN,40);
	boGraphWindowMax[2]=wxRect(boGraphWindowRect[2].GetPosition(),wxSize(boGraphWindowRect[2].GetWidth(),200));


// 2 Player: 2nd player [EXPERT], the computer
	boWindowRect[3]=wxRect(wxPoint(mainWindow->getClientRectWidth()-/*2**/THIRD_COLOUMN,mainWindow->getClientRectHeight()/2+5),wxSize(THIRD_COLOUMN,6*(FONT_SIZE+5)+3));
	boWindowMax[3]=wxRect(boWindowRect[3].GetPosition(),wxSize(boWindowRect[3].GetWidth(),mainWindow->getClientRectHeight()/2-5));

	forceWindowRect[3]=wxRect(0,mainWindow->getClientRectHeight()/2+5,FIRST_COLOUMN,14*(FONT_SIZE+5));
	forceWindowMax[3]=wxRect(forceWindowRect[3].GetPosition(),wxSize(FIRST_COLOUMN,mainWindow->getClientRectHeight()/2-5));

	timerWindowRect[3]=wxRect(wxPoint(FIRST_COLOUMN,0),wxSize(THIRD_COLOUMN,120));
	timerWindowMax[3]=timerWindowRect[3];

	boDiagramWindowRect[3]=wxRect(wxPoint(FIRST_COLOUMN,445),wxSize(SECOND_COLOUMN,110));
	boDiagramWindowMax[3]=boDiagramWindowRect[3];
																			    
	boGraphWindowRect[3]=wxRect(FIRST_COLOUMN,555,SECOND_COLOUMN,40);
	boGraphWindowMax[3]=wxRect(boGraphWindowRect[3].GetPosition(),wxSize(boGraphWindowRect[3].GetWidth(),200));



// 2 Player: 1st player [GOSU], the human - freestyle
	boWindowRect[4]=wxRect(wxPoint(mainWindow->getClientRectWidth()-THIRD_COLOUMN,0),wxSize(THIRD_COLOUMN,6*(FONT_SIZE+5)+3));
	boWindowMax[4]=wxRect(boWindowRect[4].GetPosition(),wxSize(boWindowRect[4].GetWidth(),mainWindow->getClientRectHeight()*2/3));
																			    
	forceWindowRect[4]=wxRect(0,SECOND_ROW,FIRST_COLOUMN,14*(FONT_SIZE+5));
	forceWindowMax[4]=wxRect(forceWindowRect[4].GetPosition(),wxSize(FIRST_COLOUMN,mainWindow->getClientRectHeight()/3-5));
																			    
	timerWindowRect[4]=wxRect(wxPoint(mainWindow->getClientRectWidth()-3*THIRD_COLOUMN,0),wxSize(THIRD_COLOUMN,120));
	timerWindowMax[4]=timerWindowRect[4];

	statisticsWindowRect[4]=wxRect(timerWindowRect[0].x-500,0,SECOND_COLOUMN,120);
	statisticsWindowMax[4]=statisticsWindowRect[4];

	boDiagramWindowRect[4]=wxRect(wxPoint(0,2*mainWindow->getClientRectHeight()/3+5),wxSize(SECOND_COLOUMN,110));
	boDiagramWindowMax[4]=boDiagramWindowRect[4];
																			    
	boGraphWindowRect[4]=wxRect(0,2*mainWindow->getClientRectHeight()/3+115,SECOND_COLOUMN,40);
	boGraphWindowMax[4]=wxRect(boGraphWindowRect[4].GetPosition(),wxSize(boGraphWindowRect[4].GetWidth(),200));



// 2 Player: 2nd player [GOSU/TRANSCEND], the computer - freestyle
	boWindowRect[5]=wxRect(wxPoint(mainWindow->getClientRectWidth()-2*THIRD_COLOUMN,0),wxSize(THIRD_COLOUMN,6*(FONT_SIZE+5)+3));
	boWindowMax[5]=wxRect(boWindowRect[5].GetPosition(),wxSize(boWindowRect[5].GetWidth(),mainWindow->getClientRectHeight()*2/3));
																			    
	forceWindowRect[5]=wxRect(FIRST_COLOUMN,SECOND_ROW,FIRST_COLOUMN,14*(FONT_SIZE+5));
	forceWindowMax[5]=wxRect(forceWindowRect[5].GetPosition(),wxSize(FIRST_COLOUMN,mainWindow->getClientRectHeight()/3-5));
																			    
	timerWindowRect[5]=wxRect(wxPoint(mainWindow->getClientRectWidth()-4*THIRD_COLOUMN,0),wxSize(THIRD_COLOUMN,120));
	timerWindowMax[5]=timerWindowRect[5];

	boDiagramWindowRect[5]=wxRect(wxPoint(SECOND_COLOUMN,2*mainWindow->getClientRectHeight()/3+5),wxSize(SECOND_COLOUMN,110));
	boDiagramWindowMax[5]=boDiagramWindowRect[5];
																			    
	boGraphWindowRect[5]=wxRect(SECOND_COLOUMN,2*mainWindow->getClientRectHeight()/3+115,SECOND_COLOUMN,40);
	boGraphWindowMax[5]=wxRect(boGraphWindowRect[5].GetPosition(),wxSize(boGraphWindowRect[5].GetWidth(),200));



// 2 Player: 1st player [TRANSCEND], the computer - freestyle
	boWindowRect[6]=wxRect(wxPoint(mainWindow->getClientRectWidth()-THIRD_COLOUMN,0),wxSize(THIRD_COLOUMN,6*(FONT_SIZE+5)+3));
	boWindowMax[6]=wxRect(boWindowRect[6].GetPosition(),wxSize(boWindowRect[6].GetWidth(),mainWindow->getClientRectHeight()*2/3));
																			    
	forceWindowRect[6]=wxRect(0,SECOND_ROW,FIRST_COLOUMN,14*(FONT_SIZE+5));
	forceWindowMax[6]=wxRect(forceWindowRect[6].GetPosition(),wxSize(FIRST_COLOUMN,mainWindow->getClientRectHeight()/3-5));
																			    
	timerWindowRect[6]=wxRect(wxPoint(mainWindow->getClientRectWidth()-3*THIRD_COLOUMN,0),wxSize(THIRD_COLOUMN,120));
	timerWindowMax[6]=timerWindowRect[6];

	boDiagramWindowRect[6]=wxRect(wxPoint(0,2*mainWindow->getClientRectHeight()/3+5),wxSize(SECOND_COLOUMN,110));
	boDiagramWindowMax[6]=boDiagramWindowRect[6];
																			    
	boGraphWindowRect[6]=wxRect(0,2*mainWindow->getClientRectHeight()/3+115,SECOND_COLOUMN,40);
	boGraphWindowMax[6]=wxRect(boGraphWindowRect[6].GetPosition(),wxSize(boGraphWindowRect[6].GetWidth(),200));

	infoWindowRect[0]=wxRect(wxPoint(boWindowRect[0].GetX()-200,0),wxSize(200,5+9*(FONT_SIZE+5)));
	infoWindowMax[0]=infoWindowRect[0];//wxRect(wxPoint(boWindowRect[0].x-200/*theCore->getLeftBound()+theCore->getWidth()-200*/,0),infoWindowRect[0].GetSize());

	for(int i=0;i<25;i++)
	{
		boWindowRect[i]=wxRect(boWindowRect[i].GetPosition()+mainWindow->getClientRectPosition(),boWindowRect[i].GetSize());
		forceWindowRect[i]=wxRect(forceWindowRect[i].GetPosition()+mainWindow->getClientRectPosition(),forceWindowRect[i].GetSize());
		timerWindowRect[i]=wxRect(timerWindowRect[i].GetPosition()+mainWindow->getClientRectPosition(),timerWindowRect[i].GetSize());
		statisticsWindowRect[i]=wxRect(statisticsWindowRect[i].GetPosition()+mainWindow->getClientRectPosition(),statisticsWindowRect[i].GetSize());
		boDiagramWindowRect[i]=wxRect(boDiagramWindowRect[i].GetPosition()+mainWindow->getClientRectPosition(),boDiagramWindowRect[i].GetSize());
		boGraphWindowRect[i]=wxRect(boGraphWindowRect[i].GetPosition()+mainWindow->getClientRectPosition(),boGraphWindowRect[i].GetSize());
		infoWindowRect[i]=wxRect(infoWindowRect[i].GetPosition()+mainWindow->getClientRectPosition(),infoWindowRect[i].GetSize());

		boWindowMax[i]=wxRect(boWindowMax[i].GetPosition()+mainWindow->getClientRectPosition(),boWindowMax[i].GetSize());
		forceWindowMax[i]=wxRect(forceWindowMax[i].GetPosition()+mainWindow->getClientRectPosition(),forceWindowMax[i].GetSize());
		timerWindowMax[i]=wxRect(timerWindowMax[i].GetPosition()+mainWindow->getClientRectPosition(),timerWindowMax[i].GetSize());
		statisticsWindowMax[i]=wxRect(statisticsWindowMax[i].GetPosition()+mainWindow->getClientRectPosition(),statisticsWindowMax[i].GetSize());
		boDiagramWindowMax[i]=wxRect(boDiagramWindowMax[i].GetPosition()+mainWindow->getClientRectPosition(),boDiagramWindowMax[i].GetSize());
		boGraphWindowMax[i]=wxRect(boGraphWindowMax[i].GetPosition()+mainWindow->getClientRectPosition(),boGraphWindowMax[i].GetSize());
		infoWindowMax[i]=wxRect(infoWindowMax[i].GetPosition()+mainWindow->getClientRectPosition(),infoWindowMax[i].GetSize());
	}
};
#endif 
																			    

void Player::draw(wxDC* dc)
{
//	drawGeneString(dc,wxRect(mainWindow->getPosition()+wxPoint(0,20+i*(mainWindow->getClientRectHeight()/(UI_Object::settings.getMap(0)->getMaxPlayer()-1))),wxSize(mainWindow->getWidth(),mainWindow->getClientRectHeight()/(UI_Object::settings.getMap(0)->getMaxPlayer()-1)-40))); TODO
	
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
			if(NODE* node=orderList.Find((*anarace)->getMarker(s))) // => found old one -> update the data!
			{
				ORDER* order=node->GetData();
				order->row=k+1+((orderList.getMakeSpace()>-1)*(k+1>=orderList.getMakeSpace()));
				// TODO!
				wxRect t=wxRect(0,order->row*(FONT_SIZE+5),500-8,FONT_SIZE+4);
				if(order->target!=t)
					order->start=order->rect;
				order->target=t;
				order->blendTarget=50;
				order->blendStart=order->blend;
				order->unit=(*anarace)->getPhaenoCode(s);
				order->IP=s;
				order->checked=1;
			} // => aktualisieren
			else // => neues erstellen
			{
				//TODO: testen ob anderes item da ist, das aber die gleiche Unit besitzt + an die gleiche Position kommt
				int found=0;
				NODE *node=orderList.GetFirst();
				while(node&&(!found))
				{
					if((node->GetData()->unit!=(*anarace)->getPhaenoCode(s))||(node->GetData()->row!=k+1)||(node->GetData()->target.x>0))
						node=node->GetNext();
					else //=> ueberschreiben
					{
						ORDER* order=node->GetData();
						order->blendTarget=50;
						order->blendStart=order->blend;
						//order->dx=0;order->dy=0;
						//order->y=(k+1)*(FONT_SIZE+5);
						order->row=k+1+((orderList.getMakeSpace()>-1)*(k+1>=orderList.getMakeSpace())); //?
						wxRect t=wxRect(0,order->row*(FONT_SIZE+5),500-8,FONT_SIZE+4);
						if(order->target!=t)
							order->start=order->rect;
						order->target=t;
						order->bonew=1;
						order->IP=s;
						order->unit=(*anarace)->getPhaenoCode(s);
						order->checked=1;
						found=1;
					}
				}
				if(!found)
				{
					ORDER* order=new ORDER;
					order->row=k+1+((orderList.getMakeSpace()>-1)*(k+1>=orderList.getMakeSpace()));
					order->rect=wxRect(170,order->row*(FONT_SIZE+5),0,FONT_SIZE+4);
					wxRect t=wxRect(0,order->row*(FONT_SIZE+5),500-8,FONT_SIZE+4);
					if(order->target!=t) // ?????
						order->start=order->rect;
					order->target=t;
					order->blend=1;order->blendTarget=50;
					order->blendStart=order->blend;
					order->bonew=1;
					order->unit=(*anarace)->getPhaenoCode(s);
					order->IP=s;
					order->checked=1;
					orderList.Append((long)(*anarace)->getMarker(s),order);
				}
			}
			k++;
		}
																			    
	NODE* node=orderList.GetFirst();
																			    
	while(node)
	{
		if(!node->GetData()->checked)
			node=orderList.DeleteNode(node);
		else
		{
			node->GetData()->checked=0;
			node=node->GetNext();
		}
	}
	orderList.Sort();	
};

void Player::MoveOrders() 
{
	NODE *node=orderList.GetFirst();
	while(node)
	{
		ORDER* order=node->GetData();		 
		move(order->rect.x,order->start.x,order->target.x);
		move(order->rect.y,order->start.y,order->target.y);
		move(order->rect.width,order->start.width,order->target.width);
		move(order->rect.height,order->start.height,order->target.height);

		move(order->brect.x,order->bstart.x,order->btarget.x);
		move(order->brect.y,order->bstart.y,order->btarget.y);
		move(order->brect.width,order->bstart.width,order->btarget.width);
		move(order->brect.height,order->bstart.height,order->btarget.height);

		move(order->blend, order->blendStart, order->blendTarget);
		node=node->GetNext();
	}
};


