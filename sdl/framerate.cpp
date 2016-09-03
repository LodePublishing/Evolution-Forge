#include "framerate.hpp"

FPS::FPS():
	allowStaticFramerate(true),
	desiredFramerate(FPS_DEFAULT),
	currentFramerate(FPS_DEFAULT),
	framesPerGeneration(100),
	framecount(0),
	rateTicks(1000.0 / (float) FPS_DEFAULT),
	startTicks(SDL_GetTicks()),
	lastTicks(SDL_GetTicks()),
	averagecounter(0)
{
	for(unsigned int i=100;i--;)
		average[i]=20;
}

FPS::~FPS()
{ }

void FPS::setDesiredFramerate(const unsigned int desired_frame_rate)
{
	if(desiredFramerate == desired_frame_rate)
		return;
	framecount = 0;
	desiredFramerate = desired_frame_rate;
	rateTicks = (1000.0 / (float) desiredFramerate);
}

const unsigned int FPS::getFramesPerGeneration() const
{
	return(framesPerGeneration);
}

const unsigned int FPS::getCurrentFramerate() const
{
	return (currentFramerate);
}

void FPS::setAllowStaticFramerate(const bool allow_static_framerate)
{
	allowStaticFramerate = allow_static_framerate;
}

void FPS::delay()
{
// Increase/Reduce the frames per generation
	long int difference = SDL_GetTicks() - startTicks;
	if(difference==0)
		difference=1;
   	startTicks += difference;

	if(averagecounter<2)
   		averagecounter++;
	for(unsigned int i=averagecounter;i--;)
   		average[i+1]=average[i];
	average[0]=difference;
																				
   	long int av=0;
	for(unsigned int i=0;i<averagecounter;i++)
   		av += average[i];
	currentFramerate = 1000*averagecounter / av;

	if(!allowStaticFramerate)
	{
		if((currentFramerate > desiredFramerate) && (framesPerGeneration>1))
			framesPerGeneration--;
		else 
		if((currentFramerate < desiredFramerate) && (framesPerGeneration<10000))
			framesPerGeneration++;
	} else
	{
		framesPerGeneration=100;
		Uint32 current_ticks;
		Uint32 target_ticks;
		Uint32 the_delay;

		framecount++;
		current_ticks = SDL_GetTicks();
		target_ticks = lastTicks + (Uint32) ((float) framecount * rateTicks);

		if (current_ticks <= target_ticks) {
			the_delay = target_ticks - current_ticks;
			SDL_Delay(the_delay);
		} else {
			framecount = 0;
			lastTicks = SDL_GetTicks();
		}
	}
}

