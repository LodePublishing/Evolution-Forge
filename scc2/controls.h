#ifndef __INPUT_H
#define __INPUT_H

#include "defs.h"

const int LEFT_BUTTON = 1;
const int MIDDLE_BUTTON = 2;
const int RIGHT_BUTTON = 4;

const int NOT_PRESSED=0;
const int POINTER_OVER_BUTTON=1;
const int PRESSING_LEFT_BUTTON=2;
const int LEFT_BUTTON_WAS_PRESSED_AND_RELEASED=4;
const int PRESSING_MIDDLE_BUTTON=8;
const int MIDDLE_BUTTON_WAS_PRESSED_AND_RELEASED=16;
const int PRESSING_RIGHT_BUTTON=32;
const int RIGHT_BUTTON_WAS_PRESSED_AND_RELEASED=64;

class Controls
{
	public:
		Controls();
		~Controls();
		Point getDragStartPosition();
		int getX();
		int getY();
		void setMouse(const Point mouse_position);
		void scrollMouseWheel(const int scroll_wheel);
		void rightDown();
		void rightUp();
		void leftDown();
		void leftUp();
		void middleDown();
		void middleUp();
	
		int isShiftPressed();
		void setShiftPressed(const int shift_pressed=1);
		int getPressConditionOf(const Rect rectangle);
		Point getCurrentPosition();
		Point getCurrentPosition(const Point relative_position);
		void mouseEventWasProcessed();
	private:
		
		Point currentPosition; // current position of the mouse
		Point dragStartPosition; // mouse position where dragging (pressed mouse button + mouse pointer moves) began, -1/-1 if it was not pressed
		Point dragEndPosition; // mouse position where the mouse button was released, -1/-1 if it was not pressed / is still pressed
		int shiftPressed;
		int mouseWheelY;
		int button;
};

extern Controls controls;

#endif
