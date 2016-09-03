#ifndef _GUI_THEMEMENU_HPP
#define _GUI_THEMEMENU_HPP

#include "../ui/menu.hpp"

class ThemeMenu : public UI_Menu
{
	public:
	        ThemeMenu(UI_Object* thememenu_parent, const Rect rect, const Size distance_bottom_right, const ePositionMode position_mode);
		~ThemeMenu();
		void reloadOriginalSize();
	        void process();
        	void draw(DC* dc) const;
};

#endif

