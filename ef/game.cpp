#include "game.hpp"

Game::~Game()
{ 
	for(int i=0;i<MAX_WINDOWS;i++)
		delete window[i];
}

Game::Game(UI_Object* game_parent, ANARACE** game_anarace, MessageWindow* msgWindow, const unsigned int gameNumber):
	UI_Object(game_parent),
	geneAnimation(0),
	anarace(game_anarace),
	mode(0)
{
	for(int i=0;i<MAX_WINDOWS;i++)
		window[i]=NULL;
	window[FORCE_WINDOW] = new ForceWindow(this, *anarace, msgWindow, gameNumber);
	window[TIMER_WINDOW] = new TimerWindow(this, *anarace, msgWindow, gameNumber);
	window[STATISTICS_WINDOW]=new StatisticsWindow(this, *anarace, gameNumber);
	window[INFO_WINDOW] = new InfoWindow(this, *anarace, gameNumber);
	window[BUILD_ORDER_WINDOW] = new BoWindow(this, *anarace, (InfoWindow*)window[INFO_WINDOW], msgWindow, /* fixed,*/ gameNumber);
	window[BO_GRAPH_WINDOW] = new BoGraphWindow(this, *anarace, (InfoWindow*)window[INFO_WINDOW], gameNumber);
	window[BO_DIAGRAM_WINDOW]=new BoDiagramWindow(this, *anarace, (InfoWindow*)window[INFO_WINDOW], gameNumber);
	
	// to set infoWindow above all others
	window[INFO_WINDOW]->removeFromFamily(); window[INFO_WINDOW]->setParent(this);
	setOptimizing();
//	process();
	Hide();
	setOptimizing(false);
//	for(int i=MAX_LENGTH;i--;)
//		fixed[i]=false;	
//	(*anarace)->setFixed(fixed);
}

void Game::assignAnarace(ANARACE** game_anarace)
{
	anarace = game_anarace;
	((ForceWindow*)window[FORCE_WINDOW])->assignAnarace(*anarace);
	((TimerWindow*)window[TIMER_WINDOW])->assignAnarace(*anarace);
	((StatisticsWindow*)window[STATISTICS_WINDOW])->assignAnarace(*anarace);
	((InfoWindow*)window[INFO_WINDOW])->assignAnarace(*anarace);
	((BoWindow*)window[BUILD_ORDER_WINDOW])->assignAnarace(*anarace);
	((BoGraphWindow*)window[BO_GRAPH_WINDOW])->assignAnarace(*anarace);
	((BoDiagramWindow*)window[BO_DIAGRAM_WINDOW])->assignAnarace(*anarace);
//	(*anarace)->setFixed(fixed);
}

void Game::drawGene(DC* dc, int k, const Point* points, const Point position, Pen& bla1, Pen& bla2) const
{
	if(points[0].y<points[1].y) dc->SetPen(bla1);else dc->SetPen(bla2);
	dc->DrawSpline(k, points, position);
	//dc->SetPen(bla2);
	dc->DrawSpline(k, points, position - Size(0,1));
	dc->DrawSpline(k, points, position + Size(0,1));
}

