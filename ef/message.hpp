#ifndef _GUI_MESSAGE_HPP
#define _GUI_MESSAGE_HPP

#include "../ui/statictext.hpp"

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

#endif

