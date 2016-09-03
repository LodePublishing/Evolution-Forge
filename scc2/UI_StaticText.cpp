#include "UI_StaticText.h"

UI_StaticText::UI_StaticText(UI_Object* parent, wxRect pos, eTextMode mode, eFont font, eColour colour):UI_Object(parent, pos, pos)
{
	this->text=wxString("NULL");
	this->font=font;
	setColour(colour);
	this->mode=mode;
};

UI_StaticText::UI_StaticText(UI_Object* parent, eString text, wxRect pos, eTextMode mode, eFont font, eColour colour):UI_Object(parent, pos, pos)
{
	updateText(text);
	this->font=font;
	setColour(colour);
	this->mode=mode;
};

UI_StaticText::UI_StaticText(UI_Object* parent, wxString text, wxRect pos, eTextMode mode, eFont font, eColour colour):UI_Object(parent, pos, pos)
{
	updateText(text);
	this->font=font;
	setColour(colour);
	this->mode=mode;
};

UI_StaticText::~UI_StaticText()
{
};


wxSize UI_StaticText::getSize()
{
	int dx,dy;
	wxMemoryDC* tdc=new wxMemoryDC();
    tdc->SelectObject(wxBitmap(1000,1000));
	tdc->GetTextExtent(text, &dx, &dy);
	delete tdc;
	return(wxSize(dx,dy));
};

void UI_StaticText::draw(wxDC* dc)
{
	if(!shown)
		return;
	UI_Object::draw(dc);
	if(font!=NULL_FONT)
		dc->SetFont(*theme.lookUpFont(font));

//	if(colour!=NULL_COLOUR) 
		dc->SetTextForeground(colour); //~~

	int dx, dy;
	dc->GetTextExtent(text, &dx, &dy);
	wxRect temp=getRelativeRect();
	switch(mode)
	{
		case VERTICALLY_CENTERED_TEXT_MODE:
			temp.SetY(temp.GetY() + (temp.GetHeight() - dy)/2 );break;
		case RIGHT_BOUNDED_TEXT_MODE:
			temp.SetX(temp.GetX()+temp.GetWidth()-dx);break;
		case HORIZONTALLY_CENTERED_TEXT_MODE:
			temp.SetX(temp.GetX() + (temp.GetWidth() - dx)/2);break;
		case TOTAL_CENTERED_TEXT_MODE:
			temp.SetX(temp.GetX() + (temp.GetWidth() - dx)/2);
			temp.SetY(temp.GetY() + (temp.GetHeight() - dy)/2 );break;
		
		case LOWER_CENTERED_TEXT_MODE:
			temp.SetX(temp.GetX() + (temp.GetWidth() - dx)/2);
			temp.SetY(temp.GetY() + (temp.GetHeight() - dy)*2/3 );break;
		case UPPER_CENTERED_TEXT_MODE:
			temp.SetX(temp.GetX() + (temp.GetWidth() - dx)/2);
			temp.SetY(temp.GetY() + (temp.GetHeight() - dy)/3 );break;
		default:break;
	};
	// TODO Formatted, block text

	dc->DrawText(text, temp.GetPosition() + wxPoint(0,1) + getParent()->getAbsolutePosition());
};

void UI_StaticText::process()
{
	UI_Object::process();
	// TODO, kA was
};

void UI_StaticText::updateText(wxString text)
{
	this->text=text;
};

void UI_StaticText::updateText(eString text)
{
	this->text=*theme.lookUpString(text);
};

void UI_StaticText::setColour(eColour colour)
{
	this->colour=*theme.lookUpColour(colour);
};

void UI_StaticText::setColour(wxColour colour)
{
	this->colour=colour;
};

void UI_StaticText::setMode(eTextMode  mode)
{
	this->mode=mode;
};

void UI_StaticText::setFont(eFont font)
{
	this->font=font;
};

