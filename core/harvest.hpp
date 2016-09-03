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

#endif // _CORE_HARVEST_HPP

