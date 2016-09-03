#include "dc.hpp"

DC::DC():
	surface(),
	max_x(0),
	max_y(0),
	brush(),
	pen(),
	color(NULL),
	textColor(),
	font(NULL)
{ }

DC::DC(const DC& other):
	surface(other.surface),
	max_x(0),
	max_y(0),
	brush(),
	pen(),
	color(NULL),
	textColor(),
	font(NULL)
{ }
// TODO?

DC::DC(const unsigned int width, const unsigned int height, const unsigned int bitdepth, Uint32 nflags) :
	surface(SDL_SetVideoMode(width, height, bitdepth, nflags)),
	max_x(0),
	max_y(0),
	brush(),
	pen(),
	color(NULL),
	textColor(),
	font(NULL)
{
/*	switch(surface->format->BitsPerPixel)
	{
		case 8:(DC::Draw_HLine) = (void (unsigned int, unsigned int, unsigned int)) Draw_HLine_8bit;break;
//		case 15:
//		case 16:Draw_HLine = &DC::Draw_HLine_16bit;break;
//		case 24:Draw_HLine = &DC::Draw_HLine_24bit;break;
//		case 32:Draw_HLine = &DC::Draw_HLine_32bit;break;
		default:toLog("Bit Error");break;
	}*/
}

void DC::setFullscreen(const bool full_screen)
{
	if(((surface->flags & SDL_FULLSCREEN) == SDL_FULLSCREEN) != full_screen)
	{
#ifdef __linux
		SDL_WM_ToggleFullScreen(surface);
#elif __WIN32__
		Uint32 nflags = surface->flags;
		nflags ^= SDL_FULLSCREEN;
		surface = SDL_SetVideoMode(surface->w, surface->h, surface->format->BitsPerPixel, nflags);
#endif
	}										
}

void DC::updateScreen() const
{
	SDL_Flip(surface);
//	SDL_UpdateRect(surface, 0, 0, 0, 0);
}
void DC::DrawBitmap(const Bitmap& bitmap, const signed int x, const signed int y) const
{
	SDL_Rect drect;
	drect.x = x;
	drect.y = y;
	drect.w = bitmap->w;
	drect.h = bitmap->h;
//	if((drect.x + drect.w >= max_x)||(drect.y + drect.h >= max_y) || (drect.x < 0) || (drect.y < 0))
//		return;
	SDL_BlitSurface(bitmap , 0, surface, &drect );
}

const Color DC::mixColor(const Color* id1, const Color* id2)  const
{
	return(Color(surface, id1->r()  +id2->r(),
			id1->g()+id2->g(),
			id1->b() +id2->b()));
}

const Color DC::mixColor(const Color* id1, const Color* id2, const unsigned int gradient) const
{
	return(Color(surface, (id1->r()*gradient  +id2->r()*(100-gradient))/100,
		(id1->g()*gradient+id2->g()*(100-gradient))/100,
		(id1->b()*gradient +id2->b()*(100-gradient))/100));
}

const Color DC::brightenColor(const Color* id, const unsigned int brightness) const
{
	return(Color(surface, id->r()  +brightness,
			id->g()+brightness,
			id->b() +brightness));
}

const Color DC::darkenColor(const Color* id, const unsigned int brightness) const
{
	return(Color(surface, id->r() * brightness / 100,
			id->g() * brightness / 100,
			id->b() * brightness / 100));
}

void DC::DrawLine(const signed x1, const signed y1, const signed x2, const signed y2) const
{
	if(pen.GetStyle()==TRANSPARENT_PEN_STYLE)
			return;
	int xx1 = x1;
	int xx2 = x2;
	int yy1 = y1;
	int yy2 = y2;

	if((xx1<pen.GetWidth())||(yy1<pen.GetWidth())||(xx1>=max_x-pen.GetWidth())||(yy1>=max_y-pen.GetWidth())||(xx2<pen.GetWidth())||(yy2<pen.GetWidth())||(xx2>=max_x-pen.GetWidth())||(yy2>=max_y-pen.GetWidth()))
		return;

	Draw_Line(x1, y1, x2, y2);
	if(pen.GetWidth()>1)
	{
		Draw_Line(x1, y1+1, x2, y2+1);
		Draw_Line(x1+1, y1, x2+1, y2);
		Draw_Line(x1+1, y1+1, x2+1, y2+1);
	}
}


