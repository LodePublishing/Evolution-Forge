#include "msgwindow.hpp"

MessageWindow::MessageWindow( UI_Window* parentWindow ):
	UI_Window( parentWindow, MESSAGE_WINDOW_TITLE_STRING, theme.lookUpGlobalRect(MESSAGE_WINDOW), theme.lookUpGlobalMaxHeight(MESSAGE_WINDOW), SCROLLED, NO_AUTO_SIZE_ADJUST, Rect(0, 10, 1280, 1024) ),
	message()
{}

MessageWindow::~MessageWindow()
{ 
	std::list<Message*>::iterator i = message.begin(); 
	while(i != message.end())
	{
		delete(*i);
		i = message.erase(i);
	}
}

void MessageWindow::resetData()
{
	int t =0;
	for(list<Message*>::iterator m=message.begin(); m!=message.end(); ++m)
	{
		(*m)->setOriginalRect(Rect(Point(10, 20 + t * (FONT_SIZE+5)), Size(getClientRectWidth(), FONT_SIZE+5)));
		t++;
	}
}

void MessageWindow::addMessage( const std::string& bla )
{
	setNeedRedrawNotMoved();
	Message* msg = new Message(getScrollBar(), Rect(Point(10, 20  /*message.size() * (FONT_SIZE+5)*/), Size(getClientRectWidth(), FONT_SIZE+5)), 1, bla, 100);
	message.push_front(msg);
	resetData();
//	addToProcessArray(this);
	
//	if(message.size() > 20)
//		message.pop_front();
//	moveScrollbarToBottom();
//	process();
}

void MessageWindow::reloadOriginalSize()
{
	setOriginalRect(UI_Object::theme.lookUpGlobalRect(MESSAGE_WINDOW));
	setMaxHeight(UI_Object::theme.lookUpGlobalMaxHeight(MESSAGE_WINDOW));
	UI_Window::reloadOriginalSize();
	resetData();
}

void MessageWindow::process()
{
	if(!isShown()) 
		return;
	UI_Window::process();
	getScrollBar()->checkBoundsOfChildren(getAbsoluteClientRectUpperBound()-15, getAbsoluteClientRectLowerBound());
}

void MessageWindow::draw( DC* dc ) const
{
	if(!isShown()) 
		return;
	drawWindow(dc);
	if(checkForNeedRedraw())
	{
		dc->setFont(UI_Object::theme.lookUpFont(LARGE_SHADOW_BOLD_FONT));
		Point point = Point(getAbsoluteClientRectPosition() + Size(10, getHeight()*11/16));
		dc->setTextForeground(DC::toSDL_Color(70, 70, 100));
		dc->DrawText("Evolution Forge", point);
	}
	UI_Object::draw(dc);
}

