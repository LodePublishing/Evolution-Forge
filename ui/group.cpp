#include "group.hpp"
#include "button.hpp"

UI_Group::UI_Group(UI_Object* group_parent, const Rect initial_rect, const Size bottom_right_distance, const ePositionMode position_mode, const eString txt ) :
	UI_Object(group_parent, initial_rect, bottom_right_distance, position_mode, AUTO_SIZE),
	title(txt==NULL_STRING?NULL:new UI_StaticText(this, txt, Rect(Point(0,0) - Size(3, 14), Size(0,0)), Size(0,0), BRIGHT_TEXT_COLOR, SMALL_BOLD_FONT, DO_NOT_ADJUST, AUTO_SIZE)),
	number(0)
{ }

UI_Group::~UI_Group() {
	delete title;
}

UI_Group::UI_Group(const UI_Group& object) :
	UI_Object((UI_Object)object),
	title(object.title==NULL?NULL:new UI_StaticText(*object.title)),
	number(object.number)
{ }

UI_Group& UI_Group::operator=(const UI_Group& object)
{
	(UI_Object)(*this) = (UI_Object)object;
	delete title;
	if(object.title)
		title = new UI_StaticText(*object.title);
	number = object.number;
	return(*this);
}

void UI_Group::reloadStrings() {
	UI_Object::reloadStrings();
}
#include <sstream>
void UI_Group::calculateBoxSize(const bool horizontal)
{
	UI_Object* tmp = getChildren();
	if(!tmp)
		return;
	number = 0;
	unsigned int maxWidth = 0;
	unsigned int maxHeight = 0;
	do
	{
		if((maxWidth < tmp->getWidth())&&(tmp!=title))
		{
			maxWidth = tmp->getWidth();
			number++;
			if(maxHeight < tmp->getHeight())
				maxHeight = tmp->getHeight();
		} else 
		if((maxHeight < tmp->getHeight())&&(tmp!=title))
		{
			maxHeight = tmp->getHeight();
			number++;
		} else if(tmp!=title)
			number++;
		tmp = tmp->getNextBrother();
	} while(tmp!=getChildren());
	Size s;
	if(horizontal)
		s = Size((maxWidth + 5) * number, maxHeight+13);
	else
		s =  Size(maxWidth + 5, getChildren()->getPrevBrother()->getAbsoluteLowerBound() - getChildren()->getAbsoluteUpperBound());

	adjustRelativeRect(Rect(getRelativePosition(), s));
//	adjustPositionAndSize(ADJUST_AFTER_CHILD_SIZE_WAS_CHANGED, Size(maxWidth + 5, getChildren()->getPrevBrother()->getAbsoluteLowerBound() - getChildren()->getAbsoluteUpperBound()));
}

void UI_Group::draw(DC* dc) const
{
	if((checkForNeedRedraw())&&(number>=1))
	{
		if(getAbsoluteRect().Inside(mouse))
			dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
		else	
			dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
		if(title!=NULL)
		{
			title->setColor(BRIGHT_TEXT_COLOR);
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
			dc->DrawEdgedRoundedRectangle(getAbsolutePosition() - Size(4, 6), getSize(), 4);
		}
	}
	UI_Object::draw(dc);
}

void UI_Group::process()
{
//	if(!isShown())
//		return; //?
	UI_Object::process();
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


