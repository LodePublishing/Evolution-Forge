#include "scrollbar.hpp"

#define MAX_INTERNAL_SCROLLY 10000

UI_Scrollbar& UI_Scrollbar::operator=(const UI_Scrollbar& object)
{
	((UI_Object)(*this)) = ((UI_Object)object);
	firstItemY = object.firstItemY;
	lastItemY = object.lastItemY;
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
	maxHeight = object.maxHeight;
	return(*this);
}

UI_Scrollbar::UI_Scrollbar(const UI_Scrollbar& object) :
	UI_Object((UI_Object)object),
	firstItemY(object.firstItemY),
	lastItemY(object.lastItemY),
	add(new UI_Button(*(object.add))),
	sub(new UI_Button(*(object.sub))),
	hideable(object.hideable),
	internalScrollY(object.internalScrollY),
	internalHeight(object.internalHeight),
	currentScrollY(object.currentScrollY),
	barHeight(object.barHeight),
	clientHeight(object.clientHeight),
	clientTargetHeight(object.clientTargetHeight),
	totalHeight(object.totalHeight),
	maxHeight(object.maxHeight)
{ }

UI_Scrollbar::UI_Scrollbar(UI_Object* scroll_parent, /*const Rect& clientRect TODO */ const unsigned int start_y, const unsigned int max_height, const bool scroll_hideable) :
	UI_Object(scroll_parent, Rect(0, 0, scroll_parent->getWidth(), scroll_parent->getHeight())), // TODO
	firstItemY(0),
	lastItemY(0),
	add(new UI_Button(this, Rect(getParent()->getWidth()-8, start_y, 8, 8), Size(0,0), SMALL_ARROW_DOWN_BUTTON, PRESS_BUTTON_MODE)),
	sub(new UI_Button(this, Rect(getParent()->getWidth()-8, 4, 8, 8), Size(0,0), SMALL_ARROW_UP_BUTTON, PRESS_BUTTON_MODE)),
	hideable(scroll_hideable),
	internalScrollY(0),
	internalHeight(0),
	currentScrollY(0),
	barHeight(0),
	clientHeight(0),
	clientTargetHeight(0),
	totalHeight(0),
	maxHeight(max_height)
{ }

UI_Scrollbar::~UI_Scrollbar()
{
	delete add;
	delete sub;
}

void UI_Scrollbar::setClientHeight(const unsigned int height)
{
	clientHeight = height-12;
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
//	setNeedRedrawMoved();
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

	totalHeight = lastItemY - firstItemY + clientHeight; //???

// totalHeight wird 0...

	unsigned int oldBarHeight = barHeight;
	signed int oldScrollY = currentScrollY;

	if(lastItemY == firstItemY)
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
	
	if(currentScrollY > (signed int)clientHeight - barHeight)
		currentScrollY = (signed int)clientHeight - barHeight;
	if(currentScrollY != oldScrollY)
		setNeedRedrawMoved();

//	setHeight(clientHeight);

	signed int position = 0;
	if(clientHeight <= 0)
		position = 0;
	else 
		position = currentScrollY * (signed int)(totalHeight / clientHeight);
	setPosition(Point(0, -position));
	add->setPosition(Point(getParent()->getWidth() -8, clientHeight + position + 20));
	sub->setPosition(Point(getParent()->getWidth() -8, position + 6));

	if(Rect(getParent()->getAbsolutePosition(), getParent()->getSize() + Size(8, 0)).Inside(UI_Object::mouse))
	{
		add->Show();
		sub->Show();
	} else
	{
		add->Hide();
		sub->Hide();
	}
}


#include <sstream>
void UI_Scrollbar::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Object::draw(dc);

	if(checkForNeedRedraw())
	{

		dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
		if(Rect(getParent()->getAbsolutePosition() + Point(getWidth(), 10) - Size(10,0), Size(12, getHeight()+12)).Inside(mouse))
			dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
		else
			dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
			
		dc->DrawRectangle(Rect(getParent()->getAbsolutePosition() + Size(getParent()->getWidth()-9, 18), Size(10, clientHeight)));
	
		dc->SetBrush(*theme.lookUpBrush(CONTINUE_BUTTON_BRUSH));
		dc->DrawRectangle(Rect(getParent()->getAbsolutePosition() + Point(getParent()->getWidth()-8, currentScrollY +  18), Size(8, barHeight)));
	}
		
	/*Point p = Point(100, 300);
	dc->SetTextForeground(DC::toSDL_Color(255, 20, 20));
	dc->SetFont(UI_Object::theme.lookUpFont(LARGE_BOLD_FONT));
	dc->SetBrush(Brush(Color(dc->GetSurface(), 0, 0, 0), SOLID_BRUSH_STYLE));
	dc->DrawRectangle(Rect(p, Size(200, 20)));

	std::ostringstream os;
	os.str("");
	os << "cH : " << clientHeight << " / T : " << totalHeight << " / cSY : " << currentScrollY << "  [" << firstItemY << ":" << lastItemY << "]";
	dc->DrawText(os.str(), p + Size(20, 2));	*/
}

