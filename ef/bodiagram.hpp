#ifndef _GUI_BODIAGRAM_HPP
#define _GUI_BODIAGRAM_HPP

#include "../ui/window.hpp"
#include "../core/settings.hpp"
#include "info.hpp"

class BoDiagramWindow : public UI_Window 
{
	public:
		BoDiagramWindow(UI_Object * bod_parent, ANARACE * bod_anarace, InfoWindow * bod_info_window, const unsigned int bod_window_number);
		BoDiagramWindow(const BoDiagramWindow& object);
		BoDiagramWindow& operator=(const BoDiagramWindow& object);
		~BoDiagramWindow();

		void resetData();
		void process();
		void processList();
		void draw(DC * dc) const;

		void assignAnarace(ANARACE* bod_anarace);

	private:
		ANARACE* anarace;
		InfoWindow* infoWindow;

		unsigned int count;

		Point minerals[MAX_TIME];
		Point gas[MAX_TIME];
		Point hneedSupply[MAX_TIME];
		Point nneedSupply[MAX_TIME];
		Point fitness[MAX_TIME];

		Point startMinerals[MAX_TIME];
		Point startGas[MAX_TIME];
		Point startHneedSupply[MAX_TIME];
		Point startNneedSupply[MAX_TIME];
		Point startFitness[MAX_TIME];

		Point targetMinerals[MAX_TIME];
		Point targetGas[MAX_TIME];
		Point targetHneedSupply[MAX_TIME];
		Point targetNneedSupply[MAX_TIME];
		Point targetFitness[MAX_TIME];
	
};

#endif // _GUI_BODIAGRAM_HPP
