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
		unsigned int size;
		bool underlined;
		bool shadow;
	public:
		Font(const std::string& fname, const unsigned int font_size, const bool is_under_lined = false, const bool is_shadow = false);
		Font& operator=(const Font& object);
		Font(const Font& object);
		Font();
		~Font();
		operator TTF_Font*();
		TTF_Font* getTTF_Font() const;

		const bool isUnderlined() const;
		const bool isShadow() const;
		void setUnderlined(const bool is_underlined);
		void setShadow(const bool is_shadow);

		const unsigned int getSize() const;
		
		const Size getTextExtent(const std::string& text) const;
		void DrawText(SDL_Surface* surface, const SDL_Color& color, const std::string& text, const signed int x, const signed int y) const;
};

inline Font::operator TTF_Font*() {
	return(font);
}

inline TTF_Font* Font::getTTF_Font() const {
	return(font);
}

inline const bool Font::isUnderlined() const {
	return(underlined);
}

inline const bool Font::isShadow() const {
	return(shadow);
}

inline void Font::setUnderlined(const bool under_lined) {
	underlined = under_lined;
}

inline const unsigned int Font::getSize() const {
	return(size);
}

#endif // _SDL_FONT_HPP
