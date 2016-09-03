#ifndef _GUI_MENUENTRY_HPP
#define _GUI_MENUENTRY_HPP

#include "../ui/button.hpp"

class MenuEntry : public UI_Button
{
	public:
		MenuEntry(const MenuEntry& object);
		MenuEntry(UI_Object* menuentry_parent, const Rect rect, const eString unit);
		MenuEntry(UI_Object* menuentry_parent, const Rect rect, const std::string& unit);
		~MenuEntry();
		MenuEntry& operator=(const MenuEntry& object);
		void process();
		void draw(DC* dc) const;
		void setColor(const eColor entry_color);
	private:
		eColor color;
};

#endif // _GUI_MENUENTRY_HPP

