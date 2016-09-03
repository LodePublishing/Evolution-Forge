#include "window.hpp"
#include "configuration.hpp"
const unsigned int  MIN_HEIGHT = 2;

UI_Window& UI_Window::operator=(const UI_Window& object)
{
	((UI_Object)(*this)) = ((UI_Object)object);
	delete tabRow;
	tabRow = (object.tabRow != NULL? new UI_Radio(*object.tabRow) : NULL);
	isTransparent = object.isTransparent;
	currentTab = object.currentTab;
	titleString = object.titleString;
	titleParameter = object.titleParameter;
	originalRect = object.originalRect;
	border = object.border;
	outerBorder = object.outerBorder;
	clientRect = object.clientRect;
	clientStartRect = object.clientStartRect;
	clientTargetRect = object.clientTargetRect;
	originalClientRect = object.originalClientRect;
	isAutoAdjust = object.isAutoAdjust;
	isScrollable = object.isScrollable;
	isTabbed = object.isTabbed;
	highlighted = object.highlighted;
	delete scrollBar;
	scrollBar = (object.scrollBar != NULL? new UI_Scrollbar(*object.scrollBar) : NULL);
	return(*this);
}

UI_Window::UI_Window(const UI_Window& object) :
	UI_Object((UI_Object)object),
	tabRow((object.tabRow != NULL? new UI_Radio(*object.tabRow) : NULL)),
	isTransparent( object.isTransparent),
	currentTab( object.currentTab ),
	titleString( object.titleString ),
	titleParameter( object.titleParameter ),
	originalRect( object.originalRect ),
	border( object.border ),
	outerBorder( object.outerBorder ),
	clientRect( object.clientRect ),
	clientStartRect( object.clientStartRect ),
	clientTargetRect( object.clientTargetRect ),
	originalClientRect( object.originalClientRect ),
	isAutoAdjust( object.isAutoAdjust ),
	isScrollable( object.isScrollable ),
	isTabbed( object.isTabbed ),
	highlighted( object.highlighted ),
	scrollBar((object.scrollBar != NULL? new UI_Scrollbar(*object.scrollBar) : NULL))
{ }

UI_Window::UI_Window(UI_Object* window_parent, const eString window_title_string, const Rect rect, const unsigned int max_height, const eIsScrolled window_is_scrollable, const eIsAutoAdjust window_is_auto_adjust, const eIsTabbed window_is_tabbed, const Rect window_client_area, eIsTransparent transparent) :
	UI_Object(window_parent, isScrollable==SCROLLED ? Rect(rect.GetTopLeft(), rect.GetSize() - Size(0, 0)): rect),
	tabRow(window_is_tabbed==TABBED?new UI_Radio(this) : NULL),
	isTransparent(transparent),
	currentTab(0),
	titleString(window_title_string), // ??
	titleParameter(""),
	originalRect(getRelativeRect()),
	border(), // TODO
	outerBorder(), // TODO
	clientRect(window_client_area),
	clientStartRect(),
	clientTargetRect(),
	originalClientRect(), // TODO 
	maxHeight(max_height),
	isAutoAdjust(window_is_auto_adjust), //?
	isScrollable(window_is_scrollable), // TODO
	isTabbed(window_is_tabbed), //?
	highlighted(false),
	scrollBar(isScrollable==SCROLLED?new UI_Scrollbar(this, /*getClientRect(), TODO */ getRelativeClientRectUpperBound(), getMaxHeight()) : NULL)
{

// ------ PROCESSING
	calculateClientRect();


//	filledHeight=clientRect.GetHeight();
// ------ Buttons, ScrollBars etc.

	if(isTabbed==TABBED)
		tabRow->calculateSameWidthOfButtons();
}

UI_Window::~UI_Window()
{
	delete scrollBar;
	delete tabRow;
}

