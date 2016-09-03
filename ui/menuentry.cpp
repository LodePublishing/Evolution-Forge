#include "menuentry.hpp"

UI_MenuEntry::UI_MenuEntry(UI_Object* menuentry_parent, const Rect rect, const eString unit) :
	UI_Button(menuentry_parent, rect, Size(4, 2), ADD_GOAL_BUTTON, false, PRESS_BUTTON_MODE, unit, DO_NOT_ADJUST, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH),
	color(NULL_COLOR)
{ }

UI_MenuEntry::UI_MenuEntry(UI_Object* menuentry_parent, const Rect rect, const std::string& unit) :
	UI_Button(menuentry_parent, rect, Size(4, 2), ADD_GOAL_BUTTON, false, PRESS_BUTTON_MODE, unit, DO_NOT_ADJUST, SMALL_SHADOW_BOLD_FONT, AUTO_HEIGHT_CONST_WIDTH),
	color(NULL_COLOR)
{}

UI_MenuEntry::~UI_MenuEntry()
{ }

void UI_MenuEntry::process()
{
	if(!isShown()) 
		return;
	UI_Button::process();
}

void UI_MenuEntry::draw(DC* dc) const
{
	if(!isShown()) 
		return;
	UI_Button::draw(dc);
}

void UI_MenuEntry::setColor(const eColor entry_color)
{
	color = entry_color;
}

