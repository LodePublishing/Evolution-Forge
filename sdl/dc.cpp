#include "dc.hpp"
#include <stdlib.h>

DC::DC():
	surface(),
	pressedRectangle(false),
	initOK(true),
	brush(),
	pen(),
	color(NULL),
	textColor(),
	font(NULL),
	bitDepth(DEPTH_8BIT),
	resolution(RESOLUTION_640x480),
	Draw_HLine(NULL),
	Draw_VLine(NULL),
	Draw_Line(NULL),
	DrawFilledRound(NULL),
	DrawFilledEdgedRound(NULL),
	DrawEmptyEdgedRound(NULL),
	DrawEmptyRound(NULL),
	DrawFilledEdgedBorderRound(NULL)
{ 
}

DC::DC(const DC& other):
	surface(other.surface),
	pressedRectangle(other.pressedRectangle),
	initOK(other.initOK),
	brush(other.brush),
	pen(other.pen),
	color(other.color),
	textColor(other.textColor),
	font(other.font),
	bitDepth(other.bitDepth),
	resolution(other.resolution),
	Draw_HLine(other.Draw_HLine),
	Draw_VLine(other.Draw_VLine),
	Draw_Line(other.Draw_Line),
	DrawFilledRound(other.DrawFilledRound),
	DrawFilledEdgedRound(other.DrawFilledEdgedRound),
	DrawEmptyEdgedRound(other.DrawEmptyEdgedRound),
	DrawEmptyRound(other.DrawEmptyRound),
	DrawFilledEdgedBorderRound(other.DrawFilledEdgedBorderRound)
{ 
	changedRectangles=0;
}


DC::DC(const eResolution current_resolution, const eBitDepth bit_depth, const Uint32 nflags, const Uint32 initflags) :
	surface(NULL),
	pressedRectangle(false),
	initOK(true),
	brush(),
	pen(),
	color(NULL),
	textColor(),
	font(NULL),
// evtl gar nicht initialisieren...
	bitDepth(bit_depth),
	resolution(current_resolution),
	Draw_HLine(NULL),
	Draw_VLine(NULL),
	Draw_Line(NULL),
	DrawFilledRound(NULL),
	DrawFilledEdgedRound(NULL),
	DrawEmptyEdgedRound(NULL),
	DrawEmptyRound(NULL),
	DrawFilledEdgedBorderRound(NULL)
{
	if ( SDL_Init(initflags) < 0 )
	{
		initOK = false;
		return;
	}
	atexit(SDL_Quit);
	setScreen(resolution, bit_depth, nflags);
}

DC::~DC()
{
	SDL_FreeSurface(surface);
}

#include <sstream>
std::string DC::printHardwareInformation()
{
	SDL_Rect **modes;
	std::ostringstream os;
	os.str("");
	modes = SDL_ListModes(NULL, SDL_SWSURFACE);
	if(modes == (SDL_Rect **)0)
		os << "No modes available!" << std::endl;
	else
	{
		if(modes == (SDL_Rect **)-1)
			os << "All resolutions available." << std::endl;
		else
		{
			os << "Available Modes:" << std::endl;
			for(unsigned int i=0;modes[i];++i)
				os << "  " << modes[i]->w << " x " << modes[i]->h << std::endl;
		}
	}
	const SDL_VideoInfo* hardware = SDL_GetVideoInfo();
	os << "Max color depth : " << (unsigned int)hardware->vfmt->BitsPerPixel << std::endl;
//	if(hardware->hw_availible) os << "- It is possible to create hardware surfaces" << std::endl;
	if(hardware->wm_available) os << "- There is a window manager available" << std::endl;
	if(hardware->blit_hw) os << "- Hardware to hardware blits are accelerated" << std::endl;
	if(hardware->blit_hw_CC) os << "- Hardware to hardware colorkey blits are accelerated" << std::endl;
	if(hardware->blit_hw_A) os << "- Hardware to hardware alpha blits are accelerated" << std::endl;
	if(hardware->blit_sw) os << "- Software to hardware blits are accelerated" << std::endl;
	if(hardware->blit_sw_CC) os << "- Software to hardware colorkey blits are accelerated" << std::endl;
	if(hardware->blit_sw_A)	os << "- Software to hardware alpha blits are accelerated" << std::endl;
	if(hardware->blit_fill)	os << "- Color fills are accelerated" << std::endl;
	if(hardware->video_mem>0) os << "- Total amount of video memory: " << hardware->video_mem << "kb";
	return(os.str());
}

