#include "window.hpp"
#include "configuration.hpp"

UI_Window::UI_Window(UI_Object* window_parent, const eString window_title_string, const Rect rect, const unsigned int max_height, const eIsScrolled window_is_scrollable, const eIsAutoAdjust window_is_auto_adjust, const Rect window_client_area, eIsTransparent transparent) :
	UI_Object(window_parent, rect),
	filledHeight(0),
	doAdjustments(false),
	isTransparent(transparent),
	titleString(window_title_string), // ??
	titleParameter(""),
	clientRect(window_client_area),
	clientStartRect(),
	clientTargetRect(),
	originalClientRect(), // TODO 
	maxHeight(max_height),
	isAutoAdjust(window_is_auto_adjust), //?
	isScrollable(window_is_scrollable), // TODO
	highlighted(false),
	scrollBar(isScrollable==NOT_SCROLLED?NULL:new UI_ScrollBar(this, getRelativeClientRectUpperBound(), getMaxHeight())),
	helpButton(NULL),
	helpChapter(INDEX_CHAPTER)
{

// ------ PROCESSING
	calculateClientRect();


//	filledHeight=clientRect.getHeight();
// ------ Buttons, ScrollBars etc.
}

UI_Window::~UI_Window()
{
	delete scrollBar;
	delete helpButton;
}

void UI_Window::addHelpButton(eHelpChapter help_chapter)
{
	helpButton = new UI_Button(this, Rect(Point(0,10), Size(13, 14)), Size(5 + (isScrollable==SCROLLED?8:0),10), HELP_BUTTON, HELP_BITMAP, PRESS_BUTTON_MODE, NULL_STRING, TOP_RIGHT);
	helpChapter = help_chapter;	
}

void UI_Window::calculateClientRect()
{
	Rect oldClientRect = clientRect;
	clientRect.setSize(getTargetRect().getSize());
	
	if(clientRect.getLeft() < 5) 
		clientRect.setLeft(5);
	if(clientRect.getLeft() > ((signed int)getWidth()) - 5)
		clientRect.setLeft(((signed int)getWidth()) -5);
	
	if(clientRect.getTop() < 5) 
		clientRect.setTop(5);
	if(clientRect.getTop() > ((signed int)getHeight()) - 5)
		clientRect.setTop(((signed int)getHeight()) - 5);

	if(clientRect.getBottom() > ((signed int)getHeight()) - 5) 
		clientRect.setBottom(((signed int)getHeight()) - 5);

	if(clientRect.getRight() > ((signed int)getWidth()) - 5) 
		clientRect.setRight(((signed int)getWidth()) - 5);

	originalClientRect = clientRect;
	clientStartRect = oldClientRect;
	clientTargetRect = clientRect;
	if(scrollBar)
		scrollBar->setStartY(getRelativeClientRectUpperBound());
}

void UI_Window::reloadOriginalSize()
{
	setRect(getOriginalRect());
	//adjustRelativeRect(getOriginalRect());
	clientRect.setSize(getOriginalRect().getSize());
	calculateClientRect();
	adjustClientRect();
//	setNeedRedrawMoved(); //?
	UI_Object::reloadOriginalSize();
}

void UI_Window::setTitleParameter(const std::string& p) {
	titleParameter=p;
}

UI_Object* UI_Window::checkToolTip()
{
//	if(!(isMouseInside()))
//		return(NULL);
	return(UI_Object::checkToolTip());
}

UI_Object* UI_Window::checkHighlight()
{
	if((!isMouseInside())&&( (scrollBar==NULL) || (!Rect(getAbsolutePosition() + Point(getWidth(), 0), Size(12, getHeight())).isInside(mouse))))
		return(NULL);
	return(UI_Object::checkHighlight());
}

// ----------------------
// ------ MOVEMENT ------
// ----------------------


void UI_Window::adjustClientRect()
{
//CLIENT WINDOW
	unsigned int width = getTargetWidth() + originalClientRect.getWidth();
	unsigned int height = getTargetHeight() + originalClientRect.getHeight();

	if(width < getOriginalRect().getWidth())
		width = 0;
	else width -= getOriginalRect().getWidth();

	if(height < getOriginalRect().getHeight())
		height = 0;
	else height -= getOriginalRect().getHeight();

	if(width!=clientTargetRect.getWidth())
		clientStartRect.setWidth(clientRect.getWidth());
	if(height!=clientTargetRect.getHeight())
		clientStartRect.setHeight(clientRect.getHeight());

	clientTargetRect.setWidth(width);
	clientTargetRect.setHeight(height);
}

