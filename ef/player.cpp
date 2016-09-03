#include "player.hpp"

Player::~Player()
{
}

Player::Player(UI_Object* parent, ANARACE** anarace, const int playerNumber):UI_Object(parent)
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
	setMode(BASIC_TAB, playerNumber);
}


void Player::drawGeneString(DC* dc, const Rect position) const
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
}

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
}
#endif 
																				

void Player::draw(DC* dc) const
{
//	drawGeneString(dc,Rect(mainWindow->getPosition()+Point(0,20+i*(mainWindow->getClientRectHeight()/(UI_Object::settings.getMap(0)->getMaxPlayer()-1))),Size(mainWindow->getWidth(),mainWindow->getClientRectHeight()/(UI_Object::settings.getMap(0)->getMaxPlayer()-1)-40))); TODO
	
	UI_Object::draw(dc);
}

void Player::setMode(const eTab tab, const int playerNum)//, int player1, int player2)
{
	this->mode=tab*2+playerNum-2;
	for(int i=BUILD_ORDER_WINDOW;i<=INFO_WINDOW;i++)
		window[(eWindow)i]->setTitleParameter(*theme.lookUpString((eString)(tab+HIDE_MODE_STRING)));
	switch(mode)
	{
		case 0:this->Show();(*anarace)->setActive(1);break; // first player
		case 1:this->Hide();(*anarace)->setActive(0);break; // second player
		case 2:this->Show();(*anarace)->setActive(1);break; // first player
		case 3:this->Hide();(*anarace)->setActive(0);break; // second player
		case 4:this->Show();(*anarace)->setActive(1);break; // first player
		case 5:this->Show();(*anarace)->setActive(1);break; // second player
		case 6:this->Show();(*anarace)->setActive(1);break; // first player
		case 7:this->Show();(*anarace)->setActive(1);break; // second player
		case 8:this->Show();(*anarace)->setActive(1);break; // first player
		case 9:this->Show();(*anarace)->setActive(1);break; // second player
		case 10:this->Hide();(*anarace)->setActive(0);break; // first player
		case 11:this->Hide();(*anarace)->setActive(0);break; // second player
		case 12:this->Hide();(*anarace)->setActive(0);break; // first player
		case 13:this->Hide();(*anarace)->setActive(0);break; // second player
		case 14:this->Show();(*anarace)->setActive(1);break; // first player
		case 15:this->Hide();(*anarace)->setActive(0);break; // second player
		default:break;
	}
	// TODO modes der einzelnen Windows (z.B> timer oder force)
}

void Player::process()
{
	UI_Object::process();

// ------ COMMUNICATION BETWEEN THE WINDOWS ------ TODO
//	if(window[FORCE_WINDOW]->isShown())
//	{
//		window[BO_GRAPH_WINDOW]->setMarkedUnit(window[FORCE_WINDOW]->getMarkedUnit());
//		window[BUILD_ORDER_WINDOW]->setMarkedUnit(window[FORCE_WINDOW]->getMarkedUnit());
//	}
// ------ END COMMUNICATION BETWEEN THE WINDOWS ------

	
// ------ PROCESS MEMBER VARIABLES ------	
	MoveOrders();
// ------ END PROCESSING MEMBER VARIABLES ------
}

const bool Player::getChangedFlag() const
{
	for(int i=BUILD_ORDER_WINDOW;i<MAX_WINDOWS;i++)
		if(window[(eWindow)i]->getChangedFlag())
			return(1);
	return(0);
}

void Player::updateRectangles(const int maxPlayer)
{
	for(int i=BUILD_ORDER_WINDOW;i<MAX_WINDOWS;i++)
		window[(eWindow)i]->updateRectangles(maxPlayer);
}

const bool Player::isOptimizing() const
{
	return((*anarace)->isOptimizing());
}

void Player::setOptimizing(const bool opt)
{
	(*anarace)->setOptimizing(opt);
}

void Player::changeAccepted()
{
	for(int i=BUILD_ORDER_WINDOW;i<=INFO_WINDOW;i++)
		window[(eWindow)i]->changeAccepted();
	//if((*anarace)->getPlayer()->isChanged())  //TODO, Informationsfluss ist sehr unsauber!
	//	(*anarace)->getPlayer()->changeAccepted();
}


