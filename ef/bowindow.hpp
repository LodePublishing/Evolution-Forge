#ifndef _GUI_BOWINDOW_HPP
#define _GUI_BOWINDOW_HPP

#include "unitmenu.hpp"
#include "../ui/window.hpp"
#include "../core/anabuildorder.hpp"
#include <list>

#include "boentry.hpp"
#include "../ui/scrollbar.hpp"
#include "../ui/checkbutton.hpp"

#include "savebox.hpp"
#include "bomenu.hpp"

class BoWindow : public UI_Window
{
	public:
		BoWindow(const BoWindow& object);
		BoWindow& operator=(const BoWindow& object);
		BoWindow(UI_Object* bo_parent, const unsigned int game_number, const unsigned int max_games, const unsigned int player_number, const unsigned int max_players);
		~BoWindow();
		void resetData();
		void draw(DC* dc) const;
		void reloadStrings();
		void processList();
		void process();
		void wave(SDL_snd& sound);
		void assignAnarace(ANABUILDORDER* bo_anarace);
		void reloadOriginalSize();

		const bool wasResetted() const;

		const signed int getSelectedItem() const;
		void setSelected(const unsigned int selected);

		void recheckSomeDataAfterChange();
		
	private:
		void closeMenus();
		void drawSelectionStuff(DC* dc) const;
		void checkForInfoWindow();
//		void resetButtons();
		ANABUILDORDER* anarace;
		std::list<BoEntry*> boList;
		Rect moveTarget;
		unsigned int optimizeMode;
		signed int addUnit;
		bool wasMovedByMouse;
		signed int boInsertPoint, boEndPoint;
//		int makeSpaceButton;
//		int unitButton[2*MAX_LENGTH];
//		int optButton[2*MAX_LENGTH];
//		int orderButton[2*MAX_LENGTH];
		unsigned int boGoalListOpened;
		unsigned int lastBogoal;
		unsigned int startLine;
		signed int selectedItem;

		bool newItem;

		UI_StaticText* timeSlot[60];

//		UI_Radio* menuRadio;
		UI_Button* restartBuildOrderButton;
		UI_Button* saveBuildOrderButton;
		UI_Button* loadBuildOrderButton;
		

		bool* fixed;
		UI_ScrollBar* scrollBar;
		UI_CheckButton* alwaysBuildWorker;
		UI_CheckButton* onlySwapOrders;
		
//		UI_Button* unitMenuButton;
//		UnitMenu* unitMenu;

		BoMenu* boMenu;
		SaveBox* saveBox;
};

inline const signed int BoWindow::getSelectedItem() const {
	return(selectedItem);
}


#endif // _GUI_BOWINDOW_HPP

