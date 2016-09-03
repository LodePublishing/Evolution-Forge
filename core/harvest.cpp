#include "harvest.hpp"

HARVEST_SPEED::HARVEST_SPEED()
{
	for(int i=45;i--;)
		setHarvestMineralSpeed(i,0);
	for(int i=5;i--;)
		setHarvestGasSpeed(i,0);
}

HARVEST_SPEED::~HARVEST_SPEED()
{ }


