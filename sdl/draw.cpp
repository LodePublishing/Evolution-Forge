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
#define SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX \
	if (SDL_BYTEORDER == SDL_BIG_ENDIAN) { \
		p[0] = bright_colorbyte2;					 \
		p[1] = bright_colorbyte1;					 \
		p[2] = bright_colorbyte0;					 \
	} else {								 \
		p[0] = bright_colorbyte0;					 \
		p[1] = bright_colorbyte1;					 \
		p[2] = bright_colorbyte2;					 \
	}

#define SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX \
	if (SDL_BYTEORDER == SDL_BIG_ENDIAN) { \
		p[0] = dark_colorbyte2;					 \
		p[1] = dark_colorbyte1;					 \
		p[2] = dark_colorbyte0;					 \
	} else {								 \
		p[0] = dark_colorbyte0;					 \
		p[1] = dark_colorbyte1;					 \
		p[2] = dark_colorbyte2;					 \
	}

#define SDL_DO_DRAWING_X if(k>0) switch( k % 4 ) {do{case 0: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=3;case 3: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=3;case 2: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=3;case 1: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=3;}while( (k-=4) > 0 );}
#define SDL_DO_BRIGHT_DRAWING_X if(k>0) switch( k % 4 ) {do{case 0: SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX p+=3;case 3: SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX p+=3;case 2: SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX p+=3;case 1: SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX p+=3;}while( (k-=4) > 0 );}
#define SDL_DO_DARK_DRAWING_X if(k>0) switch( k % 4 ) {do{case 0: SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX p+=3;case 3: SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX p+=3;case 2: SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX p+=3;case 1: SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX p+=3;}while( (k-=4) > 0 );}

#define SDL_DO_DRAWING_Y if(k>0) switch( k % 4 ) {do{case 0: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=surface->pitch;case 3: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=surface->pitch;case 2: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=surface->pitch;case 1: SDL_DRAW_PUTPIXEL_BPP_3_AUX p+=surface->pitch;}while( (k-=4) > 0 );}
#define SDL_DO_BRIGHT_DRAWING_Y if(k>0) switch( k % 4 ) {do{case 0: SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX p+=surface->pitch;case 3: SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX p+=surface->pitch;case 2: SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX p+=surface->pitch;case 1: SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX p+=surface->pitch;}while( (k-=4) > 0 );}
#define SDL_DO_DARK_DRAWING_Y if(k>0) switch( k % 4 ) {do{case 0: SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX p+=surface->pitch;case 3: SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX p+=surface->pitch;case 2: SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX p+=surface->pitch;case 1: SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX p+=surface->pitch;}while( (k-=4) > 0 );}

void DC::Draw_VLine_8bit(const signed int x0, const signed int y0, const signed int y1) const
{
	Uint8 pen_col = (Uint8)(*pen.GetColor());
	register Uint8 *p = (Uint8*)surface->pixels + y0 * surface->pitch + x0;
	register signed int i = y1-y0+1;
	Lock();

	if(pen.GetStyle() == SHORT_DASH_PEN_STYLE)
	{
		for(;i--;)
		{
			if(((i%5) != 2)&&((i%5) != 3))
				*p = pen_col;
			p+=surface->pitch;
		}
	} else
	{
		switch( i % 4 ) {					
			do{								 
				case 0: *p = pen_col; p+=surface->pitch;
				case 3: *p = pen_col; p+=surface->pitch;
				case 2: *p = pen_col; p+=surface->pitch;
				case 1: *p = pen_col; p+=surface->pitch;
			}while( (i-=4) > 0 );
		}
	}
	Unlock();
}


void DC::Draw_VLine_16bit(const signed int x0, const signed int y0, const signed int y1) const
{
	Uint16 pen_col = (Uint16)(*pen.GetColor());
	register Uint8 *p = (Uint8*)surface->pixels + y0 * surface->pitch + (x0 << 1);
	register signed int i = y1-y0+1;
	Lock();

	if(pen.GetStyle() == SHORT_DASH_PEN_STYLE)
	{
		for(;i--;)
		{
			if(((i%5) != 2)&&((i%5) != 3))
				*(Uint16*)p = pen_col;
			p+=surface->pitch;
		}
	} else
	{
		switch( i % 4 ) {
			do{
				case 0: *(Uint16*)p = pen_col; p+=surface->pitch;
				case 3: *(Uint16*)p = pen_col; p+=surface->pitch;
				case 2: *(Uint16*)p = pen_col; p+=surface->pitch;
				case 1: *(Uint16*)p = pen_col; p+=surface->pitch;
			}while( (i-=4) > 0 );
		}
	}
	Unlock();
}

