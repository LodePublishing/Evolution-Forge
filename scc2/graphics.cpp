#include "graphics.h"

void GraphixScrollWindow::addTitle(int x, const char* Title)
{
	if(titleNumber>=5) return;
        titlePosition[titleNumber]=x;
        this->Title[titleNumber].Printf("%s",Title);
        titleNumber++;
}

void GraphixScrollWindow::addDescription(int x, const char* Description)
{
        if(descriptionNumber>=10) return;
        descriptionPosition[descriptionNumber]=x;
        this->Description[descriptionNumber].Printf("%s",Description);
        descriptionNumber++;
};

void GraphixScrollWindow::setDescription(int nr, int x, const char* Description)
{
	if(nr>=descriptionNumber) return;
        descriptionPosition[nr]=x;
        this->Description[nr].Printf("%s",Description);
};

                                                                                                                                      
int GraphixScrollWindow::getScrollY()
{
        return(scrollY);
};

int GraphixScrollWindow::getUpperBound()
{
	return(OuterRahmen.y);
};
int GraphixScrollWindow::getLowerBound()
{
        return(OuterRahmen.y+OuterRahmen.height);
};
int GraphixScrollWindow::getLeftBound()
{
        return(OuterRahmen.x);
};
int GraphixScrollWindow::getRightBound()
{
        return(OuterRahmen.x+OuterRahmen.width);
}

int GraphixScrollWindow::getInnerUpperBound()
{
	return(clientArea.y);
};

int GraphixScrollWindow::getInnerLeftBound()
{
	return(clientArea.x);
};

int GraphixScrollWindow::getInnerHeight()
{
        return(clientArea.height);
};

int GraphixScrollWindow::getInnerWidth()
{
        return(clientArea.width);
};
                                                                                                              
void GraphixScrollWindow::setScrollY(int y)
{
	y=y*maxScrollY/(ScrollArea.height-34); //scale
        if(y<0) scrollY=0;
        else 
	if(y>maxScrollY) scrollY=maxScrollY;//-(ScrollArea.height-34)) scrollY=maxScrollY-(ScrollArea.height-34);
        else 
	scrollY=y;
	ScrollBalken.y=ScrollArea.y+17+scrollY*(ScrollArea.height-34)/maxScrollY;
};
                                                                                                                                      
void GraphixScrollWindow::setMaxScrollY(int y)
{
        if(y>0) maxScrollY=y;
	if(maxScrollY<=(ScrollArea.height-34))
	{
		ScrollBalken.y=ScrollArea.y+17;
		ScrollBalken.height=ScrollArea.height-34;
	}
	else
	{
		ScrollBalken.height=(ScrollArea.height-34)*(ScrollArea.height-34)/maxScrollY;
		ScrollBalken.y=ScrollArea.y+17+scrollY*(ScrollArea.height-34)/maxScrollY;
	}
};
                                                                                                                                      
void GraphixScrollWindow::setTitleColour(wxColour titleColour)
{
        TitleColour=titleColour;
};
                                                                                                                                      
void GraphixScrollWindow::setBackground(wxColour backgroundColour)
{
        Background=wxBrush(backgroundColour,wxCROSS_HATCH);
};


void GraphixScrollWindow::setRahmenPen(wxColour rahmenPenColour)
{
        RahmenPen=rahmenPenColour;
};
                                                                                                                                      
void GraphixScrollWindow::setRahmen(wxRect rahmen)
{
        Rahmen.x=rahmen.x+3+x;
	Rahmen.y=rahmen.y+8+y;
	Rahmen.width=rahmen.width-6;
	Rahmen.height=rahmen.height-11;
	OuterRahmen.x=Rahmen.x-2;
	OuterRahmen.y=Rahmen.y-2;
	OuterRahmen.width=Rahmen.width+4;
	OuterRahmen.height=Rahmen.height+4;
};
                                                                                                                                      
