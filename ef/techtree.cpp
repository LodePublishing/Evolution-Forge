#include "techtree.hpp"
#include "../core/database.hpp"

TechTreeWindow::TechTreeWindow(UI_Object* techtree_parent) :
	UI_Window(techtree_parent, TECHTREE_WINDOW_TITLE_STRING, theme.lookUpGlobalRect(TECHTREE_WINDOW), theme.lookUpGlobalMaxHeight(TECHTREE_WINDOW), NOT_SCROLLED),
	unitChanged(false),
	currentGoalUnit(0),
	anarace(NULL),
	s(),
	s2()
{
	reloadOriginalSize();
}

TechTreeWindow::~TechTreeWindow()
{
	if(node.size() > 0)
		for(unsigned int i = RACE::UNIT_TYPE_COUNT;i--;)
			delete node[i];
}

void TechTreeWindow::assignAnarace(ANABUILDORDER* techtree_anarace)
{
	if((!anarace)||(anarace->getRace() != techtree_anarace->getRace()))
		unitChanged = true;
	if(anarace == NULL)
	{
		node.resize(RACE::UNIT_TYPE_COUNT);
		for(unsigned int i = RACE::UNIT_TYPE_COUNT;i--;)
			node[i] = new UI_Object(this, Rect(Point((getWidth() - s.getWidth())/2, 15),s));
	}
	anarace = techtree_anarace;
}

void TechTreeWindow::reloadOriginalSize()
{
	setOriginalRect(UI_Object::theme.lookUpGlobalRect(TECHTREE_WINDOW));
	setMaxHeight(UI_Object::theme.lookUpGlobalMaxHeight(TECHTREE_WINDOW));

	s = Size(UI_Object::theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH), FONT_SIZE);
	s2 = Size(UI_Object::theme.lookUpButtonWidth(SMALL_BUTTON_WIDTH)+10, FONT_SIZE+2);

	UI_Window::reloadOriginalSize();
}

void TechTreeWindow::processList()
{
	return;
	signed int x1 = 0;
	signed int x2 = 5*s2.getWidth();
	signed int y2 = 10 * s2.getHeight() + 10;

	signed int max_width = 0;
	signed int max_height = 0;

	std::vector<bool> checked(RACE::UNIT_TYPE_COUNT);
	checked.assign(RACE::UNIT_TYPE_COUNT, false);
	tree = anarace->getGoalTree(currentGoalUnit);

	x1=9999;
	for(unsigned int k = 0; k<10; ++k)
		if(tree.width[k]>0)
		{
			bool foundOne = false;
			unsigned int x=0;
			int px=0;
			for(std::list<unsigned int>::const_iterator i = tree.unit[k].begin(); i!=tree.unit[k].end();++i)
			{
				px=x*5*s2.getWidth()/tree.width[k] + (5*s2.getWidth()/tree.width[k])/2;
				foundOne=true;
				if(x1 > px - (signed int)(s.getWidth())/2)
					x1 = px - (s.getWidth())/2;
				Rect edge = Rect(Point(px - s.getWidth()/2, 5+k*s2.getHeight()), s);
				node[*i]->adjustRelativeRect(edge);
				if(edge.getRight() > max_width)
					max_width = edge.getRight();
				if(edge.getBottom() > max_height)
					max_height = edge.getBottom();

				
					
				node[*i]->Show();
				checked[*i]=true;
                                ++x;
			}
			if((px > x2))// /+ (s.getWidth())/2)>x2)
				x2 = px;// + (s.getWidth())/2;
			if(foundOne)
				y2 = k*s2.getHeight()+3+s.getHeight();
		}
	for(unsigned int i = RACE::UNIT_TYPE_COUNT;i--;)
		if(!checked[i])
		{
			node[i]->setPosition(Point((getWidth() - s.getWidth())/2, 15));
			node[i]->Hide();
		}
	if(max_width > getOriginalRect().getWidth())
		max_width = getOriginalRect().getWidth();
	if(max_height > getOriginalRect().getHeight())
		max_height = getOriginalRect().getHeight();

	adjustRelativeRect(Rect(getOriginalRect().getLeft() + (getOriginalRect().getWidth() - max_width)/2, getOriginalRect().getTop() + (getOriginalRect().getHeight() - max_height)/2, max_width+20 , max_height+20));
}

void TechTreeWindow::process()
{
	if(!isShown())
		return;
	if(unitChanged)
	{
		processList();
		unitChanged=false;
	}
	UI_Window::process();
}

