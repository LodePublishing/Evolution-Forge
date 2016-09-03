#include "player.h"
#include "math.h"

void Player::drawGeneString(wxDC* dc, wxRect position)
{
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
				while((i<MAX_LENGTH)&&((!colors[i])||((stats[(*anarace)->getPlayer()->getRace()][colors[i]].type)&&((currentType==-1)||(stats[(*anarace)->getPlayer()->getRace()][colors[i]].type==currentType)))))
				{
					i++;
					if(!colors[i]) continue;
					if(currentType==-1)
						currentType=stats[(*anarace)->getPlayer()->getRace()][colors[i]].type;
					k++;
				}
				currentType=stats[(*anarace)->getPlayer()->getRace()][colors[i]].type;
																			    
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
3*(COLOR1R+2*BOcolor[stats[(*anarace)->getPlayer()->getRace()][colors[i-1]].type].Red())/4,
3*(COLOR1G+2*BOcolor[stats[(*anarace)->getPlayer()->getRace()][colors[i-1]].type].Green())/4,
3*(COLOR1B+2*BOcolor[stats[(*anarace)->getPlayer()->getRace()][colors[i-1]].type].Blue())/4),3,wxSOLID);
					wxPen bla2=wxPen(wxColour(
					3*(COLOR1R/2+BOcolor[stats[(*anarace)->getPlayer()->getRace()][colors[i-1]].type].Red())/10,
					3*(COLOR1G/2+BOcolor[stats[(*anarace)->getPlayer()->getRace()][colors[i-1]].type].Green())/10,
					3*(COLOR1B/2+BOcolor[stats[(*anarace)->getPlayer()->getRace()][colors[i-1]].type].Blue())/10),1,wxSOLID);
																			    
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
};

