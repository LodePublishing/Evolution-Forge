#ifndef _GUI_INTRO_HPP
#define _GUI_INTRO_HPP

#include "../ui/animation.hpp"
#include "../ui/statictext.hpp"
#include "../ui/group.hpp"

enum eIntroPhase
{
	MAIN_INTRO,
	SDL_INTRO,
	SDL_TTF_INTRO,
	SDL_IMAGE_INTRO,
	GUI_INTRO,
	SMPEG_INTRO,	
#ifdef _FMOD_SOUND
	FMOD_INTRO,
#elif _SDL_MIXER_SOUND
	SDL_MIXER_INTRO,
#endif
	MAX_INTRO_STEPS
};

class Intro : public UI_Object
{
	public:
		Intro(UI_Object* intro_parent, const Rect& intro_rect, const Size distance_bottom_right, const ePositionMode intro_position_mode);
		~Intro();
		void process();
		void draw() const;
		void reloadOriginalSize();
		const unsigned int getGameType() const;
		const bool isDone() const;
		void setWasClicked();
		const bool addKey(unsigned int key, unsigned int mod);
	private:
		UI_Animation* introAnimation[MAX_INTRO_STEPS];
		UI_StaticText* clickText;
		UI_Group* libraryGroup;
		eIntroPhase introPhase;
		bool wasClicked;
		bool soundPlayed;
};

#endif

