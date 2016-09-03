#ifndef _UI_MENUENTRY_HPP
#define _UI_MENUENTRY_HPP

#include "button.hpp"

class UI_MenuEntry : public UI_Button
{
	public:
		UI_MenuEntry(const UI_MenuEntry& object);
		UI_MenuEntry(UI_Object* menuentry_parent, const Rect rect, const eString unit);
		UI_MenuEntry(UI_Object* menuentry_parent, const Rect rect, const std::string& unit);
		~UI_MenuEntry();
		UI_MenuEntry& operator=(const UI_MenuEntry& object);
		void process();
		void draw(DC* dc) const;
		void setColor(const eColor entry_color);
	private:
		eColor color;
};

#endif // _UI_MENUENTRY_HPP

