#ifndef _FPS_SYSTEM_HPP
#define _FPS_SYSTEM_HPP

#include "../ui/object.hpp"
#include "../sdl/framerate.hpp"

#define TICK_INTERVALL 2

enum eTicks
{
	PROCESS_TICKS,
	DRAW_TICKS,
	SOUND_TICKS,
	GENERATION_TICKS,
	MESSAGE_TICKS,
	IDLE_TICKS,
	
	MAX_TICK_TYPES
};


class FPS_SYSTEM
{
	public:
		FPS_SYSTEM();
		~FPS_SYSTEM();
		void draw(DC* dc) const;
		void process();

		void delay(const bool optimizing);
		void poll(const eTicks etick);
		const bool allowCalculation();
		void updateConfiguration();
	private:
		FPS* fps;
		long unsigned int current_ticks;
		long unsigned int ticks[MAX_TICK_TYPES][TICK_INTERVALL];
		unsigned int frames_count[TICK_INTERVALL];
		unsigned int frames_per_second;
		signed int percent[MAX_TICK_TYPES];
		long unsigned int total_ticks;
		unsigned int tick_intervall;
		unsigned int frames_per_generation;
		unsigned int refresh;
};

#endif
