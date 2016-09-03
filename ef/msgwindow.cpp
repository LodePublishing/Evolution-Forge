#include "msgwindow.hpp"

MessageWindow::MessageWindow( UI_Window* parentWindow ):
	UI_Window( parentWindow, MESSAGE_WINDOW_TITLE_STRING, theme.lookUpGlobalRect(MESSAGE_WINDOW), theme.lookUpGlobalMaxHeight(MESSAGE_WINDOW), SCROLLED, NO_AUTO_SIZE_ADJUST, NOT_TABBED, Rect(0, 10, 1280, 1024) ),
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
{}

void MessageWindow::addMessage( const std::string& bla )
{
	setNeedRedrawNotMoved();
	Message* msg = new Message(getScrollbar(), Rect(Point(10, 20  /*message.size() * (FONT_SIZE+5)*/), Size(getClientRectWidth(), FONT_SIZE+5)), 1, bla, 155);
	message.push_front(msg);
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
}

void MessageWindow::process()
{
	if(!isShown()) 
		return;
	UI_Window::process();
	int t =0;
	for(list<Message*>::iterator m=message.begin(); m!=message.end(); ++m)
	{
		(*m)->setOriginalRect(Rect(Point(10, 20 + t * (FONT_SIZE+5)), Size(getClientRectWidth(), FONT_SIZE+5)));
		t++;
		if(!fitItemToAbsoluteClientRect((*m)->getAbsoluteRect()))
			(*m)->Hide();
		else (*m)->Show();
		if((*m)->checkForNeedRedraw())
			setNeedRedrawNotMoved();
	}
}

void MessageWindow::draw( DC* dc ) const
{
	if(!isShown()) 
		return;
	UI_Window::draw(dc);
	if(checkForNeedRedraw())
	{
		dc->SetFont(UI_Object::theme.lookUpFont(LARGE_BOLD_FONT));
//		std::string str="Evolution Forge";
//		Size s;
		Point point = Point(getAbsoluteClientRectPosition() + Size(getWidth()*9/20, getHeight()*3/4));
//		for(unsigned int i=0;i<str.size();++i)
//			s = helper(dc, point, s.GetWidth(), i, str.substr(0, i+1));
		dc->SetTextForeground(DC::toSDL_Color(10, 10, 100));
		dc->DrawText("Evolution Forge v1.64", point+Size(2,2));
		dc->SetTextForeground(DC::toSDL_Color(75, 75, 100));
		dc->DrawText("Evolution Forge v1.64", point);
//		dc->SetTextForeground(DC::toSDL_Color(0,0,85));
//		dc->DrawText(CORE_VERSION, point + Size(20, 30) + Size(2,2));
//		dc->SetTextForeground(DC::toSDL_Color(50, 50, 85));
//		dc->DrawText(CORE_VERSION, point + Size(20, 30)); TODO
//		if(UI_Object::tooltip)
//			UI_Object::tooltip->draw(dc);
	}
}

