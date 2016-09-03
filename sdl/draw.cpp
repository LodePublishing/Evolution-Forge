#include "dc.hpp"

#include "string.h"
#include "wchar.h"

// for 24bit:
#define SDL_DRAW_PUTPIXEL_BPP_3_AUX \
	if (SDL_BYTEORDER == SDL_BIG_ENDIAN) { \
		p[0] = colorbyte2;					 \
		p[1] = colorbyte1;					 \
		p[2] = colorbyte0;					 \
	} else {								 \
		p[0] = colorbyte0;					 \
		p[1] = colorbyte1;					 \
		p[2] = colorbyte2;					 \
	}

#define SDL_DO_DRAWING_X switch( k % 4 ) {do{case 0: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=3;case 3: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=3;case 2: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=3;case 1: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=3;}while( (k-=4) > 0 );}
#define SDL_DO_DRAWING_Y switch( k % 4 ) {do{case 0: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=surface->pitch;case 3: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=surface->pitch;case 2: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=surface->pitch;case 1: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=surface->pitch;}while( (k-=4) > 0 );}

#ifdef __linux__
#define SDL_DRAW_WMEMSET_START \
	if (sizeof(wchar_t) == sizeof(Uint32)) {		\
		wmemset((wchar_t*)p0, col, dx);			 \
		wmemset((wchar_t*)p1, col, dx);			 \
	} else {
#define SDL_DRAW_WMEMSET_END }
#else
#define SDL_DRAW_WMEMSET_START
#define SDL_DRAW_WMEMSET_END
#endif

void DC::Draw_VLine_8bit(const signed int x0, const signed int y0, const signed int y1) const
{
	if (y0 > y1)	
		return;

	register Uint8 *p = (Uint8*)surface->pixels + y0 * surface->pitch + x0;
	register Sint16 i = y1-y0+1;
	Uint32 pen_col = (Uint32)(*pen.GetColor());
	Lock();
	switch( i % 4 ) {					
		do{								 
			case 0: *p = pen_col; p+=surface->pitch;
			case 3: *p = pen_col; p+=surface->pitch;
			case 2: *p = pen_col; p+=surface->pitch;
			case 1: *p = pen_col; p+=surface->pitch;
		}while( (i-=4) > 0 );
	}
	Unlock();
}


void DC::Draw_VLine_16bit(const signed int x0, const signed int y0, const signed int y1) const
{
	if (y0 > y1)	
		return;

	register Uint8 *p = (Uint8*)surface->pixels + y0 * surface->pitch + x0 * 2;
	register Sint16 i = y1-y0+1;
	Uint32 pen_col = (Uint32)(*pen.GetColor());
	Lock();
	 switch( i % 4 ) {								 
		do{											
			case 0: *(Uint16*)p = pen_col; p+=surface->pitch;
			case 3: *(Uint16*)p = pen_col; p+=surface->pitch;
			case 2: *(Uint16*)p = pen_col; p+=surface->pitch;
			case 1: *(Uint16*)p = pen_col; p+=surface->pitch;
		}while( (i-=4) > 0 );
	}
	Unlock();
}

void DC::Draw_VLine_24bit(const signed int x0, const signed int y0, const signed int y1) const
{
	if (y0 > y1)	
		return;

	register Uint8 *p = (Uint8*)surface->pixels + y0 * surface->pitch + x0 * 3;
	register Sint16 k = y1-y0+1;
	Uint32 pen_col = (Uint32)(*pen.GetColor());
	
	Uint8 colorbyte0 = (Uint8) (pen_col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((pen_col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((pen_col >> 16) & 0xff);

	Lock();
	
	SDL_DO_DRAWING_Y

	Unlock();
}

void DC::Draw_VLine_32bit(const signed int x0, const signed int y0, const signed int y1) const
{
	if (y0 > y1)	
		return;

	register Uint8 *p = (Uint8*)surface->pixels + y0 * surface->pitch + x0 * 4;
	register Sint16 i = y1-y0+1;
	Uint32 pen_col = (Uint32)(*pen.GetColor());
	
	Lock();

	for(;i--; p+=surface->pitch)
		*(Uint32*)p = pen_col;
	
	Unlock();
}


void DC::Draw_HLine_8bit(const signed int x0, const signed int y0, const signed int x1) const
{
	if (x0 > x1)	
		return;
	Uint32 col = (Uint32)(*pen.GetColor());
	register Uint8 *p;
	p = (Uint8*)surface->pixels + y0 * surface->pitch + x0;
	memset(p, col, x1-x0+1);
}

void DC::Draw_HLine_16bit(const signed int x0, const signed int y0, const signed int x1) const
{
	if (x0 > x1)	
		return;
	Uint32 col = (Uint32)(*pen.GetColor());
	register Uint8 *p;
	register Sint16 i;
	p = (Uint8*)surface->pixels + y0 * surface->pitch + x0 * 2;

	i = x1-x0+1;						 
	switch( i % 4 ) {					 
		do{								
			case 0: *(Uint16*)p = col; p+=2;
			case 3: *(Uint16*)p = col; p+=2; 
			case 2: *(Uint16*)p = col; p+=2;
			case 1: *(Uint16*)p = col; p+=2;
		}while( (i-=4) > 0 );				
	}
}

void DC::Draw_HLine_24bit(const signed int x0, const signed int y0, const signed int x1) const
{
	if (x0 > x1)	
		return;
	Uint32 col = (Uint32)(*pen.GetColor());
	Uint8 colorbyte0 = (Uint8) (col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((col >> 16) & 0xff);
	register Uint8 *p = (Uint8*)surface->pixels + y0 * surface->pitch + x0 * 3;
	register Sint16 k = x1-x0+1;

	Lock();
	
	SDL_DO_DRAWING_X
	
	Unlock();
}

void DC::Draw_HLine_32bit(const signed int x0, const signed int y0, const signed int x1) const
{
	if (x0 > x1)	
		return;
	Uint32 pen_col = (Uint32)(*pen.GetColor());
	Lock();
	if (sizeof(wchar_t) == sizeof(Uint32)) { 
#ifdef __linux__
		wmemset((wchar_t*)( (Uint8*)surface->pixels +  y0 * surface->pitch + (x0*4)), pen_col, x1-x0);
#elif __WIN32__
		memset((wchar_t*)( (Uint8*)surface->pixels +  y0 * surface->pitch + (x0*4)), pen_col, x1-x0);
#endif
	} else 
	{
		register Uint8* p = (Uint8*)surface->pixels + y0 * surface->pitch + (x0*4);
		register Sint16 i = x1-x0;
		for(;i--;p+=4)
			*(Uint32*)p = pen_col;
	}
	Unlock();
}

void DC::Draw_Line_8bit(signed int x1, signed int y1, signed int x2, signed int y2) const
{
	if(y1 == y2) {
		(*this.*Draw_HLine)(x1, y1, x2);return;
	}
	if(x1 == x2) {
		(*this.*Draw_VLine)(x1, y1, y2);return;
	}

	Color col = (*pen.GetColor());
	int dx = x2 - x1;
	int dy = y2 - y1;
	int sx = (dx >= 0) ? 1 : -1;
	int sy = (dy >= 0) ? 1 : -1;
	dx = sx * dx + 1;
	dy = sy * dy + 1;
	int pixx = 1;
	int pixy = surface->pitch;
	Uint8* pixel = ((Uint8 *) surface->pixels) + pixx * (int) x1 + pixy * (int) y1;
	pixx *= sx;
	pixy *= sy;
	if (dx < dy) {
		int swaptmp = dx;dx = dy;dy = swaptmp;
		swaptmp = pixx;pixx = pixy;pixy = swaptmp;
	}
	int x = 0;
	int y = 0;
	
	Lock();
	for (; x < dx; x++, pixel += pixx) {
		*pixel = (Uint8)col;
		y += dy;
		if (y >= dx) {
			y -= dx;
			pixel += pixy;
		}
	}
	Unlock();
}

void DC::Draw_Line_16bit(signed int x1, signed int y1, signed int x2, signed int y2) const
{
	if(y1 == y2) {
		(*this.*Draw_HLine)(x1, y1, x2);return;
	}
	if(x1 == x2) {
		(*this.*Draw_VLine)(x1, y1, y2);return;
	}
	
	Color col = (*pen.GetColor());
	int dx = x2 - x1;
	int dy = y2 - y1;
	int sx = (dx >= 0) ? 1 : -1;
	int sy = (dy >= 0) ? 1 : -1;
	dx = sx * dx + 1;
	dy = sy * dy + 1;
	int pixx = 2;
	int pixy = surface->pitch;
	Uint8* pixel = ((Uint8 *) surface->pixels) + pixx * (int) x1 + pixy * (int) y1;
	pixx *= sx;
	pixy *= sy;
	if (dx < dy) {
		int swaptmp = dx;dx = dy;dy = swaptmp;
		swaptmp = pixx;pixx = pixy;pixy = swaptmp;
	}
	int x = 0;
	int y = 0;
	
	Lock();
	for (; x < dx; x++, pixel += pixx) {
		*(Uint16 *) pixel = (Uint16)col;
		y += dy;
		if (y >= dx) {
			y -= dx;
			pixel += pixy;
		}
	}
	Unlock();
}

void DC::Draw_Line_24bit(signed int x1, signed int y1, signed int x2, signed int y2) const
{
	if(y1 == y2) {
		(*this.*Draw_HLine)(x1, y1, x2);return;
	}
	if(x1 == x2) {
		(*this.*Draw_VLine)(x1, y1, y2);return;
	}

	Color col = (*pen.GetColor());
	Uint8 colorbyte0 = (Uint8) (col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((col >> 16) & 0xff);

	int dx = x2 - x1;
	int dy = y2 - y1;
	int sx = (dx >= 0) ? 1 : -1;
	int sy = (dy >= 0) ? 1 : -1;
	dx = sx * dx + 1;
	dy = sy * dy + 1;
	int pixx = 3;
	int pixy = surface->pitch;
	Uint8* p = ((Uint8 *) surface->pixels) + pixx * (int) x1 + pixy * (int) y1;
	pixx *= sx;
	pixy *= sy;
	if (dx < dy) {
		int swaptmp = dx;dx = dy;dy = swaptmp;
		swaptmp = pixx;pixx = pixy;pixy = swaptmp;
	}
	int x = 0;
	int y = 0;

	Lock();
	for (; x < dx; x++, p += pixx) {
		
		SDL_DRAW_PUTPIXEL_BPP_3_AUX
			
		y += dy;
		if (y >= dx) {
			y -= dx;
			p += pixy;
		}
	}
	Unlock();
}

void DC::Draw_Line_32bit(signed int x1, signed int y1, signed int x2, signed int y2) const
{
	if(y1 == y2) {
		(*this.*Draw_HLine)(x1, y1, x2);return;
	}
	if(x1 == x2) {
		(*this.*Draw_VLine)(x1, y1, y2);return;
	}

	Color col = (*pen.GetColor());
	int dx = x2 - x1;
	int dy = y2 - y1;
	int sx = (dx >= 0) ? 1 : -1;
	int sy = (dy >= 0) ? 1 : -1;
	dx = sx * dx + 1;
	dy = sy * dy + 1;
	int pixx = 4;
	int pixy = surface->pitch;
	Uint8* pixel = ((Uint8 *) surface->pixels) + pixx * (int) x1 + pixy * (int) y1;
	pixx *= sx;
	pixy *= sy;
	if (dx < dy) {
		int swaptmp = dx;dx = dy;dy = swaptmp;
		swaptmp = pixx;pixx = pixy;pixy = swaptmp;
	}
	int x = 0;
	int y = 0;

	Lock();
	for (; x < dx; x++, pixel += pixx) {
		*(Uint32 *) pixel = (Uint32)col;
		y += dy;
		if (y >= dx) {
			y -= dx;
			pixel += pixy;
		}
	}
	Unlock();
}

void DC::DrawFilledRound_8bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	unsigned int radius = corner;
	Uint32 col = (Uint32)(*brush.GetColor());

	int rightInc = 6;
	int d = width<height ? width : height;

	d = 3 - (radius*2);
	int diagonalInc = 10 - (radius*4);
 	int Xcenter = x+radius;
	int Ycenter = y+radius;
	int dx = width - (radius*2);
	int dy = height - (radius*2);
	int X2center=Xcenter + dx;
	int Y2center=Ycenter + dy;

// FILLED
	SDL_Rect r;
	// Rechteck von linker Kante nach rechter Kante
	if(dy > 0)
	{
		r.x = x; r.y = Ycenter;
		r.w = width; r.h = dy;
		SDL_FillRect(surface, &r, col);
	}

	// Rechteck entlang der oberen und entlang der unteren Kante
	if((radius > 0)&&(dx>0))
	{
		r.x = Xcenter; r.y = y;
		r.w = dx; r.h = radius;
		SDL_FillRect(surface, &r, col);

		r.x = Xcenter; r.y = Y2center;
		r.w = dx; r.h = radius;
		SDL_FillRect(surface, &r, col);
	}
	unsigned int i = 0;
	Lock();
	while (i <= radius) 
	{
// TODO optimieren evtl 4 bytes zusammenfassen
// Nur Ecken:
		for(unsigned int k=i;k--;)
		{
			*((Uint8*)((Uint8*)surface->pixels + k+ (Ycenter-radius)*surface->pitch + (Xcenter - i))) = col;
			*((Uint8*)((Uint8*)surface->pixels + k+(Ycenter-radius)*surface->pitch + X2center)) = col;
			*((Uint8*)((Uint8*)surface->pixels + k+(Y2center+radius-1)*surface->pitch + X2center)) = col;
			*((Uint8*)((Uint8*)surface->pixels + k+(Y2center+radius-1)*surface->pitch + (Xcenter-i))) = col;

		}
		for(unsigned int k=radius;k--;)
		{
			*((Uint8*)((Uint8*)surface->pixels + k+(Ycenter-i)*surface->pitch + (Xcenter - radius))) = col;
			*((Uint8*)((Uint8*)surface->pixels + k+(Ycenter-i)*surface->pitch + X2center)) = col;
			*((Uint8*)((Uint8*)surface->pixels + k+(Y2center+i-1)*surface->pitch + X2center)) = col;
			*((Uint8*)((Uint8*)surface->pixels + k+(Y2center+i-1)*surface->pitch + (Xcenter-radius))) = col;
		}
		if (d >= 0) 
		{
			d += diagonalInc;
			diagonalInc += 8;
			--radius;
		} else {
			d += rightInc;
			diagonalInc += 4;
		}
		rightInc += 4;
		++i;
	}/*while*/
	Unlock();
}

void DC::DrawFilledRound_16bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	unsigned int radius = corner;
	Uint32 col = (Uint32)(*brush.GetColor());

	int rightInc = 6;
	int d = width<height ? width : height;

	d = 3 - (radius*2);
	int diagonalInc = 10 - (radius*4);
 	int Xcenter = x+radius;
	int Ycenter = y+radius;
	int dx = width - (radius*2);
	int dy = height - (radius*2);
	int X2center=Xcenter + dx;
	int Y2center=Ycenter + dy;

// FILLED
	SDL_Rect r;
	// Rechteck von linker Kante nach rechter Kante
	if(dy > 0)
	{
		r.x = x; r.y = Ycenter;
		r.w = width; r.h = dy;
		SDL_FillRect(surface, &r, col);
	}

	// Rechteck entlang der oberen und entlang der unteren Kante
	if((radius > 0)&&(dx>0))
	{
		r.x = Xcenter; r.y = y;
		r.w = dx; r.h = radius;
		SDL_FillRect(surface, &r, col);

		r.x = Xcenter; r.y = Y2center;
		r.w = dx; r.h = radius;
		SDL_FillRect(surface, &r, col);
	}
	unsigned int i = 0;
	
	Lock();
	while (i <= radius) 
	{

// Nur Ecken:
		for(unsigned int k=i;k--;)
		{
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (k + Xcenter - i)*2)) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (k + X2center)*2)) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + (k + X2center)*2)) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + (k + Xcenter - i)*2)) = col;

		}
		for(unsigned int k=radius;k--;)
		{
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (k+Xcenter - radius)*2)) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (k+X2center)*2)) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + (k+X2center)*2)) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + (k+Xcenter-radius)*2)) = col;
		}
		if (d >= 0) 
		{
			d += diagonalInc;
			diagonalInc += 8;
			--radius;
		} else {
			d += rightInc;
			diagonalInc += 4;
		}
		rightInc += 4;
		++i;
	}/*while*/
	Unlock();

}

