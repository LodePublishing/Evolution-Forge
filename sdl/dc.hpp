#ifndef _SDL_DC_HPP
#define _SDL_DC_HPP

#include <SDL.h>

#include "rect.hpp"
#include "pen.hpp"
#include "brush.hpp"
#include "font.hpp"

#include <string>
#include <list>

enum eChooseDriverError
{
	NO_DRIVER_ERROR,
	NO_VIDEO_DRIVERS_AVAILIBLE,
	SDL_DRIVER_NOT_SUPPORTED
};

enum eBitDepth
{
	DEPTH_8BIT,
	DEPTH_16BIT,
	DEPTH_24BIT,
	DEPTH_32BIT
};

class DC
{
	public:
		DC();
		DC(const Size current_resolution, const eBitDepth bit_depth, const Uint32 nflags, const Uint32 initflags);
		~DC();
		
		DC(const DC& other);
		DC &operator=(const DC& other);

		operator SDL_Surface*() const;
		SDL_Surface* operator->() const;
		SDL_Surface* GetSurface() const;

		const unsigned int w() const;
		const unsigned int h() const;
		const bool valid() const;
		Uint32 flags() const;
		void setResolution(const Size current_resolution);
		void setBitDepth(const eBitDepth bit_depth);
		void setScreen(const Size current_resolution, const eBitDepth bit_depth, const Uint32 nflags);
		const eBitDepth getBitDepth() const;
	
		Uint16 pitch() const;
		void *pixels();
		const void *pixels() const;
		struct private_hwdata *hwdata() const;

		const bool Lock() const;
		void Unlock() const;

		
		const Color doColor(const Uint8 r, const Uint8 g, const Uint8 b) const;
		const Color mixColor(const Color& id1, const Color& id2) const;
		const Color mixColor(const Color& id1, const Color& id2, const unsigned int gradient) const;
		const Color changeAbsoluteBrightness(const Color& id, const signed int brightness) const;
		const Color changeRelativeBrightness(const Color& id, const unsigned int brightness_percent) const;

		const bool initializationOK() const;
		
		const bool SetColorKey(const Uint32 flag, const Color key) const;
		const bool SetAlpha(const Uint32 flag, const Uint8 alpha) const;
		void updateScreen();
		
		void Blit(SDL_Surface* src, SDL_Rect& dstrect) const;
		
		void clearScreen();
		
		bool SaveBMP(const std::string& file) const;

		void setFullscreen(const bool fullscreen = true);
		
		void SetBrush(const Brush& brush);
		void SetPen(const Pen& pen);
		void SetColor(const Color* color);
		void SetFont(const Font* font);

		const Font* GetFont() const;

		void SetTextForeground(const SDL_Color& textColor);

		const Size GetTextExtent(const std::string& text) const;
		
		void DrawText(const std::string& text, const Point& p) const;
		void DrawText(const std::string& text, const signed int x, const signed int y) const;
		
		void DrawBitmap(SDL_Surface* bitmap, const Point& p) const;
		void DrawBitmap(SDL_Surface* bitmap, const signed int x, const signed int y) const;

