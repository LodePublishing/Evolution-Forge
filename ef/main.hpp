#ifndef _GUI_MAIN_HPP
#define _GUI_MAIN_HPP

const float GUI_VERSION=0.50;

#include "player.hpp"
#include "message.hpp"
#include "tutorial.hpp"
#include "corewindow.hpp"
#include "mainwindow.hpp"
#include "setwindow.hpp"

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
		UI_Window* mainWindow;
		MessageWindow* msgWindow;
		UI_Window* infoWindow;
		TutorialWindow* tutorialWindow;
		Player* player[MAX_PLAYER];
		CoreWindow* theCore;
		SettingsWindow* settingsWindow;
		const bool isOptimizing() const;
		void stopOptimizing();
		void startOptimizing();

		void leftDown();
		void leftUp(const Point p);
		void rightDown();
		void rightUp(const Point p);
	
		void setMouse(const Point p);
	
		Point maus;
//	private:
		SDL_Surface* cursor_save;
		SDL_Surface* cursor;

		
		bool buttonPressed;
		bool hasAlreadyLeft;
		UI_Button* button;

		unsigned int oldrun;
		bool endrun;
		bool gizmo;
		const GA* ga;
		void drawGizmo(DC* dc) const;
		const Size helper(DC* dc, const unsigned int dx, const int i, const string& str) const;
		ANARACE* anarace[MAX_PLAYER];
		unsigned int update; // TODO anderer Name
		unsigned int refresh; // "" 
		unsigned int ani, ani2;
};
																				
#endif

