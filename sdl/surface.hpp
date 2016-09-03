#ifndef _SDL_SURFACE_HPP
#define _SDL_SURFACE_HPP

#include <SDL.h>

class Surface
{
    public:
        Surface(SDL_Surface* sdl_surf);
        Surface();
		Surface(const Surface& surf);
        virtual ~Surface();
        Surface& operator=(const Surface& surf);
        operator SDL_Surface*() const;
        SDL_Surface* getSurface();
        SDL_Surface* operator->() const;
        SDL_PixelFormat* getFormat() const;
    private:
        SDL_Surface* surface;
};

#endif // SDL_SURFACE_HPP

