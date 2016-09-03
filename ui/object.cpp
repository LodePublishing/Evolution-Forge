#include "object.hpp"

UI_Object::UI_Object(UI_Object* parent_object, const Rect relative_rect, const Size distance_bottom_right, const ePositionMode position_mode, const eAutoSize auto_size) :
	isClipped(false),
	clipRect(),
	children(NULL),
	childrenWereChanged(false),
	positionMode(position_mode),
	relativeRect(relative_rect),
	startRect(relative_rect),
	targetRect(relative_rect),
	originalRect(relative_rect),
	distanceBottomRight(distance_bottom_right),
	oldSize(relative_rect.getSize()),
	sizeHasChanged(true),

	autoSize(auto_size),
	shown(true),

	needRedraw(true),
	redrawArea(),
	parent(NULL),
	positionParent(NULL),
	prevBrother(this),
	nextBrother(this),
	toolTipEString(NULL_STRING),
	toolTipString()
{
	setParent(parent_object);
//	addToProcessArray(this);
}

UI_Object::UI_Object(UI_Object* parent_object, UI_Object* position_parent_object, const Rect relative_rect, const Size distance_bottom_right, const ePositionMode position_mode, const eAutoSize auto_size) :
	isClipped(false),
	clipRect(),
	children(NULL),
	childrenWereChanged(false),
	positionMode(position_mode),
	relativeRect(relative_rect),
	startRect(relative_rect),
	targetRect(relative_rect),
	originalRect(relative_rect),
	distanceBottomRight(distance_bottom_right),
	oldSize(relative_rect.getSize()),
	sizeHasChanged(true),

	autoSize(auto_size),
	shown(true),

	needRedraw(true),
	redrawArea(),
	parent(NULL),
	positionParent(NULL),
	prevBrother(this),
	nextBrother(this),
	toolTipEString(NULL_STRING),
	toolTipString()
{
	setParent(parent_object);
	setPositionParent(position_parent_object);
//	addToProcessArray(this);
}

UI_Object::~UI_Object()
{
	if(UI_Object::focus == this)
		UI_Object::focus = NULL;
	removeFromFamily(); // !!!!!
}

const bool UI_Object::addKey(unsigned int key, unsigned int mod)
{
	return(false);
}

void UI_Object::reloadOriginalSize()
{
//	UI_Object::addToProcessArray(this);
	UI_Object* tmp=children;  // process all children of gadget
	if (tmp) {
		do {
			tmp->reloadOriginalSize();
			tmp = tmp->nextBrother;
			} while (tmp != children);
		}	
}

void UI_Object::adjustSize(const eAdjustMode adjust_mode, const Size& size)
{
//	UI_Object::addToProcessArray(this);
	signed int left = originalRect.getLeft();
	unsigned int full_width;
	if(getParent() != NULL)
	{
		if(left + distanceBottomRight.getWidth() > getParent()->getWidth())
			full_width = 0;
		else 
			full_width = getParent()->getWidth() - left - distanceBottomRight.getWidth();
	} 
	else
		full_width = getWidth();

	if(adjust_mode == PARENT_WAS_CHANGED)
	{
// called by PARENT after PARENT's size was changed
		switch(autoSize)
		{
			case NOTHING:break;
			case NO_AUTO_SIZE:break;
			case AUTO_SIZE:break;
			case AUTO_HEIGHT_FULL_WIDTH:
			case AUTO_DOUBLE_HEIGHT_FULL_WIDTH:
			case FULL_WIDTH:setWidth(full_width);break;
			case AUTO_HEIGHT_CONST_WIDTH:break;
			case AUTO_DOUBLE_HEIGHT_CONST_WIDTH:break;
			case CONST_SIZE:break;
			default:break; // TODO ERROR
		}
	} else
	if(adjust_mode == CHILD_WAS_CHANGED)
	{
// called if the object is dependant on the child's size (e.g. Button - StaticText)
// TODO maybe update other children too
		childrenWereChanged = true;
		switch(autoSize)
		{
			case NOTHING:break;
			case NO_AUTO_SIZE:setSize(originalRect.getSize());break;
			case AUTO_SIZE:setSize(size+Size(6, 0));break;
			case AUTO_HEIGHT_FULL_WIDTH:setSize(Size(full_width, size.getHeight()));break;
			case AUTO_DOUBLE_HEIGHT_FULL_WIDTH:setSize(Size(full_width, 2*size.getHeight()));break;
			case FULL_WIDTH:setSize(Size(full_width, getHeight()));break;
			case AUTO_HEIGHT_CONST_WIDTH:setSize(Size(originalRect.getWidth(), size.getHeight()));break;
			case AUTO_DOUBLE_HEIGHT_CONST_WIDTH:setSize(Size(originalRect.getWidth(), 2*size.getHeight()));break;
			case CONST_SIZE:setSize(Size(originalRect.getWidth(), originalRect.getHeight()));break;
			default:break; // TODO ERROR
		}
	}
}
	
