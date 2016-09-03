#include "radio.hpp"
#include "configuration.hpp"
#include <math.h>


UI_Button::UI_Button(const UI_Button& object) :
	UI_Object((UI_Object)object),
	radio(object.radio), //?
	forcedPress(object.forcedPress),
	moved(object.moved),
	isOriginalPosition(object.isOriginalPosition),
	hasBitmap(object.hasBitmap),
	wasPressed(object.wasPressed),
	gradient(object.gradient),
	pressdepth(object.pressdepth),
	font(object.font),
	button(object.button),
	frameNumber(object.frameNumber),
	statusFlags(object.statusFlags),
	text(new UI_StaticText(*object.text))
{ }


UI_Button& UI_Button::operator=(const UI_Button& object)
{
	((UI_Object)(*this)) = ((UI_Object)object);
	radio = object.radio;
	forcedPress = object.forcedPress;
	moved = object.moved;
	isOriginalPosition = object.isOriginalPosition;
	hasBitmap = object.hasBitmap;
	wasPressed = object.wasPressed;
	gradient = object.gradient;
	pressdepth = object.pressdepth;
	font = object.font;
	button = object.button;
	frameNumber = object.frameNumber;
	statusFlags = object.statusFlags;

	delete text;
	text = new UI_StaticText(*object.text);
	return(*this);
}

void UI_Button::setFrameNumber(const unsigned int frame_number)
{
	frameNumber=frame_number;
}

