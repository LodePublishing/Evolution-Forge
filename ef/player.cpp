#include "player.hpp"
#include "configuration.hpp"

Player::~Player()
{ 
	delete forceWindow;
//	delete statisticsWindow;
	delete boWindow;
	delete boGraphWindow;
	delete boDiagramWindow;
}

Player::Player(UI_Object* player_parent, const unsigned int playerNumber) :
	UI_Object(player_parent),
	geneAnimation(0),
	anarace(NULL),
	mode(0),
	forceWindow(new ForceWindow(this, playerNumber)),
//	statisticsWindow(new StatisticsWindow(this, playerNumber)),
	boWindow(new BoWindow(this, /* fixed,*/ playerNumber)),
	boGraphWindow(new BoGraphWindow(this, playerNumber)),
	boDiagramWindow(new BoDiagramWindow(this, playerNumber))
{
	
	Hide();
	
//	for(int i=MAX_LENGTH;i--;)
//		fixed[i]=false;	
//	anarace->setFixed(fixed);
}

void Player::assignAnarace(ANABUILDORDER* player_anarace)
{
	anarace = player_anarace;
	forceWindow->assignAnarace(anarace);
//	statisticsWindow->assignAnarace(anarace);
	boWindow->assignAnarace(anarace);
	boGraphWindow->assignAnarace(anarace);
	boDiagramWindow->assignAnarace(anarace);
	
//	anarace->setFixed(fixed);
}

void Player::reloadStrings() //TODO
{
	if(!isShown())
		return;
	UI_Object::reloadStrings(); 
}

void Player::drawGene(DC* dc, int k, const Point* points, const Point position, Pen& bla1, Pen& bla2) const
{
	if(points[0].y<points[1].y) dc->SetPen(bla1);else dc->SetPen(bla2);
	dc->DrawSpline(k, points, position);
	dc->DrawSpline(k, points, position - Size(0,1));
	dc->DrawSpline(k, points, position + Size(0,1));
}

void Player::drawGeneString(DC* dc) const
{
	Rect position = Rect(getAbsolutePosition()+Point(210, 200+anarace->getPlayerNumber()*300), Size(600, 120));
	dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
	dc->SetPen(*theme.lookUpPen(BRIGHT_UNIT_TYPE_1_PEN));
	dc->DrawRectangle(position);

	unsigned int stringheight=0;
	Point points1[2];
	Point points2[2];
	Point points3[2];
	Point points4[2];
	std::vector<unsigned int> colors;
	unsigned int orderCount = anarace->getProgramList().size();
	if(orderCount<2)
		return;
	for(std::list<PROGRAM>::const_iterator i = anarace->getProgramList().begin();i!=anarace->getProgramList().end();i++)
		colors.push_back(i->getUnit());

//	if(anarace->isOptimizing())
	{
		unsigned int current_type = stats[anarace->getRace()][colors[0]].facilityType;
		for(unsigned int i=0;i<2*orderCount;i++)
		{
//			int k=2;
/*			while((i<orderCount)&&(stats[anarace->getRace()][colors[i]].facilityType==current_type))
			{
				i++;
				k++;
			}*/
			current_type = stats[anarace->getRace()][colors[i/2]].facilityType;
				for(unsigned int j=0;j<2;j++)
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
						
					Pen bla1=Pen(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN+stats[anarace->getRace()][colors[i/2]].unitType)));
					Pen bla2=Pen(*theme.lookUpPen((ePen)(UNIT_TYPE_0_PEN+stats[anarace->getRace()][colors[i/2]].unitType)));
					unsigned int k=2;
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
																			

void Player::draw(DC* dc) const
{
	if(!isShown())
		return;
	// TODO
	UI_Object::draw(dc);
	if(efConfiguration.isDnaSpiral())
		drawGeneString(dc);
	forceWindow->drawTechTree(dc);
}

void Player::setMode(const eTab tab, const unsigned int playerNum)//, int player1, int player2)
{
// TODO evtl eine Zwischenklasse fuer alle Playerwindows machen
//	mode=tab*2+playerNum-2;
	std::string title_parameter = *theme.lookUpString((eString)(tab+HIDE_MODE_STRING));

	forceWindow->setTitleParameter(title_parameter);
//	statisticsWindow->setTitleParameter(title_parameter);
	boWindow->setTitleParameter(title_parameter);
	boGraphWindow->setTitleParameter(title_parameter);
	boDiagramWindow->setTitleParameter(title_parameter);

	switch(tab)
	{
		case ZERO_TAB:break;
		case BASIC_TAB:
			this->Show();
			anarace->setActive(1);
			boDiagramWindow->Hide();
//			statisticsWindow->Hide();
			boGraphWindow->Hide();
		break; 
		case ADVANCED_TAB:
			this->Show();
			anarace->setActive(1);
			boDiagramWindow->Show();
//			statisticsWindow->Show();
			boGraphWindow->Show();
		break;

                case EXPERT_TAB:
                case GOSU_TAB:
			this->Show();
			anarace->setActive(1);
			boDiagramWindow->Show();
//			statisticsWindow->Hide();
			boGraphWindow->Show();
                        break;
                case TUTORIAL_TAB:
                case SETTINGS_TAB:
                case MAP_TAB:
                case COMPARE_TAB: // TODO
	                        this->Hide();
				anarace->setActive(0);
				boDiagramWindow->Hide();
//				statisticsWindow->Hide();
				boGraphWindow->Hide();
			break;
                default:break;
	}
	resetData();
	setNeedRedrawMoved(); //?
	// TODO modes der einzelnen Windows (z.B> timer oder force)
}

void Player::process()
{
	if(!isShown())
		return;
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

//	if(anarace->isOptimizing())
//		geneAnimation+=0.43;
// ------ PROCESS MEMBER VARIABLES ------	
// ------ END PROCESSING MEMBER VARIABLES ------
}

void Player::updateRectangles(const unsigned int maxPlayer)
{
	forceWindow->updateRectangles(maxPlayer);
//	statisticsWindow->updateRectangles(maxPlayer);
	boWindow->updateRectangles(maxPlayer);
	boGraphWindow->updateRectangles(maxPlayer);
	boDiagramWindow->updateRectangles(maxPlayer);
}

void Player::resetData()
{
	geneAnimation=0;
}

void Player::CheckOrders()
{
	boDiagramWindow->processList();
	boWindow->processList();
	boGraphWindow->processList();
	forceWindow->processList();
}

//virtual machen
//resetData, updateItems, assignAnarace, checkOrders

