#ifndef _GUI_PLAYER_HPP
#define _GUI_PLAYER_HPP

#include "statistics.hpp"
#include "score.hpp"
#include "bgwindow.hpp"
#include "force.hpp"
#include "bodiagram.hpp"
#include "bowindow.hpp"


enum ePlayerMode
{
	HIDE_PLAYER_MODE,
	BASIC_GAME_PLAYER_MODE,
	ADVANCED_GAME_PLAYER_MODE,
	EXPERT_GAME_PLAYER_MODE,
	EXPERT_COMPUTER_PLAYER_MODE,
	GOSU_GAME_PLAYER_MODE,
	GOSU_COMPUTER_PLAYER_MODE,
	TRANSCENDEND_COMPUTER_PLAYER_MODE,
	
	MAX_PLAYER_MODES
};
	
class Player : public UI_Object
{
	public:
		Player(UI_Object* parent, const unsigned int playerNumber);
		~Player();
		
		void update();

		void resetData();
		
		void setMode(const eTab tab, const unsigned int gameNum); 
		void assignAnarace(ANABUILDORDER* anarace);

		void draw(DC* dc) const;
		void process();

		void CheckOrders();
		void updateRectangles(const unsigned int maxPlayer);
		void reloadStrings();
	
	private:
		float geneAnimation;
		void drawGeneString(DC* dc) const;
		void drawGene(DC* dc, int k, const Point* points, const Point position, Pen& bla1, Pen& bla2) const;
		
		ANABUILDORDER* anarace; //pointer auf pointer, weil sich der pointer ja veraendert!
		unsigned int mode;

		ForceWindow* forceWindow;
//		StatisticsWindow* statisticsWindow;
		BoWindow* boWindow;
		BoGraphWindow* boGraphWindow;
		BoDiagramWindow* boDiagramWindow;

};

#endif

