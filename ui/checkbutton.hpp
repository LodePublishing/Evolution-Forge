#ifndef _UI_CHECKBUTTON_HPP
#define _UI_CHECKBUTTON_HPP

#include "button.hpp"

class UI_CheckButton : public UI_Object
{
	public:
		UI_CheckButton& operator=(const UI_CheckButton& object);
		UI_CheckButton(const UI_CheckButton& object);
		UI_CheckButton(UI_Object* checkbutton_parent, const Rect rect, const eString txt, const eString tooltip_string, const bool is_checked = false);
		~UI_CheckButton();
		UI_Object* checkTooltip();
		const bool isChecked() const;
		const bool isClicked() const;
		void check(const bool is_checked = true);
		void process();
		void draw(DC* dc) const;
	private:
		bool checked;
		UI_Button* checkButton;
		UI_StaticText* text;
};

#endif

