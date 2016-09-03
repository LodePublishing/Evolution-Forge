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
	resourceText(RACE::MAX_RESOURCE_TYPES),
	supplyText(new UI_StaticText(this, BODIAGRAM_SUPPLY_STRING, Rect(Point(8, 37), Size(0,0)), Size(0,0), SUPPLY_TEXT_COLOR, SMALL_SHADOW_BOLD_FONT, DO_NOT_ADJUST)),
	timeText(new UI_StaticText(this, BODIAGRAM_TIME_STRING, Rect(Point(8, 48), Size(0,0)), Size(0,0), FITNESS_TEXT_COLOR, SMALL_SHADOW_BOLD_FONT, DO_NOT_ADJUST)),
	resourceNumber(RACE::MAX_RESOURCE_TYPES),
	supplyNumber(new UI_StaticText(this, Rect(Point(75, 38), Size(0,0)), Size(0,0), BRIGHT_SUPPLY_TEXT_COLOR, SMALL_SHADOW_BOLD_FONT, DO_NOT_ADJUST)),
	timeNumber(new UI_StaticText(this, Rect(Point(75, 49), Size(0,0)), Size(0,0), BRIGHT_FITNESS_TEXT_COLOR, SMALL_SHADOW_BOLD_FONT, DO_NOT_ADJUST)),
	firstTime(0),
	lastTime(0)	
{
	for(unsigned int i = 0; i < RACE::MAX_RESOURCE_TYPES; i++)
	{
		resourceText[i] = new UI_StaticText(this, GAME::lookUpGameString(GAME::FIRST_RESOURCE_STRING + i), Rect(Point(8, 15+11*i), Size(0,0)), Size(0,0), (eColor)(MINERALS_TEXT_COLOR+i), SMALL_SHADOW_BOLD_FONT, DO_NOT_ADJUST); // TODO
		resourceNumber[i] = new UI_StaticText(this, Rect(Point(75, 16+11*i), Size(0,0)), Size(0,0), (eColor)(BRIGHT_MINERALS_TEXT_COLOR+i), SMALL_SHADOW_BOLD_FONT, DO_NOT_ADJUST);
	} // TODO falls mehr
	resetData();
	addHelpButton(DESCRIPTION_BODIAGRAM_WINDOW_CHAPTER);
	BoDiagramPoint::MAX_ENTRIES = RACE::MAX_RESOURCE_TYPES+2;
	HAVE_SUPPLY = RACE::MAX_RESOURCE_TYPES+0;
	NEED_SUPPLY = RACE::MAX_RESOURCE_TYPES+1;
}