void UI_Object::adjustPosition()
{
//	UI_Object::addToProcessArray(this);
	signed int left = originalRect.getLeft();
	signed int top = originalRect.getTop();
	unsigned int full_width;
	unsigned int full_height;
	if(getParent() != NULL)
	{
		full_width = getParent()->getWidth() - left - distanceBottomRight.getWidth();
		full_height = getParent()->getHeight() - top - distanceBottomRight.getHeight();
	} 
	else
	{
		full_width = getWidth();
		full_height = getHeight();
	}
	
	signed int hor_center = left + ((signed int)full_width - (signed int)getWidth())/2;
	signed int right = (signed int)full_width - left - (signed int)getWidth();

	signed int ver_center = top + ((signed int)full_height - (signed int)getHeight())/2;
	if(ver_center < top) ver_center = top;
	signed int bottom = (signed int)full_height - top - (signed int)getHeight();

//	setPosition(originalButtonRect.getTopLeft());
	//relativeRect ok... aber was ist mit startRect, targetRect etc??

 	switch(positionMode)
	{
		case DO_NOT_ADJUST:break;//setPosition(originalButtonRect.getTopLeft());break;
		case TOTAL_CENTERED:setPosition(hor_center, ver_center);break;
		case HORIZONTALLY_CENTERED:setLeft(hor_center);break;
		case VERTICALLY_CENTERED:setTop(ver_center);break;
		case TOP_LEFT:setPosition(left, top);break;
		case TOP_CENTER:setPosition(hor_center, top);break;
		case TOP_RIGHT:setPosition(right, top);break;
		case CENTER_RIGHT:setPosition(right, ver_center);break;
		case BOTTOM_RIGHT:setPosition(right, bottom);break;
		case BOTTOM_CENTER:setPosition(hor_center, bottom);break;
		case BOTTOM_LEFT:setPosition(left, bottom);break;
		case CENTER_LEFT:setPosition(left, ver_center);break;
		default:break;
	}
}


void UI_Object::adjustRelativeRect(const Rect& edge)
{
	if(edge != targetRect)
	{
// neues Ziel?
/*
		if((edge.getLeft()!=targetRect.getLeft())||(edge.getRight()!=targetRect.getRight()))
		{
			startRect.setLeft(getRelativeLeftBound());
//			if(edge.getRight()!=targetRect.getRight())
			startRect.setRight(getRelativeRightBound());
		}
			
		if((edge.getTop()!=targetRect.getTop())||(edge.getHeight()!=targetRect.getHeight()))
		{
			startRect.setTop(getRelativeUpperBound());
//			if(edge.getHeight()!=targetRect.getHeight())
			startRect.setBottom(getRelativeLowerBound());
		}*/

		if(edge.getTopLeft()!=targetRect.getTopLeft())
			startRect.setTopLeft(getRelativePosition());
		if(edge.getSize()!=targetRect.getSize())
			startRect.setSize(getSize());
 
//		UI_Object::addToProcessArray(this);
		targetRect=edge;
	}
//	targetRect.width=edge.width;
  //  targetRect.height=edge.height;
}