void UI_Window::calculateClientRect()
{
	updateBorders();
	if(clientRect.GetLeft() < border.GetLeft()+5) clientRect.SetLeft(border.GetLeft()+5);
   	if(clientRect.GetTop() < border.GetTop()+5) clientRect.SetTop(border.GetTop()+5);
	if(clientRect.GetBottom() > border.GetBottom()-5) clientRect.SetBottom(border.GetBottom()-5);
//	{
		
//		if(clientRect.GetRight() > border.GetRight() - 19) 
//			clientRect.SetRight(border.GetRight()-19);
//		if(clientRect.GetRight() > ScrollRect.GetLeft()-3) clientRect.SetRight(ScrollRect.GetLeft()-3); TODO
//	} else
//	{
		if(clientRect.GetRight() > border.GetRight() - 3) 
			clientRect.SetRight(border.GetRight()-3);
//	}

	originalClientRect=clientRect;
	clientStartRect=clientRect;
	clientTargetRect=clientRect;
}

void UI_Window::updateRectangles(const Rect rect, const unsigned int max_height)
{
	maxHeight = max_height;
	adjustRelativeRect(isScrollable==SCROLLED?Rect(rect.GetTopLeft(), rect.GetSize() - Size(0, 0)):rect);
	adjustClientRect();
//	originalRect=getRelativeRect();
// TODO Buttons benachrichtigen ueber geaenderte buttonPlacementArea!
	setNeedRedrawMoved();
}


void UI_Window::addTab(UI_Button* tab_button, const unsigned int button_id)
{
#ifdef _SCC_DEBUG
	if(isTabbed==NOT_TABBED) {
                toLog("DEBUG: (UI_Window::addTab): Tried to add a tab while window is marked as not tabbed.");return;
        }
#endif
	tabRow->addButton(tab_button, button_id);
}

void UI_Window::removeTab(const unsigned int button_id)
{
#ifdef _SCC_DEBUG
	if(isTabbed==NOT_TABBED) {
                toLog("DEBUG: (UI_Window::removeTab): Tried to remove a tab while window is marked as not tabbed.");return;
        }
#endif
	tabRow->removeButton(button_id);
	tabRow->updateIds(); // fill the space
}

void UI_Window::forcePressTab(const unsigned int press_tab)
{
	if(isTabbed==NOT_TABBED) 
		return;
	tabRow->forcePress(press_tab);
	currentTab = press_tab;
}

const bool UI_Window::tabWasChanged() const
{
	if(isTabbed==NOT_TABBED) 
		return false;
	return(tabRow->buttonHasChanged());
}

const unsigned int UI_Window::getCurrentTab() const
{
	if(isTabbed==NOT_TABBED) 
		return 0;
	unsigned int tmp = tabRow->getMarked();
//	if(tmp == 99)
//		return 0;
//	else return(tmp+1);
	return(tmp);
}

void UI_Window::setTitleParameter(const std::string& p) {
	titleParameter=p;
}

UI_Object* UI_Window::checkToolTip()
{
	if(!(isMouseInside()))
		return(NULL);
	return(UI_Object::checkToolTip());
}

UI_Object* UI_Window::checkHighlight()
{
	if((!isMouseInside())&&( (scrollBar==NULL) || (!Rect(getAbsolutePosition() + Point(getWidth(), 0), Size(12, getHeight())).Inside(mouse))))
		return(NULL);
	return(UI_Object::checkHighlight());
}

const unsigned int UI_Window::getClientTargetWidth() const
{
	return(clientTargetRect.GetWidth());
}

const unsigned int UI_Window::getClientTargetHeight() const
{
	return(clientTargetRect.GetHeight());
}

// ----------------------
// ------ MOVEMENT ------
// ----------------------


void UI_Window::adjustClientRect()
{
//CLIENT WINDOW
	unsigned int height = getTargetHeight() - originalRect.GetHeight() + originalClientRect.GetHeight();
	unsigned int width = getTargetWidth()-originalRect.GetWidth()+originalClientRect.GetWidth();
	if(height!=clientTargetRect.GetHeight())
		clientStartRect.SetHeight(clientRect.GetHeight());
	if(width!=clientTargetRect.GetWidth())
		clientStartRect.SetWidth(clientRect.GetWidth());
	clientTargetRect.SetWidth(width);
	clientTargetRect.SetHeight(height);
}