UI_Button::UI_Button(UI_Object* button_parent, const Rect button_rect, const Size distance_bottom_right, const eString button_text, const eButton button_type, const eButtonMode button_mode, const ePositionMode button_position_mode, const eFont button_font, const eAutoSize button_auto_size) :
	UI_Object(button_parent, button_rect, distance_bottom_right, button_position_mode, button_auto_size),
	radio(0), //?

	forcedPress(false),
	moved(false),
	isOriginalPosition(false),
	hasBitmap(false),
	wasPressed(false),
	gradient(0),
	pressdepth(0),
	font(button_font),
	button(button_type),
	frameNumber(0),
	statusFlags(0)
{
	switch(button_mode)
	{
		case NO_BUTTON_MODE:statusFlags |= BF_NOT_CLICKABLE;break;
		case STATIC_BUTTON_MODE:statusFlags |= BF_STATIC;break;
		case PRESS_BUTTON_MODE:break;
		case PUSH_BUTTON_MODE:statusFlags |= BF_REPEATS;break;
		case TAB_BUTTON_MODE:statusFlags |= BF_IS_TAB;statusFlags |= BF_STATIC;break;
		default:break;
	} 
						   
//	adjustPositionAndSize( theme.lookUpFont(font)->GetTextExtent(*theme.lookUpString(button_text)));//text->getTextSize());
//	startRect=getRelativeRect();
//	targetRect=getRelativeRect();
//	filledHeight=getHeight();
	text = new UI_StaticText(this, button_text, Rect(Point(0,0), getSize()), Size(0,0), theme.lookUpButtonAnimation(button_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font, TOTAL_CENTERED); // TODO

//	for(int i=DISABLED_BUTTON_PHASE;i<MAX_BUTTON_ANIMATION_PHASES;i++)
//		text[i]=new UI_StaticText(this, normalText, Rect(0,0,0,0), HORIZONTALLY_CENTERED_TEXT_MODE, theme.lookUpButtonAnimation(button)->startTextColor[i], font
}


UI_Button::UI_Button(UI_Object* button_parent, const Rect button_rect, const Size distance_bottom_right, const std::string& button_text, const eButton button_type, const eButtonMode button_mode, const ePositionMode button_position_mode, const eFont button_font, const eAutoSize button_auto_size) :
	UI_Object(button_parent, button_rect, distance_bottom_right, button_position_mode, button_auto_size),
	radio(0), //?
	forcedPress(false),
	moved(false),
	isOriginalPosition(false),
	hasBitmap(false),
	wasPressed(false),
	gradient(0),
	pressdepth(0),
	font(button_font),
	button(button_type),
	frameNumber(0),
	statusFlags(0)
//	text(new UI_StaticText(this, button_text, Rect(1,0,0,0), theme.lookUpButtonAnimation(button_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font)), // TODO
{
	switch(button_mode)
	{
		case NO_BUTTON_MODE:statusFlags |= BF_NOT_CLICKABLE;break;
		case STATIC_BUTTON_MODE:statusFlags |= BF_STATIC;break;
		case PRESS_BUTTON_MODE:break;
		case PUSH_BUTTON_MODE:statusFlags |= BF_REPEATS;break;
		case TAB_BUTTON_MODE:statusFlags |= BF_IS_TAB; statusFlags |= BF_STATIC;break;
		default:break;
	} 
//	adjustPositionAndSize( theme.lookUpFont(font)->GetTextExtent(button_text));//text->getTextSize());
//	startRect=getRelativeRect();
//	targetRect=getRelativeRect();
//	filledHeight=getHeight();
	text = new UI_StaticText(this, button_text, Rect(Point(0,0), getSize()), Size(0,0), theme.lookUpButtonAnimation(button_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font, TOTAL_CENTERED); // TODO

//	for(int i=DISABLED_BUTTON_PHASE;i<MAX_BUTTON_ANIMATION_PHASES;i++)
//		text[i]=new UI_StaticText(this, normalText, Rect(0,0,0,0), HORIZONTALLY_CENTERED_TEXT_MODE, font, theme.lookUpButtonAnimation(button)->startTextColor[i]);
}

// -> bitmap button!
UI_Button::UI_Button(UI_Object* button_parent, const Rect button_rect, const Size distance_bottom_right, const eButton button_type, const eButtonMode button_mode, const ePositionMode button_position_mode):
	UI_Object(button_parent, button_rect, distance_bottom_right, button_position_mode, NO_AUTO_SIZE),
	radio(NULL), //?
	forcedPress(false),
	moved(false),
	isOriginalPosition(false),
	hasBitmap(true),
	wasPressed(false),
	gradient(0),
	pressdepth(0),
	font(NULL_FONT),
	button(button_type),
	frameNumber(0),
	statusFlags(0),
	text(NULL)
{
	// TODO size aus Bitmap bestimmen!
	switch(button_mode)
	{
		case NO_BUTTON_MODE:statusFlags |= BF_NOT_CLICKABLE;break;
		case STATIC_BUTTON_MODE:statusFlags |= BF_STATIC;break;
		case PRESS_BUTTON_MODE:break;
		case PUSH_BUTTON_MODE:statusFlags |= BF_REPEATS;break;
		case TAB_BUTTON_MODE:statusFlags |= BF_IS_TAB; statusFlags |= BF_STATIC;break;
		default:break;
	} 
//	startRect = getRelativeRect();
//	targetRect = getRelativeRect();
//	filledHeight = getHeight();
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

// Render button.  How it draws exactly depends on it's current state.
void UI_Button::draw(DC* dc) const
{
	if(!isShown())
		return;
	if(checkForNeedRedraw())
	{

	eButtonAnimationPhase animation_phase;
	if((statusFlags & BF_STATIC)&&(isOriginalPosition))
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
}



//void UI_Button::set_hotkey_if_focus(int key)
//{
//	hotkey_if_focus = key;
//}
// reset anything that needs to be at the start of a new frame before processing
void UI_Button::frameReset()
{
//  statusFlags &= ~BF_HIGHLIGHTED;
	statusFlags &= ~BF_LEFT_CLICKED;
	statusFlags &= ~BF_DOUBLE_CLICKED;
// TODO bei -O3 bleiben die buttons gehighlighted... statusFlags = 0 behebt das, aber das wollen wir ja nicht :o
//	  hideToolTip(); ~~
}

void UI_Button::mouseHasEnteredArea()
{
	resetGradient();
	statusFlags |= BF_HIGHLIGHTED;
	if(statusFlags & BF_WAS_PRESSED)
	{
		if(isOriginalPosition)
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
		if(isOriginalPosition)
			setPressDepth(1);
		else
			setPressDepth(0);
		statusFlags &= ~BF_DOWN;
	}
}

void UI_Button::mouseLeftButtonPressed()
{
	if((statusFlags & BF_NOT_CLICKABLE)||((statusFlags & BF_IS_TAB)&&(isOriginalPosition==true)))
		return;
	statusFlags |= BF_WAS_PRESSED;
	statusFlags |= BF_DOWN;
	if(isOriginalPosition)
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
	if(statusFlags & BF_DOWN)
	{
		statusFlags &= ~BF_DOWN;
		statusFlags |= BF_LEFT_CLICKED;
		if(statusFlags & BF_STATIC)
		{
			if((isOriginalPosition)&&(!(statusFlags & BF_IS_TAB)))
				isOriginalPosition=false;
			else if(!isOriginalPosition)
				isOriginalPosition=true;
			setNeedRedrawMoved();
		}
		else
			setPressDepth(0);
		if(radio)
		{
			if(isOriginalPosition)
				radio->leftButtonPressed(this);
			else 
			if(!(statusFlags & BF_IS_TAB))
				radio->leftButtonReleased(this);
		}		
	}
}

void UI_Button::mouseRightButtonPressed()
{
	if((statusFlags & BF_NOT_CLICKABLE)||((statusFlags & BF_IS_TAB)&&(isOriginalPosition==true)))
		return;
	statusFlags |= BF_WAS_PRESSED;
	statusFlags |= BF_DOWN;
	if(isOriginalPosition)
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
	if(statusFlags & BF_DOWN)
	{
		statusFlags &= ~BF_DOWN;
		statusFlags |= BF_RIGHT_CLICKED;
		if(statusFlags & BF_STATIC)
		{
			if((isOriginalPosition)&&(!(statusFlags & BF_IS_TAB)))
				isOriginalPosition=false;
			else if(!isOriginalPosition)
				isOriginalPosition=true;
			setNeedRedrawMoved();
		}
		else
			setPressDepth(0);
		if(radio)
		{
			if(isOriginalPosition)
				radio->rightButtonPressed(this);
			else 
			if(!(statusFlags & BF_IS_TAB))
				radio->rightButtonReleased(this);
		}		
	}
}

UI_Object* UI_Button::checkTooltip() {
	if( (!isShown()) || (!getAbsoluteRect().Inside(mouse - Size(pressdepth, pressdepth))))
		return(0);
	return((UI_Object*)this);
}

UI_Object* UI_Button::checkHighlight()
{
	if( (!isShown()) || (!getAbsoluteRect().Inside(mouse - Size(pressdepth, pressdepth) )) )
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
	UI_Object::process();
	
	if(!hasBitmap)
	{
		adjustPositionAndSize(ADJUST_AFTER_CHILD_SIZE_WAS_CHANGED, text->getTextSize());
//		text->setSize(getSize());
	}

	if(statusFlags & BF_WAS_PRESSED)
		statusFlags |= BF_HIGHLIGHTED;

	if(statusFlags & BF_HIGHLIGHTED)
	{
		if(frameNumber<theme.lookUpButtonAnimation(button)->speed)
			frameNumber+=2;
		else frameNumber=0;
	}
	forcedPress = false;
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

const bool UI_Button::isCurrentlyActivated() const
{
//	if(!isShown())
//			return false;
	if ( (statusFlags & BF_STATIC) && (isOriginalPosition==true) )
		return true;
	else
		return false;
}

const bool UI_Button::isCurrentlyPressed() const
{
	if(!isShown())
		return false;
	if (statusFlags & BF_DOWN)
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
	if (!(statusFlags & BF_NOT_CLICKABLE))
	{
		if((statusFlags & BF_STATIC) && (!isOriginalPosition))
		{
			forcedPress=true;
			statusFlags |= BF_LEFT_CLICKED;
			statusFlags &= ~BF_WAS_PRESSED;
			isOriginalPosition=true;
//			if (statusFlags & BF_DOWN)
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
	if((statusFlags & BF_STATIC) && (isOriginalPosition))
	{
		isOriginalPosition = false;
		statusFlags &= ~BF_WAS_PRESSED;
//		statusFlags |= BF_LEFT_CLICKED; ?
		statusFlags &= ~BF_DOWN; // ~~
		setPressDepth(0);
	}
}
// reset the "pressed" timestamps

