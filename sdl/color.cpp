#include "color.hpp"

Color::Color(const Uint32 ucolor, const SDL_Color scolor):
	ucol(ucolor),
	scol(scolor)
{ }

Color::Color(const Color& col):
	ucol(col.ucol),
	scol(col.scol)
{ }

Color::Color(const SDL_Surface* const surface, const Uint8 red, const Uint8 green, const Uint8 blue):
	ucol(SDL_MapRGB(surface->format, red, green, blue)),
	scol()
{
	scol.r=red;
	scol.g=green;
	scol.b=blue;
	scol.unused=255;
}

Color::Color():
	ucol(0),
	scol()
{ }

Color::~Color() { }

void Color::updateColor(const SDL_Surface* const surface)
{
	ucol = SDL_MapRGB(surface->format, scol.r, scol.g, scol.b);
}

