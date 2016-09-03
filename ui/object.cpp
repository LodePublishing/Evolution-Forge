#include "object.hpp"
#include "configuration.hpp"

UI_Object::UI_Object(UI_Object* parent_object, const Rect relative_rect, const Size distance_bottom_right, const ePositionMode position_mode, const eAutoSize auto_size) :
	children(NULL),
	startRect(relative_rect),
	targetRect(relative_rect),
	originalRect(relative_rect),
	distanceBottomRight(distance_bottom_right),

	positionMode(position_mode),
	autoSize(auto_size),
	sizeHasChanged(true),
	shown(true),

	min_top_left_x(0),
	min_left_y(0),
	min_right_y(0),
	min_bottom_left_x(0),
	min_top_right_x(0),
	min_bottom_right_x(0),	

	needRedraw(true),
	prevBrother(this),
	nextBrother(this),
	parent(NULL),
	relativeRect(relative_rect),
	toolTipString(NULL_STRING)
{
	setParent(parent_object);
}

UI_Object& UI_Object::operator=(const UI_Object& object)
{
	startRect = object.startRect;
	targetRect = object.targetRect;
	originalRect = object.originalRect;
	distanceBottomRight = object.distanceBottomRight;
	children = object.children;

	positionMode = object.positionMode;
	autoSize = object.autoSize;
	sizeHasChanged = true;	
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
	relativeRect = object.relativeRect;
	toolTipString = object.toolTipString;
	
	setParent(object.parent);
	
	return(*this);
}

UI_Object::UI_Object(const UI_Object& object) :
	children( object.children ),
	startRect( object.startRect ),
	targetRect( object.targetRect ),
	originalRect( object.originalRect ),
	distanceBottomRight( object.distanceBottomRight ),
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
	prevBrother( this ), // !!
	nextBrother( this ), // !!
	parent( NULL ), // !!
	relativeRect( object.relativeRect ),
	toolTipString( object.toolTipString )
{ 
	setParent(object.parent);
}

UI_Object::~UI_Object()
{
	removeFromFamily(); // !!!!!
}


#include <sstream>
void UI_Object::adjustPositionAndSize(const eAdjustMode adjust_mode, const Size& size)
{
	sizeHasChanged=false;
	signed int left = originalRect.GetLeft();
	signed int top = originalRect.GetTop();
	
	unsigned int full_width;
	unsigned int full_height;
	if(getParent()!=NULL)
	{
		full_width = getParent()->getWidth() - left - distanceBottomRight.GetWidth();
		full_height = getParent()->getHeight() - top - distanceBottomRight.GetHeight();
		// Parent nicht initialisiert :[
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
	}
	
	signed int hor_center = left + (full_width - getWidth())/2;
	signed int right = full_width - left - getWidth();

	signed int ver_center = top + (full_height - getHeight())/2;
	signed int bottom = full_height - top - getHeight();

/*	if((positionMode==TOTAL_CENTERED)&&(getParent()))
	{
		std::ostringstream os;
		os << "2. " << hor_center << ":" << ver_center << "[" << getWidth() << ":" << getHeight() << "]" << " " << getParent()->getWidth() << "-" << left << "-" << distanceBottomRight.GetWidth() << "/ " << getParent()->getHeight() << "-" << top << "-" << distanceBottomRight.GetHeight() << " [" << full_width << ":" << full_height << "]";
						
		toLog(os.str());
	}*/

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
			setPosition(left + getParent()->getMinTopLeftX(), top);
			getParent()->addMinTopLeftX(getWidth() + MIN_DISTANCE);
		}break;
		case ARRANGE_TOP_RIGHT:
		{
			setPosition(right - getParent()->getMinTopRightX(), top);
			getParent()->addMinTopRightX(getWidth() + MIN_DISTANCE);
		}break;
		case ARRANGE_BOTTOM_LEFT:
		{ 
			setPosition(left + getParent()->getMinBottomLeftX(), bottom);
			getParent()->addMinBottomLeftX(getWidth()+MIN_DISTANCE);
		}break;
		case ARRANGE_BOTTOM_RIGHT:
		{
			setPosition(right - getParent()->getMinBottomRightX() - 20, bottom);
			getParent()->addMinBottomRightX(getWidth() + MIN_DISTANCE);
		}break;
		case ARRANGE_LEFT:
		{
			setPosition(left, top + getParent()->getMinLeftY());
			getParent()->addMinLeftY(getHeight()+MIN_DISTANCE);
		}break;
		case ARRANGE_RIGHT:
		{
			setPosition(right, top + getParent()->getMinRightY()); // TODO
			getParent()->addMinRightY(getHeight()+MIN_DISTANCE);
		}break;
		default:
			toLog("Wheee");
		break;//TODO error
	}
	
}


void UI_Object::adjustRelativeRect(Rect edge)
{
	if(edge != targetRect)
	{
// neues Ziel?
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
		}
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
			parent->children = NULL;  // if so, parent now has no children
		else
			parent->children = nextBrother;  // next sibling is now the eldest
	} 
	parent = NULL;
	if (nextBrother != this) {  // does this object have siblings?
		nextBrother->prevBrother = prevBrother;
		prevBrother->nextBrother = nextBrother;
	}
	
	nextBrother = prevBrother = this;
}

