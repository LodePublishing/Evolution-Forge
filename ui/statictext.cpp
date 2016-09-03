#include "statictext.hpp"

#include <sstream>

UI_StaticText& UI_StaticText::operator=(const UI_StaticText& object)
{
	((UI_Object)(*this)) = ((UI_Object)object);
    mode = object.mode;
    text = object.text;
    font = object.font;
    color = object.color;
//	editable = object.editable;
	return(*this);
}

UI_StaticText::UI_StaticText(const UI_StaticText& object) :
    UI_Object((UI_Object)object),
    mode(object.mode),
    text(object.text),
    font(object.font),
    color(object.color)
//	editable(object.editable)
{
    updateText(object.text);
}

UI_StaticText::UI_StaticText(UI_Object* st_parent, const Rect st_pos, const eTextMode st_mode, const eColor st_color, const eFont st_font) :
	UI_Object(st_parent, st_pos, st_pos),
	mode(st_mode),
	text("ERROR"),
	font(st_font),
	color(*theme.lookUpColor(st_color))
//	editable(false)
{ }

UI_StaticText::UI_StaticText(UI_Object* st_parent, const eString st_text, const Rect st_pos, const eTextMode st_mode, const eColor st_color, const eFont st_font) :
	UI_Object(st_parent, st_pos, st_pos),
    mode(st_mode),
	text("ERROR"),
    font(st_font),
    color(*theme.lookUpColor(st_color))
//	editable(false)
{
	updateText(st_text);
}

UI_StaticText::UI_StaticText(UI_Object* st_parent, const string st_text, const Rect st_pos, const eTextMode st_mode, const eColor st_color, const eFont st_font) :
	UI_Object(st_parent, st_pos, st_pos),
    mode(st_mode),
    text("ERROR"),
    font(st_font),
    color(*theme.lookUpColor(st_color))
//	editable(false)
{
	updateText(st_text);
}

UI_StaticText::~UI_StaticText()
{ }

UI_Object* UI_StaticText::checkHighlight() {
	return(UI_Object::checkHighlight());
}

const bool UI_StaticText::doneWriting() const {
	return(position>=text.size());
}

const Size UI_StaticText::getTextSize() const
{
	return(theme.lookUpFont(font)->GetTextExtent(text.c_str()));
}

void UI_StaticText::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Object::draw(dc);
	if(UI_Object::editTextFeld == this)
	{
		dc->DrawRectangle(Rect(50,50,500,500));
		//getAbsoluteRect());
	}

	if(font!=NULL_FONT)
		dc->SetFont(theme.lookUpFont(font));
//	if(color!=NULL_COLOR) 
		dc->SetTextForeground(color); //~~

	
	Size s;
	std::string t_text;
	if(mode == FORMATTED_TEXT_MODE)
		t_text= text.substr(0, position) + "#";
	else
	{
		t_text= text;//.substr(0, position);
		s = dc->GetTextExtent(t_text);
	}

		
	Rect temp=getAbsoluteRect();
	switch(mode)
	{
		case NO_TEXT_MODE:break;
		case VERTICALLY_CENTERED_TEXT_MODE:
			temp.SetTop(temp.GetTop() + (temp.GetHeight() - s.GetHeight())/2 );break;
		case RIGHT_BOUNDED_TEXT_MODE: // TODO
			temp.SetLeft(temp.GetLeft() + temp.GetWidth() - s.GetWidth());break;
		case HORIZONTALLY_CENTERED_TEXT_MODE:
			temp.SetLeft(temp.GetLeft() + (temp.GetWidth() - s.GetWidth())/2);break;
		case TOTAL_CENTERED_TEXT_MODE:
			temp.SetLeft(temp.GetLeft() + (temp.GetWidth() - s.GetWidth())/2);
			temp.SetTop(temp.GetTop() + (temp.GetHeight() - s.GetHeight())/2 );break;
		case LOWER_CENTERED_TEXT_MODE:
			temp.SetLeft(temp.GetLeft() + (temp.GetWidth() - s.GetWidth())/2);
			temp.SetTop(temp.GetTop() + (temp.GetHeight() - s.GetHeight())*2/3 );break;
		case UPPER_CENTERED_TEXT_MODE:
			temp.SetLeft(temp.GetLeft() + (temp.GetWidth() - s.GetWidth())/2);
			temp.SetTop(temp.GetTop() + (temp.GetHeight() - s.GetHeight())/4 );break;
		case FORMATTED_TEXT_MODE:

		{
		    unsigned int twidth = 0;
			unsigned int textCursorX = 5;
			bool bold = false;
			bool mustNotMakeNewLine = false;
		    unsigned int firstCharPosition=0;
		    unsigned int lastCharPosition=0;
			unsigned int currentRow=0;
			unsigned int maxdy=0;
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
	    	    else if(t_text[i]=='#') {
		            lastCharPosition = i;
    		        twidth=(temp.GetWidth()+textCursorX)/2;
					newLine=true;
	        	} else
// move the textcursor forward:
				{
					std::ostringstream os;
					os << t_text[i];
			        textCursorX += dc->GetTextExtent(os.str()).GetWidth() + 1;
				}
				
// reached end of line? Calculate line and draw it
		        if((textCursorX>temp.GetWidth()-5)||(newLine))
        		{
					newLine = false;
		            textCursorX = 5;
        		    int d = 1;
					if(!newLine)
	                	d = 1+(temp.GetWidth()-10-twidth)/2;
					if(d<1)
						d = 1;
				
		            for(unsigned int j = firstCharPosition; j < lastCharPosition; j++)
        		    {
                    	std::ostringstream os;
						os << t_text[j];
		                if(t_text[j]=='$')
		                {
        		            if(bold){bold=false;dc->SetTextForeground(color);} else
                		        {bold=true;dc->SetTextForeground(dc->brightenColor(&color, 40));}
	        	            continue;
    		            } else if(t_text[j]=='#')
							continue;
						else if(t_text[j]=='&')
							continue;
		                dc->DrawText(os.str(), temp.GetLeft()+textCursorX+d*(j-firstCharPosition)/(lastCharPosition-firstCharPosition+1), temp.GetTop()+currentRow);
		                s = dc->GetTextExtent(os.str());
						if(s.GetHeight() > maxdy)
							maxdy = s.GetHeight();
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
		}break;
		default:break;
	}
	if(mode!=FORMATTED_TEXT_MODE)
		dc->DrawText(t_text, temp.GetTopLeft() + Point(0,3));
}

/*void UI_StaticText::setEditable(const bool editable)
{
	this->editable = editable;
}

const bool UI_StaticText::isEditable() const
{
	return(editable);
}*/

void UI_StaticText::addChar(const char key)
{
	std::ostringstream os;
	os << text << key;
	updateText(os.str());
}

void UI_StaticText::removeChar()
{
	// TODO
}

void UI_StaticText::process()
{
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
}

void UI_StaticText::updateText(const eString st_text)
{
	updateText(*theme.lookUpString(st_text));
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

