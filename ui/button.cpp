#include "radio.hpp"
#include "configuration.hpp"
#include <math.h>


UI_Button::UI_Button(const UI_Button& object) :
	UI_Object((UI_Object)object),
	radio(object.radio), //?
	forcedPress(object.forcedPress),
	originalTopLeft(object.originalTopLeft),
	distanceBottomRight(object.distanceBottomRight),
	moved(object.moved),
	originalPosition(object.originalPosition),
	hasBitmap(object.hasBitmap),
	doNotSetSize(object.doNotSetSize),
	wasPressed(object.wasPressed),
	gradient(object.gradient),
	timeStamp(object.timeStamp),
	pressdepth(object.pressdepth),
	font(object.font),
	button(object.button),
	autoSize(object.autoSize),
	mode(object.mode),
	frameNumber(object.frameNumber),
	statusFlags(object.statusFlags),
//	tempSurface(new SDL_Surface(object.tempSurface));
	text(new UI_StaticText(*object.text)),
	nextRepeat(object.nextRepeat),
	moveByMouse(object.moveByMouse)
{ }


UI_Button& UI_Button::operator=(const UI_Button& object)
{
	((UI_Object)(*this)) = ((UI_Object)object);
	radio = object.radio;
	forcedPress = object.forcedPress;
	originalTopLeft = object.originalTopLeft;
	distanceBottomRight = object.distanceBottomRight;
	moved = object.moved;
	originalPosition = object.originalPosition;
	hasBitmap = object.hasBitmap;
	doNotSetSize = object.doNotSetSize;
	wasPressed = object.wasPressed;
	gradient = object.gradient;
	timeStamp = object.timeStamp;
	pressdepth = object.pressdepth;
	font = object.font;
	button = object.button;
	autoSize = object.autoSize;
	mode = object.mode;
	frameNumber = object.frameNumber;
	statusFlags = object.statusFlags;
//	SDL_FreeSurface(tempSurface);
//	tempSurface = new SDL_Surface(object.tempSurface);

	delete text;
	text = new UI_StaticText(*object.text);
	nextRepeat = object.nextRepeat;
	moveByMouse = object.moveByMouse;
	return(*this);
}

void UI_Button::setFrameNumber(const unsigned int frame_number)
{
	frameNumber=frame_number;
}

