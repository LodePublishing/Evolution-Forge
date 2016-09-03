#include "object.hpp"
#include <sys/time.h>

void UI_Object::move(int& x, const int sx, const int tx)
{
    x+=((x>tx)?-1:0)+((x<tx)?1:0);
	x+=(int)((x>sx)?(2*(x-sx)<(tx-sx)?(double)(x-sx)/2:(double)(tx-x)/2):(2*(x-sx)>(tx-sx)?(double)(x-sx)/1.5:(double)(tx-x)/1.5));
}

void UI_Object::move(Point& p, const Point sp, const Point tp)
{
	move(p.x, sp.x, tp.x);
	move(p.y, sp.y, tp.y);
}

void UI_Object::move(Rect& r, const Rect sr, const Rect tr)
{
	move(r.x, sr.x, tr.x);
	move(r.y, sr.y, tr.y);
	move(r.width, sr.width, tr.width);
	move(r.height, sr.height, tr.height);
}

UI_Object::UI_Object(UI_Object* parent, const Rect relativeRect, const Rect maxRect)
{
	this->parent=0;
	this->relativeRect=relativeRect;
	disabledFlag=false;
	isFreeMove=true;
	this->maxRect=maxRect;
	doAdjustments=1;
	
	startRect=relativeRect;
	targetRect=relativeRect;
	lastItemY=relativeRect.height;
	
	startTime=0;

	toolTip="";
	shown=true;
	nextBrother = prevBrother = this;
	children=0;
	min_bottom_x = min_left_y = min_top_x = min_right_y = 0;
	setParent(parent);
	needRedraw=true;
	lastRect=getAbsoluteRect();
}

UI_Object::~UI_Object()
{
	// destroy children
}

UI_Object* UI_Object::getPrevBrother() const
{
	return(prevBrother);
}

UI_Object* UI_Object::getNextBrother() const
{
	return(nextBrother);
}

UI_Object* UI_Object::getParent() const
{
	return(parent);
}

UI_Object* UI_Object::getChildren() const
{
	return(children);
}

void UI_Object::updateToolTip(const string& toolTip)
{
	this->toolTip=toolTip;
}

const int UI_Object::getDoAdjustments() const
{
	return(doAdjustments);
}

void UI_Object::setDoAdjustments(const int doAdjustments)
{
	this->doAdjustments=doAdjustments;
}

const Rect UI_Object::getMaxRect() const
{
	return(maxRect);
}

const int UI_Object::getTargetWidth() const
{
    return(targetRect.width);
}

const int UI_Object::getTargetHeight() const
{
    return(targetRect.height);
}

void UI_Object::setFreeMove(const bool isFreeMove)
{
	this->isFreeMove=isFreeMove;
}

void UI_Object::forceSmallY(const int dy)
{
	relativeRect.y+=dy;relativeRect.height-=dy;
	maxRect.y+=dy;maxRect.height-=dy;
    startRect=relativeRect;
    targetRect=relativeRect;
}

