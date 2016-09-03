#include "UI_Window.h"
#include "UI_Theme.h"

void UI_Window::updateRectangles()
{
// ------ PROCESSING
                                                                                                                                                            
//  if(isTabbed)
//      forceSmallY(21);
//	setRelativeRect(*theme.lookUpRect(window));
	setMaxRect(theme.lookUpMaxRect(window, windowNumber)); //~~
	setFreeMove();
	adjustRelativeRect(theme.lookUpRect(window, windowNumber));
//    originalRect=getRelativeRect(); ?
};

UI_Window::UI_Window(UI_Object* parent, eString titleString, eWindow window, const int windowNumber, const bool isScrollable, const bool isAutoAdjust, const bool isTabbed, Rect clientArea):UI_Object(parent, theme.lookUpRect(window, windowNumber), theme.lookUpMaxRect(window, windowNumber))
{
// ------ INITIALIZATION ------
	titleParameter=0;
	changedFlag=false;
	this->isAutoAdjust=isAutoAdjust;
	this->isTabbed=isTabbed;
	
	this->isScrollable=isScrollable;
	clientRect=clientArea;
	this->titleString = titleString;
	this->window=window;
	this->windowNumber=windowNumber;

// ------

//	updateRectangles();
	
	
// ------ PROCESSING

//	if(isTabbed)
//		forceSmallY(21);
	originalRect=getRelativeRect();
	updateBorders(); //~~

    if(clientRect.x<border.x+5) clientRect.x=border.x+5;
   	if(clientRect.y<border.y+5) clientRect.y=border.y+5;
    if(clientRect.y+clientRect.height>border.y+border.height-5) clientRect.height=border.y+border.height-5-clientRect.y;
	if(isScrollable)
	{
		if(clientRect.x+clientRect.width>border.x+border.width-3) clientRect.width=border.x+border.width-3-clientRect.x;
//        if(clientRect.x+clientRect.width>ScrollRect.x-3) clientRect.width=ScrollRect.x-3-clientRect.x; TODO
	} else
	{
		if(clientRect.x+clientRect.width>border.x+border.width-3) clientRect.width=border.x+border.width-3-clientRect.x;
	}
	
	originalClientRect=clientRect;
	clientStartRect=clientRect;
	clientTargetRect=clientRect;

	lastItemY=clientRect.height;
// ------ Buttons, ScrollBars etc.
	if(isScrollable)
	{
//		scrollBar=new UI_ScrollBar(Rect(border.GetPosition()+Point(border.GetWidth()-18,2), Size(16, border.GetHeight()-4)));
	}
//	else scrollBar=0;

	if(isTabbed)
	{
		tabRow=new UI_Radio(this);
		setCurrentTab(ADVANCED_TAB);		
	}
}

UI_Window::~UI_Window()
{
/*	if(scrollBalken)
	{
		delete scrollBalken;
		scrollBalken=0;
	};*/ // TODO
	if(titleParameter)
	{
		delete titleParameter;
		titleParameter=0;
	};
	if(tabRow)
		delete tabRow;
}

void UI_Window::addTab(UI_Button* tab)
{
	if(!isTabbed)
		return;
	tabRow->addButton(tab);
};

void UI_Window::setCurrentTab(eTab tab)
{
	if(!isTabbed) return;
	tabRow->forceTabPressed(tab);	
};

bool UI_Window::tabWasChanged()
{
	if(!isTabbed) return false;
	return(tabRow->hasChanged());
};

const eTab UI_Window::getCurrentTab()
{
	if(!isTabbed) return (eTab)0;
	return((eTab)(tabRow->getMarked()+1));
};

void UI_Window::setTitleParameter(string p)
{
	if(titleParameter)
		delete(titleParameter);
	titleParameter=new string(p);
};

Rect UI_Window::getRelativeClientRect()
{
	return(clientRect);
};

Rect UI_Window::getAbsoluteClientRect()
{
	return(Rect(getAbsoluteClientRectPosition(),getClientRectSize()));
};

int UI_Window::getClientRectHeight()
{
    return(clientRect.height);
};
                                                                                                                                                            
