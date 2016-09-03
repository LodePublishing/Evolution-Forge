#ifndef _SDL_DC_HPP
#define _SDL_DC_HPP

#include <SDL.h>

#include "rect.hpp"
#include "pen.hpp"
#include "brush.hpp"
#include "font.hpp"
#include <string>

#define SDL_DRAW_BPP 4


class DC
{
	public:
		DC();
		DC(const unsigned int width, const unsigned int height, const unsigned int bitdepth, Uint32 flags, Uint32 initflags);
		~DC() 
		{ }
		
		DC(const DC& other);
		DC &operator=(const DC& other);

		operator SDL_Surface*() const;
		SDL_Surface* operator->() const;
		SDL_Surface* GetSurface() const;
		
		const bool Lock() const;
		void Unlock() const;

		const Color doColor(const Uint8 r, const Uint8 g, const Uint8 b) const;
		
		const Color mixColor(const Color* id1, const Color* id2) const;
		const Color mixColor(const Color* id1, const Color* id2, const unsigned int gradient) const;
		const Color brightenColor(const Color* id, const unsigned int brightness) const;
		const Color darkenColor(const Color* id, const unsigned int brightness) const;

		const bool valid() const;
		Uint32 flags() const;
		const unsigned int w() const;
		const unsigned int h() const;
		Uint16 pitch() const;
		void *pixels();
		const void *pixels() const;
		struct private_hwdata *hwdata() const;

		const bool initializationOK() const;
		
		const bool SetColorKey(const Uint32 flag, const Color key) const;
		const bool SetAlpha(const Uint32 flag, const Uint8 alpha) const;
		void updateScreen() const;
		void ResetClipRect() const;
		void SetClipRect(const SDL_Rect& rect) const;
		void GetClipRect(SDL_Rect& rect) const;
		
		const int Blit(const DC& src) const;
		const int Blit(const DC& src, SDL_Rect& dstrect) const; 
		const int Blit(const DC& src, const Point& dstpoint, SDL_Rect& dstrect) const;
		const int Blit(const DC& src, const Point& dstpoint) const;
		const int Blit(const DC& src, const SDL_Rect& srcrect, SDL_Rect& dstrect) const;
		const int Blit(const DC& src, const SDL_Rect& srcrect, const Point& dstpoint, SDL_Rect& dstrect) const;
		const int Blit(const DC& src, const SDL_Rect& srcrect, const Point& dstpoint) const;
		
		const bool Fill(const Color color) const;
		const bool FillRect(SDL_Rect& dstrect, const Color color) const;
		
		bool SaveBMP(const std::string& file) const;

		void setFullscreen(const bool fullscreen = true);
		
		void SetBrush(const Brush& brush);
		void SetPen(const Pen& pen);
		void SetColor(Color* color);
		void SetFont(Font* font);

		Font* GetFont() const;

		void SetTextForeground(const SDL_Color& textColor);
		
		const Size GetTextExtent(const std::string& text) const;
		
		void DrawText(const std::string& text, const Point& p) const;
		void DrawTextA(const std::string& text, const Point& p) const;
		void DrawTextA(const std::string& text, const int x, const int y) const;
		void DrawText(const std::string& text, const signed int x, const signed int y) const;
		
		void DrawBitmap(SDL_Surface* bitmap, const Point& p) const;
		void DrawBitmap(SDL_Surface* bitmap, const signed int x, const signed int y) const;

		void DrawEmptyRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height) const;
		void DrawEmptyRectangle(const Rect& rect) const;
		void DrawEmptyRectangle(const Point& p, const Size& s) const;
		
		void DrawRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height) const;
		void DrawRectangle(const Rect& rect) const; 
		void DrawRectangle(const Point& p, const Size& s) const;
		
		void DrawEmptyRound(const Rect& rect, const unsigned int corner) const;
	
		void DrawSpline(const unsigned int c, const Point* p) const;
		void DrawSpline(const unsigned int c, const Point* p, const Point s) const;

		void DrawLine(const Point& p1, const Point& p2) const;
		
		void Draw_Line(signed int x1, signed int y1, signed int x2, signed int y2) const;
		void DrawLine(const signed int x1, const signed int y1, const signed int x2, const signed int y2) const;
