#include "graphics.h"
#include "util.h"

wxRect GraphixScrollWindow::getTarget()
{
	return(target);
};

int GraphixScrollWindow::addBitmapButton(wxRect edge, wxBitmap& bitmap, int permButton)
{
	for(int i=0;i<1000;i++)
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

void GraphixScrollWindow::writeHeader(wxString text, wxDC* dc, int mode)
{
	dc->SetFont(GraphixScrollWindow::font4);
	dc->SetTextForeground(wxColour(100,100,200));
	newTextPage();
	dc->SetFont(GraphixScrollWindow::font5);
	writeText(text,dc);
	writeText(_T(" "),dc);
	dc->SetFont(GraphixScrollWindow::font4);
	if(mode>0) textButton(_T("<- back"),dc);
	if(mode==2)
		dc->SetFont(GraphixScrollWindow::font2);
			
};

void GraphixScrollWindow::writeLongText(wxString text, wxDC* dc, wxColour colour)
{
	wxColour normalColour, boldColour;
	normalColour=colour;
	boldColour=wxColour(colour.Red()+50,colour.Green()+50,colour.Blue()+55);
	dc->SetTextForeground(normalColour);
	int lastChar, firstChar, bold;
	int twidth=0;
	textCursor.x=5;
	firstChar=0;
	lastChar=0;
	bold=0;
	for(int i=0;i<text.Length();i++)
	{
		int newLine=0;
		if(text.GetChar(i)==' ') 
		{
			lastChar=i;
			twidth=textCursor.x;
		}
		else if(text.GetChar(i)=='#')
		{
			lastChar=i;
			twidth=(textCursor.x+getInnerWidth())/2;
			newLine=1;
		}
		dc->GetTextExtent(_T(wxString::Format(wxT("%c"),text.GetChar(i))),&textCursor.width,&textCursor.height);
		textCursor.x+=textCursor.width;
		if((textCursor.x>getInnerWidth()-5)||(newLine))
		{
			textCursor.x=5;
			int d=0;
			for(int j=firstChar;j<lastChar;j++)
			{
				dc->GetTextExtent(_T(wxString::Format(wxT("%c"),text.GetChar(j))),&textCursor.width,&textCursor.height);
				if(text.GetChar(j)=='$')
				{
					if(bold){bold=0;dc->SetTextForeground(normalColour);} else
						{bold=1;dc->SetTextForeground(boldColour);}
					continue;
				}
				if(text.GetChar(j)=='#') break;
				dc->DrawText(_T(wxString::Format(wxT("%c"),text.GetChar(j))),getInnerPosition()+textCursor.GetPosition()+wxPoint(d/(lastChar-firstChar-1),0));
				textCursor.x+=textCursor.width;
				if(!newLine)
					d+=(getInnerWidth()-10-twidth);
			}
			textCursor.y+=textCursor.height+2;
			wxRect edge=wxRect(textCursor.GetPosition()+getInnerPosition(),textCursor.GetSize());
			if(!fitToClientArea(edge,1))
				return;
			twidth=0;
			firstChar=lastChar+1;
			i=lastChar;
			lastChar=0;
			textCursor.x=5;
		}
	}
};

wxRect GraphixScrollWindow::getButtonRect(int num)
{
	if(!button[num].used) return(wxRect(0,0,0,0));
	else return(button[num].edge);
};

void GraphixScrollWindow::textButton(wxString text, wxDC* dc)
{
	dc->GetTextExtent(text,&textCursor.width,&textCursor.height);
	wxRect edge=wxRect(textCursor.GetPosition()+getInnerPosition()+wxPoint(5,0),textCursor.GetSize());
	if(!fitToClientArea(edge,1))
    	return;
// TODO: echten Button mit Status Ueberpruefung einfuehren
	if(edge.Inside(controls.getCurrentPosition()))
		dc->SetTextForeground(wxColour(200,200,220));
	else
		dc->SetTextForeground(wxColour(100,100,200));
	dc->DrawText(text,edge.GetPosition());
	addButton(edge);
	textCursor.y+=textCursor.height+5;
};


void GraphixScrollWindow::newTextPage()
{
	textCursor.y=getRelativeInnerUpperBound();
};

void GraphixScrollWindow::writeText(wxString text, wxDC* dc)
{
	dc->GetTextExtent(text,&textCursor.width,&textCursor.height);
	wxRect edge=wxRect(textCursor.GetPosition()+getInnerPosition()+wxPoint(5,0),textCursor.GetSize());
	if(!fitToClientArea(edge,1))
		return;
	dc->DrawText(text, edge.GetPosition());
	textCursor.y+=textCursor.height+3;
};

int GraphixScrollWindow::addButton(wxRect edge, int permButton)
{
	for(int i=0;i<1000;i++)
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

int GraphixScrollWindow::getPressCondition(int num)
{
	if((num>=1000)||(!button[num].used)) return(0);
	return(button[num].pressed);
};

int GraphixScrollWindow::isActivated(int num)
{
	if((num>=1000)||(!button[num].used)) return(0);
	if(button[num].pressed==4)
		return(1);
	else return(0);
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

void GraphixScrollWindow::setTitle(int x, const char* Title)
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
	return(0);
//	return(scrollY);
};

int GraphixScrollWindow::getUpperBound()
{
	return(rect.y);
};
int GraphixScrollWindow::getLowerBound()
{
	return(rect.y+rect.height);
};
int GraphixScrollWindow::getLeftBound()
{
	return(rect.x);
};
int GraphixScrollWindow::getRightBound()
{
	return(rect.x+rect.width);
};

int GraphixScrollWindow::getHeight()
{
	return(rect.height);
};

int GraphixScrollWindow::getTargetWidth()
{
	return(target.width);
};

int GraphixScrollWindow::getTargetHeight()
{
	return(target.height);
};

int GraphixScrollWindow::getWidth()
{
	return(rect.width);
}

int GraphixScrollWindow::getInnerUpperBound()
{
	return(innerRect.y+rect.y);
};

int GraphixScrollWindow::getRelativeInnerUpperBound()
{
	return(innerRect.y);
};

int GraphixScrollWindow::getInnerLeftBound()
{
	return(innerRect.x+rect.x);
};

int GraphixScrollWindow::getInnerHeight()
{
	return(innerRect.height);
};

int GraphixScrollWindow::getInnerWidth()
{
	return(innerRect.width);
};


wxPoint GraphixScrollWindow::getPosition()
{
		return(rect.GetPosition());
};
																										                                                    
wxSize GraphixScrollWindow::getSize()
{
		return(rect.GetSize());
};

wxPoint GraphixScrollWindow::getInnerPosition()
{
	return(innerRect.GetPosition()+rect.GetPosition());
};

wxSize GraphixScrollWindow::getInnerSize()
{
	return(innerRect.GetSize());
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
	if(maxScrollY<=innerRect.height)
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


void GraphixScrollWindow::setRahmen(wxRect rahmen)
{
	rect=rahmen;
	Rahmen=wxRect(wxPoint(3,8),rahmen.GetSize()+wxSize(-6,-11));
	OuterRahmen=wxRect(wxPoint(1,6),rahmen.GetSize()+wxSize(-2,-7));	
};

void GraphixScrollWindow::DrawButtons(wxDC* dc)
{
	for(int i=0;i<1000;i++)
	if(button[i].used)
	{
		if(button[i].bitmapButton)
			dc->DrawBitmap(*button[i].bitmap,button[i].edge.GetPosition(),false);
//		else			
//			dc->DrawRoundedRectangle(button[i].edge,3);
	}
}
		

void GraphixScrollWindow::clearButtons()
{
	for(int i=0;i<1000;i++)
		if(button[i].used)
		{
			if(!button[i].permButton) button[i].used=0;
			else button[i].pressed=0;
		}
}

void GraphixScrollWindow::drawArrow(wxDC* dc, wxPoint position, int dir, int length)
{
	wxPoint t,t1,t2;
	switch(dir)
	{
		case 0:t=position-wxPoint(length,0);t1=t+wxPoint(length/5,-length/5);t2=t1+wxPoint(0,length/5);break;
		case 1:t=position+wxPoint(0,length);t1=t+wxPoint(length/5,-length/5);t2=t1+wxPoint(-length/5,0);break;
		case 2:t=position+wxPoint(length,0);t1=t-wxPoint(length/5,length/5);t2=t1+wxPoint(0,length/5);break;
		case 3:t=position-wxPoint(0,length);t1=t+wxPoint(length/5,length/5);t2=t1-wxPoint(length/5,0);break;
	};
	dc->DrawLine(position+getInnerPosition(),t+getInnerPosition());
	dc->DrawLine(t+getInnerPosition(),t1+getInnerPosition());
	dc->DrawLine(t+getInnerPosition(),t2+getInnerPosition());
};
															  

void GraphixScrollWindow::wasChanged()
{
	changed=1;
};

int GraphixScrollWindow::hasChanged()
{
		return(changed);
};

void GraphixScrollWindow::changeAccepted()
{
		changed=0;
};



void GraphixScrollWindow::DrawTitle(wxDC* dc)
{
	dc->SetBrush(*wxBLACK_BRUSH);
	dc->SetPen(*wxTRANSPARENT_PEN);
	for(int i=0;i<2;i++)
	{
		dc->DrawRoundedRectangle(OuterRahmen.GetPosition()+rect.GetPosition()+wxPoint(-2,OuterRahmen.GetHeight()/4),wxSize(8,OuterRahmen.GetHeight()/2),4);
		dc->DrawRoundedRectangle(OuterRahmen.GetPosition()+rect.GetPosition()+wxPoint(OuterRahmen.GetWidth()/4,OuterRahmen.GetHeight()-6),wxSize(OuterRahmen.GetWidth()/2,8),4);
		dc->DrawRoundedRectangle(OuterRahmen.GetPosition()+rect.GetPosition()+wxPoint(OuterRahmen.GetWidth()-6,OuterRahmen.GetHeight()/4),wxSize(8,OuterRahmen.GetHeight()/2),4);
		dc->DrawRoundedRectangle(OuterRahmen.GetPosition()+rect.GetPosition()+wxPoint(OuterRahmen.GetWidth()/4,-2),wxSize(OuterRahmen.GetWidth()/2,8),4);
//		dc->SetBrush(getBackground());
	}

	dc->SetFont(font2);
	dc->SetTextForeground(TitleColour);
	int dx,dy;
	dc->GetTextExtent(Title,&dx,&dy);
	titlePosition.y=-dy/2-3;
	titlePosition.SetSize(wxSize(dx+5,dy+2));
	dc->SetPen(RahmenPen);
	dc->DrawRoundedRectangle(titlePosition.GetPosition()+Rahmen.GetPosition()+rect.GetPosition(),titlePosition.GetSize(),3);
	dc->DrawText(Title,titlePosition.GetPosition()+Rahmen.GetPosition()+rect.GetPosition());
};

void GraphixScrollWindow::DrawTabs(wxDC* dc)
{
	for(int i=tabNumber;i--;)
	{
		dc->SetPen(RahmenPen);
		dc->SetBrush(*wxBLACK_BRUSH);
		wxRect edge=wxRect(tabPosition[i],0,120,30);
		dc->DrawRectangle(edge);

		if(i==currentTab)
			dc->SetBrush(getBackground());

		if(edge.Inside(controls.getCurrentPosition()))
			dc->SetPen(BrightRahmenPen);

		dc->DrawRectangle(wxRect(edge.GetPosition()+wxPoint(3,3),edge.GetSize()+wxSize(-6,-6)));
	

		dc->SetFont(font6);
		if(i==currentTab)
			dc->SetTextForeground(wxColour(200,200,255));
		else
			dc->SetTextForeground(TitleColour);
		int dx,dy;
		dc->GetTextExtent(tab[i],&dx,&dy);
		dc->DrawText(tab[i],edge.GetPosition()+wxPoint((edge.width-dx)>>1,(edge.height-dy)>>1));
		if(i==currentTab)
		{
			dc->SetBrush(*wxBLACK_BRUSH);
			dc->SetPen(*wxTRANSPARENT_PEN);
			dc->DrawRectangle(wxRect(edge.GetPosition()+wxPoint(1,edge.GetHeight()-4),wxSize(edge.GetWidth()-1,6)));
			dc->SetBrush(getBackground());
			dc->DrawRectangle(wxRect(edge.GetPosition()+wxPoint(1,edge.GetHeight()-4),wxSize(edge.GetWidth()-1,6)));
		}
	}
};

void GraphixScrollWindow::setWindow(wxRect rect)
{
	this->rect=rect;
	start=rect;
	target=rect;
};

void GraphixScrollWindow::setMaxWindow(wxRect max)
{
	maxSize=max;
};

void GraphixScrollWindow::Draw(wxDC* dc)
{
	if(!isShown())
		return;
	
//	int i;
//	if(WindowMove) ~~
//		adjustWindow(wxRect(controls.getX()-WindowMoveX,controls.getY()-WindowMoveY,Border.width,Border.height));

	move(rect.x,start.x,target.x);
	move(rect.y,start.y,target.y);
	move(rect.width,start.width,target.width);
	move(rect.height,start.height,target.height);
	move(innerRect.x,innerStart.x,innerTarget.x);
	move(innerRect.y,innerStart.y,innerTarget.y);
	move(innerRect.width,innerStart.width,innerTarget.width);
	move(innerRect.height,innerStart.height,innerTarget.height);

	updateWindow();
/*	if(WindowMove)
	{
		startx=x;starty=y;
	}*/

	setMaxScrollY(lastEntry);
	if((doAdjustments))//||(autoAdjust))
		adjustWindow(wxRect(wxPoint(target.GetPosition()),wxSize(target.GetWidth(),lastEntry+25)));
	doAdjustments=0;
	lastEntry=0;																			
	if(ScrollBalkenMove)
		moveScrollBalkenTo(controls.getCurrentPosition().y-ScrollBalkenMoveY-ScrollBalken.y);
	else moveScrollBalkenTo(0);

	if(tabNumber)
		DrawTabs(dc);
//erstmal Rahmen:
	dc->SetPen(RahmenPen);
	dc->SetBrush(getBackground());
	dc->DrawRoundedRectangle(OuterRahmen.GetPosition()+rect.GetPosition(),OuterRahmen.GetSize(),3);

	if(Rahmen.Inside(controls.getCurrentPosition(rect.GetPosition()))) // -> Maus ist innerhalb des Fensters
		dc->SetPen(BrightRahmenPen);

	dc->DrawRoundedRectangle(Rahmen.GetPosition()+rect.GetPosition(),Rahmen.GetSize(),3);


//	dc->DrawRectangle(wxRect(innerStart.GetPosition()+rect.GetPosition(),innerStart.GetSize()));
//	dc->DrawRectangle(wxRect(innerRect.GetPosition()+rect.GetPosition(),innerRect.GetSize()));
//	dc->DrawRectangle(wxRect(innerTarget.GetPosition()+rect.GetPosition(),innerTarget.GetSize()));

	if(!tabNumber)
		DrawTitle(dc);
	
	dc->SetBrush(getBackground());
/*	for(i=0;i<descriptionNumber;i++)
	{
		dc->GetTextExtent(Description[i],&dx,&dy);
		dx=dx+3;
		dc->DrawRoundedRectangle(Rahmen.x+x+descriptionPosition[i]-2,bla+Rahmen.y+y-dy/2-2,dx+2,dy+2,3);
		dc->DrawText(Description[i],Rahmen.x+x+descriptionPosition[i],bla+Rahmen.y+y-dy/2-1);
	}*/

	if(!scrolled) return;

//(RahmenPen.Red+RahmenPen.Green+RahmenPen.Blue)/3,(RahmenPen.Red+RahmenPen.Green+RahmenPen.Blue)/3,(RahmenPen.Red+RahmenPen.Green+RahmenPen.Blue)/3);

	dc->DrawRoundedRectangle(wxRect(ScrollArea.GetPosition()+rect.GetPosition(),ScrollArea.GetSize()),3);
//TODO button draus machen
	if(ScrollBalkenMove)
	{
		dc->SetBrush(clickedItemBrush);
		dc->SetPen(RahmenPen);
	} else if(ScrollBalken.Inside(controls.getCurrentPosition(rect.GetPosition())))
	{
		dc->SetBrush(RahmenBrush);
		dc->SetPen(clickedItemPen);
	} else
	{
		dc->SetBrush(getBackground());
		dc->SetPen(RahmenPen);
	}
	dc->DrawRoundedRectangle(wxRect(ScrollBalken.GetPosition()+rect.GetPosition(),ScrollBalken.GetSize()),4);

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

	if(scrollY>=maxScrollY-(innerRect.height))
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
	dc->DrawPolygon(3,points,PfeilDown.GetX()+rect.GetX(),PfeilDown.GetY()+rect.GetY());
};

bool GraphixScrollWindow::mouseInside()
{
	return(rect.Inside(controls.getCurrentPosition()));
};

bool GraphixScrollWindow::insideClientArea(wxPoint pos)//wxRect rectangle) //for all non-buttons/titles etc.
{
	return(innerRect.Inside(pos-rect.GetPosition()));
//	return(clientArea.Inside(rectangle.x,rectangle.y)||clientArea.Inside(rectangle.x+rectangle.width,rectangle.y+rectangle.height)||clientArea.Inside(rectangle.x+rectangle.width,rectangle.y)||clientArea.Inside(rectangle.x,rectangle.y+rectangle.height));
	//TODO: wenns auf beiden Seiten ueberlappt?
};

bool GraphixScrollWindow::fitToClientArea(wxRect& rectangle, int adjust)
{
	if(rectangle.x<getInnerLeftBound()) rectangle.x=getInnerLeftBound();
	if(rectangle.width+rectangle.x>getInnerLeftBound()+getInnerWidth()) 
		rectangle.width=getInnerLeftBound()+getInnerWidth()-rectangle.x;
//	if(rectangle.y<clientArea.y) rectangle.y=clientArea.y;
  //	  if(rectangle.y+rectangle.height>clientArea.y+clientArea.height) rectangle.height=clientArea.y+clientArea.height-rectangle.y;
	if(adjust)
	{
		doAdjustments=1;
		if(rectangle.y+rectangle.height-getInnerUpperBound()>lastEntry)
			lastEntry=rectangle.y+rectangle.height-getInnerUpperBound();
	}
	
	if((rectangle.width<=0)||(rectangle.y<getInnerUpperBound())||(rectangle.y+rectangle.height>getInnerUpperBound()+getInnerHeight())) return false;
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

GraphixScrollWindow::GraphixScrollWindow(int level, wxRect rahmen, wxRect maxSize, int scrolled, int autoAdjust, int tabbed, wxRect clientArea)
{
	this->autoAdjust=autoAdjust;
	for(int i=0;i<1000;i++)
	{
		button[i].used=0;
		button[i].pressed=0;
	}
	this->scrolled=scrolled;
	setTitleColour(wxColour(100,100,200));
	switch(level)
	{
		case 0:setBackground(wxBrush(wxColour(5,10,20),wxSOLID));break;
		case 1:setBackground(wxBrush(wxColour(5,15,30),wxTRANSPARENT));break; //!! TODO Warum loescht mir gtk meinen hintergrund auch noch?!
		case 2:setBackground(wxBrush(wxColour(5,25,40),wxSOLID));break; //infowindow
		default:break;
	}

	RahmenColour=wxColour(40,30,120);//,30);
	RahmenPen=wxPen(RahmenColour,1,wxSOLID);
	RahmenBrush=wxBrush(RahmenColour,wxSOLID);

	BrightRahmenColour=wxColour(RahmenColour.Red()+30,RahmenColour.Green()+60,RahmenColour.Blue()+30);
	BrightRahmenPen=wxPen(BrightRahmenColour,1,wxSOLID);

	clickedItemColour=wxColour(200,200,200);
	clickedItemBrush=wxBrush(clickedItemColour,wxSOLID);
	clickedItemPen=wxPen(clickedItemColour,1,wxSOLID);
	
	disabledItemColour=wxColour(70,70,70);
	disabledItemPen=wxPen(disabledItemColour,1,wxSOLID);
	

	changed=0;
	tabNumber=0;
	currentTab=0;

	if(tabbed)
	{
		rahmen.y+=21;
		rahmen.height-=21;
	}
	originalRect=rahmen;
	
	setRahmen(rahmen);
	setWindow(rect);
	setMaxWindow(maxSize);

	ScrollArea=wxRect(Rahmen.GetPosition()+wxPoint(Rahmen.GetWidth()-18,2),wxSize(16,Rahmen.GetHeight()-4));
	ScrollBalken=wxRect(ScrollArea.GetPosition()+wxPoint(1,17),ScrollArea.GetSize()+wxSize(-2,-34));
	PfeilUp=wxRect(ScrollArea.GetPosition(),ScrollArea.GetSize()); //???
	PfeilDown=wxRect(ScrollArea.GetPosition()+wxPoint(0,ScrollArea.height-ScrollArea.width),wxSize(ScrollArea.width,ScrollArea.width));
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

//	if(tabbed)
//		if(clientArea.y<Rahmen.y+30) {clientArea.height-=Rahmen.y+30-clientArea.y;clientArea.y=Rahmen.y+30;};

//	if(description)
//		if(clientArea.y<Rahmen.y+16) { clientArea.height-=Rahmen.y+16-clientArea.y;clientArea.y=Rahmen.y+16;}
	
	innerRect=clientArea;
	innerTarget=clientArea;
	innerStart=clientArea;
	originalInnerRect=clientArea; //brauchts eh nur fuer hoehe eigentlich

	ScrollBalkenMove=0;
	PfeilUpPressed=0;PfeilDownPressed=0;
	setMaxScrollY(1500);
//	maxcheight=maxheight-Rahmen.height+clientArea.height-2*clientArea.y;
//	descriptionNumber=0;
	setFreeMove(0);
	Show(1);
	clearButtons();
	lastEntry=clientArea.height;
	doAdjustments=1;
};


void GraphixScrollWindow::OnScrollMouse(int msy)
{
	if(!isShown()) return;
	if(insideClientArea(controls.getCurrentPosition()))
	{
		if(msy<0)
			moveScrollBalkenTo(30);
		else if(msy>0)
			moveScrollBalkenTo(-30);
	}
}


// 

void GraphixScrollWindow::checkButtons()
{
	if(!isShown())
		return;

/*	PfeilUpPressed=0;
	PfeilDownPressed0;
	ScrollBalkenMove=0;

	if(ScrollBalken.Inside(controls.getX()-x,controls.getY()-y))
	{
		ScrollBalkenMove=1;
		ScrollBalkenMoveY=controls.getY()-ScrollBalken.y;
	}*/

	PfeilUpPressed=controls.getPressCondition(wxRect(PfeilUp.GetPosition()+rect.GetPosition(),PfeilUp.GetSize()));
	PfeilDownPressed=controls.getPressCondition(wxRect(PfeilDown.GetPosition()+rect.GetPosition(),PfeilUp.GetSize()));

/*	else
	if(titlePosition.Inside(controls.getX()-x,controls.getY()-y))
	{
		WindowMove=1;
//		startx=x;starty=y;startheight=Border.height;startwidth=Border.width;
		WindowMoveX=controls.getX()-x;
		WindowMoveY=controls.getY()-y;
	}*/
//	else
	{
		for(int i=0;i<1000;i++)
			if(button[i].used)
			{
				button[i].pressed=controls.getPressCondition(button[i].edge);
				if(button[i].pressed)
				{
					int x=button[i].edge.x;
					break;
				}
			}
	}
};

/*void GraphixScrollWindow::OnMouseLeftUp()
{
	if(!isShown()) return;
	if(ScrollBalkenMove)
	{
		moveScrollBalkenTo(0); //0?
		setScrollY(ScrollBalken.y-ScrollArea.y-17);
	}
	else
	if(PfeilUp.Inside(controls.getX()-x,controls.getY()-y))
	{
		if(PfeilUpPressed)
		{
			moveScrollBalkenTo(-10);
			setScrollY(ScrollBalken.y-ScrollArea.y-17);
		}
	}
	else
	if(PfeilDown.Inside(controls.getX()-x,controls.getY()-y))
	{
		if(PfeilDownPressed)
		{
			moveScrollBalkenTo(10);
			setScrollY(ScrollBalken.y-ScrollArea.y-17);
		}
	}
	else if((!ScrollBalken.Inside(controls.getX()-x,controls.getY()-y))&&ScrollArea.Inside(controls.getX()-x,controls.getY()-y))
	{
		if(controls.getY()<ScrollBalken.y)
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
}*/

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
	originalInnerRect.width+=width;
}

void GraphixScrollWindow::transformClientHeight(int height) //handle with care O_O
{
	originalInnerRect.height+=height;
}
/*
void GraphixScrollWindow::adjustClientWindow(int width, int height)
{
//	if(edge.x!=targetcx)
  //		startcx=clientArea.x;
//	if(edge.y!=targetcy)
  //		startcy=clientArea.y;
	   if(height!=targetcheight)
//||((targetcheight>maxcheight)&&(edge.height!=maxcheight))) already checked in adjustWindow
		  startcheight=clientArea.height;
	if(width!=targetcwidth)
		startcwidth=clientArea.width;
//	  targetcx=edge.x;
  //	 targetcy=edge.y;
	targetcwidth=width;
//	if(edge.height<=maxcheight)
		  targetcheight=height;
  //  else targetcheight=maxcheight;
}*/



void GraphixScrollWindow::adjustWindow(wxRect edge)
{
	if(edge.width==-99)
		edge.width=target.width;
	if(edge.height==-99)
		edge.height=target.height;
//Only 1 change (X OR WIDTH, Y OR HEIGHT) is allowed!
	if(edge.width==target.width) // nur x ist unterschiedlich
	{
		if(((edge.x!=target.x)&&(edge.x+edge.width<maxSize.x+maxSize.width)) // es ist ein neues Ziel
		||((edge.x+edge.width>maxSize.x+maxSize.width)&&(target.x+target.width!=maxSize.x+maxSize.width)) // oder einfach ausserhalb der Begrenzung aber unser Ziel war nicht die Begrenzung
		||((edge.x<maxSize.x)&&(target.x!=maxSize.x))
		||((edge.x!=target.x)&&(freeMove)))
			start.x=rect.x; //Dann setze neuen Startpunkt
		if((edge.x+edge.width>maxSize.x+maxSize.width)&&(!freeMove))
			edge.x=maxSize.x+maxSize.width-edge.width;
	} else if(edge.x==target.x)
	{
		if(((edge.width!=target.width)&&(edge.x+edge.width<maxSize.x+maxSize.width))
		||((edge.x+edge.width>maxSize.x+maxSize.width)&&(target.x+target.width!=maxSize.x+maxSize.width))
		||((edge.width!=target.width)&&(freeMove)))
			start.width=rect.width;
		if((edge.x+edge.width>maxSize.x+maxSize.width)&&(!freeMove))
			edge.width=maxSize.x+maxSize.width-edge.x;
	} //else ?

	if(edge.height==target.height)
	{
		if(((edge.y!=target.y)&&(edge.y+edge.height<maxSize.y+maxSize.height))
		||((edge.y+edge.height>maxSize.y+maxSize.width)&&(target.y+target.height!=maxSize.y+maxSize.height))
		||((edge.y<maxSize.y)&&(target.y!=maxSize.y))
		||((edge.y!=target.y)&&(freeMove)))
			start.y=rect.y;
		if((edge.y+edge.height>maxSize.y+maxSize.height)&&(!freeMove))
			edge.y=maxSize.y+maxSize.height-edge.height;
	} else if(edge.y==target.y)
	{
		if(((edge.height!=target.height)&&(edge.y+edge.height<maxSize.y+maxSize.height))
		||((edge.y+edge.height>maxSize.y+maxSize.height)&&(target.y+target.height!=maxSize.y+maxSize.height))
		||((edge.height!=target.height)&&(freeMove)))
			start.height=rect.height;
		if((edge.y+edge.height>maxSize.y+maxSize.height)&&(!freeMove))
			edge.height=maxSize.y+maxSize.height-edge.y;
	} //else ... mmmh... 

	if((edge.x<maxSize.x)||(freeMove))
		target.x=maxSize.x;
	else target.x=edge.x;
	if((edge.y<maxSize.y)||(freeMove))
		target.y=maxSize.y;
	else target.y=edge.y;
	
	target.width=edge.width;
	target.height=edge.height;

//CLIENT WINDOW
	int height=target.height-originalRect.height+originalInnerRect.height;
	int width=target.width-originalRect.width+originalInnerRect.width;
	if(height!=innerTarget.height)
		innerStart.height=innerRect.height;
	if(width!=innerTarget.width)
		innerStart.width=innerRect.width;
	innerTarget.width=width;
	innerTarget.height=height;
	freeMove=0;
}

void GraphixScrollWindow::updateWindow()
{
	wxRect oldRahmen=Rahmen;
	setRahmen(rect);
	ScrollArea=wxRect(Rahmen.x+Rahmen.width-18,Rahmen.y+2,16,Rahmen.height-4); //relative Koordinate!
//	  ScrollBalken=wxRect(ScrollArea.x+1,ScrollArea.y+17,ScrollArea.width-2,ScrollArea.height-34);
	PfeilUp=wxRect(ScrollArea.x,ScrollArea.y,ScrollArea.width,ScrollArea.width); //relative Koordinate!
	PfeilDown=wxRect(ScrollArea.x,ScrollArea.y+ScrollArea.height-ScrollArea.width,ScrollArea.width,ScrollArea.width); //relative Koordinate!
//	clientArea.x=Rahmen.x+5;
  //	  if(scrolled)
	//		clientArea.width=ScrollArea.x-3-clientArea.x;
//	else
  //		  clientArea.width=Rahmen.x+Rahmen.width-3-clientArea.x;

/*	if(tabNumber)
	{
		
	}*/ //~~

	/*	if(descriptionNumber)
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
	shown=show;
};


wxFont GraphixScrollWindow::font;
wxFont GraphixScrollWindow::font2;
wxFont GraphixScrollWindow::font3;
wxFont GraphixScrollWindow::font4;
wxFont GraphixScrollWindow::font5;
wxFont GraphixScrollWindow::font6;
wxFont GraphixScrollWindow::font7;
Controls GraphixScrollWindow::controls;
wxBitmap GraphixScrollWindow::bmpCancel;
wxBitmap GraphixScrollWindow::bmpAdd;
wxBitmap GraphixScrollWindow::bmpSub;
wxBitmap GraphixScrollWindow::bmpArrowLeft;
wxBitmap GraphixScrollWindow::bmpArrowUp;
wxBitmap GraphixScrollWindow::bmpArrowRight;
wxBitmap GraphixScrollWindow::bmpArrowDown;
wxBitmap GraphixScrollWindow::bmpClemens;
wxBitmap GraphixScrollWindow::bmpClawsoftware;
wxBitmap GraphixScrollWindow::bmpBack;
SETTINGS GraphixScrollWindow::settings;

