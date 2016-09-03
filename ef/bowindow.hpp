#ifndef _GUI_BOWINDOW_HPP
#define _GUI_BOWINDOW_HPP

#include "../ui/window.hpp"
#include "../ui/group.hpp"

#include "boentry.hpp"
#include "bomenu.hpp"
#include "unitmenu.hpp"

class BoWindow : public UI_Window
{
	public:
		BoWindow(const BoWindow& object);
		BoWindow& operator=(const BoWindow& object);
		BoWindow(UI_Object* bo_parent, const unsigned int game_number, const unsigned int game_max, const unsigned int player_number, const unsigned int player_max);
		~BoWindow();
		void resetData();
		void draw(DC* dc) const;
		void reloadStrings();
		void processList();
		void process();

		void assignAnarace(ANABUILDORDER* bo_anarace);
		void setMode(const unsigned int game_number, const unsigned int game_max, const unsigned int player_number, const unsigned int player_max);
		void reloadOriginalSize();

		const bool wasResetted() const;

		const std::list<unsigned int>& getSelectedItems() const;
		void setSelected(const std::list<unsigned int>& selected);

		void recheckSomeDataAfterChange();
		
		void needSaveBox();
		void saveBoxIsCanceled();
		void saveBoxIsDone(std::string input_string);

		const bool saveBuildOrder();
		const bool openBoMenu();
		void mouseHasLeft();
		const signed int getLoadedBuildOrder() const;
	private:
		float geneAnimation;
		void drawGeneString(DC* dc) const;
		void drawGene(DC* dc, unsigned int k, const Point* points, const Point position, Pen& bla1, Pen& bla2) const;

		void closeMenus();
		void drawSelectionStuff(DC* dc) const;
		void checkForInfoWindow();
//		void resetButtons();
		ANABUILDORDER* anarace;
		std::list<BoEntry*> boList;
//		Rect moveTarget;
		unsigned int optimizeMode;
		signed int addUnit;
//		bool wasMovedByMouse;
//		signed int boInsertPoint, boEndPoint;
//		int makeSpaceButton;
//		int unitButton[2*MAX_LENGTH];
//		int optButton[2*MAX_LENGTH];
//		int orderButton[2*MAX_LENGTH];
		unsigned int boGoalListOpened;
		unsigned int lastBogoal;
		unsigned int startLine;
		std::list<unsigned int> selectedItems;


//		UI_StaticText* timeSlot[60];

		UI_Group* menuGroup;
		UI_Button* restartBuildOrderButton;
		UI_Button* saveBuildOrderButton;
		UI_Button* loadBuildOrderButton;
		
		UI_ScrollBar* scrollBar;
//		UI_Button* alwaysBuildWorker;
//		UI_Button* onlySwapOrders;
		
//		UI_Button* unitMenuButton;
//		UnitMenu* unitMenu;

		BoMenu* boMenu;
	
		unsigned int gameNumber;
		unsigned int gameMax;
		unsigned int playerNumber;
		unsigned int playerMax;

		bool saveBox;

		bool boMenuOpenedExternally;

		signed int loadedBuildOrder;
};

inline const signed int BoWindow::getLoadedBuildOrder() const {
	return(loadedBuildOrder);
}

inline const std::list<unsigned int>& BoWindow::getSelectedItems() const {
	return(selectedItems);
}


#endif // _GUI_BOWINDOW_HPP