void DC::Draw_VLine_24bit(const signed int x0, const signed int y0, const signed int y1) const
{
	Uint32 pen_col = (Uint32)(*pen.GetColor());
	Uint8 colorbyte0 = (Uint8) (pen_col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((pen_col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((pen_col >> 16) & 0xff);

	register Uint8 *p = (Uint8*)surface->pixels + y0 * surface->pitch + x0 * 3;
	register signed int k = y1-y0+1;

	Lock();
	if(pen.GetStyle() == SHORT_DASH_PEN_STYLE)
	{
		for(;k--;)
		{
			if(((k%5) != 2) && ((k%5) != 3))
			{
				SDL_DRAW_PUTPIXEL_BPP_3_AUX
			}
			p+=surface->pitch;
		}
	} else
	{
		SDL_DO_DRAWING_Y
	}
	Unlock();
}

void DC::Draw_VLine_32bit(const signed int x0, const signed int y0, const signed int y1) const
{
	Uint32 pen_col = (Uint32)(*pen.GetColor());

	register Uint8 *p = (Uint8*)surface->pixels + y0 * surface->pitch + (x0 << 2);
	register signed int i = y1-y0+1;
	
	Lock();
	
	if(pen.GetStyle() == SHORT_DASH_PEN_STYLE)
	{
		for(;i--;)
		{
			if(((i%5) != 2)&&((i%5) != 3))
				*(Uint32*)p = pen_col;
			p+=surface->pitch;
		}
	} else
	{
		for(;i--; p+=surface->pitch)
			*(Uint32*)p = pen_col;
	}
	
	Unlock();
}


void DC::Draw_HLine_8bit(const signed int x0, const signed int y0, const signed int x1) const
{
	Uint32 col = (Uint32)(*pen.GetColor());
	register Uint8* p = (Uint8*)surface->pixels + y0 * surface->pitch + x0;
	memset(p, col, x1-x0+1);
}

void DC::Draw_HLine_16bit(const signed int x0, const signed int y0, const signed int x1) const
{
	Uint32 col = (Uint32)(*pen.GetColor());
	register Uint8* p = (Uint8*)surface->pixels + y0 * surface->pitch + (x0 << 1);
	register signed int i = x1-x0+1;
	
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
	Uint32 col = (Uint32)(*pen.GetColor());
	Uint8 colorbyte0 = (Uint8) (col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((col >> 16) & 0xff);

	register Uint8 *p = (Uint8*)surface->pixels + y0 * surface->pitch + x0 * 3;
	register signed int k = x1-x0+1;

	Lock();
	
	SDL_DO_DRAWING_X
	
	Unlock();
}

void DC::Draw_HLine_32bit(const signed int x0, const signed int y0, const signed int x1) const
{
	Uint32 pen_col = (Uint32)(*pen.GetColor());

	Lock();
	
	if (sizeof(wchar_t) == sizeof(Uint32)) { 
#ifdef __linux__
		wmemset((wchar_t*)( (Uint8*)surface->pixels +  y0 * surface->pitch + (x0 << 2)), pen_col, x1-x0);
#elif __WIN32__
		memset((wchar_t*)( (Uint8*)surface->pixels +  y0 * surface->pitch + (x0 << 2)), pen_col, x1-x0);
#endif
	} else 
	{
		register Uint8* p = (Uint8*)surface->pixels + y0 * surface->pitch + (x0 << 2);
		register signed int i = x1-x0;
		for(;i--;p+=4)
			*(Uint32*)p = pen_col;
	}
	Unlock();
}

void DC::Draw_Line_8bit(signed int x1, signed int y1, signed int x2, signed int y2) const
{
	signed int dx = x2 - x1 + 1;
	signed int dy = y2 - y1;
	signed int sy = (dy >= 0) ? 1 : -1;
	dy = sy * dy + 1;
	unsigned int pixx = 1;
	unsigned int pixy = surface->pitch;
	Uint8* pixel = ((Uint8 *) surface->pixels) + pixx * x1 + pixy * y1;
	pixy *= sy;
	if (dx < dy) {
		signed int swaptmp = dx;dx = dy;dy = swaptmp;
		swaptmp = pixx;pixx = pixy;pixy = swaptmp;
	}

	Uint8 col = (Uint8)(*pen.GetColor());
	register signed int x = 0;
	register signed int y = 0;
	
	Lock();
	for (; x < dx; ++x, pixel += pixx) {
		*pixel = col;
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
	signed int dx = x2 - x1 + 1;
	signed int dy = y2 - y1;
	signed int sy = (dy >= 0) ? 1 : -1;
	dy = sy * dy + 1;
	unsigned int pixx = 2;
	unsigned int pixy = surface->pitch;
	Uint8* pixel = ((Uint8 *) surface->pixels) + pixx * x1 + pixy * y1;
	pixy *= sy;
	if (dx < dy) {
		signed int swaptmp = dx;dx = dy;dy = swaptmp;
		swaptmp = pixx;pixx = pixy;pixy = swaptmp;
	}
	
	Uint16 col = (Uint16)(*pen.GetColor());
	register signed int x = 0;
	register signed int y = 0;
	
	Lock();
	for (; x < dx; ++x, pixel += pixx) {
		*(Uint16 *) pixel = col;
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
	signed int dx = x2 - x1 + 1;
	signed int dy = y2 - y1;
	signed int sy = (dy >= 0) ? 1 : -1;
	dy = sy * dy + 1;
	unsigned int pixx = 3;
	unsigned int pixy = surface->pitch;
	Uint8* p = ((Uint8 *) surface->pixels) + pixx * x1 + pixy * y1;
	pixy *= sy;
	if (dx < dy) {
		signed int swaptmp = dx;dx = dy;dy = swaptmp;
		swaptmp = pixx;pixx = pixy;pixy = swaptmp;
	}

	Color col = (*pen.GetColor());
	Uint8 colorbyte0 = (Uint8) (col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((col >> 16) & 0xff);
	register signed int x = 0;
	register signed int y = 0;

	Lock();
	for (; x < dx; ++x, p += pixx) {
		
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
	signed int dx = x2 - x1 + 1;
	signed int dy = y2 - y1;
	signed int sy = (dy >= 0) ? 1 : -1;
	dy = sy * dy + 1;
	unsigned int pixx = 4;
	unsigned int pixy = surface->pitch;
	Uint8* pixel = ((Uint8 *) surface->pixels) + pixx * x1 + pixy * y1;
	pixy *= sy;
	if (dx < dy) {
		signed int swaptmp = dx;dx = dy;dy = swaptmp;
		swaptmp = pixx;pixx = pixy;pixy = swaptmp;
	}

	Uint32 col = (Uint32)(*pen.GetColor());
	register signed int x = 0;
	register signed int y = 0;

	Lock();
	for (; x < dx; ++x, pixel += pixx) {
		*(Uint32 *) pixel = (Uint32)col;
		y += dy;
		if (y >= dx) {
			y -= dx;
			pixel += pixy;
		}
	}
	Unlock();
}


// -------- ----------- --------
// -------- EMPTY ROUND --------
// -------- ----------- --------


void DC::DrawEmptyRound_8bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
// Ecke links oben
	signed int Xcenter = x+corner;
	signed int Ycenter = y+corner;
// Kantenlaengen
	unsigned int dx = width - (corner << 1);
	unsigned int dy = height - (corner << 1);
// Ecke rechts unten
	signed int X2center = Xcenter + dx - 1;
	signed int Y2center = Ycenter + dy - 1;
	Uint8 col = (Uint8)(*pen.GetColor());

	Lock();
	{
		register Uint8* p0 = (Uint8*)surface->pixels +	y	* surface->pitch + Xcenter;
		register Uint8* p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + Xcenter;
		register signed int i = dx;
		switch( i % 4 ) { 
			do{			
				case 0:	 
					*p0 = col; ++p0;		 
					*p1 = col; ++p1;		
				case 3:								
					*p0 = col; ++p0;		
					*p1 = col; ++p1;	 
				case 2:						 
					*p0 = col; ++p0;	 
					*p1 = col; ++p1;
				case 1:						
					*p0 = col; ++p0;
					*p1 = col; ++p1;
			}while( (i-=4) > 0 );			
		}											 
	}
	{
		register Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + x;
		register Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x+width-1);
		register signed int i = dy;
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
	}		

//	rounded corners 
	{
		unsigned int rightInc = 6;
		signed int diagonalInc = 10 - (corner<<2);
		signed int d = 3 - (corner << 1);
		unsigned int radius = corner;
		unsigned int i = 0;

		while (i <= radius) 
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
		}	
	}
	Unlock();
}

void DC::DrawEmptyRound_16bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
// Ecke links oben
	signed int Xcenter = x+corner;
	signed int Ycenter = y+corner;
// Kantenlaengen
	unsigned int dx = width - (corner << 1);
	unsigned int dy = height - (corner << 1);
// Ecke rechts unten
	signed int X2center = Xcenter + dx - 1;
	signed int Y2center = Ycenter + dy - 1;
	Uint32 col = (Uint32)(*pen.GetColor());
	
	Lock();
	{
		register Uint8* p0 = (Uint8*)surface->pixels +	y	* surface->pitch + (Xcenter << 1);
		register Uint8* p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + (Xcenter << 1);
		register signed int i = dx;				
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
	}
	{
		register Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x << 1);	 
		register Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + ((x+width-1) << 1);
		register signed int i = dy;				
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
	}
	{
		unsigned int rightInc = 6;
		signed int diagonalInc = 10 - (corner<<2);	
		signed int d = 3 - (corner << 1);
		unsigned int radius = corner;
		unsigned int i = 0;
		while (i <= radius)
		{
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((Xcenter - radius)<<1) )) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((Xcenter - i)<<1) )) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((X2center + i)<<1) )) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((X2center + radius)<<1) )) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + ((X2center + i)<<1) )) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + ((X2center + radius)<<1) )) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + ((Xcenter - i)<<1) )) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + ((Xcenter - radius)<<1) )) = col;
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
	}
	Unlock();

}

void DC::DrawEmptyRound_24bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
// Ecke links oben
	signed int Xcenter = x+corner;
	signed int Ycenter = y+corner;
// Kantenlaengen
	unsigned int dx = width - (corner << 1);
	unsigned int dy = height - (corner << 1);
