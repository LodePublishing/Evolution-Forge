#include "UI_Button.h"
#include "math.h"

UI_Radio::UI_Radio(UI_Object* parent):UI_Object(parent)
{
	changed=false;
};

UI_Radio::~UI_Radio()
{
};

void UI_Radio::addButton(UI_Button* button)
{
	button->setParent(this);
};

bool UI_Radio::hasChanged()
{
	return(changed);
};

int UI_Radio::getMarked()
{
	UI_Button* tmp=(UI_Button*)getChildren();
	int i=0;
	do
	{
		if(tmp->isCurrentlyPressed())
		{
			return(i);
		}
		i++;
		tmp=(UI_Button*)tmp->getNextBrother();
	} while(tmp!=getChildren());
	return(0);
};

void UI_Radio::draw(wxDC* dc)
{
	UI_Object::draw(dc);
};

void UI_Radio::process()
{
	if(!getChildren())
		return;
	changed=false;
	bool old[10];
	for(int k=10;k--;)
		old[k]=false;
    UI_Button* tmp=(UI_Button*)getChildren();
    int i=0;
    do
    {
        if(tmp->isCurrentlyPressed())
			old[i]=true;
		else old[i]=false;
        i++;
        tmp=(UI_Button*)tmp->getNextBrother();
    } while(tmp!=(UI_Button*)getChildren());

	UI_Object::process();

    tmp=(UI_Button*)getChildren();
    i=0;
    do
    {
        if(tmp->isCurrentlyPressed()&&(!old[i]))
        {
			changed=true;
			UI_Button* tmp2=(UI_Button*)getChildren();
			int j=0;
			do
			{
				if(j!=i)
					tmp2->forceUnpress();
				j++;
				tmp2=(UI_Button*)tmp2->getNextBrother();
			} while(tmp2!=(UI_Button*)getChildren());
			return;
        }
	  	else if((!tmp->isCurrentlyPressed())&&(old[i]))
		{
			tmp->forcePressed();
		}
        i++;
        tmp=(UI_Button*)tmp->getNextBrother();
    } while(tmp!=(UI_Button*)getChildren());
};

UI_Button::UI_Button(UI_Object* parent, wxRect rect, wxRect maxRect, eString normalText, eString pressedText, eButton button, eButtonMode buttonMode, ePositionMode mode, eFont font, eAutoSize autoSize):UI_Object(parent, rect, maxRect)
{
	statusFlags=0;
	firstPressed=false;
	hasToolTip=false;
	hasBitmap=false;
	frameNumber=0;
	timeStamp=0;

	switch(buttonMode)
	{
		case NO_BUTTON_MODE:statusFlags |= BF_NOT_CLICKABLE;break;
		case STATIC_BUTTON_MODE:statusFlags |= BF_STATIC;break;
		case PRESS_BUTTON_MODE:break;
		case PUSH_BUTTON_MODE:statusFlags |= BF_REPEATS;break;
		default:break;
	}; 
						   
	this->currentSize=rect;
	this->autoSize=autoSize;
	this->mode=mode;
	this->button=button;
	this->font=font;
	this->normalText=new UI_StaticText(this, normalText, wxRect(0,0,0,0), HORIZONTALLY_CENTERED_TEXT_MODE, font, theme.lookUpButtonAnimation(button)->startTextColour[PRESSED_BUTTON_PHASE]);
	this->pressedText=new UI_StaticText(this, pressedText, wxRect(0,0,0,0), HORIZONTALLY_CENTERED_TEXT_MODE, font, theme.lookUpButtonAnimation(button)->startTextColour[PRESSED_BUTTON_PHASE]);

//	for(int i=DISABLED_BUTTON_PHASE;i<MAX_BUTTON_ANIMATION_PHASES;i++)
//		text[i]=new UI_StaticText(this, normalText, wxRect(0,0,0,0), HORIZONTALLY_CENTERED_TEXT_MODE, font, theme.lookUpButtonAnimation(button)->startTextColour[i]);
	adjustButtonSize(this->normalText->getSize());
};

