#include "scrollbar.hpp"

#define MAX_INTERNAL_SCROLLY 10000

UI_Scrollbar& UI_Scrollbar::operator=(const UI_Scrollbar& object)
{
	((UI_Object)(*this)) = ((UI_Object)object);
    add = new UI_Button(*(object.add));
    sub = new UI_Button(*(object.sub));
    horizontal = object.horizontal;
    hideable = object.hideable;
    small = object.small;
    discrete = object.discrete;
    internalScrollY = object.internalScrollY;
    internalHeight = object.internalHeight;
    currentScrollY = object.currentScrollY;
    currentHeight = object.currentHeight;
	return(*this);
}

UI_Scrollbar::UI_Scrollbar(const UI_Scrollbar& object) :
    UI_Object((UI_Object)object),
    add(new UI_Button(*(object.add))),
    sub(new UI_Button(*(object.sub))),
    horizontal(object.horizontal),
    hideable(object.hideable),
    small(object.small),
    discrete(object.discrete),
    internalScrollY(object.internalScrollY),
    internalHeight(object.internalHeight),
    currentScrollY(object.currentScrollY),
    currentHeight(object.currentHeight)
{ }

UI_Scrollbar::UI_Scrollbar(UI_Object* scroll_parent, Rect scroll_rect, Rect scroll_max_rect, const bool scroll_horizontal, const signed int scroll_discrete, const bool scroll_hideable, const bool scroll_small):
	UI_Object(scroll_parent, scroll_rect, scroll_max_rect),
	add(0),
	sub(0),
	horizontal(scroll_horizontal),
	hideable(scroll_hideable),
	small(scroll_small),
	discrete(scroll_discrete),
	internalScrollY(0),
	internalHeight(0),
	currentScrollY(0),
	currentHeight(0)
{
	eButton but;
	Rect r, mr;
	if(horizontal)
	{
		if(small) but = SMALL_ARROW_LEFT_BUTTON;
		else but = ARROW_LEFT_BUTTON;
		r = Rect(Point(1, 0), Size(16 - (small * 8), 16 - (small * 8)));
		mr = Rect(Point(1, 0), Size(16 - (small * 8), 16 - (small * 8)));
	} else
	{
		if(small) but = SMALL_ARROW_UP_BUTTON;
		else but = ARROW_UP_BUTTON;
//		r = Rect(Point(0, getHeight()-18), Size(16 - (small * 8), 16 - (small * 8)));
//		mr = Rect(Point(0, getMaxRect().GetHeight()-18), Size(16 - (small * 8), 16 - (small * 8)));
		r = Rect(Point(0, 0), Size(16 - (small * 8), 16 - (small * 8)));
		mr = Rect(Point(0, 0), Size(16 - (small * 8), getMaxRect().GetHeight()-18 + (small * 8)));
	}
	
/*	sub = new UI_Button(this, r, mr, but, PRESS_BUTTON_MODE);
	add = new UI_Button(this, r, mr, (eButton)(but+1), PRESS_BUTTON_MODE);*/
}

UI_Scrollbar::~UI_Scrollbar()
{
/*	delete add;
	delete sub;*/
}

void UI_Scrollbar::process() // process messages, continue animation etc.
{
//	if(!isShown())
		return;
	UI_Object::process();
	
	int windowHeight = getHeight() - 32 + (small * 16);
	// TODO windowWidth
	int maxHeight = getMaxRect().GetHeight() + getHeight() - windowHeight;

	if(sub->isLeftClicked()&&(internalScrollY > MAX_INTERNAL_SCROLLY/10))
		internalScrollY-=MAX_INTERNAL_SCROLLY/10;
	else internalScrollY = 0;

	if(add->isLeftClicked()&&(internalScrollY + MAX_INTERNAL_SCROLLY/10 < MAX_INTERNAL_SCROLLY))
		internalScrollY+=MAX_INTERNAL_SCROLLY/10;
	else internalScrollY = MAX_INTERNAL_SCROLLY;

//	if(horizontal)
//		add->adjustRelativeRect(Rect(Point(-18, 0), Size(16 - (small * 8), 16 - (small * 8))));
//	else
		add->adjustRelativeRect(Rect(0, windowHeight + 16 - (small * 8), 16 - (small * 8), 16 - (small * 8)));

//	internalScrollY ...

	if(false)//maxHeight>0)
	{
		currentScrollY = internalScrollY * maxHeight / MAX_INTERNAL_SCROLLY;
		currentHeight = windowHeight / (maxHeight + windowHeight);
	}
	else
	{
		currentScrollY = 0;
		internalScrollY = 0;

		currentHeight = windowHeight;
		internalHeight = MAX_INTERNAL_SCROLLY;
	}
}

void UI_Scrollbar::draw(DC* dc) const
{
//	if(!isShown())
		return;
	UI_Object::draw(dc);

    dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
    dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));

	dc->DrawRectangle(getAbsolutePosition() + Point(0, currentScrollY + 16 - (small * 8)), Size(16 - (small * 8), getHeight() - 16 + (small * 8)));

	dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
	dc->DrawRoundedRectangle(getAbsolutePosition() + Point(1, currentScrollY + 16 - (small * 8)), Size(14 - (small * 8), currentHeight), 2);
}

const signed int UI_Scrollbar::getScrollY() const
{
	return(currentScrollY);
}


