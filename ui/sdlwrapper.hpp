#ifndef _UI_SDLWRAPPER_HPP
#define _UI_SDLWRAPPER_HPP

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"

#include <sstream>
#include <iomanip>
#include <string>
#include <iostream>
using namespace std;


//extern void (*Draw_Round)(SDL_Surface *super,
  //                 Sint16 x0,Sint16 y0, Uint16 w,Uint16 h,
    //               Uint16 corner, Uint32 color);
enum eBrushStyle
{
    TRANSPARENT_BRUSH_STYLE,
    SOLID_BRUSH_STYLE,
    BDIAGONAL_HATCH_BRUSH_STYLE,
    CROSSDIAG_HATCH_BRUSH_STYLE,
    FDIAGONAL_HATCH_BRUSH_STYLE,
    CROSS_HATCH_BRUSH_STYLE,
    HORIZONTAL_HATCH_BRUSH_STYLE,
    VERTICAL_HATCH_BRUSH_STYLE,
    STIPPLE_BRUSH_STYLE,
    STIPPLE_MASK_OPAQUE_BRUSH_STYLE,
                                                                                                                                                            
    MAX_BRUSH_STYLES
};
                                                                                                                                                            
enum ePenStyle
{
    TRANSPARENT_PEN_STYLE,
    SOLID_PEN_STYLE,
    DOT_PEN_STYLE,
    LONG_DASH_PEN_STYLE,
    SHORT_DASH_PEN_STYLE,
    DOT_DASH_PEN_STYLE,
                                                                                                                                                            
    MAX_PEN_STYLE
};

class Surface
{
	public:
		Surface(SDL_Surface* surf){surface=surf;};
		Surface() {surface=0;};
		~Surface() {if(surface) SDL_FreeSurface(surface);};
		Surface& operator=(const Surface& o) { surface=o;return *this;};
		operator SDL_Surface*() const { return surface; };
		SDL_Surface* operator->() const { return surface; };
	private:
		SDL_Surface* surface;
};



class Color
{
	public:
		Color(const Uint32 ucol, const SDL_Color scol); 
		Color(SDL_Surface* surface, const int r, const int g, const int b); 
		Color();
		Color(const Color& col);

		operator SDL_Color() const;
		//operator SDL_Color*() const; 
		operator Uint32() const;  
//		operator Uint32*() const;
		const int r() const;
		const int g() const;
		const int b() const;
		~Color();
		Uint32 ucol;
		SDL_Color scol;
};


class Brush
{
	public:
		Brush(const Color color, const eBrushStyle style);
		Brush(SDL_Surface* surface, const int r, const int g, const int b, const eBrushStyle style);
		Brush() {};
		~Brush() {};
		const eBrushStyle GetStyle() const {return style;};
		const Color* GetColor() const {return &color;};
	private:
		eBrushStyle style;
		Color color;
};

class Pen
{
	public:
		Pen(const Color color, const int width, const ePenStyle style); 
		Pen(SDL_Surface* surface, const int r, const int g, const int b, const int width, const ePenStyle style);
		Pen() {};
		
		~Pen() {};
		const ePenStyle GetStyle() const {return style;};
		const int GetWidth() const {return width;};
		const Color* GetColor() const {return &color;};
	private:
		int width;
		Color color;
		ePenStyle style;
};

class Bitmap:public Surface
{
	public:
		Bitmap(const string& name);
		Bitmap(const Bitmap& bmp);
		Bitmap();
		~Bitmap();
};

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
		void Font::DrawText(SDL_Surface* surface, const SDL_Color& color, const string& text, const int x, const int y) const;
	    void GetTextExtent(const string& text, int* dx, int* dy) const;
	
		Font(const string& fname, const int size);
		Font() {font=0;};
		~Font();
		operator TTF_Font* () {return(font);}; 
			
};

class Point
{
public:
    int x, y;
    Point() : x(0), y(0) { }
    Point(const int xx, const int yy) : x(xx), y(yy) { }
    // no copy ctor or assignment operator - the defaults are ok
    // comparison
    const bool operator==(const Point& p) const { return x == p.x && y == p.y; }
    const bool operator!=(const Point& p) const { return !(*this == p); }

	// arithmetic operations (component wise)
    const Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
    const Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }

	Point& operator+=(const Point& p) { x += p.x; y += p.y; return *this; }
    Point& operator-=(const Point& p) { x -= p.x; y -= p.y; return *this; }
};

