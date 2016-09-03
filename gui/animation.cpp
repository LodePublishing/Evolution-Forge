#include "animation.hpp"

UI_Animation::UI_Animation(UI_Object* animation_parent, 
				const Rect& animation_rect,
				const Size distance_bottom_right, 
				const eAnimation my_animation,
				const eBitmap end_bitmap,
				const eString animation_text,
				const bool animation_border,
				const ePositionMode animation_position_mode) :
	UI_Object(animation_parent, animation_rect,  distance_bottom_right, animation_position_mode, NO_AUTO_SIZE),
	animation(my_animation),
	animationSurface(NULL),
	endBitmap(new UI_Bitmap(this, Rect(), Size(), end_bitmap)),
	animationText(new UI_StaticText(this, animation_text, Rect(Point(0,-10),Size()), Size(), BRIGHT_TEXT_COLOR, SMALL_SHADOW_BOLD_FONT)),
	loop(false),
	border(animation_border),
	wasStarted(false)
{
	endBitmap->Hide();
	animationText->Hide();
	setAnimationFile(my_animation);
	setDrawType(SOLID_OBJECT);
}

UI_Animation::~UI_Animation()
{
	delete endBitmap;
	delete animationText;
	lockAnimation(false);
}

void UI_Animation::lockAnimation(const bool lock)
{
//	SMPEG* temp = UI_Object::theme.lookUpAnimation(animation);
//	if(temp)
//		them->lock = lock;
//		TODO
}

const bool UI_Animation::isLocked(const eAnimation id) const
{
//	SMPEG* temp = UI_Object::theme.lookUpAnimation(id);
//	if(temp)
//		return(UI_Object::theme.lookUpAnimation(id)->lock);
//	else 
	return(false);
}

void update(SDL_Surface *screen, Sint32 x, Sint32 y, Uint32 w, Uint32 h)
{
// => process
}

const bool UI_Animation::setAnimationFile(const eAnimation id)
{
	if(isLocked(id))
		return(false);
	lockAnimation(false);
	animation = id;
	lockAnimation(true);
	
	setSize(Size(UI_Object::theme.lookUpAnimationInfo(animation).width, UI_Object::theme.lookUpAnimationInfo(animation).height));	
	SDL_Surface* temp = SDL_CreateRGBSurface( SDL_SWSURFACE, getWidth(), getHeight(), 32, dc->getSurface()->format->Rmask, dc->getSurface()->format->Gmask, dc->getSurface()->format->Bmask, dc->getSurface()->format->Amask );
	animationSurface = SDL_DisplayFormat( temp );
	SDL_FreeSurface(temp);
	
	SMPEG_setdisplay(UI_Object::theme.lookUpAnimation(animation), animationSurface, NULL, NULL);//update);
	SMPEG_scaleXY(UI_Object::theme.lookUpAnimation(animation), getWidth(), getHeight());
//	SMPEG_setdisplayregion(UI_Object::theme.lookUpAnimation(animation), 50, 50, 100, 100);

	

	/* Enable software bilinear filtering, if desired */
//	if ( bilinear_filtering ) {
//		SMPEG_Filter *filter;
//		filter = SMPEGfilter_bilinear();
///		filter = SMPEG_filter( mpeg, filter );
//		filter->destroy(filter);
//	}

        /* Set any special playback parameters */
//	if ( loop ) SMPEG_loop(mpeg, 1);

	/* Seek starting position */
//	if(seek) SMPEG_seek(mpeg, seek);

	/* Skip seconds to starting position */
//	if(skip) SMPEG_skip(mpeg, skip);
	
	return(true);
}

const bool UI_Animation::isPlaying() const
{
	return( SMPEG_status(UI_Object::theme.lookUpAnimation(animation)) == SMPEG_PLAYING );
}

const bool UI_Animation::isDone() const
{
	return((!isPlaying()) && wasStarted);
}

void UI_Animation::play()
{
	if(!isPlaying())
	{
		if(!wasStarted)
		{
			wasStarted = true;
			toErrorLog("Playing:");
			toErrorLog(getAbsoluteRect().toString());
			SMPEG_play(UI_Object::theme.lookUpAnimation(animation));
		}
	}
}

