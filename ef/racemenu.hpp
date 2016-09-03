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
		RaceMenu(UI_Object* race_parent, ANARACE* race_anarace, Rect race_rect);
		~RaceMenu();
		void process();
		void draw(DC* dc) const;
	private:
		ANARACE* anarace;
};

#endif // _GUI_RACEMENU_HPP