void DC::DrawSpline(const unsigned int c, const Point* p) const
{
	if((pen.GetStyle() == TRANSPARENT_PEN_STYLE)||(c<2))
		return;
	for(unsigned int i=0;i<c-1;i++)
//	{
//		aalineColor(surface, p[i].x, p[i].y, p[i+1].x, p[i+1].y, (Uint32)(*pen.GetColor()));
//		aalineColor(surface, p[i].x, p[i].y+1, p[i+1].x, p[i+1].y+1, (Uint32)(*pen.GetColor()));
//	}
	{
/*		if((i>0)&&(i<c-2))
		{
			DrawLine(p[i].x, p[i].y + (p[i].y < p[i-1].y) + (p[i].y < p[i+1].y) - (p[i].y > p[i-1].y) - (p[i].y > p[i+1].y),
					   p[i+1].x, p[i+1].y + (p[i+1].y < p[i].y) + (p[i+1].y < p[i+2].y) - (p[i+1].y > p[i].y) - (p[i+1].y > p[i+2].y));
			DrawLine(p[i].x, p[i].y + (p[i].y < p[i-1].y) + (p[i].y < p[i+1].y) - (p[i].y > p[i-1].y) - (p[i].y > p[i+1].y) + 1,
					   p[i+1].x, p[i+1].y + (p[i+1].y < p[i].y) + (p[i+1].y < p[i+2].y) - (p[i+1].y > p[i].y) - (p[i+1].y > p[i+2].y) + 1);
		} else*/
		{
			DrawLine(p[i].x, p[i].y, p[i+1].x, p[i+1].y);
//			DrawLine(p[i].x, p[i].y+1, p[i+1].x, p[i+1].y+1);
		}
	}
}
	
void DC::DrawSpline(const unsigned int c, const Point* p, const Point s) const
{
	if((pen.GetStyle() == TRANSPARENT_PEN_STYLE)||(c<2))
		return;
	for(unsigned int i=0;i<c-1;i++)
//	{
//		aalineColor(surface, p[i].x, p[i].y, p[i+1].x, p[i+1].y, (Uint32)(*pen.GetColor()));
//		aalineColor(surface, p[i].x, p[i].y+1, p[i+1].x, p[i+1].y+1, (Uint32)(*pen.GetColor()));
//	}
	{
/*		if((i>0)&&(i<c-2))
		{
			DrawLine(p[i].x, p[i].y + (p[i].y < p[i-1].y) + (p[i].y < p[i+1].y) - (p[i].y > p[i-1].y) - (p[i].y > p[i+1].y),
					   p[i+1].x, p[i+1].y + (p[i+1].y < p[i].y) + (p[i+1].y < p[i+2].y) - (p[i+1].y > p[i].y) - (p[i+1].y > p[i+2].y));
			DrawLine(p[i].x, p[i].y + (p[i].y < p[i-1].y) + (p[i].y < p[i+1].y) - (p[i].y > p[i-1].y) - (p[i].y > p[i+1].y) + 1,
					   p[i+1].x, p[i+1].y + (p[i+1].y < p[i].y) + (p[i+1].y < p[i+2].y) - (p[i+1].y > p[i].y) - (p[i+1].y > p[i+2].y) + 1);
		} else*/
		{
			DrawLine(s.x + p[i].x, s.y + p[i].y, s.x + p[i+1].x, s.y + p[i+1].y);
	//		DrawLine(s.x + p[i].x, s.y + p[i].y+1, s.x + p[i+1].x, s.y + p[i+1].y+1);
		}
	}
	
}

