#ifndef _GUI_MESSAGE_HPP
#define _GUI_MESSAGE_HPP

#include "../ui/window.hpp"

#include <list>

using std::list;

class MessageWindow:public UI_Window
{
	struct Message
	{
		int type;
		string string;
		int col;
		Rect edge;
	};
	public:
		MessageWindow(UI_Window* parentWindow);
		~MessageWindow();
		void addMessage(string bla);
		void resetData();
		void process();
		void draw(DC* dc) const;
	private:
		list <Message> message;
};

#endif

