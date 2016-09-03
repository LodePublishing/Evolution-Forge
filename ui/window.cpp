#include "window.hpp"
#include "configuration.hpp"

UI_Window& UI_Window::operator=(const UI_Window& object)
{
	((UI_Object)(*this)) = ((UI_Object)object);
	delete tabRow;
	tabRow = (object.tabRow != NULL? new UI_Radio(*object.tabRow) : NULL);

	filledHeight = object.filledHeight;

	doAdjustments = object.doAdjustments;
	isTransparent = object.isTransparent;
	titleString = object.titleString;
	titleParameter = object.titleParameter;
	originalRect = object.originalRect;
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
	filledHeight(object.filledHeight),
	doAdjustments( object.doAdjustments ),
	isTransparent( object.isTransparent ),
	titleString( object.titleString ),
	titleParameter( object.titleParameter ),
	originalRect( object.originalRect ),
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
	tabRow(window_is_tabbed==TABBED?new UI_Radio(this, rect) : NULL),
	filledHeight(0),
	doAdjustments(false),
	isTransparent(transparent),
	titleString(window_title_string), // ??
	titleParameter(""),
	originalRect(getRelativeRect()),
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
	if(clientRect.GetLeft() < 3) clientRect.SetLeft(3);
   	if(clientRect.GetTop() < 3) clientRect.SetTop(3);
	if(clientRect.GetBottom() > getHeight() - 5) clientRect.SetBottom(getHeight() - 5);
//	{
		
//		if(clientRect.GetRight() > border.GetRight() - 19) 
//			clientRect.SetRight(border.GetRight()-19);
//		if(clientRect.GetRight() > ScrollRect.GetLeft()-3) clientRect.SetRight(ScrollRect.GetLeft()-3); TODO
//	} else
//	{
		if(clientRect.GetRight() > getWidth() - 5) 
			clientRect.SetRight(getWidth() - 5);
//	}

	originalClientRect = clientRect;
	clientStartRect = clientRect;
	clientTargetRect = clientRect;
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

// ----------------------
// ------ MOVEMENT ------
// ----------------------


void UI_Window::adjustClientRect()
{
//CLIENT WINDOW
	unsigned int height = getTargetHeight() - originalRect.GetHeight() + originalClientRect.GetHeight();
	unsigned int width = getTargetWidth() - originalRect.GetWidth() + originalClientRect.GetWidth();
	if(height!=clientTargetRect.GetHeight())
		clientStartRect.SetHeight(clientRect.GetHeight());
	if(width!=clientTargetRect.GetWidth())
		clientStartRect.SetWidth(clientRect.GetWidth());
	clientTargetRect.SetWidth(width);
	clientTargetRect.SetHeight(height);
}

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

	if(doAdjustments==true)
	{
		adjustRelativeRect(Rect(Point(getTargetRect().GetTopLeft()), Size(getTargetRect().GetWidth(), filledHeight+25))); // TODO!
		adjustClientRect();
		doAdjustments=false;
	}

//	calculateClientRect();
	
	if(scrollBar)
	{
		scrollBar->setClientHeight(getClientRectHeight());
		scrollBar->setClientTargetHeight(getClientTargetHeight());
	}

	UI_Object::process();
	
/*	if((!isMouseInside()) && (scrollBar!=NULL))
		scrollBar->Hide();
	else if((isMouseInside()) && (scrollBar!=NULL))
		scrollBar->Show();*/
	
	if(clientRect != clientTargetRect)
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


//	filledHeight=0;
//	if((ScrollBalkenPressed==2)||(ScrollBalkenPressed==3)||(ScrollBalkenPressed==4))
  //  {
//		moveScrollBalkenTo(controls.getCurrentPosition().y-controls.getDragStartPosition().y-ScrollBalken.y);
//	  controls.updateDragStartPosition(controls.getCurrentPosition());
//	}
  //  else moveScrollBalkenTo(0);

}



const bool UI_Window::fitItemToRelativeClientRect(const Rect& rect, const unsigned int adjust)
{
	if(scrollBar)
	{
		if(scrollBar->firstItemY == 0)
			scrollBar->firstItemY = rect.GetTop();
		if(scrollBar->lastItemY < rect.GetBottom())
			scrollBar->lastItemY = rect.GetBottom();
	}

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
		doAdjustments = true;
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
	if(scrollBar)
	{
		if(scrollBar->firstItemY == 0)
			scrollBar->firstItemY = rect.GetTop() - getAbsoluteClientRectUpperBound();
		if(scrollBar->lastItemY < rect.GetBottom() - getAbsoluteClientRectUpperBound())
			scrollBar->lastItemY = rect.GetBottom() - getAbsoluteClientRectUpperBound();
	}
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
		doAdjustments = true;
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
	if(titleString==NULL_STRING)
		return;
	dc->SetBrush(*theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH));
	dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	dc->SetFont(theme.lookUpFont(SMALL_ITALICS_BOLD_FONT));
	dc->SetTextForeground(*theme.lookUpColor(TITLE_COLOR));

	std::string text;
	
	if(titleParameter.size())
		text=theme.lookUpFormattedString(titleString, titleParameter);
	else text=*theme.lookUpString(titleString);
	
	Size s = dc->GetTextExtent(text);
	Rect titleRect = Rect(getAbsolutePosition() - Size(0,2), s - Size(0, 0) + Size(5,0));
	
	dc->DrawRectangle(titleRect);
	titleRect.SetTopLeft(titleRect.GetTopLeft()+Point(2,3));
	dc->DrawText(text, titleRect.GetTopLeft());
}

void UI_Window::draw(DC* dc) const
{
	if(checkForNeedRedraw())
	{
	// draw outer border:
		dc->SetPen(*theme.lookUpPen(OUTER_BORDER_PEN));
		if(isTransparent==TRANSPARENT) // => main window!
			dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
		else
			dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
		dc->DrawEdgedRoundedRectangle(Point(1, 6) + getAbsolutePosition(), getSize() - Size(2, 7), 6);
		
	// draw inner border:
		if(UI_Object::currentWindow == this)
			dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
		else
			dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
		dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
		
		dc->DrawEdgedRoundedRectangle(Point(3, 8) + getAbsolutePosition(), getSize() - Size(6, 11), 6);

	// draw title if there are no tabs: 
		if(isTabbed==NOT_TABBED)
			drawTitle(dc);
	}
	UI_Object::draw(dc);
}



bool UI_Window::changedFlag=false;
bool UI_Window::resetFlag=false;
