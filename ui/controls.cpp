#include "controls.hpp"

Controls::Controls()
{
	currentPosition = Point(-1, -1);
	mouseEventWasProcessed();
	shiftPressed = 0;
	button = 0;
}

Controls::~Controls()
{
}

int Controls::getX()
{
	return (currentPosition.x);
}

int Controls::getY()
{
	return (currentPosition.y);
}

void Controls::setShiftPressed(int shift_pressed)
{
	shiftPressed = shift_pressed;
}

int Controls::isShiftPressed()
{
	return (shiftPressed);
}

Point Controls::getCurrentPosition()
{
	return (currentPosition);
}

Point Controls::getCurrentPosition(const Point relativePosition)
{
	return (currentPosition - relativePosition);
}

void Controls::setMouse(Point position)
{
	currentPosition = position;
}

void Controls::scrollMouseWheel(int scroll)
{
	mouseWheelY+=scroll;
}

// TODO mouseWheel

void Controls::rightDown()
{
	button |= RIGHT_BUTTON;
	dragStartPosition = currentPosition;
	dragEndPosition = Point(-1, -1);
}

void Controls::rightUp()
{
	button &= ~RIGHT_BUTTON;
	dragEndPosition = currentPosition;
}

void Controls::leftDown()
{
	button |= LEFT_BUTTON;
	dragStartPosition = currentPosition;
	dragEndPosition = Point(-1, -1);
}

void Controls::leftUp()
{
	button &= ~LEFT_BUTTON;
	dragEndPosition = currentPosition;
}

void Controls::middleDown()
{
	button |= LEFT_BUTTON;
	dragStartPosition = currentPosition;
	dragEndPosition = Point(-1, -1);
}

void Controls::middleUp()
{
	button &= ~LEFT_BUTTON;
	dragEndPosition = currentPosition;
}

Point Controls::getDragStartPosition()
{
	return (dragStartPosition);
}

int Controls::getPressConditionOf(Rect rect)
{
	int flags = NOT_PRESSED;

	if(rect.Inside(currentPosition))
		flags |= POINTER_OVER_BUTTON;
	
	if (button & LEFT_BUTTON)
	{
		if(rect.Inside(dragStartPosition) && (dragEndPosition == Point(-1, -1)))
			flags |= PRESSING_LEFT_BUTTON;
	} else 
	if ((rect.Inside(dragStartPosition)) && (rect.Inside(dragEndPosition)))
	{						// pressed, waiting for acknowledgment
//		mouseEventWasProcessed();
		flags |= LEFT_BUTTON_WAS_PRESSED_AND_RELEASED;
	}
//	}

	if (button & MIDDLE_BUTTON)
	{
		if(rect.Inside(dragStartPosition) && (dragEndPosition == Point(-1, -1)))
			flags |= PRESSING_MIDDLE_BUTTON;
		if ((rect.Inside(dragStartPosition)) && (rect.Inside(dragEndPosition)))
		{						// pressed, waiting for acknowledgment
			mouseEventWasProcessed();
			flags |= MIDDLE_BUTTON_WAS_PRESSED_AND_RELEASED;
		}
	}
	
	if (button & RIGHT_BUTTON)
	{
		if(rect.Inside(dragStartPosition) && (dragEndPosition == Point(-1, -1)))
			flags |= PRESSING_RIGHT_BUTTON;
		if ((rect.Inside(dragStartPosition)) && (rect.Inside(dragEndPosition)))
		{						// pressed, waiting for acknowledgment
			mouseEventWasProcessed();
			flags |= RIGHT_BUTTON_WAS_PRESSED_AND_RELEASED;
		}
	}
	return (flags);
}

void Controls::mouseEventWasProcessed()
{
	dragStartPosition = Point(-1, -1);
	dragEndPosition = Point(-1, -1);
}

Controls controls;
