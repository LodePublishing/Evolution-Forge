#ifndef _CORE_HARVEST_HPP
#define _CORE_HARVEST_HPP

#include "defs.hpp"

class HARVEST_SPEED
{
	private:
		int minerals[45];
		int gas[5];
	public:
		HARVEST_SPEED();
		~HARVEST_SPEED();
	
		const int getHarvestMineralSpeed(const int num) const;
		const int getHarvestGasSpeed(const int num) const;
		void setHarvestMineralSpeed(const int num, const int speed);
		void setHarvestGasSpeed(const int num, const int speed);

};

inline const int HARVEST_SPEED::getHarvestMineralSpeed(const int num) const
{
#ifdef _SCC_DEBUG
	//todo range checking von minerals[num]!
	if((num<0)||(num>=45)) {
		toLog("DEBUG: (HARVEST_SPEED::getHarvestMineralSpeed): Value out of range.");return(0);
	}
#endif
	return(minerals[num]);
}

inline const int HARVEST_SPEED::getHarvestGasSpeed(const int num) const
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>=5)) {
		toLog("DEBUG: (HARVEST_SPEED::getHarvestGasSpeed): Value out of range.");return(0);
	}
#endif
	return(gas[num]);
}

inline void HARVEST_SPEED::setHarvestMineralSpeed(const int num, const int speed)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>=45)||(speed<0)) {
		toLog("DEBUG: (HARVEST_SPEED::setHarvestMineralSpeed): Value out of range.");return;
	}
#endif
	minerals[num]=speed;
}

inline void HARVEST_SPEED::setHarvestGasSpeed(const int num, const int speed)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>=5)||(speed<0)) {
		toLog("DEBUG: (HARVEST_SPEED::getHarvestGasSpeed): Value out of range.");return;
	}
#endif
	gas[num]=speed;
}

#endif // _CORE_HARVEST_HPP