void UI_Object::removeFromFamily()
{
	if((parent)&&(parent->children == this))
	{
		if (nextBrother == this)  // the only child?
			parent->children = NULL;  // if so, parent now has now no children
		else
			parent->children = nextBrother;  // next sibling is now the oldest
	} 
	parent = NULL;
	if (nextBrother != this) {  // does this object have siblings?
		nextBrother->prevBrother = prevBrother;
		prevBrother->nextBrother = nextBrother;
	}
	
	nextBrother = prevBrother = this;
}

void UI_Object::setPositionParent(UI_Object* daddy)
{
	positionParent = daddy;
}

void UI_Object::setParent(UI_Object* daddy) 
{
	setPositionParent(daddy);
	removeFromFamily();
	parent = daddy;

	if(!daddy)
		return;
	daddy->addChild(this);
}

void UI_Object::addChild(UI_Object* child)
{
	childrenWereChanged = true;
	if (!children) 
		children = child;
	else 
	{
		UI_Object *eldest_sibling, *youngest_sibling;

		eldest_sibling = children;
		youngest_sibling = eldest_sibling->prevBrother;

		child->nextBrother = eldest_sibling;
		child->prevBrother = youngest_sibling;

		eldest_sibling->prevBrother = child;
		youngest_sibling->nextBrother = child;
	}
}

void UI_Object::process()
{
//	if (!isShown()) //~~
//		return;
	if(relativeRect != targetRect)
	{
		Rect old_rect = relativeRect;
		eRectMovement t = uiConfiguration.isSmoothMovements() ? relativeRect.moveSmooth(startRect, targetRect) : relativeRect.move(targetRect);
		if(t == GOT_BIGGER)
			setNeedRedrawNotMoved();
		else if(t == GOT_SMALLER_OR_MOVED)
			setNeedRedrawMoved(old_rect, relativeRect);
		if(oldSize != relativeRect.getSize())
			setSizeHasChanged();
	}

	if(hasSizeChanged())
	{
//		adjustSize(CHILD_WAS_CHANGED, getSize()); // TODO ? muesste hier nicht die Groesse des Kinds stehen?
		if(getParent())
			getParent()->adjustSize(CHILD_WAS_CHANGED, getSize()); // problematisch TODO
		UI_Object* tmp=children;  // process all children of gadget
		if (tmp) {
			do {
				tmp->adjustSize(PARENT_WAS_CHANGED, getSize());
				tmp->adjustPosition();
				tmp = tmp->nextBrother;
			} while (tmp != children);
		}
		
		adjustPosition();
		setSizeHasChanged(false);
	}


	// hier wird height des fensters veraendert!
	UI_Object* tmp=children;  // process all children of gadget
	if (tmp) {
		do {
			tmp->process();
			tmp = tmp->nextBrother;
		} while (tmp != children);
	}
	oldSize = relativeRect.getSize();
}

void UI_Object::setClipRect(const Rect& rect)
{
	UI_Object* tmp=children;  // process all children of gadget
	if (tmp) {
		do {
			tmp->setClipRect(rect);
			tmp = tmp->nextBrother;
		} while (tmp != children);
	}
	clipRect = rect;
}

void UI_Object::setSizeHasChanged(const bool size_has_changed)
{
	if(size_has_changed == true)
	{
		sizeHasChanged = true;
	}
	else if(size_has_changed == false)
	{
		sizeHasChanged = false;
	}
		
}

void UI_Object::resetData()
{ 
	UI_Object* tmp = children;  // process all children of gadget
	if (tmp) {
		do {
			tmp->resetData();
			tmp = tmp->nextBrother;
																				
		} while (tmp != children);
	}
}