void DC::setResolution(const unsigned int dc_max_x, const unsigned int dc_max_y)
{
	max_x=dc_max_x;
	max_y=dc_max_y;
}

void DC::DrawGridEdgedRoundedRectangle(const signed int x, const signed y, const unsigned width, const unsigned int height, const unsigned int radius, std::list<Rect> notDrawRectList) const 
{
	return;
	if((width<2)||(height<2)) return;
	if((x>=max_x)||(x<0)) return;
	if((y>=max_y)||(y<0)) return;
	unsigned int ww = width;
	unsigned int hh = height;
	if(x+ww>=max_x) ww = max_x - 1 - x;
	if(y+hh>=max_y) hh = max_y - 1 - y;
	bool rounded = false;
	if((radius <= 1)||(ww<2*radius)||(hh<2*radius))
		rounded=true;

	signed int xx = x;
	signed int yy = y;
	unsigned int lastHeight = 0;

	std::list<Rect>::const_iterator i = notDrawRectList.begin();
	while(i!=notDrawRectList.end())
	{
		lastHeight=0;
		while((i!=notDrawRectList.end())&&(yy == i->GetTop()))
		{
			DrawRectangle(xx, yy, i->GetLeft() - xx, i->GetHeight());
			lastHeight = i->GetHeight();
			xx = i->GetLeft() + i->GetWidth();
			i++;
		}
		// rechter Rand
		DrawRectangle(xx, yy, x+ww - xx, lastHeight);
		// neue Zeile
		xx = x;
		yy += lastHeight;
		if((i!=notDrawRectList.end())&&(yy < i->GetTop()))
		{
			DrawRectangle(xx, yy, ww, i->GetTop() - yy);
			yy = i->GetTop();
			xx = x;
		}
	}
	DrawRectangle(xx, yy, x+ww - xx, y + hh - yy);		
				
/*	if(rounded) {
		DrawRectangle(x,y,ww,hh);
		return;
	}
	
	if (brush.GetStyle() != TRANSPARENT_BRUSH_STYLE)
		DrawFilledEdgedRound(x, y, ww, hh, radius);
	if (pen.GetStyle() != TRANSPARENT_PEN_STYLE)
		DrawEmptyEdgedRound(x, y, ww, hh, radius);*/
}

void DC::DrawEdgedRoundedRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int radius) const
{
	if((width<2)||(height<2)) return;
	if((x>=max_x)||(x<0)) return;
	if((y>=max_y)||(y<0)) return;
	unsigned int ww = width;
	unsigned int hh = height;
	if(x+ww>=max_x) ww = max_x - 1 - x;
	if(y+hh>=max_y) hh = max_y - 1 - y;
	
	if((radius <= 1)||(ww<2*radius)||(hh<2*radius))	{
		DrawRectangle(x,y,ww,hh);
		return;
	}

	if(brush.GetStyle() == TRANSPARENT_BRUSH_STYLE)
	{
		if(pen.GetStyle() != TRANSPARENT_PEN_STYLE)
			DrawEmptyEdgedRound(x, y, ww, hh, radius);
	} else
	if(pen.GetStyle() == TRANSPARENT_PEN_STYLE)
		DrawFilledEdgedRound(x, y, ww, hh, radius);
	else 
		DrawFilledEdgedBorderRound(x, y, ww, hh, radius);
}

void DC::DrawRoundedRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int radius) const
{
	if((width<2)||(height<2)) return;
	if((x>=max_x)||(x<0)) return;
	if((y>=max_y)||(y<0)) return;
	unsigned int ww = width;
	unsigned int hh = height;
	if(x+ww>=max_x) ww = max_x - 1 - x;
	if(y+hh>=max_y) hh = max_y - 1 - y;
	
	if((radius <= 1)||(ww<2*radius)||(hh<2*radius))	{
		DrawRectangle(x,y,ww,hh);
		return;
	}

	if (brush.GetStyle() != TRANSPARENT_BRUSH_STYLE)
		DrawFilledRound(x, y, ww, hh, radius);
	if (pen.GetStyle() != TRANSPARENT_PEN_STYLE)
		DrawEmptyRound(x, y, ww, hh, radius);
}


