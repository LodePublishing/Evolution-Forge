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
	barHeight = object.barHeight;
	maxScrollY = object.maxScrollY;
	startY = object.startY;
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
	barHeight(object.barHeight),
	maxScrollY(object.maxScrollY),
	startY(object.startY)
{ }

UI_Scrollbar::UI_Scrollbar(UI_Object* scroll_parent, /*const Rect& clientRect TODO */ const unsigned int start_y, const unsigned int max_height, const bool scroll_hideable) :
	UI_Object(scroll_parent, Rect(0, 0, scroll_parent->getWidth(), scroll_parent->getHeight())), // TODO
	add(new UI_Button(this, Rect(getParent()->getWidth()+2, start_y, 8, 8), SMALL_ARROW_DOWN_BUTTON, PRESS_BUTTON_MODE)),
	sub(new UI_Button(this, Rect(getParent()->getWidth()+2, 4, 8, 8), SMALL_ARROW_UP_BUTTON, PRESS_BUTTON_MODE)),
	hideable(scroll_hideable),
	internalScrollY(0),
	internalHeight(0),
	currentScrollY(0),
	barHeight(0),
	maxScrollY(0),
	startY(start_y)
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

void UI_Scrollbar::moveUp()
{
	currentScrollY-=(getParent()->getHeight() - 30)/20;
}

void UI_Scrollbar::moveDown()
{
	currentScrollY+=(getParent()->getHeight() - 30)/20;
}

void UI_Scrollbar::process() // process messages, continue animation etc.
{
	if(!isShown())
		return;
	UI_Object::process();
	
	unsigned int windowHeight = getParent()->getHeight() - startY - 28;
	unsigned int windowTargetHeight = getParent()->getTargetHeight() - startY - 28;
	
	if(getParent()->lastItemY == getParent()->firstItemY)
		barHeight = windowHeight;
	else 
		barHeight = windowHeight * windowHeight / (getParent()->lastItemY - getParent()->firstItemY);
	
	if(barHeight > windowHeight)
		barHeight = windowHeight;
		// TODO
	if(add->isLeftClicked())
		moveDown();
	if(sub->isLeftClicked())
		moveUp();
		
	if(currentScrollY < 0)
		currentScrollY = 0;
	
	if(currentScrollY > windowHeight - barHeight)
		currentScrollY = windowHeight - barHeight;

		
//	if(barHeight > 10) barHeight = 10;
	
//	unsigned int maxHeight = getMaxRect().GetHeight();

	setHeight(windowHeight);

	setPosition(Point(0, -currentScrollY)); // WAS jumpTo

	add->setPosition(Point(getParent()->getWidth() + 2, startY + windowHeight + currentScrollY + 16));
	sub->setPosition(Point(getParent()->getWidth() + 2, startY + currentScrollY));

//	internalScrollY ...
#if 0
//	if(false)//maxHeight>0)
	{
		if(maxScrollY/*+windowHeight*/ > maxHeight)
		{
			barHeight = (maxHeight*windowHeight)/(maxScrollY/*+windowHeight*/);// maxScrollY*(windowHeight - 16) / (maxHeight + windowHeight);
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
			barHeight = windowHeight-16;
			currentScrollY = 0;
			internalScrollY = 0;
		}
	}
/*	else
	{
		currentScrollY = 0;
		internalScrollY = 0;

		barHeight = windowHeight;
		internalHeight = MAX_INTERNAL_SCROLLY;
	}*/
#endif
}

#include <sstream>
void UI_Scrollbar::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Object::draw(dc);
//	std::ostringstream os;
//	os << getParent()->firstItemY << " : " << getParent()->lastItemY << " / " << getParent()->getHeight() << " : " << barHeight;
//	os << maxScrollY << ":" << internalScrollY;
//	dc->SetTextForeground(DC::toSDL_Color(255, 100, 100));
//	dc->SetFont(UI_Object::theme.lookUpFont(LARGE_NORMAL_BOLD_FONT));
//	dc->DrawText(os.str(), 500, 500);


	dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
	if(Rect(getParent()->getAbsolutePosition() + Point(getWidth(), 4), Size(12, getHeight()+12)).Inside(mouse))
		dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	else
		dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
		
	dc->DrawRectangle(Rect(getParent()->getAbsolutePosition() + Size(getParent()->getWidth()+1, startY + 12), Size(10, getHeight())));
	
	dc->SetBrush(*theme.lookUpBrush(CONTINUE_BUTTON_BRUSH));
	dc->DrawRectangle(Rect(getParent()->getAbsolutePosition() + Point(getParent()->getWidth()+2, currentScrollY + startY + 12), Size(8, barHeight)));

//	dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
//	dc->DrawRoundedRectangle(getAbsolutePosition() + Point(1, currentScrollY + 16 - (small * 8)), Size(14 - (small * 8), barHeight), 2);
}

const signed int UI_Scrollbar::getScrollY() const {
	return(currentScrollY);
}


