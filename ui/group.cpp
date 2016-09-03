#include "group.hpp"
#include "button.hpp"

UI_Group::UI_Group(UI_Object* group_parent, const Rect& initial_rect, const Size bottom_right_distance, const ePositionMode position_mode, const eString txt ) :
	UI_Object(group_parent, initial_rect, bottom_right_distance, position_mode, AUTO_SIZE),
	title(txt==NULL_STRING?NULL:new UI_StaticText(this, txt, Rect(Point(0,0) - Size(3, 14), Size(0,0)), Size(0,0), BRIGHT_TEXT_COLOR, SMALL_BOLD_FONT, DO_NOT_ADJUST, AUTO_SIZE)),
	highlighted(false)
{ }

UI_Group::~UI_Group() {
	delete title;
}

UI_Group::UI_Group(const UI_Group& object) :
	UI_Object((UI_Object)object),
	title(object.title==NULL?NULL:new UI_StaticText(*object.title)),
	highlighted(false)
{ }

UI_Group& UI_Group::operator=(const UI_Group& object)
{
	(UI_Object)(*this) = (UI_Object)object;
	delete title;
	if(object.title)
		title = new UI_StaticText(*object.title);
	highlighted = object.highlighted;
	return(*this);
}

void UI_Group::reloadStrings() {
	UI_Object::reloadStrings();
}

#include <sstream>
void UI_Group::calculateBoxSize(const eGroupType group_type)
{
	UI_Object* tmp = getChildren();
	if(!tmp)
		return;
//	resetMinXY();
	unsigned int maxWidth = 0;
	unsigned int maxHeight = 0;
	unsigned int i = 0;
	do
	{
		if((tmp==title)||(!tmp->isShown())) 
		{
			tmp=tmp->getNextBrother();
			continue;
		}
		switch(group_type)
		{
			case CUSTOM_GROUP:break;
			case ONE_COLOUMN_GROUP:
			{
				tmp->setOriginalPosition(Point(0, i*(tmp->getHeight()+tmp->getDistanceBottomRight().GetHeight())));
				i++;
			}break;			 
			case TWO_COLOUMNS_GROUP:
			{
				tmp->setOriginalPosition(Point((i%2) * (tmp->getWidth() + tmp->getDistanceBottomRight().GetWidth() + 5), (i/2)*(tmp->getHeight()+tmp->getDistanceBottomRight().GetHeight()+5)));
				i++;
			}break;
			case HORIZONTAL_GROUP:
			{
				tmp->setOriginalPosition(Point(i * (tmp->getWidth() + tmp->getDistanceBottomRight().GetWidth()), 0));
				i++;
			}break;	
		}
		tmp=tmp->getNextBrother();
	} while(tmp!=getChildren());
	
	tmp = getChildren();
	do
	{
		if((tmp->isShown())&&(tmp!=title))
		{
//			tmp->adjustPositionAndSize(ADJUST_ONLY_POSITION);
			if(maxWidth < tmp->getRelativeRightBound())
				maxWidth = tmp->getRelativeRightBound();
			if(maxHeight < tmp->getRelativeLowerBound())
				maxHeight = tmp->getRelativeLowerBound();
		}
		tmp = tmp->getNextBrother();
	} while(tmp!=getChildren());
	Size s = Size(maxWidth + 10, maxHeight + 10);

//	adjustRelativeRect(Rect(getTargetPosition(), s));
	adjustPositionAndSize(ADJUST_AFTER_CHILD_SIZE_WAS_CHANGED, s);//Size(maxWidth + 5, getChildren()->getPrevBrother()->getAbsoluteLowerBound() - getChildren()->getAbsoluteUpperBound()));
}

void UI_Group::draw(DC* dc) const
{
	if(checkForNeedRedraw())
	{
		if(highlighted)
			dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
		else	
			dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
		if(title!=NULL)
		{
			dc->SetBrush(*theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH));
			dc->DrawEdgedRoundedRectangle(getAbsolutePosition() - Size(4, 6), getSize(), 4);
		
			Size s = title->getTextSize();
		  	Rect titleRect = Rect(getAbsolutePosition() - Size(5, 15), s + Size(5,2));
			dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
			dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
			dc->DrawEdgedRoundedRectangle(titleRect, 2);
		}
		else
		{
			dc->SetBrush(*theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH));
			dc->DrawEdgedRoundedRectangle(getAbsolutePosition()-Size(3,0), getSize()+Size(6,0), 4);
		}
	}
	UI_Object::draw(dc);
}

void UI_Group::process()
{
//	if(!isShown())
//		return; //?
	UI_Object::process();

	if(getAbsoluteRect().Inside(mouse))
	{
		if(!highlighted)
		{
			highlighted = true;
			setNeedRedrawNotMoved();
		}
	} else
		if(highlighted)
		{
			setNeedRedrawNotMoved();
			highlighted = false;
		}
	
/*	UI_Object* tmp = getChildren();
	if(!tmp)
		return;
	do
	{
		if(tmp->checkForNeedRedraw())
		{
			setNeedRedrawMoved(); //?
			return;
		}
		tmp = tmp->getNextBrother();
	} while(tmp!=getChildren());*/	
}


UI_Object* UI_Group::checkToolTip()
{
//	if(!(Rect(getAbsolutePosition(), boxSize).Inside(mouse)))
//		return(0);
	return(UI_Object::checkToolTip());
}

UI_Object* UI_Group::checkHighlight()
{
//	if(!(Rect(getAbsolutePosition(), boxSize).Inside(mouse)))
//		return(0);
	return(UI_Object::checkHighlight());
}