// DONE
void DC::DrawFilledRound_24bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	unsigned int radius = corner;
	Uint32 col = (Uint32)(*brush.GetColor());

	int rightInc = 6;
	int d = width<height ? width : height;

	d = 3 - (radius*2);
	int diagonalInc = 10 - (radius*4);
 	int Xcenter = x+radius;
	int Ycenter = y+radius;
	int dx = width - (radius*2);
	int dy = height - (radius*2);
	int X2center=Xcenter + dx;
	int Y2center=Ycenter + dy;

// FILLED
	SDL_Rect r;
	// Rechteck von linker Kante nach rechter Kante
	if(dy > 0)
	{
		r.x = x; r.y = Ycenter;
		r.w = width; r.h = dy;
		SDL_FillRect(surface, &r, col);
	}

	// Rechteck entlang der oberen und entlang der unteren Kante
	if((radius > 0)&&(dx>0))
	{
		r.x = Xcenter; r.y = y;
		r.w = dx; r.h = radius;
		SDL_FillRect(surface, &r, col);

		r.x = Xcenter; r.y = Y2center;
		r.w = dx; r.h = radius;
		SDL_FillRect(surface, &r, col);
	}

	Uint8 colorbyte0 = (Uint8) (col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((col >> 16) & 0xff);

	unsigned int i = 0;
	
	Lock();	
	while (i <= radius) 
	{
		register Uint8 *p;
		register Sint16 k;

		
		p = (Uint8*)surface->pixels + (Ycenter-radius) * surface->pitch + (Xcenter-i) * 3;k = i;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Ycenter-radius) * surface->pitch + X2center * 3;k = i;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Y2center+radius-1) * surface->pitch + X2center * 3;k = i;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Y2center+radius-1) * surface->pitch + (Xcenter-i) * 3;k = i;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Ycenter-i) * surface->pitch + (Xcenter-radius) * 3;k = radius;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Ycenter-i) * surface->pitch + X2center * 3;k = radius;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Y2center+i-1) * surface->pitch + X2center * 3;k = radius;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Y2center+i-1) * surface->pitch + (Xcenter-radius) * 3;k = radius;SDL_DO_DRAWING_X
		
		if (d >= 0) 
		{
			d += diagonalInc;
			diagonalInc += 8;
			--radius;
		} else {
			d += rightInc;
			diagonalInc += 4;
		}
		rightInc += 4;
		++i;
	}/*while*/
	Unlock();
}