int UI_Window::getClientRectWidth()
{
    return(clientRect.width);
};

Point UI_Window::getRelativeClientRectPosition()
{
	return(clientRect.GetPosition());
};

int UI_Window::getRelativeClientRectUpperBound()
{
    return(clientRect.y);
};

int UI_Window::getRelativeClientRectLeftBound()
{
    return(clientRect.x);
};

int UI_Window::getRelativeClientRectRightBound()
{
	    return(clientRect.x+clientRect.width);
};
                                                                                
int UI_Window::getRelativeClientRectLowerBound()
{
	    return(clientRect.y+clientRect.height);
};


int UI_Window::getAbsoluteClientRectLeftBound()
{
    return(getAbsoluteClientRectPosition().x);
};

int UI_Window::getAbsoluteClientRectRightBound()
{
    return(getAbsoluteClientRectPosition().x+getClientRectWidth());
};

int UI_Window::getAbsoluteClientRectUpperBound()
{
    return(getAbsoluteClientRectPosition().y);
};
                                                                                                                                                            
int UI_Window::getAbsoluteClientRectLowerBound()
{
    return(getAbsoluteClientRectPosition().y+getClientRectHeight());
};

Point UI_Window::getAbsoluteClientRectPosition()
{
    return(clientRect.GetPosition()+getAbsolutePosition());
};

Size UI_Window::getClientRectSize()
{
    return(clientRect.GetSize());
};

bool UI_Window::insideClientRect(Point pos)
{
	    return(clientRect.Inside(pos-getAbsolutePosition())); //?
		//  return(clientArea.Inside(rectangle.x,rectangle.y)||clientArea.Inside(rectangle.x+rectangle.width,rectangle.y+rectangle.height)||clientArea.Inside(rectangle.x+rectangle.width,rectangle.y)||clientArea.Inside(rectangle.x,rectangle.y+rectangle.height));
		    //TODO: wenns auf beiden Seiten ueberlappt?
};

void UI_Window::adjustClientRect()
{
//CLIENT WINDOW
    int height=getTargetHeight()-originalRect.height+originalClientRect.height;
    int width=getTargetWidth()-originalRect.width+originalClientRect.width;
    if(height!=clientTargetRect.height)
        clientStartRect.height=clientRect.height;
    if(width!=clientTargetRect.width)
        clientStartRect.width=clientRect.width;
    clientTargetRect.width=width;
    clientTargetRect.height=height;
}

void UI_Window::updateBorders()
{
    border=Rect(Point(3,8),getSize()+Size(-6,-11));
    outerBorder=Rect(Point(1,6),getSize()+Size(-2,-7));
};

                                                                                                                                                            
//void UI_Window::updateWindow()
//{
//	updateBorders();
  //  ScrollArea=Rect(Rahmen.x+Rahmen.width-18,Rahmen.y+2,16,Rahmen.height-4); //relative Koordinate! TODO
//    ScrollBalken=Rect(ScrollArea.x+1,ScrollArea.y+17,ScrollArea.width-2,ScrollArea.height-34);
    //PfeilUp=Rect(ScrollArea.x,ScrollArea.y,ScrollArea.width,ScrollArea.width); //relative Koordinate!
//    PfeilDown=Rect(ScrollArea.x,ScrollArea.y+ScrollArea.height-ScrollArea.width,ScrollArea.width,ScrollArea.width); //relative Koordinate!
//  clientArea.x=Rahmen.x+5;
  //      if(scrolled)
    //      clientArea.width=ScrollArea.x-3-clientArea.x;
//  else
  //          clientArea.width=Rahmen.x+Rahmen.width-3-clientArea.x;
                                                                                                                                                            
/*  if(tabNumber)
    {
                                                                                                                                                            
    }*/ //~~
                                                                                                                                                            
    /*  if(descriptionNumber)
    {
        clientArea.height-=Rahmen.y+16-clientArea.y;
        clientArea.y=Rahmen.y+16;
    }
    else
    {
        clientArea.y+=Rahmen.y-oldRahmen.y;
        clientArea.height+=Rahmen.y+Rahmen.height-oldRahmen.y-oldRahmen.height;
    }*/
