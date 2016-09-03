#ifndef _GUI_PLAYERG_HPP
#define _GUI_PLAYERG_HPP

#include "statistics.hpp"
#include "timer.hpp"
#include "bograph.hpp"
#include "force.hpp"
#include "bodiagram.hpp"
#include "bowindow.hpp"

#include "math.h"
#include  <list>

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
		Player(UI_Object* parent, ANARACE** anarace, const int playerNumber);
		~Player();
		
		void update();

		void resetData();
		
		void setMode(const eTab tab, const int playerNum); 

		void draw(DC* dc) const;
		void process();

		void changeAccepted();
		const bool getChangedFlag() const;
		void checkForChange();

		const bool isOptimizing() const;
		void setOptimizing(bool opt=true);

		void CheckOrders();

		void updateRectangles(const int maxPlayer);
		
	private:
		void drawGeneString(DC* dc, const Rect position) const;
		
		UI_Window* window[MAX_WINDOWS]; 
		ANARACE** anarace; //pointer auf pointer, weil sich der pointer ja veraendert!
		int geneAnimation;
		map <long, Order> orderList; // nach markern sortiert
		int mode;
		void MoveOrders();
};

#endif

