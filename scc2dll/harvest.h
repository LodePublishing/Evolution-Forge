#ifndef __HARVEST_H
#define __HARVEST_H

#include "main.h"
class HARVEST_SPEED
{
	public:
		int minerals[45];
		int gas[5];
	
		int getHarvestMineralSpeed(int num);
		int getHarvestGasSpeed(int num);
		int setHarvestMineralSpeed(int num,int speed);
		int setHarvestGasSpeed(int num,int speed);

		HARVEST_SPEED();
		~HARVEST_SPEED();
};

#endif // __HARVEST_H

