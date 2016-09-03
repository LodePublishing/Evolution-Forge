#ifndef _SDL_DC_HPP
#define _SDL_DC_HPP

#include <SDL.h>

#include "rect.hpp"
#include "pen.hpp"
#include "brush.hpp"
#include "font.hpp"
#include "bitmap.hpp"
//#include <SDL/SDL_gfxPrimitives.h>
#include <string>


class DC
{
	public:
		DC();
		DC(const unsigned int width, const unsigned int height, const unsigned int bitdepth, Uint32 flags);
		~DC() 
		{ }
		
		DC(const DC& other);
		DC &operator=(const DC& other) {
			if(this != &other) SetSurface(other.surface);return *this;
		}

        operator SDL_Surface*() const { 
			return surface;
		}
		
        SDL_Surface* operator->() const {
			return surface;
		}
		
		SDL_Surface* GetSurface() const {
			return(surface);
		}
		
		const bool Lock() const;
		void Unlock() const;

		const Color doColor(const Uint8 r, const Uint8 g, const Uint8 b) const;
		
		const Color mixColor(const Color* id1, const Color* id2) const;
		const Color mixColor(const Color* id1, const Color* id2, const unsigned int gradient) const;
		const Color brightenColor(const Color* id, const unsigned int brightness) const;
		const Color darkenColor(const Color* id, const unsigned int brightness) const;

		const bool valid() const {
			return (surface!=0);
		}
		
		Uint32 flags() const {
			return GetSurface()->flags;
		}
		
		const unsigned int w() const {
			return GetSurface()->w;
		}
		
		const unsigned int h() const {
			return GetSurface()->h;
		}
		
		Uint16 pitch() const {
			return GetSurface()->pitch;
		}
		
		void *pixels() { 
			return GetSurface()->pixels; 
		}
		
		const void *pixels() const {
			return GetSurface()->pixels;
		}
		
		struct private_hwdata *hwdata() const {
			return GetSurface()->hwdata;
		}
		
		const bool SetColorKey(const Uint32 flag, const Color key) const;
		const bool SetAlpha(const Uint32 flag, const Uint8 alpha) const;
		void updateScreen() const;
		void ResetClipRect() const;
		void SetClipRect(const SDL_Rect& rect) const;
		void GetClipRect(SDL_Rect& rect) const;
		const int Blit(const DC& src) const;
		const int Blit(const DC& src, SDL_Rect& dstrect) const; 
		
		const int Blit(const DC& src, const Point& dstpoint, SDL_Rect& dstrect) const { 
			dstrect.x = dstpoint.x; dstrect.y = dstpoint.y; return Blit(src, dstrect); 
		}
		
		const int Blit(const DC& src, const Point& dstpoint) const;
		const int Blit(const DC& src, const SDL_Rect& srcrect, SDL_Rect& dstrect) const;
		
		const int Blit(const DC& src, const SDL_Rect& srcrect, const Point& dstpoint, SDL_Rect& dstrect) const { 
			dstrect.x = dstpoint.x; dstrect.y = dstpoint.y; return Blit(src, srcrect, dstrect); 
		}
		
		const int Blit(const DC& src, const SDL_Rect& srcrect, const Point& dstpoint) const;
		const bool Fill(const Color color) const;
		const bool FillRect(SDL_Rect& dstrect, const Color color) const;
		
		bool SaveBMP(const std::string& file) const {
			return SaveBMP(file.c_str()); 
		}

		void setFullscreen(const bool fullscreen = true);
		
		void SetBrush(const Brush& brush);
		void SetPen(const Pen& pen);
		void SetColor(Color* color);
		void SetFont(Font* font);

		Font* GetFont() const;

		void SetTextForeground(const SDL_Color& textColor);
		
		const Size GetTextExtent(const std::string& text) const {
			return(font->GetTextExtent(text));
		}
		
		void DrawText(const std::string& text, const Point& p) const;
		void DrawTextA(const std::string& text, const Point& p) const;
		void DrawTextA(const std::string& text, const int x, const int y) const;
		