// returns the object the mouse cursor is currently over and owns a tooltip
UI_Object* UI_Object::checkToolTip()
{
	if(!isShown())
		return(NULL);
	UI_Object* tmp=children;  // process all children of gadget
	
	if(!tmp)
		return(NULL); // return 0 as this is an object and no button!
	UI_Object* result=NULL;
	do 
	{
		result = tmp->checkToolTip();
		if((result!=NULL)&&(!result->hasToolTip()))
			result = NULL;
		tmp = tmp->nextBrother;
	} while((tmp!=children)&&(result==NULL));
	
	return(result);
}

void UI_Object::reloadStrings()
{
	UI_Object* tmp=children;  // process all children of gadget
	if(!tmp)
		return;
	do 
	{
		tmp->reloadStrings();
		tmp = tmp->nextBrother;
	}
	while(tmp!=children);
}


UI_Object* UI_Object::checkHighlight()
{
	if(!isShown())
		return(NULL);
	UI_Object* tmp=children;  // process all children of gadget
	if(!tmp)
		return(NULL); // return 0 as this is an object and no button!
	UI_Object* result=NULL;
	do 
	{
		result = tmp->checkHighlight();
		tmp = tmp->nextBrother;
	} while((tmp!=children)&&(result==NULL));
	
	return(result);
}

void UI_Object::draw(DC* dc) const
{
	// if hidden, hide children as well
	if (!isShown())
		return;
	
	if(redrawArea.size())
	{
		dc->setBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
		dc->setPen(*theme.lookUpPen(NULL_PEN));
		for(std::list<Rect>::const_iterator i = redrawArea.begin(); i!= redrawArea.end(); ++i)
			dc->DrawRectangle(*i);
	}
	
	// for 'redraw' optimizations - to see which window is redrawn
//	dc->setPen(*theme.lookUpPen((ePen)(5+(rand()%2))));
//	dc->DrawEmptyRectangle(getAbsoluteRect());

	UI_Object* tmp = children;
	if (tmp) {
		do {
			tmp->draw(dc);
			tmp = tmp->nextBrother;
		} while (tmp != children);
	}
}

void UI_Object::setSize(const unsigned int width, const unsigned int height) 
{
	setSize(Size(width, height));
}

void UI_Object::setOriginalRect(const Rect& rect) {
	originalRect = rect;
	setRect(rect);
}

void UI_Object::setOriginalSize(const Size& size) {
	originalRect.setSize(size);
	setSize(size);
}

void UI_Object::setOriginalWidth(const unsigned int width) {
	originalRect.setWidth(width);
	setWidth(width);
}

const bool UI_Object::isMoving() const
{
	return((isShown())&&(relativeRect.getLeft() != targetRect.getLeft()));
}

void UI_Object::setRect(const Rect& rect) 
{
	Rect old_rect = relativeRect;	
	eRectMovement movement = relativeRect.move(rect);
	switch(movement)
	{
		case NO_CHANGE:return;
		case GOT_SMALLER_OR_MOVED:setNeedRedrawMoved(old_rect, relativeRect);break;
		case GOT_BIGGER:setNeedRedrawNotMoved();break;
	}
	startRect = rect;
	targetRect = rect;
	setSizeHasChanged();
//	UI_Object::addToProcessArray(this);
}

void UI_Object::setPosition(const Point& position)
{
	if(position == relativeRect.getTopLeft())
		return;
#ifdef _SCC_DEBUG
	if((position.y > 900000)||(position.x > 900000))
	{
		toErrorLog("DEBUG (UI_Object::setPosition()): Value position out of range.");return;
	}
#endif
	
	Rect old_rect = relativeRect;
	startRect.setTopLeft(position);
	targetRect.setTopLeft(position);
	relativeRect.setTopLeft(position);
	
	setNeedRedrawMoved(old_rect, relativeRect);
//	UI_Object::addToProcessArray(this);
}

