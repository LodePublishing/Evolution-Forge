#include "UI_Object.h"
#include<sys/timeb.h>
#include<sys/types.h>

UI_Object::UI_Object(UI_Object* parent, const Rect relativeRect, const Rect maxRect)
{
	this->parent=0;
	this->relativeRect=relativeRect;
	disabledFlag=false;
	isFreeMove=true;
	this->maxRect=maxRect;
	doAdjustments=0;
	startRect=relativeRect;
	targetRect=relativeRect;
	absoluteCoord=Point(0,0);
	startTime=0;

	shown=true;
	nextBrother = prevBrother = this;
	children=0;
	setParent(parent);
};

UI_Object::~UI_Object()
{
	// destroy children
};

UI_Object* UI_Object::getPrevBrother()
{
	return(prevBrother);
};

UI_Object* UI_Object::getNextBrother()
{
	return(nextBrother);
};

UI_Object* UI_Object::getParent()
{
	return(parent);
};

UI_Object* UI_Object::getChildren()
{
	return(children);
};


void UI_Object::setAbsoluteCoord(Point p)
{
	absoluteCoord=p;
};

int UI_Object::getDoAdjustments()
{
	return(doAdjustments);
};

void UI_Object::setDoAdjustments(const int doAdjustments)
{
	this->doAdjustments=doAdjustments;
};

const Rect UI_Object::getMaxRect()
{
	return(maxRect);
};

int UI_Object::getTargetWidth()
{
    return(targetRect.width);
};
                                                                                                                                                            
int UI_Object::getTargetHeight()
{
    return(targetRect.height);
};

void UI_Object::setFreeMove(const bool isFreeMove)
{
	this->isFreeMove=isFreeMove;
};

void UI_Object::forceSmallY(const int dy)
{
	relativeRect.y+=dy;relativeRect.height-=dy;
	maxRect.y+=dy;maxRect.height-=dy;
    startRect=relativeRect;
    targetRect=relativeRect;
};

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
//	};

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
};

bool UI_Object::isTopItem()
{
	if(parent)
		return(false);
	else return(true);
};


Point UI_Object::getAbsolutePosition()
{
	if(parent)
		return(absoluteCoord + relativeRect.GetPosition() + parent->getAbsolutePosition());
	else return(absoluteCoord + relativeRect.GetPosition());
};


Point UI_Object::getAbsolutePosition2()
{
    if(parent)
	        return(parent->getAbsolutePosition());
	    else return(Point(0,0));
};


Point UI_Object::getRelativePosition()
{
    return(relativeRect.GetPosition());
};

Rect UI_Object::getRelativeRect()
{
	return(relativeRect);
};

void UI_Object::setSize(Size size)
{
	relativeRect.SetSize(size);
};

Size UI_Object::getSize()
{
    return(relativeRect.GetSize());
};

int UI_Object::getRelativeUpperBound()
{
    return(relativeRect.y);
};

int UI_Object::getRelativeLowerBound()
{
    return(relativeRect.y+relativeRect.height);
};

int UI_Object::getRelativeLeftBound()
{
    return(relativeRect.x);
};

int UI_Object::getRelativeRightBound()
{
    return(relativeRect.x+relativeRect.width);
};

int UI_Object::getHeight()
{
    return(relativeRect.height);
};

int UI_Object::getWidth()
{
	return(relativeRect.width);
};

void UI_Object::Show(const bool show)
{
	shown=show;
};

void UI_Object::Hide(const bool show)
{
	shown=show;
};

bool UI_Object::isShown()
{
	return(shown);
};



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
};

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
};

void UI_Object::process()
{
    if ((disabledFlag)||(!shown)) //~~
      return;

	if(doAdjustments==1)
	{
		adjustRelativeRect(Rect(Point(targetRect.GetPosition()),Size(targetRect.GetWidth(),lastItemY+25)));
		doAdjustments=2;
	}	
	move(relativeRect.x,		startRect.x,		targetRect.x);
	move(relativeRect.y,		startRect.y,		targetRect.y);
	move(relativeRect.width,	startRect.width,	targetRect.width);
	move(relativeRect.height,	startRect.height,	targetRect.height);

	min_bottom_x = min_left_y = min_top_x = min_right_y = 0;

    UI_Object* tmp=children;  // process all children of gadget
    if (tmp) {
        do {
            tmp->process();
            tmp = tmp->nextBrother;
                                                                                
        } while (tmp != children);
    }
}
                                                                                


void UI_Object::draw(DC* dc)
{
                                                                                
    // if hidden, hide children as well
    if (!shown)
		return;
		
/*	dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
	dc->SetPen(Pen(dc->doColor(255,0,0),3,SOLID_PEN_STYLE));
    dc->DrawRectangle(Rect(targetRect.GetPosition(),targetRect.GetSize()));
	dc->SetPen(Pen(dc->doColor(255,255,255),2,SHORT_DASH_PEN_STYLE));
    dc->DrawRectangle(Rect(startRect.GetPosition(),startRect.GetSize()));*/

	
	/*dc->DrawRectangle(Rect(maxRect.GetPosition()+getAbsolutePosition2(),maxRect.GetSize()));
 	
	dc->SetPen(Pen(Color(255,255,255),1,SHORT_DASH));
	dc->DrawRectangle(Rect(relativeRect.GetPosition()+getAbsolutePosition()-relativeRect.GetPosition(),relativeRect.GetSize()));
	
 	dc->SetPen(Pen(Color(0,255,0),1,SHORT_DASH));
	dc->DrawRectangle(Rect(targetRect.GetPosition()+getAbsolutePosition()-relativeRect.GetPosition(),targetRect.GetSize()));
 	
	dc->SetPen(Pen(Color(255,0,0),1,SHORT_DASH));
	dc->DrawRectangle(Rect(startRect.GetPosition()+getAbsolutePosition()-relativeRect.GetPosition(),startRect.GetSize()));*/
    UI_Object* cur;
	
    if (children) {
        cur = children;
        do {
            cur->draw(dc);
            cur = cur->nextBrother;
        } while (cur != children);
    }
};

void UI_Object::assignStartTime()
{
    struct timeb tms;time_t ts;
    ftime(&tms);time(&ts);
    startTime = tms.millitm + ts*1000;
};

long int UI_Object::getTimeStampMs(long int timeSpan)
{
	struct timeb tms;time_t ts;
    ftime(&tms);time(&ts);
	long t = tms.millitm + ts*1000 - startTime;
	return(timeSpan + t);
};

bool UI_Object::isTimeSpanElapsed(long int timeSpan)
{
	struct timeb tms;time_t ts;
	if(time==0)
		return(true);
    ftime(&tms);time(&ts);
    long t = tms.millitm + ts*1000 - startTime;
	return(timeSpan < t);
};

UI_Theme UI_Object::theme;
long UI_Object::startTime;

