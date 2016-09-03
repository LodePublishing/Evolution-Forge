#include "scrollbar.hpp"

#define MAX_INTERNAL_SCROLLY 10000

UI_Scrollbar& UI_Scrollbar::operator=(const UI_Scrollbar& object)
{
	((UI_Object)(*this)) = ((UI_Object)object);
	delete add;
    add = new UI_Button(*(object.add));
	delete sub;
    sub = new UI_Button(*(object.sub));
    hideable = object.hideable;
    internalScrollY = object.internalScrollY;
    internalHeight = object.internalHeight;
    currentScrollY = object.currentScrollY;
    currentHeight = object.currentHeight;
	maxScrollY = object.maxScrollY;
	return(*this);
}

UI_Scrollbar::UI_Scrollbar(const UI_Scrollbar& object) :
    UI_Object((UI_Object)object),
    add(new UI_Button(*(object.add))),
    sub(new UI_Button(*(object.sub))),
    hideable(object.hideable),
    internalScrollY(object.internalScrollY),
    internalHeight(object.internalHeight),
    currentScrollY(object.currentScrollY),
    currentHeight(object.currentHeight),
	maxScrollY(object.maxScrollY)
{ }

UI_Scrollbar::UI_Scrollbar(UI_Object* scroll_parent, const Point topRightCorner, const unsigned int max_height, const bool scroll_hideable) :
	UI_Object(scroll_parent, 
			Rect(topRightCorner - Size(11, 0), Size(9, scroll_parent->getHeight())),
			Rect(topRightCorner - Size(11, 0), Size(9, max_height))),
	add(new UI_Button(this, Rect(2, 0, 8, 8), Rect(1, 0, 8, max_height), SMALL_ARROW_DOWN_BUTTON, PRESS_BUTTON_MODE)),
	sub(new UI_Button(this, Rect(2, 0, 8, 8), Rect(1, max_height, 8, max_height), SMALL_ARROW_UP_BUTTON, PRESS_BUTTON_MODE)),
	hideable(scroll_hideable),
	internalScrollY(0),
	internalHeight(0),
	currentScrollY(0),
	currentHeight(0),
	maxScrollY(0)
{ }

UI_Scrollbar::~UI_Scrollbar()
{
	delete add;
	delete sub;
}

void UI_Scrollbar::setMaxScrollY(const unsigned int max_scrolly) {
	if(!isShown())
		return;
	maxScrollY = max_scrolly - getScrollY();
}

void UI_Scrollbar::process() // process messages, continue animation etc.
{
	if(!isShown())
		return;
	UI_Object::process();
	
	unsigned int windowHeight = getParent()->getHeight() - 16;
	// TODO windowWidth
	unsigned int maxHeight = getMaxRect().GetHeight();

	setHeight(windowHeight);

	add->jumpToPosition(Point(2, windowHeight + 4));

//	internalScrollY ...

//	if(false)//maxHeight>0)
	{
		if(maxScrollY/*+windowHeight*/ > maxHeight)
		{
			currentHeight = (maxHeight*windowHeight)/(maxScrollY/*+windowHeight*/);// maxScrollY*(windowHeight - 16) / (maxHeight + windowHeight);
			currentScrollY = internalScrollY * maxHeight / MAX_INTERNAL_SCROLLY;
			if(sub->isLeftClicked())
			{
				if(internalScrollY > MAX_INTERNAL_SCROLLY/10)
					internalScrollY-=MAX_INTERNAL_SCROLLY/10;
				else internalScrollY = 0;
			}
			if(add->isLeftClicked())
			{
				if(internalScrollY + MAX_INTERNAL_SCROLLY/10 < MAX_INTERNAL_SCROLLY)
					internalScrollY+=MAX_INTERNAL_SCROLLY/10;
				else internalScrollY = MAX_INTERNAL_SCROLLY;
			}
		}
		else
		{
			currentHeight = windowHeight-16;
			currentScrollY = 0;
			internalScrollY = 0;
		}
	}
/*	else
	{
		currentScrollY = 0;
		internalScrollY = 0;

		currentHeight = windowHeight;
		internalHeight = MAX_INTERNAL_SCROLLY;
	}*/
}
#include <sstream>
void UI_Scrollbar::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Object::draw(dc);

	std::ostringstream os;
	os << maxScrollY << ":" << internalScrollY;
	dc->SetTextForeground(toSDL_Color(255, 100, 100));
	dc->SetFont(UI_Object::theme.lookUpFont(LARGE_NORMAL_BOLD_FONT));
	dc->DrawText(os.str(), 500, 500);


	dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
	if(isMouseInside())
		dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	else
		dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
	dc->DrawRectangle(Rect(getAbsolutePosition() + Size(1, 10), Size(9, getHeight()-8)));
	
	dc->SetBrush(*theme.lookUpBrush(BRIGHT_UNIT_TYPE_1_BRUSH));
	dc->DrawRoundedRectangle(Rect(getAbsolutePosition() + Point(1, currentScrollY + 10), Size(8, currentHeight)), 2);

//	dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
//	dc->DrawRoundedRectangle(getAbsolutePosition() + Point(1, currentScrollY + 16 - (small * 8)), Size(14 - (small * 8), currentHeight), 2);
}

const signed int UI_Scrollbar::getScrollY() const
{
	return(currentScrollY);
}


