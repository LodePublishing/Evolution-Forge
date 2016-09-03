#ifndef _GUI_TECHTREE_HPP
#define _GUI_TECHTREE_HPP

#include "../ui/window.hpp"
#include "../core/database.hpp"
#include "../core/anabuildorder.hpp"


class TechTreeWindow : public UI_Window
{
	public:
		TechTreeWindow(UI_Object* tree_parent);
		~TechTreeWindow();
		
		void process();
		void draw(DC* dc) const;
		void assignAnarace(ANABUILDORDER* techtree_anarace);
		void setCurrentGoalUnit(unsigned int current_goal_unit);
		void reloadOriginalSize();
	private:
		void processList();
		bool unitChanged;
		unsigned int currentGoalUnit; // which goal is currently highlighted?
		ANABUILDORDER* anarace;
		Size s, s2;
		GOAL_TREE tree;
		UI_Object* node[UNIT_TYPE_COUNT];
};

inline void TechTreeWindow::setCurrentGoalUnit(unsigned int current_goal_unit)
{
	if(currentGoalUnit == current_goal_unit)
		return;
	currentGoalUnit = current_goal_unit;
	unitChanged = true;
}
#endif
