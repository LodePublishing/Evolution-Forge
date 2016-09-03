#include "group.hpp"

UI_Group::UI_Group(UI_Object* parent, Rect rect, const string& title) : 
	UI_Object(parent, rect, rect),
	title(title)
{ }

UI_Group::~UI_Group()
{ }

void UI_Group::draw(DC* dc) const
{
	if(isMouseInside())
		dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	else
		dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
	dc->SetBrush(*theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH));
	dc->DrawEdgedRoundedRectangle(getAbsoluteRect().GetLeft() - 5, getAbsoluteRect().GetTop() - 15, getWidth()+10, getHeight()+10, 4);

	dc->SetBrush(*theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH));
    Size s = dc->GetTextExtent(title);
    Rect titleRect = Rect(getAbsolutePosition() - Size(5,17), s - Size(0,2) + Size(5,0));
	dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
    dc->DrawRectangle(titleRect);
    dc->DrawText(title, titleRect.GetTopLeft()+Point(2,2));
	UI_Object::draw(dc);
}

void UI_Group::process()
{
    if(!isShown())
        return;
    UI_Object::process();
}

UI_Object* UI_Group::checkHighlight()
{
    if(!(isMouseInside()))
        return(0);
    return(UI_Object::checkHighlight());
}


