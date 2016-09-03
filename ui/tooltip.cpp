#include "tooltip.hpp"

UI_ToolTip::UI_ToolTip(UI_Object* parent_object, const std::string& tooltip_text) :
	UI_LongText::UI_LongText(parent_object, Rect(2, 2, 2*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH) , 0), Size(0, 0), tooltip_text, TEXT_COLOR, DARK_IMPORTANT_COLOR, SMALL_FONT, TEXT_BUTTON),
	boxSize()
{ 
	process();
}

UI_ToolTip::UI_ToolTip(UI_Object* parent_object, const eString tooltip_text) :
	UI_LongText::UI_LongText(parent_object, Rect(2, 2, 2*UI_Object::theme.lookUpButtonWidth(STANDARD_BUTTON_WIDTH), 0), Size(0, 0), UI_Object::theme.lookUpString(tooltip_text), TEXT_COLOR, DARK_IMPORTANT_COLOR, SMALL_FONT, TEXT_BUTTON),
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
	if(p.x + (signed int)getTextWidth() >= (signed int)max_x - 10)
		p.x =  (signed int)max_x - (signed int)getTextWidth() - 10;
	if(p.y + (signed int)getTextHeight() >= (signed int)max_y - 10)
		p.y =  (signed int)max_y - (signed int)getTextHeight() - 10;
	setPosition(p);
	UI_LongText::process();
}
                                                                                
void UI_ToolTip::draw(DC* dc) const
{
	if(!checkForNeedRedraw())
		return;
	dc->setPen(*theme.lookUpPen(RECTANGLE_PEN));
	dc->setBrush(*theme.lookUpBrush(TOOLTIP_BRUSH));
	dc->DrawRectangle(Rect(getAbsolutePosition()-Size(1,1), Size(5 + getTextWidth(), 5 + getTextHeight())));
	UI_LongText::draw(dc);	
}

