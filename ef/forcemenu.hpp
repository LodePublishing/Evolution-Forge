#ifndef _GUI_FORCEMENU_HPP
#define _GUI_FORCEMENU_HPP

#include "menu.hpp"
#include "../core/anarace.hpp"
#include "../core/settings.hpp"

class ForceMenu : public Menu
{
	public:
		ForceMenu(UI_Object* parent, ANARACE* anarace, Rect rect);
		~ForceMenu();
		void process();
		void draw(DC* dc) const;
};

#endif // _GUI_FORCEMENU_HPP

