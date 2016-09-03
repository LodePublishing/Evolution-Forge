#ifndef _UI_FRAMERATE_HPP
#define _UI_FRAMERATE_HPP

#include "../sdl/framerate.hpp"

#define TICK_INTERVALL 2

enum eTicks
{
	DRAW_TICKS,
	GENERATION_TICKS,
	PROCESS_TICKS,
	MESSAGE_TICKS,
	SOUND_TICKS,
	IDLE_TICKS,
	
	MAX_TICK_TYPES
};

#include <list>

class UI_FrameRateControl
{
	public:
		UI_FrameRateControl();
		~UI_FrameRateControl();

		void delay(const bool optimizing);
		void poll(const eTicks etick);
		const bool allowCalculation();
		void updateConfiguration();

		const std::list<unsigned int> getPercentList() const;
		const unsigned int getFramesPerSecond() const;
		const unsigned int getGenerationsPerSecond() const;
	private:
		FPS* fps;
		long unsigned int currentTicks;
		long unsigned int ticks[MAX_TICK_TYPES][TICK_INTERVALL];
		unsigned int frames_count[TICK_INTERVALL];
		unsigned int framesPerSecond;
		unsigned int generationsPerSecond;
		signed int percent[MAX_TICK_TYPES];
		long unsigned int totalTicks;
		unsigned int tickIntervall;
		unsigned int framesPerGeneration;
		unsigned int refresh;

		const bool updateDesiredValues();
		void calculateFramerate();
};

#endif
