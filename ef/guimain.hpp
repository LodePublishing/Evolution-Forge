#ifndef _GUI_MAIN_HPP
#define _GUI_MAIN_HPP

const float GUI_VERSION=0.51;

#include "game.hpp"
#include "message.hpp"
#include "info.hpp"
#include "tutorial.hpp"
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
		void loadGoals();
		
		
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
	
	private:
//		SDL_Surface* cursor_save;
//		SDL_Surface* cursor;
		void initializeGame();
		UI_Window* mainWindow;
		TutorialWindow* tutorialWindow;
		SettingsWindow* settingsWindow;
		Point maus;
//		static ProgressBar* bar;

		bool endrun;

		unsigned int gameCount; 
		Game* game[MAX_GAME];
		
		static InfoWindow* infoWindow;
		static MessageWindow* msgWindow;
};
																				
#endif

