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
	clientHeight = object.clientHeight;
	clientTargetHeight = object.clientTargetHeight;
	totalHeight = object.totalHeight;
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
	clientHeight(object.clientHeight),
	clientTargetHeight(object.clientTargetHeight),
	totalHeight(object.totalHeight)
{ }

UI_Scrollbar::UI_Scrollbar(UI_Object* scroll_parent, /*const Rect& clientRect TODO */ const unsigned int start_y, const unsigned int max_height, const bool scroll_hideable) :
	UI_Object(scroll_parent, Rect(0, 0, scroll_parent->getWidth(), scroll_parent->getHeight())), // TODO
	add(new UI_Button(this, Rect(getParent()->getWidth()-3, start_y, 8, 8), SMALL_ARROW_DOWN_BUTTON, PRESS_BUTTON_MODE)),
	sub(new UI_Button(this, Rect(getParent()->getWidth()-3, 4, 8, 8), SMALL_ARROW_UP_BUTTON, PRESS_BUTTON_MODE)),
	hideable(scroll_hideable),
	internalScrollY(0),
	internalHeight(0),
	currentScrollY(0),
	barHeight(0),
	clientHeight(0),
	clientTargetHeight(0),
	totalHeight(0)
{ }

UI_Scrollbar::~UI_Scrollbar()
{
	delete add;
	delete sub;
}

void UI_Scrollbar::setClientHeight(const unsigned int height)
{
	clientHeight = height;
}

void UI_Scrollbar::setClientTargetHeight(const unsigned int height)
{
	clientTargetHeight = height;
}

void UI_Scrollbar::moveUp()
{
	currentScrollY-=clientHeight/20;
}

void UI_Scrollbar::moveDown()
{
	currentScrollY+=clientHeight/20;
}

void UI_Scrollbar::moveToTop()
{
	currentScrollY = 0;
	setNeedRedrawMoved();
}

void UI_Scrollbar::moveToBottom()
{
	currentScrollY = 99999;
	setNeedRedrawMoved();
	process();
}

void UI_Scrollbar::reloadStrings()
{
	UI_Object::reloadStrings();
}


void UI_Scrollbar::process() // process messages, continue animation etc.
{
	if(!isShown())
		return;
	UI_Object::process();

	totalHeight = getParent()->lastItemY - getParent()->firstItemY; //???

	unsigned int oldBarHeight = barHeight;
	unsigned int oldScrollY = currentScrollY;

	if(getParent()->lastItemY == getParent()->firstItemY)
		barHeight = clientHeight;
	else 
		barHeight = clientHeight * clientHeight / totalHeight;
	
	if(barHeight > clientHeight)
		barHeight = clientHeight;
	if(barHeight != oldBarHeight)
		setNeedRedrawMoved();
		
	if(add->isLeftClicked())
		moveDown();
	if(sub->isLeftClicked())
		moveUp();
		
	if(currentScrollY < 0)
		currentScrollY = 0;
	
	if(currentScrollY > clientHeight - barHeight)
		currentScrollY = clientHeight - barHeight;
	if(currentScrollY != oldScrollY)
		setNeedRedrawMoved();

	setHeight(clientHeight);

	if(clientHeight <= 0)
		setPosition(Point(0, 0));
	else
		setPosition(Point(0, -currentScrollY * totalHeight / clientHeight));

	add->setPosition(Point(getParent()->getWidth() -3, clientHeight + currentScrollY + 16));
	sub->setPosition(Point(getParent()->getWidth() -3, currentScrollY));
}

void UI_Scrollbar::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Object::draw(dc);

	if(checkForNeedRedraw())
	{

		dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
		if(Rect(getParent()->getAbsolutePosition() + Point(getWidth(), 4) - Size(5,0), Size(12, getHeight()+12)).Inside(mouse))
			dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
		else
			dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
			
		dc->DrawRectangle(Rect(getParent()->getAbsolutePosition() + Size(getParent()->getWidth()-4, 12), Size(10, getHeight())));
	
		dc->SetBrush(*theme.lookUpBrush(CONTINUE_BUTTON_BRUSH));
		dc->DrawRectangle(Rect(getParent()->getAbsolutePosition() + Point(getParent()->getWidth()-3, currentScrollY +  12), Size(8, barHeight)));
	}
}

