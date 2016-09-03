#ifndef _LANGUAGEMENU_HPP
#define _LANGUAGEMENU_HPP

#include "menu.hpp"

class LanguageMenu : public Menu
{
    public:
        LanguageMenu(UI_Object* language_parent, Rect language_rect);
		~LanguageMenu();
        void process();
        void draw(DC* dc) const;
};

#endif

