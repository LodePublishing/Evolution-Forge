#ifndef _GUI_BOWINDOW_HPP
#define _GUI_BOWINDOW_HPP

#include "../ui/window.hpp"

#include "info.hpp"
#include <map>

class BoEntry : public UI_Button
{
    public:
        BoEntry(UI_Object* parent, Rect rect, Rect maxRect, string unit);
        ~BoEntry();
        int changed();
        void updateText(string utext);
		void process();
		void draw(DC* dc) const;
    private:
//        UI_Button* addUnit;
//      UI_Button* subUnit;
//    UI_Button* cancelUnit; TODO
};


class BoWindow:public UI_Window
{
	public:
		BoWindow(UI_Object* parent, ANARACE* anarace, InfoWindow* infoWindow, map <long, Order>* orderList, const int windowNumber);
		~BoWindow();
		void resetData();
		void draw(DC* dc) const;
		void process();
		void setMarkedUnit(int unit);
	private:
		void drawSelectionStuff(DC* dc) const;
        BoEntry* boEntry[MAX_LENGTH];
	
//		void resetButtons();
		int markedUnit;
		InfoWindow* infoWindow;
		ANARACE* anarace;
		map <long, Order>* orderList;
		int optimizeMode;
		int boInsertPoint,boEndPoint;
//		int makeSpaceButton;
//		int unitButton[2*MAX_LENGTH];
//		int optButton[2*MAX_LENGTH];
//		int orderButton[2*MAX_LENGTH];
		int boGoalListOpened;
		int lastBogoal;

		UI_Button* resetButton;

		UI_StaticText* speedText;
		UI_Button* addSpeed;
		UI_Button* subSpeed;
};

#endif // _GUI_BOWINDOW_HPP