void GraphixScrollWindow::Draw(wxDC* dc)
{
        int i;
	if(ScrollBalkenMove)
		moveScrollBalkenTo(mouseY-ScrollBalkenMoveY-ScrollBalken.y);
	else moveScrollBalkenTo(0);

//erstmal Rahmen:
        dc->SetPen(wxPen(RahmenPen,1,wxSOLID));
        dc->SetBrush(Background);
        dc->DrawRoundedRectangle(OuterRahmen,4);
        dc->DrawRoundedRectangle(Rahmen,4);

//dann Schrift:
        dc->SetBrush(wxBrush(wxColour(0,0,0),wxSOLID));
        dc->SetTextForeground(TitleColour);
	int dx,dy;
        for(i=0;i<titleNumber;i++)
	{
		dc->GetTextExtent(Title[i],&dx,&dy);
		dx=dx+3;
		dc->DrawRoundedRectangle(Rahmen.x+titlePosition[i]-2,Rahmen.y-dy/2-2,dx+2,dy+2,4);
                dc->DrawText(Title[i],Rahmen.x+titlePosition[i],Rahmen.y-dy/2-1);
	}
	int bla=dy;

        for(i=0;i<descriptionNumber;i++)
        {
                dc->GetTextExtent(Description[i],&dx,&dy);
		dx=dx+3;
                dc->DrawRectangle(Rahmen.x+descriptionPosition[i]-2,bla+Rahmen.y-dy/2-2,dx+2,dy+2);
                dc->DrawText(Description[i],Rahmen.x+descriptionPosition[i],bla+Rahmen.y-dy/2-1);
        }

	if(!scrolled) return;

//        disabledItem=wxColour(10,10,10);
	disabledItemPen=wxColour(70,70,70);
//(RahmenPen.Red+RahmenPen.Green+RahmenPen.Blue)/3,(RahmenPen.Red+RahmenPen.Green+RahmenPen.Blue)/3,(RahmenPen.Red+RahmenPen.Green+RahmenPen.Blue)/3);
        enabledItemPen=RahmenPen;
        mouseOverItem=RahmenPen;
        clickedItem=wxColour(200,200,200);

	dc->DrawRoundedRectangle(ScrollArea,4);

	if(ScrollBalkenMove)
	{
		dc->SetBrush(wxBrush(clickedItem,wxSOLID));
		dc->SetPen(wxPen(mouseOverItem,1,wxSOLID));
	} else if(ScrollBalken.Inside(mouseX,mouseY))
	{
	        dc->SetBrush(wxBrush(mouseOverItem,wxSOLID));
                dc->SetPen(wxPen(clickedItem,1,wxSOLID));
	} else
        {
        	dc->SetBrush(Background);
                dc->SetPen(wxPen(RahmenPen,1,wxSOLID));
        }
	dc->DrawRoundedRectangle(ScrollBalken,4);

        dc->SetPen(wxPen(RahmenPen,1,wxSOLID));
        dc->SetBrush(Background);
        dc->DrawRoundedRectangle(PfeilUp,4);
	if(PfeilUpPressed)
	{
		dc->SetBrush(wxBrush(clickedItem,wxSOLID));
		dc->SetPen(wxPen(mouseOverItem,1,wxSOLID));
	}
	else
	{
	        if(scrollY<=0)
		{
			dc->SetBrush(Background);
			dc->SetPen(wxPen(disabledItemPen,1,wxSOLID));
		}
	        else if(PfeilUp.Inside(mouseX,mouseY))
		{
			dc->SetBrush(wxBrush(mouseOverItem,wxSOLID));
			dc->SetPen(wxPen(clickedItem,1,wxSOLID));
		}
	        else
		{
			dc->SetBrush(Background);
			dc->SetPen(wxPen(RahmenPen,1,wxSOLID));
		}
	}
        wxPoint points[3];
        points[0].x=PfeilUp.width/2;points[0].y=2;
        points[1].x=2;points[1].y=PfeilUp.height-3;
        points[2].x=PfeilUp.width-3;points[2].y=PfeilUp.height-3;
	dc->DrawPolygon(3,points,PfeilUp.x,PfeilUp.y);

        dc->SetPen(wxPen(RahmenPen,1,wxSOLID));
        dc->SetBrush(Background);
        dc->DrawRoundedRectangle(PfeilDown,4);
        if(PfeilDownPressed)
        {
                dc->SetBrush(wxBrush(clickedItem,wxSOLID));
                dc->SetPen(wxPen(mouseOverItem,1,wxSOLID));
        }
        else
        {
                if(scrollY>=maxScrollY/*-(ScrollArea.height-34)*/)
                {
                        dc->SetBrush(Background);
                        dc->SetPen(wxPen(disabledItemPen,1,wxSOLID));
                }
                else if(PfeilDown.Inside(mouseX,mouseY))
                {
                        dc->SetBrush(wxBrush(mouseOverItem,wxSOLID));
                        dc->SetPen(wxPen(clickedItem,1,wxSOLID));
		}
                else
                {
                        dc->SetBrush(Background);
                        dc->SetPen(wxPen(RahmenPen,1,wxSOLID));
                }
        }
        points[0].x=PfeilDown.width/2;points[0].y=PfeilDown.height-3;
        points[1].x=2;points[1].y=2;
        points[2].x=PfeilDown.width-3;points[2].y=2;
        dc->DrawPolygon(3,points,PfeilDown.x,PfeilDown.y);
};

