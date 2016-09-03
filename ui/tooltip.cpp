#include "tooltip.hpp"

UI_ToolTip& UI_ToolTip::operator=(const UI_ToolTip& object)
{
	boxSize = object.boxSize;
	(UI_StaticText)(*this) = (UI_StaticText)object;
	return(*this);
}

UI_ToolTip::UI_ToolTip(const UI_ToolTip& object) :
	UI_StaticText((UI_StaticText)object),
	boxSize(object.boxSize)
{ }

UI_ToolTip::UI_ToolTip(UI_Object* parent_object, const eString tooltip_text) :
	UI_StaticText(parent_object, tooltip_text, Rect(5, 5, 150, 30), Size(0,0), TEXT_COLOR, SMALL_BOLD_FONT),
	boxSize()
{ 
	process();
}

UI_ToolTip::~UI_ToolTip()
{ }

void UI_ToolTip::process()
{
	Point t = getRelativePosition();
	Point p = mouse + Size(8, 8);
	if(p.x + (signed int)getSize().GetWidth() >= (signed int)max_x)
		p.x =  (signed int)max_x - (signed int)getSize().GetWidth();
	if(p.y + (signed int)getSize().GetHeight() >= (signed int)max_y)
		p.y =  (signed int)max_y - (signed int)getSize().GetHeight();
	setPosition(p);
	UI_StaticText::process();
//	if(t!=p)
		setNeedRedrawMoved();
}
                                                                                
void UI_ToolTip::draw(DC* dc) const
{
	if(!checkForNeedRedraw())
		return;
	dc->SetPen(*theme.lookUpPen(RECTANGLE_PEN));
	dc->SetBrush(*theme.lookUpBrush(TOOLTIP_BRUSH));
	dc->DrawRectangle(Rect(getAbsolutePosition()-Size(5,5), getSize()));
	UI_StaticText::draw(dc);
}

