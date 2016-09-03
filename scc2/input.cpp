#include "input.h"

Controls::Controls()
{
	current=wxPoint(-1,-1);
	captured();
	shiftPressed=0;
	left=false;
};

Controls::~Controls()
{
};

void Controls::setShiftPressed(int p)
{
	shiftPressed=p;
};

int Controls::isShiftPressed()
{
	return(shiftPressed);
};

wxPoint Controls::getCurrentPosition()
{
	return(current);
};

wxPoint Controls::getCurrentPosition(wxPoint pos)
{
	return(current-pos);
};


void Controls::setMouse(wxPoint position)
{
	current=position;
};

void Controls::scrollMouse(int scroll)
{
	this->scroll+=scroll;
};

int Controls::getScroll()
{
	return(scroll);
};

void Controls::rightDown()
{
	left=false;
	start=current;
	end=wxPoint(-1,-1);
};

void Controls::rightUp()
{
	left=false;
	end=current;
};

void Controls::leftDown()
{
	left=true;
	start=current;
	end=wxPoint(-1,-1);
};

void Controls::leftUp()
{
	left=true;
	end=current;
};

int Controls::getPressCondition(wxRect rect)
{
	int t=0;
	if(left)
	{
		if((rect.Inside(start))&&(rect.Inside(current))&&(end==wxPoint(-1,-1)))
			return(2); // -> currently pressing button
		if((rect.Inside(start))&&(end==wxPoint(-1,-1)))
			return(3); // -> currently pressing but not in the button -> moving!
		if((rect.Inside(start))&&(rect.Inside(end)))
		{ // pressed, waiting for acknowledgment
			captured();
			return(4);
		}
	} else
	{
                if((rect.Inside(start))&&(rect.Inside(current))&&(end==wxPoint(-1,-1)))
                        return(5); // -> currently pressing button
                if((rect.Inside(start))&&(end==wxPoint(-1,-1)))
                        return(6); // -> currently pressing but not in the button -> moving!
                if((rect.Inside(start))&&(rect.Inside(end)))
                { // pressed, waiting for acknowledgment
                        captured();
                        return(7);
                }
	}
	if(rect.Inside(current)) // -> no press at all, but mouse hovers over button
		return(1);
	return(0); //else.
}

void Controls::captured()
{
	start=wxPoint(-1,-1);
	end=wxPoint(-1,-1);
};