BoDiagramWindow::~BoDiagramWindow()
{ 
	for(unsigned int i = 0; i < RACE::MAX_RESOURCE_TYPES; i++)
	{
		delete resourceText[i];
		delete resourceNumber[i];
	}
	delete supplyText;
	delete supplyNumber;
	delete timeText;
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
	resetData();
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
			for(unsigned int j = BoDiagramPoint::MAX_ENTRIES;j--;)
			{
				if(Size::mv2(i->current_y1[j], i->start_y1[j], i->target_y1[j]))
					has_changed = true;
				if(Size::mv2(i->current_y2[j], i->start_y2[j], i->target_y2[j]))
					has_changed = true;
				if(Size::mv2(i->highlight[j], 150, 100))
					has_changed = true;
			}
		}
	}
	
	if(has_changed)
		setNeedRedrawNotMoved();
	
	
	bold = false;
	if((diagramList.size()>1) && (getAbsoluteClientRect().isInside(mouse)) && (totalTime>0))
	{
		bold = true;
		unsigned int new_mouse_time = totalTime * (mouse.x - getAbsoluteClientRectLeftBound()) / getClientRectWidth();
		if((mouseTime != new_mouse_time) || (has_changed))
		{
			mouseTime = new_mouse_time;
			setNeedRedrawNotMoved();
	
			unsigned int number = 0;
			selectedItems.clear();
			tempSelectedItems.clear();
			firstTime = lastTime = 0;
			for(std::list<PROGRAM>::const_iterator k = anarace->getProgramList().begin(); k != anarace->getProgramList().end(); ++k, ++number)
				if((k->getTime() - k->getBT() < coreConfiguration.getMaxTime()-mouseTime)&&(k->getTime() > coreConfiguration.getMaxTime() - mouseTime))
					selectedItems.push_back(number);


			timeNumber->updateText(formatTime(mouseTime, efConfiguration.getGameSpeed()));
		
			unsigned int my_time = coreConfiguration.getMaxTime() - mouseTime;
			for(std::list<STATISTICS>::const_iterator i = anarace->getTimeStatisticsList().begin(); i != anarace->getTimeStatisticsList().end(); ++i)
			{
				std::list<STATISTICS>::const_iterator j = i;
				++j;
				if(j == anarace->getTimeStatisticsList().end())
					break;
				if(i->getTime() == j->getTime())
					continue;
			
				if((i->getTime() == my_time)||(j==anarace->getTimeStatisticsList().end()))
				{
					std::ostringstream os;
					for(unsigned int k = RACE::MAX_RESOURCE_TYPES; k--;)
					{
						os << i->getHaveResource(k)/100;
						resourceNumber[k]->updateText(os.str());os.str("");
					}
					os << i->getNeedSupply() << "/" << i->getHaveSupply();supplyNumber->updateText(os.str());
					break;
				} else
				if((i->getTime() > my_time) && (j->getTime() < my_time))
				{
					// => Interpolieren zwischen i und j
					std::ostringstream os;
					for(unsigned int k = RACE::MAX_RESOURCE_TYPES; k--;)
					{
						os << (i->getHaveResource(k) + (j->getHaveResource(k) - i->getHaveResource(k)) * (i->getTime() - my_time) / (i->getTime() - j->getTime()))/100; 
						resourceNumber[k]->updateText(os.str());os.str("");
					}
					os << i->getNeedSupply() << "/" << i->getHaveSupply();supplyNumber->updateText(os.str());
					break;
				}
			}

			
			for(unsigned int k = RACE::MAX_RESOURCE_TYPES; k--;)
				resourceNumber[k]->Show();
			supplyNumber->Show();
			timeNumber->Show();
		}
	} else
	{
		selectedItems.clear();
		tempSelectedItems.clear();
		firstTime = lastTime = 0;
		for(unsigned int k = RACE::MAX_RESOURCE_TYPES; k--;)
			resourceNumber[k]->Hide();
		supplyNumber->Hide();
		timeNumber->Hide();
	}
	oldMouse = mouse;
}

