#ifndef _RESOLUTION_MENU_HPP
#define _RESOLUTION_MENU_HPP

#include "menu.hpp"

class ResolutionMenu : public Menu
{
	public:
		ResolutionMenu(UI_Object* resolution_menu_parent, const Rect rect, const Size distance_bottom_right, const ePositionMode position_mode);
		~ResolutionMenu();
		void reloadOriginalSize();
		void process();
		void draw(DC* dc) const;
};

#endif