//		void aalineColorInt(const int x1, const int y1, const int x2, const int y2) const;

		void DrawVerticalLine(const signed int x0, const signed int y0, const signed int y1) const;
		void DrawHorizontalLine(const signed int x0, const signed int y0, const signed int x1) const;
		
//		void DrawGridEdgedRoundedRectangle(const signed int x, const signed y, const unsigned width, const unsigned int height, const unsigned int radius, std::list<Rect> notDrawRectList) const;
		void DrawRoundedRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int radius) const;
		void DrawEdgedRoundedRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int radius) const;
//		void DrawGridEdgedRoundedRectangle(const Point p, const Size& s, const unsigned int radius, std::list<Rect> &notDrawRectList) const {
//			DrawRoundedRectangle(p.x, p.y, s.GetWidth(), s.GetHeight(), radius);
//		}
		void DrawRoundedRectangle(const Point& p, const Size& s, const unsigned int radius) const;
		void DrawRoundedRectangle(const Rect& rect, const unsigned int radius) const;
		void DrawEdgedRoundedRectangle(const Point& p, const Size& s, const unsigned int radius) const;
		void DrawEdgedRoundedRectangle(const Rect& rect, const unsigned int radius) const;
		
		void FillRect( SDL_Surface& dest, SDL_Rect *rc, SDL_Color &clr ) const;
		void setResolution(const unsigned int max_x, const unsigned int max_y);
		static SDL_Color toSDL_Color(const Uint8 r, const Uint8 g, const Uint8 b);
//		void printInformation() const;
		
	protected:
		SDL_Surface* surface;
		void SetSurface(SDL_Surface *sdl_surface) {
			if(surface) 
				SDL_FreeSurface(surface);
			surface = sdl_surface;
		}

	private:
		bool initOK;
		Uint16 max_x, max_y;
		Brush brush;
		Pen pen;
		Color* color;
		SDL_Color textColor;
		Font* font;

		void Draw_VLine(const signed int x0, const signed int y0, const signed int y1) const;

		void Draw_HLine(const signed int x0, const signed int y0, const signed int x1) const;
		
