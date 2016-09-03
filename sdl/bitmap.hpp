#ifndef _SDL_BITMAP_HPP
#define _SDL_BITMAP_HPP

#include <SDL/SDL.h>

#include "surface.hpp"
#include <string>

class Bitmap:public Surface
{
    public:
        Bitmap(const std::string& name);
        Bitmap(const Bitmap& bmp);
        Bitmap();
        ~Bitmap();
};

#endif // _SDL_BITMAP_HPP
