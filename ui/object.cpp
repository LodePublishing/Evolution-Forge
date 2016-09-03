#include "object.hpp"
#include "configuration.hpp"

// TODO zwischen Positionseltern und Processeltern unterscheiden (damit man z.B. ein Menue an die Position einer Group setzen kann, ohne dass es teil der group wird... mmmh


UI_Object::UI_Object(UI_Object* parent_object, const Rect relative_rect, const Size distance_bottom_right, const ePositionMode position_mode, const eAutoSize auto_size) :
	children(NULL),
	relativeRect(relative_rect),
	startRect(relative_rect),
	targetRect(relative_rect),
	originalRect(relative_rect),
	distanceBottomRight(distance_bottom_right),
	oldSize(relative_rect.GetSize()),
	sizeHasChanged(true),

	positionMode(position_mode),
	autoSize(auto_size),
	shown(true),

	min_top_left_x(0),
	min_left_y(0),
	min_right_y(0),
	min_bottom_left_x(0),
	min_top_right_x(0),
	min_bottom_right_x(0),	

	needRedraw(true),
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
	children(NULL),
	relativeRect(relative_rect),
	startRect(relative_rect),
	targetRect(relative_rect),
	originalRect(relative_rect),
	distanceBottomRight(distance_bottom_right),
	oldSize(relative_rect.GetSize()),
	sizeHasChanged(true),

	positionMode(position_mode),
	autoSize(auto_size),
	shown(true),

	min_top_left_x(0),
	min_left_y(0),
	min_right_y(0),
	min_bottom_left_x(0),
	min_top_right_x(0),
	min_bottom_right_x(0),	

	needRedraw(true),
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

UI_Object& UI_Object::operator=(const UI_Object& object)
{
	children = object.children;
	
	relativeRect = object.relativeRect;
	startRect = object.startRect;
	targetRect = object.targetRect;
	
	originalRect = object.originalRect;
	distanceBottomRight = object.distanceBottomRight;
	oldSize = object.oldSize;
	sizeHasChanged = true;	

	positionMode = object.positionMode;
	autoSize = object.autoSize;
	shown = object.shown;

	min_top_left_x = object.min_top_left_x;
	min_left_y = object.min_left_y;
	min_right_y = object.min_right_y;
	min_bottom_left_x = object.min_bottom_left_x;
	min_top_right_x = object.min_top_right_x;
	min_bottom_right_x = object.min_bottom_right_x;
	needRedraw = object.needRedraw;
	prevBrother = this; // !!
	nextBrother = this; // !!
	parent = NULL; // !!
	positionParent = NULL; //!?
	toolTipEString = object.toolTipEString;
	toolTipString = object.toolTipString;
	
	setParent(object.parent);
	
	return(*this);
}

