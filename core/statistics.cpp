#include "statistics.hpp"

STATISTICS::STATISTICS():
	needSupply(0),
	haveSupply(0),
	minerals(0),
	gas(0),
	fitness(0)
{ }

STATISTICS::~STATISTICS() {}

void STATISTICS::resetData()
{
	needSupply = 0;
	haveSupply = 0;
	minerals = 0;
	gas = 0;
	fitness = 0;
}

STATISTICS& STATISTICS::operator=(const STATISTICS& object)
{
	needSupply = object.needSupply;
	haveSupply = object.haveSupply;
	minerals = object.minerals;
	gas = object.gas;
	fitness = object.fitness;
	return(*this);
}


STATISTICS::STATISTICS(const STATISTICS& object) :
	needSupply(object.needSupply),
	haveSupply(object.haveSupply),
	minerals(object.minerals),
	gas(object.gas),
	fitness(object.fitness)
{ }


