#include "window.hpp"
#include "configuration.hpp"

UI_Window::UI_Window(UI_Object* window_parent,
		const unsigned int window_title_string_id, 
		const Rect rect, 
		const unsigned int max_height,
		const eIsScrolled window_is_scrollable, 
		const eIsAutoAdjust window_is_auto_adjust, 
		const Rect window_client_area,
		const eTransparentWindow transparent_window) :
	UI_Object(window_parent, rect, Size()),
	filledHeight(0),
	doAdjustments(false),
	title(this, window_title_string), // ??
//	titleParameter(""), ?
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
	helpChapter(INDEX_CHAPTER),
	transparentWindow(transparent_window)
{
void UI_Window::drawTitle() const
{
	if(titleString==NULL_STRING)
		return;
	dc->setBrush(*theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH));
	dc->setPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	dc->setFont(theme.lookUpFont(SMALL_BOLD_FONT));
	dc->setTextForeground(*theme.lookUpColor(TITLE_COLOR));

	std::string text;

// TODO UI_StaticText einfuegen!
	if(titleParameter != "")
		text=theme.lookUpFormattedString(titleString, titleParameter);
	else text = theme.lookUpString(titleString);
	
	Size s = dc->getTextExtent(text);
	Rect titleRect = Rect(Point(), s + Size(5, 0));
	
	dc->DrawRectangle(titleRect);
	titleRect.setTopLeft(titleRect.getTopLeft() + Point(2, 3));
	dc->DrawText(text, titleRect.getTopLeft());
}
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
	UI_Object::reloadOriginalSize();
}

void UI_Window::setTitleParameter(const std::string& p) {
	titleParameter = p;
}

UI_Object* UI_Window::checkToolTip()
{
//	if(!(isMouseInside()))
//		return(NULL);
	return(UI_Object::checkToolTip());
}

UI_Object* UI_Window::checkHighlight()
{
	if((!isMouseInside())&&( (scrollBar==NULL) || (!Rect(getAbsolutePosition() + Point(getWidth(), 0), Size(12, getHeight())).isTopLeftCornerInside(mouse))))
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
	
//	if((getScrollBar())&&(getScrollBar()->isPufferInvalid()))
//		makePufferInvalid(); //?

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
	
	
	if(uiConfiguration.isSmoothMovements())
		clientRect.moveSmooth(clientStartRect, clientTargetRect);
	else  clientRect.move(clientTargetRect);

	if(/*(!UI_Object::windowSelected)&&*/(((isMouseInside())||( (scrollBar!=NULL) && (Rect(getAbsolutePosition() + Point(getWidth(), 0), Size(12, getHeight())).isTopLeftCornerInside(mouse))))))//&&(!isTopItem()))) // => main window! WHY? TODO
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
				makePufferInvalid();
				if(UI_Object::currentWindow)
					UI_Object::currentWindow->makePufferInvalid();
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


void UI_Window::drawWindow() const
{
// draw outer border:
	dc->setPen(*theme.lookUpPen(OUTER_BORDER_PEN));
	if(transparentWindow == TRANSPARENT_WINDOW)
		dc->setBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
	else
		dc->setBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));

	dc->DrawEdgedRoundedRectangle(Point(1, 1), getSize() - Size(2, 2), 6);
	
// draw inner border:
	if(UI_Object::currentWindow == this)
		dc->setPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	else
		dc->setPen(*theme.lookUpPen(INNER_BORDER_PEN));
	dc->setBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
	
	dc->DrawEdgedRoundedRectangle(Point(3, 3), getSize() - Size(6, 6), 6);

	if(title)
	{
		dc->setBrush(*theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH));
		dc->setPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));

	//	if(titleParameter != "")
	//		text = theme.lookUpFormattedString(titleString, titleParameter);
	//	else text = theme.lookUpString(titleString);
	
		Rect titleRect = Rect(Point(), title->getTextSize() + Size(5, 0));
		dc->DrawRectangle(titleRect);
	}
// ------ PROCESSING
	calculateClientRect();




	
//	toErrorLog("draw window");
//
//
//	
}

void UI_Window::draw() const
{
	drawWindow();	
	UI_Object::draw();
}

void UI_Window::object_info()
{
	toErrorLog("ui_window");
	toErrorLog(getZ());
}

bool UI_Window::changedFlag=false;
signed int UI_Window::gotoHelpChapter=-1;

bool UI_Window::needSaveBox = false;
bool UI_Window::saveBoxCanceled = false;
bool UI_Window::saveBoxDone = false;
std::string UI_Window::saveBoxString = "";
SaveBoxParameter UI_Window::saveBoxParameter;

