#ifndef _GUI_BOGRAPH_HPP
#define _GUI_BOGRAPH_HPP

#include "../ui/window.hpp"
#include "info.hpp"

#include <map>

/*struct BoGraphItem
{
	Rect rect;
	Rect startRect;
	Rect targetRect;
	int IP;
	int unit;
	
}*/

class BoGraphWindow:public UI_Window
{
	public:
		BoGraphWindow(UI_Object* parent, ANARACE* anarace, InfoWindow* infoWindow, map <long, Order>* orderList, const int windowNumber);
		~BoGraphWindow();
		
		void resetData();
		void process();
		void draw(DC* dc) const;
		void setMarkedUnit(int unit);
	private:
		struct BOGRAPH
		{
			int type;
			int position;
			int height;
			int lines;
			Rect edge;
		} bograph[20];
		int lastbographY;

		void checkForInfoWindow();
		int markedUnit;
		int markAni;
		// TODO orderList bei jedem Schritt kopieren oder so... :/
		// WEIL: die orderlist veraendert wird von CheckOrders von player und somit die DrawFunktion von Bographwindow net funzt 
 		map <long, Order>* orderList;
		Order* sortedList[MAX_LENGTH]; // ~~
		int orderCount;
		InfoWindow* infoWindow;
		ANARACE* anarace;
};

#endif

