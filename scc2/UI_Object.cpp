#include "UI_Object.h"

UI_Object::UI_Object(UI_Object* parent, const wxRect relativeRect, const wxRect maxSize)
{
	this->parent=0;
	this->relativeRect=relativeRect;
	disabledFlag=false;
	isFreeMove=true;
	this->maxSize=maxSize;
	doAdjustments=0;
	startRect=relativeRect;
	targetRect=relativeRect;
	absoluteCoord=wxPoint(0,0);

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




void UI_Object::setAbsoluteCoord(wxPoint p)
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

void UI_Object::adjustRelativeRect(wxRect edge)
{
	//targetRect = edge;

//	if((edge.x!=targetRect.x)&&(edge.width==targetRect.width))
//	{
// an Raender anpassen		
		if((edge.x<maxSize.x) && (!isFreeMove))
			edge.x=maxSize.x;
		if((edge.x+edge.width>maxSize.x+maxSize.width)&&(!isFreeMove))
			edge.width=maxSize.x+maxSize.width-edge.x;
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
        if((edge.y<maxSize.y) && (!isFreeMove))
            edge.y=maxSize.y;
        if((edge.y+edge.height>maxSize.y+maxSize.height)&&(!isFreeMove))
            edge.height=maxSize.y+maxSize.height-edge.y;
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
        if(((edge.x!=targetRect.x)&&(edge.x+edge.width<maxSize.x+maxSize.width)) // es ist ein neues Ziel
        ||((edge.x+edge.width>maxSize.x+maxSize.width)&&(targetRect.x+targetRect.width!=maxSize.x+maxSize.width)) // oder einfach ausserhalb der Begrenzung aber unser Ziel war nicht die Begrenzung
        ||((edge.x<maxSize.x)&&(targetRect.x!=maxSize.x)) // oder links ausserhalb der Begrenzung und altes Target hat nicht gepasst
        ||((edge.x!=targetRect.x)&&(isFreeMove))) // oder einfach freemove
            startRect.x=getLeftBound(); //Dann setze neuen Startpunkt
		
        if((edge.x+edge.width>maxSize.x+maxSize.width)&&(!isFreeMove))
            edge.x=maxSize.x+maxSize.width-edge.width;
    } else if(edge.x==targetRect.x)
    {
        if(((edge.width!=targetRect.width)&&(edge.x+edge.width<maxSize.x+maxSize.width))
        ||((edge.x+edge.width>maxSize.x+maxSize.width)&&(targetRect.x+targetRect.width!=maxSize.x+maxSize.width))
        ||((edge.width!=targetRect.width)&&(isFreeMove)))
            startRect.width=getWidth();
        if((edge.x+edge.width>maxSize.x+maxSize.width)&&(!isFreeMove))
            edge.width=maxSize.x+maxSize.width-edge.x;
    } //else ?
                                                                                                                                                            
/*    if(edge.height==targetRect.height)
    {
        if(((edge.y!=targetRect.y)&&(edge.y+edge.height<maxSize.y+maxSize.height))
        ||((edge.y+edge.height>maxSize.y+maxSize.height)&&(targetRect.y+targetRect.height!=maxSize.y+maxSize.height))
        ||((edge.y<maxSize.y)&&(targetRect.y!=maxSize.y)) )
//        ||((edge.y!=targetRect.y)&&(isFreeMove)))
            startRect.y=getUpperBound();
    //    if((edge.y+edge.height>maxSize.y+maxSize.height)&&(!isFreeMove))
      //      edge.y=maxSize.y+maxSize.height-edge.height;
    } else if(edge.y==targetRect.y)
    {
        if(((edge.height!=targetRect.height)&&(edge.y+edge.height<maxSize.y+maxSize.height))
        ||((edge.y+edge.height>maxSize.y+maxSize.height)&&(targetRect.y+targetRect.height!=maxSize.y+maxSize.height)) )
    //    ||((edge.height!=targetRect.height)&&(isFreeMove)))
            startRect.height=getHeight();
//        if((edge.y+edge.height>maxSize.y+maxSize.height)&&(!isFreeMove))
  //          edge.height=maxSize.y+maxSize.height-edge.y;
    } //else ... mmmh...

/*     if((edge.x<maxSize.x)||(isFreeMove))
        targetRect.x=maxSize.x;
    else targetRect.x=edge.x;

	if((edge.y<maxSize.y)||(isFreeMove))
      targetRect.y=maxSize.y;
    else targetRect.y=edge.y;*/
  
/* 	if((edge.x>=maxSize.x)||(isFreeMove))
    	targetRect.x=edge.x;
	else targetRect.x=maxSize.x;

	if((edge.y>=maxSize.y)||(isFreeMove))
    	targetRect.y=edge.y;
	else targetRect.y=maxSize.y;*/
	targetRect=edge;

//	targetRect.width=edge.width;
  //  targetRect.height=edge.height;

//    isFreeMove=0;
};

