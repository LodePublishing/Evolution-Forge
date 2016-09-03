#include "color.hpp"

Color::Color(const Uint32 ucolor, const SDL_Color scolor):
	ucol(ucolor),
	scol(scolor)
{ }

Color::Color(const Color& col):
	ucol(col.ucol),
	scol(col.scol)
{ }

Color::Color(SDL_Surface* surface, const Uint8 red, const Uint8 green, const Uint8 blue):
	ucol(SDL_MapRGB(surface->format, red, green, blue))
//	scol( ((Uint32)red << 24) | ((Uint32)green << 16) | ((Uint32)blue << 8) | 255) 
{
	scol.r=red;
	scol.g=green;
	scol.b=blue;
	scol.unused=255;
//  ucol = (r << 24 ) + (g <<  16) + (b << 8) +200;
}

Color::Color():
	ucol(0),
	scol()
{ }

Color::~Color() { }

