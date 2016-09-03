#include "diagram.hpp"
#include <sstream>

UI_Diagram::UI_Diagram(UI_Object* diagram_parent, 
		const Rect& diagram_rect, 
		const Size distance_bottom_right,
		const ePositionMode position_mode) :
	UI_Object(diagram_parent, 
			diagram_rect, 
			distance_bottom_right, 
			position_mode),
	graphMax(),
	graphList(),
	graphType(),
	graphPen(),
	graphDashedPen(),
	graphBrush(),
	itemText(),
	itemValue()
{
	resetData();
}


UI_Diagram::~UI_Diagram()
{ 
	for(std::vector<UI_StaticText*>::iterator i = itemText.begin(); i != itemText.end(); ++i)
		delete(*i);
	itemText.clear();
	for(std::vector<UI_StaticText*>::iterator i = itemValue.begin(); i != itemValue.end(); ++i)
		delete(*i);
	itemValue.clear();
}

void UI_Diagram::showValues(const bool show) 
{
	for(std::vector<UI_StaticText*>::iterator i = itemValue.begin(); i!= itemValue.end(); ++i)
		if(show)
			(*i)->Show();
		else (*i)->Hide();
}

void UI_Diagram::addItem(const unsigned int item_text, const eColor text_color, const eColor value_color)
{
	itemText.push_back(new UI_StaticText(this, GAME::lookUpGameString(item_text), Rect(Point(8, 15+(FONT_SIZE+5) * itemText.size()), Size(0,0)), Size(0,0), text_color, SMALL_SHADOW_BOLD_FONT, DO_NOT_ADJUST));
	itemValue.push_back(new UI_StaticText(this, Rect(Point(75, 16 + (FONT_SIZE+5) * itemValue.size()), Size(0,0)), Size(0,0), value_color, SMALL_SHADOW_BOLD_FONT, DO_NOT_ADJUST));
}

void UI_Diagram::addSecondBoxGraph(const ePen graph_pen, const eBrush graph_brush)
{
	graphPen.push_back(graph_pen);
	graphDashedPen.push_back(NULL_PEN);
	graphBrush.push_back(graph_brush);
	graphType.push_back(GRAPH_SECOND_BOX);
	std::list<UI_GraphPoint> t;
	graphList.push_back(t);
	graphMax.push_back(1);
}

void UI_Diagram::addBoxGraph()
{
	graphPen.push_back(NULL_PEN);
	graphDashedPen.push_back(NULL_PEN);
	graphBrush.push_back(NULL_BRUSH);
	graphType.push_back(GRAPH_BOX);
	std::list<UI_GraphPoint> t;
	graphList.push_back(t);
	graphMax.push_back(1);
}

void UI_Diagram::addLineGraph(const ePen graph_pen, const ePen graph_dashed_pen)
{
	graphPen.push_back(graph_pen);
	graphDashedPen.push_back(graph_dashed_pen);
	graphBrush.push_back(NULL_BRUSH);
	graphType.push_back(GRAPH_LINE);
	std::list<UI_GraphPoint> t;
	graphList.push_back(t);
	graphMax.push_back(1);
}

void UI_Diagram::setItemValues(const std::vector<std::string>& value_list)
{
#ifdef _SCC_DEBUG
	if(value_list.size() != itemValue.size()) {
		toErrorLog("DEBUG (UI_Diagram::setItemValues()): Variable itemValue not initialized or value value_list not initialized.");return;
	}
#endif
	std::vector<std::string>::const_iterator j = value_list.begin();
	for(std::vector<UI_StaticText*>::iterator i = itemValue.begin(); i != itemValue.end(); ++i, ++j)
		(*i)->updateText(*j);
}


void UI_Diagram::reloadOriginalSize()
{
	UI_Object::reloadOriginalSize();
	unsigned int h = 0;
	for(std::vector<UI_StaticText*>::iterator i = itemText.begin(); i != itemText.end(); ++i, ++h)
		(*i)->setOriginalPosition(Point(8, 15 + (FONT_SIZE+5) * h));
	h = 0;
	for(std::vector<UI_StaticText*>::iterator i = itemValue.begin(); i != itemValue.end(); ++i, ++h)
		(*i)->setOriginalPosition(Point(75, 16 + (FONT_SIZE+5) * h));
	resetData();
}

void UI_Diagram::resetData()
{
	for(unsigned int i = 0; i < graphList.size(); ++i)
		graphList[i].clear();
}

void UI_Diagram::process()
{
	if(!isShown()) 
		return;
	UI_Object::process();
	bool has_changed = false;
	
	if(uiConfiguration.isSmoothMovements())
	for(unsigned int k = 0; k < graphList.size(); ++k)
		for(std::list<UI_GraphPoint>::iterator i = graphList[k].begin(); i != graphList[k].end(); ++i)
			if(i->move())
				has_changed = true;
	if(has_changed)
		makePufferInvalid();
}

