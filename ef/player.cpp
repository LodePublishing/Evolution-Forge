#include "player.hpp"

Player::~Player()
{ 
	for(int i=0;i<MAX_WINDOWS;i++)
		delete window[i];
}

Player::Player(UI_Object* player_parent, ANARACE** player_anarace, MessageWindow* msgWindow, const unsigned int playerNumber):
	UI_Object(player_parent),
	geneAnimation(0),
	anarace(player_anarace),
	orderList(),
	mode(0)
{
	for(int i=0;i<MAX_WINDOWS;i++)
		window[i]=NULL;
	window[FORCE_WINDOW] = new ForceWindow(this, *anarace, msgWindow, playerNumber);
	window[TIMER_WINDOW] = new TimerWindow(this, *anarace, playerNumber);
	window[STATISTICS_WINDOW]=new StatisticsWindow(this, *anarace, playerNumber);
	window[INFO_WINDOW] = new InfoWindow(this, *anarace, playerNumber);
	window[BUILD_ORDER_WINDOW] = new BoWindow(this, *anarace, (InfoWindow*)window[INFO_WINDOW], msgWindow, &orderList,/* fixed,*/ playerNumber);
	window[BO_GRAPH_WINDOW] = new BoGraphWindow(this, *anarace, (InfoWindow*)window[INFO_WINDOW], &orderList, playerNumber);
	window[BO_DIAGRAM_WINDOW]=new BoDiagramWindow(this, *anarace, (InfoWindow*)window[INFO_WINDOW], playerNumber);
	
	// to set infoWindow above all others
	window[INFO_WINDOW]->removeFromFamily(); window[INFO_WINDOW]->setParent(this);
	setOptimizing();
//	process();
//	return;
	Hide();
	setOptimizing(false);
//	for(int i=MAX_LENGTH;i--;)
//		fixed[i]=false;	
//	(*anarace)->setFixed(fixed);
}

void Player::assignAnarace(ANARACE** player_anarace)
{
	anarace = player_anarace;
	((ForceWindow*)window[FORCE_WINDOW])->assignAnarace(*anarace);
	((TimerWindow*)window[TIMER_WINDOW])->assignAnarace(*anarace);
	((StatisticsWindow*)window[STATISTICS_WINDOW])->assignAnarace(*anarace);
	((InfoWindow*)window[INFO_WINDOW])->assignAnarace(*anarace);
	((BoWindow*)window[BUILD_ORDER_WINDOW])->assignAnarace(*anarace);
	((BoGraphWindow*)window[BO_GRAPH_WINDOW])->assignAnarace(*anarace);
	((BoDiagramWindow*)window[BO_DIAGRAM_WINDOW])->assignAnarace(*anarace);
//	(*anarace)->setFixed(fixed);
}

void Player::drawGene(DC* dc, int k, const Point* points, const Point position, Pen& bla1, Pen& bla2) const
{
	if(points[0].y<points[1].y) dc->SetPen(bla1);else dc->SetPen(bla2);
	dc->DrawSpline(k, points, position);
	//dc->SetPen(bla2);
	dc->DrawSpline(k, points, position - Size(0,1));
	dc->DrawSpline(k, points, position + Size(0,1));
}

