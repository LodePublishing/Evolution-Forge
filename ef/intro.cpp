#include "intro.hpp"

Intro::Intro(UI_Object* intro_parent, 
				const Rect& intro_rect,
				const Size distance_bottom_right, 
				const ePositionMode intro_position_mode) :
	UI_Object(intro_parent, intro_rect,  distance_bottom_right, intro_position_mode, NO_AUTO_SIZE),
	clickText(new UI_StaticText(this, INTRO_CLICK_STRING, Rect(), Size(0, 20), BRIGHT_TEXT_COLOR, MIDDLE_SHADOW_BOLD_FONT, BOTTOM_RIGHT)),
	libraryGroup(new UI_Group(this, Rect(), Size(), TWO_LINES_GROUP, false, BOTTOM_CENTER, NULL_STRING)),
	introPhase(MAIN_INTRO),
	wasClicked(false),
	soundPlayed(false)
{
	reloadOriginalSize();
	clickText->Hide();
	Hide();
}

Intro::~Intro()
{
	delete clickText;
}

void Intro::setWasClicked()
{
	wasClicked = true;
}

const bool Intro::addKey(unsigned int key, unsigned int mod)
{
	setWasClicked();
	return(true);
}

const bool Intro::isDone() const 
{
	return(wasClicked);
}

void Intro::reloadOriginalSize()
{
	UI_Object::reloadOriginalSize();
}

void Intro::draw() const
{
	UI_Object::draw();
}


void Intro::process()
{
	if((!isShown())||isDone())
		return;
	UI_Object::focus = this;
	if(introPhase == MAX_INTRO_STEPS)
		return;


	if(!introAnimation[introPhase]->isDone())
	{
		if((introPhase == MAIN_INTRO)&&(!soundPlayed)&&(UI_Object::theme.lookUpAnimationInfo(INTRO_MAIN_ANIMATION).current_frame>80))
		{
			UI_Object::sound.playSound(INTRO_SOUND, introAnimation[introPhase]->getAbsolutePosition().x + introAnimation[introPhase]->getWidth()/2);
			soundPlayed = true;
		}
		if(!introAnimation[introPhase]->isShown())
		{
			introAnimation[introPhase]->Show();
			introAnimation[introPhase]->play();
			switch(introPhase)
			{
				case MAIN_INTRO:
					break;
				case SDL_INTRO:
				case SDL_TTF_INTRO:
				case SDL_IMAGE_INTRO:
				case GUI_INTRO:
				case SMPEG_INTRO:
#ifdef _FMOD_SOUND
				case FMOD_INTRO:
#elif _SDL_MIXER_SOUND
				case SDL_MIXER_INTRO:
#endif
					UI_Object::sound.playSound(RING_SOUND, introAnimation[introPhase]->getAbsolutePosition().x + introAnimation[introPhase]->getWidth()/2);
					break;
				default:toErrorLog(":(");break;
			}
		}
	} else
	{
		introPhase = (eIntroPhase)(introPhase+1);
		if(introPhase == MAX_INTRO_STEPS)
			clickText->Show();
	}
}

