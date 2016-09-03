#include "bodiagram.hpp"
#include "../core/configuration.hpp"
#include <sstream>

BoDiagramWindow::BoDiagramWindow(UI_Object* bod_parent, const unsigned int game_number, const unsigned int game_max, const unsigned int player_number, const unsigned int player_max) :
	UI_Window(bod_parent, BODIAGRAM_WINDOW_TITLE_STRING, theme.lookUpPlayerRect(BUILD_ORDER_DIAGRAM_WINDOW, game_number, game_max, player_number, player_max), theme.lookUpPlayerMaxHeight(BUILD_ORDER_DIAGRAM_WINDOW, game_number, game_max, player_number, player_max), NOT_SCROLLED),
	anarace(NULL),
	bold(false),
	mouseTime(0),
	totalTime(0),
	oldMouse(),
	diagramList(),
	gameNumber(game_number),
	gameMax(game_max),
	playerNumber(player_number),
	playerMax(player_max),
	minerals(new UI_StaticText(this, BODIAGRAM_MINERALS_STRING, Rect(Point(8, 15), Size(0,0)), Size(0,0), MINERALS_TEXT_COLOR, SMALL_SHADOW_BOLD_FONT, DO_NOT_ADJUST)),
	gas(new UI_StaticText(this, BODIAGRAM_GAS_STRING, Rect(Point(8, 26), Size(0,0)), Size(0,0), GAS_TEXT_COLOR, SMALL_SHADOW_BOLD_FONT, DO_NOT_ADJUST)),
	supply(new UI_StaticText(this, BODIAGRAM_SUPPLY_STRING, Rect(Point(8, 37), Size(0,0)), Size(0,0), SUPPLY_TEXT_COLOR, SMALL_SHADOW_BOLD_FONT, DO_NOT_ADJUST)),
	time(new UI_StaticText(this, BODIAGRAM_TIME_STRING, Rect(Point(8, 48), Size(0,0)), Size(0,0), FITNESS_TEXT_COLOR, SMALL_SHADOW_BOLD_FONT, DO_NOT_ADJUST)),
	mineralsNumber(new UI_StaticText(this, Rect(Point(75, 16), Size(0,0)), Size(0,0), BRIGHT_MINERALS_TEXT_COLOR, SMALL_SHADOW_BOLD_FONT, DO_NOT_ADJUST)),
	gasNumber(new UI_StaticText(this, Rect(Point(75, 27), Size(0,0)), Size(0,0), BRIGHT_GAS_TEXT_COLOR, SMALL_SHADOW_BOLD_FONT, DO_NOT_ADJUST)),
	supplyNumber(new UI_StaticText(this, Rect(Point(75, 38), Size(0,0)), Size(0,0), BRIGHT_SUPPLY_TEXT_COLOR, SMALL_SHADOW_BOLD_FONT, DO_NOT_ADJUST)),
	timeNumber(new UI_StaticText(this, Rect(Point(75, 49), Size(0,0)), Size(0,0), BRIGHT_FITNESS_TEXT_COLOR, SMALL_SHADOW_BOLD_FONT, DO_NOT_ADJUST))	
{
	resetData();
	addHelpButton(DESCRIPTION_BODIAGRAM_WINDOW_CHAPTER);
}


BoDiagramWindow::~BoDiagramWindow()
{ 
	delete minerals;
	delete mineralsNumber;
	delete gas;
	delete gasNumber;
	delete supply;
	delete supplyNumber;
	delete time;
	delete timeNumber;
}

void BoDiagramWindow::setMode(const unsigned int game_number, const unsigned int game_max, const unsigned int player_number, const unsigned int player_max)
{
	if((game_number == gameNumber) && (game_max == gameMax) && (player_number == playerNumber) && (player_max == playerMax))
		return;
	gameNumber = game_number;
	gameMax = game_max;
	playerNumber = player_number;
	playerMax = player_max;
}


void BoDiagramWindow::reloadOriginalSize()
{
	setOriginalRect(UI_Object::theme.lookUpPlayerRect(BUILD_ORDER_DIAGRAM_WINDOW, gameNumber, gameMax, playerNumber, playerMax));
	setMaxHeight(UI_Object::theme.lookUpPlayerMaxHeight(BUILD_ORDER_DIAGRAM_WINDOW, gameNumber, gameMax, playerNumber, playerMax));

	UI_Window::reloadOriginalSize();
}

