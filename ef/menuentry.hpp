#ifndef _GUI_MENUENTRY_HPP
#define _GUI_MENUENTRY_HPP

#include "../ui/button.hpp"

class MenuEntry : public UI_Button
{
	public:
		MenuEntry(UI_Object* parent, Rect rahmen, Rect maxSize, string unit);
		~MenuEntry();
		void updateText(string utext);
		void process();
		void draw(DC* dc) const;
		void setColor(const eColor color);
	private:
		eColor color;
};

#endif // _GUI_MENUENTRY_HPP

