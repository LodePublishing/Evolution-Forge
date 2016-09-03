#ifndef _GUI_PLAYER_HPP
#define _GUI_PLAYER_HPP

//#include "statistics.hpp"
#include "score.hpp"
#include "bgwindow.hpp"
#include "force.hpp"
#include "bodiagram.hpp"
#include "bowindow.hpp"


class Player : public UI_Object
{
	public:
		Player(UI_Object* player_parent, const unsigned int game_number, const unsigned int game_max, const unsigned int player_number, const unsigned int player_max);
		~Player();

//		void refreshPlayerNumbers(const unsigned int playerNumber, const unsigned int totalPlayerNumber);
		
		void update();
		void resetData();
		
		void assignAnarace(ANABUILDORDER* anarace);

		void draw(DC* dc) const;
		void process();

		void CheckOrders();
		void setMode(const unsigned int game_number, const unsigned int game_max, const unsigned int player_number, const unsigned int player_max);
		void reloadStrings();

		void reloadOriginalSize();
		const bool wasResetted() const;

		void recheckSomeDataAfterChange();
	private:
		float geneAnimation;
		void drawGeneString(DC* dc) const;
		void drawGene(DC* dc, unsigned int k, const Point* points, const Point position, Pen& bla1, Pen& bla2) const;
		
		ANABUILDORDER* anarace; //pointer auf pointer, weil sich der pointer ja veraendert!
//		unsigned int mode;

		ForceWindow* forceWindow;
		BoWindow* boWindow;
		BoGraphWindow* boGraphWindow;
		BoDiagramWindow* boDiagramWindow;

		unsigned int gameNumber;
		unsigned int gameMax;
		unsigned int playerNumber;
		unsigned int playerMax;
};

#endif

