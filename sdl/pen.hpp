#ifndef _SDL_PEN_HPP
#define _SDL_PEN_HPP

#include <SDL.h>
#include "color.hpp"

enum ePenStyle
{
	TRANSPARENT_PEN_STYLE,
	SOLID_PEN_STYLE,
	DOT_PEN_STYLE,
	LONG_DASH_PEN_STYLE,
	SHORT_DASH_PEN_STYLE,
	DOT_DASH_PEN_STYLE,
	MAX_PEN_STYLE
};

class Pen
{
	public:
		Pen(const Color pen_color, const unsigned int pen_width, const ePenStyle pen_style);
		Pen(SDL_Surface* surface, const Uint8 r, const Uint8 g, const Uint8 b, const unsigned int pen_width, const ePenStyle pen_style);
		Pen();
		~Pen() {};
		const ePenStyle GetStyle() const {return style;};
		const int GetWidth() const {return width;};
		const Color* GetColor() const {return &color;};
	private:
		unsigned int width;
		Color color;
		ePenStyle style;
};

#endif // _SDL_PEN_HPP
