#include "radio.hpp"
#include "configuration.hpp"
#include <math.h>

UI_Button::UI_Button(UI_Object* button_parent, 
		const Rect button_rect, 
		const Size distance_bottom_right, 
		const eButtonColorsType button_colors_type, 
		const bool has_bitmap, 
		const eButtonMode button_mode, 
		
		const eString button_text, 
		
		const ePositionMode button_position_mode, 
		const eFont button_font, 
		const eAutoSize button_auto_size) :
	UI_Object(button_parent, 
			button_rect, 
			distance_bottom_right, 
			button_position_mode == SPECIAL_BUTTON_LEFT ? DO_NOT_ADJUST : button_position_mode, 
			button_auto_size),
	radio(0), //?

	allowMoveByMouse(false),
	moved(false),
	isOriginalPosition(false),
	hasBitmap(has_bitmap),
	wasPressed(false),
	gradient(0),
	pressdepth(0),
	font(button_font),
	buttonColorsType(button_colors_type),
	statusFlags(0),
	frameNumber(0),
	text(NULL)
{
	switch(button_mode)
	{
		case NO_BUTTON_MODE:statusFlags |= BF_NOT_CLICKABLE;break;
		case STATIC_BUTTON_MODE:statusFlags |= BF_STATIC;break;
		case PRESS_BUTTON_MODE:break;
		case PUSH_BUTTON_MODE:statusFlags |= BF_REPEATS;break;
		case TAB_BUTTON_MODE:statusFlags |= BF_IS_TAB; statusFlags |= BF_STATIC;break;
		case BOGRAPH_BUTTON_MODE:statusFlags |= BF_IS_RECTANGLE;break;
		case CHECK_BUTTON_MODE:statusFlags |= BF_STATIC; statusFlags |= BF_IS_CHECKBUTTON;break;
		default:break;
	} 

	Size bitmap_size;
	if(hasBitmap)
		bitmap_size = Size(theme.lookUpBitmap(theme.lookUpButtonColors(buttonColorsType)->bitmap[0])->w, theme.lookUpBitmap(theme.lookUpButtonColors(buttonColorsType)->bitmap[0])->h);

	if(button_text!=NULL_STRING)
	{
		if((button_position_mode == SPECIAL_BUTTON_LEFT)/*||(button_position_mode == SPECIAL_BUTTON_ARRANGE_TOP_LEFT)||(button_position_mode == SPECIAL_BUTTON_ARRANGE_TOP_RIGHT) || (button_position_mode == SPECIAL_BUTTON_ARRANGE_LEFT) || (button_position_mode == SPECIAL_BUTTON_ARRANGE_RIGHT)*/)
			text = new UI_StaticText(this, button_text, Rect(Point(bitmap_size.getWidth()+2,0), getSize()), Size(0,0), theme.lookUpButtonColors(button_colors_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font, CENTER_LEFT); // TODO
		else if(button_auto_size == AUTO_SIZE)
			text = new UI_StaticText(this, button_text, Rect(Point(bitmap_size.getWidth()+2,0), getSize()), Size(0,0), theme.lookUpButtonColors(button_colors_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font, DO_NOT_ADJUST); // TODO
		else
			text = new UI_StaticText(this, button_text, Rect(Point(bitmap_size.getWidth()+0,0), getSize()), Size(0,0), theme.lookUpButtonColors(button_colors_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font, HORIZONTALLY_CENTERED); // TODO
	} else if(hasBitmap)
		setOriginalSize(bitmap_size);

}

UI_Button::UI_Button(UI_Object* button_parent, const Rect button_rect, const Size distance_bottom_right, const eButtonColorsType button_colors_type, const bool has_bitmap, const eButtonMode button_mode, const eString button_text, const eString button_tooltip, const ePositionMode button_position_mode, const eFont button_font, const eAutoSize button_auto_size) :
	UI_Object(button_parent, button_rect, distance_bottom_right, 
			button_position_mode == SPECIAL_BUTTON_LEFT ? DO_NOT_ADJUST : button_position_mode, button_auto_size),
	radio(0), //?

	allowMoveByMouse(false),
	moved(false),
	isOriginalPosition(false),
	hasBitmap(has_bitmap),
	wasPressed(false),
	gradient(0),
	pressdepth(0),
	font(button_font),
	buttonColorsType(button_colors_type),
	statusFlags(0),
	frameNumber(0),
	text(NULL)
{
	updateToolTip(button_tooltip);
	switch(button_mode)
	{
		case NO_BUTTON_MODE:statusFlags |= BF_NOT_CLICKABLE;break;
		case STATIC_BUTTON_MODE:statusFlags |= BF_STATIC;break;
		case PRESS_BUTTON_MODE:break;
		case PUSH_BUTTON_MODE:statusFlags |= BF_REPEATS;break;
		case TAB_BUTTON_MODE:statusFlags |= BF_IS_TAB; statusFlags |= BF_STATIC;break;
		case BOGRAPH_BUTTON_MODE:statusFlags |= BF_IS_RECTANGLE;break;
		case CHECK_BUTTON_MODE:statusFlags |= BF_STATIC; statusFlags |= BF_IS_CHECKBUTTON;break;
		default:break;
	} 

	Size bitmap_size;
	if(hasBitmap)
		bitmap_size = Size(theme.lookUpBitmap(theme.lookUpButtonColors(buttonColorsType)->bitmap[0])->w, theme.lookUpBitmap(theme.lookUpButtonColors(buttonColorsType)->bitmap[0])->h);

	if(button_text!=NULL_STRING)
	{
		if(button_position_mode == SPECIAL_BUTTON_LEFT)
			text = new UI_StaticText(this, button_text, Rect(Point(bitmap_size.getWidth()+4,1), getSize()), Size(0,0), theme.lookUpButtonColors(button_colors_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font, CENTER_LEFT); // TODO
		else if(button_auto_size == AUTO_SIZE)
			text = new UI_StaticText(this, button_text, Rect(Point(bitmap_size.getWidth()+4,1), getSize()), Size(0,0), theme.lookUpButtonColors(button_colors_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font, DO_NOT_ADJUST); // TODO
		else
			text = new UI_StaticText(this, button_text, Rect(Point(bitmap_size.getWidth()+4,1), getSize()), Size(0,0), theme.lookUpButtonColors(button_colors_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font, HORIZONTALLY_CENTERED); // TODO
	} else if(hasBitmap)
		setOriginalSize(bitmap_size);

}

UI_Button::UI_Button(UI_Object* button_parent, const Rect button_rect, const Size distance_bottom_right, const eButtonColorsType button_colors_type, const bool has_bitmap, const eButtonMode button_mode, const std::string& button_text, const ePositionMode button_position_mode, const eFont button_font, const eAutoSize button_auto_size) :
	UI_Object(button_parent, button_rect, distance_bottom_right, 
					button_position_mode == SPECIAL_BUTTON_LEFT ? DO_NOT_ADJUST : 
					button_position_mode, 
					button_auto_size),
	radio(0), //?

	allowMoveByMouse(false),
	moved(false),
	isOriginalPosition(false),
	hasBitmap(has_bitmap),
	wasPressed(false),
	gradient(0),
	pressdepth(0),
	font(button_font),
	buttonColorsType(button_colors_type),
	statusFlags(0),
	frameNumber(0),
	text(NULL)
{
	switch(button_mode)
	{
		case NO_BUTTON_MODE:statusFlags |= BF_NOT_CLICKABLE;break;
		case STATIC_BUTTON_MODE:statusFlags |= BF_STATIC;break;
		case PRESS_BUTTON_MODE:break;
		case PUSH_BUTTON_MODE:statusFlags |= BF_REPEATS;break;
		case TAB_BUTTON_MODE:statusFlags |= BF_IS_TAB; statusFlags |= BF_STATIC;break;
		case BOGRAPH_BUTTON_MODE:statusFlags |= BF_IS_RECTANGLE;break;
		case CHECK_BUTTON_MODE:statusFlags |= BF_STATIC; statusFlags |= BF_IS_CHECKBUTTON;break;
		default:break;
	} 
//	adjustPositionAndSize( theme.lookUpFont(font)->getTextExtent(button_text));//text->getTextSize());
//	startRect=getRelativeRect();
//	targetRect=getRelativeRect();
//	filledHeight=getHeight();

	Size bitmap_size;
	if(hasBitmap)
		bitmap_size = Size(theme.lookUpBitmap(theme.lookUpButtonColors(buttonColorsType)->bitmap[0])->w, theme.lookUpBitmap(theme.lookUpButtonColors(buttonColorsType)->bitmap[0])->h);
	if((button_text!="")||(!hasBitmap))
	{
		if(button_position_mode == SPECIAL_BUTTON_LEFT)
			text = new UI_StaticText(this, button_text, Rect(Point(bitmap_size.getWidth()+4,0), getSize()), Size(0,0), theme.lookUpButtonColors(button_colors_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font, CENTER_LEFT); // TODO
		else if(button_auto_size == AUTO_SIZE)
			text = new UI_StaticText(this, button_text, Rect(Point(bitmap_size.getWidth()+4,0), getSize()), Size(0,0), theme.lookUpButtonColors(button_colors_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font, DO_NOT_ADJUST); // TODO

		else
			text = new UI_StaticText(this, button_text, Rect(Point(bitmap_size.getWidth()+4,0), getSize()), Size(0,0), theme.lookUpButtonColors(button_colors_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font, TOTAL_CENTERED); // TODO
	} else if(hasBitmap)
		setOriginalSize(bitmap_size);

}

UI_Button::~UI_Button()
{
	if(UI_Button::currentButton == this)
		UI_Button::resetButton();
	delete text;
}

void UI_Button::reloadOriginalSize()
{
	if(text==NULL)
		setOriginalSize(Size(theme.lookUpBitmap(theme.lookUpButtonColors(buttonColorsType)->bitmap[0])->w, theme.lookUpBitmap(theme.lookUpButtonColors(buttonColorsType)->bitmap[0])->h));
	UI_Object::reloadOriginalSize();
	adjustSize(CHILD_WAS_CHANGED, getTextSize());
}

void UI_Button::setPressDepth(const unsigned int depth)
{
	if((pressdepth == depth)||(statusFlags & BF_IS_CHECKBUTTON))
		return;
//	UI_Object::addToProcessArray(this);
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


//		if(!(statusFlags & BF_NOT_CLICKABLE)) // ?? TODO
//		{
//			dc->setBrush(*theme.lookUpBrush(NULL_BRUSH));
//			dc->setPen(*theme.lookUpPen(DARK_BACKGROUND_PEN));
//			dc->DrawEdgedRectangle(Rect(getAbsolutePosition()-Size(1,1), getSize() + Size(2,2)));

//			dc->setBrush(*theme.lookUpBrush(NULL_BRUSH)); //?
//			dc->setPen(*theme.lookUpPen(RECTANGLE_PEN));
//			dc->DrawRoundedRectangle(Rect(getAbsolutePosition(), getSize()), 2);
//		}

		dc->setPen(Pen(dc->mixColor(
				*(theme.lookUpPen(theme.lookUpButtonColors(buttonColorsType)->startBorderPen[animation_phase])->getColor()), 
				*(theme.lookUpPen(theme.lookUpButtonColors(buttonColorsType)->endBorderPen[animation_phase])->getColor()), gradient), 
				theme.lookUpPen(theme.lookUpButtonColors(buttonColorsType)->startBorderPen[animation_phase])->getWidth(), 
				theme.lookUpPen(theme.lookUpButtonColors(buttonColorsType)->startBorderPen[animation_phase])->getStyle()));

		if((text!=NULL)||((text==NULL)&&(hasBitmap==false))) // textbutton/bitmaptextbutton or empty button (e.g. bograph)
		{
			dc->setBrush(Brush(dc->mixColor(
				*(theme.lookUpBrush(theme.lookUpButtonColors(buttonColorsType)->startBrush[animation_phase])->getColor()), 
				*(theme.lookUpBrush(theme.lookUpButtonColors(buttonColorsType)->endBrush[animation_phase])->getColor()), gradient),
				theme.lookUpBrush(theme.lookUpButtonColors(buttonColorsType)->startBrush[animation_phase])->getStyle()));
//TODO: eigene tab-button klasse
		
//			dc->setPen(*theme.lookUpPen(NULL_PEN));
//			if(statusFlags & BF_DOWN)
			if(text!=NULL)
				text->setTemporaryColor(dc->mixColor(
					*(theme.lookUpColor(theme.lookUpButtonColors(buttonColorsType)->startTextColor[animation_phase])), 
					*(theme.lookUpColor(theme.lookUpButtonColors(buttonColorsType)->endTextColor[animation_phase])), gradient));
// TODO TAB-BUTTON MODE
			if(pressdepth>0)
				dc->setPressedRectangle(true);
			else
				dc->setPressedRectangle(false);
			if(statusFlags & BF_IS_RECTANGLE)
//				dc->DrawRectangle(getAbsolutePosition()+Size(pressdepth, pressdepth), getSize()/*+Size(1,1)*/);
				dc->DrawEdgedRoundedRectangle(getAbsolutePosition()+Size(pressdepth, pressdepth), getSize()/*+Size(1,1)*/, 2);
			else
				dc->DrawEdgedRoundedRectangle(getAbsolutePosition()+Size(pressdepth, pressdepth), getSize()/*+Size(1,1)*/, 4);
			dc->setPressedRectangle(false);

		}
		if(hasBitmap)
		{
//			dc->DrawRectangle(Rect(getAbsolutePosition()-Size(1,1)+Size(pressdepth, pressdepth), getSize()+Size(2,2))); // kasterl ums bitmap
			dc->DrawBitmap(theme.lookUpBitmap(theme.lookUpButtonColors(buttonColorsType)->bitmap[animation_phase]), getAbsolutePosition()+Size(((statusFlags&BF_IS_CHECKBUTTON)==BF_IS_CHECKBUTTON)*2+pressdepth, pressdepth + (getHeight() > theme.lookUpBitmap(theme.lookUpButtonColors(buttonColorsType)->bitmap[animation_phase])->h?(getHeight()-theme.lookUpBitmap(theme.lookUpButtonColors(buttonColorsType)->bitmap[animation_phase])->h)/2:0)));
		} 
 // empty rectangle (e.g. bograph)
	
/*		if((animation_phase==PRESSED_BUTTON_PHASE)||(animation_phase==PRESSED_HIGHLIGHTED_BUTTON_PHASE))
		{
			dc->setPen(Pen(*(theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH)->getColor()),1,SOLID_PEN_STYLE));
			dc->DrawLine(getAbsolutePosition() + Point(0, getHeight()-1), getAbsolutePosition() + Point(getWidth(), getHeight()-1));
			dc->DrawLine(getAbsolutePosition() + Point(getWidth()-1, 0), getAbsolutePosition() + Point(getWidth()-1, getHeight()));
		} ??? */

//		dc->DrawRectangle(Rect(originalButtonRect.getTopLeft() + getAbsolutePosition(), originalButtonRect.getSize()));
	}
	UI_Object::draw(dc);	
//	dc->setPen(*theme.lookUpPen(RECTANGLE_PEN));
//	dc->DrawRectangle(getAbsoluteRect());

}



//void UI_Button::set_hotkey_if_focus(int key)
//{
//	hotkey_if_focus = key;
//}
// reset anything that needs to be at the start of a new frame before processing
void UI_Button::frameReset()
{
	statusFlags &= ~BF_HIGHLIGHTED;
	statusFlags &= ~BF_LEFT_CLICKED;
	statusFlags &= ~BF_DOUBLE_CLICKED;
// TODO bei -O3 bleiben die buttons gehighlighted... statusFlags = 0 behebt das, aber das wollen wir ja nicht :o
//	  hideToolTip(); ~~
}

void UI_Button::mouseHasEnteredArea()
{
//	UI_Object::addToProcessArray(this);
	resetGradient();
	if(!(statusFlags & BF_HIGHLIGHTED))
	{
		statusFlags |= BF_JUST_HIGHLIGHTED;
		UI_Object::theme.playSound(MOUSEOVER_SOUND, /*(getAbsolutePosition() + getSize()/2)*/mouse.x);
	}
	statusFlags |= BF_HIGHLIGHTED;
	if(statusFlags & BF_WAS_PRESSED)
	{
		if(isOriginalPosition)
			setPressDepth(0);
		else
			setPressDepth(1);
		statusFlags |= BF_DOWN;
	}
	if(buttonColorsType == TEXT_BUTTON)
		SDL_SetCursor(UI_Object::theme.lookUpCursor(HAND_CURSOR, 0));
}

void UI_Button::mouseHasLeftArea()
{
//	UI_Object::addToProcessArray(this);
	statusFlags &= ~BF_HIGHLIGHTED;
	if(statusFlags & BF_WAS_PRESSED)
	{
		if(isOriginalPosition)
			setPressDepth(1);
		else
			setPressDepth(0);
		statusFlags &= ~BF_DOWN;
	}
	if(buttonColorsType == TEXT_BUTTON)
		SDL_SetCursor(UI_Object::theme.lookUpCursor(ARROW_CURSOR, 0));
}

void UI_Button::mouseLeftButtonPressed()
{
	if((statusFlags & BF_NOT_CLICKABLE)||((statusFlags & BF_IS_TAB)&&(isOriginalPosition==true)))
		return;
//	UI_Object::addToProcessArray(this);
	statusFlags |= BF_WAS_PRESSED;
	statusFlags |= BF_DOWN;
	if(isOriginalPosition)
		setPressDepth(0);
	else
		setPressDepth(1);
	if(allowMoveByMouse)
	{
		UI_Button::mouseMovePoint = mouse;
		UI_Button::moveByMouse = true;
	}
}

void UI_Button::mouseLeftButtonReleased()
{
	if((statusFlags & BF_NOT_CLICKABLE))
		return;
//	UI_Object::addToProcessArray(this);
	statusFlags &= ~BF_WAS_PRESSED;
	if(statusFlags & BF_DOWN)
	{
		statusFlags &= ~BF_DOWN;
		statusFlags |= BF_LEFT_CLICKED;
		if(statusFlags & BF_STATIC)
		{
			UI_Object::theme.playSound(CLICKED_SOUND, mouse.x);
			if((isOriginalPosition)&&(!(statusFlags & BF_IS_TAB)))
				isOriginalPosition=false;
			else if(!isOriginalPosition)
				isOriginalPosition=true;
			wasPressed = true;
			UI_Object::focus = this;
		}
		else
		{
			UI_Object::theme.playSound(CLICK_SOUND, mouse.x);
			setPressDepth(0);
		}
		if(radio)
		{
			if(isOriginalPosition)
				radio->leftButtonPressed(this);
			else 
			if(!(statusFlags & BF_IS_TAB))
				radio->leftButtonReleased(this);
		}		
	}
	if(allowMoveByMouse)
	{
		UI_Button::mouseMovePoint = Point(0,0);
		UI_Button::moveByMouse = false;
	}
}

void UI_Button::mouseRightButtonPressed()
{
	if((statusFlags & BF_NOT_CLICKABLE)||(statusFlags & BF_STATIC)||((statusFlags & BF_IS_TAB)&&(isOriginalPosition==true)))
		return;
//	UI_Object::addToProcessArray(this);
	statusFlags |= BF_WAS_PRESSED;
	statusFlags |= BF_DOWN;
	
	if(isOriginalPosition)
		setPressDepth(0);
	else
		setPressDepth(1);
//	setNeedRedrawMoved();
}

void UI_Button::mouseRightButtonReleased()
{
	if((statusFlags & BF_NOT_CLICKABLE)||(statusFlags & BF_STATIC))
		return;
//	UI_Object::addToProcessArray(this);
	statusFlags &= ~BF_WAS_PRESSED;
	if(statusFlags & BF_DOWN)
	{
		statusFlags &= ~BF_DOWN;
		statusFlags |= BF_RIGHT_CLICKED;
	
		if(statusFlags & BF_STATIC)
		{
	/*		UI_Object::theme.playSound(CLICKED_SOUND, (getAbsolutePosition() + getSize()/2).x);
			if((isOriginalPosition)&&(!(statusFlags & BF_IS_TAB)))
				isOriginalPosition=false;
			else if(!isOriginalPosition)
				isOriginalPosition=true;
			setNeedRedrawMoved();*/
		}
		else
		{
			UI_Object::theme.playSound(CLICK_SOUND, mouse.x);
			setPressDepth(0);
		}
	/*	if(radio)
		{
			if(isOriginalPosition)
				radio->rightButtonPressed(this);
			else 
			if(!(statusFlags & BF_IS_TAB))
				radio->rightButtonReleased();
		}		 // TODO?*/
	}
}

UI_Object* UI_Button::checkToolTip() 
{
	if( (!isShown()) || (!getAbsoluteRect().Inside(mouse - Size(pressdepth, pressdepth))))
		return(NULL);
	return((UI_Object*)this);
}

UI_Object* UI_Button::checkHighlight()
{
	if(!isShown())
		return(NULL);
	if(!getAbsoluteRect().Inside(mouse - Size(pressdepth, pressdepth) ))
		return(UI_Object::checkHighlight());
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
	
	if((!(statusFlags & BF_HIGHLIGHTED))&&((theme.lookUpButtonColors(buttonColorsType)->type!=NO_ANIMATION)))//||(!uiConfiguration.isGlowingButtons())))
		gradient += (100 - gradient) / 6 + 1;
	else 
	if((statusFlags & BF_HIGHLIGHTED) && (!uiConfiguration.isGlowingButtons()))
	{
		if(gradient > (gradient) / 10 + 1)
			gradient -= (gradient) / 10 + 1;
		else gradient = 0;
	}
	else
	{
//		UI_Object::addToNextProcessArray(this);
		switch(theme.lookUpButtonColors(buttonColorsType)->type)
		{	
			case NO_ANIMATION:/*if(gradient < 100) ++gradient;else gradient = 100;*/break;
			case JUMPY_COLORS_ANIMATION:gradient=(frameNumber%theme.lookUpButtonColors(buttonColorsType)->speed)*100/theme.lookUpButtonColors(buttonColorsType)->speed;break;
			case GLOWING_ANIMATION:gradient=(unsigned int)(50*(sin(3.141*frameNumber/theme.lookUpButtonColors(buttonColorsType)->speed)+1));break;
			case BLINKING_ANIMATION:if(frameNumber<theme.lookUpButtonColors(buttonColorsType)->speed/2) gradient=0;else gradient=100;break;
			default:break;
		}
	}

	if(gradient > 100)
	{
		gradient = 100;
	}

	if(gradient != oldgradient)
	{
		setNeedRedrawMoved();//NotMovedFirstChild();
//		UI_Object::addToNextProcessArray(this);
	}

	Point absoluteCoord = getRelativePosition();
	Size absoluteSize = getSize();
	UI_Object::process();
	
	if(!hasBitmap)
	{
//		adjustPositionAndSize(ADJUST_AFTER_CHILD_SIZE_WAS_CHANGED, text->getTextSize());
//		text->setSize(getSize());
	}

	if(statusFlags & BF_WAS_PRESSED)
		statusFlags |= BF_HIGHLIGHTED;

	if(statusFlags & BF_HIGHLIGHTED)
	{
		if(frameNumber<theme.lookUpButtonColors(buttonColorsType)->speed)
			frameNumber+=2;
		else frameNumber=0;
	}
}

const Size UI_Button::getTextSize() const 
{
	if(text==NULL)
		return(Size(0,0));
	else
		return(text->getTextSize());
}

const unsigned int UI_Button::getTextWidth() const 
{
	if(text==NULL)
		return(0);
	else
		return(text->getTextSize().getWidth());
}

const unsigned int UI_Button::getTextHeight() const {
	if(text==NULL)
		return(0);
	else
		return(text->getTextSize().getHeight());
}

void UI_Button::updateText(const std::string& utext) 
{
	if(text==NULL)
		return;
	text->updateText(utext);
}

void UI_Button::updateText(const eString utext) 
{
	if(text==NULL)
		return;
	text->updateText(utext);
}


void UI_Button::doHighlight(const bool high_light) 
{
	if(high_light)
		statusFlags |= BF_HIGHLIGHTED;
	else
		statusFlags &= ~BF_HIGHLIGHTED;
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

// changes static buttons
void UI_Button::check(const bool is_checked)
{
        if(isCurrentlyActivated() != is_checked)
        {
                if(!is_checked)
                        forceUnpress();
                else
                        forcePress();
                setNeedRedrawMoved();
        }
}

// Force button to get pressed, doesn't work for non-static buttons yet
void UI_Button::forcePress()
{
        if (!(statusFlags & BF_NOT_CLICKABLE))
        {
                if((statusFlags & BF_STATIC) && (!isOriginalPosition))
                {
                        statusFlags |= BF_LEFT_CLICKED;
                        statusFlags &= ~BF_WAS_PRESSED;
                        isOriginalPosition=true;
//                      if (statusFlags & BF_DOWN)
//                      {
                        statusFlags &= ~BF_DOWN;
                        setPressDepth(1);
                } else if((!(statusFlags & BF_STATIC)) && (!(statusFlags & BF_DOWN)))
                {
                        statusFlags |= BF_LEFT_CLICKED | BF_DOWN;                        //~?
                        UI_Object::theme.playSound(CLICK_SOUND, (getAbsolutePosition() + getSize()/2).x);
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
//              statusFlags |= BF_LEFT_CLICKED;
                statusFlags &= ~BF_DOWN; // ~~
                setPressDepth(0);
        }
}


const bool UI_Button::addKey(unsigned int key, unsigned int mod)
{
	if(((key == SDLK_KP_ENTER)||(key == SDLK_RETURN))&& (!(mod & (KMOD_LALT | KMOD_RALT | KMOD_ALT))))
	{
		forcePress();
		return(true);
	}
	return(false);	
}

void UI_Button::resetButton()
{
	if(UI_Button::currentButton!=NULL)
		UI_Button::currentButton->frameReset();
	UI_Button::currentButton=NULL;
	UI_Button::currentButtonHasAlreadyLeft=false;
	UI_Button::currentButtonPressed=false;
	UI_Button::mouseMovePoint = Point(0, 0);
	UI_Button::moveByMouse = false;
	SDL_SetCursor(UI_Object::theme.lookUpCursor(ARROW_CURSOR, 0));
}


bool UI_Button::currentButtonPressed = false;
bool UI_Button::currentButtonHasAlreadyLeft = false;
UI_Button* UI_Button::currentButton = NULL;
bool UI_Button::moveByMouse = false;
Point UI_Button::mouseMovePoint = Point();
bool UI_Button::doClickedSound = false;
