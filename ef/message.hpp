#ifndef _GUI_MESSAGE_HPP
#define _GUI_MESSAGE_HPP

#include "../ui/window.hpp"

#include <list>

using std::list;
class Message
{
	public:
		Message(const unsigned int msg_type, const string& msg_string, const unsigned int msg_col);
		~Message();
		Rect rect;
		unsigned int type;
		string messageString;
		unsigned int col;
};

class MessageWindow:public UI_Window
{
	public:
		MessageWindow(UI_Window* parentWindow);
		~MessageWindow();
		void addMessage(const string& bla);
		void resetData();
		void process();
		void draw(DC* dc) const;
	private:
		list <Message> message;
};

#endif

