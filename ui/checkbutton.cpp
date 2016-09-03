#include "checkbutton.hpp"

UI_CheckButton& UI_CheckButton::operator=(const UI_CheckButton& object)
{
	((UI_Object)(*this)) = ((UI_Object)object);
	checked = object.checked;
	delete checkButton;
	checkButton = new UI_Button(*(object.checkButton));
	delete text;
//	text = (object.text?new UI_StaticText(*(object.text)):NULL);
	text = (object.text?new UI_Button(*(object.text)):NULL);
	return(*this);
}

UI_CheckButton::UI_CheckButton(const UI_CheckButton& object) :
	UI_Object((UI_Object)object),
	checked(object.checked),
	checkButton(new UI_Button(*(object.checkButton))),
//	text(object.text?new UI_StaticText(*(object.text)) : NULL)
	text(object.text?new UI_Button(*(object.text)) : NULL)
{ }

// TODO arrange ermoeglichen!
UI_CheckButton::UI_CheckButton(UI_Object* checkbutton_parent, const Rect checkbutton_rect, const Size distance_bottom_right, const ePositionMode position_mode, const eString txt, const eString tooltip_string, const bool is_checked) :
	UI_Object(checkbutton_parent, checkbutton_rect, distance_bottom_right, position_mode, AUTO_HEIGHT_CONST_WIDTH), // TODO
	checked(false),
	checkButton(new UI_Button(this, Rect(0, 0, 120, 10), Size(0, 0), CHECK_BUTTON, true, STATIC_BUTTON_MODE, NULL_STRING)), // TODO
//	text(new UI_StaticText(this, txt, Rect(Point(20, 1), Size(110, 0)), Size(0,0), FORCE_TEXT_COLOR, SMALL_BOLD_FONT, TOP_LEFT))
	text(new UI_Button(this, Rect(Point(20, 1), Size(110, 0)), Size(0,0), CHECK_BUTTON, true, STATIC_BUTTON_MODE, 
				todo
				FORCE_TEXT_COLOR, SMALL_BOLD_FONT, TOP_LEFT))
{
	this->updateToolTip(tooltip_string); // TODO
	check(is_checked);
}
// TODO evtl statt UI_Object ein UI_Button draus machen und text als Anhaengsel benutzen...
UI_CheckButton::~UI_CheckButton()
{
	delete checkButton;
	delete text;
}

UI_Object* UI_CheckButton::checkToolTip() {
	if( (!isShown()) || ((!checkButton->getAbsoluteRect().Inside(mouse)) && (!text->getAbsoluteRect().Inside(mouse))) )
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
		setNeedRedrawMoved();
	}
}

void UI_CheckButton::process()
{
	if(!isShown())
		return;
//	adjustPositionAndSize(ADJUST_AFTER_CHILD_SIZE_WAS_CHANGED, text->getTextSize());
	UI_Object::process();
	text->doHighlight(isMouseInside());

	if(checkButton->checkForNeedRedraw())
		setNeedRedrawMoved();
}

void UI_CheckButton::draw(DC* dc) const
{
	UI_Object::draw(dc);
}
