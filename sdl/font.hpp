#ifndef _SDL_FONT_HPP
#define _SDL_FONT_HPP

#include <SDL.h>
#include <SDL_ttf.h>
#include "size.hpp"

#include <string>

class Font
{
	private:
		TTF_Font* font;
	public:
		Font& operator=(const Font& object);
		Font(const Font& object);
		void DrawText(SDL_Surface* surface, const SDL_Color& color, const std::string& text, const signed int x, const signed int y) const;
		const Size GetTextExtent(const std::string& text) const;
		Font(const std::string& fname, const unsigned int size);
		Font();
		~Font();
		operator TTF_Font*();
		TTF_Font* getTTF_Font() const;
};

inline Font::operator TTF_Font*() 
{
	return(font);
}


inline TTF_Font* Font::getTTF_Font() const {
	return(font);
}

#endif // _SDL_FONT_HPP