void BoDiagramWindow::setSelected(const std::list<unsigned int>& selected)
{
	tempSelectedItems.clear();
	if(selected.empty() || (totalTime == 0))
		return;
	tempSelectedItems = selected;
	unsigned int id = 0;
	for(std::list<PROGRAM>::const_iterator order = anarace->getProgramList().begin(); order != anarace->getProgramList().end(); ++order, ++id)
	{
		if(selected.front() == id)
			firstTime = order->getRealTime() * getClientRectWidth() / totalTime;
		if(selected.back() == id)
		{
			lastTime = (order->getRealTime() + order->getBT()) * getClientRectWidth() / totalTime;
			setNeedRedrawNotMoved();
			break;
		}
	}
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
	
	for(unsigned int i = BoDiagramPoint::MAX_ENTRIES; i--;)
	BoDiagramPoint::max.assign(BoDiagramPoint::MAX_ENTRIES, 1);

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
		unsigned int id = 0;
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
			for(unsigned int i = RACE::MAX_RESOURCE_TYPES; i--;)
				k->setTargetY(i, order->getStatisticsBefore().getHaveResource(i), order->getStatisticsAfter().getHaveResource(i));
			k->setTargetY(HAVE_SUPPLY, order->getStatisticsBefore().getHaveSupply(), order->getStatisticsAfter().getHaveSupply());
			k->setTargetY(NEED_SUPPLY, order->getStatisticsBefore().getNeedSupply(), order->getStatisticsAfter().getNeedSupply());
			k->unit = order->getUnit();
			k->id = id;
			k->initialized = true;
			++k;
			++id;
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
	for(unsigned int i = RACE::MAX_RESOURCE_TYPES; i--;)
		diagramList.front().setTargetY(i, 0, (*anarace->getStartCondition())->getResource(i));
	diagramList.front().setTargetY(HAVE_SUPPLY, 0, (*anarace->getStartCondition())->getHaveSupply());
	diagramList.front().setTargetY(NEED_SUPPLY, 0, (*anarace->getStartCondition())->getNeedSupply());
	diagramList.front().initialized = true;

	diagramList.back().initialized = false; // letzten Eintrag immer sofort anpassen
	diagramList.back().setTargetX(1 + getClientTargetWidth()-5);
	for(unsigned int i = RACE::MAX_RESOURCE_TYPES; i--;)
		diagramList.back().setTargetY(i, anarace->getResource(i), 0);
	diagramList.back().setTargetY(HAVE_SUPPLY, anarace->getHaveSupply(), 0);
	diagramList.back().setTargetY(NEED_SUPPLY, anarace->getNeedSupply(), 0);
	diagramList.back().initialized = true;	

	for(std::list<BoDiagramPoint>::iterator i = diagramList.begin(); i != diagramList.end(); ++i)
		for(unsigned int j = BoDiagramPoint::MAX_ENTRIES; j--;)
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

	dc->setPen(*UI_Object::theme.lookUpPen(OUTER_BORDER_PEN));
	for(unsigned int x = 1; x < 10; x++)
		dc->DrawVerticalLine(x * getClientRectWidth() / 10 + getAbsoluteClientRectLeftBound() + 1, getAbsoluteClientRectUpperBound()+11, getAbsoluteClientRectLowerBound()-1);

	for(unsigned int y = 1; y < 10; y++)
		dc->DrawHorizontalLine(getAbsoluteClientRectLeftBound()+1, y * (getClientRectHeight()-10) / 10 + getAbsoluteClientRectUpperBound()+11, getAbsoluteClientRectRightBound()-1);
	if(anarace==NULL)
		return;
	std::vector<float> scale(BoDiagramPoint::MAX_ENTRIES);
	for(unsigned int i = BoDiagramPoint::MAX_ENTRIES; i--;)
		scale[i] = (float)(getClientRectHeight() - 20)/(float)BoDiagramPoint::max[i];
	
	if(diagramList.size()>0)
	{

// first draw the supply:
		for(std::list<BoDiagramPoint>::const_iterator i = diagramList.begin(); i!=diagramList.end(); ++i)
		{
			std::list<BoDiagramPoint>::const_iterator j = i;
			++j;
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
			
				Pen* pen = theme.lookUpPen(BODIAGRAM_SUPPLY_PEN);
				dc->setPen(Pen(dc->changeRelativeBrightness(*pen->getColor(), high_light), pen->getWidth(), pen->getStyle()));
				
				dc->DrawRectangle(getAbsoluteClientRectPosition() + Point(0, getClientRectHeight()) +
						Point(i->current_x, 0) - Size(0, i->current_y2[HAVE_SUPPLY]*scale[HAVE_SUPPLY]),
						Size(j->current_x - i->current_x, y*scale[HAVE_SUPPLY]) );
			} // end of  j != diagramList.end()
		} // end of for

// then the resources:		
		for(std::list<BoDiagramPoint>::const_iterator i = diagramList.begin(); i!=diagramList.end(); ++i)
		{
			std::list<BoDiagramPoint>::const_iterator j = i;
			++j;
			
			Point p = getAbsoluteClientRectPosition()+Point(0,getClientRectHeight());
			Pen* pen;
			
			int x = p.x + i->current_x;
			
			for(unsigned int k = RACE::MAX_RESOURCE_TYPES; k--;)
			{
				int y1 = p.y - i->current_y1[k] * scale[k];
				int y2 = p.y - i->current_y2[k] * scale[k];
	
				if(y1 + 10 < y2) pen = theme.lookUpPen((ePen)(DASHED_MINERALS_PEN+k));
				else pen = theme.lookUpPen((ePen)(BODIAGRAM_MINERALS_PEN+k));
			
				dc->setPen(Pen(dc->changeRelativeBrightness(*pen->getColor(), i->highlight[k]), pen->getWidth(), pen->getStyle()));
				if(y1<y2) dc->DrawVerticalLine(x, y1, y2);else  dc->DrawVerticalLine(x, y2, y1);
			}

			if(j!=diagramList.end())
			{
				for(unsigned int k = 0; k < RACE::MAX_RESOURCE_TYPES; k++)
				{
					pen = theme.lookUpPen((ePen)(BODIAGRAM_MINERALS_PEN+k+(((bold)||(i->highlight[k]>100))?3:0)));
					dc->setPen(Pen(dc->changeRelativeBrightness(*pen->getColor(), i->highlight[k]), pen->getWidth(), pen->getStyle()));
					dc->DrawLine(p.x + i->current_x, p.y - i->current_y2[k] * scale[k], p.x + j->current_x, p.y - j->current_y1[k] * scale[k]);
				}
			} // end of  j != diagramList.end()
		} // end of for

// and finally the unit text:
		dc->setFont(theme.lookUpFont(SMALL_SHADOW_BOLD_FONT));
		for(std::list<BoDiagramPoint>::const_iterator i = diagramList.begin(); i!=diagramList.end(); ++i)
		{
			std::list<BoDiagramPoint>::const_iterator j = i;
			++j;
			
			Point p = getAbsoluteClientRectPosition()+Point(0,getClientRectHeight());
			
			if(j!=diagramList.end())
			{
				if(i!=diagramList.begin())
				{
					int y1 = p.y - i->current_y1[0] * scale[0];
					int y2 =  p.y - i->current_y2[0] * scale[0];
					int x = p.x + i->current_x;

					// TODO sehr ungenau...
					bool mark = false;
					for(std::list<unsigned int>::const_iterator l = tempSelectedItems.begin(); l != tempSelectedItems.end(); ++l)
						if(*l == i->id)
						{
							mark = true;
							break;
						}
						
					if(mark)	
						dc->setTextForeground(*theme.lookUpColor(IMPORTANT_COLOR));
					else
						dc->setTextForeground(*theme.lookUpColor(BRIGHT_TEXT_COLOR));
					unsigned int width = (theme.lookUpFont(SMALL_SHADOW_BOLD_FONT)->getTextExtent(GAME::lookUpUnitString(anarace->getRace(), i->unit))).getWidth();
					unsigned int tx;
					if(x - width/2 < getAbsoluteClientRectLeftBound())
						tx = getAbsoluteClientRectLeftBound();
					else if(x + width/2 >= getAbsoluteClientRectRightBound())
						tx = getAbsoluteClientRectRightBound() - width;
					else 
						tx = x - width/2;
					dc->DrawText(GAME::lookUpUnitString(anarace->getRace(), i->unit), Point(tx, y1 + (y2 - y1)/2));
				}
			} // end of  j != diagramList.end()
		} // end of for

	
		
	} // end of diagramList.size() > 0
	if(bold && (totalTime > 0))
	{
		dc->setBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
		dc->setPen(*theme.lookUpPen(SELECT_PEN));
		dc->DrawEmptyRectangle( getAbsoluteClientRectLeftBound() - 2 + (mouseTime * getClientRectWidth()) / totalTime, getAbsoluteClientRectUpperBound()+10, 4, getClientRectHeight()-10);
	} else if(firstTime != lastTime)
	{
		dc->setBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
		dc->setPen(*theme.lookUpPen(SELECT_PEN));
		dc->DrawEmptyRectangle( getAbsoluteClientRectLeftBound() - 2 + firstTime, getAbsoluteClientRectUpperBound()+10, lastTime - firstTime, getClientRectHeight()-10);	
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

unsigned int BoDiagramWindow::HAVE_SUPPLY = 0;
unsigned int BoDiagramWindow::NEED_SUPPLY = 0;

