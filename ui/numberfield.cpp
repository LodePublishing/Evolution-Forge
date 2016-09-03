#include "numberfield.hpp"
#include "../ef/configuration.hpp" // TODO GameSpeed in UI
#include <sstream>

// TODO: ui_objects arrangen! es nur in button zu benutzen bringt wenig...
// eString
UI_NumberField::UI_NumberField(UI_Object* numberfield_parent, const Rect& rect, const Size distance_bottom_right, const ePositionMode position_mode, const unsigned int number_min, const unsigned int number_max, const eString txt, const eString tool_tip, const unsigned int number_steps, const unsigned int num, const bool shift_right, const eFieldType field_type) :
	UI_Object(numberfield_parent, rect, distance_bottom_right, position_mode, AUTO_HEIGHT_CONST_WIDTH),
	fieldType(field_type),
	addButton(new UI_Button(this, Rect(Point(UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH) + 40, 1), Size(8, 8)), Size(0, 0), MODIFY_BUTTON, INCREASE_BITMAP, PRESS_BUTTON_MODE, NULL_STRING, DO_NOT_ADJUST)),
	subButton(new UI_Button(this, Rect(Point(UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH) + 50, 1), Size(8, 8)), Size(0, 0), MODIFY_BUTTON, SUB_BITMAP, PRESS_BUTTON_MODE, NULL_STRING, DO_NOT_ADJUST)),
	text(txt == NULL_STRING ? NULL : new UI_StaticText(this, txt, Rect(Point(shift_right?10:0, 0), Size(0, 0)), Size(0, 0), FORCE_TEXT_COLOR, SMALL_BOLD_FONT, DO_NOT_ADJUST)),
	numberText(new UI_StaticText(this, "0", Rect(Point(UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH) + 10, 0), Size(0, 0)), Size(0, 0), FORCE_TEXT_COLOR, SMALL_BOLD_FONT, DO_NOT_ADJUST)),
	number(num),
	min(number_min),
	max(number_max),
	steps(number_steps),
	numberHasChanged(false), //?
	shiftRight(shift_right)
{
	if(tool_tip!=NULL_STRING)
		this->updateToolTip(tool_tip);
	updateNumber(num);
}

// std::string&
UI_NumberField::UI_NumberField(UI_Object* numberfield_parent, const Rect& rect, const Size distance_bottom_right, const ePositionMode position_mode, const unsigned int number_min, const unsigned int number_max, const std::string& txt, const eString tool_tip, const unsigned int number_steps, const unsigned int num, const bool shift_right, const eFieldType field_type) :
	UI_Object(numberfield_parent, rect, distance_bottom_right, position_mode, AUTO_HEIGHT_CONST_WIDTH),
	fieldType(field_type),
	addButton(new UI_Button(this, Rect(Point(UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH) + 40, 1), Size(8, 8)), Size(0, 0), MODIFY_BUTTON, INCREASE_BITMAP, PRESS_BUTTON_MODE, NULL_STRING, DO_NOT_ADJUST)),
	subButton(new UI_Button(this, Rect(Point(UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH) + 50, 1), Size(8, 8)), Size(0, 0), MODIFY_BUTTON, SUB_BITMAP, PRESS_BUTTON_MODE, NULL_STRING, DO_NOT_ADJUST)),
	text(new UI_StaticText(this, txt, Rect(Point(shift_right?10:0, 0), Size(0, 0)), Size(0, 0), FORCE_TEXT_COLOR, SMALL_BOLD_FONT, DO_NOT_ADJUST)),
	numberText(new UI_StaticText(this, "0", Rect(Point(UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH) + 10, 0), Size(0, 0)), Size(0, 0), FORCE_TEXT_COLOR, SMALL_BOLD_FONT, DO_NOT_ADJUST)),
	number(num),
	min(number_min),
	max(number_max),
	steps(number_steps),
	numberHasChanged(false),
	shiftRight(shift_right)
{
	if(tool_tip!=NULL_STRING)
		this->updateToolTip(tool_tip);
	updateNumber(num);
	reloadOriginalSize();
}

UI_NumberField::~UI_NumberField()
{
	delete addButton;
	delete subButton;
	delete text;
	delete numberText;
}

UI_Object* UI_NumberField::checkToolTip() 
{
	if( (!isShown()) || ((!addButton->getAbsoluteRect().isInside(mouse)) && (!subButton->getAbsoluteRect().isInside(mouse)) && ((!text)||(!text->getAbsoluteRect().isInside(mouse))) && (!numberText->getAbsoluteRect().isInside(mouse))))
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
	setOriginalSize(Size(UI_Object::theme.lookUpButtonWidth(LARGE_BUTTON_WIDTH),text->getTextSize().getHeight()));
		
	addButton->setOriginalPosition(Point(UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH) + 40, 1));
	subButton->setOriginalPosition(Point(UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH) + 50, 1));
	numberText->setOriginalPosition(Point(addButton->getRelativeLeftBound() - numberText->getTextSize().getWidth() - 5, 0));
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
			case TIME_NUMBER_TYPE:os << formatTime(number, efConfiguration.getGameSpeed());break;
			case STRING_NUMBER_TYPE: os << theme.lookUpString((eString)(number));
			default:break;
		}
		numberText->updateText(os.str());
		numberText->setOriginalPosition(Point(addButton->getRelativeLeftBound() - numberText->getTextSize().getWidth() - 5, 0));
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
		if((number < max - 10 * (signed int)steps) && (max > 10 * steps))
			number += 10*steps;
		else number = max;
		updateNumber(number);
	}
	if(text)
		text->doHighlight(isMouseInside());
	numberText->doHighlight(isMouseInside());

//	if(/*(numberText->checkForNeedRedraw())||((text)&&(text->checkForNeedRedraw())))||*/(addButton->checkForNeedRedraw())||(subButton->checkForNeedRedraw()))
//	if(checkForNeedRedraw())
//		setNeedRedrawMoved(); TODO
}

void UI_NumberField::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Object::draw(dc);
	
	if(shiftRight)
		dc->DrawBitmap(theme.lookUpBitmap(LIST_BITMAP), getAbsolutePosition());
	
/*	dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
	dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	Rect edge;
	edge.SetTopLeft(getAbsolutePosition()+Size(115,0));
	edge.SetWidth(58);
	edge.SetHeight(12);
	dc->DrawRoundedRectangle(edge, 2);*/
}