std::string DC::printSurfaceInformation(DC* surface)
{
	std::ostringstream os;
	os.str("");
	os << "Created Surface :  " << surface->GetSurface()->w << " x " << surface->GetSurface()->h << " @ " << (unsigned int)(surface->GetSurface()->format->BitsPerPixel) << std::endl;
	if (surface->flags() & SDL_SWSURFACE) os << "- Surface is stored in system memory" << std::endl;
	else if(surface->flags() & SDL_HWSURFACE) os << "- Surface is stored in video memory" << std::endl;
	if(surface->flags() & SDL_ASYNCBLIT) os << "- Surface uses asynchronous blits if possible" << std::endl;
	if(surface->flags() & SDL_ANYFORMAT) os << "- Allows any pixel-format" << std::endl;
	if(surface->flags() & SDL_HWPALETTE) os << "- Surface has exclusive palette" << std::endl;
	if(surface->flags() & SDL_DOUBLEBUF) os << "- Surface is double buffered" << std::endl;
	if(surface->flags() & SDL_OPENGL) os << "- Surface has an OpenGL context" << std::endl;
	if(surface->flags() & SDL_OPENGLBLIT) os << "- Surface supports OpenGL blitting" << std::endl;
	if(surface->flags() & SDL_RESIZABLE) os << "- Surface is resizable" << std::endl;
	if(surface->flags() & SDL_HWACCEL) os << "- Surface blit uses hardware acceleration" << std::endl;
	if(surface->flags() & SDL_SRCCOLORKEY) os << "- Surface use colorkey blitting" << std::endl;
	if(surface->flags() & SDL_RLEACCEL) os << "- Colorkey blitting is accelerated with RLE" << std::endl;
	if(surface->flags() & SDL_SRCALPHA) os << "- Surface blit uses alpha blending" << std::endl;
	if(surface->flags() & SDL_PREALLOC) os << "- Surface uses preallocated memory" << std::endl;
	if(SDL_MUSTLOCK(surface->GetSurface())) os << "- Surface needs locking" << std::endl;
	return(os.str());
}

