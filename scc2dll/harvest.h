#ifndef __HARVEST_H
#define __HARVEST_H

#include "main.h"
class HARVEST_SPEED
{
	public:
		int minerals[45];
		int gas[5];
	
		const int getHarvestMineralSpeed(const int num) const;
		const int getHarvestGasSpeed(const int num) const;
		void setHarvestMineralSpeed(const int num, const int speed);
		void setHarvestGasSpeed(const int num, const int speed);

		HARVEST_SPEED();
		~HARVEST_SPEED();
};

#endif // __HARVEST_H

