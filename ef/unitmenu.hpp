#ifndef _GUI_UNITMENU_HPP
#define _GUI_UNITMENU_HPP

#include "menu.hpp"

#include "../core/anarace.hpp"
#include "../core/settings.hpp"

class UnitMenu : public Menu
{
	public:
		UnitMenu(UI_Object* parent, ANARACE* anarace, Rect rect);
		~UnitMenu();
		void process();
		void draw(DC* dc) const;
		const int getMarkedUnit() const;
	private:
		int markedUnit;
};

#endif // _GUI_UNITMENU_HPP