//}
	
//};


void UI_Window::process()
{
//	if(!shown) return;
	UI_Object::process();
//  int i;
//  if(WindowMove) ~~
//      adjustClientRect(Rect(controls.getX()-WindowMoveX,controls.getY()-WindowMoveY,Border.width,Border.height));
    
	int a=clientRect.height;
	int b=clientTargetRect.height;

	if(a!=b)
	{

    move(clientRect.x,		clientStartRect.x,		clientTargetRect.x);
    move(clientRect.y,		clientStartRect.y,		clientTargetRect.y);
    move(clientRect.width,	clientStartRect.width,	clientTargetRect.width);
    move(clientRect.height,	clientStartRect.height,	clientTargetRect.height);
//target springt hin und her... 
	a=clientRect.height;
	b=clientTargetRect.height;
	}
                                                                                                                                                            
    updateBorders();

	
	/* if(WindowMove)
    {
        startx=x;starty=y;
    }*/
                                                                                                                                                            
//    setMaxScrollY(lastItemY);
// TODO

    if(getDoAdjustments()==2)//||(autoAdjust))
	{
        adjustClientRect();
	    setDoAdjustments(0);
	}

    lastItemY=0;
//    if((ScrollBalkenPressed==2)||(ScrollBalkenPressed==3)||(ScrollBalkenPressed==4))
  //  {
//        moveScrollBalkenTo(controls.getCurrentPosition().y-controls.getDragStartPosition().y-ScrollBalken.y);
//      controls.updateDragStartPosition(controls.getCurrentPosition());
//    }
  //  else moveScrollBalkenTo(0);

}


void UI_Window::drawTitle(DC* dc)
{
//	dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
//	dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
//	dc->DrawRectangle(Rect(getAbsolutePosition()+clientTargetRect.GetPosition(), clientTargetRect.GetSize()));
    dc->SetBrush(*theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH));
	dc->SetPen(*theme.lookUpPen(BORDER_PEN));
                                                                                                                                                            
/*  for(int i=0;i<2;i++)
    {
        dc->DrawRoundedRectangle(OuterRahmen.GetPosition()+rect.GetPosition()+Point(-2,OuterRahmen.GetHeight()/4),Size(8,OuterRahmen.GetHeight()/2),4);
        dc->DrawRoundedRectangle(OuterRahmen.GetPosition()+rect.GetPosition()+Point(OuterRahmen.GetWidth()/4,OuterRahmen.GetHeight()-6),Size(OuterRahmen.GetWidth()/2,8),4);
        dc->DrawRoundedRectangle(OuterRahmen.GetPosition()+rect.GetPosition()+Point(OuterRahmen.GetWidth()-6,OuterRahmen.GetHeight()/4),Size(8,OuterRahmen.GetHeight()/2),4);
        dc->DrawRoundedRectangle(OuterRahmen.GetPosition()+rect.GetPosition()+Point(OuterRahmen.GetWidth()/4,-2),Size(OuterRahmen.GetWidth()/2,8),4);
//      dc->SetBrush(getBackground());
    }*/
                                                                                                                                                            
    dc->SetFont(theme.lookUpFont(SMALL_ITALICS_BOLD_FONT));
    dc->SetTextForeground(*theme.lookUpColor(TITLE_COLOUR));

	string text;
	if(titleParameter)
		text=theme.lookUpFormattedString(titleString, *titleParameter);
	else text=*theme.lookUpString(titleString);
	
    int dx,dy;
    dc->GetTextExtent(text,&dx,&dy);
	Rect titleRect=Rect(border.GetPosition()+getAbsolutePosition()+Point(0,-dy/2-3), Size(dx+5,dy+2));
    dc->DrawRoundedRectangle(titleRect, 3);
	
    dc->DrawText(text, titleRect.GetPosition());
};



