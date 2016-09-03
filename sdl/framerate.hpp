#ifndef _SDL_FRAMERATE_HPP
#define _SDL_FRAMERATE_hpp
#include <SDL.h>

#define FPS_UPPER_LIMIT		200
#define FPS_LOWER_LIMIT		1
#define FPS_DEFAULT		30

class FPS
{
	public:
		FPS();
		~FPS();
		const bool setDesiredFramerate(const unsigned int desired_frame_rate);
		const bool setDesiredCPU(const unsigned int desired_cpu_usage);
		void setTotalTicks(const long unsigned int total_ticks);
		void setAdaptFramesPerGeneration(const bool adapt);
		const unsigned int getFramesPerGeneration() const;
		const unsigned int getCurrentFramerate() const;
    		void delay();
	private:
		unsigned int desiredFramerate;
		unsigned int currentFramerate;
		unsigned int desiredCPU;
		unsigned int framesPerGeneration; // output

		unsigned int framecount;
		
		float rateTicks;
		
		long unsigned int startTicks;
		long unsigned int lastTicks;
		long unsigned int totalTicks;

		long unsigned int delayTicks;

		bool adaptFramesPerGeneration;
		
		unsigned int averagecounter;
		unsigned int average[100];
};

inline void FPS::setAdaptFramesPerGeneration(const bool adapt) {
	adaptFramesPerGeneration = adapt;
}

inline const unsigned int FPS::getFramesPerGeneration() const {
	return(framesPerGeneration);
}

inline const unsigned int FPS::getCurrentFramerate() const {
	return (currentFramerate);
}

#endif	/* _SDL_FRAMERATE_HPP */

