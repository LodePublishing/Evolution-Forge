#include "statictext.hpp"
#include <sstream>

UI_StaticText::UI_StaticText(UI_Object* st_parent, const Rect st_pos, const Size distance_bottom_right, const eColor st_color, const eFont st_font, const ePositionMode position_mode) :
	UI_Object(st_parent, st_pos, distance_bottom_right, position_mode, AUTO_SIZE),
	text(),
	textWasChanged(true),
	font(st_font),
	color(st_color),
	tempColor(),
	tempColorIsSet(false),
	eText(NULL_STRING),
	pressed(false),
	highlight(false)
{ }

UI_StaticText::UI_StaticText(UI_Object* st_parent, const eString st_text, const Rect st_pos, const Size distance_bottom_right, const eColor st_color, const eFont st_font, const ePositionMode position_mode) :
	UI_Object(st_parent, st_pos, distance_bottom_right, position_mode, NO_AUTO_SIZE),
	text(),
	textWasChanged(true),
	font(st_font),
	color(st_color),
	tempColor(),
	tempColorIsSet(false),
	eText(st_text),
	pressed(false),
	highlight(false)
{}

UI_StaticText::UI_StaticText(UI_Object* st_parent, const std::string& st_text, const Rect st_pos, const Size distance_bottom_right, const eColor st_color, const eFont st_font, const ePositionMode position_mode) :
	UI_Object(st_parent, st_pos, distance_bottom_right, position_mode, NO_AUTO_SIZE),
	text(st_text),
	textWasChanged(true),
	font(st_font),
	color(st_color),
	tempColor(),
	tempColorIsSet(false),
	eText(NULL_STRING),
	pressed(false),
	highlight(false)
{}

UI_StaticText::~UI_StaticText()
{}

void UI_StaticText::doHighlight(const bool high_light) 
{
	if(high_light == highlight)
		return;
	setNeedRedrawMoved();
	highlight = high_light;
}

UI_Object* UI_StaticText::checkToolTip() {
	if( (!isShown()) || (!Rect(getAbsolutePosition(), getTextSize()).Inside(mouse )) )
		return(0);
	return((UI_Object*)this);
}

void UI_StaticText::draw(DC* dc) const
{
	if(!isShown())
		return;
	if(checkForNeedRedraw())
	{
	//	if(font!=NULL_FONT)
			dc->setFont(theme.lookUpFont(font));
		if(color!=NULL_COLOR) 
		{
			if(tempColorIsSet)
			{
				if(highlight==true)
					dc->setTextForeground(dc->changeAbsoluteBrightness(tempColor, 60));
				else
					dc->setTextForeground(tempColor);
			} else
			{
				if(highlight==true)
					dc->setTextForeground(dc->changeAbsoluteBrightness(*UI_Object::theme.lookUpColor(color), 60));
				else
					dc->setTextForeground(*UI_Object::theme.lookUpColor(color));
			}
		}
		if(pressed)
			dc->DrawText(text, getAbsolutePosition() + Size(1, 4));
		else
			dc->DrawText(text, getAbsolutePosition() + Size(0, 3));
	}
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
	tempColorIsSet = false;
	if( (isShown()) && (Rect(getAbsolutePosition(), getTextSize()).Inside(mouse )) )
		setNeedRedrawMoved();
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
	if(st_text==text)
		return; //?
	setNeedRedrawMoved();
	textWasChanged=true;
	Size old_size = getTextSize();
	text = st_text;
	setSize(getTextSize());	
}

void UI_StaticText::updateText(const eString st_text)
{
	if(eText==st_text) // o_O
		return;
	eText = st_text;
	textWasChanged=true;
	text = theme.lookUpString(st_text);
	updateText(theme.lookUpString(st_text));
}

void UI_StaticText::reloadText(const std::string& st_text)
{
	setNeedRedrawMoved();
	textWasChanged=true;
	text = st_text;
	setSize(getTextSize());
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