void UI_Object::adjustRelativeRect(Rect edge)
{
	//targetRect = edge;

//	if((edge.x!=targetRect.x)&&(edge.width==targetRect.width))
//	{
// an Raender anpassen		
		if((edge.x<maxRect.x) && (!isFreeMove))
			edge.x=maxRect.x;
		if((edge.x+edge.width>maxRect.x+maxRect.width)&&(!isFreeMove))
			edge.width=maxRect.x+maxRect.width-edge.x;
// neues Ziel?		
		if(edge.x!=targetRect.x)
			startRect.x=getRelativeLeftBound();	
		if(edge.width!=targetRect.width)
			startRect.width=getWidth();
//	} else
//	if((edge.x==targetRect.x)&&(edge.width!=targetRect.width))
//	{
//	}

// an Raender anpassen
        if((edge.y<maxRect.y) && (!isFreeMove))
            edge.y=maxRect.y;
        if((edge.y+edge.height>maxRect.y+maxRect.height)&&(!isFreeMove))
            edge.height=maxRect.y+maxRect.height-edge.y;
// neues Ziel?
        if(edge.y!=targetRect.y)
            startRect.y=getRelativeUpperBound();
      if(edge.height!=targetRect.height)
            startRect.height=getHeight();
/*
	
	
//	if(edge.width==-99)
  //      edge.width=targetRect.width;
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
//        ||((edge.y!=targetRect.y)&&(isFreeMove)))
            startRect.y=getUpperBound();
    //    if((edge.y+edge.height>maxRect.y+maxRect.height)&&(!isFreeMove))
      //      edge.y=maxRect.y+maxRect.height-edge.height;
    } else if(edge.y==targetRect.y)
    {
        if(((edge.height!=targetRect.height)&&(edge.y+edge.height<maxRect.y+maxRect.height))
        ||((edge.y+edge.height>maxRect.y+maxRect.height)&&(targetRect.y+targetRect.height!=maxRect.y+maxRect.height)) )
    //    ||((edge.height!=targetRect.height)&&(isFreeMove)))
            startRect.height=getHeight();
//        if((edge.y+edge.height>maxRect.y+maxRect.height)&&(!isFreeMove))
  //          edge.height=maxRect.y+maxRect.height-edge.y;
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
	shown=show;
}

void UI_Object::Hide(const bool show)
{
	shown=show;
}

const bool UI_Object::isShown() const
{
	return(shown);
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
            if (nextBrother == this)  // an only child?
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
    }
}

void UI_Object::process()
{
//    if ((disabledFlag)||(!shown)) //~~
  //    return;

	if(doAdjustments==1)
	{
		adjustRelativeRect(Rect(Point(targetRect.GetPosition()),Size(targetRect.GetWidth(),lastItemY+25)));
		doAdjustments=2;
	}	

	move(relativeRect,		startRect,		targetRect);

	if((lastRect!=getAbsoluteRect())||(needRedraw))
	{
		needRedraw=true;
		rectlist[rectnumber].x = lastRect.x;rectlist[rectnumber].y = lastRect.y;
		rectlist[rectnumber].w = lastRect.width; rectlist[rectnumber].h = lastRect.height;
		
		lastRect = getAbsoluteRect();
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
		if((rectlist[rectnumber].x<1280)&&(rectlist[rectnumber].y<1024)&&(rectlist[rectnumber].x + rectlist[rectnumber].w>0)&&(rectlist[rectnumber].y + rectlist[rectnumber].h > 0))
		{
			if(rectlist[rectnumber].x +rectlist[rectnumber].w > 1280) rectlist[rectnumber].w = 1280 - rectlist[rectnumber].x;
			if(rectlist[rectnumber].y +rectlist[rectnumber].h > 1024) rectlist[rectnumber].h = 1024 - rectlist[rectnumber].y;
			if(rectlist[rectnumber].x < 0) {rectlist[rectnumber].w += rectlist[rectnumber].x;rectlist[rectnumber].x=0;}
			if(rectlist[rectnumber].y < 0) {rectlist[rectnumber].h += rectlist[rectnumber].y;rectlist[rectnumber].y=0;}
#ifdef _SCC_DEBUG
			if(rectnumber>=2999) {
		        toLog("WARNING: (UI_Object::process): Value rectnumber out of range.");return;
		    }
#endif
			rectnumber++;
			
		}
	}

	min_bottom_x = min_left_y = min_top_x = min_right_y = 0;

    UI_Object* tmp=children;  // process all children of gadget
    if (tmp) {
        do {
            tmp->process();
            tmp = tmp->nextBrother;
                                                                                
        } while (tmp != children);
    }
}
                                                                                
const bool UI_Object::isMouseInside() const
{
	Point p = controls.getCurrentPosition(getAbsolutePosition());
	if((p.x>=0)&&(p.x<getWidth())&&(p.y>=0)&&(p.y<getHeight()))
		return(true);
	else return(false);
}

const bool UI_Object::doesNeedRedraw() const
{
	return(needRedraw);
}

void UI_Object::setNeedRedraw(const bool needRedraw)
{
	this->needRedraw=needRedraw;
}

void UI_Object::draw(DC* dc) const
{
    // if hidden, hide children as well
    if (!shown)
		return;

    UI_Object* tmp=children;
	
    if (tmp) {
        do {
            tmp->draw(dc);
            tmp = tmp->nextBrother;
        } while (tmp != children);
    }

	
	if(isMouseInside())
		maybeShowToolTip(dc);
	
}

void UI_Object::maybeShowToolTip(DC* dc) const
{
	if(toolTip.size()==0)
		return;
	dc->SetPen(*theme.lookUpPen(RECTANGLE_PEN));
	dc->SetBrush(*theme.lookUpBrush(TOOLTIP_BRUSH));
	dc->SetTextForeground(*theme.lookUpColor(TEXT_COLOUR));
	dc->SetFont(theme.lookUpFont(SMALL_ITALICS_BOLD_FONT));
	int dx, dy;
	dc->GetTextExtent(toolTip, &dx, &dy);
	dc->DrawRectangle(getAbsolutePosition()+Point(16,16), Size(dx+16, dy));
	dc->DrawText(toolTip, getAbsolutePosition()+Point(24,20));
}


void UI_Object::assignStartTime()
{
	timeval tim;
    gettimeofday(&tim, NULL);
    startTime=tim.tv_sec*1000000+tim.tv_usec;
}

const long int UI_Object::getTimeStampMs(const long int timeSpan)
{
    timeval tim;
    gettimeofday(&tim, NULL);
    long int t = tim.tv_sec * 1000000 + tim.tv_usec - startTime;
	return(timeSpan + t);
}

const bool UI_Object::isTimeSpanElapsed(const long int timeSpan)
{
	if(timeSpan==0)
		return(true);
    timeval tim;
    gettimeofday(&tim, NULL);
    long int t = tim.tv_sec * 1000000 + tim.tv_usec - startTime;
	return(timeSpan < t);
}

UI_Theme UI_Object::theme;
long int UI_Object::startTime;
SDL_Rect UI_Object::rectlist[3000];
int UI_Object::rectnumber;

