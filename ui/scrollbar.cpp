#include "scrollbar.hpp"

#define MAX_INTERNAL_SCROLLY 10000

UI_ScrollBar& UI_ScrollBar::operator=(const UI_ScrollBar& object)
{
	((UI_Object)(*this)) = ((UI_Object)object);
	firstItemY = object.firstItemY;
	lastItemY = object.lastItemY;
	startY = object.startY;
	delete add;
	add = new UI_Button(*(object.add));
	delete sub;
	sub = new UI_Button(*(object.sub));
	hideable = object.hideable;
	internalScrollY = object.internalScrollY;
	internalHeight = object.internalHeight;
	currentScrollY = object.currentScrollY;
	targetScrollY = object.targetScrollY;
	barHeight = object.barHeight;
	clientHeight = object.clientHeight;
	clientTargetHeight = object.clientTargetHeight;
	totalHeight = object.totalHeight;
	maxHeight = object.maxHeight;
	return(*this);
}

UI_ScrollBar::UI_ScrollBar(const UI_ScrollBar& object) :
	UI_Object((UI_Object)object),
	firstItemY(object.firstItemY),
	lastItemY(object.lastItemY),
	startY(object.startY),
	add(new UI_Button(*(object.add))),
	sub(new UI_Button(*(object.sub))),
	hideable(object.hideable),
	internalScrollY(object.internalScrollY),
	internalHeight(object.internalHeight),
	currentScrollY(object.currentScrollY),
	targetScrollY(object.targetScrollY),
	barHeight(object.barHeight),
	clientHeight(object.clientHeight),
	clientTargetHeight(object.clientTargetHeight),
	totalHeight(object.totalHeight),
	maxHeight(object.maxHeight)
{ }

UI_ScrollBar::UI_ScrollBar(UI_Object* scroll_parent, const unsigned int start_y, const unsigned int max_height, const bool scroll_hideable) :
	UI_Object(scroll_parent, Rect()),//scroll_parent->getWidth(), scroll_parent->getHeight())), // TODO
	firstItemY(0),
	lastItemY(0),
	startY(start_y),
	add(new UI_Button(this, Rect(getParent()->getWidth()-10, 0, 8, 8), Size(0,0), SMALL_ARROW_DOWN_BUTTON, true, PRESS_BUTTON_MODE, NULL_STRING)),
	sub(new UI_Button(this, Rect(getParent()->getWidth()-10, 0, 8, 8), Size(0,0), SMALL_ARROW_UP_BUTTON, true, PRESS_BUTTON_MODE, NULL_STRING)),
	hideable(scroll_hideable),
	internalScrollY(0),
	internalHeight(0),
	currentScrollY(0),
	targetScrollY(0),
	barHeight(0),
	clientHeight(0),
	clientTargetHeight(0),
	totalHeight(0),
	maxHeight(max_height)
{ 
}

UI_ScrollBar::~UI_ScrollBar()
{
	delete add;
	delete sub;
}

void UI_ScrollBar::setClientHeight(const unsigned int height)
{
	clientHeight = height-12;
}

void UI_ScrollBar::setClientTargetHeight(const unsigned int height)
{
	clientTargetHeight = height;
}

void UI_ScrollBar::moveUp()
{
	targetScrollY-=clientHeight/20;
}

void UI_ScrollBar::moveDown()
{
	targetScrollY+=clientHeight/20;
}

// TODO: Wenn Scrollbereich ausserhalb des Fensters rutscht moechte das Programm verstaendlicherweise neumalen, da es nicht weiss, dass die entsprechenden Eintraege versteckt werden... :-/
void UI_ScrollBar::moveToTop()
{
	if(currentScrollY==0)
		return;
	targetScrollY = 0;
}

void UI_ScrollBar::moveToBottom()
{
	targetScrollY = 99999;
}

void UI_ScrollBar::checkBoundsOfChildren(const signed int upper_bound, const signed int lower_bound)
{
	UI_Object* tmp = getChildren();
	if (tmp) {
		do {
			if((tmp!=add)&&(tmp!=sub))
			{
				if(((tmp->getAbsoluteUpperBound() < upper_bound) || (tmp->getAbsoluteLowerBound() > lower_bound)))
					tmp->Hide();
				else tmp->Show();
			}
			tmp = tmp->getNextBrother();
		} while (tmp != getChildren());
	}
}

void UI_ScrollBar::reloadStrings()
{
	UI_Object::reloadStrings();
}


void UI_ScrollBar::process() // process messages, continue animation etc.
{
	if(!isShown())
		return;

	
	totalHeight = lastItemY - firstItemY + clientHeight; //???

// totalHeight wird 0...

	unsigned int oldBarHeight = barHeight;

	if(lastItemY == firstItemY)
		barHeight = clientHeight;
	else 
		barHeight = clientHeight * clientHeight / totalHeight;
	
	if(barHeight > clientHeight)
		barHeight = clientHeight;
	
	signed int oldScrollY = currentScrollY;
	currentScrollY = targetScrollY;
	if(currentScrollY < 0)
		currentScrollY = 0;
	if(currentScrollY > (signed int)clientHeight - barHeight)
		currentScrollY = (signed int)clientHeight - barHeight;
	


	signed int position = 0;
	if(clientHeight <= 0)
		position = 0;
	else 
		position = currentScrollY * (signed int)(totalHeight / clientHeight);
	setPosition(Point(0, startY-position));
	add->setPosition(Point(getParent()->getWidth() -12, clientHeight + position + 6));
	sub->setPosition(Point(getParent()->getWidth() -12, position - 4));

	if(Rect(getParent()->getAbsolutePosition(), getParent()->getSize() + Size(8, 0)).Inside(UI_Object::mouse))
	{
		add->Show();
		sub->Show();
	} else
	{
		add->Hide();
		sub->Hide();
	}
	targetScrollY = currentScrollY;
	
	UI_Object::process();

	if((currentScrollY != oldScrollY)||(barHeight != oldBarHeight)||(add->checkForNeedRedraw())||(sub->checkForNeedRedraw()))
		setNeedRedrawMoved();

	if(add->isLeftClicked())
		moveDown();
	if(sub->isLeftClicked())
		moveUp();

}


#include <sstream>
void UI_ScrollBar::draw(DC* dc) const
{
	if(!isShown())
		return;

	if(checkForNeedRedraw())
	{
		dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
		if(Rect(getParent()->getAbsolutePosition() + Point(getWidth(), startY+5) - Size(14, 0), Size(12, getHeight()+12)).Inside(mouse))
			dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
		else
			dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
			
		dc->DrawRectangle(Rect(getParent()->getAbsolutePosition() + Size(getParent()->getWidth()-13, startY+5), Size(10, clientHeight)));
		dc->SetBrush(*theme.lookUpBrush(CONTINUE_BUTTON_BRUSH));
		dc->DrawRectangle(Rect(getParent()->getAbsolutePosition() + Point(getParent()->getWidth()-12, startY+currentScrollY+5), Size(8, barHeight)));
	}
	UI_Object::draw(dc);
		
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

