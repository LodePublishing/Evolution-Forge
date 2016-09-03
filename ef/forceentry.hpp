#ifndef _FORCEENTRY_HPP
#define _FORCEENTRY_HPP

#include "../ui/numberfield.hpp"
#include "../core/goal.hpp"

enum eForceEntryMessage
{
	NO_MESSAGE,
	LEFT_CLICKED,
	RIGHT_CLICKED,
	GOAL_TIME_HAS_CHANGED
};

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

		const unsigned int getHeight() const;

		//const bool showLocationMenu() const;
		const bool isLocationGoalClicked() const;
		
		void HideIt();

		UI_Object* checkTooltip();
		UI_Object* checkHighlight();
	
		UI_NumberField* timeEntryBox;
		UI_Button* makeLocationGoal;
		UI_Button* makeTimeGoal;
		static eForceEntryMessage changed;
		static bool forceEntryIsGoal;
		static unsigned int forceEntryUnit;
		static unsigned int forceEntryTime;
		static unsigned int forceEntryLocation;
	private:
        
		unsigned int startForce;
		unsigned int targetForce;
		unsigned int currentForce;
		unsigned int totalNumber;

		unsigned int unit;
		eUnitType type;
		GOAL* goal;
		bool showLocMenu;
};

#endif // _FORCEENTRY_HPP

