#include "input.h"

Controls::Controls()
{
	current=wxPoint(-1,-1);
	captured();
};

Controls::~Controls()
{
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

void Controls::leftDown()
{
	start=current;
	end=wxPoint(-1,-1);
};

void Controls::leftUp()
{
	end=current;
};

int Controls::getPressCondition(wxRect rect)
{
	int t=0;
	if((rect.Inside(start))&&(rect.Inside(current))&&(end==wxPoint(-1,-1)))
		return(2); // -> currently pressing button
	if((rect.Inside(start))&&(end==wxPoint(-1,-1)))
		return(3); // -> currently pressing but not in the button -> moving!
	if((rect.Inside(start))&&(rect.Inside(end)))
	{ // pressed, waiting for acknowledgment
		captured();
		return(4);
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

