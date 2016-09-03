#include "harvest.hpp"

HARVEST_SPEED::HARVEST_SPEED()
{
	for(int i=45;i--;)
		setHarvestMineralSpeed(i,0);
	for(int i=5;i--;)
		setHarvestGasSpeed(i,0);
}

HARVEST_SPEED::~HARVEST_SPEED()
{
}

const int HARVEST_SPEED::getHarvestMineralSpeed(const int num) const
{
#ifdef _SCC_DEBUG
	//todo range checking von minerals[num]!
	if((num<0)||(num>=45)) {
		toLog("DEBUG: (HARVEST_SPEED::getHarvestMineralSpeed): Value out of range.");return(0);
	}
#endif
	return(minerals[num]);
}

const int HARVEST_SPEED::getHarvestGasSpeed(const int num) const
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>=5)) {
		toLog("DEBUG: (HARVEST_SPEED::getHarvestGasSpeed): Value out of range.");return(0);
	}
#endif
	return(gas[num]);
}

void HARVEST_SPEED::setHarvestMineralSpeed(const int num, const int speed)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>=45)||(speed<0)) {
		toLog("DEBUG: (HARVEST_SPEED::setHarvestMineralSpeed): Value out of range.");return;
	}
#endif
	minerals[num]=speed;
}

void HARVEST_SPEED::setHarvestGasSpeed(const int num, const int speed)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>=5)||(speed<0)) {
		toLog("DEBUG: (HARVEST_SPEED::getHarvestGasSpeed): Value out of range.");return;
	}
#endif
	gas[num]=speed;
}

