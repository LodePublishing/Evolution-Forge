#ifndef _CORE_HARVEST_HPP
#define _CORE_HARVEST_HPP

#include "starcraft.hpp"
#include "../stl/misc.hpp"

class HARVEST_SPEED
{
	private:
		unsigned int minerals[45];
		unsigned int gas[5];
	public:
		HARVEST_SPEED();
		~HARVEST_SPEED();
	
		const unsigned int getHarvestMineralSpeed(const unsigned int num) const;
		const unsigned int getHarvestGasSpeed(const unsigned int num) const;
		void setHarvestMineralSpeed(const unsigned int num, const unsigned int speed);
		void setHarvestGasSpeed(const unsigned int num, const unsigned int speed);

};

inline const unsigned int HARVEST_SPEED::getHarvestMineralSpeed(const unsigned int num) const
{
#ifdef _SCC_DEBUG
	//todo range checking von minerals[num]!
	if(num>=45) {
		toErrorLog("DEBUG: (HARVEST_SPEED::getHarvestMineralSpeed): Value out of range.");return(0);
	}
#endif
	return(minerals[num]);
}

inline const unsigned int HARVEST_SPEED::getHarvestGasSpeed(const unsigned int num) const
{
#ifdef _SCC_DEBUG
	if(num>=5) {
		toErrorLog("DEBUG: (HARVEST_SPEED::getHarvestGasSpeed): Value out of range.");return(0);
	}
#endif
	return(gas[num]);
}

inline void HARVEST_SPEED::setHarvestMineralSpeed(const unsigned int num, const unsigned int speed)
{
#ifdef _SCC_DEBUG
	if(num>=45) {
		toErrorLog("DEBUG: (HARVEST_SPEED::setHarvestMineralSpeed): Value out of range.");return;
	}
#endif
	minerals[num]=speed;
}

inline void HARVEST_SPEED::setHarvestGasSpeed(const unsigned int num, const unsigned int speed)
{
#ifdef _SCC_DEBUG
	if(num>=5) {
		toErrorLog("DEBUG: (HARVEST_SPEED::getHarvestGasSpeed): Value out of range.");return;
	}
#endif
	gas[num]=speed;
}

#endif // _CORE_HARVEST_HPP

