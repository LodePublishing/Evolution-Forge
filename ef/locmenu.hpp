#ifndef _GUI_LOCATIONMENU_HPP
#define _GUI_LOCATIONMENU_HPP

#include "menu.hpp"
#include "../core/anabuildorder.hpp"

class LocationMenu : public Menu
{
	public:
		LocationMenu(UI_Object* location_parent, ANABUILDORDER* location_anarace, Rect location_rect);
		LocationMenu(const LocationMenu& object);
		LocationMenu& operator=(const LocationMenu& object);
		~LocationMenu();
		void process();
		void draw(DC* dc) const;
		void resetData();
		void assignAnarace(ANABUILDORDER* location_anarace);
	private:
		ANABUILDORDER* anarace;
};

#endif // _GUI_LOCATIONMENU_HPP

