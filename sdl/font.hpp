#ifndef _SDL_FONT_HPP
#define _SDL_FONT_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "size.hpp"

#include <string>

typedef struct {
    int minx,
        maxx,
        miny,
        maxy,
        advance;
} GlyphMetrics;

class Font
{
    private:
        TTF_Font* font;
        SDL_Surface* text[128];
        GlyphMetrics gm[128];
    public:
		Font& operator=(const Font& object);
		Font(const Font& object);
        void DrawText(SDL_Surface* surface, const SDL_Color& color, const std::string& text, const signed int x, const signed int y) const;
        const Size GetTextExtent(const std::string& text) const;
        Font(const std::string& fname, const unsigned int size);
        Font();
        ~Font();
        operator TTF_Font* ();
};


#endif // _SDL_FONT_HPP
