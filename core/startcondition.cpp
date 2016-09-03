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
	for(int i = MAX_LOCATIONS; i--;)
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
	for(int i = MAX_LOCATIONS; i--;)
		unit[i] = object.unit[i];
	return(*this);
}

void START_CONDITION::adjustSupply()
{
	unsigned int need_supply = 0;
	unsigned int have_supply = 0;

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