std::list<std::string> DC::getAvailibleDrivers()
{
	std::list<std::string> availible_drivers;
#ifdef __WIN32__
	availible_drivers.push_back("directx");
	availible_drivers.push_back("windib");
#elif __linux__
	availible_drivers.push_back("x11");
	availible_drivers.push_back("dga");
	availible_drivers.push_back("nano");
	availible_drivers.push_back("fbcon");
	availible_drivers.push_back("directfb");
	availible_drivers.push_back("svgalib");	
	availible_drivers.push_back("aalib");
#endif
	return(availible_drivers);
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
#include "../stl/misc.hpp"
#include <sstream>

void DC::addRectangle(const Rect& rect)
{
	if(changedRectangles>199)
		return;
	SDL_Rect r;
	if(rect.GetLeft() < 0)
		r.x = 0;
	else if(rect.GetLeft() >= max_x)
		return;
	else
		r.x = rect.GetLeft();

	if(rect.GetTop() < 0)
		r.y = 0;
	else if(rect.GetTop() >= max_y)
		return;
	else 
		r.y = rect.GetTop();
	
	if(rect.GetRight() < 0)
		return;
	else if(rect.GetRight() > max_x)
		r.w = max_x - r.x;
	else 
		r.w = rect.GetRight() - r.x;

	if(rect.GetBottom() < 0)
		return;
	else if(rect.GetBottom() > max_y)
		r.h = max_y - r.y;
	else 
		r.h = rect.GetBottom() - r.y;
	std::ostringstream os;

	for(unsigned int i = 0; i < changedRectangles; i ++)
	{
		if(
			(r.x >= changedRectangle[i].x)&&
			(r.x + r.w <= changedRectangle[i].x + changedRectangle[i].w)&&
			(r.y >= changedRectangle[i].y)&&
			(r.y + r.h <= changedRectangle[i].y + changedRectangle[i].h)
		)
			return;
		if(
			(r.x <= changedRectangle[i].x)&&
			(r.x + r.w >= changedRectangle[i].x + changedRectangle[i].w)&&
			(r.y <= changedRectangle[i].y)&&
			(r.y + r.h >= changedRectangle[i].y + changedRectangle[i].h)
		)
		{
			changedRectangle[i] = r;
			return;
		}
	}
	
	changedRectangle[changedRectangles] = r;
	changedRectangles++;
}

void DC::updateScreen()
{
/*	for(unsigned int i = changedRectangles; i--;)
	{
		SetPen(Pen(surface, rand()%256, rand()%256, rand()%256, 2, SOLID_PEN_STYLE)); 
		DrawEmptyRectangle(changedRectangle[i].x, changedRectangle[i].y, changedRectangle[i].w, changedRectangle[i].h);
	}*/

	SDL_UpdateRects(surface, changedRectangles, changedRectangle);
	changedRectangles=0;
}

void DC::DrawBitmap(SDL_Surface* bitmap, const signed int x, const signed int y) const
{
	SDL_Rect drect;
	drect.x = x;
	drect.y = y;
	drect.w = bitmap->w;
	drect.h = bitmap->h;
	if((drect.x + drect.w >= max_x)||(drect.y + drect.h >= max_y) || (drect.x < 0) || (drect.y < 0))
		return;
	SDL_BlitSurface(bitmap , NULL, surface, &drect);
}

const Color DC::mixColor(const Color& id1, const Color& id2)  const
{
	return(Color(surface, id1.r()  +id2.r(),
			id1.g()+id2.g(),
			id1.b() +id2.b()));
}

const Color DC::mixColor(const Color& id1, const Color& id2, const unsigned int gradient) const
{
	return(Color(surface, (id1.r()*gradient  +id2.r()*(100-gradient))/100,
		(id1.g()*gradient+id2.g()*(100-gradient))/100,
		(id1.b()*gradient +id2.b()*(100-gradient))/100));
}

const Color DC::changeAbsoluteBrightness(const Color& id, const signed int brightness) const
{
	return(Color(surface, id.r() + brightness,
			id.g() + brightness,
			id.b() + brightness));
}

const Color DC::changeRelativeBrightness(const Color& id, const unsigned int brightness_percent) const
{
	int r = id.r() * brightness_percent / 100;if(r>255) r = 255;
	int g = id.g() * brightness_percent / 100;if(g>255) g = 255;
	int b = id.b() * brightness_percent / 100;if(b>255) b = 255;
	return(Color(surface, r, g , b));
}

void DC::DrawLine(const signed x1, const signed y1, const signed x2, const signed y2) const
{
	if(pen.GetStyle()==TRANSPARENT_PEN_STYLE)
		return;
	signed int xx1, xx2, yy1, yy2;
	if(x1>x2)
		{xx1=x2;xx2=x1;yy1=y2;yy2=y1;}
	else {xx1=x1;xx2=x2;yy1=y1;yy2=y2;}

	if((xx1<pen.GetWidth()/2)||(yy1+1<pen.GetWidth()/2)||(yy2<pen.GetWidth()/2)||(xx2>=max_x-pen.GetWidth()/2)||(yy1>=max_y-pen.GetWidth()/2)||(yy2>=max_y-pen.GetWidth()/2))
		return;
		
	if(yy1==yy2)
	{
		if(xx1==xx2)
			return;
		if(pen.GetWidth()>1)
		{
			SDL_Rect rc;
			rc.x=xx1/*-pen.GetWidth()/2*/;rc.y=yy1/*-pen.GetWidth()/2*/;rc.w=xx2-xx1/*+pen.GetWidth()*/;rc.h=pen.GetWidth();
			SDL_FillRect(surface, &rc, (Uint32)(*pen.GetColor()) );
		} else
			(*this.*Draw_HLine)(xx1, yy1, xx2);
	} else
	if(xx1==xx2)
	{
		if(yy1>yy2)
			{signed int swp=yy1;yy1=yy2;yy2=swp;}
		if(pen.GetWidth()>1)
		{
			SDL_Rect rc;
			rc.x=xx1/*-pen.GetWidth()/2*/;rc.y=yy1/*-pen.GetWidth()/2*/;rc.w=pen.GetWidth();rc.h=yy2-yy1/*+pen.GetWidth()*/;
			SDL_FillRect(surface, &rc, (Uint32)(*pen.GetColor()) );
		} else
			(*this.*Draw_VLine)(xx1, yy1, yy2);
	} else
	{
		(*this.*Draw_Line)(xx1, yy1, xx2, yy2);
		if(pen.GetWidth()==2)
		{
//			Color c = *pen.GetColor();
//			const_cast<DC*>(this)->pen.SetColor(Color(surface, (Uint8)(pen.GetColor()->r()*0.5),  (Uint8)(pen.GetColor()->g()*0.5), (Uint8)(pen.GetColor()->b()*0.5)));
			(*this.*Draw_Line)(xx1, yy1+1, xx2, yy2+1);
			(*this.*Draw_Line)(xx1, yy1, xx2, yy2);
			(*this.*Draw_Line)(xx1, yy1-1, xx2, yy2-1);
//			const_cast<DC*>(this)->pen.SetColor(c);
		}
	}
}


void DC::DrawSpline(const unsigned int c, const Point* p) const
{
	if((pen.GetStyle() == TRANSPARENT_PEN_STYLE)||(c<2))
		return;
	for(unsigned int i=c-1;i--;)
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
	for(unsigned int i=c-1;i--;)
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
void DC::DrawText(const std::string& text, const signed int x, const signed int y) const 
{
	if(font->isShadow())
		font->DrawText(surface, toSDL_Color(0,0,0), text, x+font->getSize()/6, y+font->getSize()/6); 

	font->DrawText(surface, textColor, text, x, y);
	if(font->isUnderlined())
	{
		Size s = font->GetTextExtent(text);
		SDL_Rect r;
		r.x = x;r.y = y+s.GetHeight()*2/3;r.w = s.GetWidth();r.h = 1;
		SDL_FillRect(surface, &r, Color(surface, textColor.r, textColor.g, textColor.b));
	}

}


void DC::setScreen(const eResolution current_resolution, const eBitDepth bit_depth, const Uint32 nflags)
{
	if((current_resolution == resolution) && (bit_depth == bitDepth) && (surface!=NULL))
		return;
	resolution = current_resolution;
	Size s = Size(getResolutionSize());
	max_x = s.GetWidth();
	max_y = s.GetHeight();
	unsigned int bits;
	switch(bit_depth)
	{
		case DEPTH_8BIT:bits=8;break;
		case DEPTH_16BIT:bits=16;break;
		case DEPTH_24BIT:bits=24;break;
		case DEPTH_32BIT:bits=32;break;
		default:bits=8;break;
	}
//	const SDL_VideoInfo* hardware = SDL_GetVideoInfo();
//	if(bits > (unsigned int)hardware->vfmt->BitsPerPixel)
//		bits = (unsigned int)hardware->vfmt->BitsPerPixel; :(

	surface = SDL_SetVideoMode(max_x, max_y, bits, nflags);
	if(surface==NULL)
		return;
	SDL_Rect r;
	r.x=0;r.y=0;r.w=max_x;r.h=max_y;
	changedRectangle[0] = r;
	changedRectangles = 1;

	switch(surface->format->BitsPerPixel)
	{
		case 8:	bitDepth = DEPTH_8BIT;
			Draw_HLine = &DC::Draw_HLine_8bit;
			Draw_VLine = &DC::Draw_VLine_8bit;
			Draw_Line = &DC::Draw_Line_8bit;
			DrawFilledRound = &DC::DrawFilledRound_8bit;
			DrawFilledEdgedRound = &DC::DrawFilledEdgedRound_8bit;
			DrawEmptyEdgedRound = &DC::DrawEmptyEdgedRound_8bit;
			DrawEmptyRound = &DC::DrawEmptyRound_8bit;
			DrawFilledEdgedBorderRound = &DC::DrawFilledEdgedBorderRound_8bit;
			break;
		case 16:bitDepth = DEPTH_16BIT;
			Draw_HLine = &DC::Draw_HLine_16bit;
		        Draw_VLine = &DC::Draw_VLine_16bit;
		        Draw_Line = &DC::Draw_Line_16bit;
		        DrawFilledRound = &DC::DrawFilledRound_16bit;
		        DrawFilledEdgedRound = &DC::DrawFilledEdgedRound_16bit;
		        DrawEmptyEdgedRound = &DC::DrawEmptyEdgedRound_16bit;
		        DrawEmptyRound = &DC::DrawEmptyRound_16bit;
		        DrawFilledEdgedBorderRound = &DC::DrawFilledEdgedBorderRound_16bit;
			break;
		case 24:bitDepth = DEPTH_24BIT;
			Draw_HLine = &DC::Draw_HLine_24bit;
		        Draw_VLine = &DC::Draw_VLine_24bit;
		        Draw_Line = &DC::Draw_Line_24bit;
		        DrawFilledRound = &DC::DrawFilledRound_24bit;
		        DrawFilledEdgedRound = &DC::DrawFilledEdgedRound_24bit;
		        DrawEmptyEdgedRound = &DC::DrawEmptyEdgedRound_24bit;
		        DrawEmptyRound = &DC::DrawEmptyRound_24bit;
		        DrawFilledEdgedBorderRound = &DC::DrawFilledEdgedBorderRound_24bit;
			break;
		case 32:bitDepth = DEPTH_32BIT;
			Draw_HLine = &DC::Draw_HLine_32bit;
		        Draw_VLine = &DC::Draw_VLine_32bit;
		        Draw_Line = &DC::Draw_Line_32bit;
		        DrawFilledRound = &DC::DrawFilledRound_32bit;
		        DrawFilledEdgedRound = &DC::DrawFilledEdgedRound_32bit;
		        DrawEmptyEdgedRound = &DC::DrawEmptyEdgedRound_32bit;
		        DrawEmptyRound = &DC::DrawEmptyRound_32bit;
		        DrawFilledEdgedBorderRound = &DC::DrawFilledEdgedBorderRound_32bit;
			break;
		default:break;
	}
}

const Size DC::getResolutionSize() const
{
	switch(resolution)
	{
		case RESOLUTION_640x480:return(Size(640, 480));break;
		case RESOLUTION_800x600:return(Size(800, 600));break;
		case RESOLUTION_1024x768:return(Size(1024, 768));break;
		case RESOLUTION_1280x1024:return(Size(1280, 1024));break;
//		case RESOLUTION_1600x1200:break;
		default:return(Size(0,0));break;
	}
}

void DC::setBitDepth(const eBitDepth bit_depth)
{
	setScreen(resolution, bit_depth, surface->flags);
}

void DC::setResolution(const eResolution current_resolution)
{
	setScreen(current_resolution, bitDepth, surface->flags);
}

#if 0
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
#endif

void DC::DrawEdgedRoundedRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int radius) const
{
	if((width<2)||(height<2)||(x>=max_x)||(y>=max_y)||(x<0)||(y<0))
		return;
	
	unsigned int mw = width;
	unsigned int mh = height;

	if(x + width >= max_x)
		mw = max_x - x;
	if(y + height >= max_y)
		mh = max_y - y;
	
	if((radius <= 1)||(mw<2*radius)||(mh<2*radius))	{
		DrawRectangle(x,y,mw,mh);
		return;
	}

	if(brush.GetStyle() == TRANSPARENT_BRUSH_STYLE)
	{
		if(pen.GetStyle() != TRANSPARENT_PEN_STYLE)
			(*this.*DrawEmptyEdgedRound)(x, y, mw, mh, radius);
	} else
	if(pen.GetStyle() == TRANSPARENT_PEN_STYLE)
		(*this.*DrawFilledEdgedRound)(x, y, mw, mh, radius);
	else 
		(*this.*DrawFilledEdgedBorderRound)(x, y, mw, mh, radius);
}

void DC::DrawRoundedRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int radius) const
{
	if((width<2)||(height<2)||(x+width>=max_x)||(y+height>=max_y)||(x<0)||(y<0)) return;
	
	if((radius <= 1)||(width<2*radius)||(height<2*radius))	{
		DrawRectangle(x,y, width, height);
		return;
	}

	if (brush.GetStyle() != TRANSPARENT_BRUSH_STYLE)
		(*this.*DrawFilledRound)(x, y, width, height, radius);
	if (pen.GetStyle() != TRANSPARENT_PEN_STYLE)
		(*this.*DrawEmptyRound)(x, y, width, height, radius);
}