void UI_Window::updateBorders()
{
	border = Rect(Point(3,8), getSize()-Size(6,11));
	outerBorder = Rect(Point(1,6),getSize()-Size(2,7));
}

//void UI_Window::updateWindow()
//{
//	updateBorders();
  //  ScrollArea=Rect(Rahmen.x+Rahmen.width-18,Rahmen.y+2,16,Rahmen.height-4); //relative Koordinate! TODO
//	ScrollBalken=Rect(ScrollArea.x+1,ScrollArea.y+17,ScrollArea.width-2,ScrollArea.height-34);
	//PfeilUp=Rect(ScrollArea.x,ScrollArea.y,ScrollArea.width,ScrollArea.width); //relative Koordinate!
//	PfeilDown=Rect(ScrollArea.x,ScrollArea.y+ScrollArea.height-ScrollArea.width,ScrollArea.width,ScrollArea.width); //relative Koordinate!
//  clientArea.x=Rahmen.x+5;
  //	  if(scrolled)
	//	  clientArea.width=ScrollArea.x-3-clientArea.x;
//  else
  //		  clientArea.width=Rahmen.x+Rahmen.width-3-clientArea.x;
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


void UI_Window::wheelUp()
{
	if(scrollBar)
	{
		scrollBar->moveUp();
		setNeedRedrawNotMoved();
	}
}

void UI_Window::wheelDown()
{
	if(scrollBar)
	{
		scrollBar->moveDown();
		setNeedRedrawNotMoved();
	}
}

void UI_Window::moveScrollbarToTop()
{
	if(scrollBar)
		scrollBar->moveToTop();
}

void UI_Window::moveScrollbarToBottom()
{
	if(scrollBar)
		scrollBar->moveToBottom();
}

void UI_Window::process()
{
	if(!isShown()) 
		return;
//	calculateClientRect();
	
	/*Rect r = getAbsoluteRect();
	rectlist[rectnumber].x = r.x;rectlist[rectnumber].y = r.y;rectlist[rectnumber].w = r.width; rectlist[rectnumber].h = r.height;*/

	if(scrollBar)
	{
		scrollBar->setClientHeight(getClientRectHeight());
		scrollBar->setClientTargetHeight(getClientTargetHeight());
	}

	UI_Object::process();
	
/*	r = getAbsoluteRect();
	if( r.x < rectlist[rectnumber].x)	
	{
		rectlist[rectnumber].w += rectlist[rectnumber].x - r.x;
		rectlist[rectnumber].x = r.x;
	} else
	if( r.x > rectlist[rectnumber].x)	
		rectlist[rectnumber].w += r.x - rectlist[rectnumber].x;
	if( r.y < rectlist[rectnumber].y)	
	{
		rectlist[rectnumber].h += rectlist[rectnumber].y - r.y;
		rectlist[rectnumber].y = r.y;
	} else
	if( r.y > rectlist[rectnumber].y)	
		rectlist[rectnumber].h += r.y - rectlist[rectnumber].y;

	if( r.width > rectlist[rectnumber].w )
		rectlist[rectnumber].w = r.width;

	if( r.height > rectlist[rectnumber].h )
		rectlist[rectnumber].h = r.height;
	rectnumber++;*/
		
//  int i;
//  if(WindowMove) ~~
//	  adjustClientRect(Rect(controls.getX()-WindowMoveX,controls.getY()-WindowMoveY,Border.width,Border.height));
/*	if((!isMouseInside()) && (scrollBar!=NULL))
		scrollBar->Hide();
	else if((isMouseInside()) && (scrollBar!=NULL))
		scrollBar->Show();*/
	
	if(clientRect.GetHeight() != clientTargetRect.GetHeight())
	{
		if(uiConfiguration.isSmoothMovements())
		{
			if(clientRect.moveSmooth(clientStartRect, clientTargetRect))
				setNeedRedrawMoved();
		}
		else 
		{
			if(clientRect.move(clientStartRect, clientTargetRect))
				setNeedRedrawMoved();
		}	
	}

	if((!UI_Object::windowSelected)&&(((isMouseInside())||( (scrollBar!=NULL) && (Rect(getAbsolutePosition() + Point(getWidth(), 0), Size(12, getHeight())).Inside(mouse))))&&(!isTopItem()))) // => main window!
	{
		if(UI_Object::currentWindow != this)
		{
			setNeedRedrawNotMoved();
			if(UI_Object::currentWindow)
				UI_Object::currentWindow->setNeedRedrawNotMoved();
		}
		UI_Object::currentWindow = this;
		UI_Object::windowSelected = true;
	}

	updateBorders();
	
	/* if(WindowMove)
	{
		startx=x;starty=y;
	}*/

	if(getDoAdjustments() == 2)//||(autoAdjust))
	{
		adjustClientRect();
		setDoAdjustments(0);
	}

	filledHeight=0;
//	if((ScrollBalkenPressed==2)||(ScrollBalkenPressed==3)||(ScrollBalkenPressed==4))
  //  {
//		moveScrollBalkenTo(controls.getCurrentPosition().y-controls.getDragStartPosition().y-ScrollBalken.y);
//	  controls.updateDragStartPosition(controls.getCurrentPosition());
//	}
  //  else moveScrollBalkenTo(0);

}



