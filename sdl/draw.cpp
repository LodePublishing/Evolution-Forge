#include "dc.hpp"

#include "string.h"
#include "wchar.h"

// TODO: 1 Funktion fuer a) jede Aufloesung und b) jede Farbtiefe!
// => static machen!

void DC::Draw_VLine(const signed int x0, const signed int y0, const signed int y1) const
{
	if (y0 > y1)	
		return;
//		{ i=y1; y1=y0; y0=i; } // TODO?

	register Uint8 *p = (Uint8*)surface->pixels + y0 * surface->pitch + x0 * SDL_DRAW_BPP;
	register Sint16 i = y1-y0+1;
	Uint32 pen_col = (Uint32)(*pen.GetColor());
	Lock();
#if SDL_DRAW_BPP == 1
	switch( i % 4 ) {					
		do{								 
			case 0: *p = pen_col; p+=surface->pitch;
			case 3: *p = pen_col; p+=surface->pitch;
			case 2: *p = pen_col; p+=surface->pitch;
			case 1: *p = pen_col; p+=surface->pitch;
		}while( (i-=4) > 0 );				
	}
#elif SDL_DRAW_BPP == 2
	 switch( i % 4 ) {								 
		do{											
			case 0: *(Uint16*)p = pen_col; p+=surface->pitch;
			case 3: *(Uint16*)p = pen_col; p+=surface->pitch;
			case 2: *(Uint16*)p = pen_col; p+=surface->pitch;
			case 1: *(Uint16*)p = pen_col; p+=surface->pitch;
		}while( (i-=4) > 0 );
	}

#elif SDL_DRAW_BPP == 3
	Uint8 colorbyte0 = (Uint8) (pen_col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((pen_col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((pen_col >> 16) & 0xff);

	if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {		
		switch( i % 4 ) {										 
			do{												 
				case 0:p[0] = colorbyte2; p[1] = colorbyte1; p[2] = colorbyte0; p+=surface->pitch;
				case 3:p[0] = colorbyte2; p[1] = colorbyte1; p[2] = colorbyte0; p+=surface->pitch;
				case 2:p[0] = colorbyte2; p[1] = colorbyte1; p[2] = colorbyte0; p+=surface->pitch;
				case 1:p[0] = colorbyte2; p[1] = colorbyte1; p[2] = colorbyte0; p+=surface->pitch;
			}while( (i-=4) > 0 );
		}
	} else {
		switch( i % 4 ) {
			do{
				case 0:p[0] = colorbyte0; p[1] = colorbyte1; p[2] = colorbyte2; p+=surface->pitch;
				case 3:p[0] = colorbyte0; p[1] = colorbyte1; p[2] = colorbyte2; p+=surface->pitch;
				case 2:p[0] = colorbyte0; p[1] = colorbyte1; p[2] = colorbyte2; p+=surface->pitch;
				case 1:p[0] = colorbyte0; p[1] = colorbyte1; p[2] = colorbyte2; p+=surface->pitch;
			}while( (i-=4) > 0 );
		}
	}
#elif SDL_DRAW_BPP == 4
	// ------ LEFT AND RIGHT VERTICAL LINE ------
	for(;i--; p+=surface->pitch)
		*(Uint32*)p = pen_col;
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------
#endif
	Unlock();

}/*Draw_VLine*/
#if 0
void DC::Draw_HLine_8bit(const signed int x0, const signed int y0, const signed int x1) const
{
	if (x0 > x1)	
		return; // TODO?
	Uint32 col = (Uint32)(*pen.GetColor());
	register Uint8 *p;
	p = (Uint8*)surface->pixels + y0 * surface->pitch + x0 * SDL_DRAW_BPP;
	memset(p, col, x1-x0+1);
}/*Draw_HLine*/

void DC::Draw_HLine_16bit(const signed int x0, const signed int y0, const signed int x1) const
{
	if (x0 > x1)	
		return; // TODO?
	Uint32 col = (Uint32)(*pen.GetColor());
	register Uint8 *p;
	register Sint16 i;
	p = (Uint8*)surface->pixels + y0 * surface->pitch + x0 * SDL_DRAW_BPP;

	i = x1-x0+1;						 
	switch( i % 4 ) {					 
		do{								
			case 0: *(Uint16*)p = col; p+=2;
			case 3: *(Uint16*)p = col; p+=2; 
			case 2: *(Uint16*)p = col; p+=2;
			case 1: *(Uint16*)p = col; p+=2;
		}while( (i-=4) > 0 );				
	}
}/*Draw_HLine*/

void DC::Draw_HLine_24bit(const signed int x0, const signed int y0, const signed int x1) const
{
	if (x0 > x1)	
		return; // TODO?
//		{ i=x1; x1=x0; x0=i; }
	Uint32 col = (Uint32)(*pen.GetColor());
	Uint8 colorbyte0 = (Uint8) (col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((col >> 16) & 0xff);
	register Uint8 *p;
	register Sint16 i;

	p = (Uint8*)surface->pixels + y0 * surface->pitch + x0 * SDL_DRAW_BPP;

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
	i = x1-x0+1;									
	switch( i % 4 ) {							
		do{										 
			case 0: SDL_DRAW_PUTPIXEL_BPP_3_AUX	p+=3;
			case 3: SDL_DRAW_PUTPIXEL_BPP_3_AUX	p+=3;
			case 2: SDL_DRAW_PUTPIXEL_BPP_3_AUX	p+=3;
			case 1: SDL_DRAW_PUTPIXEL_BPP_3_AUX	p+=3;
		}while( (i-=4) > 0 );						
	}
}/*Draw_HLine*/
#endif

void DC::Draw_HLine(const signed int x0, const signed int y0, const signed int x1) const
{
	if (x0 > x1)	
		return;
// ------ TOP AND LOWER HORIZONTAL LINE ------
	Uint32 pen_col = (Uint32)(*pen.GetColor());
	Lock();
	if (sizeof(wchar_t) == sizeof(Uint32)) { 
#ifdef __linux__
		wmemset((wchar_t*)( (Uint8*)surface->pixels +  y0 * surface->pitch + (x0<<2)), pen_col, x1-x0);
#elif __WIN32__
		memset((wchar_t*)( (Uint8*)surface->pixels +  y0 * surface->pitch + (x0<<2)), pen_col, x1-x0);
#endif
	} else 
	{
		register Uint8* p = (Uint8*)surface->pixels + y0 * surface->pitch + (x0<<2);
		register Sint16 i = x1-x0;
		for(;i--;p+=4)
			*(Uint32*)p = pen_col;
	}
	Unlock();
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------
}/*Draw_HLine*/


#define sign(a) (((a)<0) ? -1 : (a)>0 ? 1 : 0)

void DC::Draw_Line(signed int x1, signed int y1, signed int x2, signed int y2) const
{
	if(y1 == y2)
	{
		Draw_HLine(x1, y1, x2);
		return;
	}
	if(x1 == x2)
	{
		Draw_VLine(x1, y1, y2);
		return;
	}
		
	Color col = (*pen.GetColor());
	

	int d;
	int x;
	int y;
	int ax;
	int ay;
	int sx;
	int sy;
	int dx;
	int dy;

	Uint8 *lineAddr;
	Sint32 yOffset;

	dx = x2 - x1;	
	ax = abs(dx) << 1;	
	sx = sign(dx);

	dy = y2 - y1;	
	ay = abs(dy) << 1;	
	sy = sign(dy);
	yOffset = sy * surface->pitch;

	x = x1;
	y = y1;

	lineAddr = ((Uint8 *)(surface->pixels)) + (y * surface->pitch);
	Lock();
	if (ax>ay)
	{						/* x dominant */
		d = ay - (ax >> 1);
		for (;;)
		{
			*((Uint32 *)(lineAddr + (x << 2))) = (Uint32)col;

			if (x == x2)
				return;
			if (d>=0)
			{
				y += sy;
				lineAddr += yOffset;
				d -= ax;
			}
			x += sx;
			d += ay;
		}
	}
	else
	{						/* y dominant */
		d = ax - (ay >> 1);
		for (;;)
		{
			*((Uint32 *)(lineAddr + (x << 2))) = (Uint32)col;

			if (y == y2)
				return;
			if (d>=0) 
			{
				x += sx;
				d -= ay;
			}
			y += sy;
			lineAddr += yOffset;
			d += ax;
		}
	}
	Unlock();
}

void DC::DrawFilledRound(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	unsigned int radius = corner;
	Uint32 col = (Uint32)(*brush.GetColor());

	int rightInc = 6;
	int d = width<height ? width : height;

	d = 3 - (radius<<1);
	int diagonalInc = 10 - (radius<<2);
 	int Xcenter = x+radius;
	int Ycenter = y+radius;
	int dx = width - (radius<<1);
	int dy = height - (radius<<1);
	int X2center=Xcenter + dx;
	int Y2center=Ycenter + dy;

// FILLED
	SDL_Rect r;
//	r.x = x; r.y = Ycenter;
//	r.w = width; r.h = dy;
//	SDL_FillRect(surface, &r, col);

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
	Lock();
	unsigned int i = 0;
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
		for(int k=i;k--;)
		{
			*((Uint32*)((Uint8*)surface->pixels + k+ (Ycenter-radius)*surface->pitch + (Xcenter - i)*4)) = col;
			*((Uint32*)((Uint8*)surface->pixels + k+(Ycenter-radius)*surface->pitch + X2center*4)) = col;
			*((Uint32*)((Uint8*)surface->pixels + k+(Y2center+radius-1)*surface->pitch + X2center*4)) = col;
			*((Uint32*)((Uint8*)surface->pixels + k+(Y2center+radius-1)*surface->pitch + (Xcenter-i)*4)) = col;

		}
		for(int k=radius;k--;)
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

}/*Draw_FillRound*/

void DC::DrawFilledEdgedRound(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	unsigned int radius = corner;
	Uint32 col = (Uint32)(*brush.GetColor());
	SDL_Rect r;
	Sint16 rightInc = 6;
	Sint16 d, diagonalInc;

	/*TODO: We can do better :-)*/
	d = width<height ? width : height;
	d = 3 - (radius<<1);
	diagonalInc = 10 - (radius<<2);

	// Kantenlaengen
	int dx = width - (radius<<1);
	int dy = height - (radius<<1);

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

	Lock();
// Halbkreis
	int i = 0;
	while (i <= (signed int)radius) 
	{
// Nur Ecken:
/*#ifdef __linux__
	// links oben
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (Xcenter - i)*4), col, i);
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (Xcenter - radius)*4), col, radius);
	// rechts oben	
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + X2center*4), col, i);
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + X2center*4), col, radius);
	// links unten
//		wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + (Xcenter - i)*4), col, i);
//		wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (Xcenter - radius)*4), col, radius);
	// rechts unten
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + X2center*4), col, i);
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + X2center*4), col, radius);
#else
//if __WIN32__*/
/*	// links oben
		memset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (Xcenter - i)*4), col, i);
		memset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (Xcenter - radius)*4), col, radius);
	// rechts oben
		memset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + X2center*4), col, i);
		memset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + X2center*4), col, radius);
	// rechts unten
		memset((wchar_t*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + X2center*4), col, i);
		memset((wchar_t*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + X2center*4), col, radius);*/
		
		for(int k=i;k--;)
		{
			*((Uint32*)((Uint8*)surface->pixels + k*4 + (Ycenter-radius)*surface->pitch + (Xcenter - i)*4)) = col;
			*((Uint32*)((Uint8*)surface->pixels + k*4 + (Ycenter-radius)*surface->pitch + X2center*4)) = col;
			*((Uint32*)((Uint8*)surface->pixels + k*4 + (Y2center+radius-1)*surface->pitch + X2center*4)) = col;
		}
		for(int k=radius;k--;)
		{
			*((Uint32*)((Uint8*)surface->pixels + k*4 + (Ycenter-i)*surface->pitch + (Xcenter - radius)*4)) = col;
			*((Uint32*)((Uint8*)surface->pixels + k*4 + (Ycenter-i)*surface->pitch + X2center*4)) = col;
			*((Uint32*)((Uint8*)surface->pixels + k*4 + (Y2center+i-1)*surface->pitch + X2center*4)) = col;
		}
//#endif





// Ganze Zeilen:
	// links oben
/*		wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (Xcenter - i)*4), col, dx + 2*i);
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (Xcenter - radius)*4), col, dx + 2*radius);
	// rechts oben
//		wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + X2center*4), col, i);
//		wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + X2center*4), col, radius);
	// links unten
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + (Xcenter - i)*4), col, dx + 2*i);
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (Xcenter - radius)*4), col, dx + 2*radius);
	// rechts unten
//		wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + X2center*4), col, i);
	//		wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + X2center*4), col, radius);*/

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


void DC::DrawEmptyEdgedRound(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	int dx = width - (corner<<1);
	int dy = height - (corner<<1);
	
	int Xcenter = x + corner;
	int Ycenter = y + corner;

	int X2center = Xcenter + dx;
	int Y2center = Ycenter + dy;

// ------ TOP AND LOWER HORIZONTAL LINE ------
	Uint32 pen_col = (Uint32)(*pen.GetColor());
	Lock();
	if (sizeof(wchar_t) == sizeof(Uint32)) { 
#ifdef __linux__
		wmemset((wchar_t*)( (Uint8*)surface->pixels +  y * surface->pitch + (Xcenter<<2)), pen_col, dx);
		wmemset((wchar_t*)( (Uint8*)surface->pixels +  (y+height-1) * surface->pitch + (x<<2)), pen_col, width - corner);
#elif __WIN32__
		memset((wchar_t*)( (Uint8*)surface->pixels +  y * surface->pitch + (Xcenter<<2)), pen_col, dx);
		memset((wchar_t*)( (Uint8*)surface->pixels +  (y+height-1) * surface->pitch + (x<<2)), pen_col, width - corner);
#endif
	} else 
	{
		register Uint8* p0 = (Uint8*)surface->pixels + y * surface->pitch + (Xcenter<<2);
		register Uint8* p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + (x<<2);
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
		register Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x<<2);
		register Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + ((x+width)<<2);
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

	int i = 0;	
	int rightInc = 6;
	int d = 3 - (corner<<1);
	int diagonalInc = 10 - (corner<<2);
	unsigned int radius = corner;
	
	while (i <= ((signed int)(radius))) 
	{
		*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((Xcenter - i)<<2))) = pen_col;
		*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((Xcenter - radius)<<2))) = pen_col;

		*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((X2center + i)<<2))) = pen_col;
		*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((X2center + radius)<<2))) = pen_col;

		*((Uint32*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + ((Xcenter - corner)<<2))) = pen_col;
		*((Uint32*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + ((Xcenter - corner)<<2))) = pen_col;

		*((Uint32*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + ((X2center + i)<<2))) = pen_col;
		*((Uint32*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + ((X2center + radius)<<2))) = pen_col;

		*((Uint32*)((Uint8*)surface->pixels + (Y2center+corner-1)*surface->pitch + ((Xcenter - radius)<<2))) = pen_col; // new
		*((Uint32*)((Uint8*)surface->pixels + (Y2center+corner-1)*surface->pitch + ((Xcenter - i)<<2))) = pen_col; // new

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

