#include "pen.hpp"

Pen::Pen():
	width(0),
	color(),
	style()
{ }

Pen::Pen(const Color pen_color, const unsigned int pen_width, const ePenStyle pen_style):
	width(pen_width),
	color(pen_color),
	style(pen_style)
{ }


Pen::Pen(SDL_Surface* surface, const Uint8 r, const Uint8 g, const Uint8 b, const unsigned int pen_width, const ePenStyle pen_style):
	width(pen_width),
	color(Color(surface, r, g, b)),
	style(pen_style)
{ }  // TODO: warning: will never be executed ??

