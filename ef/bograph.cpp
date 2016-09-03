#include "bograph.hpp"

BoGraphEntry::BoGraphEntry(const BoGraphEntry& object) :
	UI_Button((UI_Button)object),
	unit(object.unit)
{ }

BoGraphEntry& BoGraphEntry::operator=(const BoGraphEntry& object)
{
	((UI_Button)*this) = (UI_Button)object;
	unit = object.unit;
	return(*this);
}

BoGraphEntry::BoGraphEntry(UI_Object* entry_parent, const Rect entry_rect, const Rect entry_max_rect, const int entry_unit) : // TODO
	UI_Button(entry_parent, entry_rect, entry_max_rect, "", (eButton)(UNIT_TYPE_0_BUTTON), NO_TEXT_MODE, PRESS_BUTTON_MODE, DO_NOT_ADJUST, SMALL_NORMAL_BOLD_FONT, NOTHING),
	unit(entry_unit)
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

const unsigned int BoGraphEntry::getUnit() const
{
	return(unit);
}

void BoGraphEntry::setUnit(const unsigned int bg_unit)
{
	unit = bg_unit;
}

const unsigned int BoGraphEntry::getIP() const
{
	return(ip);
}

void BoGraphEntry::setIP(const unsigned int bg_ip)
{
	ip = bg_ip;
}

