#include "checkbutton.hpp"

UI_CheckButton& UI_CheckButton::operator=(const UI_CheckButton& object)
{
	((UI_Object)(*this)) = ((UI_Object)object);
	checked = object.checked;
	delete checkButton;
	checkButton = new UI_Button(*(object.checkButton));
	delete text;
	text = (object.text?new UI_StaticText(*(object.text)):NULL);
	return(*this);
}

UI_CheckButton::UI_CheckButton(const UI_CheckButton& object) :
	UI_Object((UI_Object)object),
	checked(object.checked),
	checkButton(new UI_Button(*(object.checkButton))),
	text(object.text?new UI_StaticText(*(object.text)) : NULL)
{ }

// TODO arrange ermoeglichen!
UI_CheckButton::UI_CheckButton(UI_Object* checkbutton_parent, const Point top_left, const Size distance_bottom_right, const ePositionMode position_mode, const eString txt, const eString tooltip_string, const bool is_checked) :
	UI_Object(checkbutton_parent, Rect(top_left, Size(0,0))), // TODO
	checked(false),
	checkButton(new UI_Button(this, Point(120, 3), distance_bottom_right, CHECK_BUTTON, STATIC_BUTTON_MODE, position_mode)),
	text(new UI_StaticText(this, txt, Rect(Point(0,0), Size(110, 0)), FORCE_TEXT_COLOR, SMALL_ITALICS_BOLD_FONT, RIGHT_BOUNDED_TEXT_MODE))
{
	this->updateToolTip(tooltip_string); // TODO
	check(is_checked);
}

UI_CheckButton::~UI_CheckButton()
{
	delete checkButton;
	delete text;
}

UI_Object* UI_CheckButton::checkTooltip() {
	if( (!isShown()) || (isDisabled()) || ((!checkButton->getAbsoluteRect().Inside(mouse)) && (!text->getTextBox().Inside(mouse))) )
		return(NULL);
	return((UI_Object*)this);
}
				
void UI_CheckButton::check(const bool is_checked)
{
	if(checkButton->isCurrentlyActivated() != is_checked)
	{
		checked = is_checked;
		if(!is_checked)
			checkButton->forceUnpress();
		else checkButton->forcePress();
	}
}

void UI_CheckButton::process()
{
// TODO Text aufleuchten lassen, wenn Maus ueber Text/Button
	if(!isShown())
		return;
	UI_Object::process();
	if(checkButton->checkForNeedRedraw())
		setNeedRedrawMoved();
}

void UI_CheckButton::draw(DC* dc) const
{
	UI_Object::draw(dc);
}