bool GraphixScrollWindow::insideClientArea(int x,int y)//wxRect rectangle) //for all non-buttons/titles etc.
{
	return(clientArea.Inside(x,y));
//	return(clientArea.Inside(rectangle.x,rectangle.y)||clientArea.Inside(rectangle.x+rectangle.width,rectangle.y+rectangle.height)||clientArea.Inside(rectangle.x+rectangle.width,rectangle.y)||clientArea.Inside(rectangle.x,rectangle.y+rectangle.height));
	//TODO: wenns auf beiden Seiten ueberlappt?
};

bool GraphixScrollWindow::fitToClientArea(wxRect& rectangle)
{
	if(rectangle.x<clientArea.x) rectangle.x=clientArea.x;
        if(rectangle.x+rectangle.width>clientArea.x+clientArea.width) rectangle.width=clientArea.x+clientArea.width-rectangle.x;
//	if(rectangle.y<clientArea.y) rectangle.y=clientArea.y;
  //      if(rectangle.y+rectangle.height>clientArea.y+clientArea.height) rectangle.height=clientArea.y+clientArea.height-rectangle.y;

        if((rectangle.width==0)||(rectangle.y<clientArea.y)||(rectangle.y+rectangle.height>clientArea.y+clientArea.height)) return false;
	else return true;
}

GraphixScrollWindow::GraphixScrollWindow()
{
	titleNumber=0;
};

