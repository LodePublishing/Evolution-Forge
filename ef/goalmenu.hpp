#ifndef _GUI_GOALMENU_HPP
#define _GUI_GOALMENU_HPP

#include "menu.hpp"
#include "../core/database.hpp"

class GoalMenu : public Menu
{
	public:
		GoalMenu(UI_Object* goal_parent, Rect goal_rect);
		GoalMenu(const GoalMenu& object);
		GoalMenu& operator=(const GoalMenu& object);
		~GoalMenu();
		void process();
		void draw(DC* dc) const;
		void resetData();
		void assignAnarace(ANABUILDORDER* goal_anarace);
	private:
		ANABUILDORDER* anarace;
};

#endif // _GUI_GOALMENU_HPP