// Ecke rechts unten
	signed int X2center = Xcenter + dx - 1;
	signed int Y2center = Ycenter + dy - 1;
	Uint32 col = (Uint32)(*pen.GetColor());
	
	Uint8 colorbyte0 = (Uint8) (col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((col >> 16) & 0xff);
// ------ TOP AND LOWER HORIZONTAL LINE ------
	Lock();
	{
		register Uint8* p;
		register signed int k;
		p = (Uint8*)surface->pixels + y * surface->pitch + Xcenter * 3;k = dx;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + Xcenter * 3;k = dx;SDL_DO_DRAWING_X
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

	
// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		register Uint8* p;
		register signed int k;
		p = (Uint8*)surface->pixels + Ycenter * surface->pitch + x * 3;k = dy;SDL_DO_DRAWING_Y
		p = (Uint8*)surface->pixels + Ycenter * surface->pitch + (x+width-1) * 3;k = dy;SDL_DO_DRAWING_Y
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------

	unsigned int rightInc = 6;
	signed int d = 3 - (corner << 1);
	signed int diagonalInc = 10 - (corner<<2);	
	unsigned int radius = corner;
	unsigned int i = 0;
	
	register Uint8* p;
	while (i <= radius) 
	{
		p = (Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (Xcenter - radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (Xcenter - i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		
		p = (Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (X2center + i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (X2center + radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		
		p = (Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + (X2center + i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (X2center + radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		
		p = (Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + (Xcenter - i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (Xcenter - radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX

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

void DC::DrawEmptyRound_32bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
// Ecke links oben
	signed int Xcenter = x+corner;
	signed int Ycenter = y+corner;
// Kantenlaengen
	unsigned int dx = width - (corner << 1);
	unsigned int dy = height - (corner << 1);
// Ecke rechts unten
	signed int X2center = Xcenter + dx - 1;
	signed int Y2center = Ycenter + dy - 1;
	Uint32 pen_col = (Uint32)(*pen.GetColor());

	Lock();
	{
	 	register Uint8* p0 = (Uint8*)surface->pixels +	y	* surface->pitch + (Xcenter<<2);
		register Uint8* p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + (Xcenter<<2);

		if (sizeof(wchar_t) == sizeof(Uint32)) 
		{
#ifdef __linux__
			wmemset((wchar_t*)p0, pen_col, dx);
			wmemset((wchar_t*)p1, pen_col, dx);
#elif __WIN32__
			memset((wchar_t*)p0, pen_col, dx);
			memset((wchar_t*)p1, pen_col, dx);
#endif
		} else 
		{
			register signed int i = dx;				
			switch( i % 4 ) { 
				do{			
					case 0:	 
						*(Uint32*)p0 = pen_col; p0+=4;		 
						*(Uint32*)p1 = pen_col; p1+=4;		
					case 3:								
						*(Uint32*)p0 = pen_col; p0+=4;		
						*(Uint32*)p1 = pen_col; p1+=4;	 
					case 2:							 
						*(Uint32*)p0 = pen_col; p0+=4;	 
						*(Uint32*)p1 = pen_col; p1+=4;
					case 1:							
						*(Uint32*)p0 = pen_col; p0+=4;
						*(Uint32*)p1 = pen_col; p1+=4;
				}while( (i-=4) > 0 );			
			}
		}
	}
	{ 
		register Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x<<2);	 
		register Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + ((x+width-1)<<2);
		register signed int i = dy;
		switch( i % 4 ) {							 
			do{										
				case 0:								 
					*(Uint32*)p0 = pen_col; p0+=surface->pitch;
					*(Uint32*)p1 = pen_col; p1+=surface->pitch;
				case 3:								 
					*(Uint32*)p0 = pen_col; p0+=surface->pitch;
					*(Uint32*)p1 = pen_col; p1+=surface->pitch;
				case 2:								 
					*(Uint32*)p0 = pen_col; p0+=surface->pitch;
					*(Uint32*)p1 = pen_col; p1+=surface->pitch;
				case 1:								 
					*(Uint32*)p0 = pen_col; p0+=surface->pitch;
					*(Uint32*)p1 = pen_col; p1+=surface->pitch;
			}while( (i-=4) > 0 );					 
		}
	}	
	{
		unsigned int rightInc = 6;
		signed int d = 3 - (corner << 1);
		signed int diagonalInc = 10 - (corner<<2);	
		unsigned int radius = corner;
		unsigned int i = 0;	
		
		while (i <= radius) 
		{
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((Xcenter - radius)<<2) )) = pen_col;
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((Xcenter - i)<<2) )) = pen_col;

			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((X2center + i)<<2) )) = pen_col;
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((X2center + radius)<<2) )) = pen_col;

			*((Uint32*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + ((X2center + i)<<2) )) = pen_col;
			*((Uint32*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + ((X2center + radius)<<2) )) = pen_col;

			*((Uint32*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + ((Xcenter - i)<<2) )) = pen_col;
			*((Uint32*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + ((Xcenter - radius)<<2) )) = pen_col;

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
	}		
	Unlock();

}


// -------- ------------ --------
// -------- FILLED ROUND --------
// -------- ------------ --------


void DC::DrawFilledRound_8bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
// Ecke links oben
	signed int Xcenter = x+corner;
	signed int Ycenter = y+corner;
// Kantenlaengen
	unsigned int dx = width - (corner << 1);
	unsigned int dy = height - (corner << 1);
// Ecke rechts unten
	signed int X2center = Xcenter + dx - 1;
	signed int Y2center = Ycenter + dy - 1;
	Uint32 col = (Uint32)(*brush.GetColor());

// Rechteck von linker Kante nach rechter Kante
	if(dy > 0)
	{
		SDL_Rect r;
		r.x = x+1; r.y = Ycenter;
		r.w = width-1; r.h = dy;
		SDL_FillRect(surface, &r, col);
	}

// Rechteck entlang der oberen und entlang der unteren Kante
	if((corner > 0)&&(dx>0))
	{
		SDL_Rect r;
		r.x = Xcenter; r.y = y+1;
		r.w = dx; r.h = corner-1;
		SDL_FillRect(surface, &r, col);

		r.x = Xcenter; r.y = Y2center;
		r.w = dx; r.h = corner;
		SDL_FillRect(surface, &r, col);
	}
	
	unsigned int rightInc = 6;
	signed int diagonalInc = 10 - (corner << 2);
	signed int d = 3 - (corner << 1);
	unsigned int radius = corner;
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
			*((Uint8*)((Uint8*)surface->pixels + k+(Y2center+radius)*surface->pitch + X2center)) = col;
			*((Uint8*)((Uint8*)surface->pixels + k+(Y2center+radius)*surface->pitch + (Xcenter-i))) = col;

		}
		for(unsigned int k=radius;k--;)
		{
			*((Uint8*)((Uint8*)surface->pixels + k+(Ycenter-i)*surface->pitch + (Xcenter - radius))) = col;
			*((Uint8*)((Uint8*)surface->pixels + k+(Ycenter-i)*surface->pitch + X2center)) = col;
			*((Uint8*)((Uint8*)surface->pixels + k+(Y2center+i)*surface->pitch + X2center)) = col;
			*((Uint8*)((Uint8*)surface->pixels + k+(Y2center+i)*surface->pitch + (Xcenter-radius))) = col;
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
// Ecke links oben
	signed int Xcenter = x+corner;
	signed int Ycenter = y+corner;
// Kantenlaengen
	unsigned int dx = width - (corner << 1);
	unsigned int dy = height - (corner << 1);
// Ecke rechts unten
	signed int X2center = Xcenter + dx - 1;
	signed int Y2center = Ycenter + dy - 1;
	Uint32 col = (Uint32)(*brush.GetColor());

// Rechteck von linker Kante nach rechter Kante
	if(dy > 0)
	{
		SDL_Rect r;
		r.x = x+1; r.y = Ycenter;
		r.w = width-1; r.h = dy;
		SDL_FillRect(surface, &r, col);
	}

// Rechteck entlang der oberen und entlang der unteren Kante
	if( (corner > 0) && (dx > 0) )
	{
		SDL_Rect r;
		r.x = Xcenter; r.y = y+1;
		r.w = dx; r.h = corner-1;
		SDL_FillRect(surface, &r, col);

		r.x = Xcenter; r.y = Y2center;
		r.w = dx; r.h = corner;
		SDL_FillRect(surface, &r, col);
	}
	unsigned int rightInc = 6;
	signed int diagonalInc = 10 - (corner << 2);
	signed int d = 3 - (corner << 1);
	unsigned int radius = corner;
	unsigned int i = 0;
	
	Lock();
	while (i <= radius) 
	{
// Nur Ecken:
		for(unsigned int k=i;k--;)
		{
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((k + Xcenter - i)<<1) )) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((k + X2center)<<1) )) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + ((k + X2center)<<1) )) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + ((k + Xcenter - i)<<1) )) = col;

		}
		for(unsigned int k=radius;k--;)
		{
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((k+Xcenter - radius)<<1) )) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((k+X2center)<<1) )) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + ((k+X2center)<<1) )) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+i-1)*surface->pitch + ((k+Xcenter-radius)<<1) )) = col;
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
	}
	Unlock();

}