void DC::DrawFilledRound_32bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	unsigned int radius = corner;
	Uint32 col = (Uint32)(*brush.GetColor());

	int rightInc = 6;
	int d = width<height ? width : height;

	d = 3 - (radius*2);
	int diagonalInc = 10 - (radius*4);
 	int Xcenter = x+radius;
	int Ycenter = y+radius;
	int dx = width - (radius*2);
	int dy = height - (radius*2);
	int X2center=Xcenter + dx;
	int Y2center=Ycenter + dy;

// FILLED
	SDL_Rect r;
	// Rechteck von linker Kante nach rechter Kante
	if(dy > 0)
	{
		r.x = x; r.y = Ycenter;
		r.w = width; r.h = dy;
		SDL_FillRect(surface, &r, col);
	}

	// Rechteck entlang der oberen und entlang der unteren Kante
	if((radius > 0)&&(dx>0))
	{
		r.x = Xcenter; r.y = y;
		r.w = dx; r.h = radius;
		SDL_FillRect(surface, &r, col);

		r.x = Xcenter; r.y = Y2center;
		r.w = dx; r.h = radius;
		SDL_FillRect(surface, &r, col);
	}
	
	unsigned int i = 0;
	Lock();
	while (i <= radius) 
	{

// Nur Ecken:
#ifdef __linux__
	// links oben
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (Xcenter - i +1)*4), col, i);
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (Xcenter - radius + 1)*4), col, radius);
	// rechts oben
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + X2center*4), col, i);
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + X2center*4), col, radius);
	// links unten
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + (Xcenter - i + 1)*4), col, i);
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + (Xcenter - radius + 1)*4), col, radius);
	// rechts unten
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + X2center*4), col, i);
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + X2center*4), col, radius);
#elif __WIN32__
		for(unsigned int k=i;k--;)
		{
			*((Uint32*)((Uint8*)surface->pixels + k+ (Ycenter-radius)*surface->pitch + (Xcenter - i)*4)) = col;
			*((Uint32*)((Uint8*)surface->pixels + k+(Ycenter-radius)*surface->pitch + X2center*4)) = col;
			*((Uint32*)((Uint8*)surface->pixels + k+(Y2center+radius-1)*surface->pitch + X2center*4)) = col;
			*((Uint32*)((Uint8*)surface->pixels + k+(Y2center+radius-1)*surface->pitch + (Xcenter-i)*4)) = col;

		}
		for(unsigned int k=radius;k--;)
		{
			*((Uint32*)((Uint8*)surface->pixels + k+(Ycenter-i)*surface->pitch + (Xcenter - radius)*4)) = col;
			*((Uint32*)((Uint8*)surface->pixels + k+(Ycenter-i)*surface->pitch + X2center*4)) = col;
			*((Uint32*)((Uint8*)surface->pixels + k+(Y2center+i-1)*surface->pitch + X2center*4)) = col;
			*((Uint32*)((Uint8*)surface->pixels + k+(Y2center+i-1)*surface->pitch + (Xcenter-radius)*4)) = col;
		}
#endif
		if (d >= 0) 
		{
			d += diagonalInc;
			diagonalInc += 8;
			--radius;
		} else {
			d += rightInc;
			diagonalInc += 4;
		}
		rightInc += 4;
		++i;
	}/*while*/
	Unlock();

}

void DC::DrawFilledEdgedRound_8bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	unsigned int radius = corner;
	Uint32 col = (Uint32)(*brush.GetColor());
	SDL_Rect r;
	Sint16 rightInc = 6;
	Sint16 d, diagonalInc;

	/*TODO: We can do better :-)*/
	d = width<height ? width : height;
	d = 3 - (radius*2);
	diagonalInc = 10 - (radius*4);

	// Kantenlaengen
	int dx = width - (radius*2);
	int dy = height - (radius*2);

	// Ecke links oben
	int Xcenter = x + radius;
	int Ycenter = y + radius;

	// Ecke rechts unten
	int X2center = Xcenter + dx;
	int Y2center = Ycenter + dy;

	// Rechteck von linker Kante nach rechter Kante
	if(dy > 0)
	{
		r.x = x; r.y = Ycenter;
		r.w = width; r.h = dy;
		SDL_FillRect(surface, &r, col);
	}

	// Rechteck entlang der oberen und entlang der unteren Kante
	if((radius > 0)&&(dx>0))
	{
		r.x = Xcenter; r.y = y;
		r.w = dx; r.h = radius;
		SDL_FillRect(surface, &r, col);

		r.x = x; r.y = Y2center;
		r.w = width-radius; r.h = radius;
		SDL_FillRect(surface, &r, col);
	}

	unsigned int i = 0;
	Lock();
// Halbkreis
	while (i <= radius) 
	{
		for(unsigned int k=i;k--;)
		{
			*((Uint8*)((Uint8*)surface->pixels + k + (Ycenter-radius)*surface->pitch + (Xcenter - i))) = col;
			*((Uint8*)((Uint8*)surface->pixels + k + (Ycenter-radius)*surface->pitch + X2center)) = col;
			*((Uint8*)((Uint8*)surface->pixels + k + (Y2center+radius-1)*surface->pitch + X2center)) = col;
		}
		for(unsigned int k=radius;k--;)
		{
			*((Uint8*)((Uint8*)surface->pixels + k + (Ycenter-i)*surface->pitch + (Xcenter - radius))) = col;
			*((Uint8*)((Uint8*)surface->pixels + k + (Ycenter-i)*surface->pitch + X2center)) = col;
			*((Uint8*)((Uint8*)surface->pixels + k + (Y2center+i-1)*surface->pitch + X2center)) = col;
		}
		if (d >= 0) 
		{
			d += diagonalInc;
			diagonalInc += 8;
			--radius;
		} else 
		{
			d += rightInc;
			diagonalInc += 4;
		}
		rightInc += 4;
		++i;
	}
	Unlock();

}/*Draw_FillRound*/

void DC::DrawFilledEdgedRound_16bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	unsigned int radius = corner;
	Uint32 col = (Uint32)(*brush.GetColor());
	SDL_Rect r;
	Sint16 rightInc = 6;
	Sint16 d, diagonalInc;

	/*TODO: We can do better :-)*/
	d = width<height ? width : height;
	d = 3 - (radius*2);
	diagonalInc = 10 - (radius*4);

	// Kantenlaengen
	int dx = width - (radius*2);
	int dy = height - (radius*2);

	// Ecke links oben
	int Xcenter = x + radius;
	int Ycenter = y + radius;

	// Ecke rechts unten
	int X2center = Xcenter + dx;
	int Y2center = Ycenter + dy;

	// Rechteck von linker Kante nach rechter Kante
	if(dy > 0)
	{
		r.x = x; r.y = Ycenter;
		r.w = width; r.h = dy;
		SDL_FillRect(surface, &r, col);
	}

	// Rechteck entlang der oberen und entlang der unteren Kante
	if((radius > 0)&&(dx>0))
	{
		r.x = Xcenter; r.y = y;
		r.w = dx; r.h = radius;
		SDL_FillRect(surface, &r, col);

		r.x = x; r.y = Y2center;
		r.w = width-radius; r.h = radius;
		SDL_FillRect(surface, &r, col);
	}

	unsigned int i = 0;
	Lock();
