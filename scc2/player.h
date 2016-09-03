#ifndef __PLAYERG_H
#define __PLAYERG_H

#include "statistics.h"
#include "timer.h"
#include "bograph.h"
#include "force.h"
#include "info.h"
#include "bodiagram.h"
#include "bowindow.h"
#include "list.h"

class Player
{
        public:
                Player(ANARACE** anarace, int mode);
                ~Player();
                void update();
                StatisticsWindow* statisticsWindow;
                BoWindow* boWindow;
                TimerWindow* timerWindow;
                BoDiagramWindow* boDiagramWindow;
                BoGraphWindow* boGraphWindow;
                ForceWindow* forceWindow;
                InfoWindow* infoWindow;
                void resetData();
		void setTitles(int mode);
		void assignAnarace(ANARACE** anarace);
                void DrawMe(wxDC* dc);
                void drawGeneString(wxDC* dc, wxRect position);
		void Show(int type); 
		void processButtons();
		int isShown();
		void changeAccepted();
		int hasChanged();
		int isOptimizing();
		void setOptimizing(int opt);
		static void InitPositions(GraphixScrollWindow* mainWindow);
		static wxRect statisticsWindowRect[25];
		static wxRect statisticsWindowMax[25];
		static wxRect boWindowRect[25];
		static wxRect boWindowMax[25];
		static wxRect timerWindowRect[25];
		static wxRect timerWindowMax[25];
		static wxRect boDiagramWindowRect[25];
		static wxRect boDiagramWindowMax[25];
		static wxRect boGraphWindowRect[25];
		static wxRect boGraphWindowMax[25];
		static wxRect forceWindowRect[25];
		static wxRect forceWindowMax[25];
		static wxRect infoWindowRect[25];
		static wxRect infoWindowMax[25];
		void CheckForInfoWindow();
		void CheckOrders();
		void MoveOrders();
        private:
                ANARACE** anarace; //pointer auf pointer, weil sich der pointer ja veraendert!
		int shown;
		int geneAnimation;
		OrderList orderList;		
};

#endif

