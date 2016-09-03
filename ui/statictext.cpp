#include "statictext.hpp"

#include <sstream>

UI_StaticText& UI_StaticText::operator=(const UI_StaticText& object)
{
	((UI_Object)(*this)) = ((UI_Object)object);
	updateText(object.text);
	font = object.font;
	position = object.position;
	color = object.color;
	eText = object.eText;
	textMode = object.textMode;
	pressed = object.pressed;
	textBox = object.textBox;
	return(*this);
}

UI_StaticText::UI_StaticText(const UI_StaticText& object) :
	UI_Object((UI_Object)object),
	text(),
	font(object.font),
	position(object.position),
	color(object.color),
	eText(object.eText),
	textMode(object.textMode),
	pressed(object.pressed),
	textBox(object.textBox)
{
	updateText(object.text);
}

UI_StaticText::UI_StaticText(UI_Object* st_parent, const Rect st_pos, const Size distance_bottom_right, const eColor st_color, const eFont st_font, const ePositionMode position_mode, const eAutoSize auto_size) :
	UI_Object(st_parent, st_pos, distance_bottom_right, position_mode, auto_size),
	text(),
	font(st_font),
	position(1),
	color(*theme.lookUpColor(st_color)),
	eText(NULL_STRING),
	textMode(true),
	pressed(false),
	textBox()
{ }

UI_StaticText::UI_StaticText(UI_Object* st_parent, const eString st_text, const Rect st_pos, const Size distance_bottom_right, const eColor st_color, const eFont st_font, const ePositionMode position_mode, const eAutoSize auto_size) :
	UI_Object(st_parent, st_pos, distance_bottom_right, position_mode, auto_size),
	text(),
	font(st_font),
	position(1),
	color(*theme.lookUpColor(st_color)),
	eText(NULL_STRING),
	textMode(true),
	pressed(false),
	textBox()
{
	updateText(st_text);
}

UI_StaticText::UI_StaticText(UI_Object* st_parent, const std::string& st_text, const Rect st_pos, const Size distance_bottom_right, const eColor st_color, const eFont st_font, const ePositionMode position_mode, const eAutoSize auto_size) :
	UI_Object(st_parent, st_pos, distance_bottom_right, position_mode, auto_size),
	text(),
	font(st_font),
	position(1),
	color(*theme.lookUpColor(st_color)),
	eText(NULL_STRING),
	textMode(true),
	pressed(false),
	textBox()
{
	updateText(st_text);
}

UI_StaticText::~UI_StaticText()
{ }

UI_Object* UI_StaticText::checkTooltip() {
	if( (!isShown()) || (!Rect(textBox.GetTopLeft() + getAbsolutePosition(), textBox.GetSize()).Inside(mouse )) )
		return(0);
	return((UI_Object*)this);
}

void UI_StaticText::draw(DC* dc) const
{
	if(!isShown())
		return;
	if(!checkForNeedRedraw())
		return;
//	dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
//	dc->SetPen(*theme.lookUpPen(BRIGHT_UNIT_TYPE_3_PEN));
//	if(getParent())
//		dc->DrawRectangle(getParent()->getAbsoluteRect());
//	dc->SetPen(*theme.lookUpPen(BRIGHT_UNIT_TYPE_2_PEN));
//	dc->DrawRectangle(getAbsoluteRect());

//	if(font!=NULL_FONT)
		dc->SetFont(theme.lookUpFont(font));
	if(color!=NULL_COLOR) 
		dc->SetTextForeground(color); //~~
	
	std::string t_text;
/*	if((mode == FORMATTED_TEXT_MODE)||(mode== FORMATTED_NON_BLOCK_TEXT_MODE))
		t_text = text + "#";
	else*/
		t_text= text;
#if 0
	switch(mode)
	{
		case FORMATTED_TEXT_MODE:
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
			s = dc->GetTextExtent(t_text);
			maxdy = s.GetHeight();
			for(unsigned int i=0;i<t_text.length();i++)
			{
				bool newLine=false;
				if(t_text[i]=='&') {
					mustNotMakeNewLine = !mustNotMakeNewLine;
				} else 
// set new 'milestone' when reaching end of a word:
				if((t_text[i]==' ')&&(!mustNotMakeNewLine)) 
				{
//					textCursorX += dc->GetTextExtent(t_text.substr(firstCharPosition, lastCharPosition - firstCharPosition + 1)).GetWidth() + 1;
					twidth = textCursorX;
					lastCharPosition = i;
				}
// ... or a line:
				else if((t_text[i]=='#')||(i==t_text.length()-1)) {
					lastCharPosition = i;
					twidth=(getWidth()+textCursorX)/2;
					newLine=true;
				} 
				else
				// move the textcursor forward:
				{
					std::ostringstream os;
					os << t_text[i];
					textCursorX += dc->GetTextExtent(os.str()).GetWidth() + 1;
				}

// reached end of line? Calculate line and draw it
				if((textCursorX>getWidth()-15)||(newLine))
				{
					newLine = false;
					textCursorX = 5;
					int d = 1;
					if(twidth>getWidth()-15)
						twidth=getWidth()-15; // ~~
					if(!newLine)
						d = (getWidth()-15-twidth)/2;
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
						dc->DrawText(os.str(), getAbsoluteLeftBound()+textCursorX+d*(j-firstCharPosition)/(lastCharPosition-firstCharPosition+1), getAbsoluteUpperBound()+currentRow);
						s = dc->GetTextExtent(os.str());
//						if(textBox.GetHeight() > maxdy)
//							maxdy = textBox.GetHeight();
						textCursorX+=s.GetWidth();
					}
					currentRow+=maxdy;
					twidth=0;
					firstCharPosition=lastCharPosition+1;
					i=lastCharPosition;
					lastCharPosition = 0;
					textCursorX = 5;
// this must be resetted!					
					mustNotMakeNewLine = false;
				}
			}
		}
		break;
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
					textCursorX += dc->GetTextExtent(t_text.substr(firstCharPosition, lastCharPosition - firstCharPosition)).GetWidth();
					twidth = textCursorX;
				}
