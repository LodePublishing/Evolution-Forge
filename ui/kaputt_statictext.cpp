#include "statictext.hpp"

#include <sstream>

UI_StaticText& UI_StaticText::operator=(const UI_StaticText& object)
{
	((UI_Object)(*this)) = ((UI_Object)object);
	mode = object.mode;
	text = object.text;
	font = object.font;
	position = object.position;
	color = object.color;
//	editable = object.editable;
	eText = object.eText;
	textMode = object.textMode;
	pressed = object.pressed;
	textBox = object.textBox;
	return(*this);
}

UI_StaticText::UI_StaticText(const UI_StaticText& object) :
	UI_Object((UI_Object)object),
	mode(object.mode),
	text(object.text),
	font(object.font),
	position(object.position),
	color(object.color),
	eText(object.eText),
	textMode(object.textMode),
//	editable(object.editable)
	pressed(object.pressed),
	textBox(object.textBox)
{
	updateText(object.text);
}

UI_StaticText::UI_StaticText(UI_Object* st_parent, const Rect st_pos, const eColor st_color, const eFont st_font, const eTextMode st_mode) :
	UI_Object(st_parent, st_pos, st_pos),
	mode(st_mode),
	text("ERROR"),
	font(st_font),
	position(1),
	color(*theme.lookUpColor(st_color)),
	eText(NULL_STRING),
	textMode(true),
//	editable(false)
	pressed(false),
	textBox(getAbsoluteRect())
{ }

UI_StaticText::UI_StaticText(UI_Object* st_parent, const eString st_text, const Rect st_pos, const eColor st_color, const eFont st_font, const eTextMode st_mode) :
	UI_Object(st_parent, st_pos, st_pos),
	mode(st_mode),
	text("ERROR"),
	font(st_font),
	position(1),
	color(*theme.lookUpColor(st_color)),
	eText(NULL_STRING),
	textMode(true),
//	editable(false)
	pressed(false),
	textBox(getAbsoluteRect())
{
	updateText(st_text);
}

UI_StaticText::UI_StaticText(UI_Object* st_parent, const string& st_text, const Rect st_pos, const eColor st_color, const eFont st_font, const eTextMode st_mode) :
	UI_Object(st_parent, st_pos, st_pos),
	mode(st_mode),
	text("ERROR"),
	font(st_font),
	position(1),
	color(*theme.lookUpColor(st_color)),
	eText(NULL_STRING),
	textMode(true),
//	editable(false)
	pressed(false),
	textBox(getAbsoluteRect())
{
	updateText(st_text);
}

UI_StaticText::~UI_StaticText()
{ }

const Size& UI_StaticText::getBoxSize() const {
	return(textBox.GetSize());
}

//const Rect& UI_StaticText::getTextBox() const {
//	return(textBox);
//}

void UI_StaticText::setPressed(const bool press) {
	pressed = press;
}

UI_Object* UI_StaticText::checkHighlight() {
	return(UI_Object::checkHighlight());
}

UI_Object* UI_StaticText::checkTooltip() {
    if( (!isShown()) || (isDisabled()) || (!textBox.Inside(mouse)) )
        return(0);
    return((UI_Object*)this);
}

const bool UI_StaticText::doneWriting() const {
	return(position >= text.size());
}

const Size UI_StaticText::getTextSize() const
{
	return(theme.lookUpFont(font)->GetTextExtent(text));
}

const Size UI_StaticText::getTextPosSize(const unsigned pos) const
{
	return(theme.lookUpFont(font)->GetTextExtent(text.substr(0, pos)));
}

