#include "menuentry.hpp"

MenuEntry::MenuEntry(UI_Object* parent, Rect rect, Rect maxRect, string unit):UI_Button(parent, rect, maxRect, unit, unit, ADD_GOAL_BUTTON, NO_TEXT_MODE, PRESS_BUTTON_MODE, DO_NOT_ADJUST, SMALL_NORMAL_BOLD_FONT, AUTO_HEIGHT_FULL_WIDTH)
	// TODO!
{
//	type=REMAINING_UNIT_TYPE;
}

MenuEntry::~MenuEntry()
{
}

void MenuEntry::process()
{
	if(!shown) return;
	UI_Button::process();
}

void MenuEntry::draw(DC* dc) const
{
	if(!shown) return;
	UI_Button::draw(dc);
}

void MenuEntry::updateText(string utext)
{
	updateNormalText(utext);
	updatePressedText(utext);
}

void MenuEntry::setColor(const eColor color)
{
	this->color=color;
}

