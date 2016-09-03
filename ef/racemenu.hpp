#ifndef _GUI_RACEMENU_HPP
#define _GUI_RACEMENU_HPP

#include "menu.hpp"
#include "../core/anarace.hpp"
#include "../core/settings.hpp"

class RaceMenu : public Menu
{
	public:
		RaceMenu(UI_Object* parent, ANARACE* anarace, Rect rect);
		~RaceMenu();
		void process();
		void draw(DC* dc) const;
};

#endif // _GUI_RACEMENU_HPP

