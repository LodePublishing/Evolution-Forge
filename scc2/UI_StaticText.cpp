#include "UI_StaticText.h"

UI_StaticText::UI_StaticText(UI_Object* parent, Rect pos, eTextMode mode, eFont font, eColor color):UI_Object(parent, pos, pos)
{
	this->text=string("NULL");
	this->font=font;
	setColor(color);
	this->mode=mode;
};

UI_StaticText::UI_StaticText(UI_Object* parent, eString text, Rect pos, eTextMode mode, eFont font, eColor color):UI_Object(parent, pos, pos)
{
	updateText(text);
	this->font=font;
	setColor(color);
	this->mode=mode;
};

UI_StaticText::UI_StaticText(UI_Object* parent, string text, Rect pos, eTextMode mode, eFont font, eColor color):UI_Object(parent, pos, pos)
{
	updateText(text);
	this->font=font;
	setColor(color);
	this->mode=mode;
};

UI_StaticText::~UI_StaticText()
{
};

Size UI_StaticText::getSize()
{
	int dx,dy;
	theme.lookUpFont(font)->GetTextExtent(text.c_str(), &dx, &dy);
	return(Size(dx,dy));
};

void UI_StaticText::draw(DC* dc)
{
	if(!shown)
		return;
	UI_Object::draw(dc);
	if(font!=NULL_FONT)
		dc->SetFont(theme.lookUpFont(font));

//	if(color!=NULL_COLOUR) 
		dc->SetTextForeground(color); //~~

	int dx, dy;
	dc->GetTextExtent(text, &dx, &dy);
	Rect temp=getRelativeRect();
	switch(mode)
	{
		case VERTICALLY_CENTERED_TEXT_MODE:
			temp.SetTop(temp.GetTop() + (temp.GetHeight() - dy)/2 );break;
		case RIGHT_BOUNDED_TEXT_MODE:
			temp.SetLeft(temp.GetLeft()+temp.GetWidth()-dx);break;
		case HORIZONTALLY_CENTERED_TEXT_MODE:
			temp.SetLeft(temp.GetLeft() + (temp.GetWidth() - dx)/2);break;
		case TOTAL_CENTERED_TEXT_MODE:
			temp.SetLeft(temp.GetLeft() + (temp.GetWidth() - dx)/2);
			temp.SetTop(temp.GetTop() + (temp.GetHeight() - dy)/2 );break;
		
		case LOWER_CENTERED_TEXT_MODE:
			temp.SetLeft(temp.GetLeft() + (temp.GetWidth() - dx)/2);
			temp.SetTop(temp.GetTop() + (temp.GetHeight() - dy)*2/3 );break;
		case UPPER_CENTERED_TEXT_MODE:
			temp.SetLeft(temp.GetLeft() + (temp.GetWidth() - dx)/2);
			temp.SetTop(temp.GetTop() + (temp.GetHeight() - dy)/3 );break;
		default:break;
	};
	// TODO Formatted, block text

	dc->DrawText(text, temp.GetPosition() + Point(0,2) + getParent()->getAbsolutePosition());
};

void UI_StaticText::process()
{
	UI_Object::process();
	// TODO, kA was
};

void UI_StaticText::updateText(string text)
{
	this->text=text;
};

void UI_StaticText::updateText(eString text)
{
	this->text=*theme.lookUpString(text);
};

void UI_StaticText::setColor(eColor color)
{
	this->color=*theme.lookUpColor(color);
};

void UI_StaticText::setColor(Color color)
{
	this->color=color;
};

void UI_StaticText::setMode(eTextMode  mode)
{
	this->mode=mode;
};

void UI_StaticText::setFont(eFont font)
{
	this->font=font;
};

