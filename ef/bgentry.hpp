#ifndef _GUI_BOGRAPH_ENTRY_HPP
#define _GUI_BOGRAPH_ENTRY_HPP

#include "../ui/button.hpp"
#include "../core/program.hpp"

class BoGraphEntry : public UI_Button
{
	public:
		BoGraphEntry(UI_Object* entry_parent, const Rect bg_rect, const Size distance_bottom_right, const PROGRAM& bg_program, const unsigned int my_number);
		~BoGraphEntry();
		
		void process();
		void draw() const;
		PROGRAM program;

		void setNumber(const unsigned int my_number);
		const unsigned int getNumber() const;
	private:
		unsigned int number;
	
		BoGraphEntry(const BoGraphEntry& object);
		BoGraphEntry& operator=(const BoGraphEntry& object);
};

inline void BoGraphEntry::setNumber(const unsigned int my_number) {
	number = my_number;
}

inline const unsigned int BoGraphEntry::getNumber() const {
	return(number);
}

#endif