// Halbkreis
	while (i <= radius) 
	{
		for(unsigned int k=i;k--;)
		{
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (k + Xcenter - i)*2)) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (k + X2center)*2)) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + (k + X2center*2))) = col;
		}
		for(unsigned int k=radius;k--;)
		{
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (k + Xcenter - radius)*2)) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (k + X2center)*2)) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + (k + X2center)*2)) = col;
		}

		if (d >= 0) 
		{
			d += diagonalInc;
			diagonalInc += 8;
			--radius;
		} else 
		{
			d += rightInc;
			diagonalInc += 4;
		}
		rightInc += 4;
		++i;
	}
	Unlock();

}/*Draw_FillRound*/

void DC::DrawFilledEdgedRound_24bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	unsigned int radius = corner;
	Uint32 col = (Uint32)(*brush.GetColor());
	SDL_Rect r;
	Sint16 rightInc = 6;
	Sint16 d, diagonalInc;

	/*TODO: We can do better :-)*/
	d = width<height ? width : height;
	d = 3 - (radius*2);
	diagonalInc = 10 - (radius*4);

	// Kantenlaengen
	int dx = width - (radius*2);
	int dy = height - (radius*2);

	// Ecke links oben
	int Xcenter = x + radius;
	int Ycenter = y + radius;

	// Ecke rechts unten
	int X2center = Xcenter + dx;
	int Y2center = Ycenter + dy;

	// Rechteck von linker Kante nach rechter Kante
	if(dy > 0)
	{
		r.x = x; r.y = Ycenter;
		r.w = width; r.h = dy;
		SDL_FillRect(surface, &r, col);
	}

	// Rechteck entlang der oberen und entlang der unteren Kante
	if((radius > 0)&&(dx>0))
	{
		r.x = Xcenter; r.y = y;
		r.w = dx; r.h = radius;
		SDL_FillRect(surface, &r, col);

		r.x = x; r.y = Y2center;
		r.w = width-radius; r.h = radius;
		SDL_FillRect(surface, &r, col);
	}

	Uint8 colorbyte0 = (Uint8) (col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((col >> 16) & 0xff);

	unsigned int i = 0;
	
	Lock();
	
	while (i <= radius) 
	{
		register Uint8 *p;
		register Sint16 k;
		p = (Uint8*)surface->pixels + (Ycenter-radius) * surface->pitch + (Xcenter-i) * 3;k = i;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Ycenter-radius) * surface->pitch + X2center * 3;k = i;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Ycenter+radius-1) * surface->pitch + X2center * 3;k = i;SDL_DO_DRAWING_X
		
		p = (Uint8*)surface->pixels + (Y2center-i) * surface->pitch + (Xcenter-radius) * 3;k = radius;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Y2center-i) * surface->pitch + X2center * 3;k = radius;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Y2center+i-1) * surface->pitch + X2center * 3;k = radius;SDL_DO_DRAWING_X

		if (d >= 0) 
		{
			d += diagonalInc;
			diagonalInc += 8;
			--radius;
		} else 
		{
			d += rightInc;
			diagonalInc += 4;
		}
		rightInc += 4;
		++i;
	}
	
	Unlock();

}/*Draw_FillRound*/

void DC::DrawFilledEdgedRound_32bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	unsigned int radius = corner;
	Uint32 col = (Uint32)(*brush.GetColor());
	SDL_Rect r;
	Sint16 rightInc = 6;
	Sint16 d, diagonalInc;

	/*TODO: We can do better :-)*/
	d = width<height ? width : height;
	d = 3 - (radius*2);
	diagonalInc = 10 - (radius*4);

	// Kantenlaengen
	int dx = width - (radius*2);
	int dy = height - (radius*2);

	// Ecke links oben
	int Xcenter = x + radius;
	int Ycenter = y + radius;

	// Ecke rechts unten
	int X2center = Xcenter + dx;
	int Y2center = Ycenter + dy;

	// Rechteck von linker Kante nach rechter Kante
	if(dy > 0)
	{
		r.x = x; r.y = Ycenter;
		r.w = width; r.h = dy;
		SDL_FillRect(surface, &r, col);
	}

	// Rechteck entlang der oberen und entlang der unteren Kante
	if((radius > 0)&&(dx>0))
	{
		r.x = Xcenter; r.y = y;
		r.w = dx; r.h = radius;
		SDL_FillRect(surface, &r, col);

		r.x = x; r.y = Y2center;
		r.w = width-radius; r.h = radius;
		SDL_FillRect(surface, &r, col);
	}
	
	unsigned int i = 0;
	Lock();
// Halbkreis
	while (i <= radius) 
	{
// Nur Ecken:
		for(unsigned int k=i;k--;)
		{
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (k + Xcenter - i)*4)) = col;
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (k + X2center)*4)) = col;
			*((Uint32*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + (k + X2center)*4)) = col;
		}
		for(unsigned int k=radius;k--;)
		{
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (k + Xcenter - radius)*4)) = col;
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (k + X2center)*4)) = col;
			*((Uint32*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + (k + X2center)*4)) = col;
		}

		if (d >= 0) 
		{
			d += diagonalInc;
			diagonalInc += 8;
			--radius;
		} else 
		{
			d += rightInc;
			diagonalInc += 4;
		}
		rightInc += 4;
		++i;
	}
	Unlock();

}/*Draw_FillRound*/

void DC::DrawEmptyEdgedRound_8bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	int dx = width - (corner*2);
	int dy = height - (corner*2);
	
	int Xcenter = x + corner;
	int Ycenter = y + corner;

	int X2center = Xcenter + dx;
	int Y2center = Ycenter + dy;

// ------ TOP AND LOWER HORIZONTAL LINE ------
	Uint32 pen_col = (Uint32)(*pen.GetColor());
	Lock();
	{
		register Uint8* p0 = (Uint8*)surface->pixels + y * surface->pitch + Xcenter;
		register Uint8* p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + x;
		register Sint16 i = corner;
		for(;i--;p1++)
			*(Uint8*)p1 = pen_col;
		i = dx;
		for(;i--;p0++,p1++)
		{
			*(Uint8*)p0 = pen_col;
			*(Uint8*)p1 = pen_col;
		}
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		if (height<3)	
			return;
		register Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + x;
		register Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x+width);
		register Sint16 i = dy;
		for(;i--;p0+=surface->pitch, p1+=surface->pitch)
		{
			*(Uint8*)p0 = pen_col;
			*(Uint8*)p1 = pen_col;
		}
		i = corner-1;
		for(;i--;p0+=surface->pitch)
			*(Uint8*)p0 = pen_col;	
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------

	int rightInc = 6;
	int d = 3 - (corner*2);
	int diagonalInc = 10 - (corner*4);
	unsigned int radius = corner;
	
	unsigned int i = 0;	
	while (i <= radius) 
	{
		*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + Xcenter - i)) = pen_col;
		*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + Xcenter - radius)) = pen_col;

		*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + X2center + i)) = pen_col;
		*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + X2center + radius)) = pen_col;

		*((Uint8*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + Xcenter - corner)) = pen_col;
		*((Uint8*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + Xcenter - corner)) = pen_col;

		*((Uint8*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + X2center + i)) = pen_col;
		*((Uint8*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + X2center + radius)) = pen_col;

		*((Uint8*)((Uint8*)surface->pixels + (Y2center+corner-1)*surface->pitch + Xcenter - radius)) = pen_col; // new
		*((Uint8*)((Uint8*)surface->pixels + (Y2center+corner-1)*surface->pitch + Xcenter - i)) = pen_col; // new

		if (d >= 0) {
			d += diagonalInc;
			diagonalInc += 8;
			--radius;
		} else {
			d += rightInc;
			diagonalInc += 4;
		}
		rightInc += 4;
		++i;
	}/*while*/
	Unlock();

}/*Draw_Round*/

void DC::DrawEmptyEdgedRound_16bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	int dx = width - (corner*2);
	int dy = height - (corner*2);
	
	int Xcenter = x + corner;
	int Ycenter = y + corner;

	int X2center = Xcenter + dx;
	int Y2center = Ycenter + dy;

