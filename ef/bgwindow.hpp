#ifndef _GUI_BOGRAPH_HPP
#define _GUI_BOGRAPH_HPP

#include "../ui/window.hpp"
#include "bograph.hpp"
#include "../core/anabuildorder.hpp"

#include <list>

#define BOGRAPH_MAX_LINES 20

class BoGraphWindow:public UI_Window
{
	public:
		BoGraphWindow(UI_Object* bograph_parent, const unsigned int bograph_window_number);
		BoGraphWindow(const BoGraphWindow& object);
		BoGraphWindow& operator=(const BoGraphWindow& object);
		~BoGraphWindow();
		
		void resetData();
		void process();
		void processList();
		void draw(DC* dc) const;
	    	void mouseHasMoved();
		void assignAnarace(ANABUILDORDER* bograph_anarace);
	private:

		struct BOGRAPH
		{
			unsigned int facility;
			unsigned int height; // number of entries per line
			unsigned int lines; // number of lines
			Rect edge;
			std::list<BoGraphEntry*> boGraphList;
		} bograph[BOGRAPH_MAX_LINES];
	
		void checkForInfoWindow();
		unsigned int markAni;
		ANABUILDORDER* anarace;
};

inline void BoGraphWindow::assignAnarace(ANABUILDORDER* bograph_anarace) {
	anarace = bograph_anarace;
}

#endif

