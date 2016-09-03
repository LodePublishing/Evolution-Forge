#include "tooltip.hpp"

UI_Tooltip& UI_Tooltip::operator=(const UI_Tooltip& object)
{
	boxSize = object.boxSize;
	(UI_StaticText)(*this) = (UI_StaticText)object;
	return(*this);
}

UI_Tooltip::UI_Tooltip(const UI_Tooltip& object) :
	UI_StaticText((UI_StaticText)object),
	boxSize(object.boxSize)
{ }

UI_Tooltip::UI_Tooltip(UI_Object* parent_object, const eString tooltip_text) :
	UI_StaticText(parent_object, tooltip_text==NULL_STRING?START_START_STRING:tooltip_text, Rect(5, 5, 100, 30), TEXT_COLOR, SMALL_ITALICS_BOLD_FONT, FORMATTED_NON_BLOCK_TEXT_MODE),
	boxSize()
{ 
	process();
}

UI_Tooltip::~UI_Tooltip()
{ }

void UI_Tooltip::process()
{
	Point p = mouse + Size(8, 8);
	if(p.x + (signed int)getBoxSize().GetWidth() >= (signed int)max_x)
		p.x =  (signed int)max_x - (signed int)getBoxSize().GetWidth();
	if(p.y + (signed int)getBoxSize().GetHeight() >= (signed int)max_y)
		p.y =  (signed int)max_y - (signed int)getBoxSize().GetHeight();
	setPosition(p);
	UI_StaticText::process();
}
                                                                                
void UI_Tooltip::draw(DC* dc) const
{
	
	dc->SetPen(*theme.lookUpPen(RECTANGLE_PEN));
	dc->SetBrush(*theme.lookUpBrush(TOOLTIP_BRUSH));
	dc->DrawRectangle(Rect(getAbsolutePosition()-Size(5,5), getBoxSize()));
	UI_StaticText::draw(dc);
}