// ------ TOP AND LOWER HORIZONTAL LINE ------
	Uint32 pen_col = (Uint32)(*pen.GetColor());
	Lock();
	{
		register Uint8* p0 = (Uint8*)surface->pixels + y * surface->pitch + (Xcenter*2);
		register Uint8* p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + (x*2);
		register Sint16 i = corner;
		for(;i--;p1+=2)
			*(Uint16*)p1 = pen_col;
		i = dx;
		for(;i--;p0+=2,p1+=2)
		{
			*(Uint16*)p0 = pen_col;
			*(Uint16*)p1 = pen_col;
		}
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		if (height<3)	
			return;
		register Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x*2);
		register Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + ((x+width)*2);
		register Sint16 i = dy;
		for(;i--;p0+=surface->pitch, p1+=surface->pitch)
		{
			*(Uint16*)p0 = pen_col;
			*(Uint16*)p1 = pen_col;
		}
		i = corner-1;
		for(;i--;p0+=surface->pitch)
			*(Uint16*)p0 = pen_col;	
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------
	{
		int rightInc = 6;
		int d = 3 - (corner*2);
		int diagonalInc = 10 - (corner*4);
		unsigned int radius = corner;
	
		unsigned int i = 0;	
		while (i <= radius) 
		{
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((Xcenter - i)*2))) = pen_col;
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((Xcenter - radius)*2))) = pen_col;

			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((X2center + i)*2))) = pen_col;
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((X2center + radius)*2))) = pen_col;

			*((Uint16*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + ((Xcenter - corner)*2))) = pen_col;
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + ((Xcenter - corner)*2))) = pen_col;

			*((Uint16*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + ((X2center + i)*2))) = pen_col;
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + ((X2center + radius)*2))) = pen_col;

			*((Uint16*)((Uint8*)surface->pixels + (Y2center+corner-1)*surface->pitch + ((Xcenter - radius)*2))) = pen_col; // new
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+corner-1)*surface->pitch + ((Xcenter - i)*2))) = pen_col; // new

			if (d >= 0) {
				d += diagonalInc;
				diagonalInc += 8;
				--radius;
			} else {
				d += rightInc;
				diagonalInc += 4;
			}
			rightInc += 4;
			++i;
		}/*while*/
	}
	Unlock();

}/*Draw_Round*/

void DC::DrawEmptyEdgedRound_24bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	int dx = width - (corner*2);
	int dy = height - (corner*2);
	
	int Xcenter = x + corner;
	int Ycenter = y + corner;

	int X2center = Xcenter + dx;
	int Y2center = Ycenter + dy;

// ------ TOP AND LOWER HORIZONTAL LINE ------
	Uint32 pen_col = (Uint32)(*pen.GetColor());
	Uint8 colorbyte0 = (Uint8) (pen_col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((pen_col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((pen_col >> 16) & 0xff);

	Lock();
	
	{
		register Uint8* p;
		register int k;
		p = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + x * 3;k = dx+corner-1;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + y * surface->pitch + Xcenter * 3;k = dx;SDL_DO_DRAWING_X
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		register Uint8* p;
		register int k;
		
		p = (Uint8*)surface->pixels + Ycenter * surface->pitch + x * 3;k = corner+dy-1;SDL_DO_DRAWING_Y
		p = (Uint8*)surface->pixels + Ycenter * surface->pitch + (x+width) * 3;k = dy;SDL_DO_DRAWING_Y
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------

	int rightInc = 6;
	int d = 3 - (corner*2);
	int diagonalInc = 10 - (corner*4);
	unsigned int radius = corner;

	register Uint8* p;
	unsigned int i = 0;	
	while (i <= radius) 
	{
		p = (Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (Xcenter - i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + X2center*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Y2center + radius - 1)*surface->pitch + X2center*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (Xcenter - radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + X2center*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Y2center + i - 1)*surface->pitch + X2center*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		if (d >= 0) {
			d += diagonalInc;
			diagonalInc += 8;
			--radius;
		} else {
			d += rightInc;
			diagonalInc += 4;
		}
		rightInc += 4;
		++i;
	}/*while*/
	
	Unlock();
}/*Draw_Round*/

void DC::DrawEmptyEdgedRound_32bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	int dx = width - (corner*2);
	int dy = height - (corner*2);
	
	int Xcenter = x + corner;
	int Ycenter = y + corner;

	int X2center = Xcenter + dx;
	int Y2center = Ycenter + dy;

// ------ TOP AND LOWER HORIZONTAL LINE ------
	Uint32 pen_col = (Uint32)(*pen.GetColor());
	Lock();
	if (sizeof(wchar_t) == sizeof(Uint32)) { 
#ifdef __linux__
		wmemset((wchar_t*)( (Uint8*)surface->pixels +  y * surface->pitch + (Xcenter*4)), pen_col, dx);
		wmemset((wchar_t*)( (Uint8*)surface->pixels +  (y+height-1) * surface->pitch + (x*4)), pen_col, width - corner);
#elif __WIN32__
		memset((wchar_t*)( (Uint8*)surface->pixels +  y * surface->pitch + (Xcenter*4)), pen_col, dx);
		memset((wchar_t*)( (Uint8*)surface->pixels +  (y+height-1) * surface->pitch + (x*4)), pen_col, width - corner);
#endif
	} else 
	{
		register Uint8* p0 = (Uint8*)surface->pixels + y * surface->pitch + (Xcenter*4);
		register Uint8* p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + (x*4);
		register Sint16 i = corner;
		for(;i--;p1+=4)
			*(Uint32*)p1 = pen_col;
		i = dx;
		for(;i--;p0+=4,p1+=4)
		{
			*(Uint32*)p0 = pen_col;
			*(Uint32*)p1 = pen_col;
		}
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		if (height<3)	
			return;
		register Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x*4);
		register Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + ((x+width)*4);
		register Sint16 i = dy;
		for(;i--;p0+=surface->pitch, p1+=surface->pitch)
		{
			*(Uint32*)p0 = pen_col;
			*(Uint32*)p1 = pen_col;
		}
		i = corner-1;
		for(;i--;p0+=surface->pitch)
			*(Uint32*)p0 = pen_col;	
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------

	int rightInc = 6;
	int d = 3 - (corner*2);
	int diagonalInc = 10 - (corner*4);
	unsigned int radius = corner;
	
	unsigned int i = 0;	
	while (i <= radius) 
	{
		*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((Xcenter - i)*4))) = pen_col;
		*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((Xcenter - radius)*4))) = pen_col;

		*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((X2center + i)*4))) = pen_col;
		*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((X2center + radius)*4))) = pen_col;

		*((Uint32*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + ((X2center + i)*4))) = pen_col;
		*((Uint32*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + ((X2center + radius)*4))) = pen_col;

		if (d >= 0) {
			d += diagonalInc;
			diagonalInc += 8;
			--radius;
		} else {
			d += rightInc;
			diagonalInc += 4;
		}
		rightInc += 4;
		++i;
	}/*while*/
	Unlock();

}/*Draw_Round*/

void DC::DrawFilledEdgedBorderRound_8bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	// Kantenlaengen
	int dx = width - (corner*2);
	int dy = height - (corner*2);

	// Ecke links oben
	int Xcenter = x + corner;
	int Ycenter = y + corner;

	// Ecke rechts unten
	int X2center = Xcenter + dx;
	int Y2center = Ycenter + dy;

// Rechteck von linker Kante nach rechter Kante
	Uint32 brush_col = (Uint32)(*brush.GetColor());
	
	if(dy > 0)
	{
		SDL_Rect r;
		r.x = x+1; r.y = Ycenter;
		r.w = width-1; r.h = dy;
		SDL_FillRect(surface, &r, brush_col);
	}

// ------ Rechteck entlang der oberen und entlang der unteren Kante ------
	if((corner > 0)&&(dx>0))
	{
		SDL_Rect r;
		
		r.x = Xcenter; r.y = y+1;
		r.w = dx; r.h = corner-1;
		SDL_FillRect(surface, &r, brush_col);

		r.x = x+1; r.y = Y2center;
		r.w = width-corner-1; r.h = corner-1;
		SDL_FillRect(surface, &r, brush_col);
	}
// ------- END OF Rechteck oben und unten ------


// ------ TOP AND LOWER HORIZONTAL LINE ------
	Uint32 pen_col = (Uint32)(*pen.GetColor());
	Lock();
	{
		register Uint8* p0 = (Uint8*)surface->pixels + y * surface->pitch + Xcenter;
		register Uint8* p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + x;
		register Sint16 i = corner;
		for(;i--;p1++)
			*p1 = pen_col;
		i = dx;
		for(;i--;p0++,p1++)
		{
			*p0 = pen_col;
			*p1 = pen_col;
		}
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

	
// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		if (height<3)	
			return;
		register Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + x;	 
		register Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + x+width;
		register Sint16 i = dy;
		for(;i--;p0+=surface->pitch, p1+=surface->pitch)
		{
			*p0 = pen_col;
			*p1 = pen_col;
		}
		i = corner-1;
		for(;i--;p0+=surface->pitch)
			*p0 = pen_col;	
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------

// Halbkreis
	//???
	{
		int d = 3 - (corner*2);
		unsigned int i = 0;
		int rightInc = 6;
		int diagonalInc = 10 - (corner*4);
		unsigned int radius = corner;
	
		while (i <= radius) 
		{
			for(unsigned int k=i;k--;)
			{
				*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + k + Xcenter - i)) = brush_col;
				*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + k + X2center)) = brush_col;
				*((Uint8*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + k + X2center)) = brush_col;
			}
			for(unsigned int k=radius;k--;)
			{
				*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + k + Xcenter - radius)) = brush_col;
				*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + k + X2center)) = brush_col;
				*((Uint8*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + k + X2center)) = brush_col;
			}
			if (d >= 0) 
			{
				d += diagonalInc;
				diagonalInc += 8;
				--radius;
			} else 
			{
				d += rightInc;
				diagonalInc += 4;
			}
			rightInc += 4;
			++i;
		}
	}
	{
		int d = 3 - (corner*2);
		int rightInc = 6;
		int diagonalInc = 10 - (corner*4);
		unsigned int radius = corner;

		unsigned int i = 0;
		while (i <= radius) 
		{
			*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + Xcenter - i)) = pen_col;
			*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + Xcenter - radius)) = pen_col;

			*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + X2center + i)) = pen_col;
			*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + X2center + radius)) = pen_col;

			*((Uint8*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + X2center + i)) = pen_col;
			*((Uint8*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + X2center + radius)) = pen_col;

			if (d >= 0) 
			{
				d += diagonalInc;
				diagonalInc += 8;
				--radius;
			} else 
			{
				d += rightInc;
				diagonalInc += 4;
			}
			rightInc += 4;
			++i;
		}
	}
	Unlock();

}/*DrawFilledEdgedBorderRound*/


