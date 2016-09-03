#ifndef _GUI_GOALMENU_HPP
#define _GUI_GOALMENU_HPP

#include "menu.hpp"
#include "../core/settings.hpp"

class GoalMenu : public Menu
{
	public:
		GoalMenu(UI_Object* parent, ANARACE* anarace, Rect rect);
		~GoalMenu();
		void process();
		void draw(DC* dc) const;
	private:
};

#endif // _GUI_GOALMENU_HPP