// ... or a line:
				else if((t_text[i]=='#')||(i==t_text.length()-1)) 
				{
					lastCharPosition = i;
					textCursorX += dc->GetTextExtent(t_text.substr(firstCharPosition, lastCharPosition - firstCharPosition)).GetWidth();
					twidth=(getWidth()+textCursorX)/2;
					newLine=true;
				} 
//				else
				// move the textcursor forward:
//				{
//					std::ostringstream os;
//					os << t_text[i];
//					textCursorX += dc->GetTextExtent(os.str()).GetWidth() + 1;
//				}

// reached end of line? Calculate line and draw it
				if((textCursorX>getWidth()-15)||(newLine))
				{
					newLine = false;
					textCursorX = 5;
					int d = 1;
					if(twidth>getWidth()-15)
						twidth=getWidth()-15; // ~~
					if(!newLine)
						d = (getWidth()-15-twidth)/2;
//					if(d<1)
//						d = 0;
					unsigned int firstChar = firstCharPosition;
					unsigned int lastChar = firstCharPosition;

					for(unsigned int j = firstCharPosition; j <= lastCharPosition; j++)
					{
						std::ostringstream os;
						os << t_text[j];
						if((t_text[j]=='$')||(t_text[j]=='#')||(t_text[j]=='&')/*||(t_text[j]==' ')*/)
						{
							if(lastChar!=firstChar)
							{
								dc->DrawText(t_text.substr(firstChar, lastChar-firstChar), getAbsoluteLeftBound()+textCursorX/*+d*(j-firstCharPosition)/(lastCharPosition-firstCharPosition+1)*/, getAbsoluteUpperBound()+currentRow);
								s = dc->GetTextExtent(t_text.substr(firstChar, lastChar-firstChar));
//								if(textBox.GetHeight() > maxdy)
//									maxdy = textBox.GetHeight();
								textCursorX += s.GetWidth();
								firstChar = lastChar;
							}
							if(lastChar == firstChar) 
							{
								firstChar++;
								lastChar++;								
							} else 
								lastChar++;
						
							if(t_text[j]=='$')
							{
								if(bold){bold=false;dc->SetTextForeground(color);} else
									{bold=true;dc->SetTextForeground(dc->brightenColor(&color, 40));}
								continue;
							} else if((t_text[j]=='#')||(j==t_text.length()-1))
								continue;
							else if(t_text[j]=='&')
								continue;
						} else lastChar++;
					}
					if(lastChar!=firstChar)
							{
								dc->DrawText(t_text.substr(firstChar, lastChar-firstChar), getAbsoluteLeftBound()+textCursorX/*+d*(j-firstCharPosition)/(lastCharPosition-firstCharPosition+1)*/, getAbsoluteUpperBound()+currentRow);
								s = dc->GetTextExtent(t_text.substr(firstChar, lastChar-firstChar));
//								if(textBox.GetHeight() > maxdy)
//									maxdy = textBox.GetHeight();
								textCursorX += s.GetWidth();
								firstChar = lastChar;
							}
				
					currentRow+=13;
					twidth=0;
					firstCharPosition=lastCharPosition+1;
					i=lastCharPosition;
					lastCharPosition = 0;
					textCursorX = 5;
// this must be resetted!					
					mustNotMakeNewLine = false;
				}
			}
		}
		break;
		default:
