#include "harvest.hpp"

HARVEST_SPEED::HARVEST_SPEED()
{
	memset(minerals, 0, 45 * sizeof(int));
	memset(gas, 0, 5 * sizeof(int));
}

HARVEST_SPEED::~HARVEST_SPEED()
{ }


