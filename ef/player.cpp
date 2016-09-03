#include "player.hpp"

Player::~Player()
{
}

Player::Player(UI_Object* player_parent, ANARACE** player_anarace, MessageWindow* msgWindow, const unsigned int playerNumber):
	UI_Object(player_parent),
	anarace(player_anarace),
	geneAnimation(0),
	orderList(),
	mode(0)
{
	Hide();
	window[FORCE_WINDOW] = new ForceWindow(this, *anarace, msgWindow, playerNumber);
	window[TIMER_WINDOW] = new TimerWindow(this, *anarace, playerNumber);
	window[STATISTICS_WINDOW]=new StatisticsWindow(this, *anarace, playerNumber);
	window[INFO_WINDOW] = new InfoWindow(this, *anarace, playerNumber);
	window[BUILD_ORDER_WINDOW]=new BoWindow(this, *anarace, (InfoWindow*)window[INFO_WINDOW], &orderList, playerNumber);
	window[BO_GRAPH_WINDOW] = new BoGraphWindow(this, *anarace, (InfoWindow*)window[INFO_WINDOW], &orderList, playerNumber);
	window[BO_DIAGRAM_WINDOW]=new BoDiagramWindow(this, *anarace, (InfoWindow*)window[INFO_WINDOW], playerNumber);
	// to set infoWindow above all others
	window[INFO_WINDOW]->removeFromFamily(); window[INFO_WINDOW]->setParent(this);
	setMode(BASIC_TAB, playerNumber);
}

