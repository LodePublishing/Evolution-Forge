#ifndef _GUI_PLAYER_HPP
#define _GUI_PLAYER_HPP

#include "bgwindow.hpp"
#include "force.hpp"
#include "bodiagram.hpp"
#include "bowindow.hpp"
#include "score.hpp"

class Player : public UI_Object
{
	public:
		Player(UI_Object* player_parent, const unsigned int game_number, const unsigned int game_max, const unsigned int player_number, const unsigned int player_max);
		~Player();

//		void refreshPlayerNumbers(const unsigned int playerNumber, const unsigned int totalPlayerNumber);
		
		void update();
		void resetData();
		
		void assignAnarace(ANABUILDORDER* anarace);

		void draw() const;
		void process();

		void CheckOrders();
		void setMode(const unsigned int game_number, const unsigned int game_max, const unsigned int player_number, const unsigned int player_max);
		void reloadStrings();

		void reloadOriginalSize();
		const bool wasResetted() const;

		void recheckSomeDataAfterChange();

		void compactDisplayModeHasChanged();

		const bool openMenu(const ePlayerOrder order);

		const signed int getLoadedBuildOrder() const;
		const signed int getAssignedGoal() const;
	private:
		
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

inline const signed int Player::getLoadedBuildOrder() const {
	return(boWindow->getLoadedBuildOrder());
}

inline const signed int Player::getAssignedGoal() const {
	return(forceWindow->getAssignedGoal());
}

#endif

