#include "harvest.hpp"

HARVEST_SPEED::HARVEST_SPEED()
{
	for(unsigned int i=45;i--;)
		setHarvestMineralSpeed(i,0);
	for(unsigned int i=5;i--;)
		setHarvestGasSpeed(i,0);
}

HARVEST_SPEED::~HARVEST_SPEED()
{ }


