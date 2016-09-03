#ifndef _GUI_RACEMENU_HPP
#define _GUI_RACEMENU_HPP

#include "../ui/menu.hpp"

class RaceMenu : public UI_Menu
{
	public:
		RaceMenu& operator=(const RaceMenu& object);
		RaceMenu(const RaceMenu& object);
		RaceMenu(UI_Object* race_parent, const Rect race_rect, const Size distance_bottom_right, const ePositionMode position_mode);
		~RaceMenu();
		void process();
		void draw(DC* dc) const;
	private:
};

#endif // _GUI_RACEMENU_HPP

