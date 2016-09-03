#ifndef _GUI_BODIAGRAM_HPP
#define _GUI_BODIAGRAM_HPP

#include "../ui/window.hpp"
#include "../core/settings.hpp"
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

		int count;

		Point minerals[MAX_TIME];
		Point gas[MAX_TIME];
		Point hneedSupply[MAX_TIME];
		Point nneedSupply[MAX_TIME];

		Point startMinerals[MAX_TIME];
		Point startGas[MAX_TIME];
		Point startHneedSupply[MAX_TIME];
		Point startNneedSupply[MAX_TIME];

		Point targetMinerals[MAX_TIME];
		Point targetGas[MAX_TIME];
		Point targetHneedSupply[MAX_TIME];
		Point targetNneedSupply[MAX_TIME];
	
};

#endif // _GUI_BODIAGRAM_HPP
