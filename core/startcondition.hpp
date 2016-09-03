#ifndef _CORE_STARTCONDITION_HPP
#define _CORE_STARTCONDITION_HPP

#include "unit.hpp"

class START_CONDITION
{
	public:
		START_CONDITION();
		~START_CONDITION();
	
		void resetUnits();
		void adjustResearches();		// fill the remaining items with either 0, 1 or 3, depending on NULL, research, upgrade
		void adjustSupply();
		
		void assignRace(const unsigned int start_race);
		const unsigned int getRace() const;

		const std::string& getName() const;
		void setName(const std::string& startcondition_name);

		const unsigned int getHaveSupply() const;
		void setHaveSupply(const unsigned int start_have_supply);
		
		const unsigned int getNeedSupply() const;
		void setNeedSupply(const unsigned int start_need_supply);

		const unsigned int getResource(const unsigned int resource_type) const;
		void setResource(const unsigned int resource_type, const unsigned int start_resource);

		const unsigned int getStartTime() const;
		void setStartTime(const unsigned int start_time);
		
		void setLocationTotal(const unsigned int loc, const unsigned int unit_type, const unsigned int unit_count);
		void setLocationAvailible(const unsigned int loc, const unsigned int unit_type, const unsigned int unit_count);
		
		const unsigned int getLocationTotal(const unsigned int loc, const unsigned int unit_type) const;
		const unsigned int getLocationAvailible(const unsigned int loc, const unsigned int unit_type) const;

		const UNIT& getUnit(const unsigned int location) const;
	
	private:
		UNIT unit[MAX_LOCATIONS];
		unsigned int race;
		std::string name;
		
		unsigned int haveSupply;
		unsigned int needSupply;
		std::vector<unsigned int> resource;
		unsigned int startTime;
		START_CONDITION(const START_CONDITION& object);
		START_CONDITION& operator=(const START_CONDITION& object);
};

inline void START_CONDITION::assignRace(const unsigned int start_race) {
#ifdef _SCC_DEBUG
	if(start_race >= GAME::MAX_RACES) {
		toErrorLog("DEBUG (START_CONDITION::assignRace()): start_race out of range.");
	}
#endif
	race = start_race;
}
			
inline const unsigned int START_CONDITION::getRace() const {
#ifdef _SCC_DEBUG
	if(race >= GAME::MAX_RACES) {
		toErrorLog("DEBUG (START_CONDITION::getRace()): race not initialized.");
	}
#endif
	return(race);
}

inline const UNIT& START_CONDITION::getUnit(const unsigned int location) const 
{
#ifdef _SCC_DEBUG
	if(location >= MAX_LOCATIONS) { // TODO
		toErrorLog("DEBUG: (START_CONDITION::getUnit): Value location out of range.");return(unit[0]);
	} 
#endif
	return(unit[location]);
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
	if(haveSupply > 2*GAME::MAX_SUPPLY) {
		toErrorLog("DEBUG: (START_CONDITION::getHaveSupply): Value haveSupply out of range.");return(0);
	}
#endif
	return(haveSupply);
}

inline void START_CONDITION::setHaveSupply(const unsigned int start_have_supply) 
{
#ifdef _SCC_DEBUG
	if(start_have_supply > 2*GAME::MAX_SUPPLY) {
		toErrorLog("DEBUG: (START_CONDITION::setHaveSupply): Value start_have_supply out of range.");return;
	}
#endif
	haveSupply = start_have_supply;
}
		
inline const unsigned int START_CONDITION::getNeedSupply() const
{
#ifdef _SCC_DEBUG
	if(needSupply > 2*GAME::MAX_SUPPLY) { // TODO
		toErrorLog("DEBUG: (START_CONDITION::getNeedSupply): Value needSupply out of range.");return(0);
	}
#endif
	return(needSupply);
}

inline void START_CONDITION::setNeedSupply(const unsigned int start_need_supply)
{
#ifdef _SCC_DEBUG
	if(start_need_supply > 2*GAME::MAX_SUPPLY) {
		toErrorLog("DEBUG: (START_CONDITION::getNeedSupply): Value start_need_supply out of range.");return;
	}
#endif
	needSupply = start_need_supply;
}

