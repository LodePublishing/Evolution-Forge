#include "button.hpp"
#include "radio.hpp"

UI_Button::UI_Button(UI_Object* button_parent, 
		const Rect button_rect, 
		const Size distance_bottom_right, 
		const eButtonColorsType button_colors_type, 
//		const bool has_bitmap, 
		const eBitmap button_bitmap,
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
	radio(NULL), //?

	allowMoveByMouse(false),
	moved(false),
	isOriginalPosition(false),
//	hasBitmap(has_bitmap),
	wasPressed(false),
	gradient(0),
	oldGradient(999),
	pressdepth(0),
	font(button_font),
	buttonColorsType(button_colors_type),
	bitmap(button_bitmap!=NULL_BITMAP?new UI_Bitmap(this, Rect(), Size(), button_bitmap, VERTICALLY_CENTERED):NULL),
	statusFlags(0),
	frameNumber(0),
	text(NULL)
{
// 	if(((theme.lookUpButtonColors(buttonColorsType)->startBrush[0] == TRANSPARENT_BRUSH) && (text != NULL))||(bitmap)) TODO
	switch(button_mode)
	{
		case NO_BUTTON_MODE:statusFlags |= BF_NOT_CLICKABLE;break;
		case STATIC_BUTTON_MODE:statusFlags |= BF_STATIC;break;
		case PRESS_BUTTON_MODE:break;
		case PUSH_BUTTON_MODE:statusFlags |= BF_REPEATS;break;
		case TAB_BUTTON_MODE:statusFlags |= BF_IS_TAB; statusFlags |= BF_STATIC;break;
		case TOP_TAB_BUTTON_MODE:statusFlags |= BF_IS_TOP_TAB; statusFlags |= BF_IS_TAB; statusFlags |= BF_STATIC;break;
		case BOGRAPH_BUTTON_MODE:statusFlags |= BF_IS_RECTANGLE;break;
		case CHECK_BUTTON_MODE:statusFlags |= BF_STATIC; statusFlags |= BF_IS_CHECKBUTTON;break;
		default:break;
	} 

	Size bitmap_size;
	if(bitmap)
	{
		bitmap_size = bitmap->getBitmapSize();
		isClipped = true;
//		bitmap->setPositionParent(this);
	}

	if(button_text!=NULL_STRING)
	{
		if((button_position_mode == SPECIAL_BUTTON_LEFT)/*||(button_position_mode == SPECIAL_BUTTON_ARRANGE_TOP_LEFT)||(button_position_mode == SPECIAL_BUTTON_ARRANGE_TOP_RIGHT) || (button_position_mode == SPECIAL_BUTTON_ARRANGE_LEFT) || (button_position_mode == SPECIAL_BUTTON_ARRANGE_RIGHT)*/)
			text = new UI_StaticText(this, button_text, Rect(Point(bitmap_size.getWidth()+4,0), getSize()), Size(0,0), theme.lookUpButtonColors(button_colors_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font, CENTER_LEFT); // TODO
		else if(button_auto_size == AUTO_SIZE)
			text = new UI_StaticText(this, button_text, Rect(Point(bitmap_size.getWidth()+4,0), getSize()), Size(0,0), theme.lookUpButtonColors(button_colors_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font, DO_NOT_ADJUST); // TODO
		else
			text = new UI_StaticText(this, button_text, Rect(Point(bitmap_size.getWidth(), 1), getSize()), Size(0,0), theme.lookUpButtonColors(button_colors_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font, TOTAL_CENTERED); // TODO
	} else if(bitmap)
		setOriginalSize(bitmap_size);
}

UI_Button::UI_Button(UI_Object* button_parent, 
		const Rect button_rect, 
		const Size distance_bottom_right, 
		const eButtonColorsType button_colors_type, /*const bool has_bitmap*/ const eBitmap button_bitmap, const eButtonMode button_mode, const eString button_text, const eString button_tooltip, const ePositionMode button_position_mode, const eFont button_font, const eAutoSize button_auto_size) :
	UI_Object(button_parent, button_rect, distance_bottom_right, 
			button_position_mode == SPECIAL_BUTTON_LEFT ? DO_NOT_ADJUST : button_position_mode, button_auto_size),
	radio(NULL), //?

	allowMoveByMouse(false),
	moved(false),
	isOriginalPosition(false),
//	hasBitmap(has_bitmap),
	wasPressed(false),
	gradient(0),
	oldGradient(999),
	pressdepth(0),
	font(button_font),
	buttonColorsType(button_colors_type),
	bitmap(button_bitmap!=NULL_BITMAP?new UI_Bitmap(this, Rect(), Size(), button_bitmap, VERTICALLY_CENTERED):NULL),
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
		case TOP_TAB_BUTTON_MODE:statusFlags |= BF_IS_TOP_TAB; statusFlags |= BF_IS_TAB; statusFlags |= BF_STATIC;break;
		case BOGRAPH_BUTTON_MODE:statusFlags |= BF_IS_RECTANGLE;break;
		case CHECK_BUTTON_MODE:statusFlags |= BF_STATIC; statusFlags |= BF_IS_CHECKBUTTON;break;
		default:break;
	} 

	Size bitmap_size;
	if(bitmap)
	{
		bitmap_size = bitmap->getBitmapSize();
		isClipped = true;
	//	bitmap->setPositionParent(this);
	}

	if(button_text!=NULL_STRING)
	{
		if(button_position_mode == SPECIAL_BUTTON_LEFT)
			text = new UI_StaticText(this, button_text, Rect(Point(bitmap_size.getWidth()+4,0), getSize()), Size(0,0), theme.lookUpButtonColors(button_colors_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font, CENTER_LEFT); // TODO
		else if(button_auto_size == AUTO_SIZE)
			text = new UI_StaticText(this, button_text, Rect(Point(bitmap_size.getWidth()+4,0), getSize()), Size(0,0), theme.lookUpButtonColors(button_colors_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font, DO_NOT_ADJUST); // TODO
		else
			text = new UI_StaticText(this, button_text, Rect(Point(bitmap_size.getWidth(), 1), getSize()), Size(0,0), theme.lookUpButtonColors(button_colors_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font, TOTAL_CENTERED); // TODO
	} else if(bitmap)
		setOriginalSize(bitmap_size);

}

UI_Button::UI_Button(UI_Object* button_parent, const Rect button_rect, 
		const Size distance_bottom_right, 
		const eButtonColorsType button_colors_type, /*const bool has_bitmap*/ const eBitmap button_bitmap, const eButtonMode button_mode, const std::string& button_text, const ePositionMode button_position_mode, const eFont button_font, const eAutoSize button_auto_size) :
	UI_Object(button_parent, button_rect, distance_bottom_right,
					button_position_mode == SPECIAL_BUTTON_LEFT ? DO_NOT_ADJUST : 
					button_position_mode, 
					button_auto_size),
	radio(NULL), //?

	allowMoveByMouse(false),
	moved(false),
	isOriginalPosition(false),
//	hasBitmap(has_bitmap),
	wasPressed(false),
	gradient(0),
	oldGradient(999),
	pressdepth(0),
	font(button_font),
	buttonColorsType(button_colors_type),
	bitmap(button_bitmap!=NULL_BITMAP?new UI_Bitmap(this, Rect(), Size(), button_bitmap, VERTICALLY_CENTERED):NULL),
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
		case TOP_TAB_BUTTON_MODE:statusFlags |= BF_IS_TOP_TAB; statusFlags |= BF_IS_TAB; statusFlags |= BF_STATIC;break;
		case BOGRAPH_BUTTON_MODE:statusFlags |= BF_IS_RECTANGLE;break;
		case CHECK_BUTTON_MODE:statusFlags |= BF_STATIC; statusFlags |= BF_IS_CHECKBUTTON;break;
		default:break;
	} 
//	adjustPositionAndSize( theme.lookUpFont(font)->getTextExtent(button_text));//text->getTextSize());
//	startRect=getRelativeRect();
//	targetRect=getRelativeRect();
//	filledHeight=getHeight();

	
	Size bitmap_size;
	if(bitmap)
	{
		bitmap_size = bitmap->getBitmapSize();
		isClipped = true;
//		bitmap->setPositionParent(this);
	}
	if((button_text!="")||(!bitmap))
	{
		if(button_position_mode == SPECIAL_BUTTON_LEFT)
			text = new UI_StaticText(this, button_text, Rect(Point(bitmap_size.getWidth()+4,0), getSize()), Size(0,0), theme.lookUpButtonColors(button_colors_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font, CENTER_LEFT); // TODO
		else if(button_auto_size == AUTO_SIZE)
			text = new UI_StaticText(this, button_text, Rect(Point(bitmap_size.getWidth()+4,0), getSize()), Size(0,0), theme.lookUpButtonColors(button_colors_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font, DO_NOT_ADJUST); // TODO

		else
			text = new UI_StaticText(this, button_text, Rect(Point(bitmap_size.getWidth(), 1), getSize()), Size(0,0), theme.lookUpButtonColors(button_colors_type)->startTextColor[PRESSED_BUTTON_PHASE], button_font, TOTAL_CENTERED); // TODO
	} else if(bitmap)
		setOriginalSize(bitmap_size);

}

UI_Button::~UI_Button()
{
	if(UI_Button::currentButton == this)
		UI_Button::resetButton();
	delete bitmap; // ~_~
	delete text;
}

void UI_Button::reloadOriginalSize()
{
	if((text==NULL)&&(bitmap))
		setOriginalSize(bitmap->getBitmapSize());
	UI_Object::reloadOriginalSize();
	adjustSize(CHILD_WAS_CHANGED, getTextSize());
}

void UI_Button::setPressDepth(const unsigned int depth)
{
	if((pressdepth == depth)||(statusFlags & BF_NOT_CLICKABLE))
		return;
//	UI_Object::addToProcessArray(this);
	pressdepth = depth;
	wasPressed = true;
	if(statusFlags & BF_IS_CHECKBUTTON)
	{
		if(pressdepth == 0)
			bitmap->setChecked(false);
		else bitmap->setChecked(true);
	}
}
void UI_Button::object_info()
{
	toErrorLog("ui_button");
}
	
// Render button.  How it draws exactly depends on it's current state.
void UI_Button::draw() const
{
//	toErrorLog("button draw");
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

	if((text!=NULL)||((text==NULL)&&(!bitmap))) // textbutton/bitmaptextbutton or empty button (e.g. bograph)
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
				*(theme.lookUpColor(theme.lookUpButtonColors(buttonColorsType)->endTextColor[animation_phase])), 
				*(theme.lookUpColor(theme.lookUpButtonColors(buttonColorsType)->startTextColor[animation_phase])), gradient));
// TODO TAB-BUTTON MODE
		if(pressdepth>0)
			dc->setPressedRectangle(true);
		else
			dc->setPressedRectangle(false);
		if(statusFlags & BF_IS_RECTANGLE)
//			dc->DrawRectangle(getAbsolutePosition()+Size(pressdepth, pressdepth), getSize()/*+Size(1,1)*/);
			dc->DrawEdgedRoundedRectangle(Point(0, 0), getSize(), 2);
		else if(statusFlags & BF_IS_TOP_TAB)
			dc->DrawTabRectangle(Point(0,0), getSize());
		else
			dc->DrawEdgedRoundedRectangle(Point(0,0), getSize(), 4);
		dc->setPressedRectangle(false);
	}

 // empty rectangle (e.g. bograph)
	
/*		if((animation_phase==PRESSED_BUTTON_PHASE)||(animation_phase==PRESSED_HIGHLIGHTED_BUTTON_PHASE))
		{
			dc->setPen(Pen(*(theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH)->getColor()),1,SOLID_PEN_STYLE));
			dc->DrawLine(getAbsolutePosition() + Point(0, getHeight()-1), getAbsolutePosition() + Point(getWidth(), getHeight()-1));
			dc->DrawLine(getAbsolutePosition() + Point(getWidth()-1, 0), getAbsolutePosition() + Point(getWidth()-1, getHeight()));
		} ??? */

//		dc->DrawRectangle(Rect(originalButtonRect.getTopLeft() + getAbsolutePosition(), originalButtonRect.getSize()));
	UI_Object::draw();
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
		UI_Object::sound.playSound(MOUSEOVER_SOUND, /*(getAbsolutePosition() + getSize()/2)*/mouse.x);
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
	if((buttonColorsType == TEXT_BUTTON) || (buttonColorsType == VISITED_TEXT_BUTTON))
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
			UI_Object::sound.playSound(CLICKED_SOUND, mouse.x);
			if((isOriginalPosition)&&(!(statusFlags & BF_IS_TAB)))
				isOriginalPosition=false;
			else if(!isOriginalPosition)
				isOriginalPosition=true;
			wasPressed = true;
			UI_Object::focus = this;
		}
		else
		{
			UI_Object::sound.playSound(CLICK_SOUND, mouse.x);
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
			UI_Object::sound.playSound(CLICK_SOUND, mouse.x);
//			UI_Object::sound.playSound(CLICKED_SOUND, (getAbsolutePosition() + getSize()/2).x);
	/*		if((isOriginalPosition)&&(!(statusFlags & BF_IS_TAB)))
				isOriginalPosition=false;
			else if(!isOriginalPosition)
				isOriginalPosition=true;*/
		}
		else
		{
			UI_Object::sound.playSound(CLICK_SOUND, mouse.x);
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
	if( (!isShown()) || (!getAbsoluteRect().isTopLeftCornerInside(mouse)))
		return(NULL);
	return((UI_Object*)this);
}

UI_Object* UI_Button::checkHighlight()
{
	if(!isShown())
		return(NULL);
	if(!getAbsoluteRect().isTopLeftCornerInside(mouse))
		return(UI_Object::checkHighlight());
	return((UI_Object*)this);
}

void UI_Button::process()
{
	// TODO evtl Animation fuer jede Phase in die config datei
	// dann waere sowas moeglich, dass ich maus reinfahr und das langsam verblasst
	// evtl auch einfach brightencolor ueberlegen...

	if(wasPressed)
	{
		wasPressed=false;
//		setNeedRedrawMoved();
	} // TODO

//		gradient = 100;
//	else
	if(!(statusFlags & BF_HIGHLIGHTED))
		gradient += (100 - gradient) / 6 + 1;
	else 
	if((!uiConfiguration.isGlowingButtons())||(theme.lookUpButtonColors(buttonColorsType)->type==NO_ANIMATION))
	{
		if(gradient > gradient / 10 + 1)
			gradient -= gradient / 10 + 1;
		else 
			gradient = 0;
	} 
	else
	{
//		UI_Object::addToNextProcessArray(this);
		switch(theme.lookUpButtonColors(buttonColorsType)->type)
		{	
//			case NO_ANIMATION:if(gradient < 100) ++gradient;else gradient = 100;break;
			case JUMPY_COLORS_ANIMATION:gradient=(frameNumber%theme.lookUpButtonColors(buttonColorsType)->speed)*100/theme.lookUpButtonColors(buttonColorsType)->speed;break;
			case GLOWING_ANIMATION:gradient=(unsigned int)(50*(sin(3.141*frameNumber/theme.lookUpButtonColors(buttonColorsType)->speed)+1));break;
			case BLINKING_ANIMATION:if(frameNumber<theme.lookUpButtonColors(buttonColorsType)->speed/2) gradient=0;else gradient=100;break;
			default:break;
		}
	}

	if(gradient > 100)
		gradient = 100;
	
	if(gradient != oldGradient)
	{
		redrawWholeObject();
		oldGradient = gradient;
	}

	Point absoluteCoord = getRelativePosition();
	Size absoluteSize = getSize();
	UI_Object::process();
	
	if(statusFlags & BF_WAS_PRESSED)
		statusFlags |= BF_HIGHLIGHTED;

	if(statusFlags & BF_HIGHLIGHTED)
	{
		if(frameNumber<theme.lookUpButtonColors(buttonColorsType)->speed)
			frameNumber+=2;
		else frameNumber=0;
	}
	if(bitmap)
	{
		bitmap->clipRect = clipRect;
		bitmap->setBrightness(100-gradient/2);
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
	if(gradient != 0)
	{
		gradient=0;
		frameNumber=0;
		makePufferInvalid();
	}
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
		makePufferInvalid();
        }
}

// Force button to get pressed, doesn't work for non-static buttons yet
void UI_Button::forcePress(const bool click_message)
{
        if (!(statusFlags & BF_NOT_CLICKABLE))
        {
                if((statusFlags & BF_STATIC) && (!isOriginalPosition))
                {
			if(click_message)
	                        statusFlags |= BF_LEFT_CLICKED;
                        statusFlags &= ~BF_WAS_PRESSED;
                        isOriginalPosition=true;
//                      if (statusFlags & BF_DOWN)
//                      {
                        statusFlags &= ~BF_DOWN;
                        setPressDepth(1);
                } else if((!(statusFlags & BF_STATIC)) && (!(statusFlags & BF_DOWN)))
                {
			if(click_message)
			{
	                        statusFlags |= BF_LEFT_CLICKED;
	                        UI_Object::sound.playSound(CLICK_SOUND, (getAbsolutePosition() + getSize()/2).x);
			}
			statusFlags |= BF_DOWN;                        //~?
                        setPressDepth(1); //?
                }
        }
}

void UI_Button::forceUnpress(const bool click_message)
{
        if((statusFlags & BF_STATIC) && (isOriginalPosition))
        {
                isOriginalPosition = false;
                statusFlags &= ~BF_WAS_PRESSED;
//		if(click_message)
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
	UI_Button::wasResetted = true;
	SDL_SetCursor(UI_Object::theme.lookUpCursor(ARROW_CURSOR, 0));
}


bool UI_Button::currentButtonPressed = false;
bool UI_Button::currentButtonHasAlreadyLeft = false;
UI_Button* UI_Button::currentButton = NULL;
bool UI_Button::moveByMouse = false;
Point UI_Button::mouseMovePoint = Point();
bool UI_Button::doClickedSound = false;
bool UI_Button::wasResetted = false;
