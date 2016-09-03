#ifndef _GUI_MAPMENU_HPP
#define _GUI_MAPMENU_HPP

#include "menu.hpp"
#include "../core/database.hpp"

class MapMenu : public Menu
{
	public:
		MapMenu(UI_Object* map_parent, const Rect map_rect, const Size distance_bottom_right, const ePositionMode position_mode);
		MapMenu(const MapMenu& object);
		MapMenu& operator=(const MapMenu& object);
		~MapMenu();
		void process();
		void draw(DC* dc) const;
		void resetData();
		void assignAnarace(ANABUILDORDER* map_anarace);
	private:
		ANABUILDORDER* anarace;
};

#endif // _GUI_MAPMENU_HPP

