#include "sdlwrapper.hpp"

SDL_Color toSDL_Color(const Uint8 r, const Uint8 g, const Uint8 b)
{
	SDL_Color c;
	c.r=r;
	c.g=g;
	c.b=b;
	c.unused=255;
	return(c);	
}

/*inline Rect::Rect(const Point& topLeft, const Point& bottomRight)
{
  x = topLeft.x;
  y = topLeft.y;
  width = bottomRight.x - topLeft.x + 1;
  height = bottomRight.y - topLeft.y + 1;

  if (width < 0)
  {
	width = -width;
	x -= width;
  }

  if (height < 0)
  {
	height = -height;
	y -= height;
  }
}*/

Rect& Rect::operator += (const Rect& rect)
{
	*this = (*this + rect);
	return ( *this ) ;
}

const bool Rect::Inside(const int cx, const int cy) const
{
	return ( (cx >= x) && (cy >= y)
		  && ((cy - y) < height)
		  && ((cx - x) < width)
		  );
}

Bitmap::Bitmap(const string& name):Surface(IMG_Load(name.c_str())) {}
Bitmap::Bitmap(const Bitmap& bmp):Surface(bmp) {}
Bitmap::Bitmap():Surface() {}
Bitmap::~Bitmap() {}

Color::Color(const Uint32 ucol, const SDL_Color scol) 
{
	this->ucol=ucol; 
	this->scol.r=scol.r;
	this->scol.g=scol.g;
	this->scol.b=scol.b;
	this->scol.unused=scol.unused;
}

Color::Color(const Color& col)
{
	ucol=col.ucol;
	scol.r=col.scol.r;
	scol.g=col.scol.b;
	scol.b=col.scol.g;
	scol.unused=col.scol.unused;
}

Color::Color(SDL_Surface* surface, const Uint8 r, const Uint8 g, const Uint8 b)
{
	scol.r=r;
	scol.g=g;
	scol.b=b;
	scol.unused=255;
//	ucol=SDL_MapRGB(surface->format, r, g, b);
	ucol = (r << 24 ) + (g << 16) + (b << 8) + 255;
}

Color::Color() {}

Color::operator SDL_Color() const {return(scol);}
//Color::operator SDL_Color*() const {return(&scol);};
Color::operator Uint32() const {return(ucol);}
//Color::operator Uint32*() const {return(&ucol);};
const Uint8 Color::r() const {return(scol.r);}
const Uint8 Color::g() const {return(scol.g);}
const Uint8 Color::b() const {return(scol.b);}
Color::~Color() {}


Font::Font(const string& fname, const int size)
{
	font=TTF_OpenFont(fname.c_str(), size);
	SDL_Color fg={0,0,0,100};
	for(int i=0; i<128; i++)
	{
	   	text[i]=TTF_RenderGlyph_Blended(font,i,fg);
		if(!text[i])
			cout << "TTF_RenderGlyph_Shaded:" << TTF_GetError() << endl;
		TTF_GlyphMetrics(font, i,
					&gm[i].minx, &gm[i].maxx,
					&gm[i].miny, &gm[i].maxy,
					&gm[i].advance);
   	};
//	cout << fname << " loaded." << endl;
}

