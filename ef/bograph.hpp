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
class BOGRAPH
{
	public:
		BOGRAPH();
		BOGRAPH(const BOGRAPH& object);
		BOGRAPH& operator=(const BOGRAPH& object);
		~BOGRAPH();
		unsigned int type;
		unsigned int position;
		unsigned int height;
		unsigned int lines;
		Rect edge;
};

class BoGraphWindow:public UI_Window
{
	public:
		BoGraphWindow(UI_Object* bograph_parent, ANARACE* bograph_anarace, InfoWindow* bograph_info_window, std::map <long, Order>* bograph_order_list, const unsigned int bograph_window_number);
		BoGraphWindow(const BoGraphWindow& object);
		BoGraphWindow& operator=(const BoGraphWindow& object);
		~BoGraphWindow();
		
		void resetData();
		void process();
		void draw(DC* dc) const;
		void setMarkedIP(const unsigned int marked_ip);
		const unsigned int getMarkedIP() const;
		void setMarkedUnit(const unsigned int marked_unit);
		const unsigned int getMarkedUnit() const;
		void copyBoGraph(const BOGRAPH* bo_graph);
		void setBoGraphY(const unsigned int last_bograph_y);
	    	void mouseHasMoved();
		void assignAnarace(ANARACE* bograph_anarace);
	private:
		void checkForInfoWindow();

		
		BOGRAPH bograph[20];
		unsigned int lastbographY;
		unsigned int targetbographY;
		unsigned int startbographY;

		unsigned int markedUnit;
		unsigned int markedIP;
	
		unsigned int ownMarkedUnit;
		unsigned int ownMarkedIP;
	
		unsigned int markAni;
		// TODO orderList bei jedem Schritt kopieren oder so... :/
		// WEIL: die orderlist veraendert wird von CheckOrders von player und somit die DrawFunktion von Bographwindow net funzt 
 		std::map <long, Order>* orderList;
//		Order* sortedList[MAX_LENGTH]; // ~~
//		int orderCount;
		InfoWindow* infoWindow;
		ANARACE* anarace;
};

#endif