void DC::DrawFilledRound_24bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
// Ecke links oben
	signed int Xcenter = x+corner;
	signed int Ycenter = y+corner;
// Kantenlaengen
	unsigned int dx = width - (corner << 1);
	unsigned int dy = height - (corner << 1);
// Ecke rechts unten
	signed int X2center = Xcenter + dx - 1;
	signed int Y2center = Ycenter + dy - 1;
	Uint32 col = (Uint32)(*brush.GetColor());

// Rechteck von linker Kante nach rechter Kante
	if(dy > 0)
	{
		SDL_Rect r;
		r.x = x+1; r.y = Ycenter;
		r.w = width-1; r.h = dy;
		SDL_FillRect(surface, &r, col);
	}

	// Rechteck entlang der oberen und entlang der unteren Kante
	if((corner > 0)&&(dx>0))
	{
		SDL_Rect r;
		r.x = Xcenter; r.y = y+1;
		r.w = dx; r.h = corner-1;
		SDL_FillRect(surface, &r, col);

		r.x = Xcenter; r.y = Y2center;
		r.w = dx; r.h = corner;
		SDL_FillRect(surface, &r, col);
	}

	Uint8 colorbyte0 = (Uint8) (col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((col >> 16) & 0xff);

	unsigned int rightInc = 6;
	signed int diagonalInc = 10 - (corner << 2);
	signed int d = 3 - (corner << 1);
	unsigned int radius = corner;
	unsigned int i = 0;
	
	Lock();	
	while (i <= radius) 
	{
		register Uint8* p;
		register signed int k;

		p = (Uint8*)surface->pixels + (Ycenter - i) * surface->pitch + (Xcenter - radius) * 3;k = radius;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Ycenter - radius) * surface->pitch + (Xcenter - i) * 3;k = i;SDL_DO_DRAWING_X
		
		p = (Uint8*)surface->pixels + (Ycenter - radius) * surface->pitch + (X2center + i) * 3;k = i;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Ycenter - i) * surface->pitch + (X2center + radius) * 3;k = radius;SDL_DO_DRAWING_X
		
		p = (Uint8*)surface->pixels + (Y2center + radius) * surface->pitch + (X2center - i) * 3;k = i;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Y2center + i) * surface->pitch + (X2center + radius) * 3;k = radius;SDL_DO_DRAWING_X
		
		p = (Uint8*)surface->pixels + (Y2center + radius) * surface->pitch + (Xcenter-i) * 3;k = i;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Y2center + i) * surface->pitch + (Xcenter-radius) * 3;k = radius;SDL_DO_DRAWING_X
	
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
	}
	Unlock();
}

void DC::DrawFilledRound_32bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
// Ecke links oben
	signed int Xcenter = x+corner;
	signed int Ycenter = y+corner;
// Kantenlaengen
	unsigned int dx = width - (corner << 1);
	unsigned int dy = height - (corner << 1);
// Ecke rechts unten
	signed int X2center = Xcenter + dx - 1;
	signed int Y2center = Ycenter + dy - 1;
	Uint32 col = (Uint32)(*brush.GetColor());

// FILLED
	// Rechteck von linker Kante nach rechter Kante
	if(dy > 0)
	{
		SDL_Rect r;
		r.x = x+1; r.y = Ycenter;
		r.w = width-1; r.h = dy;
		SDL_FillRect(surface, &r, col);
	}

	// Rechteck entlang der oberen und entlang der unteren Kante
	if((corner > 0)&&(dx>0))
	{
		SDL_Rect r;
		r.x = Xcenter; r.y = y+1;
		r.w = dx; r.h = corner-1;
		SDL_FillRect(surface, &r, col);

		r.x = Xcenter; r.y = Y2center;
		r.w = dx; r.h = corner;
		SDL_FillRect(surface, &r, col);
	}
	
	unsigned int rightInc = 6;
	signed int diagonalInc = 10 - (corner << 2);
	signed int d = 3 - (corner << 1);
	unsigned int radius = corner;
	unsigned int i = 0;
	
	Lock();
	while (i <= radius) 
	{

// Nur Ecken:
#ifdef __linux__
	// links oben
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((Xcenter - i +1)<<2) ), col, i);
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((Xcenter - radius + 1)<<2) ), col, radius);
	// rechts oben
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (X2center<<2) ), col, i);
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (X2center<<2) ), col, radius);
	// links unten
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + ((Xcenter - i + 1)<<2) ), col, i);
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + ((Xcenter - radius + 1)<<2) ), col, radius);
	// rechts unten
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + (X2center<<2) ), col, i);
		wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (X2center<<2) ), col, radius);
#elif __WIN32__
		for(unsigned int k=i; k--;)
		{
			*((Uint32*)((Uint8*)surface->pixels + k+ (Ycenter-radius)*surface->pitch + ((Xcenter - i)<<2) )) = col;
			*((Uint32*)((Uint8*)surface->pixels + k+(Ycenter-radius)*surface->pitch + (X2center<<2) )) = col;
			*((Uint32*)((Uint8*)surface->pixels + k+(Y2center+radius)*surface->pitch + (X2center<<2) )) = col;
			*((Uint32*)((Uint8*)surface->pixels + k+(Y2center+radius)*surface->pitch + ((Xcenter-i)<<2) )) = col;

		}
		for(unsigned int k=radius; k--;)
		{
			*((Uint32*)((Uint8*)surface->pixels + k+(Ycenter-i)*surface->pitch + ((Xcenter - radius)<<2) )) = col;
			*((Uint32*)((Uint8*)surface->pixels + k+(Ycenter-i)*surface->pitch + (X2center<<2) )) = col;
			*((Uint32*)((Uint8*)surface->pixels + k+(Y2center+i)*surface->pitch + (X2center<<2) )) = col;
			*((Uint32*)((Uint8*)surface->pixels + k+(Y2center+i)*surface->pitch + ((Xcenter-radius)<<2) )) = col;
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
	}
	Unlock();

}

// -------- ----------------- -------- 
// -------- EMPTY EDGED ROUND -------- 
// -------- ----------------- -------- 

void DC::DrawEmptyEdgedRound_8bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
// Ecke links oben
	signed int Xcenter = x+corner;
	signed int Ycenter = y+corner;
// Kantenlaengen
	unsigned int dx = width - (corner << 1);
	unsigned int dy = height - (corner << 1);
// Ecke rechts unten
	signed int X2center = Xcenter + dx - 1;
	signed int Y2center = Ycenter + dy - 1;
	Uint32 pen_col = (Uint32)(*pen.GetColor());

// ------ TOP AND LOWER HORIZONTAL LINE ------
	Lock();
	{
		memset((Uint8*)surface->pixels + y * surface->pitch + Xcenter, pen_col, dx);
		memset((Uint8*)surface->pixels + (y+height-1) * surface->pitch + x, pen_col, dx+corner);
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + x;
		Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x+width-1);
		signed int i = dy;
		for(;i--;p0+=surface->pitch, p1+=surface->pitch)
		{
			*(Uint8*)p0 = pen_col;
			*(Uint8*)p1 = pen_col;
		}
		i = corner;
		for(;i--;p0+=surface->pitch)
			*(Uint8*)p0 = pen_col;	
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------

	unsigned int rightInc = 6;
	signed int diagonalInc = 10 - (corner<<2);
	signed int d = 3 - (corner<<1);
	unsigned int radius = corner;
	unsigned int i = 0;	
	
	while (i <= radius) 
	{
		*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + Xcenter - i)) = pen_col;
		*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + Xcenter - radius)) = pen_col;

		*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + X2center + i)) = pen_col;
		*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + X2center + radius)) = pen_col;

		*((Uint8*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + Xcenter - corner)) = pen_col;
		*((Uint8*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + Xcenter - corner)) = pen_col;

		*((Uint8*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + X2center + i)) = pen_col;
		*((Uint8*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + X2center + radius)) = pen_col;

		*((Uint8*)((Uint8*)surface->pixels + (Y2center+corner)*surface->pitch + Xcenter - radius)) = pen_col; // new
		*((Uint8*)((Uint8*)surface->pixels + (Y2center+corner)*surface->pitch + Xcenter - i)) = pen_col; // new

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
// Ecke links oben
	signed int Xcenter = x+corner;
	signed int Ycenter = y+corner;
