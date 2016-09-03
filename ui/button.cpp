#include "button.hpp"
#include "math.h"

UI_Radio::UI_Radio(UI_Object* parent):UI_Object(parent)
{
	changed=true;
};

UI_Radio::~UI_Radio()
{
};

void UI_Radio::addButton(UI_Button* button)
{
	if(!button)
		return;
	button->setParent(this);
};

const bool UI_Radio::hasChanged() const
{
	return(changed);
};

void UI_Radio::forceTabPressed(const int tab)
{
    UI_Button* tmp=(UI_Button*)getChildren();
	if(!tmp) return;
    int i=0;
    do
    {
		if(i==tab)
		{
			tmp->forcePressed();
			return;
		};
        i++;
        tmp=(UI_Button*)tmp->getNextBrother();
    } while(tmp!=getChildren());
};

const int UI_Radio::getMarked() const
{
	UI_Button* tmp=(UI_Button*)getChildren();
	if(!tmp) return(0);
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

void UI_Radio::draw(DC* dc) const
{
	if(!shown)
		return;
	UI_Object::draw(dc);
};

void UI_Radio::process()
{
	if(!getChildren()) return;
	changed=false;
	bool old[10];
	for(int k=10;k--;)
		old[k]=false;
    UI_Button* tmp=(UI_Button*)getChildren();
    int i=0;
    do
    {
        if((tmp->isCurrentlyPressed())&&(!tmp->forcedPress))
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

void UI_Button::setFrameNumber(const int num)
{
	frameNumber=num;
};

UI_Button::UI_Button(UI_Object* parent, Rect rect, Rect maxRect, eString normalText, eString pressedText, eButton button, eTextMode textMode, eButtonMode buttonMode, ePositionMode mode, eFont font, eAutoSize autoSize):UI_Object(parent, rect, maxRect)
{
	hasBitmap=false;
	
	statusFlags=0;
	firstPressed=false;
	hasToolTip=false;
	frameNumber=0;
	timeStamp=0;
	forcedPress=false;
//	gradient=100;

	switch(buttonMode)
	{
		case NO_BUTTON_MODE:statusFlags |= BF_NOT_CLICKABLE;break;
		case STATIC_BUTTON_MODE:statusFlags |= BF_STATIC;break;
		case PRESS_BUTTON_MODE:break;
		case PUSH_BUTTON_MODE:statusFlags |= BF_REPEATS;break;
		case TAB_BUTTON_MODE:statusFlags |= BF_IS_TAB;statusFlags |= BF_STATIC;break;
		default:break;
	}; 
						   
	this->currentSize=rect;
	this->autoSize=autoSize;
	this->mode=mode;
	this->button=button;
	this->font=font;
	this->normalText=new UI_StaticText(this, normalText, Rect(0,0,0,0), textMode, theme.lookUpButtonAnimation(button)->startTextColor[PRESSED_BUTTON_PHASE], font);
	this->pressedText=new UI_StaticText(this, pressedText, Rect(0,0,0,0), textMode, theme.lookUpButtonAnimation(button)->startTextColor[PRESSED_BUTTON_PHASE], font);

//	for(int i=DISABLED_BUTTON_PHASE;i<MAX_BUTTON_ANIMATION_PHASES;i++)
//		text[i]=new UI_StaticText(this, normalText, Rect(0,0,0,0), HORIZONTALLY_CENTERED_TEXT_MODE, theme.lookUpButtonAnimation(button)->startTextColor[i], font
	adjustButtonSize(this->normalText->getSize());
};

UI_Button::UI_Button(UI_Object* parent, Rect rect, Rect maxRect, string normalText, string pressedText, eButton button, eTextMode textMode, eButtonMode buttonMode, ePositionMode mode, eFont font, eAutoSize autoSize):UI_Object(parent, rect, maxRect)
{
	hasBitmap=false;
	
	statusFlags=0;
	firstPressed=false;
	hasToolTip=false;
	frameNumber=0;
	timeStamp=0;
    forcedPress=false;     
//	gradient=100;
	
	switch(buttonMode)
	{
		case NO_BUTTON_MODE:statusFlags |= BF_NOT_CLICKABLE;break;
		case STATIC_BUTTON_MODE:statusFlags |= BF_STATIC;break;
		case PRESS_BUTTON_MODE:break;
		case PUSH_BUTTON_MODE:statusFlags |= BF_REPEATS;break;
        case TAB_BUTTON_MODE:statusFlags |= BF_IS_TAB; statusFlags |= BF_STATIC;break;
		default:break;
	}; 
						   
	this->currentSize=rect;
	this->autoSize=autoSize;
	this->mode=mode;
	this->button=button;
	this->font=font;
	this->normalText=new UI_StaticText(this, normalText, Rect(0,0,0,0), textMode, theme.lookUpButtonAnimation(button)->startTextColor[PRESSED_BUTTON_PHASE], font);
	this->pressedText=new UI_StaticText(this, pressedText, Rect(0,0,0,0), textMode, theme.lookUpButtonAnimation(button)->startTextColor[PRESSED_BUTTON_PHASE], font);

//	for(int i=DISABLED_BUTTON_PHASE;i<MAX_BUTTON_ANIMATION_PHASES;i++)
//		text[i]=new UI_StaticText(this, normalText, Rect(0,0,0,0), HORIZONTALLY_CENTERED_TEXT_MODE, font, theme.lookUpButtonAnimation(button)->startTextColor[i]);
	adjustButtonSize(this->normalText->getSize());
};

// -> bitmap button!
UI_Button::UI_Button(UI_Object* parent, Rect rect, Rect maxRect, eButton button, eButtonMode buttonMode, ePositionMode mode):UI_Object(parent, rect, maxRect)
{
	hasBitmap=true;
	
	statusFlags=0;
	firstPressed=false;
	hasToolTip=false;
	frameNumber=0;
	timeStamp=0;
    forcedPress=false;     
//	gradient=100;
	switch(buttonMode)
	{
        case NO_BUTTON_MODE:statusFlags |= BF_NOT_CLICKABLE;break;
		case STATIC_BUTTON_MODE:statusFlags |= BF_STATIC;break;
		case PRESS_BUTTON_MODE:break;
		case PUSH_BUTTON_MODE:statusFlags |= BF_REPEATS;break;
        case TAB_BUTTON_MODE:statusFlags |= BF_IS_TAB; statusFlags |= BF_STATIC;break;
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

void UI_Button::setButton(const eButton button)
{
	this->button=button;
};

void UI_Button::adjustButtonSize(const Size size)
{
   	currentSize.SetSize(size+Size(5,0));

	switch(autoSize)
	{
		case NO_AUTO_SIZE:currentSize.SetSize(getSize());break;
		case AUTO_SIZE:break;
		case AUTO_HEIGHT_FULL_WIDTH:currentSize.SetWidth(getWidth()-5);break;
		case CONSTANT_SIZE:currentSize.SetWidth(120);break;// ~~ nur fuer tabs naja
	};

	currentSize.SetPosition(getRelativePosition());

	switch(mode)
	{
		case DO_NOT_ADJUST:break;
		case TOTAL_CENTERED:currentSize.SetPosition(getRelativePosition()+Point((getWidth() - currentSize.GetWidth())/2, (getHeight() + currentSize.GetHeight())/2));break;
		case HORIZONTAL_CENTERED:currentSize.SetLeft(getRelativeUpperBound()+(getWidth() - currentSize.GetWidth())/2);break;
		case VERTICALLY_CENTERED:currentSize.SetTop(getRelativeUpperBound()+(getHeight() - currentSize.GetHeight())/2);break;
		case TOP_LEFT:break;
		case TOP_CENTER:currentSize.SetPosition(getRelativePosition()+Point((getWidth() + currentSize.GetWidth())/2, 
																				0));break;
		case TOP_RIGHT:currentSize.SetPosition(getRelativePosition()+Point(getWidth() - currentSize.GetWidth(), 
								   												0));break;
		case CENTER_RIGHT:currentSize.SetPosition(getRelativePosition()+Point(getWidth() - currentSize.GetWidth(), 
									  											(getHeight() + currentSize.GetHeight())/2));break;
		case BOTTOM_RIGHT:currentSize.SetPosition(getRelativePosition()+Point(getWidth() - currentSize.GetWidth(), 
									  											getHeight() - currentSize.GetHeight()));break;
		case BOTTOM_CENTER:currentSize.SetPosition(getRelativePosition()+Point((getWidth() - currentSize.GetWidth())/2, 
									   											getHeight() - 2*currentSize.GetHeight()));break;
		case BOTTOM_LEFT:currentSize.SetPosition(getRelativePosition()+Point(0, 
									 											getHeight() - currentSize.GetHeight()));break;
		case CENTER_LEFT:currentSize.SetPosition(getRelativePosition()+Point(0, 
									 											(getHeight() + currentSize.GetHeight())/2));break;

		case ARRANGE_TOP:
						 {
							 currentSize.SetPosition(getRelativePosition()+Point(getParent()->min_top_x, 0));
							 getParent()->min_top_x += currentSize.GetWidth() + MIN_DISTANCE;
						 };break;
		case ARRANGE_BOTTOM:currentSize.SetPosition(getRelativePosition()+Point(getParent()->min_bottom_x, getHeight() - currentSize.GetHeight()));getParent()->min_bottom_x+=currentSize.GetWidth()+MIN_DISTANCE;break;
		case ARRANGE_LEFT:currentSize.SetPosition(getRelativePosition()+Point(0, getParent()->min_left_y));getParent()->min_left_y+=currentSize.GetHeight()+MIN_DISTANCE;break;
		case ARRANGE_RIGHT:currentSize.SetPosition(getRelativePosition()+Point(getWidth() - currentSize.GetWidth(), getParent()->min_right_y));getParent()->min_right_y+=currentSize.GetHeight()+MIN_DISTANCE;break;
	};

};

void UI_Button::updatePressedText(const string utext)
{
	pressedText->updateText(utext);
};

void UI_Button::updateNormalText(const string utext)
{
	normalText->updateText(utext);
};

// Render button.  How it draws exactly depends on it's current state.
void UI_Button::draw(DC* dc) const
{
	if(!shown)
		return;

	Point m=Point(0,0);
	if(statusFlags & BF_DOWN)
		m=Point(1,1);
		
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
//		currentSize.SetPosition(currentSize.GetPosition()+m);
	}
	else
	if(statusFlags & BF_HIGHLIGHTED)
		animation_phase=HIGHLIGHT_BUTTON_PHASE;
	else animation_phase=NORMAL_BUTTON_PHASE;


	if(!(statusFlags & BF_NOT_CLICKABLE)) // ??
	{
		dc->SetBrush(*theme.lookUpBrush(NULL_BRUSH));
		dc->SetPen(*theme.lookUpPen(DARK_BACKGROUND_PEN));
		dc->DrawRectangle(Rect(getAbsolutePosition()-Point(1,1)-m, currentSize.GetSize()+Size(2,2)));

//		dc->SetBrush(*theme.lookUpBrush(NULL_BRUSH)); //?
//		dc->SetPen(*theme.lookUpPen(RECTANGLE_PEN));
//		dc->DrawRoundedRectangle(Rect(getAbsolutePosition(), currentSize.GetSize()), 2);
	}

	int gradient=100;
	switch(theme.lookUpButtonAnimation(button)->type)
	{
    	case NO_ANIMATION:if(gradient<100) gradient++;else gradient=100;break;
		case JUMPY_COLOURS_ANIMATION:gradient=(frameNumber%theme.lookUpButtonAnimation(button)->speed)*100/theme.lookUpButtonAnimation(button)->speed;break;
		case GLOWING_ANIMATION:gradient=(int)(50*(sin(3.141*frameNumber/theme.lookUpButtonAnimation(button)->speed)+1));break;
		case BLINKING_ANIMATION:if(frameNumber<theme.lookUpButtonAnimation(button)->speed/2) gradient=100;else gradient=0;break;
		default:break;
	};
// TODO evtl ueberlegen, dass markierte buttons langsam verblassen wenn sie nicht mehr gehighlighted sind
	// Problem: die mixColor arbeitet mit animation_phase, wenn ausserhalb der phase wird gradient ignoriert auf 
	dc->SetPen(Pen(dc->mixColor(
					theme.lookUpPen(theme.lookUpButtonAnimation(button)->startBorderPen[animation_phase])->GetColor(), 
					theme.lookUpPen(theme.lookUpButtonAnimation(button)->endBorderPen[animation_phase])->GetColor(), gradient), 
					theme.lookUpPen(theme.lookUpButtonAnimation(button)->startBorderPen[animation_phase])->GetWidth(), 
					theme.lookUpPen(theme.lookUpButtonAnimation(button)->startBorderPen[animation_phase])->GetStyle()));

	if(!hasBitmap)
	{
		dc->SetBrush(Brush(dc->mixColor(theme.lookUpBrush(theme.lookUpButtonAnimation(button)->startBrush[animation_phase])->GetColor(), theme.lookUpBrush(theme.lookUpButtonAnimation(button)->endBrush[animation_phase])->GetColor(), gradient),theme.lookUpBrush(theme.lookUpButtonAnimation(button)->startBrush[animation_phase])->GetStyle()));
//TODO: eigene tab-button klasse
		
//		dc->SetPen(*theme.lookUpPen(NULL_PEN));
		if(statusFlags & BF_DOWN)
			pressedText->setColor(dc->mixColor(theme.lookUpColor(theme.lookUpButtonAnimation(button)->startTextColor[animation_phase]), theme.lookUpColor( theme.lookUpButtonAnimation(button)->endTextColor[animation_phase]), gradient));
		else 
			normalText->setColor(dc->mixColor(theme.lookUpColor(theme.lookUpButtonAnimation(button)->startTextColor[animation_phase]), theme.lookUpColor( theme.lookUpButtonAnimation(button)->endTextColor[animation_phase]), gradient));
	// TODO TAB-BUTTON MODE
		dc->DrawRoundedRectangle(Rect(getAbsolutePosition(), currentSize.GetSize()), 2);
	}
	else 
	{
		dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
		dc->DrawRectangle(Rect(getAbsolutePosition()-Point(1,1),currentSize.GetSize()+Size(2,2))); // kasterl ums bitmap
		dc->DrawBitmap(*theme.lookUpBitmap(theme.lookUpButtonAnimation(button)->bitmap[animation_phase]), getAbsolutePosition());
	}
	
/*	if((animation_phase==PRESSED_BUTTON_PHASE)||(animation_phase==PRESSED_HIGHLIGHTED_BUTTON_PHASE))
	{
		dc->SetPen(Pen(*(theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH)->GetColor()),1,SOLID_PEN_STYLE));
		dc->DrawLine(getAbsolutePosition() + Point(0, currentSize.GetHeight()-1), getAbsolutePosition() + Point(currentSize.GetWidth(), currentSize.GetHeight()-1));
		dc->DrawLine(getAbsolutePosition() + Point(currentSize.GetWidth()-1, 0), getAbsolutePosition() + Point(currentSize.GetWidth()-1, currentSize.GetHeight()));
	} ??? */

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

//	if(statusFlags & BF_DOWN)
//		currentSize.SetPosition(currentSize.GetPosition()-m);
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
// TODO bei -O3 bleiben die buttons gehighlighted... statusFlags = 0 behebt das, aber das wollen wir ja nicht :o
	
//    restorePreviousCursor(); TODO... wegen DC
//	  hideToolTip(); ~~
}
                                                                               

// process all kind of messages

void UI_Button::process()
{
	if(!shown) //~~
		return;
	// first check getChildren()
	setAbsoluteCoord(currentSize.GetPosition() - getRelativePosition()); 
	UI_Object::process();
	
	Rect r = Rect(getParent()->getAbsolutePosition() + currentSize.GetPosition(), currentSize.GetSize());	
	int condition;
	if(statusFlags & BF_DOWN)
		r.SetPosition(r.GetPosition()+Point(-1,-1));
//	else 
//		r.SetPosition(r.GetPosition()+Point(-1,-1));
	r.SetSize(r.GetSize());
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
				{
					statusFlags |= BF_CLICKED;
				}
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

		if((old_status_flags!=statusFlags) && (!(statusFlags & BF_IS_TAB))) // == lock animation evtl noch spaeter seperat machen
			frameNumber=0;

	} // end shown
	forcedPress=false;
}				

const bool UI_Button::isPressed() const
{
	if (statusFlags & BF_CLICKED)
		return true;
	else
	    return false;
}


const bool UI_Button::isJustPressed() const
{
    if ( statusFlags & BF_JUST_PRESSED )
        return true;
    else
        return false;
};

const bool UI_Button::isJustReleased() const
{
    if ( statusFlags & BF_JUST_RELEASED )
        return true;
    else
        return false;
};
 			
                                                                              
const bool UI_Button::isJustHighlighted() const
{
    if ( statusFlags & BF_JUST_HIGHLIGHTED )
        return true;
    else
        return false;
};

const bool UI_Button::isCurrentlyPressed() const
{
    if ( (statusFlags & BF_DOWN) || !isTimeSpanElapsed(timeStamp) )
        return true;
    else
    	return false;
};

// Is the mouse over this button?
const bool UI_Button::isCurrentlyHighlighted() const
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
		forcedPress=true;
//        timeStamp = getTimeStampMs(100);
	      statusFlags |= BF_DOWN | BF_CLICKED;// */| BF_JUST_PRESSED;
    }
};

void UI_Button::forceUnpress()
{
	if( !disabledFlag )
	{
		statusFlags &= ~BF_DOWN;
//		statusFlags &= ~BF_JUST_RELEASED; // ~~
	//	frameReset();
	
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

