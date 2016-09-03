#include "editfield.hpp"
#include "button.hpp"
#include <sstream>

// ISO-LEVEL 2 

UI_EditField::UI_EditField(UI_Object* edit_parent, UI_Object* edit_caller, const eString edit_text, const eString description_text, const eString ok_string, const eString cancel_string, const string& name_proposal) :
	UI_Window(edit_parent, edit_text, EDIT_FIELD_WINDOW, 0),
	OK_Button(new UI_Button(this, getRelativeClientRect(), ok_string, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, PRESS_BUTTON_MODE, ARRANGE_BOTTOM_RIGHT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE)),
	Cancel_Button(new UI_Button(this, getRelativeClientRect(), cancel_string, MY_BUTTON, HORIZONTALLY_CENTERED_TEXT_MODE, PRESS_BUTTON_MODE, ARRANGE_BOTTOM_RIGHT, SMALL_NORMAL_BOLD_FONT, AUTO_SIZE)),
	editText(new UI_StaticText(this, description_text, Rect(getRelativeClientRectPosition() + Point(10, 15), getClientRectSize()), FORCE_TEXT_COLOR, SMALL_NORMAL_BOLD_FONT)),
	userText(new UI_StaticText(this, name_proposal, Rect(getRelativeClientRectPosition() + Point(17, 37), getClientRectSize()), BRIGHT_TEXT_COLOR, SMALL_NORMAL_BOLD_FONT)),
	position(name_proposal.size()),
	caller(edit_caller),
	ani(5)
{ }

UI_EditField::~UI_EditField()
{
	delete OK_Button;
	delete Cancel_Button;
	delete editText;
	delete userText;
}

UI_EditField::UI_EditField(const UI_EditField& object) :
	UI_Window((UI_Window)object),
	OK_Button(new UI_Button(*object.OK_Button)),
	Cancel_Button(new UI_Button(*object.Cancel_Button)),
	editText(new UI_StaticText(*object.editText)),
	userText(new UI_StaticText(*object.userText)),
	position(object.position),
	caller(object.caller),
	ani(object.ani)
{ }

UI_EditField& UI_EditField::operator=(const UI_EditField& object)
{
	(UI_Window)(*this) = (UI_Window)object;
	delete OK_Button;
	OK_Button = new UI_Button(*object.OK_Button);
	delete Cancel_Button;
	Cancel_Button = new UI_Button(*object.Cancel_Button);
	delete editText;
	editText = new UI_StaticText(*object.editText);
	delete userText;
	userText = new UI_StaticText(*object.userText);
	position = object.position;
	caller = object.caller;
	ani = object.ani;
	return(*this);
}

void UI_EditField::addChar(char a)
{
	if(position>=28)
		return;
	userText->addChar(position, a);
	position++;
	ani=5;
}

void UI_EditField::moveLeft()
{
	if(position>0)
		position--;
	ani=5;
}

void UI_EditField::moveRight()
{
	if(position<28)
		position++;
	ani=5;
}

void UI_EditField::removeCharBackspace()
{
	userText->removeCharBackspace(position);
	if(position>0) position--;
	ani=5;
}

void UI_EditField::removeCharDelete()
{
	userText->removeCharDelete(position);
	if(position>0) position--;
	ani=5;
}

#include <math.h>					  
void UI_EditField::draw(DC* dc) const
{
	dc->SetBrush(*theme.lookUpBrush(BRIGHT_UNIT_TYPE_3_BRUSH));
	dc->SetPen(*theme.lookUpPen(RECTANGLE_PEN));
	dc->DrawRectangle(Rect(getAbsolutePosition()-Size(10,10), getSize() + Size(20,20)));

	UI_Window::draw(dc);
	Rect entry_rect = Rect(getAbsolutePosition() + Size(25, 50), Size(getWidth()-50, 14));
	if(entry_rect.Inside(mouse))
		dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	else
		dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
	dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
	dc->DrawEdgedRoundedRectangle(entry_rect, 4);
	
	dc->SetPen(Pen(dc->mixColor(theme.lookUpColor(FORCE_TEXT_COLOR), theme.lookUpColor(BRIGHT_TEXT_COLOR), (unsigned int)(50*(sin(3.141*ani/10)+1))), 1, SOLID_PEN_STYLE));
	dc->DrawVerticalLine(entry_rect.GetLeft() + userText->getTextPosSize(position).GetWidth(),  entry_rect.GetTop() + 2, entry_rect.GetBottom() - 2);
}

void UI_EditField::process()
{
	if(!isShown())
		return;
	UI_Window::process();
	ani++;
}

UI_Object* UI_EditField::checkTooltip()
{
	if(!getAbsoluteRect().Inside(mouse))
		return(NULL);
	return(UI_Window::checkTooltip());
}

UI_Object* UI_EditField::checkHighlight()
{
	if(!getAbsoluteRect().Inside(mouse))
		return(NULL);
	return(UI_Window::checkHighlight());
}


