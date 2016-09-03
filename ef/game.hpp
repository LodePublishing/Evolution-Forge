#ifndef _GUI_GAME_HPP
#define _GUI_GAME_HPP

#include "statistics.hpp"
#include "timer.hpp"
#include "bgwindow.hpp"
#include "force.hpp"
#include "bodiagram.hpp"
#include "bowindow.hpp"
#include "message.hpp"

#include <math.h>
#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif
// Windows kotz

enum eGameMode
{
	HIDE_MODE,
	BASIC_GAME_MODE,
	ADVANCED_GAME_MODE,
	EXPERT_GAME_MODE,
	EXPERT_COMPUTER_MODE,
	GOSU_GAME_MODE,
	GOSU_COMPUTER_MODE,
	TRANSCENDEND_COMPUTER_MODE,
	
	MAX_MODES
};
	
    // hide: 0
    // basic mode [GAME]: 1     // 1 - 2 player trying to reach the goal
    // advanced mode [GAME]: 2  // 1 - 2 player trying to reach the goal
    // expert mode [GAME]: 3    // 2 - 4 player, 1-2 'human', 1-3 computers trying to stop him
    // expert mode [COMPUTER]: 4
    // gosu mode [GAME]: 5      // freeplay for 1-2 human vs 1-2 computers
    // gosu mode [COMPUTER]: 6
    // transcendend mode [COMPUTER]: 7 // freeplay for 1-2 computers vs 1-2 computers


class Game : public UI_Object
{
	public:
		Game(UI_Object* parent, ANARACE** anarace, MessageWindow* msgWindow, const unsigned int gameNumber);
		~Game();
		
		void update();

		void resetData();
		void restartAnarace();
		
		void setMode(const eTab tab, const unsigned int gameNum); 

		void draw(DC* dc) const;
		void process();

		const bool isOptimizing() const;
		void setOptimizing(bool opt=true);

		void CheckOrders();

		void updateRectangles(const unsigned int maxGame);
		UI_Window* window[MAX_WINDOWS]; 
		
		float geneAnimation;
		void assignAnarace(ANARACE** anarace);
	private:
		void drawGeneString(DC* dc, const Rect position) const;
		void drawGene(DC* dc, int k, const Point* points, const Point position, Pen& bla1, Pen& bla2) const;
		
		ANARACE** anarace; //pointer auf pointer, weil sich der pointer ja veraendert!
//		list<Order*> orderList; // sorted by IP
		unsigned int mode;
//		void MoveOrders();
//		bool fixed[MAX_LENGTH];
};

#endif