#endif
		if(pressed)
			dc->DrawText(t_text, getAbsolutePosition() + textBox.GetTopLeft() + Size(1, 4));
		else
			dc->DrawText(t_text, getAbsolutePosition() + textBox.GetTopLeft() + Size(0, 3));
#if 0
		break;
	}
#endif
	UI_Object::draw(dc);
}

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

	
#if 0
	textBox.SetTopLeft(Point(0,0));//getRelativePosition());//AbsolutePosition());
	textBox.SetSize(UI_Object::theme.lookUpFont(font)->GetTextExtent(text)); //wenn das auskommentiert ist sind die Buchstaben wild.
	switch(mode)
	{
		case NO_TEXT_MODE:
			textBox.SetSize(UI_Object::theme.lookUpFont(font)->GetTextExtent(text));break;
		case VERTICALLY_CENTERED_TEXT_MODE:
			textBox.SetTop(textBox.GetTop() + (getHeight() - textBox.GetHeight())/2 );
			textBox.SetSize(UI_Object::theme.lookUpFont(font)->GetTextExtent(text));break;
		case RIGHT_BOUNDED_TEXT_MODE:
			textBox.SetLeft(textBox.GetLeft() + getWidth() - textBox.GetWidth());
			textBox.SetSize(UI_Object::theme.lookUpFont(font)->GetTextExtent(text));break;
		case HORIZONTALLY_CENTERED_TEXT_MODE:
			textBox.SetSize(UI_Object::theme.lookUpFont(font)->GetTextExtent(text));
			textBox.SetLeft(textBox.GetLeft() + (getWidth() - textBox.GetWidth())/2);break;
		case TOTAL_CENTERED_TEXT_MODE:
			textBox.SetSize(UI_Object::theme.lookUpFont(font)->GetTextExtent(text));
			textBox.SetLeft(textBox.GetLeft() + (getWidth() - textBox.GetWidth())/2);
			textBox.SetTop(textBox.GetTop() + (getHeight() - textBox.GetHeight())/2 );break;
		case LOWER_CENTERED_TEXT_MODE:
			textBox.SetSize(UI_Object::theme.lookUpFont(font)->GetTextExtent(text));
			textBox.SetLeft(textBox.GetLeft() + (getWidth() - textBox.GetWidth())/2);
			textBox.SetTop(textBox.GetTop() + (getHeight() - textBox.GetHeight())*2/3 );break;
		case UPPER_CENTERED_TEXT_MODE:
			textBox.SetSize(UI_Object::theme.lookUpFont(font)->GetTextExtent(text));
			textBox.SetLeft(textBox.GetLeft() + (getWidth() - textBox.GetWidth())/2);
			textBox.SetTop(textBox.GetTop() + (getHeight() - textBox.GetHeight())/4 );break;
		case BOTTOM_TEXT_MODE:
			textBox.SetSize(UI_Object::theme.lookUpFont(font)->GetTextExtent(text));
			textBox.SetLeft(textBox.GetLeft() + (getWidth() - textBox.GetWidth())/2);
			textBox.SetTop(100);break;//textBox.GetBottom() - 2*textBox.GetHeight());break;
		case TOP_TEXT_MODE:
			textBox.SetSize(UI_Object::theme.lookUpFont(font)->GetTextExtent(text));
			textBox.SetLeft(textBox.GetLeft() + (getWidth() - textBox.GetWidth())/2);
			textBox.SetTop(textBox.GetTop() + textBox.GetHeight() );break;	
		case FORMATTED_TEXT_MODE:
			textBox.SetSize(UI_Object::theme.lookUpFont(font)->GetTextExtent(text)); //wenn das auskommentiert ist sind die Buchstaben wild.
			break;
		case FORMATTED_NON_BLOCK_TEXT_MODE:
		{
			Size s;
			unsigned int twidth = 0;
			unsigned int textCursorX = 5;
			bool mustNotMakeNewLine = false;
			unsigned int firstCharPosition=0;
			unsigned int lastCharPosition=0;
			unsigned int currentRow=0;
			unsigned int maxdx=0;
			unsigned int maxdy=0;
			std::string t_text=text+"#";
			s = UI_Object::theme.lookUpFont(font)->GetTextExtent(t_text);
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
					textCursorX += UI_Object::theme.lookUpFont(font)->GetTextExtent(t_text.substr(firstCharPosition, lastCharPosition - firstCharPosition)).GetWidth();
					twidth = textCursorX;
				}
// ... or a line:
				else if((t_text[i]=='#')||(i==t_text.length()-1)) 
				{
					lastCharPosition = i;
					textCursorX += UI_Object::theme.lookUpFont(font)->GetTextExtent(t_text.substr(firstCharPosition, lastCharPosition - firstCharPosition)).GetWidth();
					twidth = textCursorX;
//					twidth=(getWidth()+textCursorX)/2;
					newLine=true;
				} 
//				else
				// move the textcursor forward:
//				{
//					std::ostringstream os;
//					os << t_text[i];
//					textCursorX += UI_Object::theme.lookUpFont(font)->GetTextExtent(os.str()).GetWidth() + 1;
//				}

// reached end of line? Calculate line and draw it
				if((textCursorX>getWidth()-15)||(newLine))
				{
					newLine = false;
					textCursorX = 5;
					int d = 1;
					if(twidth>getWidth()-15)
						twidth=getWidth()-15; // ~~
					if(!newLine)
						d = (getWidth()-15-twidth)/2;
//					if(d<1)
//						d = 0;
					unsigned int firstChar = firstCharPosition;
					unsigned int lastChar = firstCharPosition;

					for(unsigned int j = firstCharPosition; j <= lastCharPosition; j++)
					{
						std::ostringstream os;
						os << t_text[j];
						if((t_text[j]=='$')||(t_text[j]=='#')||(t_text[j]=='&')||(j==t_text.length()-1))
						{
							if(lastChar!=firstChar)
							{
								textBox.SetWidth(theme.lookUpFont(font)->GetTextExtent(t_text.substr(firstChar, lastChar - firstChar)).GetWidth());
							
								s = UI_Object::theme.lookUpFont(font)->GetTextExtent(t_text.substr(firstChar, lastChar+1-firstChar));
//								if(textBox.GetHeight() > maxdy)
//									maxdy = textBox.GetHeight();
								textCursorX += s.GetWidth();
								firstChar = lastChar;
							}
							if(lastChar == firstChar) 
							{
								firstChar++;
								lastChar++;								
							} else 
								lastChar++;
						
							if(t_text[j]=='$')
							{
								continue;
							} else if(t_text[j]=='#')
								continue;
							else if(t_text[j]=='&')
								continue;
						} else lastChar++;
					}
					if(lastChar!=firstChar) // TODO verkuerzen
					{
						textBox.SetWidth(theme.lookUpFont(font)->GetTextExtent(t_text.substr(firstChar, lastChar +1 - firstChar)).GetWidth());
						if(textBox.GetWidth()>maxdx)
							maxdx=textBox.GetWidth();
						firstChar = lastChar;
					}
				
					currentRow+=13;
					twidth=0;
					firstCharPosition=lastCharPosition+1;
					i=lastCharPosition;
					lastCharPosition = 0;
					textCursorX = 5;
// this must be resetted!					
					mustNotMakeNewLine = false;
				}
			}
		
			if(currentRow==13)
				textBox.SetSize(Size(textBox.GetWidth()+45, 15));
			else
				textBox.SetSize(Size(maxdx+45, currentRow+2));
		}break;		
		default:break;
	}
}
#endif
	

