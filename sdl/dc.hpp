#ifndef _SDL_DC_HPP
#define _SDL_DC_HPP

#include "rect.hpp"
#include "pen.hpp"
#include "brush.hpp"
#include "font.hpp"

#include <list>

#define PRESSED_BRIGHTEN 50
#define PRESSED_DARKEN 130
#define PRESSED_NORMAL 90
#define NOT_PRESSED_DARKEN 60
#define NOT_PRESSED_BRIGHTEN 140
 
#include <math.h>
#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif
// Windows kotz

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

typedef struct tColorRGBA {
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
} tColorRGBA;

typedef struct tColorY {
	Uint8 y;
} tColorY;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define SDL_rmask 0xff000000
#define SDL_gmask 0x00ff0000
#define SDL_bmask 0x0000ff00
#define SDL_amask 0x00000000
#else
#define SDL_rmask 0x000000ff
#define SDL_gmask 0x0000ff00
#define SDL_bmask 0x00ff0000
#define SDL_amask 0x00000000
#endif


class DC
{
	public:
		DC();
		DC(const Size current_resolution, const eBitDepth bit_depth, const Uint32 nflags, const Uint32 initflags);
		~DC();
		
		operator SDL_Surface*() const;
		SDL_Surface* operator->() const;
		SDL_Surface* getSurface() const;

		const bool isSurfacePuffered() const;

		const unsigned int w() const;
		const unsigned int h() const;
		const bool valid() const;
		Uint32 flags() const;
		void setResolution(const Size current_resolution);
		void setBitDepth(const eBitDepth bit_depth);
		void setScreen(const Size current_resolution, const eBitDepth bit_depth, const Uint32 nflags);
		const eBitDepth getBitDepth() const;
		const unsigned int getBits() const;
	
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
		
		const bool setColorKey(const Uint32 flag, const Color key) const;
		const bool setAlpha(const Uint32 flag, const Uint8 alpha) const;
		void updateScreen();
		
		void Blit(SDL_Surface* src, SDL_Rect& dstrect) const;
		
		void clearScreen();
		
		bool SaveBMP(const std::string& file) const;

		void setFullscreen(const bool fullscreen = true);
		
		void setBrush(const Brush& brush);
		void setPen(const Pen& pen);
		void setColor(const Color* color);
		void setFont(const Font* font);

		const Font* getFont() const;

		void setTextForeground(const SDL_Color& textColor);

		const Size getTextExtent(const std::string& text) const;
		
		void DrawText(const std::string& text, const Point& p) const;
		
		void DrawBrightenedBitmap(SDL_Surface* bitmap, const Point& p, const unsigned int brightness) const;

		void DrawBrightenedBitmap(SDL_Surface* bitmap, const Point& p, const Rect& clip_rect, const unsigned int brightness) const;
	
		void DrawBitmap(SDL_Surface* bitmap, const Point& p) const;
		
		void DrawBitmap(SDL_Surface* bitmap, const Point& p, const Rect& clip_rect) const;

		void DrawEmptyRectangle(const Rect& rect) const;
		void DrawEmptyRectangle(const Point& p, const Size& s) const;
		
		void DrawRectangle(const Rect& rect) const; 
		void DrawRectangle(const Point& p, const Size& s) const;

		void DrawTabRectangle(const Rect& rect) const; 
		void DrawTabRectangle(const Point& p, const Size& s) const;


		void DrawSpline(const unsigned int c, const Point* p) const;
		void DrawSpline(const unsigned int c, const Point* p, const Point s) const;

		void DrawLine(const Point& p1, const Point& p2) const;
		
		void DrawVerticalLine(const Point& p, const unsigned int height) const;
		void DrawHorizontalLine(const Point& p, const unsigned int width) const;

		
		void DrawRoundedRectangle(const Point& p, const Size& s, const unsigned int radius) const;
		void DrawRoundedRectangle(const Rect& rect, const unsigned int radius) const;
		void DrawEdgedRoundedRectangle(const Point& p, const Size& s, const unsigned int radius) const;
		void DrawEdgedRoundedRectangle(const Rect& rect, const unsigned int radius) const;
		
		static SDL_Color toSDL_Color(const Uint8 r, const Uint8 g, const Uint8 b);
		
		static std::list<std::string> getAvailibleDrivers();
		static SDL_Cursor* createCursor(char* xpm_image[]);
		static const eChooseDriverError chooseDriver(std::list<std::string>& parameter_list, std::string& current_driver);

		void setPressedRectangle(const bool pressed = true);

		static void addRectangle(const Rect& rect);