void Font::GetTextExtent(const string& text, int* dx, int* dy) const
{
	*dx=0;*dy=0;
	for(int i=text.length();i--;)
	{
		*dx+=gm[(int)text[i]].advance;
		if(gm[(int)text[i]].maxy+gm[(int)text[i]].miny>*dy)
			*dy=gm[(int)text[i]].maxy+gm[(int)text[i]].miny;
	}
	*dy+=8;
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
	

void Font::DrawText(SDL_Surface* surface, const SDL_Color& color, const string& text, const int x, const int y) const
{
/*	SDL_Rect r;
	r.x=x;
	r.y=y;

//  r2.x=r.x+gm[i].minx;
//  r2.y=r.y+TTF_FontAscent(font)-gm[i].maxy;
	for(int i=0;i<text.length();i++)
	{
//		r.y=y+TTF_FontAscent(font)-gm[i].maxy-1;
		r.y=y-gm[text[i]].maxy;
  //	  r.y=y;
		
//		TTF_RenderText_Blended(font, test.c_str(),
//TTF_Font *font, const char *text, SDL_Color fg)

		SDL_BlitSurface(this->text[text[i]],0,surface,&r);
		r.x+=gm[text[i]].advance;
	};*/
	SDL_Surface *sText = TTF_RenderText_Blended( font, text.c_str(), color );
	SDL_Rect rcDest = {x,y-5,0,0};
	SDL_BlitSurface( sText,NULL, surface,&rcDest );
	SDL_FreeSurface( sText );
}

void DC::SetBrush(const Brush& brush) {this->brush=brush;}
void DC::SetPen(const Pen& pen) {this->pen=pen;}
void DC::SetColor(Color* color) {this->color=color;}
void DC::SetFont(Font* font) {this->font=font;}
void DC::SetTextForeground(const SDL_Colour& textColor) {this->textColor=textColor;}


void DC::DrawBitmap(const Bitmap& bitmap, const int x, const int y) const
{
	SDL_Rect drect;
	drect.x = x;
	drect.y = y;
	drect.w = bitmap->w;
	drect.h = bitmap->h;
	SDL_BlitSurface(bitmap , 0, surface, &drect );
	
}

const Color DC::mixColor(const Color* id1, const Color* id2)  const
{
//			return(*id1);
		   const Color c=Color(surface, id1->r()  +id2->r(),
					id1->g()+id2->g(),
					id1->b() +id2->b());
			return(c); // TODO
}

const Color DC::mixColor(const Color* id1, const Color* id2, const int gradient) const
{
	const Color c=Color(surface, (id1->r()*gradient  +id2->r()*(100-gradient))/100,
		(id1->g()*gradient+id2->g()*(100-gradient))/100,
		(id1->b()*gradient +id2->b()*(100-gradient))/100);
	return(c);
}

const Color DC::brightenColor(const Color* id, const int brightness) const
{
	const Color c=Color(surface, id->r()  +brightness,
			id->g()+brightness,
			id->b() +brightness);
	return(c);
}

void DC::DrawSpline(const int c, const Point* p) const
{
	if((pen.GetStyle() == TRANSPARENT_PEN_STYLE)||(c<2))
		return;
	for(int i=0;i<c-1;i++)
//	{
//		aalineColor(surface, p[i].x, p[i].y, p[i+1].x, p[i+1].y, (Uint32)(*pen.GetColor()));
//		aalineColor(surface, p[i].x, p[i].y+1, p[i+1].x, p[i+1].y+1, (Uint32)(*pen.GetColor()));
//	}
	{
		if((i>0)&&(i<c-2))
		{
			DrawBresLine(p[i].x, p[i].y + (p[i].y < p[i-1].y) + (p[i].y < p[i+1].y) - (p[i].y > p[i-1].y) - (p[i].y > p[i+1].y),
					   p[i+1].x, p[i+1].y + (p[i+1].y < p[i].y) + (p[i+1].y < p[i+2].y) - (p[i+1].y > p[i].y) - (p[i+1].y > p[i+2].y));
			DrawBresLine(p[i].x, p[i].y + (p[i].y < p[i-1].y) + (p[i].y < p[i+1].y) - (p[i].y > p[i-1].y) - (p[i].y > p[i+1].y) + 1,
					   p[i+1].x, p[i+1].y + (p[i+1].y < p[i].y) + (p[i+1].y < p[i+2].y) - (p[i+1].y > p[i].y) - (p[i+1].y > p[i+2].y) + 1);
		} else
		{
			DrawBresLine(p[i].x, p[i].y, p[i+1].x, p[i+1].y);
			DrawBresLine(p[i].x, p[i].y+1, p[i+1].x, p[i+1].y+1);
		}
	}
	
}

void DC::DrawHalfRoundedRectangle(const int x, const int y, const int w, const int h, const int radius) const
{
	if((x<0)||(y<0)||(x+w>=1280)||(y+h>=1024)) // TODO clipping
		return;
	// CMB: if radius is zero use DrawRectangle() instead to avoid
	// X drawing errors with small radii
	if((radius <= 1)||(w<6)||(h<6))	{
		DrawRectangle(x,y,w,h);
		return;
	}

	bool clippedRight=false;
	bool clippedBottom=false;
	// TODO!
	int ww,hh;
	if(x + w > 1280) {
		clippedRight=true;
		ww = 1280 - x;
	} else ww = w;
	if(y + h > 1024) {
		clippedBottom=true;
		hh = 1280 - y;
	} else hh = h;

	
/*	bool clippedLeft=false;
	bool clippedTop=false;*/

	int dd=radius*8;
	if (dd > ww) dd = ww;
	if (dd > hh) dd = hh;
	int rr = dd / 2;

	if (brush.GetStyle() != TRANSPARENT_BRUSH_STYLE)
	{
//		SDL_Rect rc;
//		rc.x=x+1;rc.y=y+rr+1;rc.w=ww-2;rc.h=h-dd-2;
		boxColor(surface, x+1, y+rr+1, x+ww-1, y+hh-rr-1, (Uint32)(*brush.GetColor()) );
	//	SDL_FillRect(surface, &rc, (Uint32)(*brush.GetColor()) );
// die 2 Seiten oben und unten:
//		rc.x=x+rr+1;rc.y=y+1;rc.w=ww-dd-2;rc.h=rr;
		boxColor(surface, x+rr+1, y+1, x+ww-rr-1, y+rr+1, (Uint32)(*brush.GetColor()) );
//		SDL_FillRect(surface, &rc, (Uint32)(*brush.GetColor()) );

//		rc.x=x+rr+1;rc.y=y+hh-rr-1;rc.w=ww-dd-2;rc.h=rr;
		boxColor(surface, x+rr+1, y+hh-rr-1, x+ww-rr-1, y+hh-1, (Uint32)(*brush.GetColor()) );
//		SDL_FillRect(surface, &rc, (Uint32)(*brush.GetColor()) );

		if(!clippedRight) {
	        filledpieColor(surface, x+ww-rr, y+rr, rr, 270, 0, (Uint32)(*brush.GetColor()));
			if(!clippedBottom)
	    	    filledpieColor(surface, x+ww-rr, y+hh-rr, rr, 0, 90, (Uint32)(*brush.GetColor()));
		}
	    filledpieColor(surface, x+rr, y+rr, rr, 180, 270, (Uint32)(*brush.GetColor()));
		if(!clippedBottom)
	        filledpieColor(surface, x+rr, y+hh-rr, rr, 90, 180, (Uint32)(*brush.GetColor()));
	}

	if (pen.GetStyle() != TRANSPARENT_PEN_STYLE)
	{
//		SDL_Rect rc;
// left to right (up)
//		rc.x=x+rr-(pen.GetWidth()>>1);rc.y=y-(pen.GetWidth()>>1);rc.w=ww-dd;rc.h=pen.GetWidth();
		hlineColor(surface, x+rr, x+ww-rr, y, (Uint32)(*pen.GetColor()));
//		SDL_FillRect(surface, &rc, (Uint32)(*pen.GetColor()));
// left to right (low)
//		rc.x=x+rr-(pen.GetWidth()>>1);rc.y=y+hh-1-(pen.GetWidth()>>1);rc.w=ww-dd;rc.h=pen.GetWidth();
		if(!clippedBottom)
			hlineColor(surface, x+rr, x+ww-rr, y+hh-1, (Uint32)(*pen.GetColor()));
//		SDL_FillRect(surface, &rc, (Uint32)(*pen.GetColor()));
// left to down
//		rc.x=x-(pen.GetWidth()>>1);rc.y=y+rr-(pen.GetWidth()>>1);rc.w=pen.GetWidth();rc.h=h-dd;
		vlineColor(surface, x, y+rr, y+hh-rr, (Uint32)(*pen.GetColor()));
//		SDL_FillRect(surface, &rc, (Uint32)(*pen.GetColor()));
		if(!clippedRight) {
// right to down
//			rc.x=x+ww-1-(pen.GetWidth()>>1);rc.y=y+rr-(pen.GetWidth()>>1);rc.w=pen.GetWidth();rc.h=h-dd;
	//		SDL_FillRect(surface, &rc, (Uint32)(*pen.GetColor()));
			vlineColor(surface, x+ww-1, y+rr, y+hh-rr, (Uint32)(*pen.GetColor()));
			pieColor(surface, x+ww-rr, y+rr, rr, 270, 0, (Uint32)(*pen.GetColor()));
			if(!clippedBottom)
				pieColor(surface, x+ww-rr, y+hh-rr, rr, 0, 90, (Uint32)(*pen.GetColor()));
		}
		pieColor(surface, x+rr, y+rr, rr, 180, 270, (Uint32)(*pen.GetColor()));
		if(!clippedBottom)
			pieColor(surface, x+rr, y+hh-rr, rr, 90, 180, (Uint32)(*pen.GetColor()));
	}
}




void DC::DrawRoundedRectangle(const int x, const int y, const int w, const int h, const int radius) const
{
	if((x<0)||(y<0)||(x+w>=1280)||(y+h>=1024)) // TODO clipping
		return;
	// CMB: if radius is zero use DrawRectangle() instead to avoid
	// X drawing errors with small radii
	if((radius <= 1)||(w<2*radius)||(h<2*radius))
	{
		DrawRectangle(x,y,w,h);
		return;
	};

	bool clippedRight=false;
	bool clippedBottom=false;
	// TODO!
	int ww, hh;
	if(x + w > 1280)
	{
		clippedRight=true;
		ww = 1280 - x;
	} else ww = w;
    if(y + h > 1024) {
	    clippedBottom=true;
		hh = 1280 - y;
    } else hh = h;

		
	
/*	bool clippedLeft=false;
	bool clippedTop=false;*/

	int dd=radius*2;
	if (dd > ww) dd = ww;
	if (dd > hh) dd = hh;
	int rr = dd / 2;

	if (brush.GetStyle() != TRANSPARENT_BRUSH_STYLE)
	{
//		SDL_Rect rc;
//		rc.x=x+1;rc.y=y+rr+1;rc.w=ww-2;rc.h=h-dd-2;
//		boxRGBA(surface, x+1, y+rr+1, x+ww-2, y+h-dd-2, 200,0,0,55 );
		boxColor(surface, x, y+rr, x+ww-1, y+hh-rr-1, (Uint32)(*brush.GetColor()) );
//		SDL_FillRect(surface, &rc, (Uint32)(*brush.GetColor()) );
// die 2 Seiten oben und unten:
//		rc.x=x+rr+1;rc.y=y+1;rc.w=ww-dd-2;rc.h=rr;
		boxColor(surface, x+rr, y, x+ww-rr-1, y+rr-1, (Uint32)(*brush.GetColor()) );
//		SDL_FillRect(surface, &rc, (Uint32)(*brush.GetColor()) );

//		rc.x=x+rr+1;rc.y=y+h-rr-1;rc.w=ww-dd-2;rc.h=rr;
		if(!clippedBottom)
			boxColor(surface, x+rr, y+hh-rr, x+ww-rr-1, y+hh-1, (Uint32)(*brush.GetColor()) );
//		SDL_FillRect(surface, &rc, (Uint32)(*brush.GetColor()) );

        filledpieColor(surface, x+rr, y+rr, rr, 180, 270, (Uint32)(*brush.GetColor()));
		if(!clippedBottom)
	        filledpieColor(surface, x+rr, y+hh-rr, rr, 90, 180, (Uint32)(*brush.GetColor()));
		if(!clippedRight)
		{
	        filledpieColor(surface, x+ww-rr, y+rr, rr, 270, 0, (Uint32)(*brush.GetColor()));
			if(!clippedBottom)
	    	    filledpieColor(surface, x+ww-rr, y+hh-rr, rr, 0, 90, (Uint32)(*brush.GetColor()));
		}
	}

	if (pen.GetStyle() != TRANSPARENT_PEN_STYLE)
	{
//		SDL_Rect rc;
// left to right (up)
//		rc.x=x+rr-(pen.GetWidth()>>1);rc.y=y-(pen.GetWidth()>>1);rc.w=ww-dd;rc.h=pen.GetWidth();
        hlineColor(surface, x+rr, x+ww-rr, y, (Uint32)(*pen.GetColor()));
//		SDL_FillRect(surface, &rc, (Uint32)(*pen.GetColor()));
// left to right (low)
//		rc.x=x+rr-(pen.GetWidth()>>1);rc.y=y+h-1-(pen.GetWidth()>>1);rc.w=ww-dd;rc.h=pen.GetWidth();
		if(!clippedBottom)
	        hlineColor(surface, x+rr, x+ww-rr, y+hh-1, (Uint32)(*pen.GetColor()));
//		SDL_FillRect(surface, &rc, (Uint32)(*pen.GetColor()));
// left to down
//		rc.x=x-(pen.GetWidth()>>1);rc.y=y+rr-(pen.GetWidth()>>1);rc.w=pen.GetWidth();rc.h=h-dd;
        vlineColor(surface, x, y+rr, y+hh-rr, (Uint32)(*pen.GetColor()));
//		SDL_FillRect(surface, &rc, (Uint32)(*pen.GetColor()));
		if(!clippedRight) {
// right to down
//			rc.x=x+ww-1-(pen.GetWidth()>>1);rc.y=y+rr-(pen.GetWidth()>>1);rc.w=pen.GetWidth();rc.h=h-dd;
//			SDL_FillRect(surface, &rc, (Uint32)(*pen.GetColor()));
            vlineColor(surface, x+ww-1, y+rr, y+hh-rr, (Uint32)(*pen.GetColor()));
			pieColor(surface, x+ww-rr, y+rr, rr, 270, 0, (Uint32)(*pen.GetColor()));
			if(!clippedBottom)
				pieColor(surface, x+ww-rr, y+hh-rr, rr, 0, 90, (Uint32)(*pen.GetColor()));
		}
		pieColor(surface, x+rr, y+rr, rr, 180, 270, (Uint32)(*pen.GetColor()));
		if(!clippedBottom)
			pieColor(surface, x+rr, y+hh-rr, rr, 90, 180, (Uint32)(*pen.GetColor()));
	}
}


void DC::DrawRectangle(const int x, const int y, const int w, const int h) const
{
	if((w<2)||(h<2)) return;
	if((x<0)||(y<0)||(x+w>=1280)||(y+h>=1024)) // TODO clipping
		return;
//	SDL_Rect rc;
//	rc.x=x+1;rc.y=y+1;rc.w=w-2;rc.h=h-2;
	if(brush.GetStyle()!=TRANSPARENT_BRUSH_STYLE)
	{
        boxColor(surface, x+1, y+1, x+w-1, y+h-1, (Uint32)(*brush.GetColor()) );
//		SDL_FillRect(surface, &rc, (Uint32)(*brush.GetColor()) );
	};
	DrawEmptyRectangle(x,y,w,h);	
}

void DC::DrawEmptyRectangle(const int x, const int y, const int w, const int h) const
{
	if(pen.GetStyle()==TRANSPARENT_PEN_STYLE) return;
//	SDL_Rect rc;
	if((x<0)||(y<0)||(x+w>=1280)||(y+h>=1024)) // TODO clipping
		return;

	rectangleColor(surface, x, y, x + w, y + h, (Uint32)(*pen.GetColor()));
/*	rc.x=x-(pen.GetWidth()>>1);rc.y=y-(pen.GetWidth()>>1);rc.w=w;rc.h=pen.GetWidth();
	SDL_FillRect(surface, &rc, (Uint32)(*pen.GetColor()));
	
	rc.x=x-(pen.GetWidth()>>1);rc.y=y+h-1-(pen.GetWidth()>>1);rc.w=w;rc.h=pen.GetWidth();
	SDL_FillRect(surface, &rc, (Uint32)(*pen.GetColor()));
	
	rc.x=x-(pen.GetWidth()>>1);rc.y=y-(pen.GetWidth()>>1);rc.w=pen.GetWidth();rc.h=h;
	SDL_FillRect(surface, &rc, (Uint32)(*pen.GetColor()));
	
	rc.x=x+w-1-(pen.GetWidth()>>1);rc.y=y-(pen.GetWidth()>>1);rc.w=pen.GetWidth();rc.h=h;
	SDL_FillRect(surface, &rc, (Uint32)(*pen.GetColor()));*/
}

void DC::DrawLine(const int x1, const int y1, const int x2, const int y2) const
{
	if(pen.GetStyle()==TRANSPARENT_PEN_STYLE) return;
	if((x2-x1==0)||(y2-y1==0))
		return;

	aalineColor(surface, x1, y1, x2, y2, (Uint32)(*pen.GetColor()));
/*	SDL_Rect rc;
	rc.x=x1-(pen.GetWidth()>>1);rc.y=y1-(pen.GetWidth()>>1);
	if(x2<x1) {
		rc.w=x1-x2;rc.x-=rc.w;
	} else rc.w=x2-x1;
		
	if(y2<y1) {
		rc.h=y1-y2;rc.y-=rc.h;
	} else rc.h=y2-y1;
	SDL_FillRect(surface, &rc, (Uint32)(*pen.GetColor()));*/
}

void DC::DrawText(const string& text, const int x, const int y) const
{
	font->DrawText(surface, textColor, text, x, y);
}

const bool DC::Lock() const {
	if(SDL_MUSTLOCK(surface)) {
		return SDL_LockSurface(surface) == 0;
	}
	else return true;
}

void DC::Unlock() const {
	if(SDL_MUSTLOCK(surface)) {
		SDL_UnlockSurface(surface);
	}
}

const int DC::Blit(const DC& src, const SDL_Rect& srcrect, SDL_Rect& dstrect) const {
	return SDL_BlitSurface(src.surface, const_cast<SDL_Rect*>(&srcrect), surface, &dstrect);
}

const int DC::Blit(const DC& src, const SDL_Rect& srcrect, const Point& dstpoint) const {
SDL_Rect  rect;
rect.x = dstpoint.x; rect.y = dstpoint.y;
return Blit(src, srcrect, rect);
}

const int DC::Blit(const DC& src, SDL_Rect& dstrect) const {
return SDL_BlitSurface(src.surface, 0, surface, &dstrect);
}

const int DC::Blit(const DC& src, const Point& dstpoint) const {
SDL_Rect  rect;
rect.x = dstpoint.x; rect.y = dstpoint.y;
return Blit(src, rect);
}

const int DC::Blit(const DC& src) const {
return SDL_BlitSurface(src.surface, 0, surface, 0);
}

const bool DC::Fill(const Color color) const {
	return SDL_FillRect(surface, 0, color) == 0;
}

const bool DC::FillRect(SDL_Rect& dstrect, const Color color) const{
	return(boxColor(surface, dstrect.x, dstrect.y, dstrect.x+dstrect.w, dstrect.y+dstrect.h, (Uint32)color )==0);
//return SDL_FillRect(surface, &dstrect, color) == 0;
}

// Set various things
const bool DC::SetColorKey(const Uint32 flag, const Color key) const {
	return SDL_SetColorKey(surface, flag, key) == 0;
}

const bool DC::SetAlpha(const Uint32 flag, const Uint8 alpha) const {
	return SDL_SetAlpha(surface, flag, alpha) == 0;
}

void DC::SetClipRect(const SDL_Rect& rect) const {
	SDL_SetClipRect(surface, const_cast<SDL_Rect*>(&rect));
}

void DC::ResetClipRect() const {
	SDL_SetClipRect(surface, 0);
}

void DC::GetClipRect(SDL_Rect& rect) const {
	SDL_GetClipRect(surface, &rect);
}

bool DC::SaveBMP(const char *file) const 
{
	if(surface) return SDL_SaveBMP(surface, file) == 0;
	return false;
}

Brush::Brush(const Color color, const eBrushStyle style) 
{this->color=color;this->style=style;}

Brush::Brush(SDL_Surface* surface, const Uint8 r, const Uint8 g, const Uint8 b, const eBrushStyle style) 
{
	color = Color(surface, r, g, b);
//	color.scol.r=r;
//	color.scol.g=g;
//	color.scol.b=b;
//	color.ucol=SDL_MapRGB(surface->format, r, g, b);
	this->style=style;
}


Pen::Pen(const Color color, const int width, const ePenStyle style)
{this->color=color;this->width=width;this->style=style;}

Pen::Pen(SDL_Surface* surface, const Uint8 r, const Uint8 g, const Uint8 b, const int width, const ePenStyle style)
{
	color = Color(surface, r, g, b);
//	color.scol.r=r;
//	color.scol.g=g;
//	color.scol.b=b;
//	color.ucol=SDL_MapRGB(surface->format, r, g, b);
	this->width=width;this->style=style;
}



//----------------------------------------------------------

// Examine the depth of a surface and select a line
// drawing routine optimized for the bytes/pixel of the
// surface.

void DC::DrawBresLine(const int x1, const int y1, const int x2, const int y2) const
{
	lineColor(surface, x1, y1, x2, y2, (Uint32)(*pen.GetColor()));
}