void BoDiagramWindow::assignAnarace(ANABUILDORDER* bod_anarace)
{
	if(anarace == bod_anarace)
		return;
	anarace = bod_anarace;
	resetData();
}

void BoDiagramWindow::resetData()
{
	diagramList.clear();
	processList();
}

void BoDiagramWindow::process()
{
	if(!isShown()) 
		return;

	UI_Window::process();
	bool has_changed = false;	
		for(std::list<BoDiagramPoint>::iterator i = diagramList.begin(); i!=diagramList.end(); ++i)
		{
			if(uiConfiguration.isSmoothMovements())
			{
				if(Size::mv2(i->current_x,  i->start_x,  i->target_x))
					has_changed = true;

				for(unsigned int j = TOTAL_STATS;j--;)
				{
					if(Size::mv2(i->current_y1[j], i->start_y1[j], i->target_y1[j]))
						has_changed = true;
					if(Size::mv2(i->current_y2[j], i->start_y2[j], i->target_y2[j]))
						has_changed = true;
					if(Size::mv(i->highlight[j], 150, 100))
						has_changed = true;
				}
			}
		}
	
	if(has_changed)
		setNeedRedrawNotMoved();
	
	
	bold = false;
	selectedItems.clear();
	if((diagramList.size()>1) && (getAbsoluteClientRect().Inside(mouse)) && (totalTime>0))
	{
		bold = true;
		if(oldMouse!=mouse)
			setNeedRedrawNotMoved();
		mouseTime = totalTime * (mouse.x - getAbsoluteClientRectLeftBound()) / getClientRectWidth();
	
		unsigned int number = 0;
		for(std::list<PROGRAM>::const_iterator k = anarace->getProgramList().begin(); k != anarace->getProgramList().end(); ++k, ++number)
			if((k->getTime() - k->getBT() < coreConfiguration.getMaxTime()-mouseTime)&&(k->getTime() > coreConfiguration.getMaxTime() - mouseTime))
				selectedItems.push_back(number);


		timeNumber->updateText(formatTime(mouseTime, efConfiguration.getGameSpeed()));
		
		unsigned int my_time = coreConfiguration.getMaxTime() - mouseTime;
		for(std::list<STATISTICS>::const_iterator i = anarace->getTimeStatisticsList().begin(); i != anarace->getTimeStatisticsList().end(); ++i)
		{
			std::list<STATISTICS>::const_iterator j = i;
			++j;
			if(i->getTime() == j->getTime())
				continue;
			
			if((i->getTime() == my_time)||(j==anarace->getTimeStatisticsList().end()))
			{
				std::ostringstream os;
				os << i->getHaveMinerals()/100;mineralsNumber->updateText(os.str());os.str("");
				os << i->getHaveGas()/100;gasNumber->updateText(os.str());os.str("");
				os << i->getNeedSupply() << "/" << i->getHaveSupply();supplyNumber->updateText(os.str());
				break;
			} else
			if((i->getTime() > my_time) && (j->getTime() < my_time))
			{
				// => Interpolieren zwischen i und j
				std::ostringstream os;
				os << (i->getHaveMinerals() + (j->getHaveMinerals() - i->getHaveMinerals()) * (i->getTime() - my_time) / (i->getTime() - j->getTime()))/100; mineralsNumber->updateText(os.str());os.str("");
				os << (i->getHaveGas() + (j->getHaveGas() - i->getHaveGas()) * (i->getTime() - my_time) / (i->getTime() - j->getTime()))/100; gasNumber->updateText(os.str());os.str("");
				os << i->getNeedSupply() << "/" << i->getHaveSupply();supplyNumber->updateText(os.str());
				break;
			}
		}

		mineralsNumber->Show();
		gasNumber->Show();
		supplyNumber->Show();
		timeNumber->Show();
	} else
	{
		mineralsNumber->Hide();
		gasNumber->Hide();
		supplyNumber->Hide();
		timeNumber->Hide();
	}
	oldMouse = mouse;
}