		static SDL_Surface* rotozoomSurface(SDL_Surface* src, double angle, double zoom, int smooth);
		static SDL_Surface* rotozoomSurfaceXY(SDL_Surface * src, double angle, double zoomx, double zoomy, int smooth);
		static void rotozoomSurfaceSize(int width, int height, double angle, double zoom, int* dstwidth, int* dstheight);
		static void rotozoomSurfaceSizeXY(int width, int height, double angle, double zoomx, double zoomy, int* dstwidth, int* dstheight);
		static SDL_Surface* zoomSurface(SDL_Surface* src, double zoomx, double zoomy, int smooth);
//		static void zoomSurfaceSize(int width, int height, double zoomx, double zoomy, int* dstwidth, int* dstheight);
		

		void switchToSurface(SDL_Surface* temp_surface);
		void switchToOriginalSurface();

		void drawFromPoint(const Point p);
	private:

		static unsigned int changedRectangles;
		static SDL_Rect changedRectangle[200];
		static Uint16 max_x, max_y;

		SDL_Surface* surface;
		SDL_Surface* oldSurface;
		void setSurface(SDL_Surface* sdl_surface) {
			if(surface) 
				SDL_FreeSurface(surface);
			surface = sdl_surface;
		}

		bool pressedRectangle;


		Point dp;
		
		bool initOK;
		Brush brush;
		Pen pen;
		const Color* color;
		SDL_Color textColor;
		const Font* font;
		
		eBitDepth bitDepth;
		unsigned int bits;
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
	
		void (DC::*DrawTab)(const signed int x, const signed int y, const unsigned int width, const unsigned int height) const;
		void DrawTab_8bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height) const;
		void DrawTab_16bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height) const;
		void DrawTab_24bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height) const;
		void DrawTab_32bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height) const;
		
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

		void DrawBrightenedBitmap(SDL_Surface* bitmap, const signed x, const signed y, const unsigned int brightness) const;
		void DrawText(const std::string& text, const signed int x, const signed int y) const;
		void DrawBrightenedBitmap(SDL_Surface* bitmap, const signed x, const signed y, const Rect& clip_rect, const unsigned int brightness) const;
		void DrawBitmap(SDL_Surface* bitmap, const signed int x, const signed int y) const;
		void DrawBitmap(SDL_Surface* bitmap, const signed int x, const signed int y, const Rect& clip_rect) const;
		void DrawEmptyRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height) const;
		void DrawRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height) const;
		void DrawTabRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height) const;
		void DrawLine(const signed int x1, const signed int y1, const signed int x2, const signed int y2) const;
		void DrawVerticalLine(const signed int x0, const signed int y0, const signed int y1) const;
		void DrawHorizontalLine(const signed int x0, const signed int y0, const signed int x1) const;
		void DrawRoundedRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int radius) const;
		void DrawEdgedRoundedRectangle(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int radius) const;

		DC(const DC& other);
		DC &operator=(const DC& other);
};

inline void DC::drawFromPoint(const Point p) {
	dp = p;
}

inline void DC::setPressedRectangle(const bool pressed) {
	pressedRectangle = pressed;
}

inline DC& DC::operator=(const DC& other) {
	if(this != &other) setSurface(other.surface);return *this;
}

inline DC::operator SDL_Surface*() const { 
	return surface;
}
		
inline SDL_Surface* DC::operator->() const {
	return surface;
}
		
inline SDL_Surface* DC::getSurface() const {
	return(surface);
}
		
inline const bool DC::valid() const {
	return (surface != NULL);
}
		
inline Uint32 DC::flags() const {
	return getSurface()->flags;
}
		
inline const unsigned int DC::w() const {
	return getSurface()->w;
}
		
inline const unsigned int DC::h() const {
	return getSurface()->h;
}
		
inline Uint16 DC::pitch() const {
	return getSurface()->pitch;
}
		
inline void* DC::pixels() { 
	return getSurface()->pixels; 
}
		
inline const void* DC::pixels() const {
	return getSurface()->pixels;
}
		
inline struct private_hwdata* DC::hwdata() const {
	return getSurface()->hwdata;
}
		
inline bool DC::SaveBMP(const std::string& file) const {
	return SaveBMP(file.c_str()); 
}

inline void DC::DrawLine(const Point& p1, const Point& p2) const {
	DrawLine(dp.x + p1.x, dp.y + p1.y, dp.x + p2.x, dp.y + p2.y);
}

inline void DC::DrawText(const std::string& text, const Point& p) const {
	DrawText(text, dp.x + p.x, dp.y + p.y);
}

