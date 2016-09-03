#ifndef _THEMEMENU_HPP
#define _THEMEMENU_HPP

#include "menu.hpp"

class ThemeMenu : public Menu
{
    public:
        ThemeMenu(UI_Object* parent, Rect rect);
		~ThemeMenu();
        void process();
        void draw(DC* dc) const;
};

#endif

