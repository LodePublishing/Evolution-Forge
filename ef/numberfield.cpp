#include "numberfield.hpp"
#include <sstream>

NumberField& NumberField::operator=(const NumberField& object)
{
    ((UI_Object)(*this)) = ((UI_Object)object);
   	fieldType = object.fieldType;
	delete addbutton;
    addbutton = new UI_Button(*(object.addbutton));
	delete subbutton;
	subbutton = new UI_Button(*(object.subbutton));
	delete text;
	text = (object.text?new UI_StaticText(*(object.text)):NULL);
	delete number;
	number = new UI_StaticText(*(object.number));
	min = object.min;
	max = object.max;
	steps = object.steps;
	return(*this);
}


NumberField::NumberField(const NumberField& object) :
    UI_Object((UI_Object)object),
    fieldType(object.fieldType),
    addbutton(new UI_Button(*(object.addbutton))),
    subbutton(new UI_Button(*(object.subbutton))),
    text(object.text?new UI_StaticText(*(object.text)):NULL),
    number(new UI_StaticText(*(object.number))),
	min(object.min),
	max(object.max),
	steps(object.steps)
{ }

NumberField::NumberField(UI_Object* parent, const Rect rect, const int min, const int max, const unsigned int steps, const int number, const eString txt, const eString tooltip, const eFieldType field_type) :
	UI_Object(parent, rect, rect),
	fieldType(field_type),
    addbutton(new UI_Button(this, Rect(Point(150, 3),Size(8,8)), Rect(Point(0,0), getSize()), ADD_BUTTON, PRESS_BUTTON_MODE)),
    subbutton(new UI_Button(this, Rect(Point(160, 3),Size(8,8)), Rect(Point(0,0), getSize()), SUB_BUTTON, PRESS_BUTTON_MODE)),
    text(txt==NULL_STRING?NULL:new UI_StaticText(this, txt, Rect(Point(0,0), Size(110,0)), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR, SMALL_ITALICS_BOLD_FONT)),
    number(new UI_StaticText(this, Rect(Point(120,1), Size(20,20)), NO_TEXT_MODE, FORCE_TEXT_COLOR, SMALL_ITALICS_BOLD_FONT)),
	min(min),
	max(max),
	steps(steps)
{
	if(tooltip!=NULL_STRING)
		text->updateToolTip(tooltip); // TODO
	updateNumber(number);
}

NumberField::~NumberField()
{
	delete addbutton;
	delete subbutton;
	delete text;
	delete number;
}

const int NumberField::getNumber() const {
	return(num);
}
const bool NumberField::addClicked() const {
	return(addbutton->isLeftClicked());
}
const bool NumberField::subClicked() const {
	return(subbutton->isLeftClicked());
}
const bool NumberField::addRightClicked() const {
	return(addbutton->isRightClicked());
}
const bool NumberField::subRightClicked() const {
	return(subbutton->isRightClicked());
}
void NumberField::updateNumber(const int number)
{
	num = number;
	std::ostringstream os;
	switch(fieldType)
	{
		case NORMAL_NUMBER_TYPE:os << number;break;
		case PERCENT_NUMBER_TYPE:os << number << "%";break;
		case TIME_NUMBER_TYPE:
			if(number%60>9)
				os << number/60 << ":" << number%60;
			else
				os << number/60 << ":0" << number%60;
			break;
		default:break;
	}
	this->number->updateText(os.str());
}

void NumberField::process()
{
	if(!isShown())
		return;
	UI_Object::process();
    if(subClicked())
    {
        if(num > min+(signed int)steps)
        	num-=steps;
        else num = min;
        updateNumber(num);
    }
    if(addClicked())
    {
        if(num < max-(signed int)steps)
            num += steps;
        else num = max;
		updateNumber(num);
    }
    if(subRightClicked())
    {
        if(num > min + 10*(signed int)steps)
            num -= 10*steps;
        else num = min;
		updateNumber(num);
    }
                                                                                                                                                            
    if(addRightClicked())
    {
        if(num < max - 10*(signed int)steps)
            num += 10*steps;
		else num = max;
		updateNumber(num);
    }
}

void NumberField::draw(DC* dc) const
{
	UI_Object::draw(dc);
    dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
    dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
    Rect edge;
	edge.SetTopLeft(getAbsolutePosition()+Size(getWidth(),0) - Size(52,0));
	edge.SetWidth(54);
	edge.SetHeight(12);
//	dc->DrawRoundedRectangle(edge,2);
}

