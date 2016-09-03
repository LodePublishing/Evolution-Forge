#include "longtext.hpp"

#include <sstream>

UI_LongText::UI_LongText(UI_Object* lt_parent, const Rect lt_pos, const Size distance_bottom_right, const std::string& lt_text, const eColor lt_color1, const eColor lt_color2, const eFont lt_font, const eButtonColorsType lt_button, const eButtonColorsType lt_visited_button, UI_Object* scroll_bar, const bool use_help_chapters, const ePositionMode position_mode, const eAutoSize auto_size) :
	UI_Object(lt_parent, lt_pos, distance_bottom_right, position_mode, auto_size),
	scrollBar(scroll_bar),
	longText(),
	longButton(),
	longNumber(),
	longBitmap(),
	text(lt_text),
	color1(lt_color1),
	color2(lt_color2),
	font(lt_font),
	button(lt_button),
	visitedButton(lt_visited_button),
	textWasChanged(true),
	useHelpChapters(use_help_chapters)
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
	std::list<UI_Bitmap*>::iterator k = longBitmap.begin();
	while(!longBitmap.empty())
	{
		delete(*k);
		k = longBitmap.erase(k);
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


	std::list<UI_StaticText*>::iterator i = longText.begin();
	while(i!=longText.end())
	{
		if((*i)->checkForNeedRedraw())
		{
			setNeedRedrawMoved();
			return;
		}
		++i;
	}
	std::list<UI_Button*>::iterator j = longButton.begin();
	while(j!=longButton.end())
	{
		if((*j)->checkForNeedRedraw())
		{
			setNeedRedrawMoved();
			return;
		}
		++j;
	}
	std::list<UI_Bitmap*>::iterator k = longBitmap.begin();
	while(k!=longBitmap.end())
	{
		if((*k)->checkForNeedRedraw())
		{
			setNeedRedrawMoved();
			return;
		}
		++k;
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
//		getParent()->adjustPositionAndSize(ADJUST_ONLY_POSITION);
//	}

	unsigned int text_cursor_x = 5;
	unsigned int first_text_cursor_x = 5;
	unsigned int bold = 0;
	unsigned int textbutton = 0;
	unsigned int bitmap = 0;
	bool must_not_make_new_line = false;
	unsigned int first_char_position=0;
	unsigned int last_char_position=0;
	signed int current_row=1;
	signed int maxdy=0;
	unsigned int bitmap_left_border = 5;
	unsigned int bitmap_lower_border = 0; // TODO, funzt nur mit Bildern links oben...

	Point p;
	if(scrollBar!=NULL)
		p = getRelativePosition();
			
	maxdy = UI_Object::theme.lookUpFont(font)->getTextExtent(text).getHeight();
	
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
	
		std::list<UI_Bitmap*>::iterator l = longBitmap.begin();
		while(!longBitmap.empty())
		{
			delete(*l);
			l = longBitmap.erase(l);
		}
	
	}

	
	for(unsigned int i=0;i<text.length();i++)
	{
		bool new_line=false;
		bool new_check = false;
		bool end_of_order=false;
//		if(text[i]=='&') {
//			must_not_make_new_line = !must_not_make_new_line;
//		} else 
// set new 'milestone' when reaching end of a word:
		if((text[i]==' ')||(text[i]=='/')||(text[i]==','))//&&(!must_not_make_new_line)) 
		{
			last_char_position = i;
			text_cursor_x += UI_Object::theme.lookUpFont(current_font)->getTextExtent(" ").getWidth();
		}
// ... or a line:
		else if(text[i]=='#') {
			last_char_position = i;
			new_line=true;
			end_of_order=true;
		} 
		else if(i==text.length()-1) {
			last_char_position = i+1;
			new_line=true;
		} 
	
// bold:
		else if(text[i]=='$')
		{
			bold++;
			last_char_position = i;
			new_check = true;
			end_of_order=true;
// button:			
		} else if(text[i]=='@')
		{
			textbutton++;
			last_char_position = i;
			new_check = true;
			end_of_order=true;
// bitmap			
		} else if(text[i]=='~')
		{
			bitmap++;
			last_char_position = i;
			new_check = true;
			end_of_order=true;
		} else
		// move the textcursor forward:
		{
			std::ostringstream os;
			os.str("");
			os << text[i];
			if(textbutton==0)
				text_cursor_x += UI_Object::theme.lookUpFont(current_font)->getTextExtent(os.str()).getWidth();
			else
				text_cursor_x += UI_Object::theme.lookUpFont((eFont)(font+3))->getTextExtent(os.str()).getWidth();
		}
		// TODO zeilenuebergreifende BOLDs und TEXTBUTTONs
// reached end of line? Calculate line and draw it
			
		if((text_cursor_x>getWidth()-10)||(new_line)||(new_check))
		{
			new_check = false;
			
			if(((bold==1)&&(text_cursor_x>getWidth()-10))||(bold==2))
			{
				if(bold==2)
					bold=0;
				current_color = color2;
				current_font = (eFont)(font+1);
			} else
			{
				current_color = color1;
				current_font = font;
			}
			std::ostringstream realstring;
			realstring.str("");
			for(unsigned int j = first_char_position; j < last_char_position; j++)
			{
				if((text[j]=='$')||(text[j]=='#')||(text[j]=='&')||(text[j]=='@')||(text[j]=='~'))//||(text[j]<32))
				// Sonderzeichen!!!
					continue;
				realstring << text[j];
			}
			if(realstring.str().length()>0)
			{
				if((textbutton==3)||(textbutton==5))
				{
					int this_number = atoi(realstring.str().c_str());
					longNumber.push_back(this_number);
					if(textbutton==5)
						longNumber.push_back(this_number); // ok... wenn button ueber zwei Zeilen geht
					// visited button?

					if((useHelpChapters) && (uiConfiguration.isVisitedHelpChapter(this_number)))
					{
						std::list<UI_Button*>::iterator i = longButton.end();
						--i;
						if(textbutton==5) --i;
						(*i)->setButtonColorsType(visitedButton);++i;
						if(textbutton==5)
							(*i)->setButtonColorsType(visitedButton);
					}

					text_cursor_x = first_text_cursor_x;
					textbutton=0;
				} else if((textbutton==2)||(textbutton==4))
				{
					unsigned int height = UI_Object::theme.lookUpFont((eFont)(font+3))->getTextExtent(realstring.str()).getHeight();
					unsigned int ty;
					if(current_row+maxdy/2 < height/2)
						ty = 0;
					else ty = current_row+maxdy/2-height/2+2;
					UI_Button* t = new UI_Button(scrollBar==NULL?this:scrollBar, Rect(p+Point(first_text_cursor_x-1, ty), Size(0, 0)), Size(0, 0), button, false, PRESS_BUTTON_MODE, realstring.str(), DO_NOT_ADJUST, (eFont)(font+3), AUTO_SIZE);
					longButton.push_back(t);
					text_cursor_x = first_text_cursor_x + t->getTextWidth();
				} else if((textbutton==1)&&(!end_of_order))
				{
					// abgebrochener button
					if(text_cursor_x>getWidth()-10)
					{
						unsigned int height = UI_Object::theme.lookUpFont((eFont)(font+3))->getTextExtent(realstring.str()).getHeight();
						unsigned int ty;
						if(current_row+maxdy/2 < height/2)
							ty = 0;
						else ty = current_row+maxdy/2-height/2+2;
						UI_Button* t = new UI_Button(scrollBar==NULL?this:scrollBar, Rect(p+Point(first_text_cursor_x-1, ty), Size(0, 0)), Size(0, 0), button, false, PRESS_BUTTON_MODE, realstring.str(), DO_NOT_ADJUST, (eFont)(font+3), AUTO_SIZE);
						longButton.push_back(t);
						textbutton=3;
					} else continue;
				} else if(bitmap==2)
				{
					bitmap=0;
					eBitmap bitmap_number = UI_Object::theme.getBitmapFromIdentifier(realstring.str());
					if(bitmap_number == NULL_BITMAP)
						toErrorLog("WARNING (UI_LongText::updateText()): Bitmap identifier " + realstring.str() + " not recognized => ignoring bitmap.");
					else
					{
						UI_Bitmap* t = new UI_Bitmap(scrollBar==NULL?this:scrollBar, Rect(p+Point(first_text_cursor_x, current_row), Size(0, 0)), Size(0, 0), bitmap_number);
						if(t->getBitmapHeight() < maxdy)
							t->setOriginalPosition(t->getOriginalPosition() + Size(0, 1+(maxdy - t->getBitmapHeight())/2)); //?
						longBitmap.push_back(t);
						text_cursor_x-= UI_Object::theme.lookUpFont(current_font)->getTextExtent(realstring.str()).getWidth();
						// TODO... ein Zaehler mit linkerRand und linkerRandEnde machen
						text_cursor_x+=t->getBitmapWidth();
						bitmap_left_border = text_cursor_x;
						bitmap_lower_border = current_row+t->getBitmapHeight();
					}
				} else
				{
					unsigned int height = UI_Object::theme.lookUpFont(current_font)->getTextExtent(realstring.str()).getHeight();
					unsigned int ty;
					if(current_row+maxdy/2 < height/2)
						ty = 0;
					else ty = current_row+maxdy/2-height/2+2;
					UI_StaticText* t = new UI_StaticText(scrollBar==NULL?this:scrollBar, realstring.str(), Rect(p+Point(first_text_cursor_x, ty), Size(0,0)), Size(0,0), current_color, current_font, DO_NOT_ADJUST);
					longText.push_back(t);
					text_cursor_x = first_text_cursor_x + UI_Object::theme.lookUpFont(current_font)->getTextExtent(realstring.str()).getWidth() ;
				}
			}
			first_char_position = last_char_position;
			first_text_cursor_x = text_cursor_x;

			if((new_line)||(text_cursor_x>getWidth()-10))
			{
				new_line = false;
				current_row+=maxdy;
				last_char_position = 0;
				if(current_row >= bitmap_lower_border)
					bitmap_left_border = 5;
				first_text_cursor_x = bitmap_left_border;
				text_cursor_x = bitmap_left_border;
// this must be resetted!
				must_not_make_new_line = false;
			}
		}
	}
//	textWasChanged=true; ?
}

