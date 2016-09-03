#include "graphics.h"


int GraphixScrollWindow::addBitmapButton(wxRect edge, wxBitmap& bitmap, int permButton)
{
	int i;
	for(i=0;i<1000;i++)
	if(!button[i].used)
	{
		button[i].permButton=permButton;
		button[i].edge=edge;
		button[i].used=1;
		button[i].bitmapButton=1;
		button[i].bitmap=&bitmap;
		button[i].pressed=0;
		return i;
	}
	return(0);
};

wxRect GraphixScrollWindow::getButtonRect(int num)
{
	if(!button[num].used) return(wxRect(0,0,0,0));
	else return(button[num].edge);
};

void GraphixScrollWindow::textButton(wxString bla, wxDC* dc)
{
        dc->DrawText(bla,getInnerLeftBound()+5,currentRow);
        int dx,dy;
        dc->GetTextExtent(bla,&dx,&dy);
        currentRow+=dy;
	addButton(wxRect(getInnerLeftBound()+5,currentRow,dx,dy));
};


void GraphixScrollWindow::newTextPage()
{
	currentRow=getInnerUpperBound();
};

void GraphixScrollWindow::writeText(wxString bla, wxDC* dc)
{
	dc->DrawText(bla,getInnerLeftBound()+5,currentRow);
	int dx,dy;
	dc->GetTextExtent(bla,&dx,&dy);
	currentRow+=dy;
};

int GraphixScrollWindow::addButton(wxRect edge, int permButton)
{
        int i;
        for(i=0;i<1000;i++)
        if(!button[i].used)
        {
                button[i].permButton=permButton;
                button[i].edge=edge;
                button[i].used=1;
                button[i].bitmapButton=0;
                button[i].pressed=0;
                return i;
        }
        return(0);
};


int GraphixScrollWindow::isPressed(int num)
{
	if((num>=1000)||(!button[num].used)) return(0);
	return(button[num].pressed);
}

/*
void GraphixScrollWindow::setButtonText(int num, const wxString text)
{
	if(!button[num].used) return;
	button[num].textButton=1;
	button[num].text=text;
};

void GraphixScrollWindow::setButtonBitmap(int num, const wxBitmap& bitmap)
{
	if(!button[num].used) return;
	button[num].textButton=0;
	button[num].bitmap=bitmap;
};

void GraphixScrollWindow::removeButton(int num)
{
	button[num].used=0;
};
*/

