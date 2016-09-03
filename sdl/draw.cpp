#include "dc.hpp"

#include "string.h"
#include "wchar.h"

#define SDL_DRAW_BPP 4

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
    	  p[0] = colorbyte2;                   \
	      p[1] = colorbyte1;                   \
	      p[2] = colorbyte0;                   \
    	} else {                               \
	      p[0] = colorbyte0;                   \
    	  p[1] = colorbyte1;                   \
	      p[2] = colorbyte2;                   \
    	}
		i = x1-x0+1;                                  
		switch( i % 4 ) {                            
			do{                                       
				case 0: SDL_DRAW_PUTPIXEL_BPP_3_AUX  p+=3;
				case 3: SDL_DRAW_PUTPIXEL_BPP_3_AUX  p+=3;
				case 2: SDL_DRAW_PUTPIXEL_BPP_3_AUX  p+=3;
				case 1: SDL_DRAW_PUTPIXEL_BPP_3_AUX  p+=3;
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
  {                      /* x dominant */
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
  {                      /* y dominant */
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
#if 0

void DC::DrawLine(const signed int xx1, const signed int yy1, const signed int xx2, const signed int yy2) const
{
	if(pen.GetStyle()==TRANSPARENT_PEN_STYLE)
		return;
	Color col = (*pen.GetColor());

//	aalineRGBA(surface, xx1, yy1, xx2, yy2,	col.r(), col.g(), col.b(), 255);
//	return; // TODO

	int x0 = xx1;
	int x1 = xx2;
	int y0 = yy1;
	int y1 = yy2;

 int i;
int sx, sy; /* step positive or negative (1 or -1) */
int dx, dy; /* delta (difference in X and Y between points) */
int dx2, dy2;
int e;
int temp;


 dx = x1 - x0;
sx = (dx > 0) ? 1 : -1;
if (dx < 0)
dx = -dx;

 dy = y1 - y0;
sy = (dy > 0) ? 1 : -1;
if (dy < 0)
dy = -dy;

 dx2 = dx << 1; /* dx2 = 2 * dx */
dy2 = dy << 1; /* dy2 = 2 * dy */

 if (dy <= dx) { /* steep */
e = dy2 - dx;

 for (i = 0; i < dx; ++i) {
	*((Uint32*)((Uint8*)surface->pixels + y0*surface->pitch + x0*4))=col;
/*	Uint32* col2 = ((Uint32*)((Uint8*)surface->pixels + y0*surface->pitch + x0*4));
	Uint32 col3 = (((*col2>>24)+(col>>24))<<23) + 
				  (( ((*col2<<8)>>24) + ((col<<8)>>24) )<<15) + 
				  (( ((*col2<<16)>>24) + ((col<<16)>>24) )<<7) + 
				  (( ((*col2<<24)>>24) + ((col<<24)>>24) )>>1);

	//Uint32 col3 = (((*col2>>24)+(col>>24))>>1) + ((col<<8)>>8);
	*col2 = col3;*/

 while (e >= 0) {
y0 += sy;
e -= dx2;
}

 x0 += sx;
e += dy2;
}
} 
else {
/* swap x0 <-> y0 */
temp = x0;
x0 = y0;
y0 = temp;

 /* swap dx <-> dy */
temp = dx;
dx = dy;
dy = temp;

 /* swap dx2 <-> dy2 */
temp = dx2;
dx2 = dy2;
dy2 = temp;

 /* swap sx <-> sy */
temp = sx;
sx = sy;
sy = temp;

 e = dy2 - dx;

 for (i = 0; i < dx; ++i) {
	*((Uint32*)((Uint8*)surface->pixels + x0*surface->pitch + y0*4))=col;
    /*Uint32* col2 = ((Uint32*)((Uint8*)surface->pixels + x0*surface->pitch + y0*4));
	Uint32 col3 = (((*col2>>24)+(col>>24))<<23) + 
				  (( ((*col2<<8)>>24) + ((col<<8)>>24) )<<15) + 
				  (( ((*col2<<16)>>24) + ((col<<16)>>24) )<<7) + 
				  (( ((*col2<<24)>>24) + ((col<<24)>>24) )>>1);
    *col2 = col3;*/
			

 while (e >= 0) {
y0 += sy;
e -= dx2;
}

 x0 += sx;
e += dy2;
}
}

} 
#endif 

// TODO!
#if 0
void DC::DrawLine(const signed int x1, const signed int y1, const signed int x2, const signed int y2) const
{
	if(pen.GetStyle()==TRANSPARENT_PEN_STYLE) 
		return;
	
	Uint32 col = (Uint32)(*pen.GetColor());
	int xx1 = x1;
	int xx2 = x2;
	int yy1 = y1;
	int yy2 = y2;

	Sint16 x = xx1;
	Sint16 y = yy1;
	Sint16 dy = yy2 - yy1;
	Sint16 dx = xx2 - xx1;

  Sint16 G, DeltaG1, DeltaG2, minG, maxG;
  Sint16 swap;
  Sint16 inc = 1;
// first pixel:
//	*((Uint32*)((Uint8*)surface->pixels + y*surface->pitch + x*4))=col;
// last pixel:
//	*((Uint32*)((Uint8*)surface->pixels + y2*surface->pitch + x2*4))=col;

	if (abs(dy) < abs(dx)) { /* -1 < ramp < 1 */
		if (dx < 0) {
			dx = -dx;
        	dy = -dy;

	        swap = yy2;
    	    yy2 = yy1;
        	yy1 = swap;

	        swap = xx2;
    	    xx2 = xx1;
        	xx1 = swap;
		}
		if (dy < 0) {
			dy = -dy;
			inc = -1;
		}
		G = 2 * dy - dx;
		DeltaG1 = 2 * (dy - dx);
		DeltaG2 = 2 * dy;

		while (x++ < x2) {
			if (G > 0) 
			{ 
				G += DeltaG1; 
				y += inc; 
			}
			else  G += DeltaG2;

			*((Uint32*)((Uint8*)surface->pixels + y*surface->pitch + x*4))=col;
		}/*while*/

	} else { /* ramp < -1 or ramp > 1 */
		if (dy < 0) {
			dx = -dx;
        	dy = -dy;

        	swap = yy2;
    	    yy2 = yy1;
	        yy1 = swap;

        	swap = xx2;
    	    xx2 = xx1;
	        xx1 = swap;
		}
		if (dx < 0) {
			dx = -dx;
			inc = -1;
		}

		G = 2 * dx - dy;
		minG = maxG = G;
		DeltaG1 = 2 * (dx - dy);
		DeltaG2 = 2 * dx;

		while (y++ < y2) {
	        if (G > 0) { G += DeltaG1; x += inc; }
    	    else  G += DeltaG2;

		*((Uint32*)((Uint8*)surface->pixels + y*surface->pitch + x*4))=col;
      }/*while*/

  }/*if*/

}/*Draw_Line*/
#endif

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
	
	unsigned int i = 0;
	while (i <= radius) 
	{

// Nur Ecken:
    // links oben
#ifdef __linux__

/*        for(int k=i;k--;)
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
/*        memset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (Xcenter - i)*4), col, i);
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
/*    // links oben
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
/*        wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + (Xcenter - i)*4), col, dx + 2*i);
        wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + (Xcenter - radius)*4), col, dx + 2*radius);
    // rechts oben
//        wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-radius)*surface->pitch + X2center*4), col, i);
//        wmemset((wchar_t*)((Uint8*)surface->pixels + (Ycenter-i)*surface->pitch + X2center*4), col, radius);
    // links unten
        wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + (Xcenter - i)*4), col, dx + 2*i);
        wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + (Xcenter - radius)*4), col, dx + 2*radius);
    // rechts unten
//        wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+radius-1)*surface->pitch + X2center*4), col, i);
  //      wmemset((wchar_t*)((Uint8*)surface->pixels + (Y2center+i)*surface->pitch + X2center*4), col, radius);*/

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

	register Uint8* p0 = (Uint8*)surface->pixels +    y    *      surface->pitch + Xcenter*4;
	register Uint8* p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + Xcenter*4;
	register Sint16 i=dx;              

#ifdef __linux__
#define SDL_DRAW_WMEMSET_START \
if (sizeof(wchar_t) == sizeof(Uint32)) {        \
  wmemset((wchar_t*)p0, col, dx);             \
  wmemset((wchar_t*)p1, col, dx);             \
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
  if (height<3)  return;                         
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
	unsigned int radius = corner;
	Uint32 col = (Uint32)(*pen.GetColor());

	register Uint8 *p0;
  register Uint8 *p1;
  register Sint16 i;
  Sint16 dx, dy;
  Sint16 Xcenter, Ycenter, X2center, Y2center;

  Sint16 rightInc = 6;
  Sint16 d, diagonalInc;

  if (width==0 || height==0)  return;

  /*TODO: We can do better :-)*/
    d = width<height ? width : height;
//  d = 3 - (radius<<1); WTF?
  diagonalInc = 10 - (radius<<2);

  /*Rectangles*/
  dx = width - (radius<<1);
  Xcenter = x+radius;
  dy = height - (radius<<1);
  Ycenter = y+radius;

  /*Centers*/
  X2center=Xcenter+dx-1;
  Y2center=Ycenter+dy-1;

  p0 = (Uint8*)surface->pixels +    y    * surface->pitch + Xcenter*SDL_DRAW_BPP;
  p1 = (Uint8*)surface->pixels + (y+height-1) * surface->pitch + Xcenter*SDL_DRAW_BPP;

#ifdef __linux__
#define SDL_DRAW_WMEMSET_START \
if (sizeof(wchar_t) == sizeof(Uint32)) {        \
  wmemset((wchar_t*)p0, col, dx);             \
  wmemset((wchar_t*)p1, col, dx);             \
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
  if (height<3)  return;                         
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



