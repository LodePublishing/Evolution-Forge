#ifndef _BOENTRY_HPP
#define _BOENTRY_HPP

#include <string>
#include "../sdl/dc.hpp"
#include "../ui/button.hpp"

class BoEntry : public UI_Button
{
	public:
		BoEntry(const BoEntry& object);
		BoEntry& operator=(const BoEntry& object);
		BoEntry(UI_Object* bo_parent, Rect bo_rect, const string& bo_unit);
		~BoEntry();
		const unsigned int changed();
		void process();
		void draw(DC* dc) const;
		void lock(const bool close=true);
		void unlock(const bool open=true);
		const bool locked() const;

		void setUnit(const unsigned int bo_unit);
		const unsigned int getUnit() const;

		void setIP(const unsigned int bo_ip);
		const unsigned int getIP() const;
	private:
		bool fixed;
		unsigned int unit;
		unsigned int ip;
//        UI_Button* addUnit;
//      UI_Button* subUnit;
//    UI_Button* cancelUnit; TODO
};

inline void BoEntry::setUnit(const unsigned int bo_unit) {
	unit = bo_unit;
}

inline const unsigned int BoEntry::getUnit() const {
	return(unit);
}

inline void BoEntry::setIP(const unsigned int bo_ip) {
	ip = bo_ip;
}

inline const unsigned int BoEntry::getIP() const {
	return(ip);
}


#endif

