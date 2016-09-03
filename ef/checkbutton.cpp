#include "checkbutton.hpp"

CheckButton& CheckButton::operator=(const CheckButton& object)
{
    ((UI_Object)(*this)) = ((UI_Object)object);
	delete checkButton;
    checkButton = new UI_Button(*(object.checkButton));
	delete text;
	text = (object.text?new UI_StaticText(*(object.text)):NULL);
	return(*this);
}


CheckButton::CheckButton(const CheckButton& object) :
    UI_Object((UI_Object)object),
    checkButton(new UI_Button(*(object.checkButton))),
    text(object.text?new UI_StaticText(*(object.text)):NULL)
	
{ }

CheckButton::CheckButton(UI_Object* parent, const Rect rect, const eString txt, const eString tooltip, const bool checked) :
	UI_Object(parent, rect, rect),
    checkButton(new UI_Button(this, Rect(Point(120, 3),Size(10,10)), Rect(Point(0,0), getSize()), CHECK_BUTTON, STATIC_BUTTON_MODE)),
    text(new UI_StaticText(this, txt, Rect(Point(0,0), Size(110, 0)), RIGHT_BOUNDED_TEXT_MODE, FORCE_TEXT_COLOR, SMALL_ITALICS_BOLD_FONT))
{
	text->updateToolTip(tooltip); // TODO
}

CheckButton::~CheckButton()
{
	delete checkButton;
	delete text;
}

const bool CheckButton::isClicked() const {
	return(checkButton->isLeftClicked());
}

const bool CheckButton::isChecked() const {
	return(checkButton->isCurrentlyActivated());
}

void CheckButton::check(const bool checked)
{
	if(checkButton->isCurrentlyActivated() != checked)
	{
//		this->checked = checked;
		if(!checked)
			checkButton->forceUnpress();
		else checkButton->forcePress();
	}
}


void CheckButton::process()
{
	if(!isShown())
		return;
	UI_Object::process();
}

void CheckButton::draw(DC* dc) const
{
	UI_Object::draw(dc);
}

