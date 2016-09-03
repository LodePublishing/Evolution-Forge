#include "UI_Window.h"
#include "UI_Theme.h"

UI_Window::UI_Window(UI_Object* parent, eString titleString, wxRect rect, wxRect maxSize, const bool isScrollable, const bool isAutoAdjust, const bool isTabbed, wxRect clientArea):UI_Object(parent, rect, maxSize)
{
// ------ INITIALIZATION ------
	titleParameter=0;
	setCurrentTab(BASIC_TAB);
	changedFlag=false;
	this->isAutoAdjust=isAutoAdjust;
//	this->tabNumber=tabNumber;
	
	this->isScrollable=isScrollable;
	clientRect=clientArea;
	this->titleString = titleString;
// ------

// ------ PROCESSING
	originalRect=getRelativeRect();

//	if(isTabbed)
//	{
//		this->rect.y+=21;
//		this->rect.height-=21;
//	}

	updateBorders();

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
//		scrollBar=new UI_ScrollBar(wxRect(border.GetPosition()+wxPoint(border.GetWidth()-18,2), wxSize(16, border.GetHeight()-4)));
	}
//	else scrollBar=0;

	if(isTabbed)
	{
//		if(!children TODO
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
}

void UI_Window::setCurrentTab(eTab tab)
{
	if(!isTabbed)
		return;
	currentTab=tab;
	theme.setTab(tab);
};

eTab UI_Window::getCurrentTab()
{
	return(currentTab);
};

void UI_Window::setTitleParameter(wxString p)
{
	if(titleParameter)
		delete(titleParameter);
	titleParameter=new wxString(p);
};
		
int UI_Window::getClientRectUpperBound()
{
    return(clientRect.y+getRelativeUpperBound());
};

wxRect UI_Window::getRelativeClientRect()
{
	return(clientRect);
};

wxPoint UI_Window::getRelativeClientRectPosition()
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
                                                                                                                                                         
int UI_Window::getClientRectLeftBound()
{
    return(clientRect.x+getRelativeLeftBound());
};
                                                                                                                                                            
int UI_Window::getClientRectHeight()
{
    return(clientRect.height);
};
                                                                                                                                                            
int UI_Window::getClientRectWidth()
{
    return(clientRect.width);
};

wxPoint UI_Window::getClientRectPosition()
{
    return(clientRect.GetPosition()+getRelativePosition());
};

wxSize UI_Window::getClientRectSize()
{
    return(clientRect.GetSize());
};

bool UI_Window::insideClientRect(wxPoint pos)
{
	    return(clientRect.Inside(pos-getRelativePosition()));
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
    border=wxRect(wxPoint(3,8),getSize()+wxSize(-6,-11));
    outerBorder=wxRect(wxPoint(1,6),getSize()+wxSize(-2,-7));
};

                                                                                                                                                            
//void UI_Window::updateWindow()
//{
//	updateBorders();
  //  ScrollArea=wxRect(Rahmen.x+Rahmen.width-18,Rahmen.y+2,16,Rahmen.height-4); //relative Koordinate! TODO
//    ScrollBalken=wxRect(ScrollArea.x+1,ScrollArea.y+17,ScrollArea.width-2,ScrollArea.height-34);
    //PfeilUp=wxRect(ScrollArea.x,ScrollArea.y,ScrollArea.width,ScrollArea.width); //relative Koordinate!
//    PfeilDown=wxRect(ScrollArea.x,ScrollArea.y+ScrollArea.height-ScrollArea.width,ScrollArea.width,ScrollArea.width); //relative Koordinate!
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
	UI_Object::process();
