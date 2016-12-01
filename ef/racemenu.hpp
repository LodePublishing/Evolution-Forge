#ifndef _GUI_RACEMENU_HPP
#define _GUI_RACEMENU_HPP

#include "../ui/menu.hpp"

class RaceMenu : public UI_Menu
{
	public:
		RaceMenu(UI_Object* race_parent, const Rect race_rect, const Size distance_bottom_right, const ePositionMode position_mode);
		~RaceMenu();
		const bool addKey(unsigned int key, unsigned int mod);
		void process();
		void draw() const;
	private:
};

#endif // _GUI_RACEMENU_HPP

