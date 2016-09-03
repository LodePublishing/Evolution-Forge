#include "harvest.h"
#include "debug.h"

int HARVEST_SPEED::getHarvestMineralSpeed(int num)
{
	//todo range checking von minerals[num]!
	if((num<0)||(num>=45))
	{
		debug.toLog(0,"DEBUG: (HARVEST_SPEED::getHarvestMineralSpeed): Value [%i] out of range.",num);
		return(0);
	}
	return(minerals[num]);
};

int HARVEST_SPEED::getHarvestGasSpeed(int num)
{
	if((num<0)||(num>=5))
	{
		debug.toLog(0,"DEBUG: (HARVEST_SPEED::getHarvestGasSpeed): Value [%i] out of range.",num);
		return(0);
	}
	return(gas[num]);
};
int HARVEST_SPEED::setHarvestMineralSpeed(int num,int speed)
{
	if((num<0)||(num>=45)||(speed<0))
	{
		debug.toLog(0,"DEBUG: (HARVEST_SPEED::setHarvestMineralSpeed): Value [%i/%i] out of range.",num,speed);
		return(0);
	}
	minerals[num]=speed;
	return(1);
};
int HARVEST_SPEED::setHarvestGasSpeed(int num,int speed)
{
	if((num<0)||(num>=5)||(speed<0))
	{
		debug.toLog(0,"DEBUG: (HARVEST_SPEED::getHarvestGasSpeed): Value [%i/%i] out of range.",num,speed);
		return(0);
	}
	gas[num]=speed;
	return(1);
};

HARVEST_SPEED::HARVEST_SPEED()
{
	int i;
	for(i=0;i<45;i++)
		setHarvestMineralSpeed(i,0);
	for(i=0;i<5;i++)
		setHarvestGasSpeed(i,0);
};

HARVEST_SPEED::~HARVEST_SPEED()
{
};

