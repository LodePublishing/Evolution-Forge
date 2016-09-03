#ifndef _CORE_STARTCONDITION_HPP
#define _CORE_STARTCONDITION_HPP

#include "unit.hpp"

class START_CONDITION
{
	private:
		UNIT unit[MAX_LOCATIONS]; // sehr seltsam, bei [2] funzts, bei [3] funzts net :[
		eRace race;
		std::string name;
		
		unsigned int haveSupply;
		unsigned int needSupply;
		unsigned int minerals;
		unsigned int gas;
		unsigned int startTime;
	public:
		START_CONDITION();
		~START_CONDITION();
		START_CONDITION(const START_CONDITION& object);
		START_CONDITION& operator=(const START_CONDITION& object);
	
		void resetUnits();
		void adjustResearches();		// fill the remaining items with either 0, 1 or 3, depending on NULL, research, upgrade
		void adjustSupply();
		
		void assignRace(const eRace start_race);
		const eRace getRace() const;

		const std::string& getName() const;
		void setName(const std::string& startcondition_name);

		const unsigned int getHaveSupply() const;
		void setHaveSupply(const unsigned int start_have_supply);
		
		const unsigned int getNeedSupply() const;
		void setNeedSupply(const unsigned int start_need_supply);

		const unsigned int getGas() const;
		const unsigned int getMinerals() const;

		void setMinerals(const unsigned int start_minerals);
		void setGas(const unsigned int start_gas);

		const unsigned int getStartTime() const;
		void setStartTime(const unsigned int start_time);
		
		void setLocationTotal(const unsigned int loc, const unsigned int unit_type, const unsigned int unit_count);
		void setLocationAvailible(const unsigned int loc, const unsigned int unit_type, const unsigned int unit_count);
		
		const unsigned int getLocationTotal(const unsigned int loc, const unsigned int unit_type) const;
		const unsigned int getLocationAvailible(const unsigned int loc, const unsigned int unit_type) const;

		const UNIT* getUnit(const unsigned int location) const;
		
};

#endif // _CORE_STARTCONDITION_HPP