UI_Button::UI_Button(UI_Object* button_parent, const Point top_left, const Size distance_bottom_right, const eString button_text, const eButton button_type, const eTextMode button_text_mode, const eButtonMode button_mode, const ePositionMode button_position_mode, const eFont button_font, const eAutoSize button_auto_size):
	UI_Object(button_parent, Rect(top_left, Size(0,0))),
	radio(0), //?

	forcedPress(false),
	originalTopLeft(top_left),
	distanceBottomRight(distance_bottom_right),
	moved(false),
	originalPosition(false),
	hasBitmap(false),
	doNotSetSize(false),
	wasPressed(false),
	gradient(0),
	timeStamp(0),
	pressdepth(0),
	font(button_font),
	button(button_type),
	autoSize(button_auto_size),
	mode(button_position_mode),
	frameNumber(0),
	statusFlags(0),
	nextRepeat(0),
	moveByMouse(false)
{
	setDoAdjustments(0);
	switch(button_mode)
	{
		case NO_BUTTON_MODE:statusFlags |= BF_NOT_CLICKABLE;break;
		case STATIC_BUTTON_MODE:statusFlags |= BF_STATIC;break;
		case PRESS_BUTTON_MODE:break;
		case PUSH_BUTTON_MODE:statusFlags |= BF_REPEATS;break;
		case TAB_BUTTON_MODE:statusFlags |= BF_IS_TAB;statusFlags |= BF_STATIC;break;
		default:break;
	} 
						   
	adjustButtonSize( theme.lookUpFont(font)->GetTextExtent(*theme.lookUpString(button_text)));//text->getTextSize());
	startRect=getRelativeRect();
	targetRect=getRelativeRect();
	filledHeight=getHeight();
	text = new UI_StaticText(this, button_text, Rect(Point(0,0), getSize()), theme.lookUpButtonAnimation(button_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font, button_text_mode); // TODO

//	for(int i=DISABLED_BUTTON_PHASE;i<MAX_BUTTON_ANIMATION_PHASES;i++)
//		text[i]=new UI_StaticText(this, normalText, Rect(0,0,0,0), HORIZONTALLY_CENTERED_TEXT_MODE, theme.lookUpButtonAnimation(button)->startTextColor[i], font
}


UI_Button::UI_Button(UI_Object* button_parent, const Point top_left, const Size distance_bottom_right, const std::string& button_text, const eButton button_type, const eTextMode button_text_mode, const eButtonMode button_mode, const ePositionMode button_position_mode, const eFont button_font, const eAutoSize button_auto_size):
	UI_Object(button_parent, Rect(top_left, Size(0,0))),
	radio(0), //?
	forcedPress(false),
	originalTopLeft(top_left),
	distanceBottomRight(distance_bottom_right),
	moved(false),
	originalPosition(false),
	hasBitmap(false),
	doNotSetSize(false),
	wasPressed(false),
	gradient(0),
	timeStamp(0),
	pressdepth(0),
	font(button_font),
	button(button_type),
	autoSize(button_auto_size),
	mode(button_position_mode),
	frameNumber(0),
	statusFlags(0),

//	text(new UI_StaticText(this, button_text, Rect(1,0,0,0), theme.lookUpButtonAnimation(button_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font, button_text_mode)), // TODO
	nextRepeat(0),
	moveByMouse(false)
{
	setDoAdjustments(0);
	switch(button_mode)
	{
		case NO_BUTTON_MODE:statusFlags |= BF_NOT_CLICKABLE;break;
		case STATIC_BUTTON_MODE:statusFlags |= BF_STATIC;break;
		case PRESS_BUTTON_MODE:break;
		case PUSH_BUTTON_MODE:statusFlags |= BF_REPEATS;break;
		case TAB_BUTTON_MODE:statusFlags |= BF_IS_TAB; statusFlags |= BF_STATIC;break;
		default:break;
	} 
	adjustButtonSize( theme.lookUpFont(font)->GetTextExtent(button_text));//text->getTextSize());
	startRect=getRelativeRect();
	targetRect=getRelativeRect();
	filledHeight=getHeight();
	text = new UI_StaticText(this, button_text, Rect(Point(0,0), getSize()), theme.lookUpButtonAnimation(button_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font, button_text_mode); // TODO

//	for(int i=DISABLED_BUTTON_PHASE;i<MAX_BUTTON_ANIMATION_PHASES;i++)
//		text[i]=new UI_StaticText(this, normalText, Rect(0,0,0,0), HORIZONTALLY_CENTERED_TEXT_MODE, font, theme.lookUpButtonAnimation(button)->startTextColor[i]);
}

// -> bitmap button!
UI_Button::UI_Button(UI_Object* button_parent, const Point top_left, const Size distance_bottom_right, const eButton button_type, const eButtonMode button_mode, const ePositionMode button_position_mode):
	UI_Object(button_parent, Rect(top_left, Size(0,0))),
	radio(NULL), //?
	forcedPress(false),
	originalTopLeft(top_left),
	distanceBottomRight(distance_bottom_right),
	moved(false),
	originalPosition(false),
	hasBitmap(true),
	doNotSetSize(false),
	wasPressed(false),
	gradient(0),
	timeStamp(0),
	pressdepth(0),
	font(NULL_FONT),
	button(button_type),
	autoSize(NO_AUTO_SIZE),
	mode(button_position_mode),
	frameNumber(0),
	statusFlags(0),
	text(NULL),
	nextRepeat(0),
	moveByMouse(false)
{
	// TODO size aus Bitmap bestimmen!
	setDoAdjustments(0);
	
	switch(button_mode)
	{
		case NO_BUTTON_MODE:statusFlags |= BF_NOT_CLICKABLE;break;
		case STATIC_BUTTON_MODE:statusFlags |= BF_STATIC;break;
		case PRESS_BUTTON_MODE:break;
		case PUSH_BUTTON_MODE:statusFlags |= BF_REPEATS;break;
		case TAB_BUTTON_MODE:statusFlags |= BF_IS_TAB; statusFlags |= BF_STATIC;break;
		default:break;
	} 
	startRect = getRelativeRect();
	targetRect = getRelativeRect();
	filledHeight = getHeight();
}

UI_Button::~UI_Button()
{
	delete text;
}

void UI_Button::setPressDepth(const unsigned int depth)
{
	pressdepth = depth;
	wasPressed = true;
	if(text)
	{
		if(pressdepth==0)
			text->setPressed(false);
		else text->setPressed(true);
	}
}

void UI_Button::adjustButtonSize(const Size& size)
{
	signed int left = originalTopLeft.GetLeft();
	signed int top = originalTopLeft.GetTop();
	
	unsigned int full_width = getParent()->getWidth() - left - distanceBottomRight.GetWidth();
	unsigned int full_height = getParent()->getHeight() - top - distanceBottomRight.GetHeight();
	
	signed int hor_center = left + (full_width - getWidth())/2;
	signed int right = full_width - left - getWidth();

	signed int ver_center = top + (full_height - getHeight())/2;
	signed int bottom = full_height - top - getHeight();

	switch(autoSize)
	{
		case NOTHING:break;
		case NO_AUTO_SIZE:setSize(full_width, full_height);break;
		case AUTO_SIZE_ONCE:
			setSize(size+Size(6, 0));
			autoSize = AUTO_SIZE_TWICE;
			break;
		case AUTO_SIZE_TWICE:setSize(size+Size(6, 0));
						autoSize = NO_AUTO_SIZE;break;
					
		case AUTO_SIZE:setSize(size+Size(6, 0));break;
		case AUTO_HEIGHT_FULL_WIDTH:setSize(Size(full_width, size.GetHeight()));break;
		case FULL_WIDTH:setSize(Size(full_width, getHeight()));break;
		case CONSTANT_SIZE:setSize(Size(120, size.GetHeight()));break;// ~~ nur fuer tabs naja
		default:break; // TODO ERROR
	}

//	setPosition(originalButtonRect.GetTopLeft());
	
	//relativeRect ok... aber was ist mit startRect, targetRect etc??

	switch(mode)
	{
		case DO_NOT_ADJUST:break;//setPosition(originalButtonRect.GetTopLeft());break;
		case TOTAL_CENTERED:setPosition(hor_center, ver_center);break;
		case HORIZONTAL_CENTERED:setLeft(hor_center);break;
		case VERTICALLY_CENTERED:setTop(ver_center);break;
		case TOP_LEFT:break;
		case TOP_CENTER:setPosition(hor_venter, top);break;
		case TOP_RIGHT:setPosition(right, top);break;
		case CENTER_RIGHT:setPosition(right, ver_center);break;
		case BOTTOM_RIGHT:setPosition(right, bottom);break;
		case BOTTOM_CENTER:setPosition(hor_center, bottom);break;
		case BOTTOM_LEFT:setPosition(left, bottom);break;
		case CENTER_LEFT:setPosition(left, ver_center);break;
		case ARRANGE_TOP_LEFT:
		{
			setPosition(left + getParent()->getMinTopLeftX(), top);
			getParent()->addMinTopLeftX(getWidth() + MIN_DISTANCE);
		}break;
		case ARRANGE_TOP_RIGHT:
		{
			setPosition(right - getParent()->getMinTopRightX() - getWidth(), top);
			getParent()->addMinTopRightX(getWidth() + MIN_DISTANCE);
			startRect=getRelativeRect(); // ?
			targetRect=getRelativeRect();
			filledHeight=getHeight();
		}break;
		case ARRANGE_BOTTOM_LEFT:
		{ 
			setPosition(left + getParent()->getMinBottomLeftX(), bottom);
			getParent()->addMinBottomLeftX(getWidth()+MIN_DISTANCE);
		}break;
		case ARRANGE_BOTTOM_RIGHT:
		{
			setPosition(right - getParent()->getMinBottomRightX() - 20 - getWidth(), bottom);
			getParent()->addMinBottomRightX(getWidth() + MIN_DISTANCE);
		}break;
		case ARRANGE_LEFT:
		{
			setPosition(left, top + getParent()->getMinLeftY());
			getParent()->addMinLeftY(getHeight()+MIN_DISTANCE);
		}break;
		case ARRANGE_RIGHT:
		{
			setPosition(right, top + getParent()->getMinRightY()); // TODO
			getParent()->addMinRightY(getHeight()+MIN_DISTANCE);
			startRect=getRelativeRect();
			targetRect=getRelativeRect();
			filledHeight=getHeight();
		}break;
		default:break;//TODO error
	}
}

// Render button.  How it draws exactly depends on it's current state.
void UI_Button::draw(DC* dc) const
{
	if(!isShown())
		return;
	if(checkForNeedRedraw())
	{

	eButtonAnimationPhase animation_phase;
	if(isDisabled())
		animation_phase=DISABLED_BUTTON_PHASE;
	else 
	if((statusFlags & BF_STATIC)&&(originalPosition))
	{
		if((statusFlags & BF_HIGHLIGHTED) || (gradient < 100 ))
			animation_phase=PRESSED_HIGHLIGHTED_BUTTON_PHASE;
		else
			animation_phase=PRESSED_BUTTON_PHASE;
	}
	else
	if(statusFlags & BF_DOWN)
	{
		if((statusFlags & BF_HIGHLIGHTED) || (gradient < 100 ))
			animation_phase=PRESSED_HIGHLIGHTED_BUTTON_PHASE;
		else
		animation_phase=PRESSED_BUTTON_PHASE;
	}
	else
	if((statusFlags & BF_HIGHLIGHTED)||(gradient < 100))
		animation_phase=HIGHLIGHT_BUTTON_PHASE;
	else animation_phase=NORMAL_BUTTON_PHASE;


//	if(!(statusFlags & BF_NOT_CLICKABLE)) // ?? TODO
//	{
//		dc->SetBrush(*theme.lookUpBrush(NULL_BRUSH));
//		dc->SetPen(*theme.lookUpPen(DARK_BACKGROUND_PEN));
//		dc->DrawEdgedRectangle(Rect(getAbsolutePosition()-Size(1,1), getSize() + Size(2,2)));

//		dc->SetBrush(*theme.lookUpBrush(NULL_BRUSH)); //?
//		dc->SetPen(*theme.lookUpPen(RECTANGLE_PEN));
//		dc->DrawRoundedRectangle(Rect(getAbsolutePosition(), getSize()), 2);
//	}

	if(isDisabled())
		dc->SetPen(*theme.lookUpPen(DISABLED_PEN));
	else
		dc->SetPen(Pen(dc->mixColor(
					theme.lookUpPen(theme.lookUpButtonAnimation(button)->startBorderPen[animation_phase])->GetColor(), 
					theme.lookUpPen(theme.lookUpButtonAnimation(button)->endBorderPen[animation_phase])->GetColor(), gradient), 
					theme.lookUpPen(theme.lookUpButtonAnimation(button)->startBorderPen[animation_phase])->GetWidth(), 
					theme.lookUpPen(theme.lookUpButtonAnimation(button)->startBorderPen[animation_phase])->GetStyle()));

	if(!hasBitmap)
	{
		if(isDisabled())
			dc->SetBrush(*theme.lookUpBrush(DISABLED_BRUSH));
		else
			dc->SetBrush(Brush(dc->mixColor(theme.lookUpBrush(theme.lookUpButtonAnimation(button)->startBrush[animation_phase])->GetColor(), theme.lookUpBrush(theme.lookUpButtonAnimation(button)->endBrush[animation_phase])->GetColor(), gradient),theme.lookUpBrush(theme.lookUpButtonAnimation(button)->startBrush[animation_phase])->GetStyle()));
//TODO: eigene tab-button klasse
		
//		dc->SetPen(*theme.lookUpPen(NULL_PEN));
//		if(statusFlags & BF_DOWN)
		text->setColor(dc->mixColor(theme.lookUpColor(theme.lookUpButtonAnimation(button)->startTextColor[animation_phase]), theme.lookUpColor( theme.lookUpButtonAnimation(button)->endTextColor[animation_phase]), gradient));
	// TODO TAB-BUTTON MODE
		dc->DrawEdgedRoundedRectangle(getAbsolutePosition()+Size(pressdepth, pressdepth), getSize(), 4);
//		ostringstream os;
//		if(getAbsoluteRect().Inside(p - Size(pressdepth, pressdepth) ))
//		os << 
//		dc->DrawText(" ", getAbsolutePosition() - Size(20, 0) +  Size(GetWidth(), 0));
	}
	else 
	{
//		dc->DrawRectangle(Rect(getAbsolutePosition()-Size(1,1)+Size(pressdepth, pressdepth), getSize()+Size(2,2))); // kasterl ums bitmap
		dc->DrawBitmap(theme.lookUpBitmap(theme.lookUpButtonAnimation(button)->bitmap[animation_phase]), getAbsolutePosition()+Size(pressdepth, pressdepth));
	}
	
/*	if((animation_phase==PRESSED_BUTTON_PHASE)||(animation_phase==PRESSED_HIGHLIGHTED_BUTTON_PHASE))
	{
		dc->SetPen(Pen(*(theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH)->GetColor()),1,SOLID_PEN_STYLE));
		dc->DrawLine(getAbsolutePosition() + Point(0, getHeight()-1), getAbsolutePosition() + Point(getWidth(), getHeight()-1));
		dc->DrawLine(getAbsolutePosition() + Point(getWidth()-1, 0), getAbsolutePosition() + Point(getWidth()-1, getHeight()));
	} ??? */

//	dc->DrawRectangle(Rect(originalButtonRect.GetTopLeft() + getAbsolutePosition(), originalButtonRect.GetSize()));
	}
	UI_Object::draw(dc);	
/*	int offset, frame_num = -1;

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

}



//void UI_Button::set_hotkey_if_focus(int key)
//{
//	hotkey_if_focus = key;
//}
// reset anything that needs to be at the start of a new frame before processing
void UI_Button::frameReset()
{
//  statusFlags &= ~BF_HIGHLIGHTED;
//	if(isTimeSpanElapsed(timeStamp))
//		statusFlags &= ~BF_DOWN;
//	statusFlags &= ~BF_JUST_PRESSED;
//  statusFlags &= ~BF_JUST_RELEASED;
	statusFlags &= ~BF_LEFT_CLICKED;
	statusFlags &= ~BF_DOUBLE_CLICKED;
//	statusFlags &= ~BF_JUST_HIGHLIGHTED;
// TODO bei -O3 bleiben die buttons gehighlighted... statusFlags = 0 behebt das, aber das wollen wir ja nicht :o
	
//	restorePreviousCursor(); TODO... wegen DC
//	  hideToolTip(); ~~
}

void UI_Button::mouseHasEnteredArea()
{
	resetGradient();
	statusFlags |= BF_HIGHLIGHTED;
	if(statusFlags & BF_WAS_PRESSED)
	{
		if(originalPosition)
			setPressDepth(0);
		else
			setPressDepth(1);
		statusFlags |= BF_DOWN;
	}
}

void UI_Button::mouseHasLeftArea()
{
	statusFlags &= ~BF_HIGHLIGHTED;
	if(statusFlags & BF_WAS_PRESSED)
	{
		if(originalPosition)
			setPressDepth(1);
		else
			setPressDepth(0);
		statusFlags &= ~BF_DOWN;
	}
}

void UI_Button::mouseLeftButtonPressed()
{
	if((statusFlags & BF_NOT_CLICKABLE)||((statusFlags & BF_IS_TAB)&&(originalPosition==true)))
		return;
	statusFlags |= BF_WAS_PRESSED;
	statusFlags |= BF_DOWN;
	timeStamp = getTimeStampMs(100000);
	if(originalPosition)
		setPressDepth(0);
	else
		setPressDepth(1);
	setNeedRedrawMoved();
}

void UI_Button::mouseLeftButtonReleased()
{
	if((statusFlags & BF_NOT_CLICKABLE))
		return;
//	if(forcedPress)
//		return;
	statusFlags &= ~BF_WAS_PRESSED;
	if ((statusFlags & BF_DOWN))// && (isTimeSpanElapsed(timeStamp)))
	{
		statusFlags &= ~BF_DOWN;
		statusFlags |= BF_LEFT_CLICKED;
		if(statusFlags & BF_STATIC)
		{
			if((originalPosition)&&(!(statusFlags & BF_IS_TAB)))
				originalPosition=false;
			else if(!originalPosition)
				originalPosition=true;
			setNeedRedrawMoved();
		}
		else
			setPressDepth(0);
		if(radio)
		{
			if(originalPosition)
				radio->leftButtonPressed(this);
			else 
			if(!(statusFlags & BF_IS_TAB))
				radio->leftButtonReleased(this);
		}		
	}
}

void UI_Button::mouseRightButtonPressed()
{
	if((statusFlags & BF_NOT_CLICKABLE)||((statusFlags & BF_IS_TAB)&&(originalPosition==true)))
		return;
	statusFlags |= BF_WAS_PRESSED;
	statusFlags |= BF_DOWN;
	timeStamp = getTimeStampMs(100000);
	if(originalPosition)
		setPressDepth(0);
	else
		setPressDepth(1);
	setNeedRedrawMoved();
}

void UI_Button::mouseRightButtonReleased()
{
	if((statusFlags & BF_NOT_CLICKABLE))
		return;
//	if(forcedPress)
//		return;
	statusFlags &= ~BF_WAS_PRESSED;
	if ((statusFlags & BF_DOWN))// && (isTimeSpanElapsed(timeStamp)))
	{
		statusFlags &= ~BF_DOWN;
		statusFlags |= BF_RIGHT_CLICKED;
		if(statusFlags & BF_STATIC)
		{
			if((originalPosition)&&(!(statusFlags & BF_IS_TAB)))
				originalPosition=false;
			else if(!originalPosition)
				originalPosition=true;
			setNeedRedrawMoved();
		}
		else
			setPressDepth(0);
		if(radio)
		{
			if(originalPosition)
				radio->rightButtonPressed(this);
			else 
			if(!(statusFlags & BF_IS_TAB))
				radio->rightButtonReleased(this);
		}		
	}
}

UI_Object* UI_Button::checkTooltip() {
	if( (!isShown()) || (isDisabled()) || (!getAbsoluteRect().Inside(mouse - Size(pressdepth, pressdepth))))
		return(0);
	return((UI_Object*)this);
}

UI_Object* UI_Button::checkHighlight()
{
	if( (!isShown()) || (isDisabled()) || (!getAbsoluteRect().Inside(mouse - Size(pressdepth, pressdepth) )) )
		return(NULL);
	resetGradient();
	return((UI_Object*)this);
}

void UI_Button::process()
{
/*	if(!isShown())
	{
		resetGradient();
		return;
	}*/
//	oldRect = getAbsoluteRect();

	// TODO evtl Animation fuer jede Phase in die config datei
	// dann waere sowas moeglich, dass ich maus reinfahr und das langsam verblasst
	// evtl auch einfach brightencolor ueberlegen...
	unsigned int oldgradient = gradient;
	if(wasPressed)
	{
		wasPressed=false;
		setNeedRedrawMoved();
	}
	if(!uiConfiguration.isGlowingButtons())
	{
		if(!(statusFlags & BF_HIGHLIGHTED))
			gradient = 100;
		else 
			if(frameNumber<theme.lookUpButtonAnimation(button)->speed/2) gradient=20;
		else 
			gradient = 100;
	} else
	if(!(statusFlags & BF_HIGHLIGHTED))
		gradient += (100 - gradient) / 5 + 1;
	else
	switch(theme.lookUpButtonAnimation(button)->type)
	{	
		case NO_ANIMATION:if(gradient < 100) gradient++;else gradient = 100;break;
		case JUMPY_COLORS_ANIMATION:gradient=(frameNumber%theme.lookUpButtonAnimation(button)->speed)*100/theme.lookUpButtonAnimation(button)->speed;break;
		case GLOWING_ANIMATION:gradient=(unsigned int)(50*(sin(3.141*frameNumber/theme.lookUpButtonAnimation(button)->speed)+1));break;
		case BLINKING_ANIMATION:if(frameNumber<theme.lookUpButtonAnimation(button)->speed/2) gradient=0;else gradient=100;break;
		default:break;
	}

	if(gradient > 100)
		gradient = 100;

	if(gradient != oldgradient)
		setNeedRedrawNotMoved();

	Point absoluteCoord = getRelativePosition();
	Size absoluteSize = getSize();
/*	if(moveByMouse)
	{
		setPosition(mouse);
		moveByMouse=false;
	}
	else*/
		UI_Object::process();
	
/*	originalButtonRect.SetLeft( originalButtonRect.GetLeft() + getRelativePosition().x - absoluteCoord.x);
	originalButtonRect.SetTop( originalButtonRect.GetTop() + getRelativePosition().y - absoluteCoord.y);
	if(!doNotSetSize)
	{
		originalButtonRect.SetWidth( originalButtonRect.GetWidth() + getWidth() - absoluteSize.GetWidth());
		originalButtonRect.SetHeight( originalButtonRect.GetHeight() + getHeight() - absoluteSize.GetHeight());
	} else doNotSetSize=false;
*/
	if(!hasBitmap)
	{
		adjustButtonSize(text->getTextSize());

		text->setSize(getSize());
	}

	if(statusFlags & BF_WAS_PRESSED)
		statusFlags |= BF_HIGHLIGHTED;

	if(statusFlags & BF_HIGHLIGHTED)
	{
		if(frameNumber<theme.lookUpButtonAnimation(button)->speed)
			frameNumber+=2;
		else frameNumber=0;
	}
	
// button was disabled -> set status as if button was just released ~	
//	if(disabledFlag)
//	{
//		if (old_status_flags & BF_DOWN)
//			statusFlags |= BF_JUST_RELEASED;
//	}


// TODO bei Force-Highlight passt das nicht mehr

	// do we have an event?
/*	if (condition & LEFT_BUTTON_WAS_PRESSED_AND_RELEASED)
	{
		statusFlags |= BF_LEFT_CLICKED;
		controls.mouseEventWasProcessed();
		if(statusFlags & BF_STATIC)
			originalPosition = !originalPosition;
	}*/

/*	
		if( !(statusFlags & BF_DOWN))
		{
			nextRepeat = 0;
			if(!(statusFlags & BF_REPEATS))
			{
				if((statusFlags & BF_JUST_RELEASED) && (statusFlags & BF_HIGHLIGHTED))
				{
					statusFlags |= BF_LEFT_CLICKED;
				}
			}

			if( (!(old_status_flags & BF_DOWN)) && (statusFlags & BF_HIGHLIGHTED ))
			{
	   			statusFlags |= BF_JUST_PRESSED;
				timeStamp = getTimeStampMs(100000);
	   //		if (user_function)
	  //		  user_function(); TODO
				if (statusFlags & BF_REPEATS) 
				{
	   				nextRepeat = getTimeStampMs(300000); // TODO
					statusFlags |= BF_LEFT_CLICKED;
				}
			} 
			// check if a repeat event should occur
			if ( isTimeSpanElapsed(nextRepeat) && (statusFlags & BF_REPEATS) ) 
			{
				nextRepeat = getTimeStampMs(100000); //TODO
				statusFlags |= BF_LEFT_CLICKED;
		  		timeStamp = getTimeStampMs(100000);
			}
	// check for double click occurance
  //  if (B1_DOUBLE_CLICKED && mouse_on_me) {
	//	statusFlags |= BF_DOUBLE_CLICKED;
	  //  m_press_linger = timestamp(100);
//	}
		} // BF_DOWN?

		if((old_status_flags!=statusFlags) && (!(statusFlags & BF_IS_TAB))) // == lock animation evtl noch spaeter seperat machen
			frameNumber=0;

	} // end shown*/

	forcedPress = false;

	
// TODO evtl ueberlegen, dass markierte buttons langsam verblassen wenn sie nicht mehr gehighlighted sind
	// Problem: die mixColor arbeitet mit animation_phase, wenn ausserhalb der phase wird gradient ignoriert auf 

		
//	if(!doesNeedRedraw())
//		getParent()->addRect(getAbsoluteRect()); // Dieses rect NICHT neuzeichnen
}

void UI_Button::doNotAdaptSize()
{
	doNotSetSize=true;
}

const bool UI_Button::isLeftClicked()
{
	if(!isShown())
		return false;
// currently pressed
	if (statusFlags & BF_LEFT_CLICKED)
	{
		statusFlags &= ~BF_LEFT_CLICKED;
		return true;
	}
	else
		return false;
}
const bool UI_Button::isRightClicked()
{
	if(!isShown())
		return false;
// currently pressed
	if (statusFlags & BF_RIGHT_CLICKED)
	{
		statusFlags &= ~BF_RIGHT_CLICKED;
		return true;
	}
	else
		return false;
}


void UI_Button::resetGradient()
{
	gradient=0;
	frameNumber=0;
}

/*const bool UI_Button::isJustPressed() const
{
	if(!isShown())
		return false;
	if ( statusFlags & BF_JUST_PRESSED )
		return true;
	else
		return false;
}

const bool UI_Button::isJustReleased() const
{
	if(!isShown())
		return false;
	if ( statusFlags & BF_JUST_RELEASED )
		return true;
	else
		return false;
}*/
 			
/*const bool UI_Button::isJustHighlighted() const
{
	if(!isShown())
		return false;
	if ( statusFlags & BF_JUST_HIGHLIGHTED )
		return true;
	else
		return false;
}*/ // TODO?

const bool UI_Button::isCurrentlyActivated() const
{
//	if(!isShown())
//			return false;
	if ( (statusFlags & BF_STATIC) && (originalPosition==true) )
		return true;
	else
		return false;
}

const bool UI_Button::isCurrentlyPressed() const
{
	if(!isShown())
		return false;
	if ( (statusFlags & BF_DOWN))// || !isTimeSpanElapsed(timeStamp) )
		return true;
	else
		return false;
}

// Is the mouse over this button?
const bool UI_Button::isCurrentlyHighlighted() const
{
	if(!isShown())
		return false;
	return (statusFlags & BF_HIGHLIGHTED);
}

// Force button to get pressed
void UI_Button::forcePress()
{
	if ( (!isDisabled() ) && (!(statusFlags & BF_NOT_CLICKABLE)))
	{
		if((statusFlags & BF_STATIC) && (!originalPosition))
		{
			forcedPress=true;
//  		timeStamp = getTimeStampMs(100);
			statusFlags |= BF_LEFT_CLICKED;// */| BF_JUST_PRESSED;
			statusFlags &= ~BF_WAS_PRESSED;
			originalPosition=true;
//			if ((statusFlags & BF_DOWN))// && (isTimeSpanElapsed(timeStamp)))
//			{
   			statusFlags &= ~BF_DOWN;
			setPressDepth(1);
		} else if((!(statusFlags & BF_STATIC)) && (!(statusFlags & BF_DOWN)))
		{
			statusFlags |= BF_LEFT_CLICKED | BF_DOWN;			 //~
			setPressDepth(1); //?
		}
	} 
}

void UI_Button::forceUnpress()
{
	if(( !isDisabled() ) && (statusFlags & BF_STATIC) && (originalPosition))
	{
		originalPosition = false;
		statusFlags &= ~BF_WAS_PRESSED;
//		statusFlags |= BF_LEFT_CLICKED; ?
		statusFlags &= ~BF_DOWN; // ~~
		setPressDepth(0);
	}
}
// reset the "pressed" timestamps
void UI_Button::resetTimestamps()
{
	timeStamp=0; //1?
	nextRepeat=0;
}
//void UI_Button::skip_first_highlight_callback()
//{
//	statusFlags |= BF_SKIP_FIRST_HIGHLIGHT_CALLBACK;
  //  first_callback = 1;
//}
#if 0
void UI_Button::maybeShowCustomCursor()
{
	if (disabledFlag) // ~~
		return;
	// set the mouseover cursor
//		if ((custom_cursor_bmap >= 0) && (previous_cursor_bmap < 0)) {
  //		  previous_cursor_bmap = gr_get_cursor_bitmap();
	//		gr_set_cursor_bitmap(custom_cursor_bmap, GR_CURSOR_LOCK);  // set and lock
//		}
}
void UI_Button::restorePreviousCursor()
{
//f( previousCursor >=0 )
		
  //  if (previous_cursor_bmap >= 0) {
	//	gr_set_cursor_bitmap(previous_cursor_bmap, GR_CURSOR_UNLOCK);	   // restore and unlock
	  //  previous_cursor_bmap = -1;
	//}
}*/
#endif


