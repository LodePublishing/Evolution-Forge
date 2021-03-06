#include "font.hpp"


#ifdef _SCC_DEBUG
        #include "../stl/misc.hpp"
#endif


Font::Font(const std::string& fname, const unsigned int font_size, const bool is_under_lined, const bool is_shadow):
	font(TTF_OpenFont(fname.c_str(), font_size)),
	size(font_size),
	underlined(is_under_lined),
	shadow(is_shadow)
{ 
#ifdef _SCC_DEBUG
	if(font == NULL)
		toErrorLog("ERROR (Font::Font()): Could not initialize font " + fname + " [TTF ERROR: \"" + TTF_GetError() + "\"].");
#endif
}

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

const Size Font::getTextExtent(const std::string& font_text) const
{
	int width;// = 0;
	int height;// = 0;
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

void Font::DrawText(SDL_Surface* surface, const SDL_Color& color, const SDL_Color& back_color, const std::string& font_text, const signed int x, const signed int y) const // TODO
{
	SDL_Surface *sText = TTF_RenderUTF8_Shaded( font, font_text.c_str(), color, back_color);
	if(sText==NULL)
		return;
	SDL_Rect rcDest = {x,y-4,0,0};
	SDL_BlitSurface( sText, NULL, surface, &rcDest );
	SDL_FreeSurface( sText );
}

