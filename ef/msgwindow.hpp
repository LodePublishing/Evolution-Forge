#ifndef _GUI_MSGWINDOW_HPP
#define _GUI_MSGWINDOW_HPP

#include "../ui/window.hpp"
#include "message.hpp"

class MessageWindow:public UI_Window
{
	public:
		MessageWindow(UI_Window* parentWindow);
		~MessageWindow();
		void addMessage(const std::string& bla);
		void resetData();
		void process();
		void draw(DC* dc) const;
		void reloadOriginalSize();
	private:
		std::list<Message*> message;
};

#endif