class Size
{
public:
    // members are public for compatibility (don't use them directly,
    // especially that there names were chosen very unfortunately - they should
    // have been called width and height)
    int x, y;
                                                                                                                                                            
    // constructors
    Size() : x(0), y(0) { }
    Size(const int xx, const int yy) : x(xx), y(yy) { }
                                                                                                                                                            
    // no copy ctor or assignment operator - the defaults are ok
                                                                                                                                                            
    const bool operator==(const Size& sz) const { return x == sz.x && y == sz.y; }
    const bool operator!=(const Size& sz) const { return x != sz.x || y != sz.y; }
                                                                                                                                                            
    // FIXME are these really useful? If they're, we should have += &c as well
    const Size operator+(const Size& sz) const { return Size(x + sz.x, y + sz.y); }
    const Size operator-(const Size& sz) const { return Size(x - sz.x, y - sz.y); }
                                                                                                                                                            
    // accessors
    void Set(const int xx, const int yy) { x = xx; y = yy; }
    void SetWidth(const int w) { x = w; }
    void SetHeight(const int h) { y = h; }
                                                                                                                                                            
    const int GetWidth() const { return x; }
    const int GetHeight() const { return y; }
};

class Rect
{
public:
    Rect()
        : x(0), y(0), width(0), height(0)
        { }
    Rect(const int xx, const int yy, const int ww, const int hh)
        : x(xx), y(yy), width(ww), height(hh)
        { };
    Rect(const Point& topLeft, const Point& bottomRight);
    Rect(const Point& pos, const Size& size);
// default copy ctor and assignment operators ok

    const int GetWidth() const { return width; };
    void SetWidth(const int w) { width = w; };
                                                                                                                                                            
    const int GetHeight() const { return height; };
    void SetHeight(const int h) { height = h; };
                                                                                                                                                            
    const Point GetPosition() const { return Point(x, y); };
    void SetPosition( const Point &p ) { x = p.x; y = p.y; };
                                                                                                                                                            
    const Size GetSize() const { return Size(width, height); };
    void SetSize( const Size &s ) { width = s.GetWidth(); height = s.GetHeight(); };
                                                                                                                                                            
    const int GetLeft() const {return x;};
	const int GetTop() const {return y;};
	const int GetBottom() const { return y + height - 1; };
    const int GetRight()  const { return x + width - 1; };
                                                                                                                                                            
    void SetLeft(const int left) { x = left; };
    void SetRight(const int right) { width = right - x + 1; };
    void SetTop(const int top) { y = top; };
    void SetBottom(const int bottom) { height = bottom - y + 1; };
                                                                                                                                                            
    const Rect operator+(const Rect& rect) const {return(Rect(this->x + rect.x, this->y + rect.y, this->width, this->height));};
    Rect& operator+=(const Rect& rect);

	void operator=(const Rect& rect) {x=rect.x;y=rect.y;width=rect.width;height=rect.height;};
                                                                                                                                                            
    // compare rectangles
    const bool operator==(const Rect& rect) const;
    const bool operator!=(const Rect& rect) const { return !(*this == rect); };
                                                                                                                                                            
    // return TRUE if the point is (not strcitly) inside the rect
    const bool Inside(const int x, const int y) const;
    const bool Inside(const Point& pt) const { return Inside(pt.x, pt.y); };
public:
    int x, y, width, height;
};

class DC
{
	public:
		DC() {surface=0;};
		
		~DC() {if(surface) SDL_FreeSurface(surface);};
        operator SDL_Surface*() const { return surface; };
        SDL_Surface* operator->() const { return surface; };
		SDL_Surface* GetSurface() const {return(surface);};
		const bool Lock() const;
		void Unlock() const;

		const Color doColor(const Uint8 r, const Uint8 g, const Uint8 b) const;
		
		const Color mixColor(const Color* id1, const Color* id2) const;
		const Color mixColor(const Color* id1, const Color* id2, const int gradient) const;
		const Color brightenColor(const Color* id, const int brightness) const;