UI_Button::UI_Button(UI_Object* parent, wxRect rect, wxRect maxRect, wxString normalText, wxString pressedText, eButton button, eButtonMode buttonMode, ePositionMode mode, eFont font, eAutoSize autoSize):UI_Object(parent, rect, maxRect)
{
	statusFlags=0;
	firstPressed=false;
	hasToolTip=false;
	hasBitmap=false;
	frameNumber=0;
	timeStamp=0;

	switch(buttonMode)
	{
		case NO_BUTTON_MODE:statusFlags |= BF_NOT_CLICKABLE;break;
		case STATIC_BUTTON_MODE:statusFlags |= BF_STATIC;break;
		case PRESS_BUTTON_MODE:break;
		case PUSH_BUTTON_MODE:statusFlags |= BF_REPEATS;break;
		default:break;
	}; 
						   
	this->currentSize=rect;
	this->autoSize=autoSize;
	this->mode=mode;
	this->button=button;
	this->font=font;
	this->normalText=new UI_StaticText(this, normalText, wxRect(0,0,0,0), HORIZONTALLY_CENTERED_TEXT_MODE, font, theme.lookUpButtonAnimation(button)->startTextColour[PRESSED_BUTTON_PHASE]);
	this->pressedText=new UI_StaticText(this, pressedText, wxRect(0,0,0,0), HORIZONTALLY_CENTERED_TEXT_MODE, font, theme.lookUpButtonAnimation(button)->startTextColour[PRESSED_BUTTON_PHASE]);

//	for(int i=DISABLED_BUTTON_PHASE;i<MAX_BUTTON_ANIMATION_PHASES;i++)
//		text[i]=new UI_StaticText(this, normalText, wxRect(0,0,0,0), HORIZONTALLY_CENTERED_TEXT_MODE, font, theme.lookUpButtonAnimation(button)->startTextColour[i]);
	adjustButtonSize(this->normalText->getSize());
};

// -> bitmap button!
UI_Button::UI_Button(UI_Object* parent, wxRect rect, wxRect maxRect, eButton button, eButtonMode buttonMode, ePositionMode mode):UI_Object(parent, rect, maxRect)
{
	statusFlags=0;
	firstPressed=false;
	hasToolTip=false;
	hasBitmap=true;
	frameNumber=0;
	timeStamp=0;

	switch(buttonMode)
	{
		case STATIC_BUTTON_MODE:statusFlags |= BF_STATIC;break;
		case PRESS_BUTTON_MODE:break;
		case PUSH_BUTTON_MODE:statusFlags |= BF_REPEATS;break;
		default:break;
	}; 
						   
	this->currentSize=rect;
	this->autoSize=NO_AUTO_SIZE;
	this->mode=mode;
	this->button=button;
	this->font=NULL_FONT;
	this->normalText=0;
	this->pressedText=0;
};

UI_Button::~UI_Button()
{
	if(normalText)
		delete normalText;
	if(pressedText)
		delete pressedText;
};

