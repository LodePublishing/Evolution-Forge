#ifndef _GUI_BOWINDOW_HPP
#define _GUI_BOWINDOW_HPP

#include "../ui/window.hpp"
#include "order.hpp"
#include "message.hpp"

#include "info.hpp"
#include <list>

#include "boentry.hpp"


class BoWindow : public UI_Window
{
	public:
		BoWindow(const BoWindow& object);
		BoWindow& operator=(const BoWindow& object);
		BoWindow(UI_Object* bo_parent, ANARACE* bo_anarace, InfoWindow* bo_info_window, MessageWindow* message_window, std::list<Order*>* bo_order_list,/* bool* fixed,*/ const unsigned int bo_window_number);
		~BoWindow();
		void resetData();
		void draw(DC* dc) const;
		void reloadStrings();
		void processList();
		void process();
		
		void setMarkedIP(const unsigned int marked_ip);
		const unsigned int getMarkedIP() const;
		
		void setMarkedUnit(const unsigned int marked_unit);
		const unsigned int getMarkedUnit() const;
		void assignAnarace(ANARACE* bo_anarace);
	private:
		void drawSelectionStuff(DC* dc) const;
//		void resetButtons();
		unsigned int markedUnit;
		unsigned int ownMarkedUnit;
		unsigned int markedIP;
		unsigned int ownMarkedIP;
		InfoWindow* infoWindow;
		ANARACE* anarace;
		std::list<Order*>* orderList;
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

		UI_Button* resetButton;
		UI_Button* saveBuildOrderButton;

		MessageWindow* msgWindow;
		bool* fixed;

};

#endif // _GUI_BOWINDOW_HPP

