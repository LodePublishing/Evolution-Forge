#ifndef _SDL_COLOR_HPP
#define _SDL_COLOR_HPP

#include <SDL/SDL.h>

class Color
{
    public:
        Color(const Uint32 ucolor, const SDL_Color scolor);
        Color(SDL_Surface* surface, const Uint8 red, const Uint8 green, const Uint8 blue);
        Color();
        Color(const Color& col);

        operator SDL_Color() const;
        operator Uint32() const;
        const Uint8 r() const;
        const Uint8 g() const;
        const Uint8 b() const;
        ~Color();
        Uint32 ucol;
        SDL_Color scol;
};

#endif // _SDL_COLOR_HPP
