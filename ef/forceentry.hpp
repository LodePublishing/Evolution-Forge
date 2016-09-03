#ifndef _FORCEENTRY_HPP
#define _FORCEENTRY_HPP

#include "../ui/numberfield.hpp"
#include "../core/goal.hpp"

enum eForceEntryMessage
{
	NO_MESSAGE,
	NUMBER_HAS_CHANGED,
	GOAL_TIME_HAS_CHANGED
};

class ForceEntry : public UI_Button
{
	public:
		ForceEntry(UI_Object* entry_parent, const Rect entry_rect, const std::string& entry_unit, const unsigned int unit_num, const eUnitType unit_type, GOAL* unit_goal);
		~ForceEntry();
	
		void process();
		void draw(DC* dc) const;

		void setTargetForce(const unsigned int force);
		void setTotalNumber(const unsigned int total_number);
		void assignGoal(GOAL* assign_goal);
		void setUnit(const unsigned int unit_type);
		void setType(const eUnitType unit_type); // ~~

		const eUnitType getType() const;
		const unsigned int getUnit() const;
		const unsigned int getTime() const;

//		const unsigned int getHeight() const;

		//const bool showLocationMenu() const;
//		const bool isLocationGoalClicked() const;
		
		void HideIt();

		static eForceEntryMessage changed;
		static bool forceEntryIsGoal;
		static unsigned int forceEntryUnit;
		static unsigned int forceEntryTime;
		static unsigned int forceEntryLocation;
		static signed int forceEntryCount;

		static bool doCompleteSound;

		static ForceEntry* currentForceEntry;
		void setTime(const unsigned int time);
	private:
		
		static UI_Button* makeTimeGoalButton;
		static UI_NumberField* timeEntryBox;

        	unsigned int oldGoalCount;

		unsigned int oldGoalTime;
		unsigned int oldGoalFinalTime;

		unsigned int startForce;
		unsigned int targetForce;
		unsigned int currentForce;
		unsigned int totalNumber;

		unsigned int highlight;
		unsigned int unit;
		eUnitType type;
		GOAL* goal;
//		bool showLocMenu;
		ForceEntry(const ForceEntry& object);
		ForceEntry& operator=(const ForceEntry& object);
};

inline void ForceEntry::assignGoal(GOAL* assign_goal) {
	goal = assign_goal;
}

inline void ForceEntry::HideIt() {
	Hide();
}

inline void ForceEntry::setTime(const unsigned int time) {
	goal->setTime(time);
}

inline const unsigned int ForceEntry::getTime() const {
	return(goal->getTime());
}

inline void ForceEntry::setUnit(const unsigned int unit_type) {
	unit = unit_type; // TODO
}

inline const unsigned int ForceEntry::getUnit() const {
	return(unit);
}

inline const eUnitType ForceEntry::getType() const {
	return(type);
}

inline void ForceEntry::setType(const eUnitType unit_type) {
	type = unit_type; // TODO
}

#endif // _FORCEENTRY_HPP