UI_Object::UI_Object(const UI_Object& object) :
	children( object.children ),
	relativeRect( object.relativeRect ),
	startRect( object.startRect ),
	targetRect( object.targetRect ),
	originalRect( object.originalRect ),
	distanceBottomRight( object.distanceBottomRight ),
	oldSize( object.oldSize ),
	sizeHasChanged( true ),

	positionMode( object.positionMode ),
	autoSize( object.autoSize ),
	
	shown( object.shown ), 

	min_top_left_x( object.min_top_left_x ),
	min_left_y( object.min_left_y ),
	min_right_y( object.min_right_y ),
	min_bottom_left_x( object.min_bottom_left_x ),
	min_top_right_x( object.min_top_right_x ),
	min_bottom_right_x( object.min_bottom_right_x ),

	needRedraw( object.needRedraw ),

	parent( NULL ), // !!
	positionParent( NULL ), // !!
	prevBrother( this ), // !!
	nextBrother( this ), // !!
	toolTipEString( object.toolTipEString ),
	toolTipString( object.toolTipString )
{ 
	setParent(object.parent);
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


#include <sstream>
void UI_Object::adjustPositionAndSize(const eAdjustMode adjust_mode, const Size& size)
{
//	UI_Object::addToProcessArray(this);
//	sizeHasChanged = false; ?
	signed int left = originalRect.GetLeft();
	signed int top = originalRect.GetTop();
	unsigned int full_width;
	unsigned int full_height;
	if(getParent() != NULL)
	{
		full_width = getParent()->getWidth() - left - distanceBottomRight.GetWidth();
		full_height = getParent()->getHeight() - top - distanceBottomRight.GetHeight();
	} 
	else
	{
		full_width = getWidth();
		full_height = getHeight();
	}
	if(adjust_mode == ADJUST_ONLY_POSITION)
	{
//		if(size!=Size(0,0))
//			setSize(size);
//		if(getParent()!=NULL)
//			getParent()->adjustPositionAndSize(ADJUST_AFTER_CHILD_SIZE_WAS_CHANGED, getSize());
// called after size was changed
	} else
	if(adjust_mode == ADJUST_AFTER_PARENT_WAS_CHANGED)
	{
// called by PARENT after PARENT's size was changed
		switch(autoSize)
		{
			case NOTHING:break;
			case NO_AUTO_SIZE:break;
			case AUTO_SIZE_ONCE:
			case AUTO_SIZE_TWICE:
			case AUTO_SIZE:;break;
			case AUTO_HEIGHT_FULL_WIDTH:
			case FULL_WIDTH:setWidth(full_width);break;
			case AUTO_HEIGHT_CONST_WIDTH:break;// ~~ nur fuer tabs naja
			case CONST_SIZE:break;// ~~ nur fuer tabs naja
			default:break; // TODO ERROR
		}
	} else
	if(adjust_mode == ADJUST_AFTER_CHILD_SIZE_WAS_CHANGED)
	{
// called if the object is dependant on the child's size (e.g. Button - StaticText)
// TODO maybe update other children too
	
		switch(autoSize)
		{
			case NOTHING:break;
			case NO_AUTO_SIZE:setSize(originalRect.GetSize());break;
			case AUTO_SIZE_ONCE:
/*				setSize(size+Size(6, 0));
				autoSize = AUTO_SIZE_TWICE;
				break;*/
			case AUTO_SIZE_TWICE:
//					setSize(size+Size(6, 0));
//					autoSize = NO_AUTO_SIZE;break;
			case AUTO_SIZE:setSize(size+Size(6, 0));break;
			case AUTO_HEIGHT_FULL_WIDTH:setSize(Size(full_width, size.GetHeight()));break;
			case FULL_WIDTH:setSize(Size(full_width, getHeight()));break;
			case AUTO_HEIGHT_CONST_WIDTH:setSize(Size(originalRect.GetWidth(), size.GetHeight()));break;// ~~ nur fuer tabs naja
			case CONST_SIZE:setSize(Size(originalRect.GetWidth(), originalRect.GetHeight()));break;// ~~ nur fuer tabs naja
			default:break; // TODO ERROR
		}
		
//		resetMinXY();	
	}
	
	signed int hor_center = left + ((signed int)full_width - (signed int)getWidth())/2;
	signed int right = (signed int)full_width - left - (signed int)getWidth();

	signed int ver_center = top + ((signed int)full_height - (signed int)getHeight())/2;
	if(ver_center < top) ver_center = top;
	signed int bottom = (signed int)full_height - top - (signed int)getHeight();

//	setPosition(originalButtonRect.GetTopLeft());
	
	//relativeRect ok... aber was ist mit startRect, targetRect etc??

 	switch(positionMode)
	{
		case DO_NOT_ADJUST:break;//setPosition(originalButtonRect.GetTopLeft());break;
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
		case ARRANGE_TOP_LEFT:
		{
			if(getParent())
			{
				setPosition(left + getParent()->getMinTopLeftX(), top);
				getParent()->addMinTopLeftX(getWidth() + MIN_DISTANCE);
			}
		}break;
		case ARRANGE_TOP_RIGHT:
		// TODO: Wird irgendwie beim Erstaufruf falsch (getmintoprightx mit falschem Wert initialisiert?)
		{
			if(getParent())
			{
				setPosition(right - getParent()->getMinTopRightX(), top);
				getParent()->addMinTopRightX(getWidth() + MIN_DISTANCE);
			}
		}break;
		case ARRANGE_BOTTOM_LEFT:
		{ 
			if(getParent())
			{
				setPosition(left + getParent()->getMinBottomLeftX(), bottom);
				getParent()->addMinBottomLeftX(getWidth()+MIN_DISTANCE);
			}
		}break;
		case ARRANGE_BOTTOM_RIGHT:
		{
			if(getParent())
			{
				setPosition(right - getParent()->getMinBottomRightX() - 20, bottom);
				getParent()->addMinBottomRightX(getWidth() + MIN_DISTANCE);
			}
		}break;
		case ARRANGE_LEFT:
		if(adjust_mode!=ADJUST_AFTER_CHILD_SIZE_WAS_CHANGED)
		{
			if(getParent())
			{
				setPosition(left, top + getParent()->getMinLeftY());
				getParent()->addMinLeftY(getHeight() + MIN_DISTANCE);
			}
		}break;
		case ARRANGE_RIGHT:
		{
			if(getParent())
			{
				setPosition(right, top + getParent()->getMinRightY()); // TODO
				getParent()->addMinRightY(getHeight()+MIN_DISTANCE);
			}
		}break;
		default:
			toLog("Wheee");
		break;//TODO error
	}
}


void UI_Object::adjustRelativeRect(const Rect& edge)
{
	if(edge != targetRect)
	{
// neues Ziel?
/*
		if((edge.GetLeft()!=targetRect.GetLeft())||(edge.GetRight()!=targetRect.GetRight()))
		{
			startRect.SetLeft(getRelativeLeftBound());
//			if(edge.GetRight()!=targetRect.GetRight())
			startRect.SetRight(getRelativeRightBound());
		}
			
		if((edge.GetTop()!=targetRect.GetTop())||(edge.GetHeight()!=targetRect.GetHeight()))
		{
			startRect.SetTop(getRelativeUpperBound());
//			if(edge.GetHeight()!=targetRect.GetHeight())
			startRect.SetBottom(getRelativeLowerBound());
		}*/

		if(edge.GetTopLeft()!=targetRect.GetTopLeft())
			startRect.SetTopLeft(getRelativePosition());
		if(edge.GetSize()!=targetRect.GetSize())
			startRect.SetSize(getSize());
 
//		UI_Object::addToProcessArray(this);
		targetRect=edge;
	}
//	targetRect.width=edge.width;
  //  targetRect.height=edge.height;
}


// Take gadget out of family.  Children of this gadget stay with gadget, though.
//
// A family is basically the whole parent/sibling/children hierarchy for gadgets.  Any gadget
// that is linked to another gadget by one of these pointers is said to be in the same family
// as that gadget.  This function, therefore, caused all references to a gadget by it's
// family's gadgets' sibling or children pointers to be broken, and all references to any of them
// by this gadget's parent or sibling pointers to also be broken.  This isolates the gadget and
// it's children into a new family.
//
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

//		children = child; ?
	}
}

