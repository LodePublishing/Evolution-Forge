#ifndef __INPUT_H
#define __INPUT_H

#include "wx/wx.h"

const int NOT_PRESSED=0;
const int POINTER_OVER_BUTTON=1;
const int PRESSING_BUTTON=2;
const int PRESSING_BUTTON_BUT_OUTSIDE=3;
const int BUTTON_WAS_PRESSED=4;

class Controls
{
        public:
                Controls();
                ~Controls();
		int isShiftPressed();
		int getX();
		int getY();
		void setMouse(wxPoint position);
		int getScroll();
		void scrollMouse(int scroll);
		void rightDown();
		void rightUp();
		void leftDown();
		void leftUp();
		void captured();
		void setShiftPressed(int p);
		int getPressCondition(wxRect rect);
		wxPoint getCurrentPosition();
		wxPoint getCurrentPosition(wxPoint pos);
	private:
                wxPoint current,start,end;
		int shiftPressed;
		int scroll;
		bool left;
};

#endif