void TechTreeWindow::draw() const
{
	UI_Window::draw();
	return; // TODO

// TODO umformen
/*	// draw outer border:
	dc->setPen(*theme.lookUpPen(OUTER_BORDER_PEN));
	dc->setBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
	dc->DrawEdgedRoundedRectangle(x1-8, p.y-8, (unsigned int)(x2 - x1 + 6), (unsigned int)(y2 - p.y + 16), 6);

	// draw inner border:
	dc->setPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	dc->setBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
	dc->DrawEdgedRoundedRectangle(x1-5, p.y-5, (unsigned int)(x2 - x1), (unsigned int)(y2 - p.y + 10), 6);*/

	for(unsigned int k = 0; k<10; ++k)
		if(tree.width[k]>0)
		{
			unsigned int x=0;
			for(std::list<unsigned int>::const_iterator i = tree.unit[k].begin(); i!=tree.unit[k].end();++i)
			{
				dc->setBrush(*theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH + GAME::race[anarace->getRace()].stats[*i].unitType)));
				dc->setPen(*theme.lookUpPen((ePen)(UNIT_TYPE_0_BOLD_PEN + GAME::race[anarace->getRace()].stats[*i].unitType)));
				
				for(std::list<unsigned int>::const_iterator j = tree.connection[*i].begin(); j!= tree.connection[*i].end(); ++j)
				{
					Point start = node[*i]->getAbsolutePosition();
					Point target = node[*j]->getAbsolutePosition();
					Point sp, tp;
				
					if(tree.level[*j] > k) // Draw from Bottom to Top of target
					{
						sp = Point(s.getWidth()/2, s.getHeight());
						tp = Point(s.getWidth()/2, 0);
					}
					else if(tree.level[*j] == k) // Draw horizontal line
					{
						if(tree.coloumn[*j]<x) // left to right side of target
						{
							sp = Point(0, s.getHeight()/2);
							tp = Point(s.getWidth(), s.getHeight()/2);
						} else // right to left side of target
						{
							sp = Point(s.getWidth(), s.getHeight()/2);
							tp = Point(0, s.getHeight()/2);
						}
					}
					else // Draw from Top to Bottom of target
					{
						sp = Point(s.getWidth()/2, 0);
						tp = Point(s.getWidth()/2, s.getHeight());
					}

					dc->DrawLine(start + sp, target + tp);
	
				/*	if(tree.level[*j] > k) // Draw from Bottom to Top of target
						dc->DrawLine(getAbsolutePosition() + Point(x*5*s2.getWidth()/tree.width[k]+s.getWidth()/2 + (5*s2.getWidth()/tree.width[k] - s.getWidth())/2, s.getHeight()+k*s2.getHeight()), 
							     getAbsolutePosition() + Point(tree.coloumn[*j]*5*s2.getWidth()/tree.width[tree.level[*j]]+s.getWidth()/2 + (5*s2.getWidth()/tree.width[tree.level[*j]] - s.getWidth())/2, tree.level[*j]*s2.getHeight()));
					else if(tree.level[*j] == k) // Draw horizontal line
					{
						if(tree.coloumn[*j]<x) // left to right side of target
							dc->DrawLine(getAbsolutePosition() + Point(x*5*s2.getWidth()/tree.width[k]+(5*s2.getWidth()/tree.width[k] - s.getWidth())/2, s.getHeight()/2+k*s2.getHeight()), 
								     getAbsolutePosition() + Point(tree.coloumn[*j]*5*s2.getWidth()/tree.width[k]+s.getWidth() + (5*s2.getWidth()/tree.width[k] - s.getWidth())/2, s.getHeight()/2+k*s2.getHeight()));
						else  // right to left side of target
							dc->DrawLine(getAbsolutePosition() + Point(x*5*s2.getWidth()/tree.width[k]+s.getWidth()+(5*s2.getWidth()/tree.width[k] - s.getWidth())/2, s.getHeight()/2+k*s2.getHeight()), 
								     getAbsolutePosition() + Point(tree.coloumn[*j]*5*s2.getWidth()/tree.width[k] + (5*s2.getWidth()/tree.width[k] - s.getWidth())/2, s.getHeight()/2+k*s2.getHeight()));
					}
					else // Draw from Top to Bottom of target
						dc->DrawLine(getAbsolutePosition() + Point(x*5*s2.getWidth()/tree.width[k]+s.getWidth()/2+(5*s2.getWidth()/tree.width[k] - s.getWidth())/2, k*s2.getHeight()), 
							     getAbsolutePosition() + Point(tree.coloumn[*j]*5*s2.getWidth()/tree.width[tree.level[*j]]+s.getWidth()/2+(5*s2.getWidth()/tree.width[tree.level[*j]] - s.getWidth())/2, s.getHeight()+tree.level[*j]*s2.getHeight()));*/
				}
				++x;
			}
		}
			
	dc->setTextForeground(*UI_Object::theme.lookUpColor(BRIGHT_TEXT_COLOR));
	dc->setPressedRectangle(false);	
	dc->setFont(UI_Object::theme.lookUpFont(SMALL_BOLD_FONT));
	for(unsigned int i = RACE::UNIT_TYPE_COUNT;i--;)
		if(node[i]->isShown())
		{
			dc->setBrush(*theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH + GAME::race[anarace->getRace()].stats[i].unitType)));
			dc->setPen(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN + GAME::race[anarace->getRace()].stats[i].unitType)));
			dc->DrawEdgedRoundedRectangle(node[i]->getAbsoluteRect(), 4);
			std::string text = GAME::lookUpUnitString(anarace->getRace(), i);
			Size text_size = dc->getTextExtent(text);
			dc->DrawText(text, node[i]->getAbsolutePosition() + s/2 - text_size/2);
		}
		
		

/*	for(unsigned int k = 0; k<10; ++k)
		if(tree.width[k]>0)
		{
			unsigned int x=0;
			int px=0;
			for(std::list<unsigned int>::const_iterator i = tree.unit[k].begin(); i!=tree.unit[k].end(); ++i)
			{
				px=x*5*s2.getWidth()/tree.width[k] + (5*s2.getWidth()/tree.width[k])/2;
				dc->setBrush(*theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[anarace->getRace()][*i].unitType)));
				dc->setPen(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN+stats[anarace->getRace()][*i].unitType)));
				
				dc->DrawEdgedRoundedRectangle(Rect(getAbsolutePosition() + Point(px - (s.getWidth())/2, k*s2.getHeight()), s), 4);
				std::string text = UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+(*i)));
				Size text_size = dc->getTextExtent(text);
				dc->DrawText(text, getAbsolutePosition() + Point(px+4- (text_size.getWidth())/2, k*s2.getHeight()+4));
				++x;
			}
		}*/
}