		void DrawEmptyRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height) const;
		void DrawEmptyRectangle(const Rect& rect) const;
		void DrawEmptyRectangle(const Point& p, const Size& s) const;
		
		void DrawRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height) const;
		void DrawRectangle(const Rect& rect) const; 
		void DrawRectangle(const Point& p, const Size& s) const;
		
		void DrawSpline(const unsigned int c, const Point* p) const;
		void DrawSpline(const unsigned int c, const Point* p, const Point s) const;

		void DrawLine(const Point& p1, const Point& p2) const;
		
		void DrawLine(const signed int x1, const signed int y1, const signed int x2, const signed int y2) const;

		void DrawVerticalLine(const signed int x0, const signed int y0, const signed int y1) const;
		void DrawHorizontalLine(const signed int x0, const signed int y0, const signed int x1) const;
		
		void DrawRoundedRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int radius) const;
		void DrawEdgedRoundedRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int radius) const;
		void DrawRoundedRectangle(const Point& p, const Size& s, const unsigned int radius) const;
		void DrawRoundedRectangle(const Rect& rect, const unsigned int radius) const;
		void DrawEdgedRoundedRectangle(const Point& p, const Size& s, const unsigned int radius) const;
		void DrawEdgedRoundedRectangle(const Rect& rect, const unsigned int radius) const;
		
		static SDL_Color toSDL_Color(const Uint8 r, const Uint8 g, const Uint8 b);
		
		static std::string printHardwareInformation();
		static std::string printSurfaceInformation(DC* surface);
		static std::list<std::string> getAvailibleDrivers();
		static SDL_Cursor* createCursor(char* xpm_image[]);
		static const eChooseDriverError chooseDriver(std::string& driver_name);

		void setPressedRectangle(const bool pressed = true);

		static void addRectangle(const Rect& rect);
	private:

		static unsigned int changedRectangles;
		static SDL_Rect changedRectangle[200];
		static Uint16 max_x, max_y;

		SDL_Surface* surface;
		void SetSurface(SDL_Surface* sdl_surface) {
			if(surface) 
				SDL_FreeSurface(surface);
			surface = sdl_surface;
		}

		bool pressedRectangle;

		bool initOK;
		Brush brush;
		Pen pen;
		const Color* color;
		SDL_Color textColor;
		const Font* font;
		
		eBitDepth bitDepth;
		Size resolution;

		void (DC::*Draw_HLine)(const signed int x0, const signed int y0, const signed int x1) const;
		void Draw_HLine_8bit(const signed int x0, const signed int y0, const signed int x1) const;
		void Draw_HLine_16bit(const signed int x0, const signed int y0, const signed int x1) const;
		void Draw_HLine_24bit(const signed int x0, const signed int y0, const signed int x1) const;
		void Draw_HLine_32bit(const signed int x0, const signed int y0, const signed int x1) const;


		void (DC::*Draw_VLine)(const signed int x0, const signed int y0, const signed int y1) const;
		void Draw_VLine_8bit(const signed int x0, const signed int y0, const signed int y1) const;
		void Draw_VLine_16bit(const signed int x0, const signed int y0, const signed int y1) const;
		void Draw_VLine_24bit(const signed int x0, const signed int y0, const signed int y1) const;
		void Draw_VLine_32bit(const signed int x0, const signed int y0, const signed int y1) const;


		void (DC::*Draw_Line)(signed int x1, signed int y1, signed int x2, signed int y2) const;
		void Draw_Line_8bit(signed int x1, signed int y1, signed int x2, signed int y2) const;
		void Draw_Line_16bit(signed int x1, signed int y1, signed int x2, signed int y2) const;
		void Draw_Line_24bit(signed int x1, signed int y1, signed int x2, signed int y2) const;
		void Draw_Line_32bit(signed int x1, signed int y1, signed int x2, signed int y2) const;
		
		void (DC::*DrawFilledRound)(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawFilledRound_8bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawFilledRound_16bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawFilledRound_24bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawFilledRound_32bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		
		void (DC::*DrawFilledEdgedRound)(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawFilledEdgedRound_8bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawFilledEdgedRound_16bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawFilledEdgedRound_24bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawFilledEdgedRound_32bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		
		void (DC::*DrawEmptyEdgedRound)(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawEmptyEdgedRound_8bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawEmptyEdgedRound_16bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawEmptyEdgedRound_24bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawEmptyEdgedRound_32bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		
		void (DC::*DrawEmptyRound)(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawEmptyRound_8bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawEmptyRound_16bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawEmptyRound_24bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawEmptyRound_32bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		
		void (DC::*DrawFilledEdgedBorderRound)(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawFilledEdgedBorderRound_8bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawFilledEdgedBorderRound_16bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawFilledEdgedBorderRound_24bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
		void DrawFilledEdgedBorderRound_32bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const;
};

inline void DC::setPressedRectangle(const bool pressed) {
	pressedRectangle = pressed;
}

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
		
inline bool DC::SaveBMP(const std::string& file) const {
	return SaveBMP(file.c_str()); 
}

inline void DC::DrawLine(const Point& p1, const Point& p2) const {
	DrawLine(p1.x, p1.y, p2.x, p2.y);
}

inline void DC::DrawText(const std::string& text, const Point& p) const {
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

inline void DC::SetColor(const Color* dc_color) {
	color = dc_color;
}

inline void DC::SetFont(const Font* dc_font) {
	font = dc_font;
}

inline void DC::SetTextForeground(const SDL_Color& dc_text_color) {
	textColor = dc_text_color;
}

inline const Font* DC::GetFont() const {
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

inline const bool DC::SetColorKey(const Uint32 flag, const Color key) const {
	return SDL_SetColorKey(surface, flag, key) == 0;
}

inline const bool DC::SetAlpha(const Uint32 flag, const Uint8 alpha) const {
	return SDL_SetAlpha(surface, flag, alpha) == 0;
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

inline const eBitDepth DC::getBitDepth() const {
	return(bitDepth);
}

inline const bool DC::initializationOK() const {
	return(initOK);
}

inline void DC::clearScreen() {
	SDL_Rect rc;
	rc.x=0;rc.y=0;rc.w=max_x;rc.h=max_y;
	SDL_FillRect(surface, &rc, 0);
}

inline void DC::Blit(SDL_Surface* src, SDL_Rect& dstrect) const {
	SDL_BlitSurface(src, NULL, surface, &dstrect);
}

#endif // _SDL_DC_HPP


