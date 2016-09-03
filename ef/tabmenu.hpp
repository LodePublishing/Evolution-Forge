#ifndef _GUI_TABMENU_HPP
#define _GUI_TABMENU_HPP

#include "menu.hpp"
#include "../ui/button.hpp"

class TabMenu : public UI_Object
{
	public:
		TabMenu(UI_Object* parent, Rect rect);
		~TabMenu();
        void addMenu(UI_Button* button);
        const int getMarked() const;
        void forceTabPressed(const int tab);
        void process();
        void draw(DC* dc) const;
        const bool hasChanged() const;
    private:
        bool changed;
};

#endif // _GUI_MENU_HPP