void Player::processBoGraph()
{
	if((!window[BO_GRAPH_WINDOW]->isShown())||(!isShown()))
		return;
    unsigned int fac[20];
    unsigned int unitCounter[UNIT_TYPE_COUNT][MAX_LENGTH];
    unsigned int height[UNIT_TYPE_COUNT];
    unsigned int lines[UNIT_TYPE_COUNT];
    unsigned int faccount=0;
	int lastbographY=0;
                                                                                                                                                            
    for(int i=UNIT_TYPE_COUNT;i--;)
    {
        for(int j=0;j<MAX_LENGTH;j++)
            unitCounter[i][j]=0;
        height[i]=0;
        lines[i]=0;
    }

	BOGRAPH bograph[20];
	for(int i=20;i--;)
        fac[i]=0; // TODO in BOGRAPH?

//calculate maximum height per facility => height is after that the maximum of force - availible for each facility
    for(map<long, Order>::const_iterator order=orderList.begin(); order!=orderList.end(); ++order)
    {
        unsigned int IP=order->second.getIP();
// falls facility benoetigt wird und Zahl der zu dem Zeitpunkt vorhandenen Einheiten minus der verfuegbaren Einheiten > hoehe => setze hoehe auf diesen Wert        
		if((*anarace)->getProgramFacility(IP)&&
          ((*anarace)->getProgramTotalCount(IP, (*anarace)->getProgramFacility(IP))-(*anarace)->getProgramAvailibleCount(IP,(*anarace)->getProgramFacility(IP))>height[(*anarace)->getProgramFacility(IP)]))
             height[(*anarace)->getProgramFacility(IP)]=(*anarace)->getProgramTotalCount(IP,(*anarace)->getProgramFacility(IP))-(*anarace)->getProgramAvailibleCount(IP,(*anarace)->getProgramFacility(IP));
        // total - availible at that time equals used facilities
    }
    //=>  height[i] = max used i-facilities
                                                                                                                                                            
//calculate number of lines per facility and adjust the height
    for(int i=UNIT_TYPE_COUNT;i--;)
    {
// at maximum MIN_HEIGHT items in one row
        while(height[i]>MIN_HEIGHT) {
            height[i]-=MIN_HEIGHT;
            lines[i]++;
        }
        if(height[i]>0)
            lines[i]++;
        if(lines[i]>1)
            height[i]=MIN_HEIGHT;
    }
                                                                                                                                                            
//make a list of facilities that are needed...
// TODO WARUM = 1 und nicht = 0?
    faccount=1;
    for(map<long, Order>::const_iterator order=orderList.begin(); order!=orderList.end(); ++order)
    {
        if((*anarace)->getProgramFacility(order->second.getIP()))
        {
            unsigned int i;
            // search for 'untaken' facilities
            for(i=1;(i<faccount)&&(fac[i]!=(*anarace)->getProgramFacility(order->second.getIP()));i++);
                                                                                                                                                            
            if(i==faccount)
            {
                fac[i]=(*anarace)->getProgramFacility(order->second.getIP());
                faccount++;
            }
        }
    }
                                                                                                                                                            
// ...and sort them (just an optical issue, scvs last)
    for(int i=0;i<20;i++)
        if(fac[i])
            for(int j=0;j<i;j++)
                if((fac[j]>0)&&(fac[i]>fac[j])) {
                        int temp=fac[i];fac[i]=fac[j];fac[j]=temp;
                    }
                                                                                                                                                            
// now put all together
    int position=0;
    for(int i=0;i<20;i++)
    {
        bograph[position].type=fac[i];
        bograph[position].lines=lines[fac[i]];
        bograph[position].height=height[fac[i]];
        position+=lines[fac[i]];
    }
                                                                                                                                                            
// create a sorted by IP - list
    int orderCount=0;
	Order* sortedList[MAX_LENGTH];
    for(map<long, Order>::iterator order=orderList.begin(); order!=orderList.end(); ++order) {
        sortedList[orderCount]=&(order->second);
        orderCount++;
    }
                                                                                                                                                            
    for(int i=0;i<orderCount;i++)
        for(int j=0;j<i;j++)
            if(sortedList[i]->getIP() > sortedList[j]->getIP())
                // > =>  earlier! first IP is MAX_LENGTH
            {
                Order* temp = sortedList[i];
                sortedList[i]=sortedList[j];
                sortedList[j]=temp;
            }
                                                                                                                                                            
    int hoehe=0;
//  for(map<long, Order>::iterator order=orderList->begin(); order!=orderList->end(); ++order)
    for(int k=0;k<orderCount;k++)
    {
        if((*anarace)->getProgramFacility(sortedList[k]->getIP()))
            for(int i=0;i<20;i++)
                if(bograph[i].type==(*anarace)->getProgramFacility(sortedList[k]->getIP()))
                {
//order->time muesste vorsortiert sein
// hoehen! Positionen anordnen damits keine ueberschneidungen gibt
                                                                                                                                                            
        // calculate the y-position of this specific order, THIS HAS TO BEGIN WITH 0 -> MAX_LENGTH, DO NOT USE -- OPTIMIZATION!
                    for(int j=0;j<MAX_LENGTH;j++)
                        if(unitCounter[bograph[i].type][j]<=(*anarace)->getProgramTime(sortedList[k]->getIP()))
                        {
// if we have found a unitCounter which has a lower time (all unitCounter start with 0) we set the unit time to starttime + buildtime
// at the end all unitCounter have the time at which programTime ... TODO
// the items are in REAL TIME, i.e. time 0 is the beginning, ga->maxTime is the end
                                                                                                                                                            
                            unitCounter[bograph[i].type][j]=(*anarace)->getProgramTime(sortedList[k]->getIP())+stats[(*anarace)->getRace()][sortedList[k]->getUnit()/*(*anarace)->getPhaenoCode(sortedList[k]->getIP())*/].BT;
                            hoehe=j;
                            j=MAX_LENGTH;
                        }
                                                                                                                                                            
                    Rect t=Rect(
Point( ( (*anarace)->getProgramTime(sortedList[k]->getIP())*window[BO_GRAPH_WINDOW]->getClientRectWidth()) / ((*anarace)->getRealTimer()),
       (1+i+hoehe/MIN_HEIGHT)*(FONT_SIZE+10)+(hoehe%MIN_HEIGHT)*(FONT_SIZE+10)/bograph[i].height),
                                                                                                                                                            
Size(  (stats[(*anarace)->getRace()][sortedList[k]->getUnit()/*(*anarace)->getPhaenoCode(sortedList[k]->getIP())*/].BT/*(*anarace)->getProgramBT(s)*/*window[BO_GRAPH_WINDOW]->getClientRectWidth())/((*anarace)->getRealTimer()),
     (FONT_SIZE+9)/(bograph[i].height)));
                    if(t!=sortedList[k]->btarget)
                        sortedList[k]->bstart=sortedList[k]->brect;
                    sortedList[k]->btarget=t;
                    if(sortedList[k]->bonew)
                    {
//                      sortedList[k]->brect=sortedList[k]->btarget;
                        sortedList[k]->bstart=sortedList[k]->btarget;
                    }
                }
    }

// assign the coordinates for the lines where the orders are printed on
    int j=0;
    for(int i=0;i<20;i++)
        if(bograph[i].type>0)
        {
            bograph[i].edge=Rect(window[BO_GRAPH_WINDOW]->getAbsoluteClientRectPosition(), Size(window[BO_GRAPH_WINDOW]->getClientRectWidth(), bograph[i].lines*(FONT_SIZE+10)));
            bograph[i].edge.SetTop(window[BO_GRAPH_WINDOW]->getAbsoluteClientRectUpperBound()+(j+1)*(FONT_SIZE+10));
            j+=bograph[i].lines;
            if(window[BO_GRAPH_WINDOW]->fitItemToAbsoluteClientRect(bograph[i].edge,1))
                lastbographY=bograph[i].edge.GetBottom();
        }
		
	((BoGraphWindow*)window[BO_GRAPH_WINDOW])->copyBoGraph(bograph);
	((BoGraphWindow*)window[BO_GRAPH_WINDOW])->setBoGraphY(lastbographY);



	//TODO Hier das bodiagramzeugs rein!
	// bodiagram braucht eine sortierte IP List
}