void UI_Window::wheelUp()
{
	if(scrollBar)
		scrollBar->moveUp();
}

void UI_Window::wheelDown()
{
	if(scrollBar)
		scrollBar->moveDown();
}

void UI_Window::moveScrollBarToTop()
{
	if(scrollBar)
		scrollBar->moveToTop();
}

void UI_Window::moveScrollBarToBottom()
{
	if(scrollBar)
		scrollBar->moveToBottom();
}

void UI_Window::process()
{
	if(!isShown()) 
		return;
	
	if((getScrollBar())&&(getScrollBar()->checkForNeedRedraw()))
		setNeedRedrawNotMoved();

	adjustClientRect();
	if(doAdjustments==true)
	{
		adjustRelativeRect(Rect(Point(getTargetRect().getTopLeft()), Size(getTargetRect().getWidth(), filledHeight+25))); // TODO!
		adjustClientRect();
		doAdjustments=false;
	}

	calculateClientRect();
	
	if(scrollBar)
	{
		scrollBar->setClientHeight(getClientRectHeight());
		scrollBar->setClientTargetHeight(getClientTargetHeight());
	}

	UI_Object::process();

	if(helpButton!=NULL)
	{
		if(isMouseInside())
		{
			helpButton->Show();
			if(helpButton->isLeftClicked())
				UI_Window::gotoHelpChapter = helpChapter;
		} else helpButton->Hide();
	}
	
	
	if(clientRect != clientTargetRect)
	{
                Rect old_rect = clientRect;
                eRectMovement t = uiConfiguration.isSmoothMovements() ? clientRect.moveSmooth(clientStartRect, clientTargetRect) : clientRect.move(clientTargetRect);
                if(t == GOT_BIGGER)
                        setNeedRedrawNotMoved();
                else if(t == GOT_SMALLER_OR_MOVED)
                        setNeedRedrawMoved(old_rect, clientRect);
	}

	if(/*(!UI_Object::windowSelected)&&*/(((isMouseInside())||( (scrollBar!=NULL) && (Rect(getAbsolutePosition() + Point(getWidth(), 0), Size(12, getHeight())).isInside(mouse))))))//&&(!isTopItem()))) // => main window! WHY? TODO
	{
		bool new_window = false;
		if(UI_Object::currentWindow != this)
		{
			// current window is inside this window
			if( (UI_Object::currentWindow==NULL)||
				(!UI_Object::currentWindow->isMouseInside())||
   			    (UI_Object::currentWindow->getAbsolutePosition() < getAbsolutePosition())||
			    (UI_Object::currentWindow->getAbsoluteRect().getBottomRight() > getAbsoluteRect().getBottomRight()) ) 
			{
				setNeedRedrawNotMoved();
				if(UI_Object::currentWindow)
					UI_Object::currentWindow->setNeedRedrawNotMoved();
				new_window = true;
			}
		} else new_window = true;
		if(new_window)
		{
			UI_Object::currentWindow = this;
			UI_Object::windowSelected = true;
		}
	}


	filledHeight=0;
}



const bool UI_Window::fitItemToRelativeClientRect(const Rect& rect, const bool adjust)
{
	if(scrollBar)
	{
		if(scrollBar->getFirstItemY() == 0)
			scrollBar->setFirstItemY(rect.getTop());
		if(scrollBar->getLastItemY() < rect.getBottom())
			scrollBar->setLastItemY(rect.getBottom());
	}

	if((rect.getLeft() >= getRelativeClientRectRightBound())||
//	   (rect.getTop()>=getRelativeClientRectLowerBound())||
	   (rect.getRight() < getRelativeClientRectLeftBound())||
	   (rect.getBottom() < getRelativeClientRectUpperBound()))
		return(false);
/*		
	if(rect.getLeft()<getRelativeClientRectLeftBound()) 
		rect.setLeft(getRelativeClientRectLeftBound());
	if((rect.getRight()>getRelativeClientRectRightBound()))
		rect.setRight(getRelativeClientRectRightBound());*/
//  if(rect.y<clientArea.y) rect.y=clientArea.y;
//	if(rect.y+rect.height>clientArea.y+clientArea.height) rect.height=clientArea.y+clientArea.height-rect.y;
	if(adjust)
	{
		doAdjustments = true;
		if((rect.getBottom()>0) &&((unsigned int)(rect.getBottom())/*-getRelativeClientRectUpperBound()*/>filledHeight))
			filledHeight = rect.getBottom()/*-getRelativeClientRectUpperBound()*/;
		if(filledHeight>getMaxHeight()) 
			filledHeight=getMaxHeight();
	}
	if((rect.getTop()<getRelativeClientRectUpperBound())||
	  (rect.getBottom()>getRelativeClientRectLowerBound())) 
		return false;
	else return true;
}

