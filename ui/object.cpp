#include "object.hpp"
#include "configuration.hpp"


UI_Object::UI_Object(UI_Object* parent_object, const Rect relative_rect, const Size distance_bottom_right) :
	startRect(relative_rect),
	targetRect(relative_rect),
	distanceBottomRight(distance_bottom_right),

	filledHeight(relative_rect.GetHeight()),
//	notDrawRectList(),
	firstItemY(0),
	lastItemY(0),
	children(NULL),
	background(NULL),
	oldRect(),
	updateRelativePositions(false),

	shown(true),
	disabledFlag(false),

	min_top_left_x(0),
	min_left_y(0),
	min_right_y(0),
	min_bottom_left_x(0),
	min_top_right_x(0),
	min_bottom_right_x(0),	

	needRedraw(true),
//	tempSurface(NULL),
	prevBrother(this),
	nextBrother(this),
	parent(NULL),
	relativeRect(relative_rect),
//	lastRect(),
	doAdjustments(1),
	toolTipString(NULL_STRING)
{
	setParent(parent_object);
//	lastRect=getAbsoluteRect();
}

UI_Object::~UI_Object()
{
	removeFromFamily(); // !!!!!
	SDL_FreeSurface(background);
}

UI_Object& UI_Object::operator=(const UI_Object& object)
{
	startRect = object.startRect;
	targetRect = object.targetRect;
	distanceBottomRight = object.distanceBottomRight;
		
	firstItemY = object.firstItemY;
	lastItemY = object.lastItemY;
	filledHeight = object.filledHeight;
	children = object.children;
	background = object.background;
	oldRect = object.oldRect;
	updateRelativePositions = false;
//	notDrawRectList = object.notDrawRectList,
	shown = object.shown;
	disabledFlag = object.disabledFlag;

	min_top_left_x = object.min_top_left_x;
	min_left_y = object.min_left_y;
	min_right_y = object.min_right_y;
	min_bottom_left_x = object.min_bottom_left_x;
	min_top_right_x = object.min_top_right_x;
	min_bottom_right_x = object.min_bottom_right_x;
	needRedraw = object.needRedraw;
//	SDL_FreeSurface(tempSurface);
//DL_Crate
//	tempSurface = object.tempSurface;
	prevBrother = this; // !!
	nextBrother = this; // !!
	parent = NULL; // !!
	relativeRect = object.relativeRect;
//  lastRect = object.lastRect;
	doAdjustments = object.doAdjustments;
	toolTipString = object.toolTipString;
	
	setParent(object.parent);
	
	return(*this);
}

UI_Object::UI_Object(const UI_Object& object) :
	startRect( object.startRect ),
	targetRect( object.targetRect ),
	distanceBottomRight( object.distanceBottomRight ),
	
	filledHeight( object.filledHeight ),
//	notDrawRectList( object.notDrawRectList ),
	firstItemY( object.firstItemY ), 
	lastItemY( object.lastItemY ), 
	children( object.children ),
	background( object.background ),
	oldRect( object.oldRect),
	updateRelativePositions( false ),

	shown( object.shown ), 
	disabledFlag( object.disabledFlag ),

	min_top_left_x( object.min_top_left_x ),
	min_left_y( object.min_left_y ),
	min_right_y( object.min_right_y ),
	min_bottom_left_x( object.min_bottom_left_x ),
	min_top_right_x( object.min_top_right_x ),
	min_bottom_right_x( object.min_bottom_right_x ),

	needRedraw( object.needRedraw ),
//	tempSurface( object.tempSurface ),
	prevBrother( this ), // !!
	nextBrother( this ), // !!
	parent( NULL ), // !!
	relativeRect( object.relativeRect ),
//	lastRect( object.lastRect ),
	doAdjustments( object.doAdjustments ),
	toolTipString( object.toolTipString )
{ 
	setParent(object.parent);
}

/*
void UI_Object::addRect(const Rect& rect)
{
	notDrawRectList.push_back(rect);
}*/




