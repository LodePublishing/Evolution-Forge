#ifndef _GUI_MAIN_HPP
#define _GUI_MAIN_HPP

const float GUI_VERSION=0.51;

#include "player.hpp"
#include "message.hpp"
#include "tutorial.hpp"
#include "mainwindow.hpp"
#include "setwindow.hpp"

#include "progressbar.hpp"

#include <math.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

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
		
		UI_Window* mainWindow;
		MessageWindow* msgWindow;
		UI_Window* infoWindow;
		TutorialWindow* tutorialWindow;
		Player* player[MAX_PLAYER];
		SettingsWindow* settingsWindow;
		static ProgressBar* bar;
		
		const bool isOptimizing() const;
		void stopOptimizing();
		void startOptimizing();

		void noticeFullscreen();

		void leftDown();
		void leftUp(const Point p);
		void rightDown();
		void rightUp(const Point p);
	
		void setMouse(const Point p);
	
		Point maus;
		bool drawing;
	private:
//		SDL_Surface* cursor_save;
//		SDL_Surface* cursor;

		

		unsigned int oldrun;
		bool endrun;
		bool gizmo;
		void drawGizmo(DC* dc) const;
		const Size helper(DC* dc, const unsigned int dx, const int i, const string& str) const;
		ANARACE* anarace[MAX_PLAYER];
		unsigned int update; // TODO anderer Name
		unsigned int refresh; // "" 
		unsigned int ani, ani2;

		bool boHasChanged;	
};
																				
#endif

