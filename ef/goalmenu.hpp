#ifndef _GUI_GOALMENU_HPP
#define _GUI_GOALMENU_HPP

#include "../ui/menu.hpp"
#include "../core/database.hpp"

class GoalMenu : public UI_Menu
{
	public:
		GoalMenu(UI_Object* goal_parent, const Rect& goal_rect, const Size distance_bottom_right, const ePositionMode position_mode);
		~GoalMenu();
		
		void process();
		void draw(DC* dc) const;
		void resetData();
		void assignAnarace(ANABUILDORDER* goal_anarace);
		void reloadOriginalSize();
		const bool addKey(unsigned int key, unsigned int mod);
	private:
		ANABUILDORDER* anarace;
		unsigned int lastRace;
	
		GoalMenu(const GoalMenu& object);
		GoalMenu& operator=(const GoalMenu& object);
};

#endif // _GUI_GOALMENU_HPP