void Game::drawGeneString(DC* dc, const Rect position) const
{
	int stringheight=0;
	Point points1[2];
	Point points2[2];
	Point points3[2];
	Point points4[2];
	vector<int> colors;
	int orderCount = (*anarace)->getProgramList().size();
	if(orderCount<2)
		return;
	for(std::list<PROGRAM>::const_iterator i = (*anarace)->getProgramList().begin();i!=(*anarace)->getProgramList().end();i++)
		colors.push_back(i->getUnit());

//	if((*anarace)->isOptimizing())
	{
		int current_type = stats[(*anarace)->getRace()][colors[0]].facilityType;
		for(int i=0;i<2*orderCount;i++)
		{
//			int k=2;
/*			while((i<orderCount)&&(stats[(*anarace)->getRace()][colors[i]].facilityType==current_type))
			{
				i++;
				k++;
			}*/
			current_type = stats[(*anarace)->getRace()][colors[i/2]].facilityType;
				for(int j=0;j<2;j++)
				{
					points1[j] = Point(5+(stringheight+j)*(position.GetWidth()-8)/(orderCount*2)-1, (int)((cos((float)(4*(stringheight+j)+geneAnimation)*5.0*M_PI/200.0)*0.9*position.GetHeight()/2)+position.GetHeight()/2.1));
					
					points2[j] = Point(5+(stringheight+j)*(position.GetWidth()-8)/(orderCount*2)-1, (int)((sin(((float)(4*(stringheight+j)+geneAnimation)+13.0)*5.0*M_PI/200.0)*0.9*position.GetHeight()/2)+position.GetHeight()/2.1));
					
					points3[j] = Point(5+(stringheight+j)*(position.GetWidth()-8)/(orderCount*2)-1, (int)((cos(((float)(4*(stringheight+j)+geneAnimation)+26.0)*5.0*M_PI/200.0)*0.9*position.GetHeight()/2)+position.GetHeight()/2.1));
					
					points4[j] = Point(5+(stringheight+j)*(position.GetWidth()-8)/(orderCount*2)-1, (int)((sin(((float)(4*(stringheight+j)+geneAnimation)+39.0)*5.0*M_PI/200.0)*0.9*position.GetHeight()/2)+position.GetHeight()/2.1));
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
}
																			

void Game::draw(DC* dc) const
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

void Game::setMode(const eTab tab, const unsigned int gameNum)//, int game1, int game2)
{
	this->mode=tab*2+gameNum-2;
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
		break; // first game basic mode
		
		case 1:
			this->Hide();(*anarace)->setActive(0);break; // second game basic mode
		case 2:
			this->Show();
			(*anarace)->setActive(1);
			window[BO_DIAGRAM_WINDOW]->Show();
			window[STATISTICS_WINDOW]->Show();
			window[BO_GRAPH_WINDOW]->Show();
		break; // first game advanced mode
		case 3:this->Hide();(*anarace)->setActive(0);break; // second game advanced mode
		case 4:this->Show();(*anarace)->setActive(1);break; // first game
		case 5:this->Show();(*anarace)->setActive(1);break; // second game
		case 6:this->Show();(*anarace)->setActive(1);break; // first game
		case 7:this->Show();(*anarace)->setActive(1);break; // second game
		case 8:this->Show();(*anarace)->setActive(1);
				break; // first game // compare?
		case 9:this->Hide();(*anarace)->setActive(0);break; // second game
		case 10:this->Hide();(*anarace)->setActive(0);break; // first game
		case 11:this->Hide();(*anarace)->setActive(0);break; // second game
		case 12:this->Hide();(*anarace)->setActive(0);break; // first game
		case 13:this->Hide();(*anarace)->setActive(0);break; // second game
		case 14:this->Show();(*anarace)->setActive(1);
			window[BO_DIAGRAM_WINDOW]->Show();
			window[STATISTICS_WINDOW]->Hide();
			window[BO_GRAPH_WINDOW]->Show();
		break; // first game compare
		case 15:this->Hide();(*anarace)->setActive(0);break; // second game compare
		default:break;
	}
	resetData();
	setNeedRedrawMoved(); //?
	// TODO modes der einzelnen Windows (z.B> timer oder force)
}

void Game::process()
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
		geneAnimation+=0.43;
// ------ PROCESS MEMBER VARIABLES ------	
//	MoveOrders();
// ------ END PROCESSING MEMBER VARIABLES ------
}

void Game::updateRectangles(const unsigned int maxGame)
{
	for(int i=BUILD_ORDER_WINDOW; i <= INFO_WINDOW; i++)
		window[(eWindow)i]->updateRectangles(maxGame);
}

const bool Game::isOptimizing() const
{
	return((*anarace)->isOptimizing());
}

void Game::setOptimizing(const bool opt)
{
	(*anarace)->setOptimizing(opt);
	((TimerWindow*)window[TIMER_WINDOW])->forcePause(opt);
}

void Game::resetData()
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
}

void Game::restartAnarace()
{
	(*anarace)->restartData();
}

void Game::CheckOrders()
{
	((BoDiagramWindow*)(window[BO_DIAGRAM_WINDOW]))->processList();
	((BoWindow*)(window[BUILD_ORDER_WINDOW]))->processList();
	((BoGraphWindow*)(window[BO_GRAPH_WINDOW]))->processList();
	((ForceWindow*)(window[FORCE_WINDOW]))->processList();
}

//virtual machen
//resetData, updateItems, assignAnarace, checkOrders