void DC::DrawFilledEdgedBorderRound_16bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	// Kantenlaengen
	int dx = width - (corner*2);
	int dy = height - (corner*2);

	// Ecke links oben
	int Xcenter = x + corner;
	int Ycenter = y + corner;

	// Ecke rechts unten
	int X2center = Xcenter + dx;
	int Y2center = Ycenter + dy;

// Rechteck von linker Kante nach rechter Kante
	Uint32 brush_col = (Uint32)(*brush.GetColor());
	
	if(dy > 0)
	{
		SDL_Rect r;
		r.x = x+1; r.y = Ycenter;
		r.w = width-1; r.h = dy;
		SDL_FillRect(surface, &r, brush_col);
	}

// ------ Rechteck entlang der oberen und entlang der unteren Kante ------
	if((corner > 0)&&(dx>0))
	{
		SDL_Rect r;
		
		r.x = Xcenter; r.y = y+1;
		r.w = dx; r.h = corner-1;
		SDL_FillRect(surface, &r, brush_col);

		r.x = x+1; r.y = Y2center;
		r.w = width-corner-1; r.h = corner-1;
		SDL_FillRect(surface, &r, brush_col);
	}
// ------- END OF Rechteck oben und unten ------


// ------ TOP AND LOWER HORIZONTAL LINE ------
	Uint32 pen_col = (Uint32)(*pen.GetColor());
	Lock();
{
	register Uint8* p0 = (Uint8*)surface->pixels + y * surface->pitch + (Xcenter*2);
	register Uint8* p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + (x*2);
	register Sint16 i = corner;
	for(;i--;p1+=2)
		*(Uint16*)p1 = pen_col;
	i = dx;
	for(;i--;p0+=2,p1+=2)
	{
		*(Uint16*)p0 = pen_col;
		*(Uint16*)p1 = pen_col;
	}
}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

	
// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		if (height<3)	
			return;
		register Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x*2);	 
		register Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + ((x+width)*2);
		register Sint16 i = dy;
		for(;i--;p0+=surface->pitch, p1+=surface->pitch)
		{
			*(Uint16*)p0 = pen_col;
			*(Uint16*)p1 = pen_col;
		}
		i = corner-1;
		for(;i--;p0+=surface->pitch)
			*(Uint16*)p0 = pen_col;	
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------

// Halbkreis
	//???
	{
		int d = 3 - (corner*2);
		int rightInc = 6;
		int diagonalInc = 10 - (corner*4);
		unsigned int radius = corner;
	
		unsigned int i = 0;
		while (i <= radius) 
		{
			for(unsigned int k=i;k--;)
			{
				*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (k + Xcenter - i)*2)) = brush_col;
				*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (k + X2center)*2)) = brush_col;
				*((Uint16*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + (k + X2center)*2)) = brush_col;
			}
			for(unsigned int k=radius;k--;)
			{
				*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (k + Xcenter - radius)*2)) = brush_col;
				*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (k + X2center) *2)) = brush_col;
				*((Uint16*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + (k + X2center)*2)) = brush_col;
			}
			if (d >= 0) 
			{
				d += diagonalInc;
				diagonalInc += 8;
				--radius;
			} else 
			{
				d += rightInc;
				diagonalInc += 4;
			}
			rightInc += 4;
			++i;
		}
	}
	{
		int d = 3 - (corner*2);
		int rightInc = 6;
		int diagonalInc = 10 - (corner*4);
		unsigned int radius = corner;
		
		unsigned int i = 0;
		while (i <= radius) 
		{
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((Xcenter - i)*2))) = pen_col;
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((Xcenter - radius)*2))) = pen_col;

			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((X2center + i)*2))) = pen_col;
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((X2center + radius)*2))) = pen_col;

			*((Uint16*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + ((X2center + i)*2))) = pen_col;
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + ((X2center + radius)*2))) = pen_col;
			if (d >= 0) 
			{
				d += diagonalInc;
				diagonalInc += 8;
				--radius;
			} else 
			{
				d += rightInc;
				diagonalInc += 4;
			}
			rightInc += 4;
			++i;
		}
	}
	Unlock();

}/*DrawFilledEdgedBorderRound*/


void DC::DrawFilledEdgedBorderRound_24bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	// Kantenlaengen
	int dx = width - (corner*2);
	int dy = height - (corner*2);

	// Ecke links oben
	int Xcenter = x + corner;
	int Ycenter = y + corner;

	// Ecke rechts unten
	int X2center = Xcenter + dx;
	int Y2center = Ycenter + dy;

// Rechteck von linker Kante nach rechter Kante
	Uint32 brush_col = (Uint32)(*brush.GetColor());
	
	if(dy > 0)
	{
		SDL_Rect r;
		r.x = x+1; r.y = Ycenter;
		r.w = width-1; r.h = dy;
		SDL_FillRect(surface, &r, brush_col);
	}

// ------ Rechteck entlang der oberen und entlang der unteren Kante ------
	if((corner > 0)&&(dx>0))
	{
		SDL_Rect r;
		
		r.x = Xcenter; r.y = y+1;
		r.w = dx; r.h = corner-1;
		SDL_FillRect(surface, &r, brush_col);

		r.x = x+1; r.y = Y2center;
		r.w = width-corner-1; r.h = corner-1;
		SDL_FillRect(surface, &r, brush_col);
	}
