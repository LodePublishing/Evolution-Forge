#ifndef _BOENTRY_HPP
#define _BOENTRY_HPP

#include "../sdl/dc.hpp"
#include "../ui/button.hpp"

#include "../core/program.hpp"

class BoEntry : public UI_Button
{
	public:
		BoEntry(const BoEntry& object);
		BoEntry& operator=(const BoEntry& object);
		BoEntry(UI_Object* bo_parent, const Point top_left, const Size distance_bottom_right, const eString bo_unit, const PROGRAM& bo_program, const unsigned int my_number);
		~BoEntry();

		const unsigned int changed();
		void process();
		void draw(DC* dc) const;
//		void lock(const bool close=true);
//		void unlock(const bool open=true);
//		const bool locked() const;

		PROGRAM program;
		void setNumber(const unsigned int my_number);
		const unsigned int getNumber() const;
	private:
		unsigned int number;
//		bool fixed;
//        UI_Button* addUnit;
//      UI_Button* subUnit;
//    UI_Button* cancelUnit; TODO
};

inline void BoEntry::setNumber(const unsigned int my_number)
{
	number = my_number;
}

inline const unsigned int BoEntry::getNumber() const
{
	return(number);
}

#endif

