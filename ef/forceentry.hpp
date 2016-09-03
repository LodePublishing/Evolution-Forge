#ifndef _FORCEENTRY_HPP
#define _FORCEENTRY_HPP

#include "../ui/numberfield.hpp"
#include "../core/goal.hpp"

class ForceEntry : public UI_Button
{
	public:
		ForceEntry(UI_Object* entry_parent, const Rect entry_rect, const Rect entry_max_size, const string& entry_unit);
		~ForceEntry();
		ForceEntry(const ForceEntry& object);
		ForceEntry& operator=(const ForceEntry& object);
		
		void process();
		void draw(DC* dc) const;

		void setTargetForce(const unsigned int force);
		void setTotalNumber(const unsigned int total_number);
		void assignGoal(GOAL* assign_goal);
		void setUnit(const unsigned int unit_type);
		void setType(const eUnitType unit_type); // ~~
		const eUnitType getType() const;
		const unsigned int getUnit() const;
		
		void HideIt();

		UI_Object* checkTooltip();
		UI_Object* checkHighlight();
	
		UI_NumberField* timeEntryBox;
//		UI_Button* makeLocationGoal;
		UI_Button* makeTimeGoal;
		static bool changed;
	
	private:
        
		unsigned int startForce;
		unsigned int targetForce;
		unsigned int currentForce;
		unsigned int totalNumber;

		unsigned int unit;
		eUnitType type;
		GOAL* goal;
//		static LocationMenu* locationMenu;
};

#endif // _FORCEENTRY_HPP