// ------- END OF Rechteck oben und unten ------

	Uint32 pen_col = (Uint32)(*pen.GetColor());
	Uint8 colorbyte0 = (Uint8) (pen_col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((pen_col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((pen_col >> 16) & 0xff);

// ------ TOP AND LOWER HORIZONTAL LINE ------
	Lock();
	{
		register Uint8* p;
		register int k;
		p = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + x * 3;k = dx+corner;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + y * surface->pitch + Xcenter * 3;k = dx;SDL_DO_DRAWING_X
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

	
// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		register Uint8* p;
		register int k;
		
		p = (Uint8*)surface->pixels + Ycenter * surface->pitch + x * 3;k = height-corner-1;SDL_DO_DRAWING_Y
		p = (Uint8*)surface->pixels + Ycenter * surface->pitch + (x+width) * 3;k = dy;SDL_DO_DRAWING_Y
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------

// Halbkreis
	//???
	{
		int d = 3 - (corner-1)*2;
		int rightInc = 6;
		int diagonalInc = 10 - (corner-1)*4;
		unsigned int radius = corner-1;
	
		colorbyte0 = (Uint8) (brush_col & 0xff);
		colorbyte1 = (Uint8) ((brush_col >> 8) & 0xff);
		colorbyte2 = (Uint8) ((brush_col >> 16) & 0xff);

		unsigned int i = 0;
		while (i < radius)
		{
			register Uint8 *p;
			register Sint16 k;
			p = (Uint8*)surface->pixels + (Ycenter-radius) * surface->pitch + (Xcenter-i) * 3;k = i;SDL_DO_DRAWING_X
			p = (Uint8*)surface->pixels + (Ycenter-radius) * surface->pitch + (X2center) * 3;k = i;SDL_DO_DRAWING_X
			p = (Uint8*)surface->pixels + (Y2center+radius-1) * surface->pitch + (X2center) * 3;k = i;SDL_DO_DRAWING_X
		
			p = (Uint8*)surface->pixels + (Ycenter-i) * surface->pitch + (Xcenter-radius) * 3;k = radius;SDL_DO_DRAWING_X
			p = (Uint8*)surface->pixels + (Ycenter-i) * surface->pitch + (X2center) * 3;k = radius;SDL_DO_DRAWING_X
			p = (Uint8*)surface->pixels + (Y2center+i-1) * surface->pitch + (X2center) * 3;k = radius;SDL_DO_DRAWING_X

			if (d >= 0) 
			{
				d += diagonalInc;
				diagonalInc += 8;
				--radius;
			} else 
			{
				d += rightInc;
				diagonalInc += 4;
			}
			rightInc += 4;
			++i;
		}
	}
	{
		int d = 3 - (corner*2);
		int rightInc = 6;
		int diagonalInc = 10 - (corner*4);
		unsigned int radius = corner;
		colorbyte0 = (Uint8) (pen_col & 0xff);
		colorbyte1 = (Uint8) ((pen_col >> 8) & 0xff);
		colorbyte2 = (Uint8) ((pen_col >> 16) & 0xff);
		register Uint8 *p;
	
		unsigned int i = 0;
		while (i <= radius) 
		{
			p = (Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (Xcenter - i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
			p = (Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (Xcenter - radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
			p = (Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (X2center + i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
			p = (Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (X2center + radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
			p = (Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + (X2center + i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
				p = (Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + (X2center + radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
			if (d >= 0) 
			{
				d += diagonalInc;
				diagonalInc += 8;
				--radius;
			} else 
			{
				d += rightInc;
				diagonalInc += 4;
			}
			rightInc += 4;
			++i;
		}
	}
	Unlock();

}/*DrawFilledEdgedBorderRound*/


void DC::DrawFilledEdgedBorderRound_32bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	// Kantenlaengen
	int dx = width - (corner*2);
	int dy = height - (corner*2);

	// Ecke links oben
	int Xcenter = x + corner;
	int Ycenter = y + corner;

	// Ecke rechts unten
	int X2center = Xcenter + dx;
	int Y2center = Ycenter + dy;

// Rechteck von linker Kante nach rechter Kante
	Uint32 brush_col = (Uint32)(*brush.GetColor());
	
	if(dy > 0)
	{
		SDL_Rect r;
		r.x = x+1; r.y = Ycenter;
		r.w = width-1; r.h = dy;
		SDL_FillRect(surface, &r, brush_col);
	}

// ------ Rechteck entlang der oberen und entlang der unteren Kante ------
	if((corner > 0)&&(dx>0))
	{
		SDL_Rect r;
		
		r.x = Xcenter; r.y = y+1;
		r.w = dx; r.h = corner-1;
		SDL_FillRect(surface, &r, brush_col);

		r.x = x+1; r.y = Y2center;
		r.w = width-corner-1; r.h = corner-1;
		SDL_FillRect(surface, &r, brush_col);
	}
// ------- END OF Rechteck oben und unten ------


// ------ TOP AND LOWER HORIZONTAL LINE ------
	Uint32 pen_col = (Uint32)(*pen.GetColor());
	Lock();
	if (sizeof(wchar_t) == sizeof(Uint32)) { 
#ifdef __linux__
		wmemset((wchar_t*)( (Uint8*)surface->pixels +  y * surface->pitch + (Xcenter*4)), pen_col, dx);
		wmemset((wchar_t*)( (Uint8*)surface->pixels +  (y+height-1) * surface->pitch + (x*4)), pen_col, width - corner);
#elif __WIN32__
		memset((wchar_t*)( (Uint8*)surface->pixels +  y * surface->pitch + (Xcenter*4)), pen_col, dx);
		memset((wchar_t*)( (Uint8*)surface->pixels +  (y+height-1) * surface->pitch + (x*4)), pen_col, width - corner);
#endif
	} else 
	{
		register Uint8* p0 = (Uint8*)surface->pixels + y * surface->pitch + (Xcenter*4);
		register Uint8* p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + (x*4);
		register Sint16 i = corner;
		for(;i--;p1+=4)
			*(Uint32*)p1 = pen_col;
		i = dx;
		for(;i--;p0+=4,p1+=4)
		{
			*(Uint32*)p0 = pen_col;
			*(Uint32*)p1 = pen_col;
		}
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

	
// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		if (height<3)	
			return;
		register Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x*4);	 
		register Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + ((x+width)*4);
		register Sint16 i = dy;
		for(;i--;p0+=surface->pitch, p1+=surface->pitch)
		{
			*(Uint32*)p0 = pen_col;
			*(Uint32*)p1 = pen_col;
		}
		i = corner-1;
		for(;i--;p0+=surface->pitch)
			*(Uint32*)p0 = pen_col;	
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------

// Halbkreis
	//???
	{
		int d = 3 - (corner*2);
		int rightInc = 6;
		int diagonalInc = 10 - (corner*4);
		unsigned int radius = corner;
	
		unsigned int i = 0;
		while (i <= radius) 
		{
			for(unsigned int k=i;k--;)
			{
				*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (k + Xcenter - i)*4)) = brush_col;
				*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (k + X2center)*4)) = brush_col;
				*((Uint32*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + (k + X2center)*4)) = brush_col;
			}
			for(unsigned int k=radius;k--;)
			{
				*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (k + Xcenter - radius)*4)) = brush_col;
				*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (k + X2center)*4)) = brush_col;
				*((Uint32*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + (k + X2center)*4)) = brush_col;
			}
			if (d >= 0) 
			{
				d += diagonalInc;
				diagonalInc += 8;
				--radius;
			} else 
			{
				d += rightInc;
				diagonalInc += 4;
			}
			rightInc += 4;
			++i;
		}
	}
	{
		int d = 3 - (corner*2);
		int rightInc = 6;
		int diagonalInc = 10 - (corner*4);
		unsigned int radius = corner;

		unsigned int i = 0;
		while (i <= radius) 
		{	
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((Xcenter - i)*4))) = pen_col;
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((Xcenter - radius)*4))) = pen_col;

			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((X2center + i)*4))) = pen_col;
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((X2center + radius)*4))) = pen_col;

			*((Uint32*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + ((X2center + i)*4))) = pen_col;
			*((Uint32*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + ((X2center + radius)*4))) = pen_col;

			if (d >= 0) 
			{
				d += diagonalInc;
				diagonalInc += 8;
				--radius;
			} else 
			{
				d += rightInc;
				diagonalInc += 4;
			}
			rightInc += 4;
			++i;
		}
	}
	Unlock();

}/*DrawFilledEdgedBorderRound*/

void DC::DrawEmptyRound_8bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	if (width==0 || height==0)
		return;
	unsigned int radius = corner;
	Uint32 col = (Uint32)(*pen.GetColor());
	register Sint16 i;
	Sint16 rightInc = 6;
	
	/*TODO: We can do better :-)*/
	Sint16 d = width<height ? width : height;
//	d = 3 - (radius*2); WTF?
	Sint16 diagonalInc = 10 - (radius*4);

	/*Rectangles*/
	Sint16 dx = width - (radius*2);
	Sint16 Xcenter = x+radius;
	Sint16 dy = height - (radius*2);
	Sint16 Ycenter = y+radius;

	/*Centers*/
	Sint16 X2center=Xcenter+dx-1;
	Sint16 Y2center=Ycenter+dy-1;

	register Uint8* p0 = (Uint8*)surface->pixels +	y	* surface->pitch + Xcenter;
	register Uint8* p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + Xcenter;

	Lock();
	i=dx;				
	switch( i % 4 ) { 
		do{			
			case 0:	 
				*p0 = col; p0++;		 
				*p1 = col; p1++;		
			case 3:								
				*p0 = col; p0++;		
				*p1 = col; p1++;	 
			case 2:							 
				*p0 = col; p0++;	 
				*p1 = col; p1++;
			case 1:						
				*p0 = col; p0++;
				*p1 = col; p1++;
		}while( (i-=4) > 0 );			
	}											 
	if (height<3)	
		return; 
	p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + x;
	p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x+width-1);
	i=dy;										
	switch( i % 4 ) {							 
		do{										
			case 0:								 
				*p0 = col; p0+=surface->pitch;
				*p1 = col; p1+=surface->pitch;
			case 3:								 
				*p0 = col; p0+=surface->pitch;
				*p1 = col; p1+=surface->pitch;
			case 2:								 
				*p0 = col; p0+=surface->pitch;
				*p1 = col; p1+=surface->pitch;
			case 1:								 
				*p0 = col; p0+=surface->pitch;
				*p1 = col; p1+=surface->pitch;
		}while( (i-=4) > 0 );					 
	}
	
	i = 0;

	while (i < radius) 
	{
		*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (Xcenter - radius))) = col;
		*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (Xcenter - i))) = col;

		*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (X2center + i))) = col;
		*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (X2center + radius))) = col;

		*((Uint8*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + (X2center + i))) = col;
		*((Uint8*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (X2center + radius))) = col;

		*((Uint8*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + (Xcenter - i))) = col;
		*((Uint8*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (Xcenter - radius))) = col;

		if (d >= 0) {
			d += diagonalInc;
			diagonalInc += 8;
			--radius;
		} else {
			d += rightInc;
			diagonalInc += 4;
		}
		rightInc += 4;
		++i;
	}/*while*/
	Unlock();

}/*Draw_Round*/

