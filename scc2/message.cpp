#include "message.h"

MessageWindow::MessageWindow(UI_Window* parentWindow):UI_Window(parentWindow, MESSAGE_WINDOW_TITLE_STRING, MESSAGE_WINDOW, SCROLLED)
{
//        resetData();
};
                                                                                                                                                            
MessageWindow::~MessageWindow()
{
};
                                                                                                                                                            
void MessageWindow::addMessage(string bla)
{
	if(!shown) return;
//        setScrollY(0); TODO
	Message msg;
	msg.col=155;
	msg.type=1;
	msg.string=bla;
	message.push_back(msg);
	if(message.size()>5)
		message.pop_front();
};

void MessageWindow::process()
{
	if(!shown) return;
	UI_Window::process();
};
                                                                                                                                                            
void MessageWindow::draw(DC* dc)
{
	if(!shown) return;
	UI_Window::draw(dc);
        int t;
        t=0;
        dc->SetFont(theme.lookUpFont(SMALL_ITALICS_BOLD_FONT));
		for(list<Message>::iterator m=message.begin(); m!=message.end(); ++m)
			if(m->type>0)
			{
//                        dc->SetTextForeground(dc->doColor(100+m->col,100+m->col,255)); TODO
                        if(m->col>5) m->col-=m->col/5+1;
                        else m->col=0;
                        Rect edge=Rect(getAbsoluteClientRectPosition()+Point(5,8+t*(FONT_SIZE+5)-getScrollY()),Size(getClientRectWidth(),FONT_SIZE+5));
                        if(fitItemToAbsoluteClientRect(edge))
                        {
//                              dc->DrawBitmap(hintBitmap,edge.x,edge.y);
  //                              dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
										//Pen(dc->doColor(20,20,20),1,SOLID_PEN_STYLE));
//                                dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
										//Brush(dc->doColor(30,30,30),SOLID_BRUSH_STYLE));
//                                dc->DrawRoundedRectangle(edge,3);
                                dc->DrawText(m->string, edge.GetPosition());
                        }
                        t++;
                }
//      setMaxScrollY(t*(FONT_SIZE+5));
};
                                                                                                                                                            