void UI_Button::adjustButtonSize(wxSize size)
{
   	currentSize.SetSize(size+wxSize(5,0));

	switch(autoSize)
	{
		case NO_AUTO_SIZE:currentSize.SetSize(getSize());break;
		case AUTO_SIZE:break;
		case AUTO_HEIGHT_FULL_WIDTH:currentSize.SetWidth(getWidth()-5);break;
	};

	currentSize.SetPosition(getRelativePosition());

	switch(mode)
	{
		case DO_NOT_ADJUST:break;
		case TOTAL_CENTERED:currentSize.SetPosition(getRelativePosition()+wxPoint((getWidth() + currentSize.GetWidth())/2, (getHeight() + currentSize.GetHeight())/2));break;
		case HORIZONTAL_CENTERED:currentSize.SetX(getRelativeUpperBound()+(getWidth() - currentSize.GetWidth())/2);break;
		case VERTICALLY_CENTERED:currentSize.SetY(getRelativeUpperBound()+(getHeight() - currentSize.GetHeight())/2);break;
		case TOP_LEFT:break;
		case TOP_CENTER:currentSize.SetPosition(getRelativePosition()+wxPoint((getWidth() + currentSize.GetWidth())/2, 0));break;
		case TOP_RIGHT:currentSize.SetPosition(getRelativePosition()+wxPoint(getWidth() - currentSize.GetWidth(), 0));break;
		case CENTER_RIGHT:currentSize.SetPosition(getRelativePosition()+wxPoint(getWidth() - currentSize.GetWidth(), (getHeight() + currentSize.GetHeight())/2));break;
		case BOTTOM_RIGHT:currentSize.SetPosition(getRelativePosition()+wxPoint(getWidth() - currentSize.GetWidth(), getHeight() - currentSize.GetHeight()));break;
		case BOTTOM_CENTER:currentSize.SetPosition(getRelativePosition()+wxPoint((getWidth() - currentSize.GetWidth())/2, getHeight() - currentSize.GetHeight()));break;
		case BOTTOM_LEFT:currentSize.SetPosition(getRelativePosition()+wxPoint(0, getHeight() - currentSize.GetHeight()));break;
		case CENTER_LEFT:currentSize.SetPosition(getRelativePosition()+wxPoint(0, (getHeight() + currentSize.GetHeight())/2));break;

		case ARRANGE_TOP:currentSize.SetPosition(getRelativePosition()+wxPoint(getParent()->min_top_x, 0));getParent()->min_top_x+=currentSize.GetWidth()+MIN_DISTANCE;break;
		case ARRANGE_BOTTOM:currentSize.SetPosition(getRelativePosition()+wxPoint(getParent()->min_bottom_x, getHeight() - currentSize.GetHeight()));getParent()->min_bottom_x+=currentSize.GetWidth()+MIN_DISTANCE;break;
		case ARRANGE_LEFT:currentSize.SetPosition(getRelativePosition()+wxPoint(0, getParent()->min_left_y));getParent()->min_left_y+=currentSize.GetHeight()+MIN_DISTANCE;break;
		case ARRANGE_RIGHT:currentSize.SetPosition(getRelativePosition()+wxPoint(getWidth() - currentSize.GetWidth(), getParent()->min_right_y));getParent()->min_right_y+=currentSize.GetHeight()+MIN_DISTANCE;break;
	};

};

void UI_Button::updatePressedText(wxString utext)
{
	pressedText->updateText(utext);
};

void UI_Button::updateNormalText(wxString utext)
{
	normalText->updateText(utext);
};

