#ifndef _GUI_BOWINDOW_HPP
#define _GUI_BOWINDOW_HPP

#include "../ui/window.hpp"
#include "numberfield.hpp"
#include "message.hpp"

#include "info.hpp"
#include <map>

class BoEntry : public UI_Button
{
    public:
		BoEntry(const BoEntry& object);
		BoEntry& operator=(const BoEntry& object);
        BoEntry(UI_Object* bo_parent, Rect bo_rect, Rect bo_max_rect, const string& bo_unit);
        ~BoEntry();
        const unsigned int changed();
        void updateText(const string& utext);
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
		BoWindow(const BoWindow& object);
		BoWindow& operator=(const BoWindow& object);
		BoWindow(UI_Object* bo_parent, ANARACE* bo_anarace, InfoWindow* bo_info_window, MessageWindow* message_window, std::map <long, Order>* bo_order_list, const unsigned int bo_window_number);
		~BoWindow();
		void resetData();
		void draw(DC* dc) const;
		void process();
		
		void setMarkedIP(const unsigned int IP);
		const unsigned int getMarkedIP() const;
		
		void setMarkedUnit(const unsigned int unit);
		const unsigned int getMarkedUnit() const;
	private:
		void drawSelectionStuff(DC* dc) const;
        BoEntry* boEntry[MAX_LENGTH];
	
//		void resetButtons();
		unsigned int markedUnit;
		unsigned int ownMarkedUnit;
		unsigned int markedIP;
		unsigned int ownMarkedIP;
		InfoWindow* infoWindow;
		ANARACE* anarace;
		std::map <long, Order>* orderList;
		unsigned int optimizeMode;
		signed int boInsertPoint, boEndPoint;
//		int makeSpaceButton;
//		int unitButton[2*MAX_LENGTH];
//		int optButton[2*MAX_LENGTH];
//		int orderButton[2*MAX_LENGTH];
		unsigned int boGoalListOpened;
		unsigned int lastBogoal;

		UI_Button* resetButton;
		UI_Button* saveBuildOrderButton;

		MessageWindow* msgWindow;

//		NumberField* speed;
};

#endif // _GUI_BOWINDOW_HPP