// Kantenlaengen
	unsigned int dx = width - (corner << 1);
	unsigned int dy = height - (corner << 1);
// Ecke rechts unten
	signed int X2center = Xcenter + dx - 1;
	signed int Y2center = Ycenter + dy - 1;
	Uint32 pen_col = (Uint32)(*pen.GetColor());
	
// ------ TOP AND LOWER HORIZONTAL LINE ------
	Lock();
	{
		register Uint8* p0 = (Uint8*)surface->pixels + y * surface->pitch + (Xcenter<<1);
		register Uint8* p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + (x<<1);
		register unsigned int i = corner;
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
		register Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x<<1);
		register Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + ((x+width-1)<<1);
		register unsigned int i = dy;
		for(;i--;p0+=surface->pitch, p1+=surface->pitch)
		{
			*(Uint16*)p0 = pen_col;
			*(Uint16*)p1 = pen_col;
		}
		i = corner;
		for(;i--;p0+=surface->pitch)
			*(Uint16*)p0 = pen_col;	
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------
	{
		unsigned int rightInc = 6;
		signed int diagonalInc = 10 - (corner<<2);
		signed int d = 3 - (corner<<1);
		unsigned int radius = corner;
		unsigned int i = 0;
		
		while (i <= radius) 
		{
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((Xcenter - i)<<1) )) = pen_col;
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((Xcenter - radius)<<1) )) = pen_col;

			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((X2center + i)<<1) )) = pen_col;
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((X2center + radius)<<1) )) = pen_col;

			*((Uint16*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + ((Xcenter - corner)<<1) )) = pen_col;
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + ((Xcenter - corner)<<1) )) = pen_col;

			*((Uint16*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + ((X2center + i)<<1) )) = pen_col;
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + ((X2center + radius)<<1) )) = pen_col;

			*((Uint16*)((Uint8*)surface->pixels + (Y2center+corner)*surface->pitch + ((Xcenter - radius)<<1) )) = pen_col; // new
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+corner)*surface->pitch + ((Xcenter - i)<<1) )) = pen_col; // new

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

}

void DC::DrawEmptyEdgedRound_24bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
// Ecke links oben
	signed int Xcenter = x+corner;
	signed int Ycenter = y+corner;
// Kantenlaengen
	unsigned int dx = width - (corner << 1);
	unsigned int dy = height - (corner << 1);
// Ecke rechts unten
	signed int X2center = Xcenter + dx - 1;
	signed int Y2center = Ycenter + dy - 1;
	Uint32 pen_col = (Uint32)(*pen.GetColor());

// ------ TOP AND LOWER HORIZONTAL LINE ------
	Uint8 colorbyte0 = (Uint8) (pen_col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((pen_col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((pen_col >> 16) & 0xff);

	Lock();
	{
		register Uint8* p;
		register signed int k;
		p = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + x * 3; k = dx+corner-1; SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + y * surface->pitch + Xcenter * 3; k = dx; SDL_DO_DRAWING_X
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		register Uint8* p;
		register signed int k;
		p = (Uint8*)surface->pixels + Ycenter * surface->pitch + x * 3;k = corner+dy-1;SDL_DO_DRAWING_Y
		p = (Uint8*)surface->pixels + Ycenter * surface->pitch + (x+width-1) * 3;k = dy;SDL_DO_DRAWING_Y
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------

	unsigned int rightInc = 6;
	signed int diagonalInc = 10 - (corner<<2);
	signed int d = 3 - (corner<<1);
	unsigned int radius = corner;
	unsigned int i = 0;
	register Uint8* p;
	
	while (i <= radius) 
	{
		p = (Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (Xcenter - i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (Xcenter - radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		
		p = (Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (X2center+i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (X2center+radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		
		p = (Uint8*)surface->pixels + (Y2center + radius)*surface->pitch + (X2center+i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
		p = (Uint8*)surface->pixels + (Y2center + i)*surface->pitch + (X2center+radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX

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

void DC::DrawEmptyEdgedRound_32bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
// Ecke links oben
	signed int Xcenter = x+corner;
	signed int Ycenter = y+corner;
// Kantenlaengen
	unsigned int dx = width - (corner << 1);
	unsigned int dy = height - (corner << 1);
// Ecke rechts unten
	signed int X2center = Xcenter + dx - 1;
	signed int Y2center = Ycenter + dy - 1;
	Uint32 pen_col = (Uint32)(*pen.GetColor());

// ------ TOP AND LOWER HORIZONTAL LINE ------
	{
		register Uint8* p0 = (Uint8*)surface->pixels + y * surface->pitch + (Xcenter<<2);
		register Uint8* p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + (x<<2);
		Lock();
		if (sizeof(wchar_t) == sizeof(Uint32)) { 
#ifdef __linux__
			wmemset((wchar_t*)p0, pen_col, dx);
			wmemset((wchar_t*)p1, pen_col, width - corner);
#elif __WIN32__
			memset((wchar_t*)p0, pen_col, dx);
			memset((wchar_t*)p1, pen_col, width - corner);
#endif
		} else 
		{
			register unsigned int i = corner;
			for(;i--;p1+=4)
				*(Uint32*)p1 = pen_col;
			i = dx;
			for(;i--;p0+=4,p1+=4)
			{
				*(Uint32*)p0 = pen_col;
				*(Uint32*)p1 = pen_col;
			}
		}
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		register Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x<<2);
		register Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + ((x+width-1)<<2);
		register unsigned int i = dy;
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

	unsigned int rightInc = 6;
	signed int diagonalInc = 10 - (corner<<2);
	signed int d = 3 - (corner<<1);
	unsigned int radius = corner;
	unsigned int i = 0;	
	
	while (i <= radius) 
	{
		*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((Xcenter - i)<<2) )) = pen_col;
		*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((Xcenter - radius)<<2) )) = pen_col;

		*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((X2center + i)<<2) )) = pen_col;
		*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((X2center + radius)<<2) )) = pen_col;

		*((Uint32*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + ((X2center + i)<<2) )) = pen_col;
		*((Uint32*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + ((X2center + radius)<<2) )) = pen_col;

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

}


// -------- ------------ --------
// -------- FILLED EDGED --------
// -------- ------------ --------


void DC::DrawFilledEdgedRound_8bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
// Ecke links oben
	signed int Xcenter = x+corner;
	signed int Ycenter = y+corner;
// Kantenlaengen
	unsigned int dx = width - (corner << 1);
	unsigned int dy = height - (corner << 1);
// Ecke rechts unten
	signed int X2center = Xcenter + dx - 1;
	signed int Y2center = Ycenter + dy - 1;
	Uint32 col = (Uint32)(*brush.GetColor());

// Rechteck von linker Kante nach rechter Kante
	if(dy > 0)
	{
		SDL_Rect r;
		r.x = x+1; r.y = Ycenter;
		r.w = width-1; r.h = dy;
		SDL_FillRect(surface, &r, col);
	}

	// Rechteck entlang der oberen und entlang der unteren Kante
	if((corner > 0)&&(dx>0))
	{
		SDL_Rect r;
		r.x = Xcenter; r.y = y+1;
		r.w = dx; r.h = corner-1;
		SDL_FillRect(surface, &r, col);

		r.x = x+1; r.y = Y2center;
		r.w = width-corner-1; r.h = corner;
		SDL_FillRect(surface, &r, col);
	}

	unsigned int rightInc = 6;
	signed int diagonalInc = 10 - (corner<<2);
	signed int d = 3 - (corner<<1);
	unsigned int radius = corner;
	unsigned int i = 0;
	Lock();
// Halbkreis
	while (i <= radius) 
	{
		for(unsigned int k=i; k--;)
		{
			*((Uint8*)((Uint8*)surface->pixels + k + (Ycenter-radius)*surface->pitch + (Xcenter - i))) = col;
			*((Uint8*)((Uint8*)surface->pixels + k + (Ycenter-radius)*surface->pitch + X2center)) = col;
			*((Uint8*)((Uint8*)surface->pixels + k + (Y2center+radius)*surface->pitch + X2center)) = col;
		}
		for(unsigned int k=radius; k--;)
		{
			*((Uint8*)((Uint8*)surface->pixels + k + (Ycenter-i)*surface->pitch + (Xcenter - radius))) = col;
			*((Uint8*)((Uint8*)surface->pixels + k + (Ycenter-i)*surface->pitch + X2center)) = col;
			*((Uint8*)((Uint8*)surface->pixels + k + (Y2center+i)*surface->pitch + X2center)) = col;
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

}

void DC::DrawFilledEdgedRound_16bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
// Ecke links oben
	signed int Xcenter = x+corner;
	signed int Ycenter = y+corner;
// Kantenlaengen
	unsigned int dx = width - (corner << 1);
	unsigned int dy = height - (corner << 1);
// Ecke rechts unten
	signed int X2center = Xcenter + dx - 1;
	signed int Y2center = Ycenter + dy - 1;
	Uint32 col = (Uint32)(*brush.GetColor());

// Rechteck von linker Kante nach rechter Kante
	if(dy > 0)
	{
		SDL_Rect r;
		r.x = x+1; r.y = Ycenter;
		r.w = width-1; r.h = dy;
		SDL_FillRect(surface, &r, col);
	}

// Rechteck entlang der oberen und entlang der unteren Kante
	if((corner > 0)&&(dx>0))
	{
		SDL_Rect r;
		r.x = Xcenter; r.y = y+1;
		r.w = dx; r.h = corner-1;
		SDL_FillRect(surface, &r, col);

		r.x = x+1; r.y = Y2center;
		r.w = width-corner-1; r.h = corner;
		SDL_FillRect(surface, &r, col);
	}

	unsigned int rightInc = 6;
	signed int diagonalInc = 10 - (corner<<2);
	signed int d = 3 - (corner<<1);
	unsigned int radius = corner;
	unsigned int i = 0;
	
	Lock();
// Halbkreis
	while (i <= radius) 
	{
		for(unsigned int k=i;k--;)
		{
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((k + Xcenter - i)<<1) )) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((k + X2center)<<1) )) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + ((k + X2center)<<1) )) = col;
		}
		for(unsigned int k=radius;k--;)
		{
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((k + Xcenter - radius)<<1) )) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((k + X2center)<<1) )) = col;
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + ((k + X2center)<<1) )) = col;
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
}

