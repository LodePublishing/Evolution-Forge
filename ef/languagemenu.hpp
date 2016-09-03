#ifndef _LANGUAGEMENU_HPP
#define _LANGUAGEMENU_HPP

#include "../ui/menu.hpp"

class LanguageMenu : public UI_Menu
{
	public:
		LanguageMenu(UI_Object* language_parent, const Rect language_rect, const Size distance_bottom_right, const ePositionMode position_mode);
		~LanguageMenu();
		void reloadOriginalSize();
		void process();
		void draw(DC* dc) const;
};

#endif