void UI_StaticText::process()
{
	setSize(getTextSize()); // TODO size ist falsch
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
//	if(position < text.size()-1)
//		position+=2;
//	else position = text.size();

//	if(eText != NULL_STRING)
  //  	updateText(*theme.lookUpString(eText));
//	updateText(eText);
}

void UI_StaticText::updateText(const std::string& st_text)
{
	if(text == st_text)
		return;
	setNeedRedrawMoved();
	text = st_text;
//	if(mode == NO_TEXT_MODE)
	setSize(getTextSize()); // TODO size ist falsch
	position = 1;
	textMode = true;
//	adjustPositionAndSize(ADJUST_ONLY_POSITION);//, getTextSize()); //evtl da size uebergeben anstatt size an sich zu aendern... mmmh... oder ueber originalRect :<
//	if(getParent()!=NULL)
//		getParent()->adjustPositionAndSize(ADJUST_AFTER_CHILD_SIZE_WAS_CHANGED, getTextSize());
}

void UI_StaticText::updateText(const eString st_text)
{
	if(st_text==eText)
		return; //?
	eText = st_text;
	text = *theme.lookUpString(st_text);
	updateText(*theme.lookUpString(st_text));
	textMode = false;
}

void UI_StaticText::reloadStrings()
{
	if(eText==NULL_STRING)
		return;
	updateText(*theme.lookUpString(eText));
}


