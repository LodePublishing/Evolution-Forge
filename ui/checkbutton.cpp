#include "checkbutton.hpp"

UI_CheckButton& UI_CheckButton::operator=(const UI_CheckButton& object)
{
	((UI_Button)(*this)) = ((UI_Button)object);
	checked = object.checked;
	delete text;
//	text = (object.text?new UI_StaticText(*(object.text)):NULL);
	text = (object.text?new UI_Button(*(object.text)):NULL);
	return(*this);
}

UI_CheckButton::UI_CheckButton(const UI_CheckButton& object) :
	UI_Button((UI_Button)object),
	checked(object.checked),
//	text(object.text?new UI_StaticText(*(object.text)) : NULL)
	text(object.text?new UI_Button(*(object.text)) : NULL)
{ }

// TODO arrange ermoeglichen!
UI_CheckButton::UI_CheckButton(UI_Object* checkbutton_parent, const Rect checkbutton_rect, const Size distance_bottom_right, const ePositionMode position_mode, const eString txt, const eString tooltip_string, const bool is_checked) :
	UI_Button(checkbutton_parent, checkbutton_rect, distance_bottom_right, CHECK_BUTTON, true, STATIC_BUTTON_MODE, NULL_STRING, position_mode), 
	checked(false),
//	text(new UI_StaticText(this, txt, Rect(Point(20, 1), Size(110, 0)), Size(0,0), FORCE_TEXT_COLOR, SMALL_BOLD_FONT, TOP_LEFT))
	text(new UI_Button(this, Rect(Point(20, 1), Size(110,0)), Size(0, 0), TEXT_BUTTON, false, STATIC_BUTTON_MODE, txt, DO_NOT_ADJUST, SMALL_BOLD_FONT, AUTO_SIZE))
{
	updateToolTip(tooltip_string); // TODO
	text->updateToolTip(tooltip_string);
	check(is_checked);
}

UI_CheckButton::~UI_CheckButton()
{
	delete text;
}

UI_Object* UI_CheckButton::checkToolTip() {
	if( (!isShown()) || ((!getAbsoluteRect().Inside(mouse)) && (!text->getAbsoluteRect().Inside(mouse))) )
		return(NULL);
	return((UI_Object*)this);
}
				
void UI_CheckButton::check(const bool is_checked)
{
	if(isCurrentlyActivated() != is_checked)
	{
		checked = is_checked;
		if(!is_checked)
		{
			forceUnpress();
			text->forceUnpress();
		}
		else 
		{
			forcePress();
			text->forcePress();
		}
		setNeedRedrawMoved();
	}
}

void UI_CheckButton::process()
{
	if(!isShown())
		return;
//	adjustPositionAndSize(ADJUST_AFTER_CHILD_SIZE_WAS_CHANGED, text->getTextSize());
	UI_Button::process();

	
	if(isLeftClicked())
	{
		if(isCurrentlyActivated())
			text->forcePress();
		else text->forceUnpress();
	}
	else if(text->isLeftClicked())
	{
		if(text->isCurrentlyActivated())
			forcePress();
		else forceUnpress();
	}
	
	if((checkForNeedRedraw())||(text->checkForNeedRedraw()))
		setNeedRedrawMoved();
}

void UI_CheckButton::draw(DC* dc) const
{
	UI_Button::draw(dc);
}

