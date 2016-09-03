#ifndef _GUI_BOGRAPH_ENTRY_HPP
#define _GUI_BOGRAPH_ENTRY_HPP

#include "../sdl/dc.hpp"
#include "../ui/button.hpp"

class BoGraphEntry : public UI_Button
{
	public:
		BoGraphEntry(UI_Object* entry_parent, const Rect entry_rect, const int entry_unit);
		BoGraphEntry(const BoGraphEntry& object);
		BoGraphEntry& operator=(const BoGraphEntry& object);
		~BoGraphEntry();
		void process();
		void draw(DC* dc) const;
		const unsigned int getUnit() const;
		void setUnit(const unsigned int bg_unit);

		const unsigned int getIP() const;
		void setIP(const unsigned int bg_ip);
	private:
		unsigned int unit;
		unsigned int ip;
};

#endif

