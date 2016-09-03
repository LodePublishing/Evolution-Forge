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
                                                                                
Color::operator SDL_Color() const {return(scol);}
Color::operator Uint32() const {return(ucol);}
const Uint8 Color::r() const {return(scol.r);}
const Uint8 Color::g() const {return(scol.g);}
const Uint8 Color::b() const {return(scol.b);}
Color::~Color() {}