void UI_Object::setHeight(const unsigned int height) 
{
	if(getTargetHeight() == height)
		return;
	
	Rect old_rect = relativeRect;
	relativeRect.setHeight(height);
	startRect.setHeight(height);
	targetRect.setHeight(height);
	
	setSizeHasChanged();
	if(height < relativeRect.getHeight())
		setNeedRedrawMoved(old_rect, relativeRect);
	else setNeedRedrawNotMoved();
	
//	UI_Object::addToProcessArray(this);
}

void UI_Object::setWidth(const unsigned int width) 
{
	if(relativeRect.getWidth() == width)
		return;
	
	Rect old_rect = relativeRect;
	relativeRect.setWidth(width);
	startRect.setWidth(width);
	targetRect.setWidth(width);
	
	setSizeHasChanged();
	if(width < relativeRect.getWidth())
		setNeedRedrawMoved(old_rect, relativeRect);
	else setNeedRedrawNotMoved();

//	UI_Object::addToProcessArray(this);
}

void UI_Object::setSize(const Size size)
{
	if(relativeRect.getSize() == size)
		return;
	
	Rect old_rect = relativeRect;
	relativeRect.setSize(size);
	startRect.setSize(size);
	targetRect.setSize(size);
	
	setSizeHasChanged();
	if(size < relativeRect.getSize())
		setNeedRedrawMoved(old_rect, relativeRect);
	else setNeedRedrawNotMoved();

	
//	UI_Object::addToProcessArray(this);
}

void UI_Object::setLeft(const signed int x) 
{
	if(relativeRect.getLeft() == x)
		return;

	Rect old_rect = relativeRect;
	relativeRect.setLeft(x);
	startRect.setLeft(x);
	targetRect.setLeft(x);

	setNeedRedrawMoved(old_rect, relativeRect);

//	UI_Object::addToProcessArray(this);
}
		
void UI_Object::setTop(const signed int y) 
{
	if(relativeRect.getTop() == y)
		return;
	
	Rect old_rect = relativeRect;
	relativeRect.setTop(y);
	startRect.setTop(y);
	targetRect.setTop(y);
	
	setNeedRedrawMoved(old_rect, relativeRect);
}

void UI_Object::Show(const bool show)
{
	if((show)&&(!shown))
	{
		shown = true;
		setNeedRedrawNotMoved();
		if(getParent())
			getParent()->childrenWereChanged = true;
	} 
	else if((!show)&&(shown))
	{
		setNeedRedrawNotMoved();
//		setNeedRedrawMoved(false); // ~~ ?
		shown = false;
		if(getParent())
			getParent()->childrenWereChanged = true;
	}
}
/*
oldrect
newrect

'newrect < oldrect':
	->redraw itself and children, redraw parent's oldrect
'newrect > oldrect :
	moved
	notmoved
redrawbackground
*/

// called when old rectangle is bigger/smaller than the old
// call this directly when only the size has changed
void UI_Object::setNeedRedrawAllThatOverlaps(const Rect& rect)
{
	if(getParent())
	{
		getParent()->setNeedRedrawArea(rect);
	        UI_Object* tmp = getParent()->children;
        	if (tmp) {
                	do {
				if(rect.overlaps(tmp->getRelativeRect()))
		                        tmp->setNeedRedrawNotMoved();
                	        tmp = tmp->nextBrother;
	                } while (tmp != getParent()->children);
        	}
	} else
		setNeedRedrawNotMoved();
}

void UI_Object::setNeedRedrawArea(const Rect& rect)
{
	Rect absolute_rect = Rect(rect.getTopLeft() + getAbsolutePosition(), rect.getSize());
// check if it's inside
	if(getAbsoluteRect().isInside(absolute_rect))
		redrawArea.push_back(absolute_rect);
	else 
		setNeedRedrawAllThatOverlaps(Rect(rect.getTopLeft() + getRelativePosition(), rect.getSize()));
}

