#ifndef _GUI_UNITMENU_HPP
#define _GUI_UNITMENU_HPP

#include "menu.hpp"

#include "../core/anarace.hpp"
#include "../core/settings.hpp"

class UnitMenu : public Menu
{
	public:
		UnitMenu(UI_Object* unit_parent, ANARACE* unit_anarace, Rect unit_rect);
		UnitMenu(const UnitMenu& object);
		UnitMenu& operator=(const UnitMenu& object);
		~UnitMenu();
		void process();
		void draw(DC* dc) const;
		void setFacilityModus(const bool facility_mode=true);
		void resetData();
		void assignAnarace(ANARACE* unit_anarace);
		const bool secondLevel() const;
	private:
		ANARACE* anarace;
// sort units in the addgoal menu rather by facility than by unitType		
		unsigned int facilityNumber;
		unsigned int facility[GAS_SCV+1];
		// facility number -> real facility 
};

#endif // _GUI_UNITMENU_HPP

