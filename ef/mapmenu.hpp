#ifndef _GUI_MAPMENU_HPP
#define _GUI_MAPMENU_HPP

#include "../ui/menu.hpp"
#include "../core/database.hpp"

class MapMenu : public UI_Menu
{
	public:
		MapMenu(UI_Object* map_parent, const Rect map_rect, const Size distance_bottom_right, const ePositionMode position_mode);
		~MapMenu();
		void process();
		void draw(DC* dc) const;
		void resetData();
		void reloadOriginalSize();
	private:
		MapMenu(const MapMenu& object);
		MapMenu& operator=(const MapMenu& object);
};

#endif // _GUI_MAPMENU_HPP