// Render button.  How it draws exactly depends on it's current state.
void UI_Button::draw(wxDC* dc)
{
	if(!shown)
		return;
	eButtonAnimationPhase animation_phase;
	if(disabledFlag)
		animation_phase=DISABLED_BUTTON_PHASE;
	else 
	if(statusFlags & BF_DOWN)
	{
		if(statusFlags & BF_HIGHLIGHTED)
			animation_phase=PRESSED_HIGHLIGHTED_BUTTON_PHASE;
		else
		animation_phase=PRESSED_BUTTON_PHASE;
		currentSize.SetPosition(currentSize.GetPosition()+wxPoint(1,1));
	}
	else
	if(statusFlags & BF_HIGHLIGHTED)
		animation_phase=HIGHLIGHT_BUTTON_PHASE;
	else animation_phase=NORMAL_BUTTON_PHASE;

	setAbsoluteCoord(currentSize.GetPosition() - getRelativePosition());

	if(!(statusFlags & BF_NOT_CLICKABLE))
	{
		dc->SetBrush(*wxTRANSPARENT_BRUSH);
		dc->SetPen(*theme.lookUpPen(DARK_BACKGROUND_PEN));
		dc->DrawRectangle(wxRect(getAbsolutePosition()-wxPoint(1,1), currentSize.GetSize()+wxSize(2,2)));

		dc->SetBrush(*wxBLACK_BRUSH);
		dc->SetPen(*theme.lookUpPen(RECTANGLE_PEN));
		dc->DrawRectangle(wxRect(getAbsolutePosition(), currentSize.GetSize()));
	}

	int gradient=0;
	switch(theme.lookUpButtonAnimation(button)->type)
	{
    	case NO_ANIMATION:gradient=100;break;
		case JUMPY_COLOURS_ANIMATION:gradient=(frameNumber%theme.lookUpButtonAnimation(button)->speed)*100/theme.lookUpButtonAnimation(button)->speed;break;
		case GLOWING_ANIMATION:gradient=50*(sin(3.141*frameNumber/theme.lookUpButtonAnimation(button)->speed)+1);break;
		case BLINKING_ANIMATION:if(frameNumber<theme.lookUpButtonAnimation(button)->speed/2) gradient=100;else gradient=0;break;
		default:break;
	};
	dc->SetPen(theme.lookUpMixedPen(theme.lookUpButtonAnimation(button)->startBorderPen[animation_phase], theme.lookUpButtonAnimation(button)->endBorderPen[animation_phase], gradient));

	if(!hasBitmap)
	{
		dc->SetBrush(theme.lookUpMixedBrush(theme.lookUpButtonAnimation(button)->startBrush[animation_phase], theme.lookUpButtonAnimation(button)->endBrush[animation_phase], gradient));
		//dc->SetPen(*wxTRANSPARENT_PEN);
		if(statusFlags & BF_DOWN)
			pressedText->setColour(theme.lookUpMixedColour(theme.lookUpButtonAnimation(button)->startTextColour[animation_phase], theme.lookUpButtonAnimation(button)->endTextColour[animation_phase], gradient));
		else 
			normalText->setColour(theme.lookUpMixedColour(theme.lookUpButtonAnimation(button)->startTextColour[animation_phase], theme.lookUpButtonAnimation(button)->endTextColour[animation_phase], gradient));
	// TODO TAB-BUTTON MODE
		dc->DrawRectangle(wxRect(getAbsolutePosition(), currentSize.GetSize()));
	}
	else 
	{
		dc->SetBrush(*wxTRANSPARENT_BRUSH);
		dc->DrawRectangle(wxRect(getAbsolutePosition()-wxPoint(1,1),currentSize.GetSize()+wxSize(2,2)));
		dc->DrawBitmap(*theme.lookUpBitmap(theme.lookUpButtonAnimation(button)->bitmap[animation_phase]), getAbsolutePosition());
	}
	
	if((animation_phase==PRESSED_BUTTON_PHASE)||(animation_phase==PRESSED_HIGHLIGHTED_BUTTON_PHASE))
	{
		dc->SetPen(wxPen(wxColour(theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH)->GetColour()),1,wxSOLID));
		dc->DrawLine(getAbsolutePosition() + wxPoint(0, currentSize.GetHeight()-1), getAbsolutePosition() + wxPoint(currentSize.GetWidth(), currentSize.GetHeight()-1));
		dc->DrawLine(getAbsolutePosition() + wxPoint(currentSize.GetWidth()-1, 0), getAbsolutePosition() + wxPoint(currentSize.GetWidth()-1, currentSize.GetHeight()));
	} 

	UI_Object::draw(dc);	
/*    int offset, frame_num = -1;

    if (uses_bmaps) {
        gr_reset_clip();
        // if button is down, draw it that way
        if (button_down()) {
            if (bmap_ids[B_PRESSED] >= 0){
                frame_num = B_PRESSED;
            }
        // otherwise if button is disabled, draw it that way
        } else if (disabled_flag) {
            if (bmap_ids[B_DISABLED] >= 0){
                frame_num = B_DISABLED;
            }
        // otherwise, if button is highlighted (mouse is over it, but mouse buttons not down) draw it that way
        } else if (statusFlags & BF_HIGHLIGHTED) {
            if (bmap_ids[B_HIGHLIGHT] >= 0){
                frame_num = B_HIGHLIGHT;
            }
        // otherwise, just draw it normally
        } else {
            if (bmap_ids[B_NORMAL] >= 0){
                frame_num = B_NORMAL;
            }
        }
                                                                                                                                                            
        if (frame_num >= 0) {
            gr_set_bitmap(bmap_ids[frame_num]);
            gr_bitmap(x, y);
        }
    } else {
        gr_set_font(my_wnd->f_id);
        gr_set_clip( x, y, w, h );
                                                                                                                                                            
        // draw the button's box
        if (button_down()) {
            ui_draw_box_in( 0, 0, w-1, h-1 );
            offset = 1;
                                                                                                                                                            
        } else {
            ui_draw_box_out( 0, 0, w-1, h-1 );
            offset = 0;
        }
                                                                                                                                                            
        // now draw the button's text
        if (disabled_flag){
            gr_set_color_fast(&CDARK_GRAY);
        } else if (my_wnd->selected_gadget == this){
            gr_set_color_fast(&CBRIGHT_GREEN);
        } else {
            gr_set_color_fast(&CBLACK);
        }
                                                                                                                                                            
        if (text){
            ui_string_centered( Middle(w) + offset, Middle(h) + offset, text );
        }
                                                                                                                                                            
        gr_reset_clip();
    }*/

