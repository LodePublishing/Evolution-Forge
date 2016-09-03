#include "tooltip.hpp"

UI_ToolTip& UI_ToolTip::operator=(const UI_ToolTip& object)
{
	boxSize = object.boxSize;
//	(UI_StaticText)(*this) = (UI_StaticText)object;
	(UI_LongText)(*this) = (UI_LongText)object;
	return(*this);
}

UI_ToolTip::UI_ToolTip(const UI_ToolTip& object) :
//	UI_StaticText((UI_StaticText)object),
	UI_LongText((UI_LongText)object),
	boxSize(object.boxSize)
{ }

UI_ToolTip::UI_ToolTip(UI_Object* parent_object, const std::string& tooltip_text) :
//	UI_StaticText(parent_object, text, Rect(5, 5, 150, 30), Size(0,0), TEXT_COLOR, SMALL_BOLD_FONT, DO_NOT_ADJUST),

	UI_LongText::UI_LongText(parent_object, Rect(2, 2, 2*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH) , 0), Size(0, 0), tooltip_text, TEXT_COLOR, IMPORTANT_COLOR, SMALL_FONT, TEXT_BUTTON),
	boxSize()
{ 
	process();
}

UI_ToolTip::UI_ToolTip(UI_Object* parent_object, const eString tooltip_text) :
//	UI_StaticText(parent_object, text, Rect(5, 5, 150, 30), Size(0,0), TEXT_COLOR, SMALL_BOLD_FONT, DO_NOT_ADJUST),
	UI_LongText::UI_LongText(parent_object, Rect(2, 2, 2*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0), Size(0, 0), UI_Object::theme.lookUpString(tooltip_text), TEXT_COLOR, IMPORTANT_COLOR, SMALL_FONT, TEXT_BUTTON),
	boxSize()
{ 
	process();
}

UI_ToolTip::~UI_ToolTip()
{ 
	UI_Object::toolTipWasDeleted = true;
}

void UI_ToolTip::reloadOriginalSize()
{
	setOriginalSize(Size(2*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0));
	UI_LongText::reloadOriginalSize();
}

void UI_ToolTip::process()
{
	Point t = getRelativePosition();
	Point p = mouse + Size(8, 8);
	if(p.x + (signed int)getWidth() >= (signed int)max_x)
		p.x =  (signed int)max_x - (signed int)getWidth();
	if(p.y + (signed int)getTextHeight() >= (signed int)max_y)
		p.y =  (signed int)max_y - (signed int)getTextHeight();
	setPosition(p);
//	UI_StaticText::process();
	UI_LongText::process();
}
                                                                                
void UI_ToolTip::draw(DC* dc) const
{
	if(!checkForNeedRedraw())
		return;
	dc->SetPen(*theme.lookUpPen(RECTANGLE_PEN));
	dc->SetBrush(*theme.lookUpBrush(TOOLTIP_BRUSH));
	dc->DrawRectangle(Rect(getAbsolutePosition()-Size(5,3), Size(10 + getWidth(), 6 + getTextHeight())));
//	UI_StaticText::draw(dc);
	UI_LongText::draw(dc);	
}

