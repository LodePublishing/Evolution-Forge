#include "bgentry.hpp"

BoGraphEntry::BoGraphEntry(const BoGraphEntry& object) :
	UI_Button((UI_Button)object),
	program(object.program),
	number(object.number)
{ }

BoGraphEntry& BoGraphEntry::operator=(const BoGraphEntry& object)
{
	((UI_Button)*this) = (UI_Button)object;
	program = object.program;
	number = object.number;
	return(*this);
}

BoGraphEntry::BoGraphEntry(UI_Object* entry_parent, const Rect bg_rect, const Size distance_bottom_right, const PROGRAM& bg_program, const unsigned int my_number) : // TODO
	UI_Button(entry_parent, bg_rect, distance_bottom_right, (eButtonColorsType)(UNIT_TYPE_0_BUTTON), false, BOGRAPH_BUTTON_MODE, NULL_STRING, DO_NOT_ADJUST, SMALL_BOLD_FONT, NOTHING),
	program(bg_program),
	number(my_number)
{ }

BoGraphEntry::~BoGraphEntry()
{ }

void BoGraphEntry::process()
{
	UI_Button::process();
	if(checkForNeedRedraw())
		getParent()->setNeedRedrawMoved();
}

void BoGraphEntry::draw(DC* dc) const
{
	UI_Button::draw(dc);
}


