#ifndef _GUI_BODIAGRAM_HPP
#define _GUI_BODIAGRAM_HPP

#include "../ui/window.hpp"
#include "info.hpp"

class BoDiagramWindow:public UI_Window {
	public:
		BoDiagramWindow(UI_Object * parent, ANARACE * anarace,
						InfoWindow * infoWindow, const int windowNumber);
		~BoDiagramWindow();

		void resetData();
		void process();
		void draw(DC * dc) const;

	private:
		ANARACE* anarace;
		InfoWindow* infoWindow;
};

#endif // _GUI_BODIAGRAM_HPP