void UI_Window::draw(DC* dc)
{
	if(!shown) return;
//    if(tabNumber)
  //      DrawTabs(dc);
// draw outer border:
	dc->SetPen(*theme.lookUpPen(OUTER_BORDER_PEN));
	if(isTopItem()) // => main window!
	    dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
	else
    	dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));

    dc->DrawRoundedRectangle(outerBorder.GetPosition()+getAbsolutePosition(),outerBorder.GetSize(),3);

	
// draw inner border:
	if(border.Inside(controls.getCurrentPosition(getAbsolutePosition())))
        dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	else 
	{
        dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
	}
	dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));

	dc->DrawRoundedRectangle(border.GetPosition()+getAbsolutePosition(),border.GetSize(),3);
	
// draw title if there are no tabs: 
    if(!isTabbed)
        drawTitle(dc);

// draw descriptions - obsolete

//   dc->SetBrush(getBackground());
/*  for(i=0;i<descriptionNumber;i++)
    {
        dc->GetTextExtent(Description[i],&dx,&dy);
        dx=dx+3;
        dc->DrawRoundedRectangle(Rahmen.x+x+descriptionPosition[i]-2,bla+Rahmen.y+y-dy/2-2,dx+2,dy+2,3);
        dc->DrawText(Description[i],Rahmen.x+x+descriptionPosition[i],bla+Rahmen.y+y-dy/2-1);
    }*/
                                                                                                                                                            
  /*  if(!scrolled) return;
                                                                                                                                                            
//(RahmenPen.Red+RahmenPen.Green+RahmenPen.Blue)/3,(RahmenPen.Red+RahmenPen.Green+RahmenPen.Blue)/3,(RahmenPen.Red+RahmenPen.Green+RahmenPen.Blue)/3);

// draw ScrollBar:
                                                                                                                                                            
    dc->DrawRoundedRectangle(Rect(ScrollArea.GetPosition()+rect.GetPosition(),ScrollArea.GetSize()),3);
//TODO button draus machen
    if((ScrollBalkenPressed==2)||(ScrollBalkenPressed==3)||(ScrollBalkenPressed==4)) //currently pressing the ScrollBalken
    {
        dc->SetBrush(clickedItemBrush);
        dc->SetPen(RahmenPen);
    } else
    if(ScrollBalkenPressed) // just inside the ScrollBalken
    {
        dc->SetBrush(RahmenBrush);
        dc->SetPen(clickedItemPen);
    } else
    {
        dc->SetBrush(getBackground());
        dc->SetPen(RahmenPen);
    }
                                                                                                                                                            
                                                                                                                                                            
    dc->DrawRoundedRectangle(Rect(ScrollBalken.GetPosition()+rect.GetPosition(),ScrollBalken.GetSize()),4);
    dc->DrawRoundedRectangle(Rect(controls.getDragStartPosition(),ScrollBalken.GetSize()),4);
                                                                                                                                                            
    dc->SetPen(RahmenPen);
    dc->SetBrush(getBackground());
    dc->DrawRoundedRectangle(Rect(PfeilUp.GetPosition()+rect.GetPosition(),PfeilUp.GetSize()),4);
                                                                                                                                                            
    if(scrollY<=0)
    {
        dc->SetBrush(getBackground());
        dc->SetPen(disabledItemPen);
    } else
    switch(PfeilUpPressed)
    {
        case POINTER_OVER_BUTTON:
        {
            dc->SetBrush(RahmenBrush);
            dc->SetPen(clickedItemPen);
        };break;
        case PRESSING_BUTTON:
        {
            dc->SetBrush(clickedItemBrush);
            dc->SetPen(RahmenPen);
      };break;
        default:
        {
            dc->SetBrush(getBackground());
            dc->SetPen(RahmenPen);
        };break;
                                                                                                                                                            
    }
    Point points[3];
    points[0].x=PfeilUp.width/2;points[0].y=2;
    points[1].x=2;points[1].y=PfeilUp.height-3;
    points[2].x=PfeilUp.width-3;points[2].y=PfeilUp.height-3;
    dc->DrawPolygon(3,points,PfeilUp.x+rect.GetX(),PfeilUp.y+rect.GetY());
                                                                                                                                                            
    dc->SetPen(RahmenPen);
    dc->SetBrush(getBackground());
    dc->DrawRoundedRectangle(PfeilDown.x+rect.GetX(),PfeilDown.y+rect.GetY(),PfeilDown.width,PfeilDown.height,4);
                                                                                                                                                            
    if(scrollY>=maxScrollY-(clientRect.height))
    {
        dc->SetBrush(getBackground());
        dc->SetPen(disabledItemPen);
    } else
    switch(PfeilDownPressed)
    {
        case POINTER_OVER_BUTTON:
        {
            dc->SetBrush(RahmenBrush);
            dc->SetPen(clickedItemPen);
        };break;
        case PRESSING_BUTTON:
        {
            dc->SetBrush(clickedItemBrush);
            dc->SetPen(RahmenPen);
        };break;
        default:
        {
            dc->SetBrush(getBackground());
            dc->SetPen(RahmenPen);
        };break;
    }
                                                                                                                                                            
    points[0].x=PfeilDown.width/2;points[0].y=PfeilDown.height-3;
    points[1].x=2;points[1].y=2;
    points[2].x=PfeilDown.width-3;points[2].y=2;
    dc->DrawPolygon(3,points,PfeilDown.GetX()+rect.GetX(),PfeilDown.GetY()+rect.GetY());*/
	UI_Object::draw(dc);
};

