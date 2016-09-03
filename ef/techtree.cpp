#include "techtree.hpp"

TechTreeWindow::TechTreeWindow(UI_Object* techtree_parent) :
	UI_Window(techtree_parent, TECHTREE_WINDOW_TITLE_STRING, theme.lookUpGlobalRect(TECHTREE_WINDOW), theme.lookUpGlobalMaxHeight(TECHTREE_WINDOW), NOT_SCROLLED),
	unitChanged(false),
	currentGoalUnit(0),
	anarace(NULL),
	s(Size(100, 15)),
	s2(Size(120, 25))
{	
	for(unsigned int i = UNIT_TYPE_COUNT;i--;)
		node[i] = new UI_Object(this, Rect(Point((getWidth() - s.GetWidth())/2, 15),s));
}

TechTreeWindow::~TechTreeWindow()
{
	for(unsigned int i = UNIT_TYPE_COUNT;i--;)
		delete node[i];
}

void TechTreeWindow::assignAnarace(ANABUILDORDER* techtree_anarace)
{
	if((!anarace)||(anarace->getRace() != techtree_anarace->getRace()))
		unitChanged = true;
	anarace = techtree_anarace;
	
}

void TechTreeWindow::reloadOriginalSize()
{
	setOriginalRect(UI_Object::theme.lookUpGlobalRect(TECHTREE_WINDOW));
	setMaxHeight(UI_Object::theme.lookUpGlobalMaxHeight(TECHTREE_WINDOW));
	UI_Window::reloadOriginalSize();
}