//		void Draw_HLine_8bit(const signed int x0, const signed int y0, const signed int x1) const;
//		void Draw_HLine_16bit(const signed int x0, const signed int y0, const signed int x1) const;
//		void Draw_HLine_24bit(const signed int x0, const signed int y0, const signed int x1) const;
//		void Draw_HLine_32bit(const signed int x0, const signed int y0, const signed int x1) const;

		void DrawFilledRound(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawFilledEdgedRound(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawEmptyEdgedRound(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawEmptyRound(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawFilledEdgedBorderRound(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;

};

inline DC& DC::operator=(const DC& other) {
	if(this != &other) SetSurface(other.surface);return *this;
}

inline DC::operator SDL_Surface*() const { 
	return surface;
}
		
inline SDL_Surface* DC::operator->() const {
	return surface;
}
		
inline SDL_Surface* DC::GetSurface() const {
	return(surface);
}
		
inline const bool DC::valid() const {
	return (surface != NULL);
}
		
inline Uint32 DC::flags() const {
	return GetSurface()->flags;
}
		
inline const unsigned int DC::w() const {
	return GetSurface()->w;
}
		
inline const unsigned int DC::h() const {
	return GetSurface()->h;
}
		
inline Uint16 DC::pitch() const {
	return GetSurface()->pitch;
}
		
inline void* DC::pixels() { 
	return GetSurface()->pixels; 
}
		
inline const void* DC::pixels() const {
	return GetSurface()->pixels;
}
		
inline struct private_hwdata* DC::hwdata() const {
	return GetSurface()->hwdata;
}
		
inline const int DC::Blit(const DC& src, const Point& dstpoint, SDL_Rect& dstrect) const { 
	dstrect.x = dstpoint.x; dstrect.y = dstpoint.y; return Blit(src, dstrect); 
}
		
inline const int DC::Blit(const DC& src, const SDL_Rect& srcrect, const Point& dstpoint, SDL_Rect& dstrect) const { 
	dstrect.x = dstpoint.x; dstrect.y = dstpoint.y; return Blit(src, srcrect, dstrect); 
}
		
inline bool DC::SaveBMP(const std::string& file) const {
	return SaveBMP(file.c_str()); 
}

inline void DC::DrawLine(const Point& p1, const Point& p2) const {
	DrawLine(p1.x, p1.y, p2.x, p2.y);
}

inline void DC::DrawText(const std::string& text, const Point& p) const {
	DrawText(text, p.x, p.y);
}

inline void DC::DrawText(const std::string& text, const signed int x, const signed int y) const {
	font->DrawText(surface, textColor, text, x, y);
}

inline void DC::DrawTextA(const std::string& text, const int x, const int y) const {
	DrawText(text, x, y);
}

inline void DC::DrawTextA(const std::string& text, const Point& p) const {
	DrawText(text, p.x, p.y);
}

inline void DC::DrawRoundedRectangle(const Point& p, const Size& s, const unsigned int radius) const {
	DrawRoundedRectangle(p.x, p.y, s.GetWidth(), s.GetHeight(), radius);
}
			
inline void DC::DrawRoundedRectangle(const Rect& rect, const unsigned int radius) const {
	DrawRoundedRectangle(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), radius);
}

inline void DC::DrawEdgedRoundedRectangle(const Point& p, const Size& s, const unsigned int radius) const {
	DrawEdgedRoundedRectangle(p.x, p.y, s.GetWidth(), s.GetHeight(), radius);
}
		
inline void DC::DrawEdgedRoundedRectangle(const Rect& rect, const unsigned int radius) const {
	DrawEdgedRoundedRectangle(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), radius);
}

inline void DC::SetBrush(const Brush& dc_brush) {
	brush = dc_brush;
}

inline void DC::SetPen(const Pen& dc_pen) {
	pen = dc_pen;
}

inline void DC::SetColor(Color* dc_color) {
	color = dc_color;
}

inline void DC::SetFont(Font* dc_font) {
	font = dc_font;
}

inline void DC::SetTextForeground(const SDL_Color& dc_text_color) {
	textColor = dc_text_color;
}

inline Font* DC::GetFont() const {
	return(font);
}

inline const Size DC::GetTextExtent(const std::string& text) const {
	return(font->GetTextExtent(text));
}
		
inline void DC::DrawBitmap(SDL_Surface* bitmap, const Point& p) const {
	DrawBitmap(bitmap, p.x, p.y);
}
		
inline void DC::DrawEmptyRectangle(const Rect& rect) const	{
	DrawEmptyRectangle(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight());
}
		
inline void DC::DrawEmptyRectangle(const Point& p, const Size& s) const {
	DrawEmptyRectangle(p.x, p.y, s.GetWidth(), s.GetHeight());
}
		
inline void DC::DrawRectangle(const Rect& rect) const { 
	DrawRectangle(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight());
}
		
inline void DC::DrawRectangle(const Point& p, const Size& s) const { 
	DrawRectangle(p.x, p.y, s.GetWidth(), s.GetHeight());
}

inline void DC::DrawEmptyRound(const Rect& rect, const unsigned int corner) const {
	DrawEmptyRound(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), corner);
}	

inline const bool DC::SetColorKey(const Uint32 flag, const Color key) const {
	return SDL_SetColorKey(surface, flag, key) == 0;
}

inline const bool DC::SetAlpha(const Uint32 flag, const Uint8 alpha) const {
	return SDL_SetAlpha(surface, flag, alpha) == 0;
}

inline void DC::SetClipRect(const SDL_Rect& rect) const {
	SDL_SetClipRect(surface, const_cast<SDL_Rect*>(&rect));
}

inline void DC::ResetClipRect() const {
	SDL_SetClipRect(surface, 0);
}

inline void DC::GetClipRect(SDL_Rect& rect) const {
	SDL_GetClipRect(surface, &rect);
}

inline const Color DC::doColor(const Uint8 r, const Uint8 g, const Uint8 b) const {
	return(Color(surface, r, g, b));
}

inline SDL_Color DC::toSDL_Color(const Uint8 r, const Uint8 g, const Uint8 b) {
	SDL_Color c;
	c.r=r;c.g=g;c.b=b;
	c.unused=255;
	return(c);	
}


#endif // _SDL_DC_HPP


