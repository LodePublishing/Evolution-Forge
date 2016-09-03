#ifndef _GUI_FORCEMENU_HPP
#define _GUI_FORCEMENU_HPP

#include "menu.hpp"
#include "../core/anabuildorder.hpp"
#include "../core/database.hpp"

class ForceMenu : public Menu
{
	public:
		ForceMenu(UI_Object* force_parent, ANABUILDORDER* force_anarace, const Rect force_rect, const Size distance_bottom_right, const ePositionMode position_mode);
		ForceMenu(const ForceMenu& object);
		ForceMenu& operator=(const ForceMenu& object);
		~ForceMenu();
		void process();
		void draw(DC* dc) const;
	private:
};

#endif // _GUI_FORCEMENU_HPP

