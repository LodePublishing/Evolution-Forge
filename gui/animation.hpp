// g++ -o plaympeg plaympeg.c -I/usr/include/SDL -lSDL -lsmpeg

#ifndef _UI_ANIMATION_HPP
#define _UI_ANIMATION_HPP

#include "bitmap.hpp"
#include "statictext.hpp"

class UI_Animation : public UI_Object
{
        public:
                UI_Animation(UI_Object* animation_parent,
                                const Rect& animation_rect,
                                const Size distance_bottom_right,
				const eAnimation my_animation,
				const eBitmap end_bitmap,
				const eString animation_text,
				const bool animation_border = false,
                                const ePositionMode animation_position_mode = DO_NOT_ADJUST);
                ~UI_Animation();
                void reloadOriginalSize();
		UI_Object* checkToolTip();
		UI_Object* checkHighlight();
                void process();
                void object_info();
                void draw() const;
		const bool setAnimationFile(const eAnimation id);
		const bool isPlaying() const;
		const bool isDone() const;
		void play();
		void pause();
        private:
		eAnimation animation;
		SDL_Surface* animationSurface;			
		UI_Bitmap* endBitmap;
		UI_StaticText* animationText;
		bool loop;
		bool border;
		bool wasStarted;

		const bool isLocked(const eAnimation id) const;
		void lockAnimation(const bool lock);
};

#endif // _UI_ANIMATION_HPP