void DC::DrawFilledEdgedBorderRound(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	// Kantenlaengen
	int dx = width - (corner<<1);
	int dy = height - (corner<<1);

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
		wmemset((wchar_t*)( (Uint8*)surface->pixels +  y * surface->pitch + (Xcenter<<2)), pen_col, dx);
		wmemset((wchar_t*)( (Uint8*)surface->pixels +  (y+height-1) * surface->pitch + (x<<2)), pen_col, width - corner);
#elif __WIN32__
		memset((wchar_t*)( (Uint8*)surface->pixels +  y * surface->pitch + (Xcenter<<2)), pen_col, dx);
		memset((wchar_t*)( (Uint8*)surface->pixels +  (y+height-1) * surface->pitch + (x<<2)), pen_col, width - corner);
#endif
	} else 
	{
		register Uint8* p0 = (Uint8*)surface->pixels + y * surface->pitch + (Xcenter<<2);
		register Uint8* p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + (x<<2);
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
		register Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x<<2);	 
		register Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + ((x+width)<<2);
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
	int d = 3 - (corner<<1);
	int i = 0;
	int rightInc = 6;
	int diagonalInc = 10 - (corner<<2);
	unsigned int radius = corner;
	
	while (i <= (signed int)radius) 
	{
		for(int k=i;k--;)
		{
			*((Uint32*)((Uint8*)surface->pixels + (k<<2) + (Ycenter-radius)*surface->pitch + ((Xcenter - i)<<2))) = brush_col;
			*((Uint32*)((Uint8*)surface->pixels + (k<<2) + (Ycenter-radius)*surface->pitch + ((X2center)<<2))) = brush_col;
			*((Uint32*)((Uint8*)surface->pixels + (k<<2) + (Y2center+radius-1)*surface->pitch + (X2center<<2))) = brush_col;
		}
		for(int k=radius;k--;)
		{
			*((Uint32*)((Uint8*)surface->pixels + (k<<2) + (Ycenter-i)*surface->pitch + ((Xcenter - radius)<<2))) = brush_col;
			*((Uint32*)((Uint8*)surface->pixels + (k<<2) + (Ycenter-i)*surface->pitch + (X2center<<2))) = brush_col;
			*((Uint32*)((Uint8*)surface->pixels + (k<<2) + (Y2center+i-1)*surface->pitch + (X2center<<2))) = brush_col;
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
	int d = 3 - (corner<<1);
	int i = 0;
	int rightInc = 6;
	int diagonalInc = 10 - (corner<<2);
	unsigned int radius = corner;

	while (i <= (signed int)radius) 
	{
                *((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((Xcenter - i)<<2))) = pen_col;
                *((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((Xcenter - radius)<<2))) = pen_col;

                *((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((X2center + i)<<2))) = pen_col;
                *((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((X2center + radius)<<2))) = pen_col;

//                *((Uint32*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + (Xcenter - corner)*4)) = pen_col;
  //              *((Uint32*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (Xcenter - corner)*4)) = pen_col;

                *((Uint32*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + ((X2center + i)<<2))) = pen_col;
                *((Uint32*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + ((X2center + radius)<<2))) = pen_col;

//                *((Uint32*)((Uint8*)surface->pixels + (Y2center+corner-1)*surface->pitch + (Xcenter - radius)*4)) = pen_col; // new
  //              *((Uint32*)((Uint8*)surface->pixels + (Y2center+corner-1)*surface->pitch + (Xcenter - i)*4)) = pen_col; // new

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


void DC::DrawEmptyRound(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
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
//	d = 3 - (radius<<1); WTF?
	diagonalInc = 10 - (radius<<2);

	/*Rectangles*/
	dx = width - (radius<<1);
	Xcenter = x+radius;
	dy = height - (radius<<1);
	Ycenter = y+radius;

	/*Centers*/
	X2center=Xcenter+dx-1;
	Y2center=Ycenter+dy-1;

	p0 = (Uint8*)surface->pixels +	y	* surface->pitch + Xcenter*SDL_DRAW_BPP;
	p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + Xcenter*SDL_DRAW_BPP;

	Lock();
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