void DC::DrawFilledEdgedRound_24bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
// Ecke links oben
	signed int Xcenter = x+corner;
	signed int Ycenter = y+corner;
// Kantenlaengen
	unsigned int dx = width - (corner << 1);
	unsigned int dy = height - (corner << 1);
// Ecke rechts unten
	signed int X2center = Xcenter + dx - 1;
	signed int Y2center = Ycenter + dy - 1;
	Uint32 col = (Uint32)(*brush.GetColor());

// Rechteck von linker Kante nach rechter Kante
	if(dy > 0)
	{
		SDL_Rect r;
		r.x = x+1; r.y = Ycenter;
		r.w = width-1; r.h = dy;
		SDL_FillRect(surface, &r, col);
	}

// Rechteck entlang der oberen und entlang der unteren Kante
	if( (corner > 0) && (dx > 0) )
	{
		SDL_Rect r;
		r.x = Xcenter; r.y = y+1;
		r.w = dx; r.h = corner-1;
		SDL_FillRect(surface, &r, col);

		r.x = x+1; r.y = Y2center;
		r.w = width-corner-1; r.h = corner;
		SDL_FillRect(surface, &r, col);
	}

	Uint8 colorbyte0 = (Uint8) (col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((col >> 16) & 0xff);

	unsigned int rightInc = 6;
	signed int diagonalInc = 10 - (corner<<2);
	signed int d = 3 - (corner<<1);
	unsigned int radius = corner;
	unsigned int i = 0;

	Lock();
	while (i <= radius) 
	{
		Uint8* p;
		signed int k;
		p = (Uint8*)surface->pixels + (Ycenter-radius) * surface->pitch + (Xcenter-i) * 3;k = i;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Ycenter-radius) * surface->pitch + X2center * 3;k = i;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Ycenter+radius) * surface->pitch + X2center * 3;k = i;SDL_DO_DRAWING_X
		
		p = (Uint8*)surface->pixels + (Y2center-i) * surface->pitch + (Xcenter-radius) * 3;k = radius;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Y2center-i) * surface->pitch + X2center * 3;k = radius;SDL_DO_DRAWING_X
		p = (Uint8*)surface->pixels + (Y2center+i) * surface->pitch + X2center * 3;k = radius;SDL_DO_DRAWING_X

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
}

void DC::DrawFilledEdgedRound_32bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
// Ecke links oben
	signed int Xcenter = x+corner;
	signed int Ycenter = y+corner;
// Kantenlaengen
	unsigned int dx = width - (corner << 1);
	unsigned int dy = height - (corner << 1);
// Ecke rechts unten
	signed int X2center = Xcenter + dx - 1;
	signed int Y2center = Ycenter + dy - 1;
	Uint32 col = (Uint32)(*brush.GetColor());


	// Rechteck von linker Kante nach rechter Kante
	if(dy > 0)
	{
		SDL_Rect r;
		r.x = x+1; r.y = Ycenter;
		r.w = width-1; r.h = dy;
		SDL_FillRect(surface, &r, col);
	}

	// Rechteck entlang der oberen und entlang der unteren Kante
	if((corner > 0)&&(dx>0))
	{
		SDL_Rect r;
		r.x = Xcenter; r.y = y+1;
		r.w = dx; r.h = corner-1;
		SDL_FillRect(surface, &r, col);

		r.x = x+1; r.y = Y2center;
		r.w = width-corner-1; r.h = corner;
		SDL_FillRect(surface, &r, col);
	}
	
	unsigned int rightInc = 6;
	signed int diagonalInc = 10 - (corner << 2);
	signed int d = 3 - (corner << 2);
	unsigned int radius = corner;
	unsigned int i = 0;

	Lock();
// Halbkreis
	while (i <= radius) 
	{
// Nur Ecken:
		for(unsigned int k=i;k--;)
		{
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((k + Xcenter - i)<<2))) = col;
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((k + X2center)<<2))) = col;
			*((Uint32*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + ((k + X2center)<<2))) = col;
		}
		for(unsigned int k=radius;k--;)
		{
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((k + Xcenter - radius)<<2))) = col;
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((k + X2center)<<2))) = col;
			*((Uint32*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + ((k + X2center)<<2))) = col;
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
}


// -------- ------------------- --------
// -------- FILLED EDGED BORDER --------
// -------- ------------------- --------

void DC::DrawFilledEdgedBorderRound_8bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
// Ecke links oben
	signed int Xcenter = x+corner;
	signed int Ycenter = y+corner;
// Kantenlaengen
	unsigned int dx = width - (corner << 1);
	unsigned int dy = height - (corner << 1);
// Ecke rechts unten
	signed int X2center = Xcenter + dx - 1;
	signed int Y2center = Ycenter + dy - 1;
	Uint32 brush_col = (Uint32)(*brush.GetColor());

// Rechteck von linker Kante nach rechter Kante
	
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
		r.w = width-corner-1; r.h = corner;
		SDL_FillRect(surface, &r, brush_col);
	}
