#ifndef _UI_NUMBERFIELD_HPP
#define _UI_NUMBERFIELD_HPP

#include "button.hpp"

enum eFieldType
{
	NORMAL_NUMBER_TYPE,
	PERCENT_NUMBER_TYPE,
	TIME_NUMBER_TYPE
};

class UI_NumberField : public UI_Object
{
	public:
		UI_NumberField& operator=(const UI_NumberField& object);
		UI_NumberField(const UI_NumberField& object);
		
		UI_NumberField(UI_Object* numberfield_parent, const Rect field_rect, const Size distance_bottom_right, const ePositionMode position_mode, const unsigned int number_min, const unsigned int number_max, const unsigned int number_steps=1, const unsigned int num=0, const eString txt=NULL_STRING, const eString tool_tip=NULL_STRING, const eFieldType field_type = NORMAL_NUMBER_TYPE);

		~UI_NumberField();
		UI_Object* checkTooltip();
		
		const bool addClicked() const;
		const bool subClicked() const;
		const bool addRightClicked() const;
		const bool subRightClicked() const;
	
		const unsigned int getNumber() const;
		void updateNumber(const unsigned int num);
		void process();
		void draw(DC* dc) const;
	private:
		eFieldType fieldType;
		UI_Button* addbutton;
		UI_Button* subbutton;
		UI_StaticText* text;
		UI_StaticText* numberText;
		unsigned int number;
		unsigned int min;
		unsigned int max;
		unsigned int steps;
};

inline const unsigned int UI_NumberField::getNumber() const {
	return(number);
}

inline const bool UI_NumberField::addClicked() const {
	return(addbutton->isLeftClicked());
}

inline const bool UI_NumberField::subClicked() const {
	return(subbutton->isLeftClicked());
}

inline const bool UI_NumberField::addRightClicked() const {
	return(addbutton->isRightClicked());
}

inline const bool UI_NumberField::subRightClicked() const {
	return(subbutton->isRightClicked());
}

#endif

