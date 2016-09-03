#ifndef _GAMEMENU_HPP
#define _GAMEMENU_HPP

#include "../ui/menu.hpp"

class GameMenu : public UI_Menu
{
	public:
		GameMenu(UI_Object* game_parent, const Rect& game_rect, const Size distance_bottom_right, const ePositionMode position_mode);
		~GameMenu();
		void process();
		void draw() const;
};

#endif