void GraphixScrollWindow::addTitle(int x, const char* Title)
{
        titlePosition.x=x;
        this->Title.Printf("%s",Title);
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

void GraphixScrollWindow::addTab(int x, const char* tab)
{
        if(tabNumber>=10) return;
	tabPosition[tabNumber]=x;
	tabButton[tabNumber]=addButton(wxRect(x,0,140,30),1);
        this->tab[tabNumber].Printf("%s",tab);
        tabNumber++;
};
                                                                                                                                                            
int GraphixScrollWindow::getScrollY()
{
        return(scrollY);
};

int GraphixScrollWindow::getUpperBound()
{
	return(Border.y+y);
};
int GraphixScrollWindow::getLowerBound()
{
        return(Border.y+Border.height+y);
};
int GraphixScrollWindow::getLeftBound()
{
        return(Border.x+x);
};
int GraphixScrollWindow::getRightBound()
{
        return(Border.x+Border.width+x);
}

int GraphixScrollWindow::getHeight()
{
	return(Border.height);
}

int GraphixScrollWindow::getWidth()
{
	return(Border.width);
}

int GraphixScrollWindow::getInnerUpperBound()
{
	return(clientArea.y+y);
};

int GraphixScrollWindow::getRelativeInnerUpperBound()
{
	return(clientArea.y);
};

int GraphixScrollWindow::getInnerLeftBound()
{
	return(clientArea.x+x);
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
	if(ScrollArea.height<=34) return; //~~~ argh 
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
	if(maxScrollY<=clientArea.height)
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
                                                                                                                                      
void GraphixScrollWindow::setBackground(wxBrush backgroundColour)
{
        Background=backgroundColour;
};

wxBrush GraphixScrollWindow::getBackground()
{
	return(Background);
}


void GraphixScrollWindow::setRahmenPen(wxColour rahmenPenColour)
{
        RahmenPen=rahmenPenColour;
	RahmenPen2=wxColour(rahmenPenColour.Red()+30,rahmenPenColour.Green()+60,rahmenPenColour.Blue()+30);
};
                                                                                                                                      
void GraphixScrollWindow::setRahmen(wxRect rahmen)
{
	Border.x=rahmen.x;
	Border.y=rahmen.y;
	Border.width=rahmen.width;
	Border.height=rahmen.height;
        Rahmen.x=rahmen.x+3;
	Rahmen.y=rahmen.y+8;
	Rahmen.width=rahmen.width-6;
	Rahmen.height=rahmen.height-11;
	OuterRahmen.x=rahmen.x+1;
	OuterRahmen.y=rahmen.y+6;
	OuterRahmen.width=rahmen.width-2;
	OuterRahmen.height=rahmen.height-7;
};

void GraphixScrollWindow::DrawButtons(wxDC* dc)
{
	int i;
        for(i=0;i<1000;i++)
        if(button[i].used)
        {
                if(button[i].bitmapButton)
//              else
                {
        //                dc->DrawRoundedRectangle(button[i].x-1,button[i].y-1,10,10,3);
                        dc->DrawBitmap(*button[i].bitmap,button[i].edge.x,button[i].edge.y,false);
                }
        }
}
                

void GraphixScrollWindow::clearButtons()
{
	int i;
	for(i=0;i<1000;i++)
	if(button[i].used)
	{
	        if(!button[i].permButton) button[i].used=0;
		else button[i].pressed=0;
		
	}
}

void GraphixScrollWindow::drawArrow(wxDC* dc, int x,int y, int dir, int length)
{
	int tx,ty,t1x,t1y,t2x,t2y;
	switch(dir)
	{
		case 0:tx=x-length;ty=y;t1x=tx+length/5;t2x=t1x;t1y=ty-length/5;t2y=ty+length/5;break;
		case 1:tx=x;ty=y+length;t1x=tx+length/5;t2x=tx-length/5;t1y=ty-length/5;t2y=t1y;break;
		case 2:tx=x+length;ty=y;t1x=tx-length/5;t2x=t1x;t1y=ty-length/5;t2y=ty+length/5;break;
		case 3:tx=x;ty=y-length;t1x=tx+length/5;t2x=tx-length/5;t1y=ty+length/5;t2y=t1y;break;
	};
	dc->DrawLine(x+getInnerLeftBound(),y+getInnerUpperBound(),tx+getInnerLeftBound(),ty+getInnerUpperBound());
	dc->DrawLine(tx+getInnerLeftBound(),ty+getInnerUpperBound(),t1x+getInnerLeftBound(),t1y+getInnerUpperBound());
	dc->DrawLine(tx+getInnerLeftBound(),ty+getInnerUpperBound(),t2x+getInnerLeftBound(),t2y+getInnerUpperBound());
};
                                                                                                                      

void GraphixScrollWindow::Draw(wxDC* dc)
{
	if(!isShown())
		return;
	dc->SetFont(font2);
        int i;
	if(WindowMove)
		adjustWindow(wxRect(mouseX-WindowMoveX,mouseY-WindowMoveY,Border.width,Border.height));

	if(targetx>x) 
	{
		if(x-startx>(targetx-startx)/2) x+=1+(x-startx)/2;
		else x+=1+(targetx-x)/2;
	}
	else if(targetx<x)
        {
                if(startx-x>(startx-targetx)/2) x-=1+(startx-x)/2;
                else x-=1+(x-targetx)/2;
        }

        if(targety>y)
        {
                if(y-starty>(targety-starty)/2) y+=1+(y-starty)/2;
                else y+=1+(targety-y)/2;
        }
        else if(targety<y)
        {
                if(starty-y>(starty-targety)/2) y-=1+(starty-y)/2;
                else y-=1+(y-targety)/2;
        }

        if(targetwidth>Border.width)
        {
                if(Border.width-startwidth>(targetwidth-startwidth)/2) Border.width+=1+(Border.width-startwidth)/2;
                else Border.width+=1+(targetwidth-Border.width)/2;
        }
        else if(targetwidth<Border.width)
        {
                if(startwidth-Border.width>(startwidth-targetwidth)/2) Border.width-=1+(startwidth-Border.width)/2;
                else Border.width-=1+(Border.width-targetwidth)/2;
        }

        if(targetheight>Border.height)
        {
                if(Border.height-startheight>(targetheight-startheight)/2) Border.height+=1+(Border.height-startheight)/2;
                else Border.height+=1+(targetheight-Border.height)/2;
        }
        else if(targetheight<Border.height)
        {
                if(startheight-Border.height>(startheight-targetheight)/2) Border.height-=1+(startheight-Border.height)/2;
                else Border.height-=1+(Border.height-targetheight)/2;
        }



/*

        if(targetcx>clientArea.x)
        {
                if(clientArea.x-startcx>(targetcx-startcx)/2) clientArea.x+=1+(clientArea.x-startcx)/2;
                else clientArea.x+=1+(targetcx-clientArea.x)/2;
        }
        else if(targetcx<clientArea.x)
        {
                if(startcx-clientArea.x>(startcx-targetcx)/2) clientArea.x-=1+(startcx-clientArea.x)/2;
                else clientArea.x-=1+(clientArea.x-targetcx)/2;
        }
                                                                                                                                                            
        if(targetcy>clientArea.y)
        {
                if(clientArea.y-startcy>(targetcy-startcy)/2) clientArea.y+=1+(clientArea.y-startcy)/2;
                else clientArea.y+=1+(targetcy-clientArea.y)/2;
        }
        else if(targetcy<clientArea.y)
        {
                if(startcy-clientArea.y>(startcy-targetcy)/2) clientArea.y-=1+(startcy-clientArea.y)/2;
                else clientArea.y-=1+(clientArea.y-targetcy)/2;
        }*/
                                                                                                                                                            
        if(targetcwidth>clientArea.width)
        {
                if(clientArea.width<(targetcwidth-startcwidth)/2) clientArea.width+=1+(clientArea.width-startcwidth)/2;
                else clientArea.width+=1+(targetcwidth-clientArea.width)/2;
        }
        else if(targetcwidth<clientArea.width)
        {
                if(clientArea.width>(startcwidth-targetcwidth)/2) clientArea.width-=1+(startcwidth-clientArea.width)/2;
                else clientArea.width-=1+(clientArea.width-targetcwidth)/2;
        }
                                                                                                                                                            
        if(targetcheight>clientArea.height)
        {
                if(clientArea.height-startcheight>(targetcheight-startcheight)/2) clientArea.height+=1+(clientArea.height-startcheight)/2;
                else clientArea.height+=1+(targetcheight-clientArea.height)/2;
        }
        else if(targetcheight<clientArea.height)
        {
                if(startcheight-clientArea.height>(startcheight-targetcheight)/2) clientArea.height-=1+(startcheight-clientArea.height)/2;
                else clientArea.height-=1+(clientArea.height-targetcheight)/2;
        }


	updateWindow();

/*	if(WindowMove)
	{
		startx=x;starty=y;
	}*/

        setMaxScrollY(lastEntry);
                                                                                                                                                            
        if(ScrollBalkenMove)
                moveScrollBalkenTo(mouseY-ScrollBalkenMoveY-ScrollBalken.y);
        else moveScrollBalkenTo(0);

	
//erstmal Rahmen:
        dc->SetPen(wxPen(RahmenPen,1,wxSOLID));

        dc->SetBrush(getBackground());
        dc->DrawRoundedRectangle(OuterRahmen.x+x,OuterRahmen.y+y,OuterRahmen.width,OuterRahmen.height,3);
        if(Rahmen.Inside(mouseX-x,mouseY-y))
		dc->SetPen(wxPen(RahmenPen2,1,wxSOLID));
	dc->DrawRoundedRectangle(Rahmen.x+x,Rahmen.y+y,Rahmen.width,Rahmen.height,3);

if(!tabNumber)
{
        dc->SetBrush(wxBrush(wxColour(0,0,0),wxSOLID));
	dc->SetPen(wxPen(wxColour(0,0,0),0,wxTRANSPARENT));
	dc->DrawRoundedRectangle(OuterRahmen.x+x-2,OuterRahmen.y+y+OuterRahmen.height/4,8,OuterRahmen.height/2,4);
        dc->DrawRoundedRectangle(OuterRahmen.x+x+OuterRahmen.width/4,OuterRahmen.y+y+OuterRahmen.height-6,OuterRahmen.width/2,8,4);
        dc->DrawRoundedRectangle(OuterRahmen.x+x-6+OuterRahmen.width,OuterRahmen.y+y+OuterRahmen.height/4,8,OuterRahmen.height/2,4);
        dc->DrawRoundedRectangle(OuterRahmen.x+x+OuterRahmen.width/4,OuterRahmen.y+y-2,OuterRahmen.width/2,8,4);
        dc->SetBrush(getBackground());
        dc->SetPen(wxPen(wxColour(0,0,0),0,wxTRANSPARENT));
        dc->DrawRoundedRectangle(OuterRahmen.x+x-2,OuterRahmen.y+y+OuterRahmen.height/4,8,OuterRahmen.height/2,4);
        dc->DrawRoundedRectangle(OuterRahmen.x+x+OuterRahmen.width/4,OuterRahmen.y+y+OuterRahmen.height-6,OuterRahmen.width/2,8,4);
        dc->DrawRoundedRectangle(OuterRahmen.x+x-6+OuterRahmen.width,OuterRahmen.y+y+OuterRahmen.height/4,8,OuterRahmen.height/2,4);
        dc->DrawRoundedRectangle(OuterRahmen.x+x+OuterRahmen.width/4,OuterRahmen.y+y-2,OuterRahmen.width/2,8,4);
}


//	dc->DrawRectangle(clientArea.x+x,clientArea.y+y,clientArea.width,clientArea.height);
//	dc->DrawRoundedRectangle(targetcx+x,targetcy+y,targetcwidth,targetcheight,4);

//dann Schrift:
        dc->SetBrush(wxBrush(wxColour(0,0,0),wxSOLID));
        dc->SetTextForeground(TitleColour);
if(!tabNumber)
{
	int dx,dy;
	dc->GetTextExtent(Title,&dx,&dy);
	dx=dx+3;
	titlePosition.y=-dy/2-1;
	titlePosition.width=dx+2;
	titlePosition.height=dy+2;
	dc->DrawRoundedRectangle(Rahmen.x+x+titlePosition.x-2,Rahmen.y+y+titlePosition.y-2,titlePosition.width,titlePosition.height,3);
        dc->DrawText(Title,Rahmen.x+x+titlePosition.x,Rahmen.y+y+titlePosition.y);
}

//	int bla=dy;

	for(i=tabNumber;i--;)
		{
                        dc->SetPen(wxPen(RahmenPen,1,wxSOLID));
			if(i==currentTab)
			        dc->SetTextForeground(wxColour(150,150,200));
			else
			        dc->SetTextForeground(TitleColour);
			wxRect edge=wxRect(tabPosition[i],0,140,30);
		        dc->SetBrush(wxBrush(wxColour(0,0,0),wxSOLID));
			dc->DrawRectangle(edge);
			if(i==currentTab)
	                        dc->SetBrush(getBackground());

        		if(edge.Inside(mouseX-x,mouseY-y))
		                dc->SetPen(wxPen(RahmenPen2,1,wxSOLID));

			dc->DrawRectangle(wxRect(edge.x+3,edge.y+3,edge.width-6,edge.height-6));
		
		        dc->SetFont(font4);
			int dx,dy;
			dc->GetTextExtent(tab[i],&dx,&dy);
			dc->DrawText(tab[i],edge.x+(edge.width-dx)/2,edge.y+(edge.height-dy)/2);
			if(i==currentTab)
			{
	                        dc->SetBrush(wxBrush(wxColour(0,0,0),wxSOLID));
				dc->SetPen(wxPen(wxColour(0,0,0),0,wxTRANSPARENT));
				dc->DrawRectangle(wxRect(edge.x+1,edge.y+edge.height-4,edge.width-1,6));
	                        dc->SetBrush(getBackground());
				dc->DrawRectangle(wxRect(edge.x+1,edge.y+edge.height-4,edge.width-1,6));
			}
		}

        dc->SetBrush(getBackground());

/*        for(i=0;i<descriptionNumber;i++)
        {
                dc->GetTextExtent(Description[i],&dx,&dy);
		dx=dx+3;
                dc->DrawRoundedRectangle(Rahmen.x+x+descriptionPosition[i]-2,bla+Rahmen.y+y-dy/2-2,dx+2,dy+2,3);
                dc->DrawText(Description[i],Rahmen.x+x+descriptionPosition[i],bla+Rahmen.y+y-dy/2-1);
        }*/

	if(!scrolled) return;

//        disabledItem=wxColour(10,10,10);
	disabledItemPen=wxColour(70,70,70);
//(RahmenPen.Red+RahmenPen.Green+RahmenPen.Blue)/3,(RahmenPen.Red+RahmenPen.Green+RahmenPen.Blue)/3,(RahmenPen.Red+RahmenPen.Green+RahmenPen.Blue)/3);
        enabledItemPen=RahmenPen;
        mouseOverItem=RahmenPen;
        clickedItem=wxColour(200,200,200);

	dc->DrawRoundedRectangle(ScrollArea.x+x,ScrollArea.y+y,ScrollArea.width,ScrollArea.height,3);

	if(ScrollBalkenMove)
	{
		dc->SetBrush(wxBrush(clickedItem,wxSOLID));
		dc->SetPen(wxPen(mouseOverItem,1,wxSOLID));
	} else if(ScrollBalken.Inside(mouseX-x,mouseY-y))
	{
	        dc->SetBrush(wxBrush(mouseOverItem,wxSOLID));
                dc->SetPen(wxPen(clickedItem,1,wxSOLID));
	} else
        {
        	dc->SetBrush(getBackground());
                dc->SetPen(wxPen(RahmenPen,1,wxSOLID));
        }
	dc->DrawRoundedRectangle(ScrollBalken.x+x,ScrollBalken.y+y,ScrollBalken.width,ScrollBalken.height,4);

        dc->SetPen(wxPen(RahmenPen,1,wxSOLID));
        dc->SetBrush(getBackground());
        dc->DrawRoundedRectangle(PfeilUp.x+x,PfeilUp.y+y,PfeilUp.width,PfeilUp.height,4);
	if(PfeilUpPressed)
	{
		dc->SetBrush(wxBrush(clickedItem,wxSOLID));
		dc->SetPen(wxPen(mouseOverItem,1,wxSOLID));
	}
	else
	{
	        if(scrollY<=0)
		{
			dc->SetBrush(getBackground());
			dc->SetPen(wxPen(disabledItemPen,1,wxSOLID));
		}
	        else if(PfeilUp.Inside(mouseX-x,mouseY-y))
		{
			dc->SetBrush(wxBrush(mouseOverItem,wxSOLID));
			dc->SetPen(wxPen(clickedItem,1,wxSOLID));
		}
	        else
		{
			dc->SetBrush(getBackground());
			dc->SetPen(wxPen(RahmenPen,1,wxSOLID));
		}
	}
        wxPoint points[3];
        points[0].x=PfeilUp.width/2;points[0].y=2;
        points[1].x=2;points[1].y=PfeilUp.height-3;
        points[2].x=PfeilUp.width-3;points[2].y=PfeilUp.height-3;
	dc->DrawPolygon(3,points,PfeilUp.x+x,PfeilUp.y+y);

        dc->SetPen(wxPen(RahmenPen,1,wxSOLID));
        dc->SetBrush(getBackground());
        dc->DrawRoundedRectangle(PfeilDown.x+x,PfeilDown.y+y,PfeilDown.width,PfeilDown.height,4);
        if(PfeilDownPressed)
        {
                dc->SetBrush(wxBrush(clickedItem,wxSOLID));
                dc->SetPen(wxPen(mouseOverItem,1,wxSOLID));
        }
        else
        {
                if(scrollY>=maxScrollY-(clientArea.height))
                {
                        dc->SetBrush(getBackground());
                        dc->SetPen(wxPen(disabledItemPen,1,wxSOLID));
                }
                else if(PfeilDown.Inside(mouseX-x,mouseY-y))
                {
                        dc->SetBrush(wxBrush(mouseOverItem,wxSOLID));
                        dc->SetPen(wxPen(clickedItem,1,wxSOLID));
		}
                else
                {
                        dc->SetBrush(getBackground());
                        dc->SetPen(wxPen(RahmenPen,1,wxSOLID));
                }
        }
        points[0].x=PfeilDown.width/2;points[0].y=PfeilDown.height-3;
        points[1].x=2;points[1].y=2;
        points[2].x=PfeilDown.width-3;points[2].y=2;
        dc->DrawPolygon(3,points,PfeilDown.x+x,PfeilDown.y+y);
};

bool GraphixScrollWindow::insideClientArea(int x,int y)//wxRect rectangle) //for all non-buttons/titles etc.
{
	return(clientArea.Inside(x-this->x,y-this->y));
//	return(clientArea.Inside(rectangle.x,rectangle.y)||clientArea.Inside(rectangle.x+rectangle.width,rectangle.y+rectangle.height)||clientArea.Inside(rectangle.x+rectangle.width,rectangle.y)||clientArea.Inside(rectangle.x,rectangle.y+rectangle.height));
	//TODO: wenns auf beiden Seiten ueberlappt?
};

bool GraphixScrollWindow::fitToClientArea(wxRect& rectangle)
{
	if(rectangle.x<0) rectangle.x=0;
        if(rectangle.width+rectangle.x>getInnerWidth()) rectangle.width=getInnerWidth()-rectangle.x;
//	if(rectangle.y<clientArea.y) rectangle.y=clientArea.y;
  //      if(rectangle.y+rectangle.height>clientArea.y+clientArea.height) rectangle.height=clientArea.y+clientArea.height-rectangle.y;
	lastEntry=rectangle.y+rectangle.height;
        if((rectangle.width<=0)||(rectangle.y<0)||(rectangle.y+rectangle.height>getInnerHeight())) return false;
	else return true;
}

void GraphixScrollWindow::moveToClientArea(wxRect& rectangle)
{
        if(rectangle.x<getInnerLeftBound()) rectangle.x=getInnerLeftBound();
	if(rectangle.x+rectangle.width>getInnerLeftBound()+getInnerWidth())
		rectangle.x=getInnerLeftBound()+getInnerWidth()-rectangle.width;
	if(rectangle.y<getInnerUpperBound()) rectangle.y=getInnerUpperBound();
	if(rectangle.y+rectangle.height>getInnerUpperBound()+getInnerHeight())
		rectangle.y=getInnerUpperBound()+getInnerHeight()-rectangle.height;
}


/*
void GraphixScrollWindow::adjustElements()
{
	
}*/

GraphixScrollWindow::GraphixScrollWindow()
{
};

GraphixScrollWindow::GraphixScrollWindow(int level, wxRect rahmen, wxRect maxSize, int x, int y, int scrolled, int tabbed, wxRect clientArea)
{
	int i;
	for(i=0;i<1000;i++)
	{
		button[i].used=0;
		button[i].pressed=0;
	}
	targetx=x;
	targety=y;
	this->x=x;
	this->y=y;
        this->scrolled=scrolled;
        setTitleColour(wxColour(100,100,200));
	switch(level)
	{
		case 0:setBackground(wxBrush(wxColour(5,25,0),wxCROSS_HATCH));break;
		case 1:setBackground(wxBrush(wxColour(5,25,80),wxCROSS_HATCH));break; //!! TODO Warum loescht mir gtk meinen hintergrund auch noch?!
		case 2:setBackground(wxBrush(wxColour(5,25,40),wxSOLID));break; //infowindow
		default:break;
	}
        setRahmenPen(wxColour(40,120,30));
	if(tabbed)
	{
		rahmen.y+=21;
		rahmen.height-=21;
	}
	
		
        setRahmen(rahmen);
	targetwidth=Border.width;
	targetheight=Border.height;

	startwidth=Border.width;
	startheight=Border.height;
	startx=x;
	starty=y;

        ScrollArea=wxRect(Rahmen.x+Rahmen.width-18,Rahmen.y+2,16,Rahmen.height-4);
	ScrollBalken=wxRect(ScrollArea.x+1,ScrollArea.y+17,ScrollArea.width-2,ScrollArea.height-34);
	PfeilUp=wxRect(ScrollArea.x,ScrollArea.y,ScrollArea.width,ScrollArea.width);
	PfeilDown=wxRect(ScrollArea.x,ScrollArea.y+ScrollArea.height-ScrollArea.width,ScrollArea.width,ScrollArea.width);
        //TODO Resize, Move etc.
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

	tabNumber=0;
	currentTab=0;
//	if(tabbed)
//		if(clientArea.y<Rahmen.y+30) {clientArea.height-=Rahmen.y+30-clientArea.y;clientArea.y=Rahmen.y+30;};

//	if(description)
//		if(clientArea.y<Rahmen.y+16) { clientArea.height-=Rahmen.y+16-clientArea.y;clientArea.y=Rahmen.y+16;}
	
	this->clientArea=clientArea;
	originalClientArea=clientArea;
	originalBorder=Border;
	ScrollBalkenMove=0;
        PfeilUpPressed=0;PfeilDownPressed=0;
	setMaxScrollY(1500);
	this->maxSize=maxSize;
//	maxcheight=maxheight-Rahmen.height+clientArea.height-2*clientArea.y;
	targetcx=clientArea.x;targetcy=clientArea.y;
	targetcwidth=clientArea.width;targetcheight=clientArea.height;
	startcx=clientArea.x;startcy=clientArea.y;
	startcwidth=clientArea.width;startcheight=clientArea.height;
//	descriptionNumber=0;
	setFreeMove(0);
	Show(0);
	clearButtons();
	lastEntry=clientArea.height;
};


void GraphixScrollWindow::OnScrollMouse(int msy)
{
	if(!isShown()) return;
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
	if(!isShown())
		return;
        PfeilUpPressed=0;
        PfeilDownPressed=0;
        ScrollBalkenMove=0;
	if(ScrollBalken.Inside(mouseX-x,mouseY-y))
	{
		ScrollBalkenMove=1;
                ScrollBalkenMoveY=mouseY-ScrollBalken.y;
	}
	else if(PfeilUp.Inside(mouseX-x,mouseY-y)&&(scrollY>0))
		PfeilUpPressed=1;
	else if(PfeilDown.Inside(mouseX-x,mouseY-y)&&(scrollY<maxScrollY))
		PfeilDownPressed=1;
	else
	if(titlePosition.Inside(mouseX-x,mouseY-y))
	{
		WindowMove=1;
//		startx=x;starty=y;startheight=Border.height;startwidth=Border.width;
		WindowMoveX=mouseX-x;
		WindowMoveY=mouseY-y;
	}
	else
	{
		int i;
		for(i=0;i<1000;i++)
			if((button[i].used)&&(button[i].edge.Inside(mouseX,mouseY)))
				button[i].pressed+=1+button[i].pressed/5;
	}
};

void GraphixScrollWindow::OnMouseLeftUp()
{
	if(!isShown()) return;
	if(ScrollBalkenMove)
	{
		moveScrollBalkenTo(0); //0?
	        setScrollY(ScrollBalken.y-ScrollArea.y-17);
	}
	else
	if(PfeilUp.Inside(mouseX-x,mouseY-y))
	{
		if(PfeilUpPressed)
		{
                        moveScrollBalkenTo(-10);
		        setScrollY(ScrollBalken.y-ScrollArea.y-17);
		}
	}
	else
	if(PfeilDown.Inside(mouseX-x,mouseY-y))
	{
		if(PfeilDownPressed)
		{
			moveScrollBalkenTo(10);
		        setScrollY(ScrollBalken.y-ScrollArea.y-17);
		}
	}
	else if((!ScrollBalken.Inside(mouseX-x,mouseY-y))&&ScrollArea.Inside(mouseX-x,mouseY-y))
	{
		if(mouseY<ScrollBalken.y)
                        moveScrollBalkenTo(-maxScrollY/20);
		else
                        moveScrollBalkenTo(maxScrollY/20);
	        setScrollY(ScrollBalken.y-ScrollArea.y-17);
	}
	else if(WindowMove)
	{
		WindowMove=0;
	}
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

void GraphixScrollWindow::transformClientWidth(int width) //handle with care O_O
{
	originalClientArea.width+=width;
}

void GraphixScrollWindow::transformClientHeight(int height) //handle with care O_O
{
        originalClientArea.height+=height;
}
/*
void GraphixScrollWindow::adjustClientWindow(int width, int height)
{
//        if(edge.x!=targetcx)
  //            startcx=clientArea.x;
//        if(edge.y!=targetcy)
  //            startcy=clientArea.y;
       if(height!=targetcheight)
//||((targetcheight>maxcheight)&&(edge.height!=maxcheight))) already checked in adjustWindow
              startcheight=clientArea.height;
        if(width!=targetcwidth)
                startcwidth=clientArea.width;
//      targetcx=edge.x;
  //     targetcy=edge.y;
        targetcwidth=width;
//        if(edge.height<=maxcheight)
              targetcheight=height;
  //  else targetcheight=maxcheight;
}*/



void GraphixScrollWindow::adjustWindow(wxRect edge)
{
//Only 1 change (X OR WIDTH, Y OR HEIGHT) is allowed!
	if(edge.width==targetwidth) // nur x ist unterschiedlich
	{
		if(((edge.x!=targetx)&&(edge.x+edge.width<maxSize.x+maxSize.width)) // es ist ein neues Ziel
		||((edge.x+edge.width<maxSize.x+maxSize.width)&&(targetx+targetwidth!=maxSize.x+maxSize.width)) // oder einfach ausserhalb der Begrenzung aber unser Ziel war nicht die Begrenzung
		||((edge.x<maxSize.x)&&(targetx!=maxSize.x))
		||((edge.x!=targetx)&&(freeMove)))
			startx=x; //Dann setze neuen Startpunkt
		if((edge.x+edge.width>maxSize.x+maxSize.width)&&(!freeMove))
			edge.x=maxSize.x+maxSize.width-edge.width;
	} else if(edge.x==targetx)
	{
		if(((edge.width!=targetwidth)&&(edge.x+edge.width<maxSize.x+maxSize.width))
		||((edge.x+edge.width>maxSize.x+maxSize.width)&&(targetx+targetwidth!=maxSize.x+maxSize.width))
		||((edge.width!=targetwidth)&&(freeMove)))
			startwidth=Border.width;
                if((edge.x+edge.width>maxSize.x+maxSize.width)&&(!freeMove))
                        edge.width=maxSize.x+maxSize.width-edge.x;
	} //else ?

	if(edge.height==targetheight)
	{
	        if(((edge.y!=targety)&&(edge.y+edge.height<maxSize.y+maxSize.height))
        	||((edge.y+edge.height<maxSize.y+maxSize.width)&&(targety+targetheight!=maxSize.y+maxSize.height))
		||((edge.y<maxSize.y)&&(targety!=maxSize.y))
		||((edge.y!=targety)&&(freeMove)))
			starty=y;
                if((edge.y+edge.height>maxSize.y+maxSize.height)&&(!freeMove))
                        edge.y=maxSize.y+maxSize.height-edge.height;
	} else if(edge.y==targety)
	{
		if(((edge.height!=targetheight)&&(edge.y+edge.height<maxSize.y+maxSize.height))
		||((edge.y+edge.height>maxSize.y+maxSize.height)&&(targety+targetheight!=maxSize.y+maxSize.height))
		||((edge.height!=targetheight)&&(freeMove)))
			startheight=Border.height;
                if((edge.y+edge.height>maxSize.y+maxSize.height)&&(!freeMove))
                        edge.height=maxSize.y+maxSize.height-edge.y;
	} //else ... mmmh... 

	if((edge.x<maxSize.x)||(freeMove))
		targetx=maxSize.x;
	else targetx=edge.x;
	if((edge.y<maxSize.y)||(freeMove))
		targety=maxSize.y;
	else targety=edge.y;
	
        targetwidth=edge.width;
	targetheight=edge.height;

//CLIENT WINDOW
	int height=targetheight-originalBorder.height+originalClientArea.height;
	int width=targetwidth-originalBorder.width+originalClientArea.width;
        if(height!=targetcheight)
              startcheight=clientArea.height;
        if(width!=targetcwidth)
                startcwidth=clientArea.width;
        targetcwidth=width;
        targetcheight=height;
}

void GraphixScrollWindow::updateWindow()
{
	wxRect oldRahmen=Rahmen;
        setRahmen(Border);


        ScrollArea=wxRect(Rahmen.x+Rahmen.width-18,Rahmen.y+2,16,Rahmen.height-4);
//      ScrollBalken=wxRect(ScrollArea.x+1,ScrollArea.y+17,ScrollArea.width-2,ScrollArea.height-34);
        PfeilUp=wxRect(ScrollArea.x,ScrollArea.y,ScrollArea.width,ScrollArea.width);
        PfeilDown=wxRect(ScrollArea.x,ScrollArea.y+ScrollArea.height-ScrollArea.width,ScrollArea.width,ScrollArea.width);
//        clientArea.x=Rahmen.x+5;
  //      if(scrolled)
    //            clientArea.width=ScrollArea.x-3-clientArea.x;
//        else
  //              clientArea.width=Rahmen.x+Rahmen.width-3-clientArea.x;

/*	if(tabNumber)
	{
		
	}*/ //~~

    /*    if(descriptionNumber)
        { 
		clientArea.height-=Rahmen.y+16-clientArea.y;
		clientArea.y=Rahmen.y+16;
	}
	else
	{
	        clientArea.y+=Rahmen.y-oldRahmen.y;
        	clientArea.height+=Rahmen.y+Rahmen.height-oldRahmen.y-oldRahmen.height;
	}*/
}

void GraphixScrollWindow::setFreeMove(int move)
{
	if(move)
		freeMove=1;
	else freeMove=0;
}
int GraphixScrollWindow::isShown()
{
	return shown;
};

void GraphixScrollWindow::Show(int show)
{
	if(show)
		shown=1;
	else shown=0;
};


wxFont GraphixScrollWindow::font;
wxFont GraphixScrollWindow::font2;
wxFont GraphixScrollWindow::font3;
wxFont GraphixScrollWindow::font4;
wxFont GraphixScrollWindow::font5;
wxFont GraphixScrollWindow::font6;
