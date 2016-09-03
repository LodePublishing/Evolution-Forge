#ifndef _SDL_BRUSH_HPP
#define _SDL_BRUSH_HPP

#include "color.hpp"

enum eBrushStyle
{
    TRANSPARENT_BRUSH_STYLE,
    SOLID_BRUSH_STYLE,
    BDIAGONAL_HATCH_BRUSH_STYLE,
    CROSSDIAG_HATCH_BRUSH_STYLE,
    FDIAGONAL_HATCH_BRUSH_STYLE,
    CROSS_HATCH_BRUSH_STYLE,
    HORIZONTAL_HATCH_BRUSH_STYLE,
    VERTICAL_HATCH_BRUSH_STYLE,
    STIPPLE_BRUSH_STYLE,
    STIPPLE_MASK_OPAQUE_BRUSH_STYLE,
    MAX_BRUSH_STYLES
};

class Brush
{
    public:
        Brush(const Color brush_color, const eBrushStyle brush_style);
        Brush(SDL_Surface* surface, const Uint8 r, Uint8 g, Uint8 b, const eBrushStyle brush_style);
        Brush();
        ~Brush();
        const eBrushStyle GetStyle() const;
        const Color* GetColor() const;
    private:
        Color color;
        eBrushStyle style;
};


#endif // _SDL_BRUSH_HPP