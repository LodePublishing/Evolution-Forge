#ifndef _GUI_BOGRAPH_HPP
#define _GUI_BOGRAPH_HPP

#include "../ui/window.hpp"
#include "bograph.hpp"
#include "order.hpp"
#include "info.hpp"

#include <list>

#define BOGRAPH_MAX_LINES 20

class BoGraphWindow:public UI_Window
{
	public:
		BoGraphWindow(UI_Object* bograph_parent, ANARACE* bograph_anarace, InfoWindow* bograph_info_window, std::list<Order*>* bograph_order_list, const unsigned int bograph_window_number);
		BoGraphWindow(const BoGraphWindow& object);
		BoGraphWindow& operator=(const BoGraphWindow& object);
		~BoGraphWindow();
		
		void resetData();
		void process();
		void processList();
		void draw(DC* dc) const;
		void setMarkedIP(const unsigned int marked_ip);
		const unsigned int getMarkedIP() const;
		void setMarkedUnit(const unsigned int marked_unit);
		const unsigned int getMarkedUnit() const;
	    	void mouseHasMoved();
		void assignAnarace(ANARACE* bograph_anarace);
	private:

		struct BOGRAPH
		{
			unsigned int facility;
			unsigned int height; // number of entries per line
			unsigned int lines; // number of lines
			Rect edge;
			list<BoGraphEntry*> boGraphList;
		} bograph[BOGRAPH_MAX_LINES];
	
		void checkForInfoWindow();
		
//		list<BoGraphEntry*> boGraphList;

		unsigned int markedUnit;
		unsigned int markedIP;
	
		unsigned int ownMarkedUnit;
		unsigned int ownMarkedIP;
	
		unsigned int markAni;
 		std::list<Order*>* orderList;
		InfoWindow* infoWindow;
		ANARACE* anarace;
};

#endif

