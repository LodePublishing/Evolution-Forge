#include "startcondition.h"
#include "debug.h"
// this needs still some work to make everything private

START_CONDITION::START_CONDITION()
{
};

START_CONDITION::~START_CONDITION()
{
};

void START_CONDITION::adjustSupply()
{
	int needSupply=0;
	int haveSupply=0;

	for(int i=1;i<MAX_LOCATIONS;i++)
    	unit[i].adjustSupply(race, needSupply, haveSupply);

	setNeedSupply(needSupply);
	setHaveSupply(haveSupply);
};

void START_CONDITION::adjustResearches()
{
	for(int i=1;i<MAX_LOCATIONS;i++)
		unit[i].adjustResearches(race);
};

void START_CONDITION::setRace(const eRace race)
{
	this->race=race;
};

const eRace START_CONDITION::getRace()
{
	return(race);
};


const UNIT* START_CONDITION::getUnit(const int location) const 
{
#ifdef _SCC_DEBUG
    if((location<0)||(location>=MAX_LOCATIONS))
    {
        toLog("DEBUG: (START_CONDITION::getUnit): Value location out of range.");
        return(0);
    } 
#endif
    return(&unit[location]);
};

const string& START_CONDITION::getName() const
{
	return(name);
};

void START_CONDITION::setName(const string& name)
{
	this->name=name;
};

const int START_CONDITION::getHaveSupply() const
{
	return(haveSupply);
};

void START_CONDITION::setHaveSupply(const int haveSupply)
{
	this->haveSupply=haveSupply;
};
		
const int START_CONDITION::getNeedSupply() const
{
	return(needSupply);
};

void START_CONDITION::setNeedSupply(const int needSupply)
{
	this->needSupply=needSupply;
};

const int START_CONDITION::getGas() const
{
	return(gas);
};

const int START_CONDITION::getMinerals() const
{
	return(minerals);
};
	
void START_CONDITION::setMinerals(const int minerals)
{
	this->minerals=minerals;
};

void START_CONDITION::setGas(const int gas)
{
	this->gas=gas;	
};

const int START_CONDITION::getStartTime() const
{
	return(startTime);
};

void START_CONDITION::setStartTime(const int startTime)
{
	this->startTime=startTime;
};

void START_CONDITION::setLocationAvailible(const int location, const int unit, const int num)
{
#ifdef _SCC_DEBUG
    if((location<0)||(location>=MAX_LOCATIONS))
    {
        toLog("DEBUG: (START_CONDITION::setLocationAvailible): Value location out of range.");
        return;
    }
#endif
    this->unit[location].setAvailible(unit, num);
};
                                                                                                                                                            
const int START_CONDITION::getLocationAvailible(const int location, const int unit) const
{
#ifdef _SCC_DEBUG
    if((location<0)||(location>=MAX_LOCATIONS))
    {
        toLog("DEBUG: (START_CONDITION::getLocationAvailible): Value location out of range.");
        return(0);
    }
#endif
    return(this->unit[location].getAvailible(unit));
};

const int START_CONDITION::getLocationTotal(const int location, const int unit) const
{
#ifdef _SCC_DEBUG
    if((location<0)||(location>=MAX_LOCATIONS))
    {
        toLog("DEBUG: (START_CONDITION::getLocationTotal): Value location out of range.");
        return(0);
    }
#endif
    return(this->unit[location].getTotal(unit));
};


void START_CONDITION::setLocationTotal(const int location, const int unit, const int num)
{
#ifdef _SCC_DEBUG
    if((location<0)||(location>=MAX_PLAYER))
    {
        toLog("DEBUG: (START_CONDITION::setLocationTotal): Value location out of range.");
        return;
    }
#endif
    this->unit[location].setTotal(unit, num);
};


void START_CONDITION::resetUnits()
{
	for(int i=MAX_LOCATIONS;i--;)
		for(int j=UNIT_TYPE_COUNT;j--;)
		{
			unit[i].setTotal(j, 0);
			unit[i].setAvailible(j, 0);
		}
};

/*
void START_CONDITION::copy(int player, UNIT* unit)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=MAX_PLAYER))
	{
		toLog("DEBUG: (START_CONDITION::copy): Value player out of range.");
		return;
	}
#endif
	this->unit[player].copy(unit);
};

void START_CONDITION::copyBasic(START_CONDITION* location)
{
	setName(location->getName());
	setMineralDistance(location->getMineralDistance());
	for(int i=MAX_LOCATIONS;i--;)
		setDistance(i,location->getDistance(i));
};


void START_CONDITION::copy(START_CONDITION* location)
{
	for(int i=MAX_PLAYER;i--;)
		unit[i].copy(location->getUnits(i));
	setName(location->getName());
	setMineralDistance(location->getMineralDistance());
	for(int i=MAX_LOCATIONS;i--;)
		setDistance(i,location->getDistance(i));
};
*/

//		void copy(START_CONDITION* sforce);


