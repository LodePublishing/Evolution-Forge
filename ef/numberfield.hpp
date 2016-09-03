#ifndef _NUMBERFIELD_HPP
#define _NUMBERFIELD_HPP

#include "../ui/button.hpp"

enum eFieldType
{
	NORMAL_NUMBER_TYPE,
	PERCENT_NUMBER_TYPE,
	TIME_NUMBER_TYPE
};

class NumberField : public UI_Object
{
	public:
		NumberField& operator=(const NumberField& object);
		NumberField(const NumberField& object);
		
		NumberField(UI_Object* parent, const Rect rect, const int min, const int max, const unsigned int steps=1, const int number=0, const eString txt=NULL_STRING, const eString tooltip=NULL_STRING, const eFieldType field_type = NORMAL_NUMBER_TYPE);

		~NumberField();
		const bool addClicked() const;
		const bool subClicked() const;
		const bool addRightClicked() const;
		const bool subRightClicked() const;
	
		const int getNumber() const;
		void updateNumber(const int number);
		void process();
		void draw(DC* dc) const;
	private:
		eFieldType fieldType;
		UI_Button* addbutton;
		UI_Button* subbutton;
		UI_StaticText* text;
		UI_StaticText* number;
		int num;
		int min;
		int max;
		unsigned int steps;
};

#endif

