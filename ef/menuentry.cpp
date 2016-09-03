#include "menuentry.hpp"

MenuEntry::MenuEntry(UI_Object* menuentry_parent, const Rect rect, const eString unit) :
	UI_Button(menuentry_parent, rect, Size(4, 2), unit, ADD_GOAL_BUTTON, PRESS_BUTTON_MODE, DO_NOT_ADJUST, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH),
	color(NULL_COLOR)
{ }

MenuEntry::MenuEntry(UI_Object* menuentry_parent, const Rect rect, const std::string& unit) :
	UI_Button(menuentry_parent, rect, Size(4, 2), unit, ADD_GOAL_BUTTON, PRESS_BUTTON_MODE, DO_NOT_ADJUST, SMALL_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH),
	color(NULL_COLOR)
{ }

MenuEntry& MenuEntry::operator=(const MenuEntry& object)
{
	((UI_Button)(*this)) = ((UI_Button)object);
	color = object.color;
	return(*this);
}

MenuEntry::MenuEntry(const MenuEntry& object) :
	UI_Button((UI_Button)object),
	color(object.color)
{ }

MenuEntry::~MenuEntry()
{ }

void MenuEntry::process()
{
	if(!isShown()) 
		return;
	UI_Button::process();
}

void MenuEntry::draw(DC* dc) const
{
	if(!isShown()) 
		return;
	UI_Button::draw(dc);
}

void MenuEntry::setColor(const eColor entry_color)
{
	color = entry_color;
}