void DC::DrawRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height) const
{
	if((width<2)||(height<2)||(width>max_x)||(height>max_y)||(x<0)||(y<0)) return;
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
	if((width<2)||(height<2)||(width>max_x)||(height>max_y)||(x<0)||(y<0)) return;
	
	Uint32 dark_pen_col, bright_pen_col;
	if(pressedRectangle)
	{
		dark_pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 110));
		bright_pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 70));
	
	} else
	{
		dark_pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 80));
		bright_pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 120));
	}

	SDL_Rect rc;
	rc.x=x-(pen.GetWidth()>>1);rc.y=y-(pen.GetWidth()>>1);rc.w=width;rc.h=pen.GetWidth();
	SDL_FillRect(surface, &rc, bright_pen_col);
	
	rc.x=x-(pen.GetWidth()>>1);rc.y=y+height-1-(pen.GetWidth()>>1);rc.w=width;rc.h=pen.GetWidth();
	SDL_FillRect(surface, &rc, dark_pen_col);
	
	rc.x=x-(pen.GetWidth()>>1);rc.y=y-(pen.GetWidth()>>1);rc.w=pen.GetWidth();rc.h=height;
	SDL_FillRect(surface, &rc, bright_pen_col);
	
	rc.x=x+width-1-(pen.GetWidth()>>1);rc.y=y-(pen.GetWidth()>>1);rc.w=pen.GetWidth();rc.h=height;
	SDL_FillRect(surface, &rc, dark_pen_col);
}

