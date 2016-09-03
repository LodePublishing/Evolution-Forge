#ifndef __PLAYERG_H
#define __PLAYERG_H

#include "statistics.h"
#include "timer.h"
#include "bograph.h"
#include "force.h"
#include "info.h"
#include "bodiagram.h"
#include "bowindow.h"
#include  <list>
#include "order.h"
#include "UI_Object.h"

enum ePlayerMode
{
	HIDE_MODE,
	BASIC_PLAYER_MODE,
	ADVANCED_PLAYER_MODE,
	EXPERT_PLAYER_MODE,
	EXPERT_COMPUTER_MODE,
	GOSU_PLAYER_MODE,
	GOSU_COMPUTER_MODE,
	TRANSCENDEND_COMPUTER_MODE,
	
	MAX_MODES
};
	
    // hide: 0
    // basic mode [PLAYER]: 1     // 1 - 2 player trying to reach the goal
    // advanced mode [PLAYER]: 2  // 1 - 2 player trying to reach the goal
    // expert mode [PLAYER]: 3    // 2 - 4 player, 1-2 'human', 1-3 computers trying to stop him
    // expert mode [COMPUTER]: 4
    // gosu mode [PLAYER]: 5      // freeplay for 1-2 human vs 1-2 computers
    // gosu mode [COMPUTER]: 6
    // transcendend mode [COMPUTER]: 7 // freeplay for 1-2 computers vs 1-2 computers


class Player : public UI_Object
{
	public:
		Player(UI_Object* parent, ANARACE** anarace, ePlayerMode mode, const int playerNumber);
		~Player();
		
		void update();

		void resetData();
		
		void setMode(ePlayerMode mode); 

		void draw(DC* dc);
		void process();

		void changeAccepted();
		bool getChangedFlag();

		bool isOptimizing();
		void setOptimizing(bool opt=true);

		void CheckOrders();

		void updateRectangles();
		
	private:
		void setWindowTitles(ePlayerMode mode);
		
		void drawGeneString(DC* dc, Rect position);

		UI_Window* window[MAX_WINDOWS]; // TODO 
		
		ANARACE** anarace; //pointer auf pointer, weil sich der pointer ja veraendert!
		int geneAnimation;
		map <long, Order> orderList; // nach markern sortiert
		ePlayerMode mode;
		void MoveOrders();
};

#endif