GraphixScrollWindow::GraphixScrollWindow(wxColour TitleColour, wxColour BackgroundColour, wxColour RahmenPenColour, wxRect rahmen, int description, int x, int y, int scrolled, wxRect clientArea)
{
	this->x=x;
	this->y=y;
        this->scrolled=scrolled;
        setTitleColour(TitleColour);
        setBackground(BackgroundColour);
        setRahmenPen(RahmenPenColour);
        setRahmen(rahmen);

        ScrollArea=wxRect(Rahmen.x+Rahmen.width-18,Rahmen.y+2,16,Rahmen.height-4);
	ScrollBalken=wxRect(ScrollArea.x+1,ScrollArea.y+17,ScrollArea.width-2,ScrollArea.height-34);
	PfeilUp=wxRect(ScrollArea.x,ScrollArea.y,ScrollArea.width,ScrollArea.width);
	PfeilDown=wxRect(ScrollArea.x,ScrollArea.y+ScrollArea.height-ScrollArea.width,ScrollArea.width,ScrollArea.width);
        //TODO Resize, Move etc.
        titleNumber=0;
	if(clientArea.x<Rahmen.x+5) clientArea.x=Rahmen.x+5;
	if(clientArea.y<Rahmen.y+5) clientArea.y=Rahmen.y+5;
        if(clientArea.y+clientArea.height>Rahmen.y+Rahmen.height-5) clientArea.height=Rahmen.y+Rahmen.height-5-clientArea.y;

	if(scrolled)
	{
		if(clientArea.x+clientArea.width>ScrollArea.x-3) clientArea.width=ScrollArea.x-3-clientArea.x;
	}
	else
	{
               if(clientArea.x+clientArea.width>Rahmen.x+Rahmen.width-3) clientArea.width=Rahmen.x+Rahmen.width-3-clientArea.x;
	}

	if(description)
		if(clientArea.y<Rahmen.y+16) { clientArea.height-=Rahmen.y+16-clientArea.y;clientArea.y=Rahmen.y+16;}

	
	this->clientArea=clientArea;
	ScrollBalkenMove=0;
        PfeilUpPressed=0;PfeilDownPressed=0;
	setMaxScrollY(1500);
	descriptionNumber=0;
};


void GraphixScrollWindow::OnScrollMouse(int msy)
{
        if(insideClientArea(mouseX,mouseY))
        {
                if(msy<0)
                        moveScrollBalkenTo(30);
                else if(msy>0)
                        moveScrollBalkenTo(-30);
        }
}

void GraphixScrollWindow::OnMouseLeftDown()
{
        PfeilUpPressed=0;
        PfeilDownPressed=0;
        ScrollBalkenMove=0;
	if(ScrollBalken.Inside(mouseX,mouseY))
	{
		ScrollBalkenMove=1;
                ScrollBalkenMoveY=mouseY-ScrollBalken.y;
	}
	else if(PfeilUp.Inside(mouseX,mouseY)&&(scrollY>0))
		PfeilUpPressed=1;
	else if(PfeilDown.Inside(mouseX,mouseY)&&(scrollY<maxScrollY))
		PfeilDownPressed=1;
};

void GraphixScrollWindow::OnMouseLeftUp()
{
	if(ScrollBalkenMove)
		moveScrollBalkenTo(0); //0?
	else
	if(PfeilUp.Inside(mouseX,mouseY))
	{
		if(PfeilUpPressed)
                        moveScrollBalkenTo(-10);
	}
	else
	if(PfeilDown.Inside(mouseX,mouseY))
	{
		if(PfeilDownPressed)
			moveScrollBalkenTo(10);
	}
	else if((!ScrollBalken.Inside(mouseX,mouseY))&&ScrollArea.Inside(mouseX,mouseY))
	{
		if(mouseY<ScrollBalken.y)
                        moveScrollBalkenTo(-maxScrollY/20);
		else
                        moveScrollBalkenTo(maxScrollY/20);
	}
        setScrollY(ScrollBalken.y-ScrollArea.y-17);
        PfeilUpPressed=0;
        PfeilDownPressed=0;
        ScrollBalkenMove=0;
}

void GraphixScrollWindow::setMouse(int x,int y)
{
	mouseX=x;mouseY=y;
}

void GraphixScrollWindow::moveScrollBalkenTo(int y)
{
	y+=ScrollBalken.y;
	if(y<ScrollArea.y+17) 
		ScrollBalken.y=ScrollArea.y+17;
	else if(y+ScrollBalken.height>=ScrollArea.y+ScrollArea.height-17)
		ScrollBalken.y=ScrollArea.y+ScrollArea.height-17-ScrollBalken.height;
	else ScrollBalken.y=y;
        setScrollY(ScrollBalken.y-ScrollArea.y-16);
}

