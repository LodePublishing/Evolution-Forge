#include "brush.hpp"

Brush::Brush(const Color brush_color, const eBrushStyle brush_style):
	color(brush_color),
	style(brush_style)
{}
                                                                                
Brush::Brush(SDL_Surface* surface, const Uint8 r, const Uint8 g, const Uint8 b, const eBrushStyle brush_style):
	color(Color(surface, r, g, b)),
	style(brush_style)
{}

Brush::Brush():
	color(),
	style()
{}

Brush::~Brush()
{}

void Brush::updateColor(SDL_Surface* surface)
{
	color.updateColor(surface);
}