void DC::DrawVerticalLine(const signed int x0, const signed int y0, const signed int y1) const
{
	if((y1<0)||(y0>=max_y)||(x0<0)||(x0>=max_x)||(y1<y0))
	{
//		std::ostringstream os;
//		os.str("");
//		os << "Line out of range: " << x0 << ", " << y0 << ", " << y1;
//		toLog("Line out of range");//os.str());
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
	(*this.*Draw_VLine)(x0, yy0, yy1);
}

void DC::DrawHorizontalLine(const signed int x0, const signed int y0, const signed int x1) const
{
	if((x1<0)||(x0>=max_x)||(y0<0)||(y0>=max_y)||(x1<x0))
	{
//	  std::ostringstream os;
//	  os.str("");
//	  os << "Line out of range: " << x0 << ", " << y0 << ", " << y1;
//		toLog("Line out of range");//os.str());
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
	(*this.*Draw_HLine)(xx0, y0, xx1);
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

const eChooseDriverError DC::chooseDriver(std::string& driver_name)
{
	std::list<std::string> availible_drivers = DC::getAvailibleDrivers();
	if(availible_drivers.empty())
		return(NO_VIDEO_DRIVERS_AVAILIBLE);
	if(driver_name=="")
		driver_name = *availible_drivers.begin();
	for(std::list<std::string>::const_iterator j = availible_drivers.begin();j!=availible_drivers.end();++j)
		if(driver_name == *j)
		{
			std::ostringstream video;
			video.str("");
			video << "SDL_VIDEODRIVER=" << driver_name;
			char* video_cstr = new char[strlen(video.str().c_str())];
			strcpy(video_cstr, video.str().c_str());
//			putenv(video_cstr);
			return(NO_DRIVER_ERROR);
		}
	return(SDL_DRIVER_NOT_SUPPORTED);
}

SDL_Rect DC::changedRectangle[200];
unsigned int DC::changedRectangles=0;
Uint16 DC::max_x = 0;
Uint16 DC::max_y = 0;
