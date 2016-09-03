#ifndef _GUI_BODIAGRAM_HPP
#define _GUI_BODIAGRAM_HPP

#include "../ui/window.hpp"
#include "../core/database.hpp"
#include "../core/anabuildorder.hpp"

class BoDiagramWindow : public UI_Window 
{
	public:
		BoDiagramWindow(UI_Object* bod_parent, const unsigned int bod_window_number);
		BoDiagramWindow(const BoDiagramWindow& object);
		BoDiagramWindow& operator=(const BoDiagramWindow& object);
		~BoDiagramWindow();

		void resetData();
		void process();
		void processList();
		void draw(DC * dc) const;

		void assignAnarace(ANABUILDORDER* bod_anarace);

	private:
		ANABUILDORDER* anarace;

		unsigned int count;
		bool bold;

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