/*
void UI_Object::getHasFocus()
{
	return(hasFocus)
}
const bool UI_Object::setNextFocus()
{
	UI_Object* tmp=children;  // process all children of gadget
   	if (tmp) {
	   	do {
			if(tmp->getHasFocus() && (tmp->nextBrother!=children))
			{
				if(tmp->nextBrother->setFocus())
				return(true);
			}
			tmp = tmp->nextBrother;
   		} while (tmp != children);
	}
	return(false);
}

const bool UI_Object::setFocus()
{
	if(canGetFocus)
	{
		hasFocus=true;
		return true;
	}
	else 
	{
		hasFocus=false;
		UI_Object* tmp=children;  // process all children of gadget
		if (tmp) {
			do {
				if(tmp->setFocus())
					return(true);
				tmp = tmp->nextBrother;
			} while (tmp != children);
		}
		return(false);
	}
}
*/

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
	if (parent) 
	{
		if (parent->children == this) 
		{
			if (nextBrother == this)  // the only child?
				parent->children = NULL;  // if so, parent now has no children
			else
				parent->children = nextBrother;  // next sibling is now the eldest
		}
	} 
	parent = NULL;
	if (nextBrother != this) {  // does this object have siblings?
		nextBrother->prevBrother = prevBrother;
		prevBrother->nextBrother = nextBrother;
	}
	
	nextBrother = prevBrother = this;
}

// Put gadget into a new family (removing from old one if needed first).
// See remove_from_family() for definition of what a family is.
//


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

//		children = child;
	}
}

// TODO
#if 0
void UI_Object::addRectToBeDrawn(Rect& lastRect, const Rect currentRect)
{
/*	rectlist[rectnumber].x = lastRect.x;rectlist[rectnumber].y = lastRect.y;
	rectlist[rectnumber].w = lastRect.width; rectlist[rectnumber].h = lastRect.height;
	
	lastRect = currentRect;
	if( lastRect.x < rectlist[rectnumber].x)
	{
		rectlist[rectnumber].w += rectlist[rectnumber].x - lastRect.x;
	rectlist[rectnumber].x = lastRect.x;
	} else
	if( lastRect.x > rectlist[rectnumber].x)	
		rectlist[rectnumber].w += lastRect.x - rectlist[rectnumber].x;
	if( lastRect.y < rectlist[rectnumber].y)	
	{
		rectlist[rectnumber].h += rectlist[rectnumber].y - lastRect.y;
		rectlist[rectnumber].y = lastRect.y;
	} else
	if( lastRect.y > rectlist[rectnumber].y)	
		rectlist[rectnumber].h += lastRect.y - rectlist[rectnumber].y;

	if( lastRect.width > rectlist[rectnumber].w )
		rectlist[rectnumber].w = lastRect.width;

	if( lastRect.height > rectlist[rectnumber].h )
		rectlist[rectnumber].h = lastRect.height;
	if((rectlist[rectnumber].x<max_x)&&(rectlist[rectnumber].y<max_y)&&(rectlist[rectnumber].x + rectlist[rectnumber].w>0)&&(rectlist[rectnumber].y + rectlist[rectnumber].h > 0))
	{
		if(rectlist[rectnumber].x +rectlist[rectnumber].w > max_x) rectlist[rectnumber].w = max_x - rectlist[rectnumber].x;
		if(rectlist[rectnumber].y +rectlist[rectnumber].h > max_y) rectlist[rectnumber].h = max_y - rectlist[rectnumber].y;
		if(rectlist[rectnumber].x < 0) {rectlist[rectnumber].w += rectlist[rectnumber].x;rectlist[rectnumber].x=0;}
		if(rectlist[rectnumber].y < 0) {rectlist[rectnumber].h += rectlist[rectnumber].y;rectlist[rectnumber].y=0;}
#ifdef _SCC_DEBUG
		if(rectnumber>=2999) {
			toLog("WARNING: (UI_Object::process): Value rectnumber out of range.");return;
		}
#endif
		rectnumber++;
	}*/
}
#endif
void UI_Object::process()
{
//	if (/*(disabledFlag)||*/(!isShown())) //~~
//		return;

//	while(!notDrawRectList.empty())
//		notDrawRectList.pop_front();

	if(doAdjustments==1)
	{
		adjustRelativeRect(Rect(Point(targetRect.GetTopLeft()), Size(targetRect.GetWidth(), filledHeight+25))); // TODO!
//		ARGH
		doAdjustments=2;
	}

	if(relativeRect.getSize() != targetRect.getSize())
		updateRelativePositions=true;
	else updateRelativePositions=false;

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
	
#if 0
	if((lastRect!=getAbsoluteRect())||(needRedraw))
	{
		if(needRedraw) 
			needRedraw=true;
		else needRedraw=true;
/*		if(lastRect!=getAbsoluteRect())
		{
			tempRect.x=getAbsolutePosition().x;
			tempRect.y=getAbsolutePosition().y;
			tempRect.w=getWidth();
			tempRect.h=getHeight();
			SDL_GetClipRect(tempSurface, &tempRect);
		}*/
		addRectToBeDrawn(lastRect, getAbsoluteRect());

	}
#endif
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
	}
	while((tmp!=children)&&(result==NULL));
	return(result);
}

void UI_Object::adjustButtonPlacementSize()
{ }

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
	}
	while((tmp!=children)&&(result==NULL));
	return(result);
}

