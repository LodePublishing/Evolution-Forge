#ifndef _NUMBERFIELD_HPP
#define _NUMBERFIELD_HPP

#include "../ui/button.hpp"

class NumberField : public UI_Object
{
	public:
		NumberField& operator=(const NumberField& object);
		NumberField(const NumberField& object);
		NumberField(UI_Object* parent, const Rect rect, const eString txt, const eString tooltip, const int number = 0, const bool percent = false);
		~NumberField();
		const bool addClicked() const;
		const bool subClicked() const;
		void updateNumber(const int number);
		void process();
		void draw(DC* dc) const;
	private:
		bool percent;
		UI_Button* addbutton;
		UI_Button* subbutton;
		UI_StaticText* text;
		UI_StaticText* number;
};

#endif