void BoDiagramWindow::processList()
{
	if((anarace==NULL))
		return;

//	if(anarace->getProgramList().size()==0)
		// TODO
	totalTime = anarace->getRealTimer();
	if(totalTime == coreConfiguration.getMaxTime())
	{
		totalTime = 0;
		for(std::list<PROGRAM>::const_iterator t = anarace->getProgramList().begin(); t != anarace->getProgramList().end(); ++t)
			if(t->getRealTime() + t->getBT() > totalTime)
				totalTime = t->getRealTime() + t->getBT();
	}
	
	for(unsigned int i = TOTAL_STATS; i--;)
		BoDiagramPoint::max[i] = 1;

	if(diagramList.size()==0)
	{
		BoDiagramPoint p;
		diagramList.push_front(p);
	}

	bool new_items = false;		
	if(totalTime>0) 
	{
		std::list<BoDiagramPoint>::iterator k = diagramList.begin();
		++k;
		for(std::list<PROGRAM>::const_iterator order = anarace->getProgramList().begin(); order != anarace->getProgramList().end(); ++order)
		{
			if(k==diagramList.end())
			{
				BoDiagramPoint p;
				diagramList.push_back(p);
				--k;
				new_items = true;
			}
			unsigned int x = 1 + (int)(order->getRealTime() * (getClientTargetWidth()-5) / (float)(totalTime));
			k->setTargetX(x);
			k->setTargetY(HAVE_MINERALS, order->getStatisticsBefore().getHaveMinerals(), order->getStatisticsAfter().getHaveMinerals());
			k->setTargetY(HAVE_GAS, order->getStatisticsBefore().getHaveGas(), order->getStatisticsAfter().getHaveGas());
			k->setTargetY(HAVE_SUPPLY, order->getStatisticsBefore().getHaveSupply(), order->getStatisticsAfter().getHaveSupply());
			k->setTargetY(NEED_SUPPLY, order->getStatisticsBefore().getNeedSupply(), order->getStatisticsAfter().getNeedSupply());
			k->unit = order->getUnit();
			k->initialized = true;
			++k;
		}
		while(k!=diagramList.end())
			k = diagramList.erase(k);
	}

	if((new_items)||(diagramList.size()==1))
	{
		BoDiagramPoint p;
		diagramList.push_back(p);
	}

	diagramList.front().setTargetX(1);
	diagramList.front().setTargetY(HAVE_MINERALS, 0, (*anarace->getStartCondition())->getMinerals());
	diagramList.front().setTargetY(HAVE_GAS, 0, (*anarace->getStartCondition())->getGas());
	diagramList.front().setTargetY(HAVE_SUPPLY, 0, (*anarace->getStartCondition())->getHaveSupply());
	diagramList.front().setTargetY(NEED_SUPPLY, 0, (*anarace->getStartCondition())->getNeedSupply());
	diagramList.front().initialized = true;

	diagramList.back().initialized = false; // letzten Eintrag immer sofort anpassen
	diagramList.back().setTargetX(1 + getClientTargetWidth()-5);
	diagramList.back().setTargetY(HAVE_MINERALS, anarace->getMinerals(), 0);
	diagramList.back().setTargetY(HAVE_GAS, anarace->getGas(), 0);
	diagramList.back().setTargetY(HAVE_SUPPLY, anarace->getHaveSupply(), 0);
	diagramList.back().setTargetY(NEED_SUPPLY, anarace->getNeedSupply(), 0);
	diagramList.back().initialized = true;	

	for(std::list<BoDiagramPoint>::iterator i = diagramList.begin(); i != diagramList.end(); ++i)
		for(unsigned int j = TOTAL_STATS; j--;)
		{
			if(i->current_y1[j] > i->max[j]) i->max[j] = i->current_y1[j];
			if(i->current_y2[j] > i->max[j]) i->max[j] = i->current_y2[j];
			if(i->target_y1[j] > i->max[j]) i->max[j] = i->target_y1[j];
			if(i->target_y2[j] > i->max[j]) i->max[j] = i->target_y2[j];
		}

	// special rule for supply:
	if(BoDiagramPoint::max[HAVE_SUPPLY] > BoDiagramPoint::max[NEED_SUPPLY])
		BoDiagramPoint::max[NEED_SUPPLY] = BoDiagramPoint::max[HAVE_SUPPLY];
	else if(BoDiagramPoint::max[NEED_SUPPLY] > BoDiagramPoint::max[HAVE_SUPPLY])
		BoDiagramPoint::max[HAVE_SUPPLY] = BoDiagramPoint::max[NEED_SUPPLY];
}

