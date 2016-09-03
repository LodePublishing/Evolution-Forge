#include "group.hpp"

UI_Group::UI_Group(UI_Object* group_parent, 
		const Rect& initial_rect, 
		const Size bottom_right_distance, 
		const eGroupType group_type, 
		const bool draw_background, 
		const ePositionMode position_mode, 
		const eString txt ) :
	UI_Object(group_parent, initial_rect, bottom_right_distance, position_mode, NOTHING),
	title(txt==NULL_STRING?NULL:new UI_StaticText(this, txt, Rect(Point(0,0) - Size(3, 14), Size(0,0)), Size(0,0), BRIGHT_TEXT_COLOR, SMALL_BOLD_FONT, DO_NOT_ADJUST)),
	highlighted(false),
	groupType(group_type),
	drawBackground(draw_background)
{ 
	setDrawType(TRANSPARENT_OBJECT);
}

UI_Group::~UI_Group() 
{
	delete title;
}

void UI_Group::calculateBoxSize()
{
	UI_Object* tmp = getChildren();
	if(!tmp)
		return;
	unsigned int i = 0;
	unsigned int current_height = 2;
	do
	{
		if((tmp==title)||(!tmp->isShown())) 
		{
			tmp=tmp->getNextBrother();
			continue;
		}
		switch(groupType)
		{
			case CUSTOM_GROUP:break;
			case ONE_COLOUMN_GROUP:
			{
				tmp->setOriginalPosition(Point(2, current_height + tmp->getDistanceBottomRight().getHeight()));
				current_height += tmp->getHeight() + tmp->getDistanceBottomRight().getHeight();
			}break;			 
			case TWO_COLOUMNS_GROUP:
			{
				tmp->setOriginalPosition(Point(2 + (i%2) * (tmp->getWidth() + tmp->getDistanceBottomRight().getWidth() + 5), (i/2)*(tmp->getHeight()+tmp->getDistanceBottomRight().getHeight()+5  + tmp->getDistanceBottomRight().getHeight())));
				i++;
			}break;
			case HORIZONTAL_GROUP:
			{
				tmp->setOriginalPosition(Point(i * (tmp->getWidth() + tmp->getDistanceBottomRight().getWidth()) + 2, 3 + tmp->getDistanceBottomRight().getHeight() ));
				i++;
			}break;	
		}
		tmp=tmp->getNextBrother();
	} while(tmp!=getChildren());
	
	unsigned int maxWidth = 0;
	unsigned int maxHeight = 0;
	tmp = getChildren();
	do
	{
		if((tmp->isShown())&&(tmp!=title))
		{
			Rect r = tmp->getTargetRect();
			if((r.getRight()>0) && (maxWidth < (unsigned int)(r.getRight())))
				maxWidth = r.getRight();
			if((r.getBottom()>0) && (maxHeight < (unsigned int)(r.getBottom())))
				maxHeight = r.getBottom();
		}
		tmp = tmp->getNextBrother();
	} while(tmp!=getChildren());
	Size s = Size(maxWidth+7, maxHeight+7);

	setOriginalSize(s); // ?
//	adjustRelativeRect(Rect(getTargetPosition(), s));
}

void UI_Group::alignWidth(const unsigned int width)
{
	UI_Object* tmp = getChildren();
	do
	{
		if((tmp->isShown())&&(tmp!=title))
			tmp->setOriginalWidth(width);
		tmp = tmp->getNextBrother();
	} while(tmp!=getChildren());
}

void UI_Group::reloadOriginalSize()
{
	UI_Object::reloadOriginalSize();
	calculateBoxSize();
}

void UI_Group::draw() const
{
	if(title!=NULL)
	{
//		dc->setBrush(*theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH));
//		dc->DrawRoundedRectangle(getAbsolutePosition() - Size(4, 5), getSize(), 4);
		
		Size s = title->getTextSize();
	  	Rect titleRect = Rect(Point(0,0), s + Size(5,2));
		dc->setPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
		dc->setBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
		dc->DrawEdgedRoundedRectangle(titleRect, 2);
	}
	if(drawBackground)
	{
		if(highlighted)
			dc->setPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
		else	
			dc->setPen(*theme.lookUpPen(INNER_BORDER_PEN));
		dc->setBrush(*theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH));
		dc->DrawEdgedRoundedRectangle(Point(0,0), getSize()+Size(6,0), 4);
	}
	UI_Object::draw();
}

void UI_Group::process()
{
//	if(!isShown())
//		return; //?
	UI_Object::process();
//	calculateBoxSize(); ?
//	nur aufrufen wenn: child added/removed, child size changed

	if(childrenWereChanged)
	{
		calculateBoxSize();
		childrenWereChanged = false;
	}
	if(getAbsoluteRect().isTopLeftCornerInside(mouse))
	{
		if(!highlighted)
		{
			highlighted = true;
			makePufferInvalid();
		}
	} else
	{
		if(highlighted)
		{
			makePufferInvalid();
			highlighted = false;
		}
	}
}



