#ifndef __EF2_H
#define __EF2_H

const float GUI_VERSION=0.50;

#include "UI_Object.h"
#include "UI_Window.h"
#include "UI_Theme.h"
#include "player.h"
#include "message.h"
#include "tutorial.h"
#include "corewindow.h"
#include "SDL/SDL.h"

const int GIZMO_NUMBER=13;

const char gizmo[GIZMO_NUMBER][40]=
{
	"Perfection is the key",
	"Look at la Luna",
	"It is a good day to optimize",
	"The early ling catches the worm",
	"Build orders are best served optimized",
	"Pool Pool Pool Pool Dead - A newbie",
	"Good... Bad... I'm the guy with the gum",
	"LOOOOOOOOOOOOOOOOOOOOOOOOOL",
	"ALT+F4 - Zergling has left the game :/",
	"WHY ARE YOUR ZEALOTS INVISIBLE!?",
	"!!!!! 3VS3 BGH NO NOOBS !!!!",
	"<ý¦<¬ ;) ýJ<¼ýD Ìýt<Üý >_<",
	"60 Minutes no Rush!"
};
																					
class Main
{
public:
	Main() {};
	~Main() {};
	void OnPaint(DC* dc);
	void OnIdle();
	void resetData();
//	void showToolTip();
	UI_Window* mainWindow;
	MessageWindow* msgWindow;
	UI_Window* haxor;
	UI_Window* infoWindow;
	TutorialWindow* tutorialWindow;
	Player* player[MAX_PLAYER];
	CoreWindow* theCore;
	const int isOptimizing();
	void stopOptimizing();
	void startOptimizing();
	void Init(DC* dc);
private:
	int userIsNewbie;
	
	int mouseLeft;
	int oldrun,endrun;
	const GA* ga;
	void process();
	void drawGizmo(DC* dc) const;
	void showCoreAnimation();
	ANARACE* anarace[MAX_PLAYER];
	int animationNumbers;
	int update;
	int refresh;
	int tutorialChapter;
	int ani;
	int tutorialAnimation;
};
																				
#endif

