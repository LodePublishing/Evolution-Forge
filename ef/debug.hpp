#ifndef _DEBUG_WINDOW_HPP
#define _DEBUG_WINDOW_HPP

#include "../ui/window.hpp"
#include "../sdl/framerate.hpp"
#include "../ui/group.hpp"

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


class DebugWindow : public UI_Window
{
	public:
		DebugWindow(UI_Object* parent_window);
		~DebugWindow();
		void draw(DC* dc) const;
		void process();

		void delay(const bool optimizing);
		void poll(const eTicks etick);
		const bool allowCalculation();
		void updateConfiguration();
		void reloadOriginalSize();
	private:
		FPS* fps;
		long unsigned int currentTicks;
		long unsigned int ticks[MAX_TICK_TYPES][TICK_INTERVALL];
		unsigned int frames_count[TICK_INTERVALL];
		unsigned int framesPerSecond;
		signed int percent[MAX_TICK_TYPES];
		long unsigned int totalTicks;
		unsigned int tickIntervall;
		unsigned int framesPerGeneration;
		unsigned int refresh;

		UI_Group* tickGroup;
		UI_StaticText* tickText[MAX_TICK_TYPES];
		
		UI_StaticText* fpsText;
		UI_StaticText* gpsText;

		unsigned int generationsPerSecond;
		
		void updateDesiredValues();
};

#endif
