#include "statistics.hpp"
#include "configuration.hpp"

STATISTICS::STATISTICS():
	needSupply(0),
	haveSupply(0),
	fitness(0),
	time(coreConfiguration.getMaxTime())
{ 
	resource.assign(RACE::MAX_RESOURCE_TYPES, 0);
}

STATISTICS::~STATISTICS() {}

void STATISTICS::resetData()
{
	needSupply = 0;
	haveSupply = 0;
	resource.assign(RACE::MAX_RESOURCE_TYPES, 0);
	fitness = 0;
	time = coreConfiguration.getMaxTime();
}

STATISTICS& STATISTICS::operator=(const STATISTICS& object)
{
	needSupply = object.needSupply;
	haveSupply = object.haveSupply;
	resource = object.resource;
	fitness = object.fitness;
	time = object.time;
	return(*this);
}


STATISTICS::STATISTICS(const STATISTICS& object) :
	needSupply(object.needSupply),
	haveSupply(object.haveSupply),
	resource(object.resource),
	fitness(object.fitness),
	time(object.time)
{ }


