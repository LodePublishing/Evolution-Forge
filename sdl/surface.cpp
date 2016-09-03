#include "surface.hpp"

Surface::Surface(SDL_Surface* sdl_surf):
	surface(sdl_surf)
{
}

Surface::Surface():
	surface(NULL)
{
}

Surface::~Surface() 
{
	if(surface) 
		SDL_FreeSurface(surface);
}

Surface::Surface(const Surface& surf):
	surface(surf) // TODO
{
}

Surface& Surface::operator=(const Surface& sdl_surface) 
{ 
	this->surface=sdl_surface;
	return *this;
}

Surface::operator SDL_Surface*() const 
{ 
	return surface; 
}

SDL_Surface* Surface::getSurface() 
{
	return surface;
} //TODO SEHR UNSAUBER

SDL_Surface* Surface::operator->() const 
{ 
	return surface; 
}

SDL_PixelFormat* Surface::getFormat() const 
{
	return surface->format;
}