const bool UI_Window::fitItemToRelativeClientRect(const Rect& rect, const unsigned int adjust)
{
	if(firstItemY == 0)
		firstItemY = rect.GetTop();
	if(lastItemY < rect.GetBottom())
		lastItemY = rect.GetBottom();

	if((rect.GetLeft() >= getRelativeClientRectRightBound())||
//	   (rect.GetTop()>=getRelativeClientRectLowerBound())||
	   (rect.GetRight() < getRelativeClientRectLeftBound())||
	   (rect.GetBottom() < getRelativeClientRectUpperBound()))
		return(false);
/*		
	if(rect.GetLeft()<getRelativeClientRectLeftBound()) 
		rect.SetLeft(getRelativeClientRectLeftBound());
	if((rect.GetRight()>getRelativeClientRectRightBound()))
		rect.SetRight(getRelativeClientRectRightBound());*/
//  if(rect.y<clientArea.y) rect.y=clientArea.y;
//	if(rect.y+rect.height>clientArea.y+clientArea.height) rect.height=clientArea.y+clientArea.height-rect.y;
	if(adjust)
	{
		setDoAdjustments(1);
		if((rect.GetBottom()>0) &&((unsigned int)(rect.GetBottom())/*-getRelativeClientRectUpperBound()*/>filledHeight))
		filledHeight = rect.GetBottom()/*-getRelativeClientRectUpperBound()*/;
	
		if(filledHeight>getMaxHeight()) 
			filledHeight=getMaxHeight();
	}
	if((rect.GetTop()<getRelativeClientRectUpperBound())||
	  (rect.GetBottom()>getRelativeClientRectLowerBound())) 
		return false;
	else return true;
}

const bool UI_Window::fitItemToAbsoluteClientRect(const Rect& rect, const unsigned int adjust)
{
	if(firstItemY == 0)
		firstItemY = rect.GetTop() - getAbsoluteClientRectUpperBound();
	if(lastItemY < rect.GetBottom() - getAbsoluteClientRectUpperBound())
		lastItemY = rect.GetBottom() - getAbsoluteClientRectUpperBound();
	if((rect.GetLeft() >= getAbsoluteClientRectRightBound())||
	   (rect.GetTop() >= getAbsoluteClientRectLowerBound())||
	   (rect.GetRight() < getAbsoluteClientRectLeftBound())||
	   (rect.GetBottom() < getAbsoluteClientRectUpperBound()))
		return(false);
/*	if(rect.GetLeft() < getAbsoluteClientRectLeftBound()) 
		rect.SetLeft(getAbsoluteClientRectLeftBound());
		
	if(rect.GetRight() > getAbsoluteClientRectRightBound())
		rect.SetRight(getAbsoluteClientRectRightBound());*/
//  if(rect.y<clientArea.y) rect.y=clientArea.y;
  //	  if(rect.y+rect.height>clientArea.y+clientArea.height) rect.height=clientArea.y+clientArea.height-rect.y;
	if(adjust)
	{
		setDoAdjustments(1);
		if((rect.GetBottom() > getAbsoluteClientRectUpperBound())&&((unsigned int)(rect.GetBottom() - getAbsoluteClientRectUpperBound()) > filledHeight))
		filledHeight = (unsigned int) (rect.GetBottom()-getAbsoluteClientRectUpperBound());
		if(filledHeight > getMaxHeight()) 
			filledHeight = getMaxHeight();
	}
	if((rect.GetTop() < getAbsoluteClientRectUpperBound())||
	 (rect.GetBottom() > getAbsoluteClientRectLowerBound())) 
		return false;
	else return true;
}


