#ifndef _FORCEENTRY_HPP
#define _FORCEENTRY_HPP

#include "numberfield.hpp"
#include "../core/goal.hpp"

class ForceEntry : public UI_Button
{
	public:
		ForceEntry(UI_Object* entry_parent, const Rect entry_rect, const Rect entry_max_size, const string& entry_unit);
		~ForceEntry();
		ForceEntry(const ForceEntry& object);
		ForceEntry& operator=(const ForceEntry& object);
		
		void updateText(const string& utext);
		void process();
		void draw(DC* dc) const;

		void setTargetForce(const unsigned int force);
		void setTotalNumber(const unsigned int totalNumber);
		void assignGoal(GOAL* goal);
		void setUnit(const unsigned int unit);
		void setType(const eUnitType type);
		const eUnitType getType() const;
		const unsigned int getUnit() const;
		
		void HideIt();

		
		UI_Object* checkHighlight();
	
		NumberField* timeEntryBox;
//		UI_Button* makeLocationGoal;
		UI_Button* makeTimeGoal;
	
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

