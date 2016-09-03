#ifndef _GUI_MAIN_HPP
#define _GUI_MAIN_HPP

const float GUI_VERSION=0.50;

#include "player.hpp"
#include "message.hpp"
#include "tutorial.hpp"
#include "corewindow.hpp"
#include "mainwindow.hpp"

#include <math.h>
#include <iostream>

#include "SDL/SDL_framerate.h"

class Main
{
	public:
		Main() {gizmo=true;};
		~Main() {};
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
		const int isOptimizing();
		void stopOptimizing();
		void startOptimizing();
		void Init(DC* dc);
	private:
		int oldrun,endrun;
		bool gizmo;
		const GA* ga;
		void drawGizmo(DC* dc) const;
		void helper(DC* dc, int i, int &dx, int &dy, const string& str) const;
		ANARACE* anarace[MAX_PLAYER];
		int update;
		int refresh;
		int ani;
};
																				
#endif

