#ifndef _GUI_GOALMENU_HPP
#define _GUI_GOALMENU_HPP

#include "menu.hpp"
#include "../core/settings.hpp"

class GoalMenu : public Menu
{
	public:
		GoalMenu(UI_Object* goal_parent, ANARACE* goal_anarace, Rect goal_rect);
		GoalMenu(const GoalMenu& object);
		GoalMenu& operator=(const GoalMenu& object);
		~GoalMenu();
		void process();
		void draw(DC* dc) const;
	private:
		ANARACE* anarace;
};

#endif // _GUI_GOALMENU_HPP

