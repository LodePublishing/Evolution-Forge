#include "msgwindow.hpp"

MessageWindow::MessageWindow( UI_Window* parent_window ) :
	UI_Window( parent_window, MESSAGE_WINDOW_TITLE_STRING, theme.lookUpGlobalRect(MESSAGE_WINDOW), theme.lookUpGlobalMaxHeight(MESSAGE_WINDOW), SCROLLED, NO_AUTO_SIZE_ADJUST, Rect(0, 10, 1280, 1024) ),
	message()
{ }

MessageWindow::~MessageWindow()
{ 
	std::list<Message*>::iterator i = message.begin(); 
	while(i != message.end())
	{
		delete(*i);
		i = message.erase(i);
	}
}

void MessageWindow::process()
{
	if(!isShown()) 
		return;
	if(remainingMessages.size())
	{
		makePufferInvalid();
		for(std::list<std::string>::iterator i = UI_Object::remainingMessages.begin(); i != UI_Object::remainingMessages.end(); )
		{
			Message* my_message = new Message(getScrollBar(), Rect(Point(10, 20  /*message.size() * (FONT_SIZE+5)*/), Size(getClientRectWidth(), FONT_SIZE+5)), 1, *i, 100);
			message.push_front(my_message);
			i = remainingMessages.erase(i);
		}
		resetData();
//	addToProcessArray(this);
//	if(message.size() > 20)
//		message.pop_front();
//	moveScrollbarToBottom();
	}
	UI_Window::process();
	getScrollBar()->checkBoundsOfChildren(getAbsoluteClientRectUpperBound()-15, getAbsoluteClientRectLowerBound());
}

void MessageWindow::draw() const
{
	drawWindow();
	dc->setFont(UI_Object::theme.lookUpFont(LARGE_SHADOW_BOLD_FONT));
	Point point = Point(getRelativeClientRectPosition() + Size(10, getHeight()*11/16));
	dc->setTextForeground(DC::toSDL_Color(70, 70, 100));
	dc->DrawText("Evolution Forge", point);
	UI_Object::draw();
}

void MessageWindow::resetData()
{
	int t =0;
	for(std::list<Message*>::iterator m=message.begin(); m!=message.end(); ++m)
	{
		(*m)->setOriginalRect(Rect(Point(10, 20 + t * (FONT_SIZE+5)), Size(getClientRectWidth(), FONT_SIZE+5)));
		t++;
	}
}

void MessageWindow::reloadOriginalSize()
{
	setOriginalRect(UI_Object::theme.lookUpGlobalRect(MESSAGE_WINDOW));
	setMaxHeight(UI_Object::theme.lookUpGlobalMaxHeight(MESSAGE_WINDOW));
	UI_Window::reloadOriginalSize();
	resetData();
}