inline void DC::DrawRoundedRectangle(const Point& p, const Size& s, const unsigned int radius) const {
	DrawRoundedRectangle(dp.x + p.x, dp.y + p.y, s.getWidth(), s.getHeight(), radius);
}
			
inline void DC::DrawRoundedRectangle(const Rect& rect, const unsigned int radius) const {
	DrawRoundedRectangle(dp.x + rect.getLeft(), dp.y + rect.getTop(), rect.getWidth(), rect.getHeight(), radius);
}

inline void DC::DrawEdgedRoundedRectangle(const Point& p, const Size& s, const unsigned int radius) const {
	DrawEdgedRoundedRectangle(dp.x + p.x, dp.y + p.y, s.getWidth(), s.getHeight(), radius);
}
		
inline void DC::DrawEdgedRoundedRectangle(const Rect& rect, const unsigned int radius) const {
	DrawEdgedRoundedRectangle(dp.x + rect.getLeft(), dp.y + rect.getTop(), rect.getWidth(), rect.getHeight(), radius);
}

inline void DC::setTextForeground(const SDL_Color& dc_text_color) {
	textColor = dc_text_color;
}

inline const Font* DC::getFont() const {
	return(font);
}

inline const Size DC::getTextExtent(const std::string& text) const {
	return(font->getTextExtent(text));
}
		
inline void DC::DrawBrightenedBitmap(SDL_Surface* bitmap, const Point& p, const unsigned int brightness) const {
	DrawBrightenedBitmap(bitmap, dp.x + p.x, dp.y + p.y, brightness);
}

inline void DC::DrawBrightenedBitmap(SDL_Surface* bitmap, const Point& p, const Rect& clip_rect, const unsigned int brightness) const {
	DrawBrightenedBitmap(bitmap, dp.x + p.x, dp.y + p.y, clip_rect, brightness);
}

inline void DC::DrawBitmap(SDL_Surface* bitmap, const Point& p) const {
	DrawBitmap(bitmap, dp.x + p.x, dp.y + p.y);
}

inline void DC::DrawBitmap(SDL_Surface* bitmap, const Point& p, const Rect& clip_rect) const {
	DrawBitmap(bitmap, dp.x + p.x, dp.y + p.y, clip_rect);
}	

inline void DC::DrawEmptyRectangle(const Rect& rect) const	{
	DrawEmptyRectangle(dp.x + rect.getLeft(), dp.y + rect.getTop(), rect.getWidth(), rect.getHeight());
}
		
inline void DC::DrawEmptyRectangle(const Point& p, const Size& s) const {
	DrawEmptyRectangle(dp.x + p.x, dp.y + p.y, s.getWidth(), s.getHeight());
}
		
inline void DC::DrawRectangle(const Rect& rect) const { 
	DrawRectangle(dp.x + rect.getLeft(), dp.y + rect.getTop(), rect.getWidth(), rect.getHeight());
}
		
inline void DC::DrawRectangle(const Point& p, const Size& s) const { 
	DrawRectangle(dp.x + p.x, dp.y + p.y, s.getWidth(), s.getHeight());
}

inline void DC::DrawTabRectangle(const Rect& rect) const { 
	DrawTabRectangle(dp.x + rect.getLeft(), dp.y + rect.getTop(), rect.getWidth(), rect.getHeight());
}
		
inline void DC::DrawTabRectangle(const Point& p, const Size& s) const { 
	DrawTabRectangle(dp.x + p.x, dp.y + p.y, s.getWidth(), s.getHeight());
}

inline const bool DC::setColorKey(const Uint32 flag, const Color key) const {
	return SDL_SetColorKey(surface, flag, key) == 0;
}

inline const bool DC::setAlpha(const Uint32 flag, const Uint8 alpha) const {
	return SDL_SetAlpha(surface, flag, alpha) == 0;
}

inline void DC::setBrush(const Brush& dc_brush) {
	brush = dc_brush;
}

inline void DC::setPen(const Pen& dc_pen) {
	pen = dc_pen;
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

inline const unsigned int DC::getBits() const {
	return(bits);
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
	SDL_FillRect(surface, &rc, 1);
}

inline void DC::Blit(SDL_Surface* src, SDL_Rect& dstrect) const {
	SDL_BlitSurface(src, NULL, surface, &dstrect);
}

inline void DC::DrawVerticalLine(const Point& p, const unsigned int height) const {
	DrawVerticalLine(dp.x + p.x, dp.y + p.y, dp.y + p.y + height);
}

inline void DC::DrawHorizontalLine(const Point& p, const unsigned int width) const {
	DrawHorizontalLine(dp.x + p.x, dp.y + p.y, dp.x + p.x + width);
}



#endif // _SDL_DC_HPP


