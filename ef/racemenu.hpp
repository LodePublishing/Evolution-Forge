#ifndef _GUI_RACEMENU_HPP
#define _GUI_RACEMENU_HPP

#include "menu.hpp"
#include "../core/anarace.hpp"
#include "../core/settings.hpp"

class RaceMenu : public Menu
{
	public:
		RaceMenu& operator=(const RaceMenu& object);
		RaceMenu(const RaceMenu& object);
		RaceMenu(UI_Object* race_parent, Rect race_rect);
		~RaceMenu();
		void process();
		void draw(DC* dc) const;
	private:
};

#endif // _GUI_RACEMENU_HPP