void Player::drawGeneString(DC* dc, const Rect position) const
{
	//TODO
	return;
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
	int boanzahl=orderList.size();
	int colors[MAX_LENGTH];													
	for(int i=0;i<MAX_LENGTH;i++)
		colors[i]=0;
	
    for(map<long, Order>::const_iterator order=orderList.begin(); order!=orderList.end(); ++order)
		colors[order->second.getIP()]=order->second.getUnit();

	currentType=-1;

	if((*anarace)->isOptimizing())
	{
		for(int i=0;i<MAX_LENGTH;i++)
			if(colors[i])
			{
				int k=1;
				while((i<MAX_LENGTH)&&((!colors[i])||((stats[(*anarace)->getRace()][colors[i]].facilityType)&&((currentType==-1)||(stats[(*anarace)->getRace()][colors[i]].facilityType==currentType)))))
				{
					i++;
					if(!colors[i]) continue;
					if(currentType==-1)
						currentType=stats[(*anarace)->getRace()][colors[i]].facilityType;
					k++;
				}
				currentType=stats[(*anarace)->getRace()][colors[i]].facilityType;
																				
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
					Pen bla1=Pen(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN+stats[(*anarace)->getRace()][colors[i-1]].unitType)));
					Pen bla2=Pen(*theme.lookUpPen((ePen)(UNIT_TYPE_0_PEN+stats[(*anarace)->getRace()][colors[i-1]].unitType)));
																				
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
	} //end if(isOptimizing)
	#endif
}
																			

void Player::draw(DC* dc) const
{
	if(!isShown())
		return;
	drawGeneString(dc,Rect(getAbsolutePosition()+Point(40,40+(*anarace)->getPlayerNum()*300), Size(1200, 240)));
	// TODO
	UI_Object::draw(dc);
}

void Player::setMode(const eTab tab, const unsigned int playerNum)//, int player1, int player2)
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
	if(!isShown())
		return;
	UI_Object::process();

// ------ COMMUNICATION BETWEEN THE WINDOWS ------ TODO
	if(window[FORCE_WINDOW]->isShown())
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
	}
