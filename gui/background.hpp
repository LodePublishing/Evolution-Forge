#ifndef _UI_BACKGROUND_HPP
#define _UI_BACKGROUND_HPP

#include "object.hpp"

class UI_BackGround : public UI_Object
{
	public:
		UI_BackGround(SDL_Surface* background_bitmap, Size size);
		~UI_BackGround();
		void process();
		void object_info();
		void draw() const;
	private:
		SDL_Surface* backgroundBitmap;
};

#endif

