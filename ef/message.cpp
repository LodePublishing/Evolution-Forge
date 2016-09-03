#include "message.hpp"

Message::Message(const unsigned int msg_type, const string& msg_string, const unsigned int msg_col):
	rect(),
	type(msg_type),
	messageString(msg_string),
	col(msg_col)
{ }

Message::~Message()
{ }

MessageWindow::MessageWindow( UI_Window* parentWindow ):
	UI_Window( parentWindow, MESSAGE_WINDOW_TITLE_STRING, MESSAGE_WINDOW, SCROLLED )	
{ }
//		resetData();?


MessageWindow::~MessageWindow()
{ }

void MessageWindow::addMessage( const string& bla )
{
//	if(!isShown()) 
//		return;
//		setScrollY(0); TODO
	Message msg(1, bla, 155);
	message.push_back(msg);
	if(message.size() > 8)
		message.pop_front();
	process();
}

void MessageWindow::process()
{
	if(!isShown()) return;
	UI_Window::process();
	int t=0;
	for(list<Message>::iterator m=message.begin(); m!=message.end(); ++m)
		if(m->type>0)
		{
			if(m->col>5) 
				m->col-=m->col/5+1;
			else m->col=0;
			m->rect = Rect(getAbsoluteClientRectPosition() + Point(5, 8+t*(FONT_SIZE+5)/*-getScrollY()*/), Size(getClientRectWidth(), FONT_SIZE+5));
			if(fitItemToAbsoluteClientRect(m->rect));
			// TODO
			t++;
		}
//	  setMaxScrollY(t*(FONT_SIZE+5));
}

void MessageWindow::draw( DC* dc ) const
{
	if(!isShown()) return;
	if(!checkForNeedRedraw())
	{
		UI_Window::draw(dc);
		return;
	}
	UI_Window::draw(dc);
	
	
	dc->SetFont(theme.lookUpFont(SMALL_ITALICS_BOLD_FONT));
	dc->SetTextForeground(*theme.lookUpColor(FORCE_TEXT_COLOR));
	for(list<Message>::const_iterator m=message.begin(); m!=message.end(); ++m)
//		if(m->type>0)
		{
//			if(insideAbsoluteClientRect(m->rect))
			{
//				dc->DrawBitmap(hintBitmap,rect.x,rect.y);
// 				dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
//				Pen(dc->doColor(20,20,20),1,SOLID_PEN_STYLE));
//				dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
//				Brush(dc->doColor(30,30,30),SOLID_BRUSH_STYLE));
//				dc->DrawRoundedRectangle(edge,3);
				dc->SetTextForeground(dc->doColor(m->col+100, m->col+100, m->col+100));
				dc->DrawText(m->messageString, m->rect.GetTopLeft());
			}
		}
}