void UI_Object::setNeedRedrawMoved(const Rect& old_rect, const Rect& new_rect)
{
	if(old_rect.isInside(new_rect))
	{
		setNeedRedrawAllThatOverlaps(old_rect);
		return;
	} else if(new_rect.isInside(old_rect))
	{
		setNeedRedrawAllThatOverlaps(new_rect);
		return;
	}
	setNeedRedrawAllThatOverlaps(old_rect);
	setNeedRedrawAllThatOverlaps(new_rect);
}

void UI_Object::setNeedRedrawNotMoved()
{
	if(!isShown())
		return;

	needRedraw = true;

	DC::addRectangle(Rect(getAbsolutePosition(), getSize()+Size(1,1))); // TODO Hack fuer buttons
        UI_Object* tmp = children;

        if (tmp) {
                do {
                        tmp->setNeedRedrawNotMoved();
                        tmp = tmp->nextBrother;
                } while (tmp != children);
        }
}

void UI_Object::makeFirstChild()
{
	if((!getParent())||((getParent()->children == this)&&(getParent()->children->getNextBrother() == this)))
		return;
	UI_Object* old_parent = getParent();
	removeFromFamily();
	setParent(old_parent);
}

void UI_Object::setNeedRedrawNotMovedFirstChild()
{
	if(!isShown())
		return;

	needRedraw = true;

	DC::addRectangle(Rect(getAbsolutePosition(), getSize()+Size(1,1))); // TODO Hack fuer buttons
	if(children)
		children->setNeedRedrawNotMoved();
}

void UI_Object::clearRedrawFlag()
{
	if(!isShown())
	{
		needRedraw = false;
		redrawArea.clear();
		return;
	}
	if(needRedraw)
	{
		needRedraw = false;
		redrawArea.clear();
	}
	UI_Object* tmp = children;
	if (tmp) {
		do {
			tmp->clearRedrawFlag();
			tmp = tmp->nextBrother;
		} while (tmp != children);
	}
}

const bool UI_Object::checkForNeedRedraw() const
{
	if(needRedraw)
	{
		++redrawnObjects;
		return(true);
	} else return(false);
}

void UI_Object::setResolution(const Size resolution)
{
	max_x = resolution.getWidth();
	max_y = resolution.getHeight();
}


/*void UI_Object::addToProcessArray(UI_Object* item)
{
	for(std::list<UI_Object*>::const_iterator i = processArray.begin(); i != processArray.end(); ++i)
		if(*i == item)
			return;
	processArray.push_back(item);
}

void UI_Object::addToNextProcessArray(UI_Object* item)
{
	for(std::list<UI_Object*>::const_iterator i = nextProcessArray.begin(); i != nextProcessArray.end(); ++i)
		if(*i == item)
			return;
	nextProcessArray.push_back(item);
}
	
void UI_Object::copyToNextProcessArray()
{
	processArray.clear();
	processArray = nextProcessArray;
	nextProcessArray.clear();
}*/

void UI_Object::resetWindow()
{
	UI_Object::windowSelected = false;
	UI_Object::currentWindow = NULL;
}

UI_Object* UI_Object::focus(NULL);

UI_Theme UI_Object::theme;
UI_Sound UI_Object::sound;

UI_ToolTip* UI_Object::tooltip(NULL);
UI_Object* UI_Object::toolTipParent(NULL);
unsigned int UI_Object::max_x(0);
unsigned int UI_Object::max_y(0);
Point UI_Object::mouse(Point(0,0));

unsigned int UI_Object::mouseType(0);

UI_Window* UI_Object::currentWindow = NULL;
bool UI_Object::windowSelected = false;
unsigned int UI_Object::redrawnObjects(0);
std::list<std::string> UI_Object::msgList;

//std::list<UI_Object*> UI_Object::processArray;
//std::list<UI_Object*> UI_Object::nextProcessArray;

bool UI_Object::toolTipWasDeleted = false;

