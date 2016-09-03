#ifndef _GUI_FORCEMENU_HPP
#define _GUI_FORCEMENU_HPP

#include "menu.hpp"
#include "../core/anarace.hpp"
#include "../core/settings.hpp"

class ForceMenu : public Menu
{
	public:
		ForceMenu(UI_Object* force_parent, ANARACE* force_anarace, Rect force_rect);
		ForceMenu(const ForceMenu& object);
		ForceMenu& operator=(const ForceMenu& object);
		~ForceMenu();
		void process();
		void draw(DC* dc) const;
	private:
};

#endif // _GUI_FORCEMENU_HPP