		const bool valid() const {return (surface!=0);};
		Uint32 flags() const {return GetSurface()->flags;};
		const int w() const {return GetSurface()->w;};
		const int h() const { return GetSurface()->h; }                      
		Uint16 pitch() const { return GetSurface()->pitch; }                   
		void *pixels() { return GetSurface()->pixels; }
		const void *pixels() const { return GetSurface()->pixels; }
		struct private_hwdata *hwdata() const { return GetSurface()->hwdata; }
		const bool SetColorKey(const Uint32 flag, const Color key) const;
		const bool SetAlpha(const Uint32 flag, const Uint8 alpha) const;
		void ResetClipRect() const;
		void SetClipRect(const SDL_Rect& rect) const;
		void GetClipRect(SDL_Rect& rect) const;
		const int Blit(const DC& src) const;
		const int Blit(const DC& src, SDL_Rect& dstrect) const; 
		const int Blit(const DC& src, const Point& dstpoint, SDL_Rect& dstrect) const { dstrect.x = dstpoint.x; dstrect.y = dstpoint.y; return Blit(src, dstrect); };
		const int Blit(const DC& src, const Point& dstpoint) const;
		const int Blit(const DC& src, const SDL_Rect& srcrect, SDL_Rect& dstrect) const;
		const int Blit(const DC& src, const SDL_Rect& srcrect, const Point& dstpoint, SDL_Rect& dstrect) const { dstrect.x = dstpoint.x; dstrect.y = dstpoint.y; return Blit(src, srcrect, dstrect); };
		const int Blit(const DC& src, const SDL_Rect& srcrect, const Point& dstpoint) const;
		const bool Fill(const Color color) const;
		const bool FillRect(SDL_Rect& dstrect, const Color color) const;
		bool SaveBMP(const char *file) const;
		bool SaveBMP(const std::string& file) const { return SaveBMP(file.c_str()); }
		
		void SetBrush(const Brush& brush);
		void SetPen(const Pen& pen);
		void SetColor(Color* color);
		void SetFont(Font* font);
		void SetTextForeground(const SDL_Color& textColor);
		
		void GetTextExtent(const string& text, int* dx, int* dy) const { font->GetTextExtent(text,dx,dy);};
		void DrawText(const string& text, const Point& p) const {DrawText(text, p.x, p.y);};
		void DrawText(const string& text, const int x, const int y) const;
		void DrawBitmap(const Bitmap& bitmap, const Point& p) const {DrawBitmap(bitmap, p.x, p.y);};
		void DrawBitmap(const Bitmap& bitmap, const int x, const int y) const;

		void DrawEmptyRectangle(const int x1, const int y1, const int x2, const int y2) const;
		void DrawRectangle(const int x1, const int y1, const int x2, const int y2) const;
		void DrawRectangle(const Rect& rect) const { DrawRectangle(rect.x, rect.y, rect.width, rect.height);};
		void DrawRectangle(const Point& p, const Size& s) const { DrawRectangle(p.x, p.y, s.x, s.y);};
		void DrawSpline(const int c, const Point* p) const;
		void DrawLine(const Point& p1, const Point& p2) const {DrawLine(p1.x, p1.y, p2.x, p2.y);};
		void DrawLine(const int x1, const int y1, const int x2, const int y2) const;
		
		void DrawRoundedRectangle(const int x1, const int y1, const int x2, const int y2, const int radius) const;
		void DrawRoundedRectangle(const Point& p, const Size& s, const int radius) const {DrawRoundedRectangle(p.x, p.y, s.x, s.y, radius);};
		void DrawRoundedRectangle(const Rect& rect, const int radius) const {DrawRoundedRectangle(rect.x, rect.y, rect.width, rect.height,radius);};

		void FillRect( SDL_Surface& dest, SDL_Rect *rc, SDL_Color &clr ) const;
		explicit DC(SDL_Surface *sf): surface(sf) {};
		void DrawBresLine(const int x1, const int y1, const int x2, const int y2) const;
	protected:
		SDL_Surface* surface;
		void SetSurface(SDL_Surface *surface) {
			if(this->surface) SDL_FreeSurface(this->surface);
			this->surface = surface;
		}

		DC(const DC& other): surface(other.surface) {};
		DC &operator=(const DC& other) {if(this != &other) SetSurface(other.surface);return *this;};
	private:
		void line8(const int x1, const int y1, const int x2, const int y2) const;
		void line16(const int x1, const int y1, const int x2, const int y2) const;
		void line24(const int x1, const int y1, const int x2, const int y2) const;
		void line32(const int x1, const int y1, const int x2, const int y2) const;
		Brush brush;
		Pen pen;
		Color* color;
		SDL_Color textColor;
		Font* font;
};

#endif // _UI_SDLWRAPPER_HPP

SDL_Color toSDL_Color(const int r, const int g, const int b);

