#ifndef __MESSAGE_H
#define __MESSAGE_H

#include "graphics.h"

class MessageWindow:public GraphixScrollWindow
{
        public:
                MessageWindow(wxRect rahmen, wxRect maxSize);
                ~MessageWindow();
                void addMessage(wxString bla);
                void resetData();
                void drawMessages(wxDC* dc);
        private:
                struct Message
                {
                        int type;
                        wxString string;
                        int col;
                } message[1000];
                int msgCount;
};

#endif

