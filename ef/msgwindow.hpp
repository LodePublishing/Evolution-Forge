#ifndef _GUI_MSGWINDOW_HPP
#define _GUI_MSGWINDOW_HPP

#include "../ui/window.hpp"
#include "message.hpp"

class MessageWindow:public UI_Window
{
	public:
		MessageWindow(UI_Window* parent_window);
		~MessageWindow();
		void resetData();
		void process();
		void draw() const;
		void reloadOriginalSize();
	private:
		std::list<Message*> message;
};

#endif

