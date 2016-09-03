#ifndef _CORE_STARTCONDITION_HPP
#define _CORE_STARTCONDITION_HPP

#include "unit.hpp"

class START_CONDITION
{
	private:
		UNIT unit[MAX_LOCATIONS];
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
	
		void resetUnits();
		void adjustResearches();		// fill the remaining items with either 0, 1 or 3, depending on NULL, research, upgrade
		void adjustSupply();
		
		void assignRace(const eRace race);
		const eRace getRace() const;

		const std::string& getName() const;
		void setName(const std::string& name);

		const unsigned int getHaveSupply() const;
		void setHaveSupply(const unsigned int haveSupply);
		
		const unsigned int getNeedSupply() const;
		void setNeedSupply(const unsigned int needSupply);

		const unsigned int getGas() const;
		const unsigned int getMinerals() const;

		void setMinerals(const unsigned int minerals);
		void setGas(const unsigned int gas);

		const unsigned int getStartTime() const;
		void setStartTime(const unsigned int startTime);
		
		void setLocationTotal(const unsigned int loc, const unsigned int unit, const unsigned int num);
		void setLocationAvailible(const unsigned int loc, const unsigned int unit, const unsigned int num);
		
		const unsigned int getLocationTotal(const unsigned int loc, const unsigned int unit) const;
		const unsigned int getLocationAvailible(const unsigned int loc, const unsigned int unit) const;

		const UNIT* getUnit(const unsigned int location) const;
		
};

#endif // _CORE_STARTCONDITION_HPP

