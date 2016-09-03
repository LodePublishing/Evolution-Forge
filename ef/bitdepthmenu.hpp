#ifndef _BITDEPTH_MENU_HPP
#define _BITDEPTH_MENU_HPP

#include "menu.hpp"

class BitDepthMenu : public Menu
{
	public:
		BitDepthMenu(UI_Object* bit_depth_menu_parent, const Rect rect, const Size distance_bottom_right, const ePositionMode position_mode);
		~BitDepthMenu();
		void reloadOriginalSize();
		void process();
		void draw(DC* dc) const;
};

#endif