void UI_Animation::pause()
{
	if(isPlaying())
		SMPEG_pause(UI_Object::theme.lookUpAnimation(animation));
}

/*			  // Toggle filtering on/off
			  if ( bilinear_filtering ) {
			    SMPEG_Filter *filter = SMPEGfilter_null();
			    filter = SMPEG_filter( mpeg, filter );
			    filter->destroy(filter);
			    bilinear_filtering = 0;
			  } else {
			    SMPEG_Filter *filter = SMPEGfilter_bilinear();
			    filter = SMPEG_filter( mpeg, filter );
			    filter->destroy(filter);
			    bilinear_filtering = 1;
			  }*/

//			    SMPEG_skip(mpeg, 50);
//			  SMPEG_setvolume(mpeg, volume);
		  
/*case SDL_VIDEORESIZE: {
                        SDL_Surface *old_screen = screen;
                        SMPEG_pause(mpeg);
                        screen = SDL_SetVideoMode(event.resize.w, event.resize.h, screen->format->BitsPerPixel, screen->flags);
			if ( old_screen != screen ) {
                            SMPEG_setdisplay(mpeg, screen, NULL, update);
                        }
                        SMPEG_scaleXY(mpeg, screen->w, screen->h);
                        SMPEG_pause(mpeg);
                    } break;
*/


void UI_Animation::reloadOriginalSize()
{
//	setOriginalSize(getBitmapSize());
	UI_Object::reloadOriginalSize();
//	adjustSize(CHILD_WAS_CHANGED); // TODO
}

/*
void UI_Animation::setBrightness(const unsigned int bitmap_brightness) 
{
	if(brightness != bitmap_brightness)
	{
		brightness = bitmap_brightness;
		makePufferInvalid();
//		setNeedRedrawAllThatOverlaps(getRelativeRect());
//		toErrorLog(brightness);
	}
}*/
// hoechstens bei update aufrufen...

void UI_Animation::object_info()
{
	toErrorLog("ui_animation");
}
	

// Render button.  How it draws exactly depends on it's current state.
void UI_Animation::draw() const
{
//	if(clipRect != Rect())
//		dc->DrawBrightenedBitmap(theme.lookUpBitmap(checked?(eBitmap)(bitmap+1):bitmap), Point(), clipRect, brightness);
//	else
//		dc->DrawBrightenedBitmap(theme.lookUpBitmap(checked?(eBitmap)(bitmap+1):bitmap), Point(), brightness);
	UI_Object::draw();
		
	// leeres / letztes Bild evtl

/*	if(border)
	{
		dc->setBrush(*UI_Object::theme.lookUpBrush(TRANSPARENT_BRUSH));
		dc->setPen(*UI_Object::theme.lookUpPen(INNER_BORDER_PEN));
		dc->DrawEdgedRoundedRectangle(getRelativeRect(), 4);
		dc->setPen(*UI_Object::theme.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN));
		dc->DrawEdgedRoundedRectangle(Rect(getRelativePosition() + Size(3,3), getSize() - Size(6,6)), 4);
	}*/
}

UI_Object* UI_Animation::checkToolTip() {
	if( (!isShown()) || (!getAbsoluteRect().isTopLeftCornerInside(mouse)))
		return(0);
	return((UI_Object*)this);
}

UI_Object* UI_Animation::checkHighlight()
{
	if(!isShown())
		return(NULL);
	return(UI_Object::checkHighlight());
}

void UI_Animation::process()
{
	UI_Object::process();
	if(!isShown())
		return;
	if(isPlaying())
	{
		animationText->Show();
		endBitmap->Hide();
		SDL_Rect dest;
		dest.x = getAbsolutePosition().x;
		dest.y = getAbsolutePosition().y;
		dest.w = getWidth();
		dest.h = getHeight();
		SDL_BlitSurface(animationSurface, NULL, UI_Object::dc->getSurface(), &dest);
		SDL_UpdateRect(UI_Object::dc->getSurface(), dest.x, dest.y, dest.w, dest.h);
//		UI_Object::addNewRectToList(getAbsoluteRect(), getZ());
	}
	else	
	if(wasStarted && !isPlaying())
		endBitmap->Show();
}