// if statusFlags & BF_HIGHLIGHTED 
// maybeShowToolTip()

	if(statusFlags & BF_DOWN)
		currentSize.SetPosition(currentSize.GetPosition()-wxPoint(1,1));
}



//void UI_Button::set_hotkey_if_focus(int key)
//{
//    hotkey_if_focus = key;
//}
                                                                               
// reset anything that needs to be at the start of a new frame before processing
void UI_Button::frameReset()
{
    statusFlags &= ~BF_HIGHLIGHTED;
	if(!(statusFlags & BF_STATIC))
	    statusFlags &= ~BF_DOWN;
    statusFlags &= ~BF_JUST_PRESSED;
    statusFlags &= ~BF_JUST_RELEASED;
    statusFlags &= ~BF_CLICKED;
    statusFlags &= ~BF_DOUBLE_CLICKED;
    statusFlags &= ~BF_JUST_HIGHLIGHTED;
                                                                               
//    restorePreviousCursor(); TODO... wegen DC
//	  hideToolTip(); ~~
}
                                                                               

// process all kind of messages

void UI_Button::process()
{
	// first check getChildren()
	UI_Object::process();

	wxRect r = wxRect(getParent()->getAbsolutePosition() + currentSize.GetPosition(), currentSize.GetSize());	
	int condition;
	if(statusFlags & BF_DOWN)
		r.SetPosition(r.GetPosition()+wxPoint(-2,-2));
	else 
		r.SetPosition(r.GetPosition()+wxPoint(-1,-1));
	r.SetSize(r.GetSize()+wxSize(2,2));
	condition = controls.getPressConditionOf(r);

	int old_status_flags = statusFlags;
	frameReset();

	if(frameNumber<theme.lookUpButtonAnimation(button)->speed)
		frameNumber++;
	else frameNumber=0;
	
// button was disabled -> set status as if button was just released ~	
	if(disabledFlag)
	{
		if (old_status_flags & BF_DOWN)
			statusFlags |= BF_JUST_RELEASED;
	}

	if (shown)
	{
		// currently pressing the button?
		if( condition & POINTER_OVER_BUTTON )
		{
			statusFlags |= BF_HIGHLIGHTED;
				// if(!(old_status_flags & BF_HIGHLIGHTED )) => callback!
//				maybeShowCustomCursor(); TODO
//				maybeShowToolTip(); TODO
			if( statusFlags & BF_STATIC )
			{
				if(condition & LEFT_BUTTON_WAS_PRESSED_AND_RELEASED)
				{
					controls.mouseEventWasProcessed();
					if(old_status_flags & BF_DOWN)
						statusFlags &= ~BF_DOWN;
					else if(!(old_status_flags & BF_NOT_CLICKABLE))
						statusFlags |= BF_DOWN;
				}
			}
			else			
			if( (condition & PRESSING_LEFT_BUTTON)&&( !(old_status_flags & BF_NOT_CLICKABLE)))
				statusFlags |= BF_DOWN;
				
			if(( condition & PRESSING_LEFT_BUTTON ) && (!(old_status_flags & BF_DOWN)))
				statusFlags |= BF_JUST_PRESSED;
		}
		
		if( !(statusFlags & BF_DOWN))
		{
			nextRepeat = 0;
			if( (old_status_flags & BF_DOWN) && !(old_status_flags & BF_CLICKED))
				statusFlags |= BF_JUST_RELEASED;
			if(!(statusFlags & BF_REPEATS))
			{
				if((statusFlags & BF_JUST_RELEASED) && (statusFlags & BF_HIGHLIGHTED))
					statusFlags |= BF_CLICKED;
			}
			if(!hasBitmap)
			{
				pressedText->Hide();
				normalText->Show();
				adjustButtonSize(normalText->getSize());
				normalText->setSize(currentSize.GetSize());
			}
		} else
		{
			if(!hasBitmap)
			{
				pressedText->Show();
				normalText->Hide();
				adjustButtonSize(pressedText->getSize());
				pressedText->setSize(currentSize.GetSize());
			}

		if( !(old_status_flags & BF_DOWN))
		{
       		statusFlags |= BF_JUST_PRESSED;
	        timeStamp = getTimeStampMs(100);
       //		if (user_function)
	  //          user_function(); TODO
                                                                              
	        if (statusFlags & BF_REPEATS) 
			{
       		    nextRepeat = getTimeStampMs(300); // TODO
		        statusFlags |= BF_CLICKED;
	        }
	    } 
	    // check if a repeat event should occur
	    if ( isTimeSpanElapsed(nextRepeat) && (statusFlags & BF_REPEATS) ) 
		{
			nextRepeat = getTimeStampMs(100); //TODO
    		statusFlags |= BF_CLICKED;
      		timeStamp = getTimeStampMs(100);
	    }
                                                                               
    // check for double click occurance
  //  if (B1_DOUBLE_CLICKED && mouse_on_me) {
    //    statusFlags |= BF_DOUBLE_CLICKED;
      //  m_press_linger = timestamp(100);
//    }
		} // BF_DOWN?

//		if(old_status_flags!=statusFlags)
//			frameNumber=0;		 TODO

	} // end shown
}				

