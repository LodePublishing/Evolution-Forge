#ifndef _SDL_COLOR_HPP
#define _SDL_COLOR_HPP

#include <SDL.h>

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

		void updateColor(SDL_Surface* surface);
		
		~Color();
		Uint32 ucol;
		SDL_Color scol;
};

inline Color::operator SDL_Color() const {return(scol);}
inline Color::operator Uint32() const {return(ucol);}
inline const Uint8 Color::r() const {return(scol.r);}
inline const Uint8 Color::g() const {return(scol.g);}
inline const Uint8 Color::b() const {return(scol.b);}
#endif // _SDL_COLOR_HPP
