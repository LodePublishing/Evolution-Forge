#include "numberfield.hpp"
#include <sstream>

NumberField& NumberField::operator=(const NumberField& object)
{
    ((UI_Object)(*this)) = ((UI_Object)object);
    percent = object.percent;
	delete addbutton;
    addbutton = new UI_Button(*(object.addbutton));
	delete subbutton;
	subbutton = new UI_Button(*(object.subbutton));
	delete text;
	text = new UI_StaticText(*(object.text));
	delete number;
	number = new UI_StaticText(*(object.number));

	return(*this);
}


NumberField::NumberField(const NumberField& object) :
    UI_Object((UI_Object)object),
    percent(object.percent),
    addbutton(new UI_Button(*(object.addbutton))),
    subbutton(new UI_Button(*(object.subbutton))),
    text(new UI_StaticText(*(object.text))),
    number(new UI_StaticText(*(object.number)))
{ }

NumberField::NumberField(UI_Object* parent, const Rect rect, const eString txt, const eString tooltip, const int number, const bool percent):
	UI_Object(parent, rect, rect),
	percent(percent),
    addbutton(new UI_Button(this, Rect(Point(getWidth()-20, 2),Size(8,8)), Rect(Point(0,0), getSize()), ADD_BUTTON, PRESS_BUTTON_MODE)),
    subbutton(new UI_Button(this, Rect(Point(getWidth()-10, 2),Size(8,8)), Rect(Point(0,0), getSize()), SUB_BUTTON, PRESS_BUTTON_MODE)),
    text(new UI_StaticText(this, txt, Rect(Point(0,0), getSize()), NO_TEXT_MODE, FORCE_TEXT_COLOR, SMALL_ITALICS_BOLD_FONT)),
    number(new UI_StaticText(this, Rect(Point(getWidth()-50,0), Size(20,20)), NO_TEXT_MODE, FORCE_TEXT_COLOR, SMALL_ITALICS_BOLD_FONT))
{
	text->updateToolTip(*theme.lookUpString(tooltip)); // TODO
	updateNumber(number);
}

NumberField::~NumberField()
{
	delete addbutton;
	delete subbutton;
	delete text;
	delete number;
}
const bool NumberField::addClicked() const {
	return(addbutton->isLeftClicked());
}
const bool NumberField::subClicked() const {
	return(subbutton->isLeftClicked());
}

void NumberField::updateNumber(const int number)
{
	std::ostringstream os;
	os << number;
	if(percent)
		os << "%";
	this->number->updateText(os.str());
}

void NumberField::process()
{
	if(!isShown())
		return;
	UI_Object::process();
}

void NumberField::draw(DC* dc) const
{
	UI_Object::draw(dc);
}

