#ifndef _SDL_FRAMERATE_HPP
#define _SDL_FRAMERATE_hpp
#include <SDL.h>

/* --------- Definitions */

#define FPS_UPPER_LIMIT		200
#define FPS_LOWER_LIMIT		1
#define FPS_DEFAULT		30

/* Set up for C function definitions, even when using C++ */
class FPS
{
/* --------- Structure variables */
	private:
		bool allowStaticFramerate;
		unsigned int desiredFramerate;
		unsigned int currentFramerate;
		unsigned int framesPerGeneration; // output

		unsigned int framecount;
		
		float rateTicks;
		
		long unsigned int startTicks;
		long unsigned int lastTicks;
		
		
		unsigned int averagecounter;
		unsigned int average[100];
		
/* --------- Function prototypes */

	public:
		FPS();
		~FPS();
		void setDesiredFramerate(const unsigned int desired_frame_rate);
		const unsigned int getFramesPerGeneration() const;
		const unsigned int getCurrentFramerate() const;
		void setAllowStaticFramerate(const bool allow_static_framerate=true);
    	void delay();

/* --- */

};

#endif				/* _SDL_framerate_hpp */

