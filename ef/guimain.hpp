#ifndef _GUI_MAIN_HPP
#define _GUI_MAIN_HPP

const float GUI_VERSION=0.51;

#include "game.hpp"
#include "msgwindow.hpp"
#include "info.hpp"
#include "helpwindow.hpp"
#include "mapwindow.hpp"
#include "mainwindow.hpp"
#include "setwindow.hpp"

#include "progressbar.hpp"

#include <math.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

#include "endrundialog.hpp"

class Main
{
	public:
		Main(DC* dc);
		~Main();
		void draw(DC* dc) const;
		void process();
		void OnIdle();
		void resetData();
		
		
		const bool isAnyOptimizing() const;
		void stopAllOptimizing();
		void startAllOptimizing();

		void noticeFullscreen();

		void leftDown();
		void leftUp(const Point p);
		void rightDown();
		void rightUp(const Point p);

		void wheelUp();
		void wheelDown();
	
		void setMouse(const Point p);

		const bool newRun();
		const bool hasBitDepthChanged() const;
		const bool hasResolutionChanged() const;
		const bool hasFullScreenChanged() const;

		void reloadOriginalSize();
		void reloadStrings();
	
	private:
		void initializeGame(const unsigned int game_number);
		MainWindow* mainWindow;
		HelpWindow* helpWindow;
		SettingsWindow* settingsWindow;
		MapWindow* mapWindow;
		Point maus;
		unsigned int gameCount; 
		Game* game[MAX_GAME];
		static InfoWindow* infoWindow;
		static MessageWindow* msgWindow;
//		StatisticsWindow* statisticsWindow;
		void loadGoals();
		void loadStartConditions();
		void loadHarvestData();
		void loadMaps();

		signed int tabToGameList[MAX_TABS];
		signed int tabToSplitGameList[MAX_TABS];

		unsigned int currentTab;
};

inline const bool Main::hasBitDepthChanged() const {
	return(settingsWindow->hasBitDepthChanged());
}

inline const bool Main::hasResolutionChanged() const {
	return(settingsWindow->hasResolutionChanged());
}

inline const bool Main::hasFullScreenChanged() const {
	return(settingsWindow->hasFullScreenChanged());
}
																			
#endif

