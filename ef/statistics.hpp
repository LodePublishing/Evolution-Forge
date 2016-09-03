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
		
		int ani;
		
		UI_Button* statEntry[9];
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
		unsigned int fps[200];
		unsigned int oldForceCounter[20];
		signed int oldForce[20];
		unsigned int oldGasCounter[20];
		signed int oldGas[20];
		unsigned int oldMineralsCounter[20];
		signed int oldMinerals[20];
		unsigned int oldBoLengthCounter[20];
		signed int oldBoLength[20];
		unsigned int maxpFitness;
		unsigned int maxsFitness;
		unsigned int maxtFitness;
		unsigned int mintFitness;
		unsigned int maxaFitness;
		unsigned int maxvFitness;
		unsigned int maxForce;
		unsigned int maxHarvestedRessources;
		unsigned int maxfps;
		unsigned int average[100];
		unsigned int averagecounter;
};
#endif

