#ifndef _GUI_MAIN_HPP
#define _GUI_MAIN_HPP

const float GUI_VERSION=0.51;

#include "game.hpp"
#include "msgwindow.hpp"
//#include "info.hpp"
#include "helpwindow.hpp"
#include "mapwindow.hpp"
#include "datawindow.hpp"
#include "mainwindow.hpp"
#include "setwindow.hpp"

#include "progressbar.hpp"

#include <math.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

#include "endrundialog.hpp"
#include "intro.hpp"

class Main
{
	public:
		Main();
		~Main();

		const bool initGUI(DC* dc);
		const bool initCore();
		
		
		void draw(DC* dc) const;
		
		void process();
		void resetData();
		
		void newGeneration();
		
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
		const bool hasThemeChanged() const;

		void resetDataChange();

		void reloadOriginalSize();
		void reloadStrings();

		void needRedraw();
	
		void initializeGame(const unsigned int game_number);
	private:
		MainWindow* mainWindow;
		IntroWindow* introWindow;
		HelpWindow* helpWindow;
		SettingsWindow* settingsWindow;
		DataBaseWindow* dataBaseWindow;
		MapWindow* mapWindow;
		Point maus;
		unsigned int gameCount; 
		Game* game[MAX_GAME];
//		static InfoWindow* infoWindow;
		static MessageWindow* msgWindow;
//		StatisticsWindow* statisticsWindow;
		const bool loadGoals();
		const bool loadBuildOrders();
		const bool loadStartConditions();
		const bool loadHarvestData();
		const bool loadMaps();

		signed int tabToGameList[MAX_TABS];
		signed int tabToSplitGameList[MAX_TABS];

		signed int currentTab;
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

inline const bool Main::hasThemeChanged() const {
	return(settingsWindow->hasThemeChanged());
}

																		
#endif