//  int i;
//  if(WindowMove) ~~
//      adjustClientRect(wxRect(controls.getX()-WindowMoveX,controls.getY()-WindowMoveY,Border.width,Border.height));
    
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
/*  if(WindowMove)
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


void UI_Window::drawTitle(wxDC* dc)
{

	dc->SetPen(wxPen(wxColour(0,255,0),2,wxSOLID));
	dc->SetBrush(*wxTRANSPARENT_BRUSH);
	dc->DrawRectangle(wxRect(getAbsolutePosition()+clientTargetRect.GetPosition(), clientTargetRect.GetSize()));
    dc->SetBrush(*theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH));

	dc->SetPen(*theme.lookUpPen(BORDER_PEN));
                                                                                                                                                            
/*  for(int i=0;i<2;i++)
    {
        dc->DrawRoundedRectangle(OuterRahmen.GetPosition()+rect.GetPosition()+wxPoint(-2,OuterRahmen.GetHeight()/4),wxSize(8,OuterRahmen.GetHeight()/2),4);
        dc->DrawRoundedRectangle(OuterRahmen.GetPosition()+rect.GetPosition()+wxPoint(OuterRahmen.GetWidth()/4,OuterRahmen.GetHeight()-6),wxSize(OuterRahmen.GetWidth()/2,8),4);
        dc->DrawRoundedRectangle(OuterRahmen.GetPosition()+rect.GetPosition()+wxPoint(OuterRahmen.GetWidth()-6,OuterRahmen.GetHeight()/4),wxSize(8,OuterRahmen.GetHeight()/2),4);
        dc->DrawRoundedRectangle(OuterRahmen.GetPosition()+rect.GetPosition()+wxPoint(OuterRahmen.GetWidth()/4,-2),wxSize(OuterRahmen.GetWidth()/2,8),4);
//      dc->SetBrush(getBackground());
    }*/
                                                                                                                                                            
    dc->SetFont(*theme.lookUpFont(SMALL_ITALICS_BOLD_FONT));
    dc->SetTextForeground(*theme.lookUpColour(TITLE_COLOUR));

	wxString text;
	if(titleParameter)
		text=theme.lookUpFormattedString(titleString, (const char*)(*titleParameter));
	else text=*theme.lookUpString(titleString);
	
    int dx,dy;
    dc->GetTextExtent(text,&dx,&dy);
	wxRect titleRect=wxRect(border.GetPosition()+getRelativePosition()+wxPoint(0,-dy/2-3), wxSize(dx+5,dy+2));
    dc->DrawRoundedRectangle(titleRect, 3);
	
    dc->DrawText(text, titleRect.GetPosition());
};



void UI_Window::draw(wxDC* dc)
{
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
	if(border.Inside(controls.getCurrentPosition(getRelativePosition())))
        dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	else 
        dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
	dc->SetBrush(*wxTRANSPARENT_BRUSH);
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
                                                                                                                                                            
    dc->DrawRoundedRectangle(wxRect(ScrollArea.GetPosition()+rect.GetPosition(),ScrollArea.GetSize()),3);
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
                                                                                                                                                            
                                                                                                                                                            
    dc->DrawRoundedRectangle(wxRect(ScrollBalken.GetPosition()+rect.GetPosition(),ScrollBalken.GetSize()),4);
    dc->DrawRoundedRectangle(wxRect(controls.getDragStartPosition(),ScrollBalken.GetSize()),4);
                                                                                                                                                            
    dc->SetPen(RahmenPen);
    dc->SetBrush(getBackground());
    dc->DrawRoundedRectangle(wxRect(PfeilUp.GetPosition()+rect.GetPosition(),PfeilUp.GetSize()),4);
                                                                                                                                                            
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
    wxPoint points[3];
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
                                                                                                                                                            

bool UI_Window::fitItemToClientRect(wxRect& rect, const int adjust)
{
    if(rect.x<getRelativeClientRectLeftBound()) rect.x=getClientRectLeftBound();
    if(rect.width+rect.x>getRelativeClientRectLeftBound()+getClientRectWidth())
        rect.width=getRelativeClientRectLeftBound()+getClientRectWidth()-rect.x;
//  if(rect.y<clientArea.y) rect.y=clientArea.y;
  //      if(rect.y+rect.height>clientArea.y+clientArea.height) rect.height=clientArea.y+clientArea.height-rect.y;
    if(adjust)
    {
        setDoAdjustments(1);
        if(rect.y+rect.height/*-getRelativeClientRectUpperBound()*/>lastItemY)
            lastItemY=rect.y+rect.height/*-getRelativeClientRectUpperBound()*/;
    }
    if((rect.width<=0)||(rect.y<getRelativeClientRectUpperBound())||(rect.y+rect.height>getRelativeClientRectUpperBound()+getClientRectHeight())) 
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

	