bool UI_Object::isTopItem()
{
	if(parent)
		return(false);
	else return(true);
};


wxPoint UI_Object::getAbsolutePosition()
{
	if(parent)
		return(absoluteCoord + relativeRect.GetPosition() + parent->getAbsolutePosition());
	else return(absoluteCoord + relativeRect.GetPosition());
};


wxPoint UI_Object::getAbsolutePosition2()
{
    if(parent)
	        return(parent->getAbsolutePosition());
	    else return(wxPoint(0,0));
};


wxPoint UI_Object::getRelativePosition()
{
    return(relativeRect.GetPosition());
};

wxRect UI_Object::getRelativeRect()
{
	return(relativeRect);
};

void UI_Object::setSize(wxSize size)
{
	relativeRect.SetSize(size);
};

wxSize UI_Object::getSize()
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
    if ((disabledFlag))
      return;

	if(doAdjustments==1)
	{
		adjustRelativeRect(wxRect(wxPoint(targetRect.GetPosition()),wxSize(targetRect.GetWidth(),lastItemY+25)));
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
                                                                                


void UI_Object::draw(wxDC* dc)
{
                                                                                
    // if hidden, hide children as well
    if (!shown)
		return;
		
	dc->SetBrush(*wxTRANSPARENT_BRUSH);
	dc->SetPen(wxPen(wxColour(255,0,0),3,wxSOLID));
    dc->DrawRectangle(wxRect(targetRect.GetPosition(),targetRect.GetSize()));
	dc->SetPen(wxPen(wxColour(255,255,255),2,wxSHORT_DASH));
    dc->DrawRectangle(wxRect(startRect.GetPosition(),startRect.GetSize()));

	
	/*dc->DrawRectangle(wxRect(maxSize.GetPosition()+getAbsolutePosition2(),maxSize.GetSize()));
 	
	dc->SetPen(wxPen(wxColour(255,255,255),1,wxSHORT_DASH));
	dc->DrawRectangle(wxRect(relativeRect.GetPosition()+getAbsolutePosition()-relativeRect.GetPosition(),relativeRect.GetSize()));
	
 	dc->SetPen(wxPen(wxColour(0,255,0),1,wxSHORT_DASH));
	dc->DrawRectangle(wxRect(targetRect.GetPosition()+getAbsolutePosition()-relativeRect.GetPosition(),targetRect.GetSize()));
 	
	dc->SetPen(wxPen(wxColour(255,0,0),1,wxSHORT_DASH));
	dc->DrawRectangle(wxRect(startRect.GetPosition()+getAbsolutePosition()-relativeRect.GetPosition(),startRect.GetSize()));*/
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
	startTime=wxDateTime::UNow();
};

long int UI_Object::getTimeStampMs(long int timeSpan)
{
	wxDateTime t=wxDateTime::UNow();
	wxTimeSpan ts=t.Subtract(startTime);
	return(timeSpan + ts.GetMilliseconds().ToLong());
};

bool UI_Object::isTimeSpanElapsed(long int time)
{
	if(time==0)
		return(true);
	wxDateTime t=wxDateTime::UNow();
	wxTimeSpan ts=t.Subtract(startTime);
	return(time < ts.GetMilliseconds().ToLong());
};


UI_Theme UI_Object::theme;

wxDateTime UI_Object::startTime;