void Player::drawGeneString(DC* dc, const Rect position) const
{
#if 0
	int stringheight=0;
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
	int colors[MAX_LENGTH];													
	for(int i=0;i<MAX_LENGTH;i++)
		colors[i]=0;

	const Order* sortedList[MAX_LENGTH];
	int orderCount=0;
	for(map<long, Order>::const_iterator order=orderList.begin(); order!=orderList.end(); ++order) {
		sortedList[orderCount]=&((*order)->second);
		orderCount++;
	}
	if(orderCount<2)
		return;
	std::sort(sortedList, sortedList+orderCount, Order::OrderDescendingIPSort());

	for(int k=0;k<orderCount;k++)
		colors[k]=sortedList[k]->getUnit();
	
//	for(map<long, Order>::const_iterator order=orderList.begin(); order!=orderList.end(); ++order)
//		colors[(*order)->second.getIP()]=(*order)->second.getUnit();

//	if((*anarace)->isOptimizing())
	{
		int current_type=stats[(*anarace)->getRace()][colors[0]].facilityType;
		for(int i=0;i<2*orderCount;i++)
		{
//			int k=2;
/*			while((i<orderCount)&&(stats[(*anarace)->getRace()][colors[i]].facilityType==current_type))
			{
				i++;
				k++;
			}*/
			current_type=stats[(*anarace)->getRace()][colors[i/2]].facilityType;
			
			
/*		for(int i=0;i<MAX_LENGTH;i++)
			if(colors[i])
			{
				int k=1; // TODO
				while((i<MAX_LENGTH)&&((!colors[i])||((stats[(*anarace)->getRace()][colors[i]].facilityType)&&((current_type==-1)||(stats[(*anarace)->getRace()][colors[i]].facilityType==current_type)))))
				{
					i++;
					if(!colors[i]) continue;
					if(current_type==-1)
						current_type=stats[(*anarace)->getRace()][colors[i]].facilityType;
					k++;
				}
				current_type=stats[(*anarace)->getRace()][colors[i]].facilityType;*/
																				
				for(int j=0;j<2;j++)
				{
					points1[j] = Point(5+(stringheight+j)*(position.GetWidth()-8)/(orderCount*2)-1, (int)((cos((float)(4*(stringheight+j)+geneAnimation/2)*5.0*M_PI/200.0)*0.9*position.GetHeight()/2)+position.GetHeight()/2.1));
					
					points2[j] = Point(5+(stringheight+j)*(position.GetWidth()-8)/(orderCount*2)-1, (int)((sin(((float)(4*(stringheight+j)+geneAnimation/2)+13.0)*5.0*M_PI/200.0)*0.9*position.GetHeight()/2)+position.GetHeight()/2.1));
					
					points3[j] = Point(5+(stringheight+j)*(position.GetWidth()-8)/(orderCount*2)-1, (int)((cos(((float)(4*(stringheight+j)+geneAnimation/2)+26.0)*5.0*M_PI/200.0)*0.9*position.GetHeight()/2)+position.GetHeight()/2.1));
					
					points4[j] = Point(5+(stringheight+j)*(position.GetWidth()-8)/(orderCount*2)-1, (int)((sin(((float)(4*(stringheight+j)+geneAnimation/2)+39.0)*5.0*M_PI/200.0)*0.9*position.GetHeight()/2)+position.GetHeight()/2.1));
				} //end for(j=0;j<k;j++)
				stringheight+=1;
//				k++;
//				if(k>=1)
				{
						
					Pen bla1=Pen(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN+stats[(*anarace)->getRace()][colors[i/2]].unitType)));
					Pen bla2=Pen(*theme.lookUpPen((ePen)(UNIT_TYPE_0_PEN+stats[(*anarace)->getRace()][colors[i/2]].unitType)));
					int k=2;
					if(points1[0].y>points1[1].y) // faellt -> hinten
					{
						if(points1[0].y>points2[0].y)
						{
							drawGene(dc, k, points1, position.GetTopLeft(), bla1, bla2);
							drawGene(dc, k, points2, position.GetTopLeft(), bla1, bla2);
						}
						else
						{
							drawGene(dc, k, points2, position.GetTopLeft(), bla1, bla2);
							drawGene(dc, k, points1, position.GetTopLeft(), bla1, bla2);
						}
						if(points3[0].y>points4[0].y)
						{
							drawGene(dc, k, points3, position.GetTopLeft(), bla1, bla2);
							drawGene(dc, k, points4, position.GetTopLeft(), bla1, bla2);
						} else
						{
							drawGene(dc, k, points4, position.GetTopLeft(), bla1, bla2);
							drawGene(dc, k, points3, position.GetTopLeft(), bla1, bla2);
						}
					} else
					{
						if(points3[0].y>points4[0].y)
						{
							drawGene(dc, k, points3, position.GetTopLeft(), bla1, bla2);
							drawGene(dc, k, points4, position.GetTopLeft(), bla1, bla2);
						} else
						{
							drawGene(dc, k, points4, position.GetTopLeft(), bla1, bla2);
							drawGene(dc, k, points3, position.GetTopLeft(), bla1, bla2);
						}
						if(points1[0].y>points2[0].y)
						{
							drawGene(dc, k, points1, position.GetTopLeft(), bla1, bla2);
							drawGene(dc, k, points2, position.GetTopLeft(), bla1, bla2);
						}
						else
						{
							drawGene(dc, k, points2, position.GetTopLeft(), bla1, bla2);
							drawGene(dc, k, points1, position.GetTopLeft(), bla1, bla2);
						}
					
					}
					k--;
				}
			} //end blend
	} //end if(isOptimizing)