void UI_Object::resetMinXY()
{
	UI_Object* tmp=children;  // process all children of gadget
	if (tmp) {
		min_top_right_x = 0; min_bottom_right_x = 0; min_bottom_left_x = 0; min_left_y = 0; min_top_left_x = 0; min_right_y = 0;
		do {
			tmp->resetMinXY();
			tmp = tmp->nextBrother;
		} while (tmp != children);
	}

}

void UI_Object::process()
{
//	if (!isShown()) //~~
//		return;

	if(startRect != targetRect)
	{
		if(uiConfiguration.isSmoothMovements())
		{

			if(relativeRect.moveSmooth(startRect, targetRect))
			{
				setNeedRedrawMoved();
			}
		}
		else 
		{
			if(relativeRect.move(startRect, targetRect))
			{
				setNeedRedrawMoved();
			}
		}
		if(oldSize != relativeRect.GetSize())
			sizeHasChanged=true;
	}

	if(sizeHasChanged==true)
	{
		adjustPositionAndSize(ADJUST_ONLY_POSITION);
		UI_Object* tmp=children;  // process all children of gadget
		if (tmp) {
			do {
				tmp->adjustPositionAndSize(ADJUST_AFTER_PARENT_WAS_CHANGED);
				tmp = tmp->nextBrother;
			} while (tmp != children);
		}
		sizeHasChanged=false;
	}
	

	UI_Object* tmp=children;  // process all children of gadget
	if (tmp) {
		do {
			tmp->process();
			tmp = tmp->nextBrother;
		} while (tmp != children);
	}
	
	if(/*(efConfiguration.isToolTips())&&*/(isMouseInside())&&(hasToolTip()))
		toolTipParent = this;
	oldSize = relativeRect.GetSize();
//	resetMinXY();	
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

UI_Object* UI_Object::checkToolTip()
{
	if(!isShown())
		return(NULL);
//	if(!(getAbsoluteRect().Inside(p)))
//		return(0); 0 size players ?
	UI_Object* tmp=children;  // process all children of gadget
	
	if(!tmp)
		return(NULL); // return 0 as this is an object and no button!
	UI_Object* result=NULL;
	do 
	{
		result = tmp->checkToolTip();
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
//	if(!(getAbsoluteRect().Inside(p)))
//		return(0); 0 size players ?

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
	UI_Object* tmp = children;
	if (tmp) {
		do {
			tmp->draw(dc);
			tmp = tmp->nextBrother;
		} while (tmp != children);
	}
}

void UI_Object::setOriginalRect(const Rect& rect) {
	originalRect = rect;
	setRect(rect);
}

const bool UI_Object::isMoving() const
{
	return((isShown())&&(relativeRect.GetLeft() != targetRect.GetLeft()));
}

void UI_Object::setRect(const Rect& rect) 
{
	if(rect == relativeRect)
		return;
	startRect = rect;
	targetRect = rect;
	relativeRect = rect;
	setNeedRedrawMoved();
	sizeHasChanged=true;
//	UI_Object::addToProcessArray(this);
}

void UI_Object::setPosition(const Point& position)
{
	if(position == relativeRect.GetTopLeft())
		return;
#ifdef _SCC_DEBUG
	if((position.y > 900000)||(position.x > 900000))
	{
		toLog("DEBUG (UI_Object::setPosition()): Value position out of range.");return;
	}
#endif
	startRect.SetTopLeft(position);
	targetRect.SetTopLeft(position);
	relativeRect.SetTopLeft(position);
	setNeedRedrawMoved();
//	UI_Object::addToProcessArray(this);
}

void UI_Object::setHeight(const unsigned int height) 
{
	if(getTargetHeight() == height)
		return;
	sizeHasChanged = true;
	relativeRect.SetHeight(height);
	startRect.SetHeight(height);
	targetRect.SetHeight(height);
	setNeedRedrawNotMoved(); // TODO wenns kleiner wird
//	UI_Object::addToProcessArray(this);
}

void UI_Object::setWidth(const unsigned int width) 
{
	if(relativeRect.GetWidth() == width)
		return;
//	UI_Object::addToProcessArray(this);
	sizeHasChanged = true;
	relativeRect.SetWidth(width);
	startRect.SetWidth(width);
	targetRect.SetWidth(width);
	setNeedRedrawNotMoved(); // TODO Wenns kleiner wird!
}

void UI_Object::setSize(const Size size)
{
	if(relativeRect.GetSize() == size)
		return;
//	UI_Object::addToProcessArray(this);
	sizeHasChanged = true;
	relativeRect.SetSize(size);
	startRect.SetSize(size);
	targetRect.SetSize(size);
	
	setNeedRedrawNotMoved(); // TODO Wenns kleiner wird!
}

void UI_Object::setLeft(const signed int x) 
{
	if(relativeRect.GetLeft() == x)
		return;
//	UI_Object::addToProcessArray(this);
	relativeRect.SetLeft(x);
	startRect.SetLeft(x);
	targetRect.SetLeft(x);
	setNeedRedrawMoved();
}
		
void UI_Object::setTop(const signed int y) 
{
	if(relativeRect.GetTop() == y)
		return;
	relativeRect.SetTop(y);
	startRect.SetTop(y);
	targetRect.SetTop(y);
	setNeedRedrawMoved();
}

void UI_Object::Show(const bool show)
{
	if((show)&&(!shown))
	{
		shown = true;
		setNeedRedrawMoved(true);
	} 
	else if((!show)&&(shown))
	{
		setNeedRedrawMoved(true);
//		setNeedRedrawMoved(false); // ~~ ?
		shown = false;
	}
}

void UI_Object::setNeedRedrawMoved(const bool need_redraw)  // moved item
{
	if(!isShown())
		return;
	setNeedRedrawNotMoved(need_redraw);
	if((need_redraw)&&(getParent()))
	{
//		getParent()->checkForChildrenOverlap(getRelativeRect()); TODO
		
		if(!getParent()->getAbsoluteRect().Inside(getAbsoluteRect()))
			getParent()->setNeedRedrawMoved(true);
		else
			getParent()->setNeedRedrawNotMoved(true);
	}
}

void UI_Object::checkForChildrenOverlap(const Rect& rect)
{
        UI_Object* tmp = children;
        if (tmp) {
                do {
			if(rect.overlaps(tmp->getRelativeRect()))
	                        tmp->setNeedRedrawNotMoved(true);
                        tmp = tmp->nextBrother;
                } while (tmp != children);
        }
}

void UI_Object::setNeedRedrawNotMoved(const bool need_redraw)
{
	if(!isShown())
		return;

	needRedraw = need_redraw;

	if(!need_redraw)
		return;

	DC::addRectangle(Rect(getAbsolutePosition(), getSize()+Size(1,1))); // TODO Hack fuer buttons
        UI_Object* tmp = children;

        if (tmp) {
                do {
                        tmp->setNeedRedrawNotMoved(needRedraw);
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

void UI_Object::setNeedRedrawNotMovedFirstChild(const bool need_redraw)
{
	if(!isShown())
		return;

	needRedraw = need_redraw;

	if(!need_redraw)
		return;

	DC::addRectangle(Rect(getAbsolutePosition(), getSize()+Size(1,1))); // TODO Hack fuer buttons
	if(children)
		children->setNeedRedrawNotMoved(needRedraw);
}

void UI_Object::clearRedrawFlag()
{
	if(!isShown())
	{
		needRedraw = false;
		return;
	}
	if(needRedraw)
		needRedraw = false;
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

void UI_Object::addMinTopLeftX(signed int dx)
{
	if((dx + min_top_left_x > (signed int)max_x)||(dx + min_top_left_x < 0))
	{
//		std::ostringstream os;os.str("");
//		os << "MIN_TOP_LEFT_X out of range [ dx: " << dx << ", min_top_left_x: " << min_top_left_x << ", max_x: " << max_x << " ]  { " << getAbsolutePosition().x << ":" << getAbsolutePosition().y << "  " << getWidth() << "x" << getHeight() << " }";
//		toLog(os.str());
	}
	min_top_left_x += dx;		
}

void UI_Object::addMinLeftY(signed int dy)
{
	if((dy + min_left_y > (signed int)max_y)||(dy + min_left_y < 0));
//		toLog("MIN_LEFT_Y out of range");
	min_left_y += dy;
}

void UI_Object::addMinRightY(signed int dy)
{
	if((dy + min_right_y > (signed int)max_y)||(dy + min_right_y < 0));
//		toLog("MIN_RIGHT_Y out of range");
	min_right_y += dy;
}

void UI_Object::addMinBottomLeftX(signed int dx)
{
	if((dx + min_bottom_left_x > (signed int)max_x)||(dx + min_bottom_left_x < 0));
//		toLog("MIN_BOTTOM_LEFT_X out of range");
	min_bottom_left_x += dx;		
}

void UI_Object::addMinTopRightX(signed int dx)
{
	if((dx + min_top_right_x > (signed int)max_x)||(dx + min_top_right_x < 0))
	{
//		std::ostringstream os;os.str("");
//		os << "MIN_TOP_RIGHT_X out of range [ dx: " << dx << ", min_top_right_x: " << min_top_right_x << ", max_x: " << max_x << " ]  { " << getAbsolutePosition().x << ":" << getAbsolutePosition().y << "  " << getWidth() << "x" << getHeight() << " }";
//		toLog(os.str());
	}
	min_top_right_x += dx;		
}

void UI_Object::addMinBottomRightX(signed int dx)
{
	if((dx + min_bottom_right_x > (signed int)max_x)||(dx + min_bottom_right_x < 0));
//		toLog("BUTTON MIN_BOTTOM_RIGHT_X out of range");
	min_bottom_right_x += dx;		
}


void UI_Object::setResolution(const Size resolution)
{
	max_x = resolution.GetWidth();
	max_y = resolution.GetHeight();
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
/*
bool UI_Object::editFieldActive()
{
	if(editFieldList.empty())
		return(false);
	else return(true);
}

void UI_Object::addEditField(UI_EditField* edit_field)
{
	if(*editField == edit_field)
		return;
	else
		for(std::list<UI_EditField*>::iterator i = editFieldList.begin(); i != editFieldList.end(); ++i)
			if(*i==edit_field)
				return;
	editField = editFieldList.insert(editFieldList.end(), edit_field);
}

void UI_Object::removeEditField(UI_EditField* edit_field)
{
	if(*editField == edit_field)
		editField = editFieldList.erase(editField);
	else
	{
		for(std::list<UI_EditField*>::iterator i = editFieldList.begin(); i != editFieldList.end(); ++i)
		{
			if(*i==edit_field)
			{
				editFieldList.erase(i);
				break;
			}
		}
	}
}

void UI_Object::rotateEditField()
{
	if(!editFieldActive())
		return;
	editField++;
}

UI_EditField* UI_Object::getEditField()
{
	if(!editFieldActive())
		return(NULL);
	return(*editField);
}*/


//std::list<UI_EditField*> UI_Object::editFieldList;
//std::list<UI_EditField*>::iterator UI_Object::editField;
//UI_Object* hotkey[300];

UI_Object* UI_Object::focus(NULL);

//unsigned int UI_Object::key;

UI_Theme UI_Object::theme;
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

//unsigned int UI_Object::key;
//unsigned int UI_Object::mod;

