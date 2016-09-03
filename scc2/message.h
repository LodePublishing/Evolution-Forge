#ifndef __MESSAGE_H
#define __MESSAGE_H

#include "UI_Window.h"

class MessageWindow:public UI_Window
{
        public:
                MessageWindow(UI_Window* parentWindow);
                ~MessageWindow();
                void addMessage(wxString bla);
                void resetData();
				void process();
                void draw(wxDC* dc);
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

