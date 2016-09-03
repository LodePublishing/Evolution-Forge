#include "numberfield.hpp"
#include <sstream>

UI_NumberField& UI_NumberField::operator=(const UI_NumberField& object)
{
	((UI_Object)(*this)) = ((UI_Object)object);
   	fieldType = object.fieldType;
	delete addbutton;
	addbutton = new UI_Button(*(object.addbutton));
	delete subbutton;
	subbutton = new UI_Button(*(object.subbutton));
	delete text;
	text = (object.text?new UI_StaticText(*(object.text)):NULL);
	delete number_text;
	number_text = new UI_StaticText(*(object.number_text));
	number = object.number;
	min = object.min;
	max = object.max;
	steps = object.steps;
	return(*this);
}


UI_NumberField::UI_NumberField(const UI_NumberField& object) :
	UI_Object((UI_Object)object),
	fieldType(object.fieldType),
	addbutton(new UI_Button(*(object.addbutton))),
	subbutton(new UI_Button(*(object.subbutton))),
	text(object.text?new UI_StaticText(*(object.text)):NULL),
	number_text(new UI_StaticText(*(object.number_text))),
	number(object.number),
	min(object.min),
	max(object.max),
	steps(object.steps)
{ }
// TODO: ui_objects arrangen! es nur in button zu benutzen bringt wenig...
UI_NumberField::UI_NumberField(UI_Object* numberfield_parent, const Rect rect, const unsigned int number_min, const unsigned int number_max, const unsigned int number_steps, const unsigned int num, const eString txt, const eString tool_tip, const eFieldType field_type) :
	UI_Object(numberfield_parent, rect),
	fieldType(field_type),
	addbutton(new UI_Button(this, Rect(Point(150, 3),Size(8,8)), ADD_BUTTON, PRESS_BUTTON_MODE)),
	subbutton(new UI_Button(this, Rect(Point(160, 3),Size(8,8)), SUB_BUTTON, PRESS_BUTTON_MODE)),
	text(txt==NULL_STRING?NULL:new UI_StaticText(this, txt, Rect(Point(0,0), Size(110,0)), FORCE_TEXT_COLOR, SMALL_ITALICS_BOLD_FONT, RIGHT_BOUNDED_TEXT_MODE)),
	number_text(new UI_StaticText(this, Rect(Point(120, 1), Size(20,20)), FORCE_TEXT_COLOR, SMALL_ITALICS_BOLD_FONT)),
	number(num),
	min(number_min),
	max(number_max),
	steps(number_steps)
{
	if(tool_tip!=NULL_STRING)
		this->updateToolTip(tool_tip);
	updateNumber(num);
}

UI_NumberField::~UI_NumberField()
{
	delete addbutton;
	delete subbutton;
	delete text;
	delete number_text;
}

UI_Object* UI_NumberField::checkTooltip() {
	if( (!isShown()) || (isDisabled()) || ((!addbutton->getAbsoluteRect().Inside(mouse)) && (!subbutton->getAbsoluteRect().Inside(mouse)) && ((!text)||(!text->getTextBox().Inside(mouse))) && (!number_text->getTextBox().Inside(mouse))))
		return(NULL);
	return((UI_Object*)this);
}


void UI_NumberField::updateNumber(const unsigned int num)
{
	number = num;
	std::ostringstream os;
	switch(fieldType)
	{
		case NORMAL_NUMBER_TYPE:os << number;break;
		case PERCENT_NUMBER_TYPE:os << number << "%";break;
		case TIME_NUMBER_TYPE:
			os << formatTime(number);
			break;
		default:break;
	}
	number_text->updateText(os.str());
	setNeedRedrawMoved();
}

void UI_NumberField::process()
{
	if(!isShown())
		return;
	UI_Object::process();
	if(subClicked())
	{
		if(number > min+(signed int)steps)
			number-=steps;
		else number = min;
		updateNumber(number);
	}
	if(addClicked())
	{
		if(number < max-(signed int)steps)
			number += steps;
		else number = max;
		updateNumber(number);
	}
	if(subRightClicked())
	{
		if(number > min + 10*(signed int)steps)
			number -= 10*steps;
		else number = min;
		updateNumber(number);
	}
	if(addRightClicked())
	{
		if(number < max - 10*(signed int)steps)
			number += 10*steps;
		else number = max;
		updateNumber(number);
	}
}

void UI_NumberField::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Object::draw(dc);
	dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
	dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	Rect edge;
	edge.SetTopLeft(getAbsolutePosition()+Size(115,1));
	edge.SetWidth(58);
	edge.SetHeight(12);
	dc->DrawRoundedRectangle(edge,2);
}

