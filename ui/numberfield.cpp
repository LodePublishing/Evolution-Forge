#include "numberfield.hpp"
#include <sstream>

UI_NumberField& UI_NumberField::operator=(const UI_NumberField& object)
{
	((UI_Object)(*this)) = ((UI_Object)object);
   	fieldType = object.fieldType;
	delete addbutton;
	addbutton = new UI_Button(*(object.addbutton)); // Problem PARENT!
	delete subbutton;
	subbutton = new UI_Button(*(object.subbutton));
	delete text;
	text = (object.text?new UI_StaticText(*(object.text)):NULL);
	delete numberText;
	numberText = new UI_StaticText(*(object.numberText));
	number = object.number;
	min = object.min;
	max = object.max;
	steps = object.steps;
	numberHasChanged = object.numberHasChanged;
	return(*this);
}


UI_NumberField::UI_NumberField(const UI_NumberField& object) :
	UI_Object((UI_Object)object),
	fieldType(object.fieldType),
	addbutton(new UI_Button(*(object.addbutton))),
	subbutton(new UI_Button(*(object.subbutton))),
	text(object.text?new UI_StaticText(*(object.text)):NULL),
	numberText(new UI_StaticText(*(object.numberText))),
	number(object.number),
	min(object.min),
	max(object.max),
	steps(object.steps),
	numberHasChanged(object.numberHasChanged)
{ }
// TODO: ui_objects arrangen! es nur in button zu benutzen bringt wenig...
UI_NumberField::UI_NumberField(UI_Object* numberfield_parent, const Rect& rect, const Size distance_bottom_right, const ePositionMode position_mode, const unsigned int number_min, const unsigned int number_max, const eString txt, const eString tool_tip, const unsigned int number_steps, const unsigned int num, const eFieldType field_type) :
	UI_Object(numberfield_parent, rect, distance_bottom_right, position_mode, AUTO_HEIGHT_CONST_WIDTH),
	fieldType(field_type),
	addbutton(new UI_Button(this, Rect(Point(UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH) + 40, 1), Size(8, 8)), Size(0, 0), INCREASE_BUTTON, true, PRESS_BUTTON_MODE, NULL_STRING, DO_NOT_ADJUST)),
	subbutton(new UI_Button(this, Rect(Point(UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH) + 50, 1), Size(8, 8)), Size(0, 0), SUB_BUTTON, true, PRESS_BUTTON_MODE, NULL_STRING, DO_NOT_ADJUST)),
	text(txt == NULL_STRING ? NULL : new UI_StaticText(this, txt, Rect(Point(0, 0), Size(0, 0)), Size(0, 0), FORCE_TEXT_COLOR, SMALL_BOLD_FONT, DO_NOT_ADJUST)),
	numberText(new UI_StaticText(this, "0", Rect(Point(UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0), Size(0, 0)), Size(0, 0), FORCE_TEXT_COLOR, SMALL_BOLD_FONT, DO_NOT_ADJUST)),
	number(num),
	min(number_min),
	max(number_max),
	steps(number_steps),
	numberHasChanged(true)
{
	if(tool_tip!=NULL_STRING)
		this->updateToolTip(tool_tip);
	updateNumber(num);
}

UI_NumberField::UI_NumberField(UI_Object* numberfield_parent, const Rect& rect, const Size distance_bottom_right, const ePositionMode position_mode, const unsigned int number_min, const unsigned int number_max, const std::string& txt, const eString tool_tip, const unsigned int number_steps, const unsigned int num, const eFieldType field_type) :
	UI_Object(numberfield_parent, rect, distance_bottom_right, position_mode, AUTO_HEIGHT_CONST_WIDTH),
	fieldType(field_type),
	addbutton(new UI_Button(this, Rect(Point(UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH) + 40, 1), Size(8, 8)), Size(0, 0), INCREASE_BUTTON, true, PRESS_BUTTON_MODE, NULL_STRING, DO_NOT_ADJUST)),
	subbutton(new UI_Button(this, Rect(Point(UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH) + 50, 1), Size(8, 8)), Size(0, 0), SUB_BUTTON, true, PRESS_BUTTON_MODE, NULL_STRING, DO_NOT_ADJUST)),
	text(new UI_StaticText(this, txt, Rect(Point(0, 0), Size(0, 0)), Size(0, 0), FORCE_TEXT_COLOR, SMALL_BOLD_FONT, DO_NOT_ADJUST)),
	numberText(new UI_StaticText(this, "0", Rect(Point(UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0), Size(0, 0)), Size(0, 0), FORCE_TEXT_COLOR, SMALL_BOLD_FONT, DO_NOT_ADJUST)),
	number(num),
	min(number_min),
	max(number_max),
	steps(number_steps),
	numberHasChanged(true)
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
	delete numberText;
}

UI_Object* UI_NumberField::checkToolTip() 
{
	if( (!isShown()) || ((!addbutton->getAbsoluteRect().Inside(mouse)) && (!subbutton->getAbsoluteRect().Inside(mouse)) && ((!text)||(!text->getAbsoluteRect().Inside(mouse))) && (!numberText->getAbsoluteRect().Inside(mouse))))
		return(NULL);
	return((UI_Object*)this);
}

void UI_NumberField::updateText(const std::string& txt) {
	text->updateText(txt);
}

void UI_NumberField::updateText(const eString txt) {
	text->updateText(txt);
}

void UI_NumberField::reloadOriginalSize()
{
	setOriginalSize(Size(3*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH)/2,0));
		
	addbutton->setOriginalPosition(Point(UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH) + 40, 1));
	subbutton->setOriginalPosition(Point(UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH) + 50, 1));
	numberText->setOriginalPosition(Point(UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 1));
	UI_Object::reloadOriginalSize();
}


void UI_NumberField::updateNumber(const unsigned int num)
{
	if(number!=num)
		return;
	number = num;
	numberHasChanged = true;
}

void UI_NumberField::process()
{
	if(!isShown())
		return;
	UI_Object::process();

	if(numberHasChanged)
	{
		numberHasChanged=false;
		std::ostringstream os;
		os.str("");
		switch(fieldType)
		{
			case NORMAL_NUMBER_TYPE:os << number; break;
			case PERCENT_NUMBER_TYPE:os << number << "%"; break;
			case TIME_NUMBER_TYPE:
				os << formatTime(number);
				break;
			default:break;
		}
		numberText->updateText(os.str());
	//	setNeedRedrawMoved();
	//	adjustPositionAndSize(ADJUST_AFTER_CHILD_SIZE_WAS_CHANGED, numberText->getTextSize()); // TODO
	}
	
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
		if((number < max - 10*(signed int)steps)&&(max > 10*(signed int)steps))
			number += 10*steps;
		else number = max;
		updateNumber(number);
	}
	text->doHighlight(isMouseInside());
	numberText->doHighlight(isMouseInside());

	if((numberText->checkForNeedRedraw())||(text->checkForNeedRedraw())||(addbutton->checkForNeedRedraw())||(subbutton->checkForNeedRedraw()))
		setNeedRedrawMoved();
}

void UI_NumberField::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Object::draw(dc);
/*	dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
	dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	Rect edge;
	edge.SetTopLeft(getAbsolutePosition()+Size(115,0));
	edge.SetWidth(58);
	edge.SetHeight(12);
	dc->DrawRoundedRectangle(edge, 2);*/
}