void BoDiagramWindow::draw(DC* dc) const
{
	if(!isShown()) 
		return;
	if(!checkForNeedRedraw())
		return;
	UI_Window::drawWindow(dc);
//	if(infoWindow->isShown())
		dc->setBrush(*theme.lookUpBrush(BODIAGRAM_BACK1));
//	else
//		dc->setBrush(*theme.lookUpBrush(BODIAGRAM_BACK2));
	dc->DrawRectangle(getAbsoluteClientRectPosition()+Point(0,10), getClientRectSize() - Size(0,10));

	float scale[TOTAL_STATS];
	for(unsigned int i = TOTAL_STATS; i--;)
		scale[i] = (float)(getClientRectHeight() - 20)/(float)BoDiagramPoint::max[i];
	
	if(diagramList.size()>0)
	{
		dc->setFont(theme.lookUpFont(SMALL_SHADOW_BOLD_FONT));

		for(std::list<BoDiagramPoint>::const_iterator i = diagramList.begin(); i!=diagramList.end(); ++i)
		{
			std::list<BoDiagramPoint>::const_iterator j = i;
			++j;
			
			Point p = getAbsoluteClientRectPosition()+Point(0,getClientRectHeight());
			Pen* pen;
			
					
			int x = p.x + i->current_x;
			int y1 = p.y - i->current_y1[HAVE_GAS]*scale[HAVE_GAS];
			int y2 =  p.y - i->current_y2[HAVE_GAS]*scale[HAVE_GAS];
	
			if(y1 + 10 < y2) pen = theme.lookUpPen(DASHED_GAS_PEN);else pen = theme.lookUpPen(BODIAGRAM_GAS_PEN);
			dc->setPen(Pen(dc->changeRelativeBrightness(*pen->getColor(), i->highlight[HAVE_GAS]), pen->getWidth(), pen->getStyle()));
			if(y1<y2) dc->DrawVerticalLine(x, y1, y2);else  dc->DrawVerticalLine(x, y2, y1);

			y1 = p.y - i->current_y1[HAVE_MINERALS] * scale[HAVE_MINERALS];
			y2 = p.y - i->current_y2[HAVE_MINERALS] * scale[HAVE_MINERALS];
			if(y1 + 10 < y2) pen = theme.lookUpPen(DASHED_MINERALS_PEN);else pen = theme.lookUpPen(BODIAGRAM_MINERALS_PEN);
			dc->setPen(Pen(dc->changeRelativeBrightness(*pen->getColor(), i->highlight[HAVE_MINERALS]), pen->getWidth(), pen->getStyle()));
			if(y1<y2) dc->DrawVerticalLine(x, y1, y2);else  dc->DrawVerticalLine(x, y2, y1);

			if(j!=diagramList.end())
			{
				dc->setBrush(*theme.lookUpBrush(BODIAGRAM_SUPPLY_BRUSH));
				unsigned int y;
				if(i->current_y2[HAVE_SUPPLY] < i->current_y2[NEED_SUPPLY])
					y = i->current_y2[NEED_SUPPLY] - i->current_y2[HAVE_SUPPLY];
				else 
					y = i->current_y2[HAVE_SUPPLY] - i->current_y2[NEED_SUPPLY];
				
				unsigned int high_light;
				if(i->highlight[HAVE_SUPPLY] > i->highlight[NEED_SUPPLY])
					high_light = i->highlight[HAVE_SUPPLY];
				else high_light = i->highlight[NEED_SUPPLY];
			
				pen = theme.lookUpPen(BODIAGRAM_SUPPLY_PEN);
				dc->setPen(Pen(dc->changeRelativeBrightness(*pen->getColor(), high_light), pen->getWidth(), pen->getStyle()));
				
				dc->DrawRectangle(getAbsoluteClientRectPosition() + Point(0, getClientRectHeight()) +
						Point(i->current_x, 0) - Size(0, i->current_y2[HAVE_SUPPLY]*scale[HAVE_SUPPLY]),
						Size(j->current_x - i->current_x, y*scale[HAVE_SUPPLY]) );

				pen = theme.lookUpPen((ePen)(BODIAGRAM_MINERALS_PEN+(((bold)||(i->highlight[HAVE_MINERALS]>100))?3:0)));
				dc->setPen(Pen(dc->changeRelativeBrightness(*pen->getColor(), i->highlight[HAVE_MINERALS]), pen->getWidth(), pen->getStyle()));
				dc->DrawLine(p.x + i->current_x, p.y - i->current_y2[HAVE_MINERALS] * scale[HAVE_MINERALS], p.x + j->current_x, p.y - j->current_y1[HAVE_MINERALS] * scale[HAVE_MINERALS]);
	
				pen = theme.lookUpPen((ePen)(BODIAGRAM_GAS_PEN+(((bold)||(i->highlight[HAVE_GAS]>100))?3:0)));
				dc->setPen(Pen(dc->changeRelativeBrightness(*pen->getColor(), i->highlight[HAVE_GAS]), pen->getWidth(), pen->getStyle()));
				dc->DrawLine(p.x + i->current_x, p.y - i->current_y2[HAVE_GAS]*scale[HAVE_GAS], p.x + j->current_x, p.y - j->current_y1[HAVE_GAS]*scale[HAVE_GAS]);
				if(i!=diagramList.begin())
				{
					dc->setTextForeground(*theme.lookUpColor(BRIGHT_TEXT_COLOR));
					unsigned int width = (theme.lookUpFont(SMALL_SHADOW_BOLD_FONT)->getTextExtent(stats[(*anarace->getStartCondition())->getRace()][i->unit].name)).getWidth();
					unsigned int tx;
					if(x - width/2 < getAbsoluteClientRectLeftBound())
						tx = getAbsoluteClientRectLeftBound();
					else if(x + width/2 >= getAbsoluteClientRectRightBound())
						tx = getAbsoluteClientRectRightBound() - width;
					else 
						tx = x - width/2;
					dc->DrawText(stats[(*anarace->getStartCondition())->getRace()][i->unit].name, Point(tx, y1 + (y2 - y1)/2));
				}
			}
		}
	}
	if(bold)
	{
		dc->setBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
		dc->setPen(*theme.lookUpPen(SELECT_PEN));
		dc->DrawEmptyRectangle( getAbsoluteClientRectLeftBound() - 2 + (mouseTime * getClientRectWidth()) / totalTime, getAbsoluteClientRectUpperBound()+10, 4, getClientRectHeight()-10);
	}

	dc->setPen(*theme.lookUpPen(RECTANGLE_PEN));
//		}
		#if 0 
		if(infoWindow->isShown())
		{
			if(stats[(*anarace->getStartCondition())->getRace()][infoWindow->getUnit()].needSupply)
			{
				dc->setPen(*theme.lookUpPen(DASHED_SUPPLY_PEN));
				int y1=stats[(*anarace->getStartCondition())->getRace()][infoWindow->getUnit()].needSupply*5;
				if(y1<0) y1=-y1;
					dc->DrawHorizontalLine(getAbsoluteClientRectLeftBound()+infoWindow->getBx()+1,
								 getAbsoluteClientRectLowerBound()-y1, 
								 getAbsoluteClientRectLeftBound()+infoWindow->getBx()+infoWindow->getBWidth()-1);
			}

			if(stats[(*anarace->getStartCondition())->getRace()][infoWindow->getUnit()].minerals)
			{
				dc->setPen(*theme.lookUpPen(DASHED_MINERALS_PEN));
				dc->DrawHorizontalLine(getAbsoluteClientRectLeftBound()+infoWindow->getBx()+1,
							 getAbsoluteClientRectLowerBound()-stats[(*anarace->getStartCondition())->getRace()][infoWindow->getUnit()].minerals/1000,
							 getAbsoluteClientRectLeftBound()+infoWindow->getBx()+infoWindow->getBWidth()-1);
			}

			if(stats[(*anarace->getStartCondition())->getRace()][infoWindow->getUnit()].current[HAVE_GAS])
			{
				dc->setPen(*theme.lookUpPen(DASHED_GAS_PEN));
				dc->DrawHorizontalLine(getAbsoluteClientRectLeftBound()+infoWindow->getBx()+1,
							 getAbsoluteClientRectLowerBound()-stats[(*anarace->getStartCondition())->getRace()][infoWindow->getUnit()].current[HAVE_GAS]/1000,
							 getAbsoluteClientRectLeftBound()+infoWindow->getBx()+infoWindow->getBWidth()-1);
			}
			dc->setPen(*theme.lookUpPen(DASHED_MARKER_PEN));
			dc->DrawVerticalLine(getAbsoluteClientRectLeftBound()+infoWindow->getBx()+infoWindow->getBWidth(),
						 getAbsoluteClientRectUpperBound()+10,
						 getAbsoluteClientRectLowerBound());
						 
			dc->DrawVerticalLine(getAbsoluteClientRectLeftBound()+infoWindow->getBx(),
						 getAbsoluteClientRectUpperBound()+10,
						 getAbsoluteClientRectLowerBound());
		}
		#endif
	UI_Object::draw(dc);

}


