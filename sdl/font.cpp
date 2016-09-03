#include "font.hpp"

Font& Font::operator=(const Font& object)
{
	font = object.font;
	return(*this);
}

Font::Font(const Font& object) :
	font(object.font)
{ }

Font::Font(const std::string& fname, const unsigned int size):
	font(TTF_OpenFont(fname.c_str(), size))
{ }

Font::Font():
	font(0)
{ }

const Size Font::GetTextExtent(const std::string& font_text) const
{
	int width, height;
	TTF_SizeUTF8(font, font_text.c_str(), &width, &height);
	return(Size(width, height-2));
}

Font::~Font()
{
	if(font) 
	{	
		TTF_CloseFont(font);
		font=NULL;
	}
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


