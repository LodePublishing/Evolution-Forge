#ifndef __MESSAGE_H
#define __MESSAGE_H

#include "UI_Window.h"
#include <list>
using std::list;

struct Message
{
	int type;
	string string;
	int col;
};

class MessageWindow:public UI_Window
{
        public:
                MessageWindow(UI_Window* parentWindow);
                ~MessageWindow();
                void addMessage(string bla);
                void resetData();
				void process();
		void draw(DC* dc);
	private:
		list <Message> message;
};

#endif

