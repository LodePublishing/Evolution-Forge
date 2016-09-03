#include "longtext.hpp"

#include <sstream>

UI_LongText& UI_LongText::operator=(const UI_LongText& object)
{
	((UI_Object)(*this)) = ((UI_Object)object);
	updateText(object.text);
	longText = object.longText;
	longButton = object.longButton;
	longNumber = object.longNumber;
	color1 = object.color1;
	color2 = object.color2;
	button = object.button;
	textWasChanged=true;
	return(*this);
}

UI_LongText::UI_LongText(const UI_LongText& object) :
	UI_Object((UI_Object)object),
	longText(object.longText),
	longButton(object.longButton),
	longNumber(object.longNumber),
	text(object.text),
	color1(object.color1),
	color2(object.color2),
	button(object.button),
	textWasChanged(true)
{}

UI_LongText::UI_LongText(UI_Object* lt_parent, const Rect lt_pos, const Size distance_bottom_right, const std::string& lt_text, const eColor lt_color1, const eColor lt_color2, const eFont lt_font, const eButtonColorsType lt_button, const ePositionMode position_mode, const eAutoSize auto_size) :
	UI_Object(lt_parent, lt_pos, distance_bottom_right, position_mode, auto_size),
	longText(),
	longButton(),
	longNumber(),
	text(lt_text),
	color1(lt_color1),
	color2(lt_color2),
	font(lt_font),
	button(lt_button),
	textWasChanged(true)
{}

UI_LongText::~UI_LongText()
{
	std::list<UI_StaticText*>::iterator i = longText.begin();
	while(!longText.empty())
	{
		delete(*i);
		i = longText.erase(i);
	}
	std::list<UI_Button*>::iterator j = longButton.begin();
	while(!longButton.empty())
	{
		delete(*j);
		j = longButton.erase(j);
	}
}

void UI_LongText::draw(DC* dc) const
{
	if(!isShown())
		return;
	if(!checkForNeedRedraw())
		return;
	UI_Object::draw(dc);
}

void UI_LongText::process()
{
	UI_Object::process();
	if(textWasChanged)
	{
		updateText(text);
		textWasChanged=false;
	}
}

void UI_LongText::reloadOriginalSize()
{
	reloadStrings();
	UI_Object::reloadOriginalSize();
}

void UI_LongText::updateText(const std::string& lt_text)
{
//	if(text == lt_text)
//		return; OMFG
	setNeedRedrawMoved();
	text = lt_text;
//	if(getParent()!=NULL)
//	{
//		getParent()->resetMinXY();
//		getParent()->adjustPositionAndSize(ADJUST_ONLY_POSITION);
//	}

	unsigned int textCursorX = 5;
	unsigned int firstTextCursorX = 5;
	unsigned int bold = 0;
	unsigned int textbutton = 0;
	bool mustNotMakeNewLine = false;
	unsigned int firstCharPosition=0;
	unsigned int lastCharPosition=0;
	unsigned int currentRow=0;
	unsigned int maxdy=0;

	maxdy = UI_Object::theme.lookUpFont(SMALL_BOLD_FONT)->GetTextExtent(text).GetHeight()*13/10;
	
	eColor current_color = color1;
	eFont current_font = font;

	{
		std::list<UI_StaticText*>::iterator i = longText.begin();
		while(!longText.empty())
		{
			delete(*i);
			i = longText.erase(i);
		}	
		std::list<UI_Button*>::iterator j = longButton.begin();
		while(!longButton.empty())
		{
			delete(*j);
			j = longButton.erase(j);
		}
		std::list<signed int>::iterator k = longNumber.begin();
		while(!longNumber.empty())
			k = longNumber.erase(k);
		UI_Button::resetButton();
	}
	
	
	for(unsigned int i=0;i<text.length();i++)
	{
		bool newLine=false;
		bool newCheck = false;
//		if(text[i]=='&') {
//			mustNotMakeNewLine = !mustNotMakeNewLine;
//		} else 
// set new 'milestone' when reaching end of a word:
		if((text[i]==' '))//&&(!mustNotMakeNewLine)) 
		{
			lastCharPosition = i;
		}
// ... or a line:
		else if(text[i]=='#') {
			lastCharPosition = i;
			newLine=true;
		} 
		else if(i==text.length()-1) {
			lastCharPosition = i+1;
			newLine=true;
		} 
	
// bold:
		else if(text[i]=='$')
		{
			bold++;
			lastCharPosition = i;
			newCheck = true;
// button:			
		} else if(text[i]=='@')
		{
			textbutton++;
			lastCharPosition = i;
			newCheck = true;
		} else
		// move the textcursor forward:
		{
			std::ostringstream os;
			os.str("");
			os << text[i];
			textCursorX += UI_Object::theme.lookUpFont(current_font)->GetTextExtent(os.str()).GetWidth();
		}
// reached end of line? Calculate line and draw it
		if((textCursorX>getWidth()-15)||(newLine)||(newCheck))
		{
			newCheck = false;
			
			if(bold==2){bold=0;current_color = color2;current_font = (eFont)(font+1);} else
			{
				current_color = color1;
				current_font = font;
			}
			std::ostringstream realstring;
			realstring.str("");
			for(unsigned int j = firstCharPosition; j < lastCharPosition; j++)
			{
				if((text[j]=='$')||(text[j]=='#')||(text[j]=='&')||(text[j]=='@'))
					continue;
				realstring << text[j];
			}
			if(realstring.str().length()>0)
			{
			
			textCursorX = firstTextCursorX + UI_Object::theme.lookUpFont(current_font)->GetTextExtent(realstring.str()).GetWidth() ;
			unsigned int height = UI_Object::theme.lookUpFont(current_font)->GetTextExtent(realstring.str()).GetHeight();
			if(textbutton==3)
			{
				textbutton=0;
				longNumber.push_back(atoi(realstring.str().c_str()));
			} else
			if(textbutton==2)
			{
				UI_Button* t = new UI_Button(this, Rect(Point(firstTextCursorX, currentRow-5), Size(0, 0)), Size(0, 0), realstring.str(), button, PRESS_BUTTON_MODE, DO_NOT_ADJUST, (eFont)(current_font+1), AUTO_SIZE);
				longButton.push_back(t);
			} else
			{
				UI_StaticText* t = new UI_StaticText(this, realstring.str(), Rect(firstTextCursorX, currentRow+(maxdy-height)/2, 0,0), Size(0,0), current_color, current_font, DO_NOT_ADJUST);
				longText.push_back(t);
			}
			}
			firstCharPosition = lastCharPosition+1;
//			i = firstCharPosition;
			firstTextCursorX = textCursorX;

			if((newLine)||(textCursorX>getWidth()-15))
			{
				newLine = false;
				currentRow+=maxdy;
				lastCharPosition = 0;
				firstTextCursorX = 5;
				textCursorX = 5;
// this must be resetted!
				mustNotMakeNewLine = false;
			}
		}
	}

	textWasChanged=true;
}

const signed int UI_LongText::getPressed() const
{
	std::list<UI_Button*>::const_iterator i = longButton.begin();
	std::list<signed int>::const_iterator j = longNumber.begin();
	while(i!=longButton.end())
	{
		if((*i)->isLeftClicked())
			return(*j);
		++j;
		++i;
	}
	return(-1);
}

void UI_LongText::reloadStrings()
{
	updateText(text);
}