void DC::DrawEmptyRound_16bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	unsigned int radius = corner;
	Uint32 col = (Uint32)(*pen.GetColor());

	register Uint8 *p0;
	register Uint8 *p1;
	register Sint16 i;
	Sint16 dx, dy;
	Sint16 Xcenter, Ycenter, X2center, Y2center;

	Sint16 rightInc = 6;
	Sint16 d, diagonalInc;

	if (width==0 || height==0)
		return;

	/*TODO: We can do better :-)*/
	d = width<height ? width : height;
//	d = 3 - (radius*2); WTF?
	diagonalInc = 10 - (radius*4);

	/*Rectangles*/
	dx = width - (radius*2);
	Xcenter = x+radius;
	dy = height - (radius*2);
	Ycenter = y+radius;

	/*Centers*/
	X2center=Xcenter+dx-1;
	Y2center=Ycenter+dy-1;

	p0 = (Uint8*)surface->pixels +	y	* surface->pitch + Xcenter*2;
	p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + Xcenter*2;

	Lock();

	i=dx;				
	switch( i % 4 ) { 
		do{			
			case 0:	 
				*(Uint16*)p0 = col; p0+=2;
				*(Uint16*)p1 = col; p1+=2;		
			case 3:								
				*(Uint16*)p0 = col; p0+=2;		
				*(Uint16*)p1 = col; p1+=2;	 
			case 2:							 
				*(Uint16*)p0 = col; p0+=2;	 
				*(Uint16*)p1 = col; p1+=2;
			case 1:						
				*(Uint16*)p0 = col; p0+=2;
				*(Uint16*)p1 = col; p1+=2;
		}while( (i-=4) > 0 );			
	}											 
	if (height<3)	
		return; 
	p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + x*2;	 
	p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x+width-1)*2;
	i=dy;										
	switch( i % 4 ) {							 
		do{										
			case 0:								 
				*(Uint16*)p0 = col; p0+=surface->pitch;
				*(Uint16*)p1 = col; p1+=surface->pitch;
			case 3:								 
				*(Uint16*)p0 = col; p0+=surface->pitch;
				*(Uint16*)p1 = col; p1+=surface->pitch;
			case 2:								 
				*(Uint16*)p0 = col; p0+=surface->pitch;
				*(Uint16*)p1 = col; p1+=surface->pitch;
			case 1:								 
				*(Uint16*)p0 = col; p0+=surface->pitch;
				*(Uint16*)p1 = col; p1+=surface->pitch;
		}while( (i-=4) > 0 );					 
	}


	i = 0;
	while (i < (signed int)radius)
	{
		*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (Xcenter - radius)*2)) = col;
		*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (Xcenter - i)*2)) = col;
		*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (X2center + i)*2)) = col;
		*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (X2center + radius)*2)) = col;
		*((Uint16*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + (X2center + i)*2)) = col;
		*((Uint16*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (X2center + radius)*2)) = col;
		*((Uint16*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + (Xcenter - i)*2)) = col;
		*((Uint16*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (Xcenter - radius)*2)) = col;
		if (d >= 0) {
			d += diagonalInc;
			diagonalInc += 8;
			--radius;
		} else {
			d += rightInc;
			diagonalInc += 4;
		}
		rightInc += 4;
		++i;
	}
	Unlock();

}

void DC::DrawEmptyRound_24bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	unsigned int radius = corner;
	Uint32 col = (Uint32)(*pen.GetColor());
	Uint8 colorbyte0 = (Uint8) (col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((col >> 16) & 0xff);
	Sint16 dx, dy;
	Sint16 Xcenter, Ycenter, X2center, Y2center;

	Sint16 rightInc = 6;
	Sint16 d, diagonalInc;

	if (width==0 || height==0)
		return;

/*TODO: We can do better :-)*/
	d = width<height ? width : height;
//	d = 3 - (radius*2); WTF?
	diagonalInc = 10 - (radius*4);

	/*Rectangles*/
	dx = width - (radius*2);
	Xcenter = x+radius;
	dy = height - (radius*2);
	Ycenter = y+radius;

	/*Centers*/
	X2center=Xcenter+dx-1;
	Y2center=Ycenter+dy-1;

// ------ TOP AND LOWER HORIZONTAL LINE ------
	Lock();
	{
		register Uint8* p;
		register int k;
		p = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + x * 3;k = dx+corner-1;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + y * surface->pitch + Xcenter * 3;k = dx;SDL_DO_DRAWING_X
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

	
// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		register Uint8* p;
		register int k;
		
		p = (Uint8*)surface->pixels + Ycenter * surface->pitch + x * 3;k = corner+dy-1;SDL_DO_DRAWING_Y
		p = (Uint8*)surface->pixels + Ycenter * surface->pitch + (x+width) * 3;k = dy;SDL_DO_DRAWING_Y
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------

	unsigned int i = 0; //TODO

	register Uint8* p;

	while (i < radius) 
	{
		p = (Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (Xcenter - i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (Xcenter - radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (X2center + i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (X2center + radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + (Xcenter + i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (Xcenter + radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + (X2center - i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (X2center - radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX

		if (d >= 0) {
			d += diagonalInc;
			diagonalInc += 8;
			--radius;
		} else {
			d += rightInc;
			diagonalInc += 4;
		}
		rightInc += 4;
		++i;
	}/*while*/
	Unlock();
}/*Draw_Round*/

void DC::DrawEmptyRound_32bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	unsigned int radius = corner;
	Uint32 col = (Uint32)(*pen.GetColor());

	register Uint8 *p0;
	register Uint8 *p1;
	register Sint16 i;
	Sint16 dx, dy;
	Sint16 Xcenter, Ycenter, X2center, Y2center;

	Sint16 rightInc = 6;
	Sint16 d, diagonalInc;

	if (width==0 || height==0)
		return;

	/*TODO: We can do better :-)*/
	d = width<height ? width : height;
//	d = 3 - (radius*2); WTF?
	diagonalInc = 10 - (radius*4);

	/*Rectangles*/
	dx = width - (radius*2);
	Xcenter = x+radius;
	dy = height - (radius*2);
	Ycenter = y+radius;

	/*Centers*/
	X2center=Xcenter+dx-1;
	Y2center=Ycenter+dy-1;

	p0 = (Uint8*)surface->pixels +	y	* surface->pitch + Xcenter*4;
	p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + Xcenter*4;

	Lock();

SDL_DRAW_WMEMSET_START
	i=dx;				
	switch( i % 4 ) { 
		do{			
			case 0:	 
				*(Uint32*)p0 = col; p0+=4;		 
				*(Uint32*)p1 = col; p1+=4;		
			case 3:								
				*(Uint32*)p0 = col; p0+=4;		
				*(Uint32*)p1 = col; p1+=4;	 
			case 2:							 
				*(Uint32*)p0 = col; p0+=4;	 
				*(Uint32*)p1 = col; p1+=4;
			case 1:						
				*(Uint32*)p0 = col; p0+=4;
				*(Uint32*)p1 = col; p1+=4;
		}while( (i-=4) > 0 );			
	}											 
SDL_DRAW_WMEMSET_END						 
	if (height<3)	
		return; 
	p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + x*4;	 
	p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x+width-1)*4;
	i=dy;										
	switch( i % 4 ) {							 
		do{										
			case 0:								 
				*(Uint32*)p0 = col; p0+=surface->pitch;
				*(Uint32*)p1 = col; p1+=surface->pitch;
			case 3:								 
				*(Uint32*)p0 = col; p0+=surface->pitch;
				*(Uint32*)p1 = col; p1+=surface->pitch;
			case 2:								 
				*(Uint32*)p0 = col; p0+=surface->pitch;
				*(Uint32*)p1 = col; p1+=surface->pitch;
			case 1:								 
				*(Uint32*)p0 = col; p0+=surface->pitch;
				*(Uint32*)p1 = col; p1+=surface->pitch;
		}while( (i-=4) > 0 );					 
	}
	
/*	unsigned int */i = 0; //TODO

	while (i < ((signed int)(radius))) 
	{
		*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (Xcenter - radius)*4)) = col;
		*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (Xcenter - i)*4)) = col;

		*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (X2center + i)*4)) = col;
		*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (X2center + radius)*4)) = col;

		*((Uint32*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + (X2center + i)*4)) = col;
		*((Uint32*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (X2center + radius)*4)) = col;

		*((Uint32*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + (Xcenter - i)*4)) = col;
		*((Uint32*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (Xcenter - radius)*4)) = col;

		if (d >= 0) {
			d += diagonalInc;
			diagonalInc += 8;
			--radius;
		} else {
			d += rightInc;
			diagonalInc += 4;
		}
		rightInc += 4;
		++i;
	}/*while*/
	Unlock();

}/*Draw_Round*/