void UI_StaticText::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Object::draw(dc);
//	if(font!=NULL_FONT)
		dc->SetFont(theme.lookUpFont(font));
	if(color!=NULL_COLOR) 
		dc->SetTextForeground(color); //~~

	std::string t_text;
	if(mode == FORMATTED_TEXT_MODE)
		t_text = text + "#";
	else
		t_text= text;

	switch(mode)
	{
		case NO_TEXT_MODE:break;
		case VERTICALLY_CENTERED_TEXT_MODE:break;
		case RIGHT_BOUNDED_TEXT_MODE:break;
		case HORIZONTALLY_CENTERED_TEXT_MODE:break;
		case TOTAL_CENTERED_TEXT_MODE:break;
		case LOWER_CENTERED_TEXT_MODE:break;
		case UPPER_CENTERED_TEXT_MODE:break;
		case FORMATTED_TEXT_MODE:
/*		{
			unsigned int twidth = 0;
			unsigned int textCursorX = 5;
			bool bold = false;
			bool mustNotMakeNewLine = false;
			unsigned int firstCharPosition=0;
			unsigned int lastCharPosition=0;
			unsigned int currentRow=0;
			unsigned int maxdy=0;
			Size s = dc->GetTextExtent(t_text);
			maxdy = s.GetHeight();
			for(unsigned int i=0;i<t_text.length();i++)
			{
				bool newLine=false;
				if(t_text[i]=='&') {
					mustNotMakeNewLine = !mustNotMakeNewLine;

				} else 
// set new 'milestone' when reaching end of a word:
				if((t_text[i]==' ')&&(!mustNotMakeNewLine)) {
					lastCharPosition = i;
					twidth = textCursorX;
				}
// ... or a line:
				else if((t_text[i]=='#')||(i==t_text.length()-1)) {
					lastCharPosition = i;
					twidth=(textBox.GetWidth()+textCursorX)/2;
					newLine=true;
				} else
// move the textcursor forward:
				{
					std::ostringstream os;
					os << t_text[i];
					textCursorX += dc->GetTextExtent(os.str()).GetWidth() + 1;
				}
				
// reached end of line? Calculate line and draw it
				if((textCursorX>textBox.GetWidth()-5)||(newLine))
				{
					newLine = false;
					textCursorX = 5;
					int d = 1;
					if(twidth>textBox.GetWidth()-10)
						twidth=textBox.GetWidth()-10; // ~~
					if(!newLine)
						d = (textBox.GetWidth()-10-twidth)/2;
//					if(d<1)
//						d = 0;
				
					for(unsigned int j = firstCharPosition; j < lastCharPosition; j++)
					{
						std::ostringstream os;
						os << t_text[j];
						if(t_text[j]=='$')
						{
							if(bold){bold=false;dc->SetTextForeground(color);} else
								{bold=true;dc->SetTextForeground(dc->brightenColor(&color, 40));}
							continue;
						} else if((t_text[j]=='#')||(j==t_text.length()-1))
							continue;
						else if(t_text[j]=='&')
							continue;
						dc->DrawText(os.str(), textBox.GetLeft()+textCursorX+d*(j-firstCharPosition)/(lastCharPosition-firstCharPosition+1), textBox.GetTop()+currentRow);
						s = dc->GetTextExtent(os.str());
//						if(s.GetHeight() > maxdy)
//							maxdy = s.GetHeight();
						textCursorX+=s.GetWidth();
					}
					currentRow+=maxdy-4;
					twidth=0;
					firstCharPosition=lastCharPosition+1;
					i=lastCharPosition;
					lastCharPosition = 0;
					textCursorX = 5;
// this must be resetted!					
					mustNotMakeNewLine = false;
				}
			}
		}*/break;
		case FORMATTED_NON_BLOCK_TEXT_MODE:
		/*{
			unsigned int twidth = 0;
			unsigned int textCursorX = 5;
			bool bold = false;
			bool mustNotMakeNewLine = false;
			unsigned int firstCharPosition=0;
			unsigned int lastCharPosition=0;
			unsigned int currentRow=0;
			unsigned int maxdy=0;
			Size s = dc->GetTextExtent(t_text);
			maxdy = s.GetHeight();
			for(unsigned int i=0;i<t_text.length();i++)
			{
				bool newLine=false;
				if(t_text[i]=='&') {
					mustNotMakeNewLine = !mustNotMakeNewLine;
				} else 
// set new 'milestone' when reaching end of a word:
				if((t_text[i]==' ')&&(!mustNotMakeNewLine)) {
					lastCharPosition = i;
					twidth = textCursorX;
				}
// ... or a line:
				else if((t_text[i]=='#')||(i==t_text.length()-1)) {
					lastCharPosition = i;
					twidth=(textBox.GetWidth()+textCursorX)/2;
					newLine=true;
				} else
// move the textcursor forward:
				{
					std::ostringstream os;
					os << t_text[i];
					textCursorX += dc->GetTextExtent(os.str()).GetWidth() + 1;
				}
				
// reached end of line? Calculate line and draw it
				if((textCursorX>textBox.GetWidth()-5)||(newLine))
				{
					newLine = false;
					textCursorX = 5;
					int d = 1;
					if(twidth>textBox.GetWidth()-10)
						twidth=textBox.GetWidth()-10; // ~~
					if(!newLine)
						d = (textBox.GetWidth()-10-twidth)/2;
//					if(d<1)
//						d = 0;
				
					dc->DrawText(t_text.substr(firstCharPosition, lastCharPosition + 1 - firstCharPosition), textBox.GetLeft(), textBox.GetTop()+currentRow);
					s = dc->GetTextExtent(t_text.substr(firstCharPosition, lastCharPosition + 1 - firstCharPosition));
					
					currentRow+=maxdy-4;
					twidth=0;
					firstCharPosition=lastCharPosition+1;
					i=lastCharPosition;
					lastCharPosition = 0;
					textCursorX = 5;
// this must be resetted!					
					mustNotMakeNewLine = false;
				}
			}
		}*/break;

		default:break;
	}
	if((mode!=FORMATTED_TEXT_MODE)&&(mode!=FORMATTED_NON_BLOCK_TEXT_MODE))
		dc->DrawText(t_text, textBox.GetTopLeft());
}


