#include "object.hpp"
#include "../core/configuration.hpp"


UI_Object::UI_Object(UI_Object* parent_object, const Rect relative_rect, const Rect max_rect):
	startRect(relative_rect),
	targetRect(relative_rect),

	filledHeight(relative_rect.GetHeight()),
	children(NULL),
//	notDrawRectList(),
	shown(true),
	disabledFlag(false),

	min_top_left_x(0),
	min_left_y(0),
	min_right_y(0),
	min_bottom_left_x(0),
	min_top_right_x(0),
	min_bottom_right_x(0),	


//	needRedraw(true),
	isFreeMove(true),
//	tempSurface(NULL),
	prevBrother(this),
	nextBrother(this),
	parent(0),
	relativeRect(relative_rect),
//	lastRect(),
	maxRect(max_rect),
	doAdjustments(1),
	toolTipString(NULL_STRING)
{
	setParent(parent_object);
//	lastRect=getAbsoluteRect();
}

UI_Object::~UI_Object()
{
	removeFromFamily(); // !!!!!
}

UI_Object& UI_Object::operator=(const UI_Object& object)
{
	startRect = object.startRect;
	targetRect = object.targetRect;
	filledHeight = object.filledHeight;
	children = object.children;
//	notDrawRectList = object.notDrawRectList,
	shown = object.shown;
	disabledFlag = object.disabledFlag;

	min_top_left_x = object.min_top_left_x;
	min_left_y = object.min_left_y;
	min_right_y = object.min_right_y;
	min_bottom_left_x = object.min_bottom_left_x;
	min_top_right_x = object.min_top_right_x;
	min_bottom_right_x = object.min_bottom_right_x;
//	needRedraw = object.needRedraw;
	isFreeMove = object.isFreeMove;
//	SDL_FreeSurface(tempSurface);
//DL_Crate
//	tempSurface = object.tempSurface;
	prevBrother = this; // !!
	nextBrother = this; // !!
	parent = NULL; // !!
	relativeRect = object.relativeRect;
//  lastRect = object.lastRect;
	maxRect = object.maxRect;
	doAdjustments = object.doAdjustments;
	toolTipString = object.toolTipString;
	
	setParent(object.parent);
	
	return(*this);
}

UI_Object::UI_Object(const UI_Object& object) :
	startRect( object.startRect ),
	targetRect( object.targetRect ),
	filledHeight( object.filledHeight ),
	children( object.children ),
//	notDrawRectList( object.notDrawRectList ),
	shown( object.shown ), 
	disabledFlag( object.disabledFlag ),

	min_top_left_x( object.min_top_left_x ),
	min_left_y( object.min_left_y ),
	min_right_y( object.min_right_y ),
	min_bottom_left_x( object.min_bottom_left_x ),
	min_top_right_x( object.min_top_right_x ),
	min_bottom_right_x( object.min_bottom_right_x ),

//	needRedraw( object.needRedraw ),
	isFreeMove( object.isFreeMove ),
//	tempSurface( object.tempSurface ),
	prevBrother( this ), // !!
	nextBrother( this ), // !!
	parent( NULL ), // !!
	relativeRect( object.relativeRect ),
//	lastRect( object.lastRect ),
	maxRect( object.maxRect ),
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


const Point UI_Object::getAbsolutePosition() const	
{
	if(parent)
		return(relativeRect.GetTopLeft() + parent->getAbsolutePosition());
	else return(relativeRect.GetTopLeft());
}

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


UI_Object* UI_Object::getPrevBrother() const {
	return(prevBrother);
}

UI_Object* UI_Object::getNextBrother() const {
	return(nextBrother);
}

UI_Object* UI_Object::getParent() const {
	return(parent);
}

UI_Object* UI_Object::getChildren() const {
	return(children);
}

void UI_Object::updateToolTip(const eString tool_tip) {
	toolTipString = tool_tip;
}

const eString UI_Object::getToolTipString() const {
	return(toolTipString);
}

const unsigned int UI_Object::getDoAdjustments() const {
	return(doAdjustments);
}

void UI_Object::setDoAdjustments(const unsigned int do_adjustments) {
	doAdjustments=do_adjustments;
}

