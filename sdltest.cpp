#include <stdlib.h>
#include <SDL.h>

int main(int argc, char *argv[])
{
    SDL_Surface *screen, *image;
	SDL_Rect dst, dstblue;
    SDL_Event event;
	Uint8 *keys;
	int tuxX = 50, tuxY = 50;
    bool done = false;
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        printf("Can't init SDL:  %s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit); 
    screen = SDL_SetVideoMode(640, 480, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 255));
    if(screen == NULL)
    {
        printf("Can't set video mode: %s\n", SDL_GetError());
        exit(1);
    }
    image = SDL_LoadBMP("tux.bmp");
    if(image == NULL)
    {
        printf("Can't load image of tux: %s\n", SDL_GetError());
        exit(1);
    }	
	dst.w = image->w;
	dst.h = image->h;
	dstblue.w = image->w + 1;
	dstblue.h = image->h + 1;
    SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(image->format, 255, 0, 255));    
    while(!done)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                done = true;
                break;
            }
        }
		keys = SDL_GetKeyState(NULL);
		if(keys[SDLK_UP])
		{
			if(tuxY > 10)
				tuxY--;
		}
		if(keys[SDLK_DOWN])
		{
			if(tuxY < 470 - image->h)
				tuxY++;
		}
		if(keys[SDLK_RIGHT])
		{
			if(tuxX < 630 - image->w)
				tuxX++;
		}
		if(keys[SDLK_LEFT])
		{
			if(tuxX > 10)
				tuxX--;
		}
		dst.x = tuxX;
		dst.y = tuxY;
		dstblue.x = tuxX - 1;
		dstblue.y = tuxY - 1;
		SDL_FillRect(screen, &dstblue, SDL_MapRGB(screen->format, 0, 0, 255));
		SDL_BlitSurface(image, NULL, screen, &dst);
		SDL_Flip(screen);
    }
    SDL_FreeSurface(image);
    return 0;
}


