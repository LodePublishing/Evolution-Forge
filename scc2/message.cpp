#include "message.h"

MessageWindow::MessageWindow(UI_Window* parentWindow):UI_Window(parentWindow, MESSAGE_WINDOW_TITLE_STRING, *theme.lookUpRect(MESSAGE_WINDOW), *theme.lookUpMaxRect(MESSAGE_WINDOW), SCROLLED)
{
        resetData();
};
                                                                                                                                                            
MessageWindow::~MessageWindow()
{
};
                                                                                                                                                            
void MessageWindow::resetData()
{
        msgCount=0;
        for(int i=0;i<1000;i++)
        {
                message[i].type=0;
                message[i].col=0;
                message[i].string=_T("");
        }
};
                                                                                                                                                            
void MessageWindow::addMessage(wxString bla)
{
//        setScrollY(0); TODO
        message[msgCount].col=155;
        message[msgCount].type=1;
        message[msgCount].string=bla;
        msgCount++;
};

void MessageWindow::process()
{
	UI_Window::process();
};
                                                                                                                                                            
void MessageWindow::draw(wxDC* dc)
{
	UI_Window::draw(dc);
        int t;
        t=0;
        dc->SetFont(*theme.lookUpFont(SMALL_ITALICS_BOLD_FONT));
        for(int i=msgCount;i--;)
                if(message[i].type>0)
                {
                        dc->SetTextForeground(wxColour(100+message[i].col,100+message[i].col,255));
                        if(message[i].col>5) message[i].col-=message[i].col/5+1;
                        else message[i].col=0;
                        wxRect edge=wxRect(getClientRectPosition()+wxPoint(5,8+t*(FONT_SIZE+5)-getScrollY()),wxSize(getClientRectWidth(),FONT_SIZE+5));
                        if(fitItemToClientRect(edge))
                        {
//                              dc->DrawBitmap(hintBitmap,edge.x,edge.y);
                                dc->SetPen(wxPen(wxColour(20,20,20),1,wxSOLID));
                                dc->SetBrush(wxBrush(wxColour(30,30,30),wxSOLID));
                                dc->DrawRoundedRectangle(edge,3);
                                dc->DrawText(message[i].string,edge.x,edge.y);
                        }
                        t++;
                }
//      setMaxScrollY(t*(FONT_SIZE+5));
};
                                                                                                                                                            

