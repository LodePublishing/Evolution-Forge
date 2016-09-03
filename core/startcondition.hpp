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

inline void START_CONDITION::assignRace(const eRace start_race) {
	race = start_race;
}
			
inline const eRace START_CONDITION::getRace() const {
	return(race);
}

inline const UNIT* START_CONDITION::getUnit(const unsigned int location) const 
{
#ifdef _SCC_DEBUG
	if(location >= MAX_LOCATIONS) { // TODO
		toLog("DEBUG: (START_CONDITION::getUnit): Value location out of range.");return(0);
	} 
#endif
	return(&unit[location]);
}

inline const std::string& START_CONDITION::getName() const {
	return(name);
}

inline void START_CONDITION::setName(const std::string& startcondition_name) {
	name = startcondition_name;
}

inline const unsigned int START_CONDITION::getHaveSupply() const 
{
#ifdef _SCC_DEBUG
	if(haveSupply > MAX_SUPPLY) {
		toLog("DEBUG: (START_CONDITION::getHaveSupply): Value haveSupply out of range.");return(0);
	}
#endif
	return(haveSupply);
}

inline void START_CONDITION::setHaveSupply(const unsigned int start_have_supply) 
{
#ifdef _SCC_DEBUG
	if(start_have_supply > MAX_SUPPLY) {
		toLog("DEBUG: (START_CONDITION::setHaveSupply): Value start_have_supply out of range.");return;
	}
#endif
	haveSupply = start_have_supply;
}
		
inline const unsigned int START_CONDITION::getNeedSupply() const
{
#ifdef _SCC_DEBUG
	if(needSupply > MAX_SUPPLY) { // TODO
		toLog("DEBUG: (START_CONDITION::getNeedSupply): Value needSupply out of range.");return(0);
	}
#endif
	return(needSupply);
}

inline void START_CONDITION::setNeedSupply(const unsigned int start_need_supply)
{
#ifdef _SCC_DEBUG
	if(start_need_supply > MAX_SUPPLY) {
		toLog("DEBUG: (START_CONDITION::getNeedSupply): Value start_need_supply out of range.");return;
	}
#endif
	needSupply = start_need_supply;
}

inline const unsigned int START_CONDITION::getGas() const
{
#ifdef _SCC_DEBUG
	if(gas > MAX_GAS) {
		toLog("DEBUG: (START_CONDITION::getGas): Variable gas out of range.");return(0);
	}
#endif
	return(gas);
}

inline const unsigned int START_CONDITION::getMinerals() const
{
#ifdef _SCC_DEBUG
	if(minerals > MAX_MINERALS)
	{
		toLog("DEBUG: (START_CONDITION::getMinerals): Variable minerals out of range.");
		return(0);
	}
#endif
	return(minerals);
}
	
inline void START_CONDITION::setMinerals(const unsigned int start_minerals)
{
#ifdef _SCC_DEBUG
	if(start_minerals > MAX_MINERALS) {
		toLog("DEBUG: (START_CONDITION::setMinerals): Value start_minerals out of range.");return;
	}
#endif
	minerals = start_minerals;
}

inline void START_CONDITION::setGas(const unsigned int start_gas)
{
#ifdef _SCC_DEBUG
	if(start_gas > MAX_GAS) {
		toLog("DEBUG: (START_CONDITION::setGas): Value start_gas out of range.");return;
	}
#endif
	gas = start_gas;	
}

inline const unsigned int START_CONDITION::getStartTime() const
{
#ifdef _SCC_DEBUG
	if(startTime >= MAX_TIME) { // TODO was wenn im Nachhinein settings.maxtime veraendert wird? 
		toLog("DEBUG: (START_CONDITION::getStartTime): Variable startTime out of range.");return(0);
	}
#endif
	return(startTime);
}

inline void START_CONDITION::setStartTime(const unsigned int start_time)
{
#ifdef _SCC_DEBUG
	if(start_time >= MAX_TIME) {
		toLog("DEBUG: (START_CONDITION::setStartTime): Value start_time out of range.");return;
	}
#endif
	startTime = start_time;
}

inline void START_CONDITION::setLocationAvailible(const unsigned int location, const unsigned int unit_type, const unsigned int unit_count)
{
#ifdef _SCC_DEBUG
	if((location == 0) || (location >= MAX_LOCATIONS)) {
		toLog("DEBUG: (START_CONDITION::setLocationAvailible): Value location out of range.");return;
	}
#endif
	unit[location].setAvailible(unit_type, unit_count);
	unit[GLOBAL].addAvailible(unit_type, unit_count);
	// TODO UNSAUBER!
}

inline const unsigned int START_CONDITION::getLocationAvailible(const unsigned int location, const unsigned int unit_type) const
{
#ifdef _SCC_DEBUG
	if(location >= MAX_LOCATIONS) {
		toLog("DEBUG: (START_CONDITION::getLocationAvailible): Value location out of range.");return(0);
	}
#endif
	return(unit[location].getAvailible(unit_type));
}

inline const unsigned int START_CONDITION::getLocationTotal(const unsigned int location, const unsigned int unit_type) const
{
#ifdef _SCC_DEBUG
	if(location >= MAX_LOCATIONS) { // TODO
		toLog("DEBUG: (START_CONDITION::getLocationTotal): Value location out of range.");return(0);
	}
#endif
	return(unit[location].getTotal(unit_type));
}


inline void START_CONDITION::setLocationTotal(const unsigned int location, const unsigned int unit_type, const unsigned int unit_count)
{
#ifdef _SCC_DEBUG
	if((location == 0)||(location >= MAX_LOCATIONS)) { // TODO
		toLog("DEBUG: (START_CONDITION::setLocationTotal): Value location out of range.");return;
	}
#endif
	unit[location].setTotal(unit_type, unit_count);
	unit[GLOBAL].addTotal(unit_type, unit_count);
	// TODO UNSAUBER!
}


inline void START_CONDITION::resetUnits()
{
	for(unsigned int i=MAX_LOCATIONS;i--;)
		unit[i].resetData();
}


#endif // _CORE_STARTCONDITION_HPP

