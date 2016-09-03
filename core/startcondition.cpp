#include "startcondition.hpp"

START_CONDITION::START_CONDITION():
	race(0),
//am Namen liegts... seltsam... checken... ! o_O ?
	name("ERROR"),
	haveSupply(0),
	needSupply(0),
	resource(RACE::MAX_RESOURCE_TYPES),
	startTime(0)
{ }

START_CONDITION::~START_CONDITION()
{ }

START_CONDITION::START_CONDITION(const START_CONDITION& object) :
	race(object.race),
	name(object.name),
	haveSupply(object.haveSupply),
	needSupply(object.needSupply),
	startTime(object.startTime)
{
	for(unsigned int i = MAX_LOCATIONS; i--;)
		unit[i] = object.unit[i];
	resource = object.resource;
}

START_CONDITION& START_CONDITION::operator=(const START_CONDITION& object)
{
	race = object.race;
	name = object.name;
	haveSupply = object.haveSupply;
	needSupply = object.needSupply;
	resource = object.resource;
	startTime = object.startTime;
	for(unsigned int i = MAX_LOCATIONS; i--;)
		unit[i] = object.unit[i];
	return(*this);
}

void START_CONDITION::adjustSupply()
{
	unsigned int need_supply = 0;
	unsigned int have_supply = 0;

	for(unsigned int i=1; i<MAX_LOCATIONS; ++i)
		unit[i].adjustSupply(race, need_supply, have_supply);
	
	setNeedSupply(need_supply);
	setHaveSupply(have_supply);
}

void START_CONDITION::adjustResearches()
{
// set researches on all locations because when a research is completed it is removed everywhere!
	for(unsigned int i=0; i<MAX_LOCATIONS; ++i)
		unit[i].adjustResearches(race);
}