void Player::InitPositions(GraphixScrollWindow* mainWindow)
{
// 1 Player: 1st player [BASIC]
	boWindowRect[0]=wxRect(wxPoint(mainWindow->getInnerWidth()-THIRD_COLOUMN,SECOND_ROW),wxSize(THIRD_COLOUMN,6*(FONT_SIZE+5)+3));
	boWindowMax[0]=wxRect(boWindowRect[0].GetPosition(),wxSize(boWindowRect[0].GetWidth(),mainWindow->getInnerHeight()-30-120));
	forceWindowRect[0]=wxRect(0,SECOND_ROW,FIRST_COLOUMN,14*(FONT_SIZE+5));
//TODO ne Art * Operator reinbringen, damit die Hoehe genommen wird, die die entsprechende Drawroutine berechnet
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
																			    
	boGraphWindowRect[3]=wxRect(FIRST_COLOUMN,555,SECOND_COLOUMN,40);
	boGraphWindowMax[3]=wxRect(boGraphWindowRect[3].GetPosition(),wxSize(boGraphWindowRect[3].GetWidth(),200));



// 2 Player: 1st player [GOSU], the human - freestyle
	boWindowRect[4]=wxRect(wxPoint(mainWindow->getInnerWidth()-THIRD_COLOUMN,0),wxSize(THIRD_COLOUMN,6*(FONT_SIZE+5)+3));
	boWindowMax[4]=wxRect(boWindowRect[4].GetPosition(),wxSize(boWindowRect[4].GetWidth(),mainWindow->getInnerHeight()*2/3));
																			    
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
	boWindowMax[5]=wxRect(boWindowRect[5].GetPosition(),wxSize(boWindowRect[5].GetWidth(),mainWindow->getInnerHeight()*2/3));
																			    
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
	boWindowMax[6]=wxRect(boWindowRect[6].GetPosition(),wxSize(boWindowRect[6].GetWidth(),mainWindow->getInnerHeight()*2/3));
																			    
	forceWindowRect[6]=wxRect(0,SECOND_ROW,FIRST_COLOUMN,14*(FONT_SIZE+5));
	forceWindowMax[6]=wxRect(forceWindowRect[6].GetPosition(),wxSize(FIRST_COLOUMN,mainWindow->getInnerHeight()/3-5));
																			    
	timerWindowRect[6]=wxRect(wxPoint(mainWindow->getInnerWidth()-3*THIRD_COLOUMN,0),wxSize(THIRD_COLOUMN,120));
	timerWindowMax[6]=timerWindowRect[6];

	boDiagramWindowRect[6]=wxRect(wxPoint(0,2*mainWindow->getInnerHeight()/3+5),wxSize(SECOND_COLOUMN,110));
	boDiagramWindowMax[6]=boDiagramWindowRect[6];
																			    
	boGraphWindowRect[6]=wxRect(0,2*mainWindow->getInnerHeight()/3+115,SECOND_COLOUMN,40);
	boGraphWindowMax[6]=wxRect(boGraphWindowRect[6].GetPosition(),wxSize(boGraphWindowRect[6].GetWidth(),200));

	infoWindowRect[0]=wxRect(wxPoint(boWindowRect[0].GetX()-200,0),wxSize(200,5+9*(FONT_SIZE+5)));
	infoWindowMax[0]=infoWindowRect[0];//wxRect(wxPoint(boWindowRect[0].x-200/*theCore->getLeftBound()+theCore->getWidth()-200*/,0),infoWindowRect[0].GetSize());

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
																			    
Player::Player(ANARACE** anarace, int mode)
{
	this->anarace=anarace;
	boWindow=new BoWindow(boWindowRect[0],boWindowMax[0]);
	forceWindow=new ForceWindow(forceWindowRect[0],forceWindowMax[0]);
	timerWindow=new TimerWindow(timerWindowRect[0],timerWindowMax[0]);
	statisticsWindow=new StatisticsWindow(statisticsWindowRect[0],statisticsWindowMax[0]);
	boDiagramWindow=new BoDiagramWindow(boDiagramWindowRect[0],boDiagramWindowMax[0]);
	boGraphWindow=new BoGraphWindow(boGraphWindowRect[0],boGraphWindowMax[0]);
	infoWindow=new InfoWindow(infoWindowRect[0],infoWindowMax[0]);

	boGraphWindow->assignInfoWindow(infoWindow);
	boWindow->assignInfoWindow(infoWindow);
	boDiagramWindow->assignInfoWindow(infoWindow);

//	boGraphWindow->setOrderList(&orderList);
//	boWindow->setOrderList(&orderList);
	
	assignAnarace(anarace);
	setTitles(mode);

	//Show(0);
	resetData();
};

void Player::setTitles(int mode)
{
	wxString bla;
	switch(mode)
	{
		case 0:bla=_T("player");break;
		case 1:bla=_T("1. computer enemy");break;
		case 2:bla=_T("2. computer enemy");break;
		default:bla=_T("ERROR");break;
	};

	infoWindow->setTitle(0,"Order description");
	boGraphWindow->setTitle(0,_T(wxString::Format(wxT("Graphical build order"+bla))));
	boDiagramWindow->setTitle(0,_T(wxString::Format(wxT("Overview "+bla))));
	statisticsWindow->setTitle(0,_T(wxString::Format(wxT("Statistics "+bla))));
	timerWindow->setTitle(0,_T(wxString::Format(wxT("Best of breed "+bla))));
	forceWindow->setTitle(0,_T(wxString::Format(wxT("Units list "+bla))));
	boWindow->setTitle(0,_T(wxString::Format(wxT("Build Order "+bla))));
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

//TODO check ob wir den Type nicht schon vorliegen haben
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
//		infoWindow->Show(show[type][2]);
		boDiagramWindow->Show(show[type][3]);
		boGraphWindow->Show(show[type][4]);
       	timerWindow->Show(show[type][5]);
		statisticsWindow->Show(show[type][6]);
//		(*anarace)->setActive(1);
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
		case 3:setTitles(0);break;
		case 4:setTitles(1);break;
		case 5:setTitles(0);break;
		case 6:setTitles(1);break;
		case 7:setTitles(2);break;
		default:break;
	}
//	(*anarace)->setActive(1);
};

void Player::processButtons()
{
	if(boWindow->isShown()) boWindow->processButtons();
	if(forceWindow->isShown()) forceWindow->processButtons();
//	if(infoWindow->isShown()) infoWindow->processButtons();
//if(boDiagramWindow->isShown()) boDiagramWindow->processButtons();
//	if(boGraphWindow->isShown()) boGraphWindow->processButtons();
	if(timerWindow->isShown()) timerWindow->processButtons();
//	if(statisticsWindow->isShown()) statisticsWindow->processButtons();	
};