const bool UI_Window::fitItemToAbsoluteClientRect(const Rect& rect, const bool adjust)
{
	if(scrollBar)
	{
		if(scrollBar->getFirstItemY() == 0)
			scrollBar->setFirstItemY(rect.getTop() - getAbsoluteClientRectUpperBound());
		if(scrollBar->getLastItemY() < rect.getBottom() - getAbsoluteClientRectUpperBound())
			scrollBar->setLastItemY(rect.getBottom() - getAbsoluteClientRectUpperBound());
	}
	
	if((rect.getLeft() >= getAbsoluteClientRectRightBound())||
	   (rect.getTop() >= getAbsoluteClientRectLowerBound())||
	   (rect.getRight() < getAbsoluteClientRectLeftBound())||
	   (rect.getBottom() < getAbsoluteClientRectUpperBound()))
		return(false);
/*	if(rect.getLeft() < getAbsoluteClientRectLeftBound()) 
		rect.setLeft(getAbsoluteClientRectLeftBound());
		
	if(rect.getRight() > getAbsoluteClientRectRightBound())
		rect.setRight(getAbsoluteClientRectRightBound());*/
//  if(rect.y<clientArea.y) rect.y=clientArea.y;
  //	  if(rect.y+rect.height>clientArea.y+clientArea.height) rect.height=clientArea.y+clientArea.height-rect.y;
	if(adjust)
	{
		doAdjustments = true;
		if((rect.getBottom() > getAbsoluteClientRectUpperBound())&&((unsigned int)(rect.getBottom() - getAbsoluteClientRectUpperBound()) > filledHeight))
		filledHeight = (unsigned int) (rect.getBottom()-getAbsoluteClientRectUpperBound());
		if(filledHeight > getMaxHeight()) 
			filledHeight = getMaxHeight();
	}
	if((rect.getTop() < getAbsoluteClientRectUpperBound())||
	 (rect.getBottom() > getAbsoluteClientRectLowerBound())) 
		return false;
	else return true;
}


void UI_Window::drawTitle(DC* dc) const
{
	if(titleString==NULL_STRING)
		return;
	dc->setBrush(*theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH));
	dc->setPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	dc->setFont(theme.lookUpFont(SMALL_BOLD_FONT));
	dc->setTextForeground(*theme.lookUpColor(TITLE_COLOR));

	std::string text;


	// TODO UI_StaticText einfuegen!
	if(titleParameter!="")
		text=theme.lookUpFormattedString(titleString, titleParameter);
	else text = theme.lookUpString(titleString);
	
	Size s = dc->getTextExtent(text);
	Rect titleRect = Rect(getAbsolutePosition() - Size(0,0), s - Size(0, 0) + Size(5,0));
	
	dc->DrawRectangle(titleRect);
	titleRect.setTopLeft(titleRect.getTopLeft()+Point(2,3));
	dc->DrawText(text, titleRect.getTopLeft());
}

void UI_Window::drawWindow(DC* dc) const
{
	if(checkForNeedRedraw())
	{
	// draw outer border:
		dc->setPen(*theme.lookUpPen(OUTER_BORDER_PEN));
		if(isTransparent==TRANSPARENT) // => main window!
			dc->setBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
		else
			dc->setBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
		dc->DrawEdgedRoundedRectangle(Point(1, 1) + getAbsolutePosition(), getSize() - Size(2, 2), 6);
		
	// draw inner border:
		if(UI_Object::currentWindow == this)
			dc->setPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
		else
			dc->setPen(*theme.lookUpPen(INNER_BORDER_PEN));
		dc->setBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
		
		dc->DrawEdgedRoundedRectangle(Point(3, 3) + getAbsolutePosition(), getSize() - Size(6, 6), 6);
		
		drawTitle(dc);
	}
}

void UI_Window::draw(DC* dc) const
{
	drawWindow(dc);	
	UI_Object::draw(dc);
}



bool UI_Window::changedFlag=false;
signed int UI_Window::gotoHelpChapter=-1;

bool UI_Window::needSaveBox = false;
bool UI_Window::saveBoxCanceled = false;
bool UI_Window::saveBoxDone = false;
std::string UI_Window::saveBoxString = "";
SaveBoxParameter UI_Window::saveBoxParameter;

