#ifndef _GUI_BOGRAPH_ENTRY_HPP
#define _GUI_BOGRAPH_ENTRY_HPP

#include "../sdl/dc.hpp"
#include "../ui/button.hpp"

#include "../core/program.hpp"

class BoGraphEntry : public UI_Button
{
	public:
		BoGraphEntry(UI_Object* entry_parent, const Point top_left, const Size distance_bottom_right, const PROGRAM& bg_program);
		BoGraphEntry(const BoGraphEntry& object);
		BoGraphEntry& operator=(const BoGraphEntry& object);
		~BoGraphEntry();
		
		void process();
		void draw(DC* dc) const;
		PROGRAM program;
	private:
};

#endif