void DC::DrawRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height) const
{
	if((width<2)||(height<2)) 
		return;
	if((x<0)||(y<0)||(x+width>=max_x)||(y+height>=max_y)) // TODO clipping
		return;
	SDL_Rect rc;
	rc.x=x+1;rc.y=y+1;rc.w=width-2;rc.h=height-2;
	if(brush.GetStyle() != TRANSPARENT_BRUSH_STYLE)
		SDL_FillRect(surface, &rc, (Uint32)(*brush.GetColor()) );
	DrawEmptyRectangle(x, y, width, height);	
}

void DC::DrawEmptyRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height) const
{
	if(pen.GetStyle()==TRANSPARENT_PEN_STYLE) 
		return;
	SDL_Rect rc;
	if((width<2)||(height<2)||(x<0)||(y<0)||(x+width>=max_x)||(y+height>=max_y)) // TODO clipping
		return;

	rc.x=x-(pen.GetWidth()>>1);rc.y=y-(pen.GetWidth()>>1);rc.w=width;rc.h=pen.GetWidth();
	SDL_FillRect(surface, &rc, (Uint32)(*pen.GetColor()));
	
	rc.x=x-(pen.GetWidth()>>1);rc.y=y+height-1-(pen.GetWidth()>>1);rc.w=width;rc.h=pen.GetWidth();
	SDL_FillRect(surface, &rc, (Uint32)(*pen.GetColor()));
	
	rc.x=x-(pen.GetWidth()>>1);rc.y=y-(pen.GetWidth()>>1);rc.w=pen.GetWidth();rc.h=height;
	SDL_FillRect(surface, &rc, (Uint32)(*pen.GetColor()));
	
	rc.x=x+width-1-(pen.GetWidth()>>1);rc.y=y-(pen.GetWidth()>>1);rc.w=pen.GetWidth();rc.h=height;
	SDL_FillRect(surface, &rc, (Uint32)(*pen.GetColor()));
}

void DC::DrawVerticalLine(const signed int x0, const signed int y0, const signed int y1) const
{
	if((y1<0)||(y0>=max_y)||(x0<0)||(x0>=max_x)||(y1<y0))
	{
//		std::ostringstream os;
//		os << "Line out of range: " << x0 << ", " << y0 << ", " << y1;
		toLog("Line out of range");//os.str());
		return;
	}
	signed int yy0;
	signed int yy1;
	if(y0 < 0)
		yy0=0;
	else yy0=y0;
	if(y1>=max_y)
		yy1=max_y-1;
	else 
		yy1=y1;
	Draw_VLine(x0, yy0, yy1);		
}

void DC::DrawHorizontalLine(const signed int x0, const signed int y0, const signed int x1) const
{
	if((x1<0)||(x0>=max_x)||(y0<0)||(y0>=max_y)||(x1<x0))
	{
//	  std::ostringstream os;
//	  os << "Line out of range: " << x0 << ", " << y0 << ", " << y1;
		toLog("Line out of range");//os.str());
		return;
	}
	signed int xx0;
	signed int xx1;
	if(x0 < 0)
		xx0=0;
	else xx0=x0;
	if(x1>=max_x)
		xx1=max_x-1;
	else
		xx1=x1;
	Draw_HLine(xx0, y0, xx1);
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
/*
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
//	return(boxColor(surface, dstrect.x, dstrect.y, dstrect.x+dstrect.w, dstrect.y+dstrect.h, (Uint32)color )==0);
	return SDL_FillRect(surface, &dstrect, color) == 0;
}
*/
// Set various things


