#ifndef _GUI_LOCATIONMENU_HPP
#define _GUI_LOCATIONMENU_HPP

#include "menu.hpp"
#include "../core/settings.hpp"

class LocationMenu : public Menu
{
	public:
		LocationMenu(UI_Object* location_parent, ANARACE* location_anarace, Rect location_rect);
		LocationMenu(const LocationMenu& object);
		LocationMenu& operator=(const LocationMenu& object);
		~LocationMenu();
		void process();
		void draw(DC* dc) const;
		void resetData();
		void assignAnarace(ANARACE* location_anarace);
	private:
		ANARACE* anarace;
};

#endif // _GUI_LOCATIONMENU_HPP