void UI_Object::setParent(UI_Object* daddy) 
{
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

void UI_Object::process()
{
//	if (!isShown()) //~~
//		return;

	Size old_size  = relativeRect.GetSize();
	if(uiConfiguration.isSmoothMovements())
	{
		if(relativeRect.moveSmooth(startRect, targetRect))
			setNeedRedrawMoved();
	}
	else 
	{
		if(relativeRect.move(startRect, targetRect))
			setNeedRedrawMoved();
	}
	if(old_size != relativeRect.GetSize())
		sizeHasChanged=true;

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
//	if(adjust_mode == ADJUST_AFTER_CHILD_SIZE_WAS_CHANGED)	
	
	min_top_right_x = min_bottom_right_x = min_bottom_left_x = min_left_y = min_top_left_x = min_right_y = 0;

	UI_Object* tmp=children;  // process all children of gadget
	if (tmp) {
		do {
			tmp->process();
			tmp = tmp->nextBrother;
																				
		} while (tmp != children);
	}
	
	if((toolTipString!=NULL_STRING)/*&&(uiConfiguration.isToolTips())*/&&(isMouseInside()))
		toolTipParent = this;
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

void UI_Object::setPosition(const Point& position)
{
	if(position == relativeRect.GetTopLeft())
		return;
	startRect.SetTopLeft(position);
	targetRect.SetTopLeft(position);
	relativeRect.SetTopLeft(position);
	setNeedRedrawMoved();
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
}

void UI_Object::setWidth(const unsigned int width) 
{
	if(relativeRect.GetWidth() == width)
		return;
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

const bool UI_Object::isTopItem() const
{
	return(parent==NULL);
}


void UI_Object::Show(const bool show)
{
	if((show)&&(!shown))
	{
		shown = true;
		setNeedRedrawMoved(true);
		process(); //?
	} 
	else if((!show)&&(shown))
	{
		setNeedRedrawMoved(true);
		setNeedRedrawMoved(false); // ~~
		shown = false;
	}
}

void UI_Object::setNeedRedrawMoved(const bool need_redraw)  // moved item
{
//	if(!isShown())
//		return;
	setNeedRedrawNotMoved(need_redraw);
	if((need_redraw)&&(getParent()))
	{
		getParent()->checkForChildrenOverlap(getRelativeRect());
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
	//if(!isShown())
	//	return;

	needRedraw = need_redraw;

	if(!need_redraw)
		return;

        UI_Object* tmp = children;

        if (tmp) {
                do {
                        tmp->setNeedRedrawNotMoved(needRedraw);
                        tmp = tmp->nextBrother;
                } while (tmp != children);
        }
}

void UI_Object::clearRedrawFlag()
{
//	if(!isShown())
//		return;
	if(needRedraw)
	{
		needRedraw = false;
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
		redrawnObjects++;
//		const_cast< UI_Object* > (this)->needRedraw=false;
		return(true);
	} else return(false);
}

void UI_Object::addMinTopLeftX(signed int dx)
{
	if((dx + min_top_left_x > (signed int)max_x)||(dx + min_top_left_x < 0))
		toLog("BUTTON MIN_TOP_LEFT_X out of range");
	min_top_left_x += dx;		
}

void UI_Object::addMinLeftY(signed int dy)
{
	if((dy + min_left_y > (signed int)max_y)||(dy + min_left_y < 0))
		toLog("BUTTON MIN_LEFT_Y out of range");
	min_left_y += dy;
}

void UI_Object::addMinRightY(signed int dy)
{
	if((dy + min_right_y > (signed int)max_y)||(dy + min_right_y < 0))
		toLog("BUTTON MIN_RIGHT_Y out of range");
	min_right_y += dy;
}

void UI_Object::addMinBottomLeftX(signed int dx)
{
	if((dx + min_bottom_left_x > (signed int)max_x)||(dx + min_bottom_left_x < 0))
		toLog("BUTTON MIN_BOTTOM_LEFT_X out of range");
	min_bottom_left_x += dx;		
}

void UI_Object::addMinTopRightX(signed int dx)
{
	if((dx + min_top_right_x > (signed int)max_x)||(dx + min_top_right_x < 0))
		toLog("BUTTON MIN_TOP_RIGHT_X out of range");
	min_top_right_x += dx;		
}

void UI_Object::addMinBottomRightX(signed int dx)
{
	if((dx + min_bottom_right_x > (signed int)max_x)||(dx + min_bottom_right_x < 0))
		toLog("BUTTON MIN_BOTTOM_RIGHT_X out of range");
	min_bottom_right_x += dx;		
}

void UI_Object::resetButton()
{
	currentButton=NULL;
	currentButtonHasAlreadyLeft=false;	
	currentButtonPressed=false;
}
	
UI_Theme UI_Object::theme;
UI_ToolTip* UI_Object::tooltip(NULL);
UI_Object* UI_Object::toolTipParent(NULL);
unsigned int UI_Object::max_x(0);
unsigned int UI_Object::max_y(0);
Point UI_Object::mouse(Point(0,0));

UI_EditField* UI_Object::editTextField(NULL);
unsigned int UI_Object::mouseType(0);

bool UI_Object::currentButtonPressed = false;
bool UI_Object::currentButtonHasAlreadyLeft = false;
UI_Button* UI_Object::currentButton = NULL;
UI_Window* UI_Object::currentWindow = NULL;
bool UI_Object::windowSelected = false;
unsigned int UI_Object::redrawnObjects(0);
std::list<std::string> UI_Object::msgList;
