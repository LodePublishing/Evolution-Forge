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
//	setNeedRedrawMoved(); TODO
	highlight = high_light;
}

UI_Object* UI_StaticText::checkToolTip() {
	if( (!isShown()) || (!Rect(getAbsolutePosition(), getTextSize()).isInside(mouse )) )
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
		Color normal;
		Color highlighted;
		if(color!=NULL_COLOR) 
		{
			if(tempColorIsSet)
			{
				highlighted = dc->changeAbsoluteBrightness(tempColor, 60);
				normal = tempColor;
			} else
			{
				highlighted = dc->changeAbsoluteBrightness(*UI_Object::theme.lookUpColor(color), 60);
				normal = *UI_Object::theme.lookUpColor(color);
			}
			if(highlight)
				dc->setTextForeground(highlighted);
			else
				dc->setTextForeground(normal);
		}
		Point p = getAbsolutePosition() + Size(0,3);
		if(pressed) 
			p = p + Size(1,1);
		bool done = false;
		if(color!=NULL_COLOR)
		for(unsigned int i = 0; i < text.size(); i++)
			if(text[i] == '&')
			{
				if(i > 0)
					dc->DrawText(text.substr(0, i), p);
				dc->setTextForeground(*UI_Object::theme.lookUpColor(FORCE_TEXT_COLOR));
				if(i + 1 < text.size())
					dc->DrawText(text.substr(i+1, 1), p + Size(dc->getTextExtent(text.substr(0,i)).getWidth(),0));

				if(highlight)
					dc->setTextForeground(highlighted);
				else
					dc->setTextForeground(normal);
				if((i+2) < text.size())
					dc->DrawText(text.substr(i+2, text.size() - i - 2), p + Size(dc->getTextExtent(text.substr(i+1, 1)).getWidth() + dc->getTextExtent(text.substr(0,i)).getWidth(), 0));	
				done = true;
				break;
			}
		if(!done)
			dc->DrawText(text, p);
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
//	if( (isShown()) && (Rect(getAbsolutePosition(), getTextSize()).Inside(mouse )) )
//		setNeedRedrawMoved(); ??
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
//	setNeedRedrawMoved(); TODO
	textWasChanged=true;
	Size old_size = getTextSize();
	text = st_text;
	eText = NULL_STRING;
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
//	setNeedRedrawMoved(); TODO
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

