#ifndef _DEBUG_WINDOW_HPP
#define _DEBUG_WINDOW_HPP

#include "../ui/window.hpp"
#include "../ui/group.hpp"
#include "../ui/framerate.hpp"

class DebugWindow : public UI_Window
{
	public:
		DebugWindow(UI_Object* parent_window);
		~DebugWindow();
		void draw() const;
		void process();
		void updateData(const std::list<unsigned int> percent_list, const unsigned int fps, const unsigned int gps);
		void reloadOriginalSize();
	private:
		unsigned int framesPerSecond;
		unsigned int generationsPerSecond;
		std::list<unsigned int> percentList;

		UI_Group* tickGroup;
		UI_StaticText* tickText[MAX_TICK_TYPES];
		
		UI_StaticText* fpsText;
		UI_StaticText* gpsText;
};

#endif
