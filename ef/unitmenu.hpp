#ifndef _GUI_UNITMENU_HPP
#define _GUI_UNITMENU_HPP

#include "menu.hpp"
#include "../core/anabuildorder.hpp"

class UnitMenu : public Menu
{
	public:
		UnitMenu(UI_Object* unit_parent, const Rect unit_rect, const Size distance_bottom_right, const ePositionMode position_mode);
		UnitMenu(const UnitMenu& object);
		UnitMenu& operator=(const UnitMenu& object);
		~UnitMenu();
		void process();
		void draw(DC* dc) const;
		void setFacilityModus(const bool facility_mode=true);
		void resetData();
		void assignAnarace(ANABUILDORDER* unit_anarace);
		const bool secondLevel() const;

		void reloadOriginalSize();
	private:
		ANABUILDORDER* anarace;
// sort units in the addgoal menu rather by facility than by unitType		
		unsigned int facilityNumber;
		unsigned int facility[GAS_SCV+1];
		// facility number -> real facility 
//		void setMenuLevel(const unsigned int menu_level);
		void processMenu();
};

#endif // _GUI_UNITMENU_HPP

