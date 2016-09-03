#include "progressbar.hpp"
#include "../ui/object.hpp"

void ProgressBar::draw(DC* dc, const unsigned int dp)
{
	if(dp == 100)
		p = 100;
	else
		p += dp;

    dc->SetPen(Pen(Color(dc->GetSurface(), 80, 100, 200), 1, SOLID_PEN_STYLE));
    dc->SetBrush(Brush(Color(dc->GetSurface(), 50, 60, 155), SOLID_BRUSH_STYLE));

    dc->DrawEdgedRoundedRectangle(Rect(bar.GetLeft(), bar.GetTop(), bar.GetWidth() * p/ 100, bar.GetHeight()), 8);
	dc->updateScreen();
}

void ProgressBar::draw(DC* dc, const unsigned int dp, const string& text)
{
    //dc->SetPen(Pen(Color(dc->GetSurface(), 255, 255, 255), 1, TRANSPARENT_PEN_STYLE));
  //  dc->SetBrush(Brush(Color(dc->GetSurface(), 0, 0, 0), SOLID_BRUSH_STYLE));
//    dc->DrawRectangle(Rect(bar.GetLeft(), bar.GetTop()+bar.GetHeight()+40+h*20, 350, 50));
    dc->SetFont(UI_Object::theme.lookUpFont(MIDDLE_NORMAL_BOLD_FONT));
	dc->SetTextForeground(*UI_Object::theme.lookUpColor(BRIGHT_TEXT_COLOR));
	dc->DrawText(text, Point(bar.GetLeft(), bar.GetTop()+bar.GetHeight()+50+h*20));
	dc->SetTextForeground(*UI_Object::theme.lookUpColor(FORCE_TEXT_COLOR));
	dc->DrawText("OK", Point(bar.GetLeft()+400, bar.GetTop()+bar.GetHeight()+50+h*20));
	draw(dc, dp);
	h++;
}

