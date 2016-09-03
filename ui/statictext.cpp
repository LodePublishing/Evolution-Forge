#include "statictext.hpp"

#include <sstream>

UI_StaticText& UI_StaticText::operator=(const UI_StaticText& object)
{
	((UI_Object)(*this)) = ((UI_Object)object);
	updateText(object.text);
	textWasChanged=true;
	font = object.font;
	color = object.color;
	eText = object.eText;
	pressed = object.pressed;
	highlight = object.highlight;
	return(*this);
}

UI_StaticText::UI_StaticText(const UI_StaticText& object) :
	UI_Object((UI_Object)object),
	text(),
	textWasChanged(true),
	font(object.font),
	color(object.color),
	eText(object.eText),
	pressed(object.pressed),
	highlight(object.highlight)
{}

UI_StaticText::UI_StaticText(UI_Object* st_parent, const Rect st_pos, const Size distance_bottom_right, const eColor st_color, const eFont st_font, const ePositionMode position_mode, const eAutoSize auto_size) :
	UI_Object(st_parent, st_pos, distance_bottom_right, position_mode, auto_size),
	text(),
	textWasChanged(true),
	font(st_font),
	color(*theme.lookUpColor(st_color)),
	eText(NULL_STRING),
	pressed(false),
	highlight(false)
{ }

UI_StaticText::UI_StaticText(UI_Object* st_parent, const eString st_text, const Rect st_pos, const Size distance_bottom_right, const eColor st_color, const eFont st_font, const ePositionMode position_mode, const eAutoSize auto_size) :
	UI_Object(st_parent, st_pos, distance_bottom_right, position_mode, auto_size),
	text(),
	textWasChanged(true),
	font(st_font),
	color(*theme.lookUpColor(st_color)),
	eText(st_text),
	pressed(false),
	highlight(false)
{
//	updateText(eText);
//	if(getParent()) 
//		adjustPositionAndSize(ADJUST_AFTER_CHILD_SIZE_WAS_CHANGED, getTextSize()); TODO
}

UI_StaticText::UI_StaticText(UI_Object* st_parent, const std::string& st_text, const Rect st_pos, const Size distance_bottom_right, const eColor st_color, const eFont st_font, const ePositionMode position_mode, const eAutoSize auto_size) :
	UI_Object(st_parent, st_pos, distance_bottom_right, position_mode, auto_size),
	text(st_text),
	textWasChanged(true),
	font(st_font),
	color(*theme.lookUpColor(st_color)),
	eText(NULL_STRING),
	pressed(false),
	highlight(false)
{
//	updateText(text);
//	if(getParent()) 
//		adjustPositionAndSize(ADJUST_AFTER_CHILD_SIZE_WAS_CHANGED, getTextSize()); TODO, fuer UI_Group groessen bei Buttons... AUTO_HEIGHT muss irgendwo berechnet werden...
}

UI_StaticText::~UI_StaticText()
{}

UI_Object* UI_StaticText::checkTooltip() {
	if( (!isShown()) || (!Rect(getAbsolutePosition(), getTextSize()).Inside(mouse )) )
		return(0);
	return((UI_Object*)this);
}

void UI_StaticText::draw(DC* dc) const
{
	if(!isShown())
		return;
	if(!checkForNeedRedraw())
		return;

//	if(font!=NULL_FONT)
		dc->SetFont(theme.lookUpFont(font));
	if(color!=NULL_COLOR) 
	{
		if(highlight==true)
			dc->SetTextForeground(dc->brightenColor(color, 60));
		else
			dc->SetTextForeground(color);
	}
		
	
	if(pressed)
		dc->DrawText(text, getAbsolutePosition() + Size(1, 4));
	else
		dc->DrawText(text, getAbsolutePosition() + Size(0, 3));
	UI_Object::draw(dc);
}

void UI_StaticText::addChar(const unsigned int pos, const char key)
{
	std::ostringstream os;
	os.str("");
	if(pos>=text.size())
		os << text << key;
	else
	if(pos==0)
		os << key << text;
	else
		os << text.substr(0, pos) << key << text.substr(pos);
	updateText(os.str());
}

void UI_StaticText::removeCharBackspace(const unsigned int pos)
{
	if((pos==0)||(text.size()==0))
		return;
	std::ostringstream os;
	os.str("");
	if(pos>=text.size())
		os << text.substr(0, text.size()-1);
	else
		os << text.substr(0, pos-1) << text.substr(pos);
	updateText(os.str());
}

void UI_StaticText::removeCharDelete(const unsigned int pos)
{
	if((text.size()==0)||(pos>=text.size()))
		return;
	std::ostringstream os;
	os.str("");
	if(pos==0)
		os << text.substr(1);
	else
	if(pos==text.size()-1)
		os << text.substr(0, text.size()-1);
	else
		os << text.substr(0, pos) << text.substr(pos+1);
	updateText(os.str());
}

void UI_StaticText::process()
{
	UI_Object::process();
	if(textWasChanged)
	{
		if(eText != NULL_STRING)
			reloadText(theme.lookUpString(eText));
		else reloadText(text);
		textWasChanged=false;
	}
}

void UI_StaticText::reloadOriginalSize()
{
	reloadStrings();
	UI_Object::reloadOriginalSize();
}

void UI_StaticText::updateText(const std::string& st_text)
{
//	if(st_text==text)
//		return;
	setNeedRedrawMoved();
	textWasChanged=true;
	Size old_size = getTextSize();
	text = st_text;
	setSize(getTextSize());
	if((getParent()!=NULL)&&(old_size!=getTextSize()))
	{
//		getParent()->resetMinXY();
		getParent()->adjustPositionAndSize(ADJUST_AFTER_CHILD_SIZE_WAS_CHANGED, getSize());
	}
}

void UI_StaticText::updateText(const eString st_text)
{
	eText = st_text;
//	if(eText!=st_text)
		textWasChanged=true;
	text = theme.lookUpString(st_text);
	updateText(theme.lookUpString(st_text));
}

void UI_StaticText::reloadText(const std::string& st_text)
{
	setNeedRedrawMoved();
	textWasChanged=true;
	Size old_size = getTextSize();
	text = st_text;
	setSize(getTextSize());
	if((getParent()!=NULL))
	{
//		getParent()->resetMinXY();
		getParent()->adjustPositionAndSize(ADJUST_AFTER_CHILD_SIZE_WAS_CHANGED, getSize());
	}
}

void UI_StaticText::reloadText(const eString st_text)
{
	eText = st_text;
	textWasChanged=true;
	text = theme.lookUpString(st_text);
	reloadText(theme.lookUpString(st_text));
}


void UI_StaticText::reloadStrings()
{
	if(eText!=NULL_STRING)
		reloadText(theme.lookUpString(eText));
	else
		reloadText(text);
}