inline const unsigned int START_CONDITION::getResource(const unsigned int resource_type) const
{
#ifdef _SCC_DEBUG
	if(resource_type >= RACE::MAX_RESOURCE_TYPES) {
		toErrorLog("DEBUG (START_CONDITION::getResource): Value resource_type out of range.");return(0);
	}
	if(resource[resource_type] > GAME::MAX_RESOURCES) {
		toErrorLog("DEBUG (START_CONDITION::getResource): Variable resource not initialized.");return(0);
	}
#endif
	return(resource[resource_type]);
}
	
inline void START_CONDITION::setResource(const unsigned int resource_type, const unsigned int start_resource)
{
#ifdef _SCC_DEBUG
	if(resource_type >= RACE::MAX_RESOURCE_TYPES) {
		toErrorLog("DEBUG (START_CONDITION::setResource): Value resource_type out of range.");return;
	}
	if(start_resource > GAME::MAX_RESOURCES) {
		toErrorLog("DEBUG: (START_CONDITION::setResource): Value start_resource out of range.");return;
	}
#endif
	resource[resource_type] = start_resource;
}

inline const unsigned int START_CONDITION::getStartTime() const
{
#ifdef _SCC_DEBUG
	if(startTime >= MAX_TIME) { // TODO was wenn im Nachhinein settings.maxtime veraendert wird? 
		toErrorLog("DEBUG: (START_CONDITION::getStartTime): Variable startTime out of range.");return(0);
	}
#endif
	return(startTime);
}

inline void START_CONDITION::setStartTime(const unsigned int start_time)
{
#ifdef _SCC_DEBUG
	if(start_time >= MAX_TIME) {
		toErrorLog("DEBUG: (START_CONDITION::setStartTime): Value start_time out of range.");return;
	}
#endif
	startTime = start_time;
}

inline void START_CONDITION::setLocationAvailible(const unsigned int location, const unsigned int unit_type, const unsigned int unit_count)
{
#ifdef _SCC_DEBUG
	if((location == GLOBAL) || (location >= MAX_LOCATIONS)) {
		toErrorLog("DEBUG: (START_CONDITION::setLocationAvailible): Value location out of range.");return;
	}
#endif
	unit[location].setAvailible(unit_type, unit_count);
	unit[GLOBAL].modifyAvailible(unit_type, unit_count);
	// TODO UNSAUBER!
}

inline const unsigned int START_CONDITION::getLocationAvailible(const unsigned int location, const unsigned int unit_type) const
{
#ifdef _SCC_DEBUG
	if(location >= MAX_LOCATIONS) {
		toErrorLog("DEBUG: (START_CONDITION::getLocationAvailible): Value location out of range.");return(0);
	}
#endif
	return(unit[location].getAvailible(unit_type));
}

inline const unsigned int START_CONDITION::getLocationTotal(const unsigned int location, const unsigned int unit_type) const
{
#ifdef _SCC_DEBUG
	if(location >= MAX_LOCATIONS) { // TODO
		toErrorLog("DEBUG: (START_CONDITION::getLocationTotal): Value location out of range.");return(0);
	}
#endif
	return(unit[location].getTotal(unit_type));
}


inline void START_CONDITION::setLocationTotal(const unsigned int location, const unsigned int unit_type, const unsigned int unit_count)
{
#ifdef _SCC_DEBUG
	if((location == GLOBAL)||(location >= MAX_LOCATIONS)) { // TODO
		toErrorLog("DEBUG: (START_CONDITION::setLocationTotal): Value location out of range.");return;
	}
#endif
	unit[location].setTotal(unit_type, unit_count);
	unit[GLOBAL].modifyTotal(unit_type, unit_count);
	// TODO UNSAUBER!
}


inline void START_CONDITION::resetUnits()
{
	for(unsigned int i=MAX_LOCATIONS;i--;)
		unit[i].resetData();
}


#endif // _CORE_STARTCONDITION_HPP

