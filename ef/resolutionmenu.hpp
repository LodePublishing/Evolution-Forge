#ifndef _RESOLUTIONMENU_HPP
#define _RESOLUTIONMENU_HPP

#include "menu.hpp"

class ResolutionMenu : public Menu
{
    public:
        ResolutionMenu(UI_Object* resmenu_parent, Rect rect);
		~ResolutionMenu();
        void process();
        void draw(DC* dc) const;
};

#endif