void TechTreeWindow::processList()
{
	signed int x1 = 0;
	signed int x2 = 5*s2.GetWidth();
	signed int y2 = 10 * s2.GetHeight() + 10;

	signed int max_width = 0;
	signed int max_height = 0;

	bool checked[UNIT_TYPE_COUNT];
	tree = anarace->getGoalTree(currentGoalUnit);

	for(unsigned int i = UNIT_TYPE_COUNT;i--;)
		checked[i]=false;

	x1=9999;
	for(unsigned int k = 0; k<10; ++k)
		if(tree.width[k]>0)
		{
			bool foundOne = false;
			unsigned int x=0;
			int px=0;
			for(std::list<unsigned int>::const_iterator i = tree.unit[k].begin(); i!=tree.unit[k].end();++i)
			{
				px=x*5*s2.GetWidth()/tree.width[k] + (5*s2.GetWidth()/tree.width[k])/2;
				foundOne=true;
				if(x1 > px - (signed int)(s.GetWidth())/2)
					x1 = px - (s.GetWidth())/2;
				Rect edge = Rect(Point(px - s.GetWidth()/2, 15+k*s2.GetHeight()), s);
				node[*i]->adjustRelativeRect(edge);
				if(edge.GetRight() > max_width)
					max_width = edge.GetRight();
				if(edge.GetBottom() > max_height)
					max_height = edge.GetBottom();

				
					
				node[*i]->Show();
				checked[*i]=true;
                                ++x;
			}
			if((px > x2))// /+ (s.GetWidth())/2)>x2)
				x2 = px;// + (s.GetWidth())/2;
			if(foundOne)
				y2 = k*s2.GetHeight()+3+s.GetHeight();
		}
	for(unsigned int i = UNIT_TYPE_COUNT;i--;)
		if(!checked[i])
		{
			node[i]->setPosition(Point((getWidth() - s.GetWidth())/2, 15));
			node[i]->Hide();
		}
	if(max_width > getOriginalRect().GetWidth())
		max_width = getOriginalRect().GetWidth();
	if(max_height > getOriginalRect().GetHeight())
		max_height = getOriginalRect().GetHeight();

	adjustRelativeRect(Rect(getOriginalRect().GetLeft() + (getOriginalRect().GetWidth() - max_width)/2, getOriginalRect().GetTop() + (getOriginalRect().GetHeight() - max_height)/2, max_width+20 , max_height+20));
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

void TechTreeWindow::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Window::draw(dc);


// TODO umformen
/*	// draw outer border:
	dc->SetPen(*theme.lookUpPen(OUTER_BORDER_PEN));
	dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
	dc->DrawEdgedRoundedRectangle(x1-8, p.y-8, (unsigned int)(x2 - x1 + 6), (unsigned int)(y2 - p.y + 16), 6);

	// draw inner border:
	dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
	dc->DrawEdgedRoundedRectangle(x1-5, p.y-5, (unsigned int)(x2 - x1), (unsigned int)(y2 - p.y + 10), 6);*/

	for(unsigned int k = 0; k<10; ++k)
		if(tree.width[k]>0)
		{
			unsigned int x=0;
			for(std::list<unsigned int>::const_iterator i = tree.unit[k].begin(); i!=tree.unit[k].end();++i)
			{
				dc->SetBrush(*theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[anarace->getRace()][*i].unitType)));
				dc->SetPen(*theme.lookUpPen((ePen)(UNIT_TYPE_0_BOLD_PEN+stats[anarace->getRace()][*i].unitType)));
				
				for(std::list<unsigned int>::const_iterator j = tree.con[*i].begin(); j!= tree.con[*i].end(); ++j)
				{
					Point start = node[*i]->getAbsolutePosition();
					Point target = node[*j]->getAbsolutePosition();
					Point sp, tp;
				
					if(tree.level[*j] > k) // Draw from Bottom to Top of target
					{
						sp = Point(s.GetWidth()/2, s.GetHeight());
						tp = Point(s.GetWidth()/2, 0);
					}
					else if(tree.level[*j] == k) // Draw horizontal line
					{
						if(tree.coloumn[*j]<x) // left to right side of target
						{
							sp = Point(0, s.GetHeight()/2);
							tp = Point(s.GetWidth(), s.GetHeight()/2);
						} else // right to left side of target
						{
							sp = Point(s.GetWidth(), s.GetHeight()/2);
							tp = Point(0, s.GetHeight()/2);
						}
					}
					else // Draw from Top to Bottom of target
					{
						sp = Point(s.GetWidth()/2, 0);
						tp = Point(s.GetWidth()/2, s.GetHeight());
					}

					dc->DrawLine(start + sp, target + tp);
	
				/*	if(tree.level[*j] > k) // Draw from Bottom to Top of target
						dc->DrawLine(getAbsolutePosition() + Point(x*5*s2.GetWidth()/tree.width[k]+s.GetWidth()/2 + (5*s2.GetWidth()/tree.width[k] - s.GetWidth())/2, s.GetHeight()+k*s2.GetHeight()), 
							     getAbsolutePosition() + Point(tree.coloumn[*j]*5*s2.GetWidth()/tree.width[tree.level[*j]]+s.GetWidth()/2 + (5*s2.GetWidth()/tree.width[tree.level[*j]] - s.GetWidth())/2, tree.level[*j]*s2.GetHeight()));
					else if(tree.level[*j] == k) // Draw horizontal line
					{
						if(tree.coloumn[*j]<x) // left to right side of target
							dc->DrawLine(getAbsolutePosition() + Point(x*5*s2.GetWidth()/tree.width[k]+(5*s2.GetWidth()/tree.width[k] - s.GetWidth())/2, s.GetHeight()/2+k*s2.GetHeight()), 
								     getAbsolutePosition() + Point(tree.coloumn[*j]*5*s2.GetWidth()/tree.width[k]+s.GetWidth() + (5*s2.GetWidth()/tree.width[k] - s.GetWidth())/2, s.GetHeight()/2+k*s2.GetHeight()));
						else  // right to left side of target
							dc->DrawLine(getAbsolutePosition() + Point(x*5*s2.GetWidth()/tree.width[k]+s.GetWidth()+(5*s2.GetWidth()/tree.width[k] - s.GetWidth())/2, s.GetHeight()/2+k*s2.GetHeight()), 
								     getAbsolutePosition() + Point(tree.coloumn[*j]*5*s2.GetWidth()/tree.width[k] + (5*s2.GetWidth()/tree.width[k] - s.GetWidth())/2, s.GetHeight()/2+k*s2.GetHeight()));
					}
					else // Draw from Top to Bottom of target
						dc->DrawLine(getAbsolutePosition() + Point(x*5*s2.GetWidth()/tree.width[k]+s.GetWidth()/2+(5*s2.GetWidth()/tree.width[k] - s.GetWidth())/2, k*s2.GetHeight()), 
							     getAbsolutePosition() + Point(tree.coloumn[*j]*5*s2.GetWidth()/tree.width[tree.level[*j]]+s.GetWidth()/2+(5*s2.GetWidth()/tree.width[tree.level[*j]] - s.GetWidth())/2, s.GetHeight()+tree.level[*j]*s2.GetHeight()));*/
				}
				++x;
			}
		}
			
	dc->SetTextForeground(*UI_Object::theme.lookUpColor(BRIGHT_TEXT_COLOR));
	dc->setPressedRectangle(false);	
	dc->SetFont(UI_Object::theme.lookUpFont(SMALL_BOLD_FONT));
	for(unsigned int i = UNIT_TYPE_COUNT;i--;)
		if(node[i]->isShown())
		{
			dc->SetBrush(*theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[anarace->getRace()][i].unitType)));
			dc->SetPen(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN+stats[anarace->getRace()][i].unitType)));
			dc->DrawEdgedRoundedRectangle(node[i]->getAbsoluteRect(), 4);
			std::string text = UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+i));
			Size text_size = dc->GetTextExtent(text);
			dc->DrawText(text, node[i]->getAbsolutePosition() + Point(2 + (s.GetWidth())/2, 5) - Size(text_size.GetWidth()/2, 0));
		}
		
		

/*	for(unsigned int k = 0; k<10; ++k)
		if(tree.width[k]>0)
		{
			unsigned int x=0;
			int px=0;
			for(std::list<unsigned int>::const_iterator i = tree.unit[k].begin(); i!=tree.unit[k].end(); ++i)
			{
				px=x*5*s2.GetWidth()/tree.width[k] + (5*s2.GetWidth()/tree.width[k])/2;
				dc->SetBrush(*theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[anarace->getRace()][*i].unitType)));
				dc->SetPen(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN+stats[anarace->getRace()][*i].unitType)));
				
				dc->DrawEdgedRoundedRectangle(Rect(getAbsolutePosition() + Point(px - (s.GetWidth())/2, k*s2.GetHeight()), s), 4);
				std::string text = UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+(*i)));
				Size text_size = dc->GetTextExtent(text);
				dc->DrawText(text, getAbsolutePosition() + Point(px+4- (text_size.GetWidth())/2, k*s2.GetHeight()+4));
				++x;
			}
		}*/
}





