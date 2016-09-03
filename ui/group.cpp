#include "group.hpp"
#include "button.hpp"

UI_Group::UI_Group(UI_Object* group_parent, Point position, const eString txt) :
	UI_Object(group_parent, Rect(position, Size(0,0))),
	title(txt==NULL_STRING?NULL:new UI_StaticText(this, txt, Rect(Point(0,0) - Size(3, 19), Size(0,0)), BRIGHT_TEXT_COLOR, SMALL_ITALICS_BOLD_FONT, NO_TEXT_MODE)),
	boxSize(),
	maxWidth(0)
{ }

UI_Group::~UI_Group() {
	delete title;
}

UI_Group::UI_Group(const UI_Group& object) :
	UI_Object((UI_Object)object),
	title(object.title==NULL?NULL:new UI_StaticText(*object.title)),
	boxSize(object.boxSize),		
	maxWidth(object.maxWidth)
{ }

UI_Group& UI_Group::operator=(const UI_Group& object)
{
	(UI_Object)(*this) = (UI_Object)object;
	delete title;
	if(object.title)
		title = new UI_StaticText(*object.title);
	boxSize = object.boxSize;
	maxWidth = object.maxWidth;
	return(*this);
}

void UI_Group::reloadStrings() {
	UI_Object::reloadStrings();
}

void UI_Group::calculateBoxSize(const bool horizontal)
{
	if(!getChildren())
		return;
	UI_Object* tmp = getChildren();
	maxWidth=0;
	int number=0;
	do
	{
		if(maxWidth < tmp->getWidth())
			maxWidth = tmp->getWidth();
		tmp=tmp->getNextBrother();
		number++;
	} while(tmp!=getChildren());
	maxWidth+=3;
	if(horizontal)
		boxSize = Size(maxWidth*number, getChildren()->getHeight());
	else
	{
		//boxSize = Size(maxWidth, 13+number*(getChildren()->getHeight()));
		boxSize = Size(maxWidth, getChildren()->getPrevBrother()->getAbsoluteLowerBound() - getChildren()->getAbsoluteUpperBound() + 13);
	}
}

void UI_Group::calculateSameWidthOfButtons(const bool horizontal)
{
	if(!getChildren())
		return;
	UI_Object* tmp=getChildren();
	calculateBoxSize(horizontal);
	do
	{
		tmp->setWidth(maxWidth);
		tmp->adjustButtonPlacementSize();
		tmp=tmp->getNextBrother();
	} while(tmp!=getChildren());
}

void UI_Group::draw(DC* dc) const
{
	if((Rect(getAbsolutePosition(), boxSize).Inside(mouse)))
		dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	else
		dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
	if(title!=NULL)
	{
		title->setColor(BRIGHT_TEXT_COLOR);
		dc->SetBrush(*theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH));
		dc->DrawEdgedRoundedRectangle(getAbsolutePosition() - Size(5, 13), boxSize + Size(12, 0) - Size(0, 10), 4);
		
		Size s = title->getBoxSize();
	  	Rect titleRect = Rect(getAbsolutePosition() - Size(5, 20), s + Size(5,2));
		dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
		dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
		dc->DrawEdgedRoundedRectangle(titleRect, 2);
	}
	else
	{
		dc->SetBrush(*theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH));
		dc->DrawEdgedRoundedRectangle(getAbsolutePosition() - Size(3, 3), boxSize + Size(12, 6), 4);
	}
	UI_Object::draw(dc);
}

void UI_Group::process()
{
	if(!isShown())
		return;
	UI_Object::process();
}


UI_Object* UI_Group::checkTooltip()
{
//	if(!(Rect(getAbsolutePosition(), boxSize).Inside(mouse)))
//		return(0);
	return(UI_Object::checkTooltip());
}

UI_Object* UI_Group::checkHighlight()
{
//	if(!(Rect(getAbsolutePosition(), boxSize).Inside(mouse)))
//		return(0);
	return(UI_Object::checkHighlight());
}


