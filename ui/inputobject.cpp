#include "editfield.hpp"

// TODO: Tatsaechliches Eingabefeld seperat machen
// ISO-LEVEL 2 

UI_EditField::UI_EditField(UI_Object* edit_parent, 
		const Rect& edit_rect, 
		const Size& edit_size, 
		const signed int zcoord,
		const ePositionMode position_mode, 
		const eColor st_color, 
		const eFont st_font, 
		const std::string& name_proposal) :
	UI_StaticText(edit_parent, name_proposal, edit_rect, edit_size, zcoord, st_color, st_font, position_mode),
	position(name_proposal.size()),
	ani(5),
	pressedEnter(false),
	pressedEscape(false)
{ 
	UI_Object::addEditField(this);
}

UI_EditField::~UI_EditField()
{
	UI_Object::removeEditField(this);
}

void UI_EditField::resetData()
{
	updateText(" ");
	pressedEnter = false;
	pressedEscape = false;
}

/*
UI_EditField::UI_EditField(const UI_EditField& object) :
	UI_StaticText((UI_StaticText)object),
	position(object.position),
	ani(object.ani),
	pressedEnter(object.pressedEnter)
{ }

UI_EditField& UI_EditField::operator=(const UI_EditField& object)
{
	(UI_StaticText)(*this) = (UI_StaticText)object;
	position = object.position;
	ani = object.ani;
	pressedEnter = object.pressedEnter;
	return(*this);
}*/

void UI_EditField::addChar(char a)
{
	if(position>=28)
		return;
	((UI_StaticText*)(this))->addChar(position, a);
	++position;
	ani=5;
}

void UI_EditField::moveLeft()
{
	if(position>0)
		--position;
	ani=5;
}

void UI_EditField::moveRight()
{
	if(position<28)
		++position;
	ani=5;
}

void UI_EditField::removeCharBackspace()
{
	((UI_StaticText*)(this))->removeCharBackspace(position);
	if(position>0) --position;
	ani=5;
}

void UI_EditField::removeCharDelete()
{
	((UI_StaticText*)(this))->removeCharDelete(position);
	if(position>0) --position;
	ani=5;
}

void UI_EditField::draw() const
{
	UI_StaticText::draw();
	Rect entry_rect = Rect(Point(0,0), Size(getOriginalRect().GetWidth(), getTextSize().GetHeight()));
	if(entry_rect.Inside(mouse))
		dc->SetPen(*theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
	else
		dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
	dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
	dc->DrawEdgedRoundedRectangle(entry_rect, 4);
	
	dc->SetPen(Pen(dc->mixColor(*theme.lookUpColor(FORCE_TEXT_COLOR), *theme.lookUpColor(BRIGHT_TEXT_COLOR), (unsigned int)(50*(sin(3.141*ani/10)+1))), 1, SOLID_PEN_STYLE));
	dc->DrawVerticalLine(getTextPosSize(position).GetWidth(), entry_rect.GetTop() + 2, entry_rect.GetBottom() - 2);
}

void UI_EditField::process()
{
	if(!isShown())
		return;
	UI_StaticText::process();
	++ani;
}