void UI_Window::drawTitle(DC* dc) const
{
	dc->SetBrush(*theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH));
	dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	dc->SetFont(theme.lookUpFont(SMALL_ITALICS_BOLD_FONT));
	dc->SetTextForeground(*theme.lookUpColor(TITLE_COLOR));

	std::string text;
	
	if(titleParameter.size())
		text=theme.lookUpFormattedString(titleString, titleParameter);
	else text=*theme.lookUpString(titleString);
	
	Size s = dc->GetTextExtent(text);
	Rect titleRect = Rect(getAbsolutePosition() - Size(0,2), s - Size(0, 0) + Size(5,0));///*+Point(0,-s.GetHeight()/2-2)
	
	dc->DrawRectangle(titleRect);
	titleRect.SetTopLeft(titleRect.GetTopLeft()+Point(2,3));
	dc->DrawText(text, titleRect.GetTopLeft());
}

void UI_Window::draw(DC* dc) const
{
//	if(!isShown()) 
//		return;

	if(checkForNeedRedraw())
	{

//	if(doesNeedRedraw())
//	{
//		... Draaw at new position:
		
	// draw outer border:
		dc->SetPen(*theme.lookUpPen(OUTER_BORDER_PEN));
		if(isTransparent==TRANSPARENT) // => main window!
		{
			dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
/*			SDL_Rect rc;
			rc.x = getRelativeLeftBound();rc.y = getRelativeUpperBound(); rc.w = getWidth(); rc.h = getHeight();
			if(uiConfiguration.isBackgroundBitmap())
				SDL_BlitSurface(*UI_Object::theme.lookUpBitmap(BACKGROUND_BITMAP) , 0, dc->GetSurface(), &rc);
			else
				SDL_FillRect(dc->GetSurface(), &rc, 0);*/
		}
		else
//		{
			dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
//		if(notDrawRectList.empty())
			dc->DrawEdgedRoundedRectangle(outerBorder.GetTopLeft()+getAbsolutePosition(),outerBorder.GetSize(), 6);
//		}
//		else 
//			dc->DrawGridEdgedRoundedRectangle(outerBorder.GetLeft()+getAbsolutePosition().x, outerBorder.GetTop() + getAbsolutePosition().y, outerBorder.GetWidth(), outerBorder.GetHeight(), 6, notDrawRectList);

	// draw inner border:
		if(UI_Object::currentWindow == this)
			dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
		else
			dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
		dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
		dc->DrawEdgedRoundedRectangle(border.GetTopLeft()+getAbsolutePosition(),border.GetSize(),6);

		
	// draw title if there are no tabs: 
		if(isTabbed==NOT_TABBED)
			drawTitle(dc);

	// draw descriptions - obsolete

	//   dc->SetBrush(getBackground());
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
//	dc->DrawEmptyRectangle(getAbsoluteClientRect());
	}
		UI_Object::draw(dc);
//	}
//	}

//	dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
//	dc->DrawEmptyRectangle(Rect(getAbsolutePosition()+clientTargetRect.GetTopLeft(), clientTargetRect.GetSize()));
//	dc->SetPen(*theme.lookUpPen(OUTER_BORDER_PEN));
//	dc->DrawEmptyRectangle(getAbsoluteClientRect());

}



bool UI_Window::changedFlag=false;
bool UI_Window::resetFlag=false;