#endif
}
																			

void Player::draw(DC* dc) const
{
	if(!isShown())
		return;
	// TODO
	UI_Object::draw(dc);
	if(configuration.isDnaSpiral())
	{
		Rect r = Rect(getAbsolutePosition()+Point(210, 200+(*anarace)->getPlayerNumber()*300), Size(600, 120));
		dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
		dc->SetPen(*theme.lookUpPen(BRIGHT_UNIT_TYPE_1_PEN));
		dc->DrawRectangle(r);
		drawGeneString(dc, r);
	};
	((ForceWindow*)window[FORCE_WINDOW])->drawTechTree(dc);
}

void Player::setMode(const eTab tab, const unsigned int playerNum)//, int player1, int player2)
{
	this->mode=tab*2+playerNum-2;
	for(int i=BUILD_ORDER_WINDOW;i<=INFO_WINDOW;i++)
		window[(eWindow)i]->setTitleParameter(*theme.lookUpString((eString)(tab+HIDE_MODE_STRING)));
	switch(mode)
	{
		case 0:
			this->Show();
			(*anarace)->setActive(1);
			window[BO_DIAGRAM_WINDOW]->Hide();
			window[STATISTICS_WINDOW]->Hide();
			window[BO_GRAPH_WINDOW]->Hide();
		break; // first player basic mode
		
		case 1:
			this->Hide();(*anarace)->setActive(0);break; // second player basic mode
		case 2:
			this->Show();
			(*anarace)->setActive(1);
			window[BO_DIAGRAM_WINDOW]->Show();
			window[STATISTICS_WINDOW]->Show();
			window[BO_GRAPH_WINDOW]->Show();
		break; // first player advanced mode
		case 3:this->Hide();(*anarace)->setActive(0);break; // second player advanced mode
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
	resetData();
	// TODO modes der einzelnen Windows (z.B> timer oder force)
}

void Player::process()
{
	if(!isShown())
		return;
	window[INFO_WINDOW]->Hide();
	UI_Object::process();
// TODO!
// ------ COMMUNICATION BETWEEN THE WINDOWS ------ TODO
/*	if(window[FORCE_WINDOW]->isShown())
	{
		int markedUnit=((ForceWindow*)window[FORCE_WINDOW])->getMarkedUnit();
		int markedIP=((BoGraphWindow*)window[BO_GRAPH_WINDOW])->getMarkedIP();
		if(!markedIP)
			markedIP=((BoWindow*)window[BUILD_ORDER_WINDOW])->getMarkedIP();

		((BoGraphWindow*)window[BO_GRAPH_WINDOW])->setMarkedUnit(markedUnit);
//	((BoWindow*)window[BUILD_ORDER_WINDOW])->setMarkedUnit(markedUnit);
		
		((BoGraphWindow*)window[BO_GRAPH_WINDOW])->setMarkedIP(markedIP);
//		((BoWindow*)window[BUILD_ORDER_WINDOW])->setMarkedIP(markedIP);
	
		if(!markedUnit)
			markedUnit=((BoGraphWindow*)window[BO_GRAPH_WINDOW])->getMarkedUnit();
		if(!markedUnit)
			markedUnit=((BoWindow*)window[BUILD_ORDER_WINDOW])->getMarkedUnit();
		((ForceWindow*)window[FORCE_WINDOW])->setMarkedUnit(markedUnit);
		if(!markedIP)
			window[INFO_WINDOW]->Hide();
	}*/
// ------ END COMMUNICATION BETWEEN THE WINDOWS ------

	if((*anarace)->isOptimizing())
		geneAnimation++;
// ------ PROCESS MEMBER VARIABLES ------	
//	MoveOrders();
// ------ END PROCESSING MEMBER VARIABLES ------
}

void Player::updateRectangles(const unsigned int maxPlayer)
{
	for(int i=BUILD_ORDER_WINDOW;i<=INFO_WINDOW;i++)
		window[(eWindow)i]->updateRectangles(maxPlayer);
}

const bool Player::isOptimizing() const
{
	return((*anarace)->isOptimizing());
}

void Player::setOptimizing(const bool opt)
{
	(*anarace)->setOptimizing(opt);
	((TimerWindow*)window[TIMER_WINDOW])->forcePause(opt);
}

void Player::resetData()
{
	((BoWindow*)window[BUILD_ORDER_WINDOW])->resetData();
//	((ForceWindow*)window[FORCE_WINDOW])->resetData(); ?
	((BoGraphWindow*)window[BO_GRAPH_WINDOW])->resetData();
	((StatisticsWindow*)window[STATISTICS_WINDOW])->resetData();
	((BoDiagramWindow*)window[BO_DIAGRAM_WINDOW])->resetData();
	((TimerWindow*)window[TIMER_WINDOW])->resetData();
	((InfoWindow*)window[INFO_WINDOW])->resetData();
	geneAnimation=0;
//	Hide();
        list<Order*>::iterator order = orderList.begin();
        while(order != orderList.end())
        {
                delete(*order);
                order = orderList.erase(order);
        }
	//orderList.clear();
}

void Player::restartAnarace()
{
	(*anarace)->restartData();
}

void Player::CheckOrders()
{
	list<Order*>::iterator order = orderList.begin();
        while(order != orderList.end())
        {
                delete(*order);
                order = orderList.erase(order);
        }
	order = orderList.begin();

	for(int s=MAX_LENGTH;s--;)
		if((*anarace)->getProgramIsBuilt(s)&&((*anarace)->getRealProgramTime(s) + stats[(*anarace)->getRace()][(*anarace)->getPhaenoCode(s)].BT<=(*anarace)->getRealTimer()))
		{
			if(order == orderList.end()) // => insert at the back
			{
				Order* new_order = new Order();
				new_order->setUnit((*anarace)->getPhaenoCode(s));
				new_order->setIP(s);
				new_order->setMarker((*anarace)->getMarker(s));
				orderList.push_back(new_order);
			} else 
			if((*order)->getMarker() != (*anarace)->getMarker(s))
			{
				std::list<Order*>::iterator i = order;
				while(i != orderList.end())
				{
					if((*i)->getMarker() == (*anarace)->getMarker(s))
						break;
					i++;
				}
				if(i != orderList.end()) // Found, Move the entry
				{
					Order* old = *i;
					orderList.insert(order, old);
					order = orderList.erase(i);
					old->setUnit((*anarace)->getPhaenoCode(s));
					old->setIP(s);
				} else // => not found, insert a new one
				{
					Order* new_order = new Order();
					new_order->setUnit((*anarace)->getPhaenoCode(s));
					new_order->setIP(s);
					new_order->setMarker((*anarace)->getMarker(s));
					orderList.insert(order, new_order);
				}
			} else  // ok
			{
				(*order)->setIP(s);
				order++;
			}
		}

	while(order != orderList.end())
	{
		delete(*order);
		order = orderList.erase(order);
	}

		#if 0	
			while((s) && (order != orderList.end()) && ((*order)->getMarker() == (*anarace)->getMarker(s)))
			{
			// update
				(*order)->setRow(k+1);
				Rect t = Rect(0, (k+1)*(FONT_SIZE+7), 500-8, FONT_SIZE+6);
				if((*order)->target!=t)
				{
					(*order)->start = (*order)->rect;
					(*order)->target = t;
					(*order)->blendTarget = 50;
					(*order)->blendStart = (*order)->blend;
					(*order)->setUnit((*anarace)->getPhaenoCode(s));
				}
				(*order)->setIP(s);
				s--;
				order++;
				k++;
			}

			list<Order*>::iterator i = order;
			while(i != orderList.end())
			{
				if((*i)->getMarker() == (*anarace)->getMarker(s))
					break;
				i++;
			}
			if(i == orderList.end()) // => not found, insert new
			{ 
				Order* new_order = new Order();
				new_order->setRow(k+1);
				new_order->rect = Rect(170, new_order->getRow()*(FONT_SIZE+7), 0, FONT_SIZE+6);
				Rect t = Rect(0, new_order->getRow()*(FONT_SIZE+7), 500-8, FONT_SIZE+6);
				new_order->start = new_order->rect;
				new_order->target = t;
				new_order->blend = 1;
				new_order->blendTarget = 50;
				new_order->blendStart = new_order->blend;
				new_order->setUnit((*anarace)->getPhaenoCode(s));
				new_order->setIP(s);
				new_order->setMarker((*anarace)->getMarker(s));
				orderList.insert(order, new_order);
				k++;
			} else
// update and swap
			{
				Order* old = *i;
				orderList.erase(i);
				orderList.insert(order, old);

				old->setRow(k+1);
				Rect t = Rect(0, (k+1)*(FONT_SIZE+7), 500-8, FONT_SIZE+6);
				if(old->target != t)
				{
					old->start = old->rect;
					old->target = t;
				}
				old->blendTarget = 50;
				old->blendStart = (*i)->blend;
				old->setUnit((*anarace)->getPhaenoCode(s));
				old->setIP(s);
				k++;
			}
		}
#endif
/*

			
			map<long, Order>::iterator order = orderList.find((*anarace)->getMarker(s)) ;// => found old one -> update the data!
			if(order!=orderList.end())
			{
				(*order)->second.setRow(k+1);//+((orderList.getMakeSpace()>-1)*(k+1>=orderList.getMakeSpace())); TODO
				//Rect t=Rect(((*order)->second.getRow()%2)*(window[BUILD_ORDER_WINDOW]->getWidth()/2), ((*order)->second.getRow()/2)*(FONT_SIZE+6),(window[BUILD_ORDER_WINDOW]->getWidth()-8)/2,FONT_SIZE+4);
				Rect t=Rect(0,(*order)->second.getRow()*(FONT_SIZE+7),500-8,FONT_SIZE+6);
				if((*order)->second.target!=t)
					(*order)->second.start=(*order)->second.rect;
				(*order)->second.target=t;
				(*order)->second.blendTarget=50;
				(*order)->second.blendStart=(*order)->second.blend;
				(*order)->second.setUnit((*anarace)->getPhaenoCode(s));
				(*order)->second.setIP(s);
				(*order)->second.checked=true;
				(*order)->second.bonew=false;
			} // => aktualisieren
			else
			// => neues erstellen
			{
				//TODO: testen ob anderes item da ist, das aber die gleiche Unit besitzt + an die gleiche Position kommt
				bool found=false;
				map<long, Order>::iterator current_order = orderList.begin();
				while((!found) && (current_order != orderList.end()))
				{
					if((current_(*order)->second.getUnit()!=(*anarace)->getPhaenoCode(s))||(current_(*order)->second.getRow()!=k+1)||(current_(*order)->second.target.GetLeft()>0)||(s!=current_(*order)->second.getIP()))
						current_order++;
					else //=> ueberschreiben
					{
						found=true;
						Order neuorder;
						neuorder.setRow(k+1);//+((orderList.getMakeSpace()>-1)*(k+1>=orderList.getMakeSpace())); TODO
//						Rect t=Rect(((*order)->second.getRow()%2)*(window[BUILD_ORDER_WINDOW]->getWidth()/2), ((*order)->second.getRow()/2)*(FONT_SIZE+6),(window[BUILD_ORDER_WINDOW]->getWidth()-8)/2,FONT_SIZE+4);
						Rect t=Rect(0, current_(*order)->second.getRow()*(FONT_SIZE+7), 500-8, FONT_SIZE+6);
						if(current_(*order)->second.target != t)
							neuorder.start = current_(*order)->second.rect;
						else neuorder.start = current_(*order)->second.start;
						neuorder.target = t;

						neuorder.brect = current_(*order)->second.brect;
						neuorder.bstart = current_(*order)->second.bstart;
						neuorder.btarget = current_(*order)->second.btarget;
						
						neuorder.blendTarget = 50;
						neuorder.blendStart = current_(*order)->second.blend;
						neuorder.setUnit((*anarace)->getPhaenoCode(s));
						neuorder.marker = current_(*order)->second.marker;
						neuorder.checked=true;
						neuorder.bonew=true;
						orderList.insert(pair<long, Order>((*anarace)->getMarker(s), neuorder));
						
						map<long, Order>::iterator temp = current_order;
						temp++;
						orderList.erase(current_order);
						current_order = temp;
					}
				}
				if((!found)&&(k+1<MAX_LENGTH)) // neues erstellen!
				{
					Order neuorder;
					neuorder.setRow(k+1);//+((orderList.getMakeSpace()>-1)*(k+1>=orderList.getMakeSpace()));
					neuorder.rect=Rect(170, neuorder.getRow()*(FONT_SIZE+7), 0, FONT_SIZE+6);
					//Rect t=Rect(((*order)->second.getRow()%2)*(window[BUILD_ORDER_WINDOW]->getWidth()/2), ((*order)->second.getRow()/2)*(FONT_SIZE+6),(window[BUILD_ORDER_WINDOW]->getWidth()-8)/2,FONT_SIZE+4);
					Rect t=Rect(0, neuorder.getRow()*(FONT_SIZE+7), 500-8, FONT_SIZE+6);
					neuorder.start = neuorder.rect;
					neuorder.target = t;
					neuorder.blend = 1;neuorder.blendTarget = 50;
					neuorder.blendStart = neuorder.blend;
					neuorder.bonew = true;
					neuorder.setUnit((*anarace)->getPhaenoCode(s));
					neuorder.setIP(s);
					neuorder.checked=true;
					orderList.insert(pair<long, Order>((*anarace)->getMarker(s), neuorder));
				}
			}
			k++;
		}*/

		((BoWindow*)(window[BUILD_ORDER_WINDOW]))->processList();
		((BoGraphWindow*)(window[BO_GRAPH_WINDOW]))->processList();
		((ForceWindow*)(window[FORCE_WINDOW]))->processList();

}


#if 0
// ----- MOVE ORDERS IN BOWINDOW AND BOGRAPH -----
void Player::MoveOrders() 
{
/*	for(map<long, Order>::iterator order=orderList.begin(); order!=orderList.end(); ++order)
	{
		(*order)->second.rect.move((*order)->second.start, (*order)->second.target);
		(*order)->second.brect.move((*order)->second.bstart, (*order)->second.btarget);
		Size::mv((*order)->second.blend, (*order)->second.blendStart, (*order)->second.blendTarget);
	}*/
	for(list<Order*>::iterator order=orderList.begin(); order!=orderList.end(); ++order)
	{
//		(*order)->rect.move((*order)->start, (*order)->target);
//		(*order)->brect.move((*order)->bstart, (*order)->btarget);
//		Size::mv((*order)->blend, (*order)->blendStart, (*order)->blendTarget);
	}
}
#endif

