#ifndef _BOENTRY_HPP
#define _BOENTRY_HPP

#include <string>
#include "../sdl/dc.hpp"
#include "../ui/button.hpp"

#include "../core/program.hpp"

class BoEntry : public UI_Button
{
	public:
		BoEntry(const BoEntry& object);
		BoEntry& operator=(const BoEntry& object);
		BoEntry(UI_Object* bo_parent, Rect bo_rect, const string& bo_unit, const PROGRAM& bo_program);
		~BoEntry();

		const unsigned int changed();
		void process();
		void draw(DC* dc) const;
//		void lock(const bool close=true);
//		void unlock(const bool open=true);
//		const bool locked() const;

		PROGRAM program;
	private:
//		bool fixed;
//        UI_Button* addUnit;
//      UI_Button* subUnit;
//    UI_Button* cancelUnit; TODO
};

#endif