/*void UI_StaticText::setEditable(const bool editable)
{
	this->editable = editable;
}

const bool UI_StaticText::isEditable() const
{
	return(editable);
}*/

void UI_StaticText::addChar(const unsigned int pos, const char key)
{
	std::ostringstream os;
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
	if(pos==0)
		os << text.substr(1);
	else
    if(pos==text.size()-1)
        os << text.substr(0, text.size()-1);
    else
        os << text.substr(0, pos) << text.substr(pos+1);
    updateText(os.str());
}

const std::string& UI_StaticText::getString() const
{
	return(text);
}

void UI_StaticText::process()
{
	std::string t_text;
	if(mode == FORMATTED_TEXT_MODE)
		t_text = text + "#";
	else
	{
		t_text= text;
		textBox.SetSize(theme.lookUpFont(font)->GetTextExtent(t_text));
	}

	textBox.SetTopLeft(getAbsolutePosition());
	
	switch(mode)
	{
		case NO_TEXT_MODE:break;
		case VERTICALLY_CENTERED_TEXT_MODE:
			textBox.SetTop(textBox.GetTop() + (getHeight() - textBox.GetHeight())/2 );break;
		case RIGHT_BOUNDED_TEXT_MODE: // TODO
			textBox.SetLeft(textBox.GetLeft() + getWidth() - textBox.GetWidth());break;
		case HORIZONTALLY_CENTERED_TEXT_MODE:
			textBox.SetLeft(textBox.GetLeft() + (getWidth() - textBox.GetWidth())/2);break;
		case TOTAL_CENTERED_TEXT_MODE:
			textBox.SetLeft(textBox.GetLeft() + (getWidth() - textBox.GetWidth())/2);
			textBox.SetTop(textBox.GetTop() + (getHeight() - textBox.GetHeight())/2 );break;
		case LOWER_CENTERED_TEXT_MODE:
			textBox.SetLeft(textBox.GetLeft() + (getWidth() - textBox.GetWidth())/2);
			textBox.SetTop(textBox.GetTop() + (getHeight() - textBox.GetHeight())*2/3 );break;
		case UPPER_CENTERED_TEXT_MODE:
			textBox.SetLeft(textBox.GetLeft() + (getWidth() - textBox.GetWidth())/2);
			textBox.SetTop(textBox.GetTop() + (getHeight() - textBox.GetHeight())/4 );break;
		case FORMATTED_TEXT_MODE:break;
/*		{
			unsigned int twidth = 0;
			unsigned int textCursorX = 5;
			bool bold = false;
			bool mustNotMakeNewLine = false;
			unsigned int firstCharPosition=0;
			unsigned int lastCharPosition=0;
			unsigned int currentRow=0;
			unsigned int maxdy=0;
			s = dc->GetTextExtent(t_text);
			maxdy = s.GetHeight();
			for(unsigned int i=0;i<t_text.length();i++)
			{
				bool newLine=false;
				if(t_text[i]=='&') {
					mustNotMakeNewLine = !mustNotMakeNewLine;

				} else 
// set new 'milestone' when reaching end of a word:
				if((t_text[i]==' ')&&(!mustNotMakeNewLine)) {
					lastCharPosition = i;
					twidth = textCursorX;
				}
// ... or a line:
				else if((t_text[i]=='#')||(i==t_text.length()-1)) {
					lastCharPosition = i;
					twidth=(textBox.GetWidth()+textCursorX)/2;
					newLine=true;
				} else
// move the textcursor forward:
				{
					std::ostringstream os;
					os << t_text[i];
					textCursorX += dc->GetTextExtent(os.str()).GetWidth() + 1;
				}
				
// reached end of line? Calculate line and draw it
				if((textCursorX>textBox.GetWidth()-5)||(newLine))
				{
					newLine = false;
					textCursorX = 5;
					int d = 1;
					if(twidth>textBox.GetWidth()-10)
						twidth=textBox.GetWidth()-10; // ~~
					if(!newLine)
						d = (textBox.GetWidth()-10-twidth)/2;
//					if(d<1)
//						d = 0;
				
					for(unsigned int j = firstCharPosition; j < lastCharPosition; j++)
					{
						std::ostringstream os;
						os << t_text[j];
						if(t_text[j]=='$')
						{
							if(bold){bold=false;dc->SetTextForeground(color);} else
								{bold=true;dc->SetTextForeground(dc->brightenColor(&color, 40));}
							continue;
						} else if((t_text[j]=='#')||(j==t_text.length()-1))
							continue;
						else if(t_text[j]=='&')
							continue;
						dc->DrawText(os.str(), textBox.GetLeft()+textCursorX+d*(j-firstCharPosition)/(lastCharPosition-firstCharPosition+1), textBox.GetTop()+currentRow);
						s = dc->GetTextExtent(os.str());
//						if(s.GetHeight() > maxdy)
//							maxdy = s.GetHeight();
						textCursorX+=s.GetWidth();
					}
					currentRow+=maxdy-4;
					twidth=0;
					firstCharPosition=lastCharPosition+1;
					i=lastCharPosition;
					lastCharPosition = 0;
					textCursorX = 5;
// this must be resetted!					
					mustNotMakeNewLine = false;
				}
			}
		}break;*/ // TODO
	case FORMATTED_NON_BLOCK_TEXT_MODE:
	{
		Size s;
			unsigned int twidth = 0;
			unsigned int textCursorX = 5;
			bool bold = false;
			bool mustNotMakeNewLine = false;
			unsigned int firstCharPosition=0;
			unsigned int lastCharPosition=0;
			unsigned int currentRow=0;
			unsigned int maxdy=0;
			unsigned int maxdx=0;
			for(unsigned int i=0;i<text.length();i++)
			{
				bool newLine=false;
				if(text[i]=='&') {
					mustNotMakeNewLine = !mustNotMakeNewLine;
				} else 
// set new 'milestone' when reaching end of a word:
				if((text[i]==' ')&&(!mustNotMakeNewLine)) {
					lastCharPosition = i;
					twidth = textCursorX;
				}
// ... or a line:
				else if((text[i]=='#')||(i==text.length()-1)) {
					lastCharPosition = i;
					twidth=(getWidth()+textCursorX)/2;
					newLine=true;
				} else
// move the textcursor forward:
				{
					std::ostringstream os;
					os << text[i];
					textCursorX += theme.lookUpFont(font)->GetTextExtent(os.str()).GetWidth() + 1;
					if(textCursorX>maxdx)
						maxdx=textCursorX;
				}
				
// reached end of line? Calculate line and draw it
				if((textCursorX>getWidth()-5)||(newLine))
				{
					newLine = false;
					textCursorX = 5;
					int d = 1;
					if(twidth>getWidth()-10)
						twidth=getWidth()-10; // ~~
					if(!newLine)
						d = (getWidth()-10-twidth)/2;
//					if(d<1)
//						d = 0;
					s = theme.lookUpFont(font)->GetTextExtent(text.substr(firstCharPosition, lastCharPosition + 1 - firstCharPosition));
					if(s.GetHeight() > maxdy)
						maxdy = s.GetHeight();
					textCursorX+=s.GetWidth()+1;
					if(textCursorX>maxdx)
						maxdx=textCursorX;
					currentRow+=maxdy;
					twidth=0;
					firstCharPosition=lastCharPosition+1;
					i=lastCharPosition;
					lastCharPosition = 0;
					textCursorX = 5;
//	 this must be resetted!					
					mustNotMakeNewLine = false;
				}
			} // end for
			if(currentRow==0)
				textBox.SetSize(Size(maxdx, currentRow + maxdy));
			else 
				textBox.SetSize(Size(maxdx, currentRow));
		}break;
		default:break;
	}

	if((mode!=FORMATTED_TEXT_MODE)&&(mode!=FORMATTED_NON_BLOCK_TEXT_MODE))
	{
		if(pressed)
			textBox.SetTopLeft(textBox.GetTopLeft() + Size(1, 4));
		else
			textBox.SetTopLeft(textBox.GetTopLeft() + Size(0, 3));
	}

	UI_Object::process();
/*	if(isEditable())
	{
		if(isMouseInside())
		{
			UI_Object::editTextFeld = this;
//			while(true);
		}
	}*/
	// TODO, kA was
	if(position < text.size()-1)
		position+=2;
	else position = text.size();

//	if(eText != NULL_STRING)
  //  	updateText(*theme.lookUpString(eText));
//	updateText(eText);
}

void UI_StaticText::updateText(const string& st_text)
{
	if(text == st_text)
		return;
	setNeedRedraw();
	text = st_text;
	if(mode == NO_TEXT_MODE)
		setSize(getTextSize()); // TODO size ist falsch

	position = 1;
	textMode = true;
}

void UI_StaticText::updateText(const eString st_text)
{
	if(st_text==eText)
		return;
	eText = st_text;
	updateText(*theme.lookUpString(st_text));
	textMode = false;
}

void UI_StaticText::setColor(const eColor st_color)
{
	color=*theme.lookUpColor(st_color);
}

void UI_StaticText::setColor(const Color st_color)
{
	color=st_color;
}

void UI_StaticText::setMode(const eTextMode st_mode)
{
	mode=st_mode;
}

void UI_StaticText::setFont(const eFont st_font)
{
	font=st_font;
}

