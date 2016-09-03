#include "msgwindow.hpp"

MessageWindow::MessageWindow( UI_Window* parentWindow ):
	UI_Window( parentWindow, MESSAGE_WINDOW_TITLE_STRING, theme.lookUpGlobalRect(MESSAGE_WINDOW), theme.lookUpGlobalMaxHeight(MESSAGE_WINDOW), SCROLLED )
{ }
//		resetData();?

MessageWindow::~MessageWindow()
{ }

void MessageWindow::resetData()
{}

void MessageWindow::addMessage( const std::string& bla )
{
	setNeedRedrawNotMoved();
	moveScrollbarToBottom();
	Message msg(getScrollbar(), Rect(Point(5, 10 + message.size() * (FONT_SIZE+5)), Size(getClientRectWidth(), FONT_SIZE+5)), 1, bla, 155);
	message.push_back(msg);
//	if(message.size() > 20)
//		message.pop_front();
	process();
}

void MessageWindow::reloadOriginalSize()
{
	UI_Window::reloadOriginalSize();
}


void MessageWindow::process()
{
	if(!isShown()) 
		return;
	UI_Window::process();
	int t =0;
	for(list<Message>::iterator m=message.begin(); m!=message.end(); ++m)
	{
		m->adjustRelativeRect(Rect(Point(5, 10 + t * (FONT_SIZE+10)), Size(getClientRectWidth(), FONT_SIZE+5)));
		t++;
		if(!fitItemToAbsoluteClientRect(m->getAbsoluteRect()))
			m->Hide();
		else m->Show();
	}
}

void MessageWindow::draw( DC* dc ) const
{
	if(!isShown()) 
		return;
	UI_Window::draw(dc);
}