const Rect& UI_Object::getMaxRect() const {
	return(maxRect);
}

const unsigned int UI_Object::getTargetWidth() const {
	return(targetRect.GetWidth());
}

const unsigned int UI_Object::getTargetHeight() const {
	return(targetRect.GetHeight());
}

void UI_Object::setFreeMove(const bool is_free_move) {
	isFreeMove=is_free_move;
}

void UI_Object::adjustRelativeRect(Rect edge)
{
	if(edge != targetRect)
	{
// an Raender anpassen		
		if((edge.GetLeft()<maxRect.GetLeft()) && (!isFreeMove))
			edge.SetLeft(maxRect.GetLeft());
			
		if((edge.GetRight() > maxRect.GetRight())&&(!isFreeMove)&&(maxRect.GetRight() > edge.GetLeft()))
			edge.SetRight( maxRect.GetRight() );
			
		if((edge.GetTop()<maxRect.GetTop()) && (!isFreeMove))
			edge.SetTop(maxRect.GetTop());
			
		if((edge.GetBottom() > maxRect.GetBottom())&&(!isFreeMove)&&(maxRect.GetBottom() > edge.GetBottom()))
			edge.SetBottom(maxRect.GetBottom());
		 
// neues Ziel?

		if((edge.GetLeft()!=targetRect.GetLeft())||(edge.GetRight()!=targetRect.GetRight()))
		{
			startRect.SetLeft(getRelativeLeftBound());
			
//		if(edge.GetRight()!=targetRect.GetRight())
			startRect.SetRight(getRelativeRightBound());
		}
			
		if((edge.GetTop()!=targetRect.GetTop())||(edge.GetHeight()!=targetRect.GetHeight()))
		{
			startRect.SetTop(getRelativeUpperBound());
			
//		if(edge.GetHeight()!=targetRect.GetHeight())
			startRect.SetBottom(getRelativeLowerBound());
		}
		

		
/*
	
	
//	if(edge.width==-99)
  //	  edge.width=targetRect.width;
	//if(edge.height==-99)
	  //  edge.height=targetRect.height;
//Only 1 change (X OR WIDTH, Y OR HEIGHT) is allowed!
	if(edge.width==targetRect.width) // nur x ist unterschiedlich
	{
		if(((edge.x!=targetRect.x)&&(edge.x+edge.width<maxRect.x+maxRect.width)) // es ist ein neues Ziel
		||((edge.x+edge.width>maxRect.x+maxRect.width)&&(targetRect.x+targetRect.width!=maxRect.x+maxRect.width)) // oder einfach ausserhalb der Begrenzung aber unser Ziel war nicht die Begrenzung
		||((edge.x<maxRect.x)&&(targetRect.x!=maxRect.x)) // oder links ausserhalb der Begrenzung und altes Target hat nicht gepasst
		||((edge.x!=targetRect.x)&&(isFreeMove))) // oder einfach freemove
			startRect.x=getLeftBound(); //Dann setze neuen Startpunkt
		
		if((edge.x+edge.width>maxRect.x+maxRect.width)&&(!isFreeMove))
			edge.x=maxRect.x+maxRect.width-edge.width;
	} else if(edge.x==targetRect.x)
	{
		if(((edge.width!=targetRect.width)&&(edge.x+edge.width<maxRect.x+maxRect.width))
		||((edge.x+edge.width>maxRect.x+maxRect.width)&&(targetRect.x+targetRect.width!=maxRect.x+maxRect.width))
		||((edge.width!=targetRect.width)&&(isFreeMove)))
			startRect.width=getWidth();
		if((edge.x+edge.width>maxRect.x+maxRect.width)&&(!isFreeMove))
			edge.width=maxRect.x+maxRect.width-edge.x;
	} //else ?
																																							
	if(edge.height==targetRect.height)
	{
		if(((edge.y!=targetRect.y)&&(edge.y+edge.height<maxRect.y+maxRect.height))
		||((edge.y+edge.height>maxRect.y+maxRect.height)&&(targetRect.y+targetRect.height!=maxRect.y+maxRect.height))
		||((edge.y<maxRect.y)&&(targetRect.y!=maxRect.y)) )
//		||((edge.y!=targetRect.y)&&(isFreeMove)))
			startRect.y=getUpperBound();
	//	if((edge.y+edge.height>maxRect.y+maxRect.height)&&(!isFreeMove))
	  //	  edge.y=maxRect.y+maxRect.height-edge.height;
	} else if(edge.y==targetRect.y)
	{
		if(((edge.height!=targetRect.height)&&(edge.y+edge.height<maxRect.y+maxRect.height))
		||((edge.y+edge.height>maxRect.y+maxRect.height)&&(targetRect.y+targetRect.height!=maxRect.y+maxRect.height)) )
	//	||((edge.height!=targetRect.height)&&(isFreeMove)))
			startRect.height=getHeight();
//		if((edge.y+edge.height>maxRect.y+maxRect.height)&&(!isFreeMove))
  //		  edge.height=maxRect.y+maxRect.height-edge.y;
	} //else ... mmmh...

	 if((edge.x<maxRect.x)||(isFreeMove))
		targetRect.x=maxRect.x;
	else targetRect.x=edge.x;

	if((edge.y<maxRect.y)||(isFreeMove))
	  targetRect.y=maxRect.y;
	else targetRect.y=edge.y;*/
  
/* 	if((edge.x>=maxRect.x)||(isFreeMove))
		targetRect.x=edge.x;
	else targetRect.x=maxRect.x;

	if((edge.y>=maxRect.y)||(isFreeMove))
		targetRect.y=edge.y;
	else targetRect.y=maxRect.y;*/
	targetRect=edge;
	}

//	targetRect.width=edge.width;
  //  targetRect.height=edge.height;

	isFreeMove=0;
}

