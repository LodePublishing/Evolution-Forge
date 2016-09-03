#ifndef _CHECKBUTTON_HPP
#define _CHECKBUTTON_HPP

#include "../ui/button.hpp"

class CheckButton : public UI_Object
{
	public:
		CheckButton& operator=(const CheckButton& object);
		CheckButton(const CheckButton& object);
		CheckButton(UI_Object* parent, const Rect rect, const eString txt, const eString tooltip, const bool checked = false);
		~CheckButton();
		const bool isChecked() const;
		const bool isClicked() const;
		void check(const bool checked = true);
		void process();
		void draw(DC* dc) const;
	private:
		bool checked;
		UI_Button* checkButton;
		UI_StaticText* text;
};

#endif

