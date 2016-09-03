#ifndef _SDL_framerate_hpp
#define _SDL_framerate_hpp
#include <SDL/SDL.h>

/* --------- Definitions */

#define FPS_UPPER_LIMIT		200
#define FPS_LOWER_LIMIT		1
#define FPS_DEFAULT		30

/* Set up for C function definitions, even when using C++ */
class FPS
{
/* --------- Structure variables */
	private:
		unsigned int framecount;
		unsigned int rate;
		float rateticks;
		unsigned int lastticks;

/* --------- Function prototypes */

	public:
/* Functions return 0 or value for sucess and -1 for error */
		FPS();
		~FPS();
		bool setFramerate(const unsigned int rate);
	    const unsigned int getFramerate();
    	void delay();

/* --- */

};

#endif				/* _SDL_framerate_hpp */