void Player::resetData()
{
	((BoWindow*)window[BUILD_ORDER_WINDOW])->resetData();
	((ForceWindow*)window[FORCE_WINDOW])->resetData();
	((BoGraphWindow*)window[BO_GRAPH_WINDOW])->resetData();
	((StatisticsWindow*)window[STATISTICS_WINDOW])->resetData();
	((BoDiagramWindow*)window[BO_DIAGRAM_WINDOW])->resetData();
	((TimerWindow*)window[TIMER_WINDOW])->resetData();
	((InfoWindow*)window[INFO_WINDOW])->resetData();
//	geneAnimation=0;
//	shown=0;
	orderList.clear();
}

void Player::checkForChange()
{
	if(window[FORCE_WINDOW]->getChangedFlag())
	{
		resetData();
		(*anarace)->resetData();
		(*(*anarace)->getCurrentGoal())->adjustGoals(true, (*   (*anarace)->getStartCondition()   )->getUnit(0) );
		window[FORCE_WINDOW]->changeAccepted();
	}
}



void Player::CheckOrders()
{
	int k=0;
																			   
	for(int s=MAX_LENGTH;s--;)
		if((*anarace)->getProgramIsBuilt(s)&&((*anarace)->getProgramTime(s)<=(*anarace)->getRealTimer()))
		{
			map<long, Order>::iterator order=orderList.find((*anarace)->getMarker(s)) ;// => found old one -> update the data!
			if(order!=orderList.end())
			{
				order->second.setRow(k+1);//+((orderList.getMakeSpace()>-1)*(k+1>=orderList.getMakeSpace())); TODO
				Rect t=Rect(0,order->second.getRow()*(FONT_SIZE+5),500-8,FONT_SIZE+4);
				if(order->second.target!=t)
					order->second.start=order->second.rect;
				order->second.target=t;
				order->second.blendTarget=50;
				order->second.blendStart=order->second.blend;
				order->second.setUnit((*anarace)->getPhaenoCode(s));
				order->second.setIP(s);
				order->second.checked=true;
				order->second.bonew=false;
			} // => aktualisieren
			else
			// => neues erstellen
			{
				//TODO: testen ob anderes item da ist, das aber die gleiche Unit besitzt + an die gleiche Position kommt
				bool found=false;
				map<long, Order>::iterator order=orderList.begin();
				while((!found) && (order!=orderList.end()))
				{
					if((order->second.getUnit()!=(*anarace)->getPhaenoCode(s))||(order->second.getRow()!=k+1)||(order->second.target.x>0))
						order++;
					else //=> ueberschreiben
					{
						found=true;
						Order neuorder;
						neuorder.setRow(k+1);//+((orderList.getMakeSpace()>-1)*(k+1>=orderList.getMakeSpace())); TODO
						Rect t=Rect(0,order->second.getRow()*(FONT_SIZE+5),500-8,FONT_SIZE+4);
						if(order->second.target!=t)
							neuorder.start=order->second.rect;
						else neuorder.start=order->second.start;
						neuorder.target=t;
						neuorder.blendTarget=50;
						neuorder.blendStart=order->second.blend;
						neuorder.setUnit((*anarace)->getPhaenoCode(s));
						neuorder.marker=order->second.marker;
						neuorder.checked=true;
						neuorder.bonew=true;
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
					neuorder.setRow(k+1);//+((orderList.getMakeSpace()>-1)*(k+1>=orderList.getMakeSpace()));
					neuorder.rect=Rect(170,neuorder.getRow()*(FONT_SIZE+5),0,FONT_SIZE+4);
					Rect t=Rect(0,neuorder.getRow()*(FONT_SIZE+5),500-8,FONT_SIZE+4);
					neuorder.start=neuorder.rect;
					neuorder.target=t;
					neuorder.blend=1;neuorder.blendTarget=50;
					neuorder.blendStart=neuorder.blend;
					neuorder.bonew=true;
					neuorder.setUnit((*anarace)->getPhaenoCode(s));
					neuorder.setIP(s);
					neuorder.checked=true;
					orderList.insert(pair<long, Order>((*anarace)->getMarker(s), neuorder));
				}
			}
			k++;
		}

		map<long, Order>::iterator order=orderList.begin();
		while(order!=orderList.end())
			if(!order->second.checked)
			{
				map<long, Order>::iterator temp=order;
				temp++;
				orderList.erase(order);
				order=temp;
			}
			else
			{
				order->second.checked=false;
				order++;
			}
}




void Player::MoveOrders() 
{
	for(map<long, Order>::iterator order=orderList.begin(); order!=orderList.end(); ++order)
	{
		move(order->second.rect, order->second.start, order->second.target);

		move(order->second.brect, order->second.bstart, order->second.btarget);

		move(order->second.blend, order->second.blendStart, order->second.blendTarget);
	}
}


