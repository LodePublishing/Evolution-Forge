#include "font.hpp"

Font::Font(const std::string& fname, const unsigned int font_size, const bool is_under_lined, const bool is_shadow):
	font(TTF_OpenFont(fname.c_str(), font_size)),
	size(font_size),
	underlined(is_under_lined),
	shadow(is_shadow)
{ }

Font::Font():
	font(NULL),
	size(0),
	underlined(false),
	shadow(false)
{ }

Font& Font::operator=(const Font& object)
{
	font = object.font;
	size = object.size;
	underlined = object.underlined;
	shadow = object.shadow;
	return(*this);
}

Font::Font(const Font& object) :
	font(object.font),
	size(object.size),
	underlined(object.underlined),
	shadow(object.shadow)
{ }



Font::~Font()
{
	if(font) 
	{	
		TTF_CloseFont(font);
		font=NULL;
	}
}

const Size Font::GetTextExtent(const std::string& font_text) const
{
	int width, height;
	TTF_SizeUTF8(font, font_text.c_str(), &width, &height);
	return(Size(width, height-2));
}


void Font::DrawText(SDL_Surface* surface, const SDL_Color& color, const std::string& font_text, const signed int x, const signed int y) const
{
	SDL_Surface *sText = TTF_RenderUTF8_Blended( font, font_text.c_str(), color);
	if(sText==NULL)
		return;
	SDL_Rect rcDest = {x,y-4,0,0};
	SDL_BlitSurface( sText, NULL, surface, &rcDest );
	SDL_FreeSurface( sText );
}


