#include "startcondition.hpp"

START_CONDITION::START_CONDITION():
	race(TERRA),
//am Namen liegts... seltsam... checken... ! o_O
	name("ERROR"),
	haveSupply(0),
	needSupply(0),
	minerals(0),
	gas(0),
	startTime(0)
{ }

START_CONDITION::~START_CONDITION()
{ }

START_CONDITION::START_CONDITION(const START_CONDITION& object) :
    race(object.race),
    name(object.name),
    haveSupply(object.haveSupply),
    needSupply(object.needSupply),
    minerals(object.minerals),
    gas(object.gas),
    startTime(object.startTime)
{
	for(int i = MAX_LOCATIONS;i--;)
		unit[i] = object.unit[i];
}

START_CONDITION& START_CONDITION::operator=(const START_CONDITION& object)
{
    race = object.race;
    name = object.name;
    haveSupply = object.haveSupply;
    needSupply = object.needSupply;
    minerals = object.minerals;
    gas = object.gas;
    startTime = object.startTime;
	for(int i = MAX_LOCATIONS;i--;)
		unit[i] = object.unit[i];
	return(*this);
}

void START_CONDITION::adjustSupply()
{
	unsigned int need_supply=0;
	unsigned int have_supply=0;

	for(int i=1;i<MAX_LOCATIONS;i++)
		unit[i].adjustSupply(race, need_supply, have_supply);
	
	setNeedSupply(need_supply);
	setHaveSupply(have_supply);
}

void START_CONDITION::adjustResearches()
{
//	for(int i=1;i<MAX_LOCATIONS;i++)
		unit[0].adjustResearches(race);
}

void START_CONDITION::assignRace(const eRace start_race)
{
	race = start_race;
}
			
const eRace START_CONDITION::getRace() const
{
	return(race);
}

const UNIT* START_CONDITION::getUnit(const unsigned int location) const 
{
#ifdef _SCC_DEBUG
	if(location >= MAX_LOCATIONS) {
		toLog("DEBUG: (START_CONDITION::getUnit): Value location out of range.");return(0);
	} 
#endif
	return(&unit[location]);
}

const std::string& START_CONDITION::getName() const
{
	return(name);
}

void START_CONDITION::setName(const std::string& startcondition_name)
{
	name = startcondition_name;
}

const unsigned int START_CONDITION::getHaveSupply() const
{
#ifdef _SCC_DEBUG
	if(haveSupply > MAX_SUPPLY) {
		toLog("DEBUG: (START_CONDITION::getHaveSupply): Value haveSupply out of range.");return(0);
	}
#endif
	return(haveSupply);
}

void START_CONDITION::setHaveSupply(const unsigned int start_have_supply)
{
#ifdef _SCC_DEBUG
	if(start_have_supply > MAX_SUPPLY) {
		toLog("DEBUG: (START_CONDITION::setHaveSupply): Value start_have_supply out of range.");return;
	}
#endif
	haveSupply = start_have_supply;
}
		
const unsigned int START_CONDITION::getNeedSupply() const
{
#ifdef _SCC_DEBUG
	if(needSupply > MAX_SUPPLY) { // TODO
		toLog("DEBUG: (START_CONDITION::getNeedSupply): Value needSupply out of range.");return(0);
	}
#endif
	return(needSupply);
}

void START_CONDITION::setNeedSupply(const unsigned int start_need_supply)
{
#ifdef _SCC_DEBUG
	if(start_need_supply > MAX_SUPPLY) {
		toLog("DEBUG: (START_CONDITION::getNeedSupply): Value start_need_supply out of range.");return;
	}
#endif
	needSupply = start_need_supply;
}

const unsigned int START_CONDITION::getGas() const
{
#ifdef _SCC_DEBUG
	if(gas > MAX_GAS) {
		toLog("DEBUG: (START_CONDITION::getGas): Variable gas out of range.");return(0);
	}
#endif
	return(gas);
}

const unsigned int START_CONDITION::getMinerals() const
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
	
void START_CONDITION::setMinerals(const unsigned int start_minerals)
{
#ifdef _SCC_DEBUG
	if(start_minerals > MAX_MINERALS) {
		toLog("DEBUG: (START_CONDITION::setMinerals): Value start_minerals out of range.");return;
	}
#endif
	minerals = start_minerals;
}

void START_CONDITION::setGas(const unsigned int start_gas)
{
#ifdef _SCC_DEBUG
	if(start_gas > MAX_GAS) {
		toLog("DEBUG: (START_CONDITION::setGas): Value start_gas out of range.");return;
	}
#endif
	gas = start_gas;	
}

const unsigned int START_CONDITION::getStartTime() const
{
#ifdef _SCC_DEBUG
	if(startTime >= MAX_TIME) { // TODO was wenn im Nachhinein settings.maxtime veraendert wird? 
		toLog("DEBUG: (START_CONDITION::getStartTime): Variable startTime out of range.");return(0);
	}
#endif
	return(startTime);
}

void START_CONDITION::setStartTime(const unsigned int start_time)
{
#ifdef _SCC_DEBUG
	if(start_time >= MAX_TIME) {
		toLog("DEBUG: (START_CONDITION::setStartTime): Value start_time out of range.");return;
	}
#endif
	startTime = start_time;
}

void START_CONDITION::setLocationAvailible(const unsigned int location, const unsigned int unit_type, const unsigned int unit_count)
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

const unsigned int START_CONDITION::getLocationAvailible(const unsigned int location, const unsigned int unit_type) const
{
#ifdef _SCC_DEBUG
	if(location >= MAX_LOCATIONS) {
		toLog("DEBUG: (START_CONDITION::getLocationAvailible): Value location out of range.");return(0);
	}
#endif
	return(unit[location].getAvailible(unit_type));
}

const unsigned int START_CONDITION::getLocationTotal(const unsigned int location, const unsigned int unit_type) const
{
#ifdef _SCC_DEBUG
	if(location >= MAX_LOCATIONS) {
		toLog("DEBUG: (START_CONDITION::getLocationTotal): Value location out of range.");return(0);
	}
#endif
	return(unit[location].getTotal(unit_type));
}


void START_CONDITION::setLocationTotal(const unsigned int location, const unsigned int unit_type, const unsigned int unit_count)
{
#ifdef _SCC_DEBUG
	if((location == 0)||(location >= MAX_LOCATIONS)) {
		toLog("DEBUG: (START_CONDITION::setLocationTotal): Value location out of range.");return;
	}
#endif
	unit[location].setTotal(unit_type, unit_count);
	unit[GLOBAL].addTotal(unit_type, unit_count);
	// TODO UNSAUBER!
}


void START_CONDITION::resetUnits()
{
	for(int i=MAX_LOCATIONS;i--;)
		unit[i].resetData();
}