void UI_Object::draw(DC* dc) const
{
	// if hidden, hide children as well
//	if (!isShown())
//		return;
/*	
		SDL_Rect temp;
		if(background!=NULL)
		{
			temp.x = oldRect.GetLeft();
			temp.y = oldRect.GetTop();
			temp.w = oldRect.GetWidth();
			temp.h = oldRect.GetHeight();
			SDL_BlitSurface(background, NULL, dc->GetSurface(), &temp); // erase old
			SDL_FreeSurface(background);
			const_cast< UI_Object* > (this)->background = NULL;
		}
		const_cast< UI_Object* > (this)->background = SDL_CreateRGBSurface(SDL_SWSURFACE, getWidth(), getHeight(), 24, 0, 0, 0, 0);
		temp.x = getAbsoluteLeftBound();
		temp.y = getAbsoluteUpperBound();
		temp.w = getWidth();
		temp.h = getHeight();	
		SDL_BlitSurface(dc->GetSurface(), &temp, background, NULL); // save new background
	}*/

//	if(getParent())
//	{
//	dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
//	dc->DrawEmptyRectangle(getParent()->getAbsolutePosition() + targetRect.GetTopLeft(), targetRect.GetSize());
//	dc->DrawEmptyRectangle(getParent()->getAbsolutePosition() + startRect.GetTopLeft(), startRect.GetSize());
//	}

	UI_Object* tmp = children;
	
	if (tmp) {
		do {
			tmp->draw(dc);
			tmp = tmp->nextBrother;
		} while (tmp != children);
	}
}

/*void UI_Object::setPosition(const Point& position)
{
	if(position == relativeRect.GetTopLeft())
		return;
	startRect.SetTopLeft(startRect.GetTopLeft() + (position - relativeRect.GetTopLeft()));
	targetRect.SetTopLeft(targetRect.GetTopLeft() + (position - relativeRect.GetTopLeft()));
	relativeRect.SetTopLeft(position);
	setNeedRedraw();
}*/

void UI_Object::setPosition(const Point& position)
//void UI_Object::jumpToPosition(const Point& position)
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
	if(relativeRect.GetHeight() == height)
		return;
	relativeRect.SetHeight(height);
	startRect.SetHeight(height);
	targetRect.SetHeight(height);
	setNeedRedrawNotMoved(); // TODO wenns kleiner wird
}

void UI_Object::setWidth(const unsigned int width) 
{
	if(relativeRect.GetWidth() == width)
		return;
	relativeRect.SetWidth(width);
	startRect.SetWidth(width);
	targetRect.SetWidth(width);
	setNeedRedrawNotMoved(); // TODO Wenns kleiner wird!
}

void UI_Object::setSize(const Size size)
{
	if(relativeRect.GetSize() == size)
		return;
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
	

/*const Point UI_Object::getAbsolutePosition() const	
{
	if(parent)
		return(relativeRect.GetTopLeft() + parent->getAbsolutePosition());
	else return(relativeRect.GetTopLeft());
}*/

const bool UI_Object::isTopItem() const
{
	if(parent)
		return(false);
	else return(true);
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
		getParent()->checkForChildrenOverlap(oldRect);
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

const bool UI_Object::isUpdateRelativePositions() const
{
	return(updateRelativePositions());
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

const unsigned long int UI_Object::getTimeStampMs(const unsigned long int timeSpan)
{
	return(timeSpan + SDL_GetTicks() - startTime);
}

const bool UI_Object::isTimeSpanElapsed(const unsigned long int timeSpan)
{
	if(timeSpan==0)
		return(true);
	return(timeSpan < SDL_GetTicks());
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
long unsigned int UI_Object::startTime(0);
//SDL_Rect UI_Object::rectlist[3000];
//unsigned int UI_Object::rectnumber(0);
UI_ToolTip* UI_Object::tooltip(NULL);
UI_Object* UI_Object::toolTipParent(NULL);
unsigned int UI_Object::max_x(0);
unsigned int UI_Object::max_y(0);
Point UI_Object::mouse(Point(0,0));

UI_EditField* UI_Object::editTextField(NULL);
//UI_EndRunDialog* UI_Object::endRunDialog(NULL);
unsigned int UI_Object::mouseType(0);

bool UI_Object::currentButtonPressed = false;
bool UI_Object::currentButtonHasAlreadyLeft = false;
UI_Button* UI_Object::currentButton = NULL;
UI_Window* UI_Object::currentWindow = NULL;
bool UI_Object::windowSelected = false;
unsigned int UI_Object::redrawnObjects(0);
std::list<std::string> UI_Object::msgList;