const bool UI_Object::isTopItem() const
{
	if(parent)
		return(false);
	else return(true);
}


void UI_Object::Show(const bool show)
{
	shown = show;
	if(show)
		process();
}

void UI_Object::Hide(const bool hide)
{
	shown=!hide;
}

void UI_Object::Enable(const bool enable)
{
	disabledFlag = !enable;
}

void UI_Object::Disable(const bool disable)
{
	disabledFlag = disable;
}

const bool UI_Object::isShown() const
{
	return(shown);
}

const bool UI_Object::isDisabled() const
{
	return(disabledFlag);
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
	if (/*(disabledFlag)||*/(!shown)) //~~
		return;
//	needRedraw=false;

//	while(!notDrawRectList.empty())
//		notDrawRectList.pop_front();

	if(doAdjustments==1)
	{
		adjustRelativeRect(Rect(Point(targetRect.GetTopLeft()), Size(targetRect.GetWidth(), filledHeight+25))); // TODO!
//		ARGH
		doAdjustments=2;
	}	

	relativeRect.move(startRect, targetRect);
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
	
	if((toolTipString!=NULL_STRING)&&(configuration.isTooltips())&&(isMouseInside()))
		toolTipParent = this;
}

UI_Object* UI_Object::checkTooltip()
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
		result = tmp->checkTooltip();
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

const bool UI_Object::isMouseInside() const
{
	return(getAbsoluteRect().Inside(mouse));
}

/*const bool UI_Object::doesNeedRedraw() const
{
	return(needRedraw);
}

void UI_Object::setNeedRedraw(const bool need_redraw)
{
	this->needRedraw=need_redraw;
}*/

void UI_Object::draw(DC* dc) const
{
	// if hidden, hide children as well
	if (!shown)
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
	startRect.SetTopLeft(startRect.GetTopLeft() + (position - relativeRect.GetTopLeft()));
	targetRect.SetTopLeft(targetRect.GetTopLeft() + (position - relativeRect.GetTopLeft()));
	relativeRect.SetTopLeft(position);
}

void UI_Object::jumpToPosition(const Point& position)
{
	startRect.SetTopLeft(position);
	targetRect.SetTopLeft(position);
	relativeRect.SetTopLeft(position);
}

void UI_Object::assignStartTime()
{
	startTime = SDL_GetTicks();
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
UI_Tooltip* UI_Object::tooltip(NULL);
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


