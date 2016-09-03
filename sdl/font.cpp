#include "font.hpp"

Font& Font::operator=(const Font& object)
{
	font = object.font;
	for(int i=0;i<128;i++)
	{
		text[i]=object.text[i];
		gm[i]=object.gm[i];
	}
	return(*this);
}

Font::Font(const Font& object) :
	font(object.font)
{
	for(int i=0;i<128;i++)
	{
		text[i]=object.text[i];
		gm[i]=object.gm[i];
	}
}

Font::Font(const std::string& fname, const unsigned int size):
	font(TTF_OpenFont(fname.c_str(), size))
{
    SDL_Color fg={0,0,0,100};
    for(int i=0; i<128; i++)
    {
        text[i]=TTF_RenderGlyph_Blended(font, i, fg);
		// TODO FEHLERBEHANDLUNG
//        if(!text[i])
  //          cout << "TTF_RenderGlyph_Shaded:" << TTF_GetError() << endl;
        TTF_GlyphMetrics(font, i,
                    &gm[i].minx, &gm[i].maxx,
                    &gm[i].miny, &gm[i].maxy,
                    &gm[i].advance);
    }
//  cout << fname << " loaded." << endl;
}

Font::Font():
	font(0)
{
}

const Size Font::GetTextExtent(const std::string& font_text) const
{
	Size s;
    for(int i=font_text.length();i--;)
    {
        s.SetWidth(s.GetWidth() + gm[(int)font_text[i]].advance);
        if(gm[(int)font_text[i]].maxy+gm[(int)font_text[i]].miny>(signed int)(s.GetHeight()))
            s.SetHeight(gm[(int)font_text[i]].maxy+gm[(int)font_text[i]].miny);
    }
    s.SetHeight(s.GetHeight() + 8);
	return(s);
}

Font::~Font()
{
    if(font) TTF_CloseFont(font);
    font=0;
    for(int i=0; i<128; i++)
    {
        if(text[i])
            SDL_FreeSurface(text[i]);
        text[i]=0;
    }
}

void Font::DrawText(SDL_Surface* surface, const SDL_Color& color, const std::string& font_text, const signed int x, const signed int y) const
{
/*  SDL_Rect r;
    r.x=x;
    r.y=y;
                                                                                
//  r2.x=r.x+gm[i].minx;
//  r2.y=r.y+TTF_FontAscent(font)-gm[i].maxy;
    for(int i=0;i<text.length();i++)
    {
//      r.y=y+TTF_FontAscent(font)-gm[i].maxy-1;
        r.y=y-gm[text[i]].maxy;
  //      r.y=y;
                                                                                
//      TTF_RenderText_Blended(font, test.c_str(),
//TTF_Font *font, const char *text, SDL_Color fg)
                                                                                
        SDL_BlitSurface(this->text[text[i]],0,surface,&r);
        r.x+=gm[text[i]].advance;
    };*/
    SDL_Surface *sText = TTF_RenderText_Blended( font, font_text.c_str(), color );
    SDL_Rect rcDest = {x,y-5,0,0};
    SDL_BlitSurface( sText,NULL, surface,&rcDest );
    SDL_FreeSurface( sText );
}

Font::operator TTF_Font*() 
{
	return(font);
}

