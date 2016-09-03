#ifndef _GUI_BOWINDOW_HPP
#define _GUI_BOWINDOW_HPP

#include "../ui/window.hpp"
#include "../core/anabuildorder.hpp"
#include <list>

#include "boentry.hpp"
#include "../ui/scrollbar.hpp"

class BoWindow : public UI_Window
{
	public:
		BoWindow(const BoWindow& object);
		BoWindow& operator=(const BoWindow& object);
		BoWindow(UI_Object* bo_parent, /* bool* fixed,*/ const unsigned int bo_window_number);
		~BoWindow();
		void resetData();
		void draw(DC* dc) const;
		void reloadStrings();
		void processList();
		void process();
		
		void assignAnarace(ANABUILDORDER* bo_anarace);
	private:
		void drawSelectionStuff(DC* dc) const;
		void checkForInfoWindow();
//		void resetButtons();
		ANABUILDORDER* anarace;
		std::list<BoEntry*> boList;
		unsigned int optimizeMode;
//		int new_one;
//		int same;
//		int moved;
//		int add_end;
//		int deleted;
//		int size1;
//		int size2;
		signed int boInsertPoint, boEndPoint;
//		int makeSpaceButton;
//		int unitButton[2*MAX_LENGTH];
//		int optButton[2*MAX_LENGTH];
//		int orderButton[2*MAX_LENGTH];
		unsigned int boGoalListOpened;
		unsigned int lastBogoal;

		UI_Button* saveBuildOrderButton;
		UI_Button* loadBuildOrderButton;

		bool* fixed;
		UI_Scrollbar* scrollBar;
};

#endif // _GUI_BOWINDOW_HPP

