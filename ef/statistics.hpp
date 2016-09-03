#ifndef _GUI_STATISTICS_HPP
#define _GUI_STATISTICS_HPP

#include "../ui/window.hpp"
#include "../core/anarace.hpp"

#include "math.h"

class StatisticsWindow:public UI_Window
{
	public:
		StatisticsWindow(UI_Object* parent, const ANARACE* anarace, const int windowNumber);
		~StatisticsWindow();
		
		void process();
		void draw(DC* dc) const;
		
		void resetData();
		
	private:
		UI_StaticText* mspf;
		UI_StaticText* fps;
		UI_StaticText* bps;
		
		UI_Button* statEntry[8];
		long int start_time;
		const ANARACE* anarace;
		void showGraph(DC* dc, const int* data, const int min, const int max, const Color col) const;
		int harvestedMinerals[200];
		int harvestedGas[200];
		int pFitness[200];
		int tFitness[200];
		int aFitness[200];
		int vFitness[200];
		int alength[200];
		int time[200];
		int force[200];
		int generation[200];
		int oldForceCounter[20],oldForce[20];
		int oldGasCounter[20],oldGas[20];
		int oldMineralsCounter[20],oldMinerals[20];
		int maxsFitness;
		int maxpFitness;
		int maxtFitness;
		int mintFitness;
		int maxaFitness;
		int maxvFitness;
		int maxForce;
		int maxHarvestedRessources;
		int average[100];
		int averagecounter;
};
#endif

