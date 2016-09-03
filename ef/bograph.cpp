#include "bograph.hpp"

BoGraphEntry::BoGraphEntry(const BoGraphEntry& object) :
	UI_Button((UI_Button)object),
	program(object.program)
{ }

BoGraphEntry& BoGraphEntry::operator=(const BoGraphEntry& object)
{
	((UI_Button)*this) = (UI_Button)object;
	program = object.program;
	return(*this);
}

BoGraphEntry::BoGraphEntry(UI_Object* entry_parent, const Rect entry_rect, const PROGRAM& bg_program) : // TODO
	UI_Button(entry_parent, entry_rect, "", (eButton)(UNIT_TYPE_0_BUTTON), NO_TEXT_MODE, PRESS_BUTTON_MODE, DO_NOT_ADJUST, SMALL_NORMAL_BOLD_FONT, NOTHING),
	program(bg_program)
{ }

BoGraphEntry::~BoGraphEntry()
{ }

void BoGraphEntry::process()
{
	UI_Button::process();
}

void BoGraphEntry::draw(DC* dc) const
{
	UI_Button::draw(dc);
}