		void DrawText(const std::string& text, const signed int x, const signed int y) const;
		void DrawBitmap(const Bitmap& bitmap, const Point& p) const {
			DrawBitmap(bitmap, p.x, p.y);
		}
		
		void DrawBitmap(const Bitmap& bitmap, const signed int x, const signed int y) const;

		void DrawEmptyRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height) const;
		
		void DrawEmptyRectangle(const Rect& rect) const	{
			DrawEmptyRectangle(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight());
		}
		
		void DrawEmptyRectangle(const Point& p, const Size& s) const {
			DrawEmptyRectangle(p.x, p.y, s.GetWidth(), s.GetHeight());
		}
		
		void DrawRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height) const;
		
		void DrawRectangle(const Rect& rect) const { 
			DrawRectangle(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight());
		}
		
		void DrawRectangle(const Point& p, const Size& s) const { 
			DrawRectangle(p.x, p.y, s.GetWidth(), s.GetHeight());
		}

		void DrawEmptyRound(const Rect& rect, const unsigned int corner) const {
			DrawEmptyRound(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), corner);
		}
	
		void DrawSpline(const unsigned int c, const Point* p) const;
		void DrawSpline(const unsigned int c, const Point* p, const Point s) const;

		void DrawLine(const Point& p1, const Point& p2) const {
			DrawLine(p1.x, p1.y, p2.x, p2.y);
		}
		
		void Draw_Line(signed int x1, signed int y1, signed int x2, signed int y2) const;
		void DrawLine(const signed int x1, const signed int y1, const signed int x2, const signed int y2) const;
//		void aalineColorInt(const int x1, const int y1, const int x2, const int y2) const;

		
		void DrawVerticalLine(const signed int x0, const signed int y0, const signed int y1) const;
		void DrawHorizontalLine(const signed int x0, const signed int y0, const signed int x1) const;
		
		void DrawGridEdgedRoundedRectangle(const signed int x, const signed y, const unsigned width, const unsigned int height, const unsigned int radius, std::list<Rect> notDrawRectList) const;
		void DrawRoundedRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int radius) const;
		void DrawEdgedRoundedRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int radius) const;
		
//		void DrawGridEdgedRoundedRectangle(const Point p, const Size& s, const unsigned int radius, std::list<Rect> &notDrawRectList) const {
//			DrawRoundedRectangle(p.x, p.y, s.GetWidth(), s.GetHeight(), radius);
//		}

		void DrawRoundedRectangle(const Point& p, const Size& s, const unsigned int radius) const {
			DrawRoundedRectangle(p.x, p.y, s.GetWidth(), s.GetHeight(), radius);
		}
			
		void DrawRoundedRectangle(const Rect& rect, const unsigned int radius) const {
			DrawRoundedRectangle(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), radius);
		}

		void DrawEdgedRoundedRectangle(const Point& p, const Size& s, const unsigned int radius) const {
			DrawEdgedRoundedRectangle(p.x, p.y, s.GetWidth(), s.GetHeight(), radius);
		}
		
		void DrawEdgedRoundedRectangle(const Rect& rect, const unsigned int radius) const {
			DrawEdgedRoundedRectangle(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), radius);
		}
		
		void FillRect( SDL_Surface& dest, SDL_Rect *rc, SDL_Color &clr ) const;
		void setResolution(const unsigned int max_x, const unsigned int max_y);
		
	protected:
		SDL_Surface* surface;
		void SetSurface(SDL_Surface *sdl_surface) {
			if(surface) 
				SDL_FreeSurface(surface);
			surface = sdl_surface;
		}

	private:
		Uint16 max_x, max_y;
		Brush brush;
		Pen pen;
		Color* color;
		SDL_Color textColor;
		Font* font;

		void Draw_VLine(const signed int x0, const signed int y0, const signed int y1) const;
		void Draw_HLine(const signed int x0, const signed int y0, const signed int x1) const;

		void DrawFilledRound(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawFilledEdgedRound(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawEmptyEdgedRound(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawEmptyRound(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;

};

SDL_Color toSDL_Color(const Uint8 r, const Uint8 g, const Uint8 b);

#endif // _SDL_DC_HPP


