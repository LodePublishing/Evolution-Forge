#include "bodiagram.hpp"
#include "../core/configuration.hpp"
#include <sstream>

BoDiagramWindow::BoDiagramWindow(UI_Object* bod_parent, const unsigned int game_number, const unsigned int game_max, const unsigned int player_number, const unsigned int player_max) :
	UI_Window(bod_parent, BODIAGRAM_WINDOW_TITLE_STRING, theme.lookUpPlayerRect(BUILD_ORDER_DIAGRAM_WINDOW, game_number, game_max, player_number, player_max), theme.lookUpPlayerMaxHeight(BUILD_ORDER_DIAGRAM_WINDOW, game_number, game_max, player_number, player_max), NOT_SCROLLED),
	diagram(new UI_Diagram(this, Rect(getRelativeClientRectPosition() + Point(0,10), getClientRectSize() - Size(0,10)), Size(), DO_NOT_ADJUST)),
	anarace(NULL),
	bold(false),
	mouseTime(0),
	totalTime(0),
	oldMouse(),
	gameNumber(game_number),
	gameMax(game_max),
	playerNumber(player_number),
	playerMax(player_max),
	firstTime(0),
	lastTime(0)	
{
	resetData();
	addHelpButton(DESCRIPTION_BODIAGRAM_WINDOW_CHAPTER);
	
	HAVE_SUPPLY = RACE::MAX_RESOURCE_TYPES+0;
	NEED_SUPPLY = RACE::MAX_RESOURCE_TYPES+1;

	for(unsigned int i = 0; i < RACE::MAX_RESOURCE_TYPES; i++)
	{
		diagram->addItem((eString)(GAME::FIRST_RESOURCE_STRING + i), (eColor)(MINERALS_TEXT_COLOR+i), (eColor)(BRIGHT_MINERALS_TEXT_COLOR+i));
		diagram->addLineGraph((ePen)(BODIAGRAM_MINERALS_PEN+i), (ePen)(BODIAGRAM_DASHED_MINERALS_PEN+i));
	}
	diagram->addItem(GAME::GAME_DIAGRAM_SUPPLY_STRING, SUPPLY_TEXT_COLOR, BRIGHT_SUPPLY_TEXT_COLOR);
	diagram->addBoxGraph();
	diagram->addSecondBoxGraph(BODIAGRAM_SUPPLY_PEN, BODIAGRAM_SUPPLY_BRUSH);
	
	diagram->addItem(GAME::GAME_DIAGRAM_TIME_STRING,  FITNESS_TEXT_COLOR, BRIGHT_FITNESS_TEXT_COLOR);
}


BoDiagramWindow::~BoDiagramWindow()
{ 
	delete diagram;
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
	diagram->setOriginalSize(getClientRectSize() - Size(0,10));
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
	diagram->resetData();
	processList();
}

