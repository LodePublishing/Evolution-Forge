#include "statictext.hpp"

#include <sstream>

UI_StaticText& UI_StaticText::operator=(const UI_StaticText& object)
{
	((UI_Object)(*this)) = ((UI_Object)object);
    mode = object.mode;
    text = object.text;
    font = object.font;
    color = object.color;
	return(*this);
}

UI_StaticText::UI_StaticText(const UI_StaticText& object) :
    UI_Object((UI_Object)object),
    mode(object.mode),
    text("ERROR"),
    font(object.font),
    color(object.color)
{
    updateText(object.text);
}

UI_StaticText::UI_StaticText(UI_Object* st_parent, const Rect st_pos, const eTextMode st_mode, const eColor st_color, const eFont st_font) :
	UI_Object(st_parent, st_pos, st_pos),
	mode(st_mode),
	text("ERROR"),
	font(st_font),
	color(*theme.lookUpColor(st_color))
{ }

UI_StaticText::UI_StaticText(UI_Object* st_parent, const eString st_text, const Rect st_pos, const eTextMode st_mode, const eColor st_color, const eFont st_font) :
	UI_Object(st_parent, st_pos, st_pos),
    mode(st_mode),
	text("ERROR"),
    font(st_font),
    color(*theme.lookUpColor(st_color))
{
	updateText(st_text);
}

UI_StaticText::UI_StaticText(UI_Object* st_parent, const string st_text, const Rect st_pos, const eTextMode st_mode, const eColor st_color, const eFont st_font) :
	UI_Object(st_parent, st_pos, st_pos),
    mode(st_mode),
    text("ERROR"),
    font(st_font),
    color(*theme.lookUpColor(st_color))
{
	updateText(st_text);
}

UI_StaticText::~UI_StaticText()
{ }

UI_Object* UI_StaticText::checkHighlight() {
	return(UI_Object::checkHighlight());
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
	if(font!=NULL_FONT)
		dc->SetFont(theme.lookUpFont(font));

//	if(color!=NULL_COLOR) 
		dc->SetTextForeground(color); //~~
	Size s = dc->GetTextExtent(text);
	Rect temp=getAbsoluteRect();
	switch(mode)
	{
		case NO_TEXT_MODE:break;
		case VERTICALLY_CENTERED_TEXT_MODE:
			temp.SetTop(temp.GetTop() + (temp.GetHeight() - s.GetHeight())/2 );break;
		case RIGHT_BOUNDED_TEXT_MODE: // TODO
			temp.SetLeft(temp.GetLeft()+temp.GetWidth()-s.GetWidth());break;
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
		    unsigned int firstChar, lastChar, twidth;
			unsigned int textCursorX = 5;
			bool bold = false;
		    firstChar=0;
		    lastChar=0;
		    twidth=0;
			unsigned int currentRow=0;
			unsigned int maxdy=0;
		    for(unsigned int i=0;i<text.length();i++)
		    {
				bool newLine=false;
		        if(text[i]==' ') {
		            lastChar=i;
					twidth=textCursorX;
		        }
	    	    else if(text[i]=='#') {
		            lastChar=i;
    		        twidth=(temp.GetWidth()+textCursorX)/2;
					newLine=true;
	        	}
				std::ostringstream os;
				os << text[i];
    	    	s = dc->GetTextExtent(os.str());os.str("");
		        textCursorX+=s.GetWidth();
		        if((textCursorX>temp.GetWidth()-5)||(newLine))
        		{
					newLine=false;
		            textCursorX=5;
        		    int d=0;
		            for(unsigned int j=firstChar;j<lastChar;j++)
        		    {
						os.str("");
						os << text[j];
		                s = dc->GetTextExtent(os.str());
		                if(text[j]=='$')
		                {
        		            if(bold){bold=false;dc->SetTextForeground(color);} else
                		        {bold=true;dc->SetTextForeground(dc->brightenColor(&color, 40));}
	        	            continue;
    		            }
		                if(text[j]=='#') break;
		                dc->DrawText(os.str(), temp.GetLeft()+textCursorX+d/(lastChar-firstChar-1), temp.GetTop()+currentRow);
						if(s.GetHeight() > maxdy)
							maxdy = s.GetHeight();
		                textCursorX+=s.GetWidth();
        		        if(!newLine)
                		    d+=(temp.GetWidth()-10-twidth);
		            }
        		    currentRow+=maxdy;
		            twidth=0;
		            firstChar=lastChar+1;
        		    i=lastChar;
		            lastChar=0;
        	    	textCursorX=5;
		        }
    		}
		}break;
		default:break;
	}
	if(mode!=FORMATTED_TEXT_MODE)
		dc->DrawText(text, temp.GetTopLeft() + Point(0,3));
}

void UI_StaticText::process()
{
	UI_Object::process();
	// TODO, kA was
}

void UI_StaticText::updateText(const string& st_text)
{
	if(text == st_text)
		return;
	setNeedRedraw();
	text = st_text;
	if(mode == NO_TEXT_MODE)
		setSize(getTextSize());
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