// ------- END OF Rechteck oben und unten ------


// ------ TOP AND LOWER HORIZONTAL LINE ------
	Uint32 dark_pen_col, bright_pen_col, pen_col;
	if(pressedRectangle)
	{
		dark_pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 110));
		bright_pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 70));
		pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 90));
	
	} else
	{
		dark_pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 80));
		bright_pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 120));
		pen_col = (Uint32)(*pen.GetColor());
	}
	
	Lock();
	{
		memset((Uint8*)surface->pixels + y * surface->pitch + Xcenter, bright_pen_col, dx);
		memset((Uint8*)surface->pixels + (y+height-1) * surface->pitch + x, dark_pen_col, dx+corner);
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

	
// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		register Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + x;	 
		register Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + x+width-1;
		register unsigned int i = dy;
		for(;i--;p0+=surface->pitch, p1+=surface->pitch)
		{
			*p0 = bright_pen_col;
			*p1 = dark_pen_col;
		}
		i = corner;
		for(;i--;p0+=surface->pitch)
			*p0 = bright_pen_col;	
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------

// Halbkreis
	{
		unsigned int rightInc = 6;
		signed int diagonalInc = 10 - (corner<<2);
		signed int d = 3 - (corner<<1);
		unsigned int radius = corner;
		unsigned int i = 0;
		
		while (i <= radius) 
		{
			for(unsigned int k=i;k--;)
			{
				*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + k + Xcenter - i)) = brush_col;
				*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + k + X2center)) = brush_col;
				*((Uint8*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + k + X2center)) = brush_col;
			}
			for(unsigned int k=radius;k--;)
			{
				*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + k + Xcenter - radius)) = brush_col;
				*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + k + X2center)) = brush_col;
				*((Uint8*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + k + X2center)) = brush_col;
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
		unsigned int rightInc = 6;
		signed int diagonalInc = 10 - (corner<<2);
		signed int d = 3 - (corner<<1);
		unsigned int radius = corner;
		unsigned int i = 0;
		
		while (i <= radius) 
		{
			*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + Xcenter - i)) = bright_pen_col;
			*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + Xcenter - radius)) = bright_pen_col;

			*((Uint8*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + X2center + i)) = pen_col;
			*((Uint8*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + X2center + radius)) = pen_col;

			*((Uint8*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + X2center + i)) = dark_pen_col;
			*((Uint8*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + X2center + radius)) = dark_pen_col;

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

}


void DC::DrawFilledEdgedBorderRound_16bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
// Ecke links oben
	signed int Xcenter = x+corner;
	signed int Ycenter = y+corner;
// Kantenlaengen
	unsigned int dx = width - (corner << 1);
	unsigned int dy = height - (corner << 1);
// Ecke rechts unten
	signed int X2center = Xcenter + dx - 1;
	signed int Y2center = Ycenter + dy - 1;
	Uint32 brush_col = (Uint32)(*brush.GetColor());

// Rechteck von linker Kante nach rechter Kante
	
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
		r.w = width-corner-1; r.h = corner;
		SDL_FillRect(surface, &r, brush_col);
	}
// ------- END OF Rechteck oben und unten ------


// ------ TOP AND LOWER HORIZONTAL LINE ------
	Uint32 dark_pen_col, bright_pen_col, pen_col;
	if(pressedRectangle)
	{
		dark_pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 110));
		bright_pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 70));
		pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 90));
	
	} else
	{
		dark_pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 80));
		bright_pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 120));
		pen_col = (Uint32)(*pen.GetColor());
	}
	

	Lock();
	{
		register Uint8* p0 = (Uint8*)surface->pixels + y * surface->pitch + (Xcenter<<1);
		register Uint8* p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + (x<<1);
		register unsigned int i = corner;
		for(;i--;p1+=2)
			*(Uint16*)p1 = dark_pen_col;
		i = dx;
		for(;i--;p0+=2,p1+=2)
		{
			*(Uint16*)p0 = bright_pen_col;
			*(Uint16*)p1 = dark_pen_col;
		}
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

	
// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		register Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x<<1);	 
		register Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + ((x+width-1)<<1);
		register unsigned int i = dy;
		for(;i--;p0+=surface->pitch, p1+=surface->pitch)
		{
			*(Uint16*)p0 = bright_pen_col;
			*(Uint16*)p1 = dark_pen_col;
		}
		i = corner;
		for(;i--;p0+=surface->pitch)
			*(Uint16*)p0 = bright_pen_col;	
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------

// Halbkreis
	//???
	{
		unsigned int rightInc = 6;
		signed int diagonalInc = 10 - (corner<<2);
		signed int d = 3 - (corner<<1);
		unsigned int radius = corner;
		unsigned int i = 0;
		
		while (i <= radius) 
		{
			for(unsigned int k=i;k--;)
			{
				*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((k + Xcenter - i)<<1) )) = brush_col;
				*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((k + X2center)<<1) )) = brush_col;
				*((Uint16*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + ((k + X2center)<<1) )) = brush_col;
			}
			for(unsigned int k=radius;k--;)
			{
				*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((k + Xcenter - radius)<<1) )) = brush_col;
				*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((k + X2center)<<1) )) = brush_col;
				*((Uint16*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + ((k + X2center)<<1) )) = brush_col;
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
		unsigned int rightInc = 6;
		signed int diagonalInc = 10 - (corner<<2);
		signed int d = 3 - (corner<<1);
		unsigned int radius = corner;
		unsigned int i = 0;
	
		while (i <= radius) 
		{
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((Xcenter - i)<<1) )) = bright_pen_col;
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((Xcenter - radius)<<1) )) = bright_pen_col;

			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((X2center + i)<<1) )) = pen_col;
			*((Uint16*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((X2center + radius)<<1) )) = pen_col;

			*((Uint16*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + ((X2center + i)<<1) )) = dark_pen_col;
			*((Uint16*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + ((X2center + radius)<<1) )) = dark_pen_col;
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

}


void DC::DrawFilledEdgedBorderRound_24bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
// Ecke links oben
	signed int Xcenter = x+corner;
	signed int Ycenter = y+corner;
// Kantenlaengen
	unsigned int dx = width - (corner << 1);
	unsigned int dy = height - (corner << 1);
// Ecke rechts unten
	signed int X2center = Xcenter + dx - 1;
	signed int Y2center = Ycenter + dy - 1;
	Uint32 brush_col = (Uint32)(*brush.GetColor());

// Rechteck von linker Kante nach rechter Kante
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
		r.w = width-corner-1; r.h = corner;
		SDL_FillRect(surface, &r, brush_col);
	}
// ------- END OF Rechteck oben und unten ------

	Uint32 dark_pen_col, bright_pen_col, pen_col;
	
	if(pressedRectangle)
	{
		dark_pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 70));
		bright_pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 110));
		pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 90));
	
	} else
	{
		dark_pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 80));
		bright_pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 120));
		pen_col = (Uint32)(*pen.GetColor());
	}

//	Uint32 pen_col = (Uint32)(*pen.GetColor());

	Uint8 colorbyte0 = (Uint8) (pen_col & 0xff);
	Uint8 colorbyte1 = (Uint8) ((pen_col >> 8) & 0xff);
	Uint8 colorbyte2 = (Uint8) ((pen_col >> 16) & 0xff);
	
	Uint8 dark_colorbyte0 = (Uint8) (dark_pen_col & 0xff);
	Uint8 dark_colorbyte1 = (Uint8) ((dark_pen_col >> 8) & 0xff);
	Uint8 dark_colorbyte2 = (Uint8) ((dark_pen_col >> 16) & 0xff);
	
	Uint8 bright_colorbyte0 = (Uint8) (bright_pen_col & 0xff);
	Uint8 bright_colorbyte1 = (Uint8) ((bright_pen_col >> 8) & 0xff);
	Uint8 bright_colorbyte2 = (Uint8) ((bright_pen_col >> 16) & 0xff);

