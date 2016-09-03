#ifndef __STATISTICS_H
#define __STATISTICS_H

#include "UI_Window.h"
#include "../scc2dll/anarace.h"

class StatisticsWindow:public UI_Window
{
	public:
		StatisticsWindow(UI_Object* parent, wxRect rahmen, wxRect maxSize, ANARACE* anarace);
		~StatisticsWindow();
		
		void process();
		void draw(wxDC* dc);
		
		void resetData();
		
	private:
		ANARACE* anarace;
		void showGraph(wxDC* dc, int* data, int min, int max, wxColour col);
		int harvestedMins[200];
		int harvestedGas[200];
		int pFitness[200];
		int tFitness[200];
		int aFitness[200];
		int vFitness[200];
		int length[200];
		int time[200];
		int force[200];
		int oldForceCounter[20],oldForce[20];
		int oldGasCounter[20],oldGas[20];
		int oldMinsCounter[20],oldMins[20];
		int maxsFitness;
		int maxpFitness;
		int maxtFitness;
		int mintFitness;
		int maxaFitness;
		int maxvFitness;
		int maxForce;
		int maxHarvestedRessources;
		int average[100];int averagecounter;
//		wxDateTime dt1;
//		wxDateTime dt2; taken out because DLL file was not compiled with wxDateTime
};
#endif

