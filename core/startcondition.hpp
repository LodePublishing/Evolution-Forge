#ifndef _CORE_STARTCONDITION_HPP
#define _CORE_STARTCONDITION_HPP

#include "unit.hpp"

class EXPORT START_CONDITION
{
	private:
		UNIT unit[MAX_LOCATIONS];
		eRace race;
		string name;
		
		int haveSupply;
		int needSupply;
		int minerals;
		int gas;
		int startTime;
	public:
		START_CONDITION();
		~START_CONDITION();
	
		void resetUnits();
		void adjustResearches();		// fill the remaining items with either 0, 1 or 3, depending on NULL, research, upgrade
		void adjustSupply();
		
		void assignRace(const eRace race);
		const eRace getRace() const;

		const string& getName() const;
		void setName(const string& name);

		const int getHaveSupply() const;
		void setHaveSupply(const int haveSupply);
		
		const int getNeedSupply() const;
		void setNeedSupply(const int needSupply);

		const int getGas() const;
		const int getMinerals() const;

		void setMinerals(const int minerals);
		void setGas(const int gas);

		const int getStartTime() const;
		void setStartTime(const int startTime);
		
		void setLocationTotal(const int loc, const int unit, const int num);
		void setLocationAvailible(const int loc, const int unit, const int num);
		
		const int getLocationTotal(const int loc, const int unit) const;
		const int getLocationAvailible(const int loc, const int unit) const;

		const UNIT* getUnit(const int location) const;
		
};

#endif // _CORE_STARTCONDITION_HPP