// ------ TOP AND LOWER HORIZONTAL LINE ------
	Lock();
	{
		register Uint8* p;
		register signed int k;
		p = (Uint8*)surface->pixels + y * surface->pitch + Xcenter * 3;k = dx;SDL_DO_BRIGHT_DRAWING_X
		p = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + x * 3;k = dx+corner;SDL_DO_DARK_DRAWING_X
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

	
// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		register Uint8* p;
		register signed int k;
		
		p = (Uint8*)surface->pixels + Ycenter * surface->pitch + x * 3;k = height-corner-1;SDL_DO_BRIGHT_DRAWING_Y
		p = (Uint8*)surface->pixels + Ycenter * surface->pitch + (x+width-1) * 3;k = dy;SDL_DO_DARK_DRAWING_Y
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------


// Halbkreis

	colorbyte0 = (Uint8) (brush_col & 0xff);
	colorbyte1 = (Uint8) ((brush_col >> 8) & 0xff);
	colorbyte2 = (Uint8) ((brush_col >> 16) & 0xff);
	{
		unsigned int rightInc = 6;
		signed int diagonalInc = 10 - (corner << 2);
		signed int d = 3 - (corner << 1);
		unsigned int radius = corner;
		unsigned int i = 0;
		
		while (i <= radius)
		{
			register Uint8 *p;
			register signed int k;
			p = (Uint8*)surface->pixels + (Ycenter-radius) * surface->pitch + (Xcenter-i) * 3; k = i; SDL_DO_DRAWING_X
			p = (Uint8*)surface->pixels + (Ycenter-radius) * surface->pitch + (X2center) * 3; k = i; SDL_DO_DRAWING_X
			p = (Uint8*)surface->pixels + (Y2center+radius) * surface->pitch + (X2center) * 3; k = i; SDL_DO_DRAWING_X
			p = (Uint8*)surface->pixels + (Ycenter-i) * surface->pitch + (Xcenter-radius) * 3; k = radius; SDL_DO_DRAWING_X
			p = (Uint8*)surface->pixels + (Ycenter-i) * surface->pitch + (X2center) * 3; k = radius; SDL_DO_DRAWING_X
			p = (Uint8*)surface->pixels + (Y2center+i) * surface->pitch + (X2center) * 3; k = radius; SDL_DO_DRAWING_X

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

	colorbyte0 = (Uint8) (pen_col & 0xff);
	colorbyte1 = (Uint8) ((pen_col >> 8) & 0xff);
	colorbyte2 = (Uint8) ((pen_col >> 16) & 0xff);

	{
		unsigned int rightInc = 6;
		signed int diagonalInc = 10 - (corner << 2);
		signed int d = 3 - (corner << 1);
		unsigned int radius = corner;
		register Uint8 *p;
	
		unsigned int i = 0;
		while (i <= radius) 
		{
			p = (Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (Xcenter - i)*3; SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX
			p = (Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (Xcenter - radius)*3; SDL_DRAW_BRIGHT_PUTPIXEL_BPP_3_AUX
			
			p = (Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (X2center + i)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
			p = (Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (X2center + radius)*3; SDL_DRAW_PUTPIXEL_BPP_3_AUX
			
			p = (Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + (X2center + i)*3; SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX
			p = (Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (X2center + radius)*3; SDL_DRAW_DARK_PUTPIXEL_BPP_3_AUX
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

}


void DC::DrawFilledEdgedBorderRound_32bit(const signed int x, const signed int y, const unsigned int width, const unsigned int height, const unsigned int corner) const
{
// Ecke links oben
	signed int Xcenter = x+corner;
	signed int Ycenter = y+corner;
// Kantenlaengen
	unsigned int dx = width - (corner << 1);
	unsigned int dy = height - (corner << 1);
// Ecke rechts unten
	signed int X2center = Xcenter + dx - 1;
	signed int Y2center = Ycenter + dy - 1;
	Uint32 brush_col = (Uint32)(*brush.GetColor());

// Rechteck von linker Kante nach rechter Kante
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
		r.w = width-corner-1; r.h = corner;
		SDL_FillRect(surface, &r, brush_col);
	}
// ------- END OF Rechteck oben und unten ------
	Uint32 dark_pen_col, bright_pen_col, pen_col;
	if(pressedRectangle)
	{
		dark_pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 110));
		bright_pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 70));
		pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 90));
	
	} else
	{
		dark_pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 80));
		bright_pen_col = (Uint32)(changeRelativeBrightness(*pen.GetColor(), 120));
		pen_col = (Uint32)(*pen.GetColor());
	}
// ------ TOP AND LOWER HORIZONTAL LINE ------
	{
		register Uint8* p0 = (Uint8*)surface->pixels + y * surface->pitch + (Xcenter<<2);
		register Uint8* p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + (x<<2);
		Lock();
		if (sizeof(wchar_t) == sizeof(Uint32)) { 
#ifdef __linux__
			wmemset((wchar_t*)p0, bright_pen_col, dx);
			wmemset((wchar_t*)p1, dark_pen_col, width - corner);
#elif __WIN32__
			memset((wchar_t*)p0, bright_pen_col, dx);
			memset((wchar_t*)p1, dark_pen_col, width - corner);
#endif
		} else 
		{
			register unsigned int i = corner;
			for(;i--;p1+=4)
				*(Uint32*)p1 = dark_pen_col;
			i = dx;
			for(;i--;p0+=4,p1+=4)
			{
				*(Uint32*)p0 = bright_pen_col;
				*(Uint32*)p1 = dark_pen_col;
			}
		}
	}
// ------ END OF TOP AND LOWER HORIZONTAL LINE ------

	
// ------ LEFT AND RIGHT VERTICAL LINE ------
	{
		register Uint8* p0 = (Uint8*)surface->pixels + Ycenter*surface->pitch + (x<<2);	 
		register Uint8* p1 = (Uint8*)surface->pixels + Ycenter*surface->pitch + ((x+width-1)<<2);
		register unsigned int i = dy;
		for(;i--;p0+=surface->pitch, p1+=surface->pitch)
		{
			*(Uint32*)p0 = bright_pen_col;
			*(Uint32*)p1 = dark_pen_col;
		}
		i = corner;
		for(;i--;p0+=surface->pitch)
			*(Uint32*)p0 = bright_pen_col;	
	}
// ------ END OF LEFT AND RIGHT VERTICAL LINE ------

// Halbkreis
	{
		unsigned int rightInc = 6;
		signed int diagonalInc = 10 - (corner << 2);
		signed int d = 3 - (corner << 1);
		unsigned int radius = corner;
		unsigned int i = 0;
		
		while (i <= radius) 
		{
			for(unsigned int k=i;k--;)
			{
				*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((k + Xcenter - i)<<2) )) = brush_col;
				*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((k + X2center)<<2) )) = brush_col;
				*((Uint32*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + ((k + X2center)<<2) )) = brush_col;
			}
			for(unsigned int k=radius;k--;)
			{
				*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((k + Xcenter - radius)<<2) )) = brush_col;
				*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((k + X2center)<<2) )) = brush_col;
				*((Uint32*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + ((k + X2center)<<2) )) = brush_col;
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
		unsigned int rightInc = 6;
		signed int diagonalInc = 10 - (corner<<2);
		signed int d = 3 - (corner << 1);
		unsigned int radius = corner;
		unsigned int i = 0;
		
		while (i <= radius) 
		{	
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((Xcenter - i)<<2))) = bright_pen_col;
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((Xcenter - radius)<<2))) = bright_pen_col;

			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + ((X2center + i)<<2))) = pen_col;
			*((Uint32*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + ((X2center + radius)<<2))) = pen_col;

			*((Uint32*)((Uint8*)surface->pixels + (Y2center+radius)*surface->pitch + ((X2center + i)<<2))) = dark_pen_col;
			*((Uint32*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + ((X2center + radius)<<2))) = dark_pen_col;

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

}




