
/*

 SDL_framerate: framerate manager

 LGPL (c) A. Schiffler
 
 */

#include "framerate.hpp"

/* 
   Initialize the framerate manager
*/

FPS::FPS():
	framecount(0),
	rate(FPS_DEFAULT),
	rateticks(1000.0 / (float) FPS_DEFAULT),
	lastticks(SDL_GetTicks())
{
}

FPS::~FPS()
{
}

/* 
   Set the framerate in Hz 
*/

bool FPS::setFramerate(const unsigned int frame_rate)
{
    if ((frame_rate >= FPS_LOWER_LIMIT) && (frame_rate <= FPS_UPPER_LIMIT)) {
		framecount = 0;
		rate = frame_rate;
		rateticks = (1000.0 / (float) rate);
		return (true);
    } else {
		return (false);
    }
}

/* 
  Return the current target framerate in Hz 
*/

const unsigned int FPS::getFramerate()
{
	return (rate);
}

/* 
  Delay execution to maintain a constant framerate. Calculate fps.
*/

void FPS::delay()
{
    Uint32 current_ticks;
    Uint32 target_ticks;
    Uint32 the_delay;

    /*
     * Next frame 
     */
    framecount++;

    /*
     * Get/calc ticks 
     */
    current_ticks = SDL_GetTicks();
    target_ticks = lastticks + (Uint32) ((float) framecount * rateticks);

    if (current_ticks <= target_ticks) {
		the_delay = target_ticks - current_ticks;
		SDL_Delay(the_delay);
    } else {
		framecount = 0;
		lastticks = SDL_GetTicks();
    }
}

