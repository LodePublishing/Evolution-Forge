#include "savebox.hpp"
#include <sstream>


// TODO: Tatsaechliches Eingabefeld seperat machen
// ISO-LEVEL 2 

SaveBox::SaveBox(UI_Object* savebox_parent, const eString savebox_text, const eString description_text, const eString ok_string, const eString cancel_string, const std::string& name_proposal) :
	UI_Window(savebox_parent, savebox_text, theme.lookUpGlobalRect(SAVE_BOX_WINDOW), theme.lookUpGlobalMaxHeight(SAVE_BOX_WINDOW)),
	OK_Button(new UI_Button(this, getRelativeClientRect(), Size(0,0), STANDARD_BUTTON, NULL_BITMAP, PRESS_BUTTON_MODE, ok_string, BOTTOM_CENTER, MIDDLE_BOLD_FONT, AUTO_SIZE)),
	Cancel_Button(new UI_Button(this, getRelativeClientRect(), Size(0,0), STANDARD_BUTTON, NULL_BITMAP, PRESS_BUTTON_MODE, cancel_string, BOTTOM_RIGHT, MIDDLE_BOLD_FONT, AUTO_SIZE)),
	editField(new UI_EditField(this, Rect(getRelativeClientRectPosition() + Point(30, 50), Size(getClientRectWidth()-60, FONT_SIZE+6)), Size(0,0), MIDDLE_BOLD_FONT, DO_NOT_ADJUST, description_text, name_proposal))
{ 
	UI_Object::focus = editField;
}

SaveBox::SaveBox(UI_Object* savebox_parent, const SaveBoxParameter savebox_parameter):
	UI_Window(savebox_parent, savebox_parameter.saveboxText, theme.lookUpGlobalRect(SAVE_BOX_WINDOW), theme.lookUpGlobalMaxHeight(SAVE_BOX_WINDOW)),
	OK_Button(new UI_Button(this, getRelativeClientRect(), Size(0,0), STANDARD_BUTTON, NULL_BITMAP, PRESS_BUTTON_MODE, savebox_parameter.okString, BOTTOM_CENTER, MIDDLE_BOLD_FONT, AUTO_SIZE)),
	Cancel_Button(new UI_Button(this, getRelativeClientRect(), Size(0,0), STANDARD_BUTTON, NULL_BITMAP, PRESS_BUTTON_MODE, savebox_parameter.cancelString, BOTTOM_RIGHT, MIDDLE_BOLD_FONT, AUTO_SIZE)),
	editField(new UI_EditField(this, Rect(getRelativeClientRectPosition() + Point(30, 50), Size(getClientRectWidth()-60, FONT_SIZE+6)), Size(0,0), MIDDLE_BOLD_FONT, DO_NOT_ADJUST, savebox_parameter.descriptionText, savebox_parameter.inputProposal))
{ 
	UI_Object::focus = editField;
	process();
}

SaveBox::~SaveBox()
{
	delete OK_Button;
	delete Cancel_Button;
	delete editField;
	UI_Object::resetWindow();
	UI_Button::resetButton();
	if(UI_Object::focus==editField)
		UI_Object::focus=NULL;
}

void SaveBox::reloadOriginalSize()
{
	setOriginalRect(UI_Object::theme.lookUpGlobalRect(SAVE_BOX_WINDOW));
	setMaxHeight(UI_Object::theme.lookUpGlobalMaxHeight(SAVE_BOX_WINDOW));
	OK_Button->setOriginalRect(getRelativeClientRect());
	Cancel_Button->setOriginalRect(getRelativeClientRect());
	editField->setOriginalRect(Rect(getRelativeClientRectPosition() + Point(30, 37), getClientRectSize()));

	UI_Window::reloadOriginalSize();
}

void SaveBox::draw() const
{
	dc->setBrush(*theme.lookUpBrush(BRIGHT_UNIT_TYPE_3_BRUSH));
	dc->setPen(*theme.lookUpPen(RECTANGLE_PEN));
	dc->DrawEdgedRoundedRectangle(Rect(Point(0,0), getSize() + Size(2,2)),6);
	UI_Window::draw();
}

void SaveBox::process()
{
	if(!isShown())
		return;
	UI_Window::process();
}

UI_Object* SaveBox::checkToolTip()
{
	if(!getAbsoluteRect().isTopLeftCornerInside(mouse))
		return(NULL);
	return(UI_Window::checkToolTip());
}

UI_Object* SaveBox::checkHighlight()
{
	if(!getAbsoluteRect().isTopLeftCornerInside(mouse))
		return(NULL);
	return(UI_Window::checkHighlight());
}

const bool SaveBox::isCanceled() const {
	return(Cancel_Button->isLeftClicked()||editField->canceled());
}

const bool SaveBox::isDone() const {
	return((OK_Button->isLeftClicked()||editField->done())&&(getString().size()>0));
}
