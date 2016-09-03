#include "dc.hpp"

#include "string.h"
#include "wchar.h"

#define SDL_DRAW_BPP 2

void DC::Draw_VLine(const signed int x0, const signed int y0, const signed int y1) const
{
	if (y0 > y1)	
		return;
//		{ i=y1; y1=y0; y0=i; } // TODO?

	register Uint8 *p;
	register Sint16 i;
	p = (Uint8*)surface->pixels + y0 * surface->pitch + x0 * SDL_DRAW_BPP;
	i = y1-y0+1;

	Uint32 col = (Uint32)(*pen.GetColor());

#if SDL_DRAW_BPP == 1
	switch( i % 4 ) {					
	do{								 
		case 0: *p = col; p+=surface->pitch;
		case 3: *p = col; p+=surface->pitch;
		case 2: *p = col; p+=surface->pitch;
		case 1: *p = col; p+=surface->pitch;
	}while( (i-=4) > 0 );				
	}
#elif SDL_DRAW_BPP == 2
	 switch( i % 4 ) {								 
	do{											
		case 0: *(Uint16*)p = col; p+=surface->pitch;
		case 3: *(Uint16*)p = col; p+=surface->pitch;
		case 2: *(Uint16*)p = col; p+=surface->pitch;
		case 1: *(Uint16*)p = col; p+=surface->pitch;
	}while( (i-=4) > 0 );
	}

#elif SDL_DRAW_BPP == 3
	Uint8 colorbyte0 = (Uint8) (col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((col >> 16) & 0xff);

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
	switch( i % 4 ) {								
	do{										 
		case 0: *(Uint32*)p = col; p+=surface->pitch;
		case 3: *(Uint32*)p = col; p+=surface->pitch;
		case 2: *(Uint32*)p = col; p+=surface->pitch;
		case 1: *(Uint32*)p = col; p+=surface->pitch;
	}while( (i-=4) > 0 );
	}
#endif

}/*Draw_VLine*/

void DC::Draw_HLine(const signed int x0, const signed int y0, const signed int x1) const
{
	if (x0 > x1)	
		return; // TODO?
//		{ i=x1; x1=x0; x0=i; }
	Uint32 col = (Uint32)(*pen.GetColor());
#if SDL_DRAW_BPP == 3
		Uint8 colorbyte0 = (Uint8) (col & 0xff);
		Uint8 colorbyte1 = (Uint8) ((col >> 8) & 0xff);
		Uint8 colorbyte2 = (Uint8) ((col >> 16) & 0xff);
#endif
																																							
	register Uint8 *p;
	register Sint16 i;
																																							
	p = (Uint8*)surface->pixels + y0 * surface->pitch + x0 * SDL_DRAW_BPP;

#if SDL_DRAW_BPP == 1
		memset(p, col, x1-x0+1);
																																							
#elif SDL_DRAW_BPP == 2
		i = x1-x0+1;						 
		switch( i % 4 ) {					 
			do{								
				case 0: *(Uint16*)p = col; p+=2;
				case 3: *(Uint16*)p = col; p+=2; 
				case 2: *(Uint16*)p = col; p+=2;
				case 1: *(Uint16*)p = col; p+=2;
			}while( (i-=4) > 0 );				
		}
																																							
#elif SDL_DRAW_BPP == 3
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
#elif SDL_DRAW_BPP == 4
	if (sizeof(wchar_t) == sizeof(Uint32)) { 
	#ifdef __linux__
		wmemset((wchar_t*)p, col, x1-x0+1);	
	#elif __WIN32__
		memset((wchar_t*)p, col, x1-x0+1);
	#endif
	} else
	{
		i = x1-x0+1;
		do
		{
			*(Uint32*)p = col; p+=4;
		}
		while((i-=4)>0);
	}
#endif
}/*Draw_HLine*/

#define sign(a) (((a)<0) ? -1 : (a)>0 ? 1 : 0)

void DC::Draw_Line(signed int x1, signed int y1, signed int x2, signed int y2) const
{
	return;
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
	if (ax>ay)
	{						/* x dominant */
	d = ay - (ax >> 1);
	for (;;)
	{
		*((Uint32 *)(lineAddr + (x << 2))) = (Uint32)col;

		if (x == x2)
		{
		return;
		}
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
		{
		return;
		}
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
}

void DC::DrawFilledRound(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	return;
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
	
	unsigned int i = 0;
	while (i <= radius) 
	{

// Nur Ecken:
	// links oben
#ifdef __linux__

/*		for(int k=i;k--;)
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
		}*/


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
/*		memset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (Xcenter - i)*4), col, i);
		memset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (Xcenter - radius)*4), col, radius);
	// rechts oben
		memset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + X2center*4), col, i);
		memset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + X2center*4), col, radius);
	// links unten
		memset((wchar_t*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + (Xcenter - i)*4), col, i);
		memset((wchar_t*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + (Xcenter - radius)*4), col, radius);
	// rechts unten
		memset((wchar_t*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + X2center*4), col, i);
		memset((wchar_t*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + X2center*4), col, radius);*/
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

}/*Draw_FillRound*/

void DC::DrawFilledEdgedRound(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	return;
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

}/*Draw_FillRound*/


void DC::DrawEmptyEdgedRound(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	return;
	unsigned int radius = corner;
	Uint32 col = (Uint32)(*pen.GetColor());

	int rightInc = 6;

	/*TODO: We can do better :-)*/
	int d = width<height ? width : height;
	d = 3 - (radius<<1);
	int diagonalInc = 10 - (radius<<2);

	int dx = width - (radius<<1);
	int dy = height - (radius<<1);
	
	int Xcenter = x + radius;
	int Ycenter = y + radius;

	int X2center = Xcenter + dx;
	int Y2center = Ycenter + dy;

	register Uint8* p0 = (Uint8*)surface->pixels +	y	*		surface->pitch + Xcenter*4;
	register Uint8* p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + Xcenter*4;
	register Sint16 i=dx;				

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
	if (height<3)	return;						 
	p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + x*4;	 
	p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x+width)*4;
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

	/*unsigned int*/ i = 0; // TODO
	
	while (i <= ((signed int)(radius))) {


*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (Xcenter - i)*4)) = col;
*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (Xcenter - radius)*4)) = col;

*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (X2center + i)*4)) = col;
*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (X2center + radius)*4)) = col;

*((Uint32*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + (Xcenter - corner)*4)) = col;
*((Uint32*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (Xcenter - corner)*4)) = col;

*((Uint32*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + (X2center + i)*4)) = col;
*((Uint32*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + (X2center + radius)*4)) = col;

*((Uint32*)((Uint8*)surface->pixels + (Y2center+corner-1)*surface->pitch + (Xcenter - radius)*4)) = col; // new
*((Uint32*)((Uint8*)surface->pixels + (Y2center+corner-1)*surface->pitch + (Xcenter - i)*4)) = col; // new

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

}/*Draw_Round*/

void DC::DrawEmptyRound(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
	return;
	unsigned int radius = corner;
	Uint32 col = (Uint32)(*pen.GetColor());

	register Uint8 *p0;
	register Uint8 *p1;
	register Sint16 i;
	Sint16 dx, dy;
	Sint16 Xcenter, Ycenter, X2center, Y2center;

	Sint16 rightInc = 6;
	Sint16 d, diagonalInc;

	if (width==0 || height==0)	return;

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
	if (height<3)	return;						 
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

	while (i < ((signed int)(radius))) {

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

}/*Draw_Round*/