const unsigned int UI_LongText::getTextHeight() const
{
	unsigned int text_height = 0;
	if((longText.size()>0)&&(text_height < longText.back()->getRelativeLowerBound()))
		text_height = longText.back()->getRelativeLowerBound();
	if((longButton.size()>0)&&(text_height < longButton.back()->getRelativeLowerBound()))
		text_height = longButton.back()->getRelativeLowerBound();
	if((longBitmap.size()>0)&&(text_height < longBitmap.back()->getRelativeLowerBound()))
		text_height = longBitmap.back()->getRelativeLowerBound();
	return(text_height);
}

const unsigned int UI_LongText::getTextWidth() const
{
	unsigned int text_width = 0;

	for(std::list<UI_StaticText*>::const_iterator i = longText.begin(); i != longText.end(); ++i)
		if(text_width < (*i)->getRelativeRightBound())
			text_width = (*i)->getRelativeRightBound();
	
	for(std::list<UI_Button*>::const_iterator i = longButton.begin(); i != longButton.end(); ++i)
		if(text_width < (*i)->getRelativeRightBound())
			text_width = (*i)->getRelativeRightBound();
	
	for(std::list<UI_Bitmap*>::const_iterator i = longBitmap.begin(); i != longBitmap.end(); ++i)
		if(text_width < (*i)->getRelativeRightBound())
			text_width = (*i)->getRelativeRightBound();

	return(text_width);
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