int Player::hasChanged()
{
	if(forceWindow->hasChanged())
	{
		forceWindow->resetData();
		return(1);
	}
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
	//if((*anarace)->getPlayer()->isChanged())  //TODO, Informationsfluss ist sehr unsauber!
	//	(*anarace)->getPlayer()->changeAccepted();

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
	boWindow->resetData();
	forceWindow->resetData();
	boGraphWindow->resetData();
	statisticsWindow->resetData();
	boDiagramWindow->resetData();
	timerWindow->resetData();
	infoWindow->resetData();
	geneAnimation=0;
	shown=0;
	orderList.Clear();
};

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
				order->row=k+1+((boWindow->makeSpace>-1)*(k+1>=boWindow->makeSpace));
				wxRect t=wxRect(0,order->row*(FONT_SIZE+5),SECOND_COLOUMN-8,FONT_SIZE+4);
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
						order->row=k+1+((boWindow->makeSpace>-1)*(k+1>=boWindow->makeSpace)); //?
						wxRect t=wxRect(0,order->row*(FONT_SIZE+5),SECOND_COLOUMN-8,FONT_SIZE+4);
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
					order->row=k+1+((boWindow->makeSpace>-1)*(k+1>=boWindow->makeSpace));
					order->rect=wxRect(170,order->row*(FONT_SIZE+5),0,FONT_SIZE+4);
					wxRect t=wxRect(0,order->row*(FONT_SIZE+5),SECOND_COLOUMN-8,FONT_SIZE+4);
					if(order->target!=t)
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


void Player::CheckForInfoWindow()
{
	NODE *node=orderList.GetFirst();
	while(node)
	{
		ORDER* order=node->GetData();
//	      int row=((boInsertPoint>-1)&&(order->row>=boInsertPoint))*(boEndPoint-boInsertPoint);
		if(boWindow->mouseOnOrder(order)||(boGraphWindow->mouseOnOrder(order)))
		{
//infoWindow->adjustWindow(wxRect(wxPoint(infoWindow->getPosition().x,500)/*order->rect.GetY()+getInnerUpperBound()-getScrollY()+((boInsertPoint>-1)&&(order->row>=boInsertPoint))*(boEndPoint-boInsertPoint)*(FONT_SIZE+5)-1)*/,infoWindow->getSize()));
			infoWindow->setUnit(order->unit);
			infoWindow->setKey(node->GetKey());
			infoWindow->setOrder(order);
			infoWindow->setBx(order->brect.x);
			infoWindow->setBWidth(order->brect.width);
			infoWindow->Show(1);
			return;
		}
		node=node->GetNext();
	}
	infoWindow->Show(0);
};



																			    
void Player::DrawMe(wxDC* dc)
{
	MoveOrders();
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

	if(forceWindow->isShown())
	{
		forceWindow->Draw(dc);
		forceWindow->drawGoalList(dc);
	}


	if(boGraphWindow->isShown())
	{
		if(forceWindow->isShown())
			boGraphWindow->setMarkedUnit(forceWindow->getMarkedUnit());
		boGraphWindow->Draw(dc);
		boGraphWindow->showBoGraph(dc, &orderList);
	}
																			    
	if(boWindow->isShown())
	{
        if(forceWindow->isShown())
	            boWindow->setMarkedUnit(forceWindow->getMarkedUnit());
		dc->SetTextForeground(wxColour(255,0,0));
		boWindow->Draw(dc);
		boWindow->drawSelectionStuff(dc);
		boWindow->drawBuildOrder(dc, &orderList);
	}

	if(boDiagramWindow->isShown())
	{
		boDiagramWindow->Draw(dc);
		boDiagramWindow->showProgramGraph(dc);
	}

	
	if(infoWindow->isShown())
	{
		infoWindow->Draw(dc);
		infoWindow->drawInfo(dc);
	};
};

void Player::update()
{
	int t=0;
	forceWindow->maxUnitForce=4;
	for(int i=0;i<=GAS_SCV;i++)
	{
		if((*anarace)->getLocationForce(0,i)>forceWindow->maxUnitForce)
			forceWindow->maxUnitForce=(*anarace)->getLocationForce(0,i);
		if((*anarace)->getPlayer()->getGoal()->allGoal[i]>forceWindow->maxUnitForce)
			forceWindow->maxUnitForce=(*anarace)->getPlayer()->getGoal()->allGoal[i];
		t+=(*anarace)->getLocationForce(0,i);;
	}
	if(t>statisticsWindow->maxForce) statisticsWindow->maxForce=t;
	statisticsWindow->currentForce=t;
	if(statisticsWindow->isShown()) //they are sorted there, otherwise we get an infinite loop
		statisticsWindow->ProgressGraph();
	CheckOrders();
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



