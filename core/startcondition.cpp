#include "startcondition.hpp"

START_CONDITION::START_CONDITION():
	race(TERRA),
	name("ERROR"),
	haveSupply(0),
	needSupply(0),
	minerals(0),
	gas(0),
	startTime(0)
{
}

START_CONDITION::~START_CONDITION()
{
}

void START_CONDITION::adjustSupply()
{
	unsigned int needSupply=0;
	unsigned int haveSupply=0;

	for(int i=1;i<MAX_LOCATIONS;i++)
		unit[i].adjustSupply(race, needSupply, haveSupply);
	
	setNeedSupply(needSupply);
	setHaveSupply(haveSupply);
}

void START_CONDITION::adjustResearches()
{
//	for(int i=1;i<MAX_LOCATIONS;i++)
		unit[0].adjustResearches(race);
}

void START_CONDITION::assignRace(const eRace race)
{
	this->race=race;
}
			
const eRace START_CONDITION::getRace() const
{
	return(race);
}

const UNIT* START_CONDITION::getUnit(const unsigned int location) const 
{
#ifdef _SCC_DEBUG
	if(location>=MAX_LOCATIONS) {
		toLog("DEBUG: (START_CONDITION::getUnit): Value location out of range.");return(0);
	} 
#endif
	return(&unit[location]);
}

const std::string& START_CONDITION::getName() const
{
	return(name);
}

void START_CONDITION::setName(const std::string& name)
{
	this->name=name;
}

const unsigned int START_CONDITION::getHaveSupply() const
{
#ifdef _SCC_DEBUG
	if(haveSupply>MAX_SUPPLY) {
		toLog("DEBUG: (START_CONDITION::getHaveSupply): Value haveSupply out of range.");return(0);
	}
#endif
	return(haveSupply);
}

void START_CONDITION::setHaveSupply(const unsigned int haveSupply)
{
#ifdef _SCC_DEBUG
	if(haveSupply>MAX_SUPPLY) {
		toLog("DEBUG: (START_CONDITION::setHaveSupply): Value haveSupply out of range.");return;
	}
#endif
	this->haveSupply=haveSupply;
}
		
const unsigned int START_CONDITION::getNeedSupply() const
{
#ifdef _SCC_DEBUG
	if(needSupply>MAX_SUPPLY) { // TODO
		toLog("DEBUG: (START_CONDITION::getNeedSupply): Value needSupply out of range.");return(0);
	}
#endif
	return(needSupply);
}

void START_CONDITION::setNeedSupply(const unsigned int needSupply)
{
#ifdef _SCC_DEBUG
	if(needSupply>MAX_SUPPLY) {
		toLog("DEBUG: (START_CONDITION::getNeedSupply): Value needSupply out of range.");return;
	}
#endif
	this->needSupply=needSupply;
}

const unsigned int START_CONDITION::getGas() const
{
#ifdef _SCC_DEBUG
	if(gas>MAX_GAS) {
		toLog("DEBUG: (START_CONDITION::getGas): Variable gas out of range.");return(0);
	}
#endif
	return(gas);
}

const unsigned int START_CONDITION::getMinerals() const
{
#ifdef _SCC_DEBUG
	if(minerals>MAX_MINERALS)
	{
		toLog("DEBUG: (START_CONDITION::getMinerals): Variable minerals out of range.");
		return(0);
	}
#endif
	return(minerals);
}
	
void START_CONDITION::setMinerals(const unsigned int minerals)
{
#ifdef _SCC_DEBUG
	if(minerals>MAX_MINERALS) {
		toLog("DEBUG: (START_CONDITION::setMinerals): Value minerals out of range.");return;
	}
#endif
	this->minerals=minerals;
}

void START_CONDITION::setGas(const unsigned int gas)
{
#ifdef _SCC_DEBUG
	if(gas>MAX_GAS) {
		toLog("DEBUG: (START_CONDITION::setGas): Value gas out of range.");return;
	}
#endif
	this->gas=gas;	
}

const unsigned int START_CONDITION::getStartTime() const
{
#ifdef _SCC_DEBUG
	if(startTime>=MAX_TIME) {
		toLog("DEBUG: (START_CONDITION::getStartTime): Variable startTime out of range.");return(0);
	}
#endif
	return(startTime);
}

void START_CONDITION::setStartTime(const unsigned int startTime)
{
#ifdef _SCC_DEBUG
	if(startTime>=MAX_TIME) {
		toLog("DEBUG: (START_CONDITION::setStartTime): Value startTime out of range.");return;
	}
#endif
	this->startTime=startTime;
}

void START_CONDITION::setLocationAvailible(const unsigned int location, const unsigned int unit, const unsigned int num)
{
#ifdef _SCC_DEBUG
	if((location==0)||(location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (START_CONDITION::setLocationAvailible): Value location out of range.");return;
	}
#endif
	this->unit[location].setAvailible(unit, num);
	this->unit[GLOBAL].addAvailible(unit, num);
	// TODO UNSAUBER!
}

const unsigned int START_CONDITION::getLocationAvailible(const unsigned int location, const unsigned int unit) const
{
#ifdef _SCC_DEBUG
	if(location>=MAX_LOCATIONS) {
		toLog("DEBUG: (START_CONDITION::getLocationAvailible): Value location out of range.");return(0);
	}
#endif
	return(this->unit[location].getAvailible(unit));
}

const unsigned int START_CONDITION::getLocationTotal(const unsigned int location, const unsigned int unit) const
{
#ifdef _SCC_DEBUG
	if(location>=MAX_LOCATIONS) {
		toLog("DEBUG: (START_CONDITION::getLocationTotal): Value location out of range.");return(0);
	}
#endif
	return(this->unit[location].getTotal(unit));
}


void START_CONDITION::setLocationTotal(const unsigned int location, const unsigned int unit, const unsigned int num)
{
#ifdef _SCC_DEBUG
	if((location==0)||(location>=MAX_LOCATIONS)) {
		toLog("DEBUG: (START_CONDITION::setLocationTotal): Value location out of range.");return;
	}
#endif
	this->unit[location].setTotal(unit, num);
	this->unit[GLOBAL].addTotal(unit, num);
	// TODO UNSAUBER!
}


void START_CONDITION::resetUnits()
{
	for(int i=MAX_LOCATIONS;i--;)
		unit[i].resetData();
}

