#ifndef _GUI_MESSAGE_HPP
#define _GUI_MESSAGE_HPP

#include "../ui/window.hpp"

#include <list>

using std::list;
class Message : public UI_StaticText
{
	public:
		Message(UI_Object* msg_parent, Rect rect, const unsigned int msg_type, const std::string& msg_string, const unsigned int color);
		~Message();
		void draw(DC* dc) const;
		void process();
		unsigned int type;
		unsigned int col;
};

class MessageWindow:public UI_Window
{
	public:
		MessageWindow(UI_Window* parentWindow);
		~MessageWindow();
		void addMessage(const std::string& bla);
		void resetData();
		void process();
		void draw(DC* dc) const;
	private:
		list <Message> message;
};

#endif

