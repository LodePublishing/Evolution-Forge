#ifndef _GUI_STATISTICS_HPP
#define _GUI_STATISTICS_HPP

#include "../ui/window.hpp"
#include "../core/anarace.hpp"

#include "math.h"

class StatisticsWindow:public UI_Window
{
	public:
		StatisticsWindow(UI_Object* stat_parent, const ANARACE* stat_anarace, const unsigned int stat_window_number);
		~StatisticsWindow();
		
		void process();
		void draw(DC* dc) const;
		
		void resetData();
		
	private:
		void showGraph(DC* dc, const unsigned int* data, const unsigned int min, const unsigned int max, const Color col) const;
		
		UI_StaticText* mspf;
		UI_StaticText* fps;
		UI_StaticText* bps;
		
		UI_Button* statEntry[8];
		long unsigned int start_time;
		const ANARACE* anarace;
		unsigned int harvestedMinerals[200];
		unsigned int harvestedGas[200];
		unsigned int pFitness[200];
		unsigned int tFitness[200];
		unsigned int aFitness[200];
		unsigned int vFitness[200];
		unsigned int alength[200];
		unsigned int time[200];
		unsigned int force[200];
		unsigned int generation[200];
		unsigned int oldForceCounter[20],oldForce[20];
		unsigned int oldGasCounter[20],oldGas[20];
		unsigned int oldMineralsCounter[20],oldMinerals[20];
		unsigned int maxpFitness;
		unsigned int maxsFitness;
		unsigned int maxtFitness;
		unsigned int mintFitness;
		unsigned int maxaFitness;
		unsigned int maxvFitness;
		unsigned int maxForce;
		unsigned int maxHarvestedRessources;
		unsigned int average[100];
		unsigned int averagecounter;
};
#endif

