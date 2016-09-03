#include "map.h"
#include "string.h"
#include "debug.h"
#include "location.h"


const char* MAP::getName()
{
	return(name);
};

int MAP::getMaxLocations()
{
	if((maxLocations<0)||(maxLocations>MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (MAP::getMaxLocations): Variable not initialized [%i].",maxLocations);
		return(0);
	}
	return(maxLocations);
};

int MAP::getMaxPlayer()
{
	if((maxPlayer<0)||(maxPlayer>MAX_PLAYER))
	{
		debug.toLog(0,"DEBUG: (MAP::getMaxPlayer): Variable not initialized [%i].",maxPlayer);
		return(0);
	}
	return(maxPlayer);
};

int MAP::setName(const char* line)
{
	if(!line)
	{
		debug.toLog(0,"DEBUG: (MAP::setName): Variable line not initialized [%i].",line);
		return(0);
	}
	strcpy(name,line);
	return(1);
};

int MAP::setMaxLocations(int num)
{
	if((num<=0)||(num>MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (MAP::setMaxLocations): Value [%i] out of range.",num);
		return(0);
	}
	maxLocations=num;
	return(1);
};

int MAP::setMaxPlayer(int num)
{
	if((num<1)||(num>MAX_PLAYER))
	{
		debug.toLog(0,"DEBUG: (MAP::setMaxPlayer): Value [%i] out of range.",num);
		return(0);
	}
	maxPlayer=num;
	return(1);
};

int MAP::adjustSupply()
{
	//initialized?
	for(int k=0;k<MAX_PLAYER;k++)
	{
		player[k].setSupply(0);
		player[k].setMaxSupply(0);
		for(int i=0;i<MAX_LOCATIONS;i++)
			for(int j=UNIT_TYPE_COUNT;j--;)
			{
				if(i>0)
				{
					if(stats[player[k].getRace()][j].supply<0)
					{
						if(player[k].getMaxSupply()-stats[player[k].getRace()][j].supply*location[i].force[k][j]>MAX_SUPPLY)
						{
							if(player[k].getMaxSupply()<MAX_SUPPLY)
							{
								player[k].setSupply(player[k].getSupply()+(MAX_SUPPLY-player[k].getMaxSupply()));
								player[k].setMaxSupply(MAX_SUPPLY);
							}
						}
						else
						{
							player[k].setSupply(player[k].getSupply()-stats[player[k].getRace()][j].supply*location[i].force[k][j]);
							player[k].setMaxSupply(player[k].getMaxSupply()-stats[player[k].getRace()][j].supply*location[i].force[k][j]);
						}
					} else
					player[k].setSupply(player[k].getSupply()-stats[player[k].getRace()][j].supply*location[i].force[k][j]);
				}
			};
	}
	return(1);
};

int MAP::adjustDistanceList()
{
	//initialized?
	for(int i=1;i<MAX_LOCATIONS;i++)
	{
		for(int counter=1;counter<MAX_LOCATIONS;counter++)
		{
			int min=200;
			for(int j=1;j<MAX_LOCATIONS;j++)
				if(location[i].getDistance(j)<min)
				{
					int ok=1;
					for(int k=1;k<counter;k++)
						if(locationList[i][k]==j) ok=0;
					if(ok)
					{
						min=location[i].getDistance(j);
						locationList[i][counter]=j;
		       			}
			       	}
		}
	}
	return(1);
};


MAP::MAP()
{
	name=new char[64];
	strcpy(name,"Error!");
	maxLocations=0;
	maxPlayer=0;
};

MAP::~MAP()
{
	delete[] name;
};

