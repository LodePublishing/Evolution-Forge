#include "message.hpp"

Message::Message(UI_Object* msg_parent, Rect rect, const unsigned int msg_type, const std::string& msg_string, const unsigned int msg_color) :
	UI_StaticText(msg_parent, msg_string, rect, FORCE_TEXT_COLOR, SMALL_ITALICS_BOLD_FONT), 
	type(msg_type),
	col(msg_color)
{ }

Message::~Message()
{ }

void Message::process()
{
	if(type>0)
	{
		if(col>5) 
		{
			col -= col/5+1;
			setNeedRedrawMoved();
		}
		else col=0;
	}
}

void Message::draw(DC* dc) const
{
	if(!isShown())
		return;
	const_cast<Message*>(this)->setColor(dc->doColor(col+100, col+100, col+100));
	UI_StaticText::draw(dc);
}

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

