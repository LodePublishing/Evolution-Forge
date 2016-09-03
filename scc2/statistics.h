#ifndef __STATISTICS_H
#define __STATISTICS_H

#include "graphics.h"
#include "../scc2dll/anarace.h"

class StatisticsWindow:public GraphixScrollWindow
{
        public:
                StatisticsWindow(wxRect rahmen, wxRect maxSize);
                ~StatisticsWindow();
		void setAnarace(ANARACE* anarace);
		void drawStatistics(wxDC* dc);
		void ProgressGraph();
                void resetData();
	        int maxForce; //all units
		int currentForce;
        private:
		ANARACE* anarace;
        	void showGraph(wxDC* dc, int* data,int max,wxColour col);
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
		int average[100];int averagecounter;
};
#endif

