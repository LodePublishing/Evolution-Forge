#ifndef _GUI_BOGRAPH_LINE_HPP
#define _GUI_BOGRAPH_LINE_HPP

#include "bgentry.hpp"

#include <list>

class BoGraphLine : public UI_Object
{
	public:
		BoGraphLine(UI_Object* bg_parent, const Rect& initial_rect, const eRace bg_race, const unsigned int bg_facility, const unsigned int bg_line_height, const unsigned int bg_lines);
		~BoGraphLine();
		
		void resetData();
		void checkSelected(const unsigned int selected);

		void process();
		void draw(DC* dc) const;

		void setRace(const eRace race);
		void setFacility(const unsigned int bg_facility);
		void setLineHeight(const unsigned int bg_line_height);
		void setLines(const unsigned int bg_lines);

		const eRace getRace() const;
		const unsigned int getFacility() const;
		const unsigned int getLineHeight() const;
		const unsigned int getLines() const;
		std::list<BoGraphEntry*> boGraphList;
	private:
		eRace race;
		unsigned int facility;
		unsigned int lineHeight; // number of entries per line
		unsigned int lines; // number of lines
		bool facilityChanged;
		UI_StaticText* facilityName;
};

inline void BoGraphLine::setRace(const eRace bg_race) {
	if(bg_race == race)
		return;
	race = bg_race;
	facilityChanged = true;
}

inline void BoGraphLine::setFacility(const unsigned int bg_facility) {
	if(facility == bg_facility)
		return;
	facility = bg_facility;
	facilityChanged = true;
}

inline void BoGraphLine::setLineHeight(const unsigned int bg_line_height) {
	lineHeight = bg_line_height;
}

inline void BoGraphLine::setLines(const unsigned int bg_lines) {
	lines = bg_lines;
}

inline const eRace BoGraphLine::getRace() const {
	return(race);
}

inline const unsigned int BoGraphLine::getFacility() const {
	return(facility);
}

inline const unsigned int BoGraphLine::getLineHeight() const {
	return(lineHeight);
}

inline const unsigned int BoGraphLine::getLines() const {
	return(lines);
}


#endif
