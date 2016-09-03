#include "message.hpp"

Message::Message(UI_Object* msg_parent, const Rect& rect, const unsigned int msg_type, const std::string& msg_string, const unsigned int msg_color) :
	UI_StaticText(msg_parent, msg_string, rect, Size(0,0), WINDOW_TEXT_COLOR, SMALL_BOLD_FONT, DO_NOT_ADJUST), 
	type(msg_type),
	col(msg_color)
{ }

Message::~Message()
{ }


void Message::process()
{
	if(type>0)
	{
		if(col>5) 
		{
			col -= col/5+1;
			setNeedRedrawMoved();
		}
		else col=0;
	}
}

void Message::draw(DC* dc) const
{
	if(!isShown())
		return;
	const_cast<Message*>(this)->setTemporaryColor(dc->doColor(col+155, col+155, col+155));
	UI_StaticText::draw(dc);
}

