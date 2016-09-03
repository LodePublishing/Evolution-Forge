#ifndef __INPUT_H
#define __INPUT_H

#include "wx/wx.h"

class Controls
{
        public:
                Controls();
                ~Controls();
		int getX();
		int getY();
		void setMouse(wxPoint position);
		int getScroll();
		void scrollMouse(int scroll);
		void leftDown();
		void leftUp();
		void captured();
		int getPressCondition(wxRect rect);
		wxPoint getCurrentPosition();
		wxPoint getCurrentPosition(wxPoint pos);
	private:
                wxPoint current,start,end;
		int scroll;
};

#endif