void UI_Diagram::draw() const
{
	dc->setBrush(*theme.lookUpBrush(BODIAGRAM_BACK1));
	dc->setPen(*UI_Object::theme.lookUpPen(OUTER_BORDER_PEN));
	dc->DrawRectangle(getRelativeRect());
	for(unsigned int x = 1; x < 10; x++)
		dc->DrawVerticalLine(Point(x * (getWidth()-2) / 10 + 1, 1), getHeight() - 2);
	for(unsigned int y = 1; y < 10; y++)
		dc->DrawHorizontalLine(Point(1, y * (getHeight()-2) / 10 + 1), getWidth() - 2);
	
	std::vector<float> scale(graphType.size());
	for(unsigned int i = graphType.size(); i--;)
		scale[i] = (float)(getHeight() - 2)/(float)graphMax[i];

	unsigned int diagram_first_box = 9999;
	
	for(unsigned int i = 0; i < graphType.size(); i++)
		if(graphList[i].size())
		{
			switch(graphType[i])
			{
				case GRAPH_LINE:
				for(std::list<UI_GraphPoint>::const_iterator item = graphList[i].begin(); item != graphList[i].end(); ++item)
				{
					std::list<UI_GraphPoint>::const_iterator next_item = item;
					++next_item;
			
					Point p = Point(0, getHeight());
			
					int x = p.x + item->x;
					
					int y1 = p.y - item->current_y1 * scale[i];
					int y2 = p.y - item->current_y2 * scale[i];
					Pen* pen;
					if(y1 + 10 < y2) pen = theme.lookUpPen(graphDashedPen[i]); // TODO
					else pen = theme.lookUpPen(graphPen[i]);
					dc->setPen(Pen(dc->changeRelativeBrightness(*pen->getColor(), item->highlight), pen->getWidth(), pen->getStyle()));

					if(y1<y2) dc->DrawVerticalLine(Point(x, y1), y2-y1);else  dc->DrawVerticalLine(Point(x, y2), y1-y2);
					

					if(next_item != graphList[i].end())
					{
						pen = theme.lookUpPen((ePen)(graphPen[i] + ((/*(bold)||*/(item->highlight>100))?3:0)));  // TODO
						dc->setPen(Pen(dc->changeRelativeBrightness(*pen->getColor(), item->highlight), pen->getWidth(), pen->getStyle()));
						dc->DrawLine(Point(x, p.y - item->current_y2 * scale[i]), Point(p.x + next_item->x, p.y - next_item->current_y1 * scale[i]));
					} // end of  j != graphList.end()
				} // end of for
				break;
				case GRAPH_FILLED_LINE:break;
				case GRAPH_BOX:diagram_first_box = i;break;
				case GRAPH_SECOND_BOX:{
#ifdef _SCC_DEBUG
				 if(diagram_first_box == 9999) {
					 toErrorLog("DEBUG (UI_Diagram::draw()): diagram_first_box was not initialized!");break;
				 }
#endif
				dc->setBrush(*theme.lookUpBrush(graphBrush[i]));
			
				std::list<UI_GraphPoint>::const_iterator item = graphList[diagram_first_box].begin();
				for(std::list<UI_GraphPoint>::const_iterator item2 = graphList[i].begin(); item2 != graphList[i].end(); ++item, ++item2)
				{
					std::list<UI_GraphPoint>::const_iterator next_item = item2;
					++next_item;
					if(next_item != graphList[i].end())
					{
						unsigned int y;
						if(item->current_y2 < item2->current_y2)
							y = item2->current_y2 - item->current_y2;
						else 
							y = item->current_y2 - item2->current_y2;
						
						unsigned int high_light;
						
						if(item->highlight > item2->highlight)
							high_light = item->highlight;
						else high_light = item2->highlight;

						Pen* pen = theme.lookUpPen(graphPen[i]);
						dc->setPen(Pen(dc->changeRelativeBrightness(*pen->getColor(), high_light), pen->getWidth(), pen->getStyle()));
	
			
						dc->DrawRectangle(Point(0, getHeight()) +
							Point(item->x, 0) - Size(0, item->current_y2 * scale[i]),
							Size(next_item->x - item->x, y * scale[i]) );
					} // end of  j != graphList.end()
				} // end of for
				}
				 break;
				default:break;
			}
		}
				

// first draw the supply:

// and finally the unit text:
		dc->setFont(theme.lookUpFont(SMALL_SHADOW_BOLD_FONT));
		for(unsigned int i = 0; i < graphList.size(); i++)
			if(graphList[i].size())
		for(std::list<UI_GraphPoint>::const_iterator item = graphList[i].begin(); item != graphList[i].end(); ++item)
			if(item->text!="")
		{
			std::list<UI_GraphPoint>::const_iterator next_item = item;
			++next_item;
			
			Point p = Point(0, getHeight());
			if(next_item != graphList[i].end())
			{
				if(item != graphList[i].begin())
				{
					int y1 = p.y - item->current_y1 * scale[i];
					int y2 =  p.y - item->current_y2 * scale[i];
					int x = p.x + item->x;

					// TODO sehr ungenau...
/*					bool mark = false;
					for(std::list<unsigned int>::const_iterator l = tempSelectedItems.begin(); l != tempSelectedItems.end(); ++l)
						if(*l == i->id)
						{
							mark = true;
							break;
						}
						
					if(mark)*/	
/*						dc->setTextForeground(*theme.lookUpColor(IMPORTANT_COLOR));
					else*/
						dc->setTextForeground(*theme.lookUpColor(BRIGHT_TEXT_COLOR));
					unsigned int width = (theme.lookUpFont(SMALL_SHADOW_BOLD_FONT)->getTextExtent(item->text)).getWidth();
					unsigned int tx;
					if(x - width/2 < 0)
						tx = 0;
					else if(x + width/2 >= getWidth())
						tx = getWidth() - width;
					else 
						tx = x - width/2;
					dc->DrawText(item->text, Point(tx, y1 + (y2 - y1)/2));
				}
			} // end of  j != graphList.end()
		} // end of for

	
		
//	} // end of graphList.size() > 0
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
	UI_Object::draw();

}