int UI_Window::getScrollY()
{
	return 0;
};
                                                                                                                                                            
bool UI_Window::fitItemToRelativeClientRect(Rect& rect, const int adjust)
{
    if(rect.x<getRelativeClientRectLeftBound()) rect.x=getRelativeClientRectLeftBound();
    if(rect.width+rect.x>getRelativeClientRectRightBound())
        rect.width=getRelativeClientRectRightBound()-rect.x;
//  if(rect.y<clientArea.y) rect.y=clientArea.y;
  //      if(rect.y+rect.height>clientArea.y+clientArea.height) rect.height=clientArea.y+clientArea.height-rect.y;
    if(adjust)
    {
        setDoAdjustments(1);
        if(rect.y+rect.height/*-getRelativeClientRectUpperBound()*/>lastItemY)
            lastItemY=rect.y+rect.height/*-getRelativeClientRectUpperBound()*/;
		if(lastItemY>getMaxRect().height) lastItemY=getMaxRect().height;
    }
    if((rect.width<=0)||(rect.y<getRelativeClientRectUpperBound())||(rect.y+rect.height>getRelativeClientRectLowerBound())) 
	return false;
    else return true;
}

bool UI_Window::fitItemToAbsoluteClientRect(Rect& rect, const int adjust)
{
    if(rect.x<getAbsoluteClientRectLeftBound()) rect.x=getAbsoluteClientRectLeftBound();
    if(rect.width+rect.x>getAbsoluteClientRectRightBound())
        rect.width=getAbsoluteClientRectRightBound()-rect.x;
//  if(rect.y<clientArea.y) rect.y=clientArea.y;
  //      if(rect.y+rect.height>clientArea.y+clientArea.height) rect.height=clientArea.y+clientArea.height-rect.y;
    if(adjust)
    {
        setDoAdjustments(1);
        if(rect.y+rect.height-getAbsoluteClientRectUpperBound()>lastItemY)
            lastItemY=rect.y+rect.height-getAbsoluteClientRectUpperBound();
		if(lastItemY>getMaxRect().height) lastItemY=getMaxRect().height;
    }
    if((rect.width<=0)||(rect.y<getAbsoluteClientRectUpperBound())||(rect.y+rect.height>getAbsoluteClientRectLowerBound())) 
	return false;
    else return true;
}

void UI_Window::setChangedFlag(bool flag)
{
	changedFlag=flag;
};

bool UI_Window::getChangedFlag()
{
	return(changedFlag);
};

void UI_Window::changeAccepted()
{
	changedFlag=0;
};

// TODO evtl in UI_Object und alle Kinder immer aufrufen!

	