void BoDiagramWindow::process()
{
	if(!isShown()) 
		return;
	UI_Window::process();
	
	bold = false;
	if((getAbsoluteClientRect().isTopLeftCornerInside(mouse)) && (totalTime>0))
	{
		bold = true;
		unsigned int new_mouse_time = totalTime * (mouse.x - getAbsoluteClientRectLeftBound()) / getClientRectWidth();
		if(mouseTime != new_mouse_time)
		{
			mouseTime = new_mouse_time;
			makePufferInvalid();
	
			unsigned int number = 0;
			selectedItems.clear();
			tempSelectedItems.clear();
			firstTime = lastTime = 0;
			for(std::list<PROGRAM>::const_iterator k = anarace->getProgramList().begin(); k != anarace->getProgramList().end(); ++k, ++number)
				if((k->getTime() - k->getBT() < coreConfiguration.getMaxTime()-mouseTime)&&(k->getTime() > coreConfiguration.getMaxTime() - mouseTime))
					selectedItems.push_back(number);

			std::vector<std::string> value_list;
			
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
						value_list.push_back(os.str());
						os.str("");
					}
					os << i->getNeedSupply() << "/" << i->getHaveSupply();
					value_list.push_back(os.str());
					break;
				} else
				if((i->getTime() > my_time) && (j->getTime() < my_time))
				{
					// => Interpolieren zwischen i und j
					std::ostringstream os;
					for(unsigned int k = RACE::MAX_RESOURCE_TYPES; k--;)
					{
						os << (i->getHaveResource(k) + (j->getHaveResource(k) - i->getHaveResource(k)) * (i->getTime() - my_time) / (i->getTime() - j->getTime()))/100;
						value_list.push_back(os.str());
						os.str("");
					}
					os << i->getNeedSupply() << "/" << i->getHaveSupply();
					value_list.push_back(os.str());
					break;
				}
			}
			value_list.push_back(formatTime(mouseTime, efConfiguration.getGameSpeed()));
			diagram->setItemValues(value_list);
			diagram->showValues();
		}
	} else
	{
		selectedItems.clear();
		tempSelectedItems.clear();
		firstTime = lastTime = 0;
		diagram->showValues(false);
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
			makePufferInvalid();
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
	
	std::vector<std::list<UI_GraphPoint> >& graph_list = diagram->getGraphList();

	for(unsigned int i = 0; i < graph_list.size(); i++)
	{
	if(graph_list[i].size()==0)
	{
		UI_GraphPoint p;
		graph_list[i].push_front(p);
	}
// ?? TODO
	bool new_items = false;		

	if(totalTime>0)
	{
		std::list<UI_GraphPoint>::iterator k = graph_list[i].begin();
		++k;
		unsigned int id = 0;
		for(std::list<PROGRAM>::const_iterator order = anarace->getProgramList().begin(); order != anarace->getProgramList().end(); ++order)
		{
			if(k == graph_list[i].end())
			{
				UI_GraphPoint p;
				graph_list[i].push_back(p);
				--k;
				new_items = true;
			}
			unsigned int x = 1 + (int)(order->getRealTime() * (getClientTargetWidth()-5) / (float)(totalTime));
			k->setTargetX(x);
			if(i < RACE::MAX_RESOURCE_TYPES)
				k->setTargetY(order->getStatisticsBefore().getHaveResource(i), order->getStatisticsAfter().getHaveResource(i));
			else if(i == HAVE_SUPPLY)
				k->setTargetY(order->getStatisticsBefore().getHaveSupply(), order->getStatisticsAfter().getHaveSupply());
			else if(i == NEED_SUPPLY)
				k->setTargetY(order->getStatisticsBefore().getNeedSupply(), order->getStatisticsAfter().getNeedSupply());
			if(i==0)
				k->setText(GAME::lookUpUnitString(anarace->getRace(), order->getUnit()));
			++k; // TODO
		}
		while(k != graph_list[i].end())
			k = graph_list[i].erase(k);
	}

	if((new_items)||(graph_list[i].size()==1))
	{
		UI_GraphPoint p;
		graph_list[i].push_back(p);
	}

	graph_list[i].front().setTargetX(1);
	graph_list[i].back().setTargetX(1 + getClientTargetWidth()-5);
	
	if( i < RACE::MAX_RESOURCE_TYPES)
	{
		graph_list[i].front().setTargetY(0, (*anarace->getStartCondition())->getResource(i));
		graph_list[i].back().setTargetY(anarace->getResource(i), 0);
	}
	else if(i == HAVE_SUPPLY)
	{
		graph_list[i].front().setTargetY(0, (*anarace->getStartCondition())->getHaveSupply());
		graph_list[i].back().setTargetY(anarace->getHaveSupply(), 0);
	}
	else if(i == NEED_SUPPLY)
	{
		graph_list[i].front().setTargetY(0, (*anarace->getStartCondition())->getNeedSupply());
		graph_list[i].back().setTargetY(anarace->getNeedSupply(), 0);
	}
	
	for(std::list<UI_GraphPoint>::iterator item = graph_list[i].begin(); item != graph_list[i].end(); ++item)
	{
		if(item->current_y1 > diagram->graphMax[i]) diagram->graphMax[i] = item->current_y1;
		if(item->current_y2 > diagram->graphMax[i]) diagram->graphMax[i] = item->current_y2;
		if(item->target_y1 > diagram->graphMax[i]) diagram->graphMax[i] = item->target_y1;
		if(item->target_y2 > diagram->graphMax[i]) diagram->graphMax[i] = item->target_y2;
	}

	}
	// special rule for supply:
	if(diagram->graphMax[HAVE_SUPPLY] > diagram->graphMax[NEED_SUPPLY])
		diagram->graphMax[NEED_SUPPLY] = diagram->graphMax[HAVE_SUPPLY];
	else if(diagram->graphMax[NEED_SUPPLY] > diagram->graphMax[HAVE_SUPPLY])
		diagram->graphMax[HAVE_SUPPLY] = diagram->graphMax[NEED_SUPPLY];
}

void BoDiagramWindow::draw() const
{
	UI_Window::drawWindow();
	if(anarace==NULL)
		return;
	UI_Object::draw();
	
	if(bold && (totalTime > 0))
	{
		dc->setBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
		dc->setPen(*theme.lookUpPen(SELECT_PEN));
		dc->DrawEmptyRectangle( Rect(getRelativeClientRectLeftBound() - 2 + (mouseTime * getClientRectWidth()) / totalTime, getRelativeClientRectUpperBound()+10, 4, getClientRectHeight()-10));
	} else if(firstTime != lastTime)
	{
		dc->setBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
		dc->setPen(*theme.lookUpPen(SELECT_PEN));
		dc->DrawEmptyRectangle( Rect(getRelativeClientRectLeftBound() - 2 + firstTime, getRelativeClientRectUpperBound()+10, lastTime - firstTime, getClientRectHeight()-10));	
	}
}

unsigned int BoDiagramWindow::HAVE_SUPPLY = 0;
unsigned int BoDiagramWindow::NEED_SUPPLY = 0;