bool UI_Button::isPressed()
{
	if (statusFlags & BF_CLICKED)
		return true;
	else
	    return false;
}


bool UI_Button::isJustPressed()
{
    if ( statusFlags & BF_JUST_PRESSED )
        return true;
    else
        return false;
};

bool UI_Button::isJustReleased()
{
    if ( statusFlags & BF_JUST_RELEASED )
        return true;
    else
        return false;
};
 			
                                                                              
bool UI_Button::isJustHighlighted()
{
    if ( statusFlags & BF_JUST_HIGHLIGHTED )
        return true;
    else
        return false;
};

bool UI_Button::isCurrentlyPressed()
{
    if ( (statusFlags & BF_DOWN) || !isTimeSpanElapsed(timeStamp) )
        return true;
    else
    	return false;
};

// Is the mouse over this button?
bool UI_Button::isCurrentlyHighlighted()
{
    return (statusFlags & BF_HIGHLIGHTED);
};
                                                                               
// Is the mouse over this button?
void UI_Button::forceHighlighted()
{
    statusFlags |= BF_HIGHLIGHTED;
};
                                                                               
// Force button to get pressed
void UI_Button::forcePressed()
{
    if ( !disabledFlag ) 
	{
        statusFlags |= BF_DOWN | BF_CLICKED;
        //statusFlags |= BF_JUST_PRESSED;
    }
};

void UI_Button::forceUnpress()
{
	if( !disabledFlag )
	{
		statusFlags &= ~BF_DOWN;
		frameReset();
	};
};
                                                                               
// reset the "pressed" timestamps
void UI_Button::resetTimestamps()
{
	timeStamp=0; //1?
	nextRepeat=0;
};
                                                                               
//void UI_Button::skip_first_highlight_callback()
//{
//    statusFlags |= BF_SKIP_FIRST_HIGHLIGHT_CALLBACK;
  //  first_callback = 1;
//}
                                                                               
#if 0
void UI_Button::maybeShowCustomCursor()
{
    if (disabledFlag) // ~~
        return;
                                                                               
    // set the mouseover cursor
//        if ((custom_cursor_bmap >= 0) && (previous_cursor_bmap < 0)) {
  //          previous_cursor_bmap = gr_get_cursor_bitmap();
    //        gr_set_cursor_bitmap(custom_cursor_bmap, GR_CURSOR_LOCK);  // set and lock
//        }
};
                                                                               
void UI_Button::restorePreviousCursor()
{
//f( previousCursor >=0 )
		
  //  if (previous_cursor_bmap >= 0) {
    //    gr_set_cursor_bitmap(previous_cursor_bmap, GR_CURSOR_UNLOCK);       // restore and unlock
      //  previous_cursor_bmap = -1;
    //}
};


void UI_Button::maybeShowToolTip()
{
};

void UI_Button::hideToolTip()
{
};
#endif