// ------ END COMMUNICATION BETWEEN THE WINDOWS ------

	if((*anarace)->isOptimizing())
		geneAnimation++;
// ------ PROCESS MEMBER VARIABLES ------	
	MoveOrders();
// ------ END PROCESSING MEMBER VARIABLES ------
}

const bool Player::getChangedFlag() const
{
	for(int i=BUILD_ORDER_WINDOW;i<=INFO_WINDOW;i++)
		if(window[(eWindow)i]->getChangedFlag())
			return(1);
	return(0);
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
}

void Player::changeAccepted()
{
	for(int i=BUILD_ORDER_WINDOW;i<=INFO_WINDOW;i++)
		window[(eWindow)i]->changeAccepted();
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
	geneAnimation=0;
//	Hide();
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
	unsigned int k=1;
	for(int s=MAX_LENGTH;s--;)
		if((*anarace)->getProgramIsBuilt(s)&&((*anarace)->getProgramTime(s)<=(*anarace)->getRealTimer()))
		{
			map<long, Order>::iterator order=orderList.find((*anarace)->getMarker(s)) ;// => found old one -> update the data!
			if(order!=orderList.end())
			{
			
				order->second.setRow(k+1);//+((orderList.getMakeSpace()>-1)*(k+1>=orderList.getMakeSpace())); TODO
				//Rect t=Rect((order->second.getRow()%2)*(window[BUILD_ORDER_WINDOW]->getWidth()/2), (order->second.getRow()/2)*(FONT_SIZE+6),(window[BUILD_ORDER_WINDOW]->getWidth()-8)/2,FONT_SIZE+4);
				Rect t=Rect(0,order->second.getRow()*(FONT_SIZE+7),500-8,FONT_SIZE+6);
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
					if((order->second.getUnit()!=(*anarace)->getPhaenoCode(s))||(order->second.getRow()!=k+1)||(order->second.target.GetLeft()>0))
						order++;
					else //=> ueberschreiben
					{
						found=true;
						Order neuorder;
						neuorder.setRow(k+1);//+((orderList.getMakeSpace()>-1)*(k+1>=orderList.getMakeSpace())); TODO
//						Rect t=Rect((order->second.getRow()%2)*(window[BUILD_ORDER_WINDOW]->getWidth()/2), (order->second.getRow()/2)*(FONT_SIZE+6),(window[BUILD_ORDER_WINDOW]->getWidth()-8)/2,FONT_SIZE+4);
						Rect t=Rect(0,order->second.getRow()*(FONT_SIZE+7),500-8,FONT_SIZE+6);
						if(order->second.target!=t)
							neuorder.start=order->second.rect;
						else neuorder.start=order->second.start;
						neuorder.target=t;

						neuorder.brect=order->second.brect;
						neuorder.bstart=order->second.bstart;
						neuorder.btarget=order->second.btarget;
						
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
					neuorder.rect=Rect(170,neuorder.getRow()*(FONT_SIZE+7),0,FONT_SIZE+6);
					//Rect t=Rect((order->second.getRow()%2)*(window[BUILD_ORDER_WINDOW]->getWidth()/2), (order->second.getRow()/2)*(FONT_SIZE+6),(window[BUILD_ORDER_WINDOW]->getWidth()-8)/2,FONT_SIZE+4);
					Rect t=Rect(0,neuorder.getRow()*(FONT_SIZE+7),500-8,FONT_SIZE+6);
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
		
	processBoGraph();
}



// ----- MOVE ORDERS IN BOWINDOW AND BOGRAPH -----
void Player::MoveOrders() 
{
	for(map<long, Order>::iterator order=orderList.begin(); order!=orderList.end(); ++order)
	{
		order->second.rect.move(order->second.start, order->second.target);
		order->second.brect.move(order->second.bstart, order->second.btarget);
		Size::mv(order->second.blend, order->second.blendStart, order->second.blendTarget);
	}
}


