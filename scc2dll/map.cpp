#include "map.h"
#include "string.h"
#include "debug.h"
#include "location.h"

int MAP::getStartPlayerRace(int player)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=MAX_PLAYER))
	{
		debug.toLog(0,"DEBUG: (MAP::getStartPlayerRace): Value player [%i] out of range.",player);
		return(0);
	};
#endif
	return(startPlayer[player].getRace());
};

PLAYER* MAP::getStartPlayer(int player)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=MAX_PLAYER))
	{
		debug.toLog(0,"DEBUG: (MAP::getStartPlayer): Value player [%i] out of range.",player);
		return(0);
	};
#endif
	return(&(startPlayer[player]));
};

int MAP::setStartPlayerGoal(int player, GOAL_ENTRY* goal)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=MAX_PLAYER))
	{
	       debug.toLog(0,"DEBUG: (MAP::setStartPlayerGoal): Value player [%i] out of range.",player);
		return(0);
						                                
	};       
	if(!goal)
	{
	       debug.toLog(0,"DEBUG: (MAP::setStartPlayerGoal): Variable goal not initialized [%i].",goal);
	       return(0);
						                                
	};
#endif
	return(startPlayer[player].setGoal(goal));
};

int MAP::setStartPlayerRace(int player, int race)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=MAX_PLAYER))
	{
	       debug.toLog(0,"DEBUG: (MAP::setStartPlayerRace): Value player [%i] out of range.",player);
		return(0);
						                                
	};
#endif
	return(startPlayer[player].setRace(race));
};

int MAP::setStartPlayerHarvestSpeed(int player, const int* mining, const int* gasing)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=MAX_PLAYER))
	{
	       debug.toLog(0,"DEBUG: (MAP::setStartPlayerHarvestSpeed): Value player [%i] out of range.", player);
		return(0);
						                                
	};
#endif
	return(startPlayer[player].setHarvest(mining,gasing));
};

int MAP::setStartPlayerPosition(int player, int position)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=MAX_PLAYER))
	{
	       debug.toLog(0,"DEBUG: (MAP::setStartPlayerPosition): Value player [%i] out of range.", player);
	       return(0);
						                                                                                                            
	};
#endif
	return(startPlayer[player].setPosition(position));
};

int MAP::setStartPlayerMins(int player, int mins)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=MAX_PLAYER))
	{
	       debug.toLog(0,"DEBUG: (MAP::setStartPlayerMins): Value player [%i] out of range.", player);
	       return(0);
						                                                                                                            
	};
#endif
	return(startPlayer[player].setMins(mins));
};

int MAP::setStartPlayerGas(int player, int gas)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=MAX_PLAYER))
	{
	       debug.toLog(0,"DEBUG: (MAP::setStartPlayerGas): Value player [%i] out of range.", player);
		return(0);
						                                                                                                            
	};
#endif
	return(startPlayer[player].setGas(gas));
};

int MAP::setStartPlayerTime(int player, int time)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=MAX_PLAYER))
	{
	       debug.toLog(0,"DEBUG: (MAP::setStartPlayerTime): Value player [%i] out of range.", player);
	       return(0);
						                                                                                                            
	};
#endif
	return(startPlayer[player].setTime(time));
};



int MAP::getLocationForce(int loc, int player, int unit)
{
#ifdef _SCC_DEBUG
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
	       debug.toLog(0,"DEBUG: (MAP::getLocationForce): Value loc [%i] out of range.",loc);
		return(0);

	};
#endif
	return(location[loc].getForce(player,unit));
};

int MAP::getDistance(int loc1, int loc2)
{
#ifdef _SCC_DEBUG
	if((loc1<1)||(loc1>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (MAP::getDistance): Value loc1 [%i] out of range..",loc1);
		return(0);
	};
#endif
	return(location[loc1].getDistance(loc2));
};

int MAP::getNearestLocation(int start, int step)
{
#ifdef _SCC_DEBUG
	if((start<1)||(start>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (MAP::getNearestLocation): Value start [%i] out of range..",start);
		return(0);
	};
	if((step<1)||(step>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (MAP::getNearestLocation): Value step [%i] out of range..",step);
		return(0);
	}
	if((locationList[start][step]<1)||(locationList[start][step]>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (MAP_LOCATION::getNearestLocation): Value locationList[%i][%i] out of range [%i].",start,step,locationList[start][step]);
		return(0);
	}
#endif
	return(locationList[start][step]);
};

int MAP::setLocationAvailible(int loc, int player, int unit, int num)
{
#ifdef _SCC_DEBUG
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (MAP::setLocationAvailible): Value loc [%i] out of range.",loc);
		return(0);
	}
#endif
	return(location[loc].setAvailible(player,unit,num));
};


int MAP::setLocationForce(int loc, int player, int unit, int num)
{
#ifdef _SCC_DEBUG
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (MAP::setLocationForce): Value loc [%i] out of range.",loc);
		return(0);
	}
#endif
	return(location[loc].setForce(player,unit,num));
};


int MAP::setMineralDistance(int loc, int num)
{
#ifdef _SCC_DEBUG
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (MAP::setMineralDistance): Value loc [%i] out of range.",loc);
		return(0);
	}
#endif
	return(location[loc].setMineralDistance(num));
};

int MAP::setDistance(int loc1, int loc2, int num)
{
#ifdef _SCC_DEBUG
	if((loc1<0)||(loc1>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (MAP::setDistance): Value loc [%i] out of range.",loc1);
		return(0);
	}
#endif
	location[loc1].setDistance(loc2,num);
	return(1);
};


int MAP::setLocationName(int loc, const char* name)
{
#ifdef _SCC_DEBUG
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (MAP::setLocationName): Value loc [%i] out of range.",loc);
		return(0);
	}
#endif
	location[loc].setName(name);
	return(1);
};

const char* MAP::getName()
{
	return(name);
};

int MAP::getMaxLocations()
{
#ifdef _SCC_DEBUG
	if((maxLocations<0)||(maxLocations>MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (MAP::getMaxLocations): Variable not initialized [%i].",maxLocations);
		return(0);
	}
#endif
	return(maxLocations);
};

int MAP::getMaxPlayer()
{
#ifdef _SCC_DEBUG
	if((maxPlayer<0)||(maxPlayer>MAX_PLAYER))
	{
		debug.toLog(0,"DEBUG: (MAP::getMaxPlayer): Variable not initialized [%i].",maxPlayer);
		return(0);
	}
#endif
	return(maxPlayer);
};

int MAP::setName(const char* line)
{
#ifdef _SCC_DEBUG
	if(!line)
	{
		debug.toLog(0,"DEBUG: (MAP::setName): Variable line not initialized [%i].",line);
		return(0);
	}
#endif
	strcpy(name,line);
	return(1);
};

int MAP::setMaxLocations(int num)
{
#ifdef _SCC_DEBUG
	if((num<=0)||(num>MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (MAP::setMaxLocations): Value [%i] out of range.",num);
		return(0);
	}
#endif
	maxLocations=num;
	return(1);
};

int MAP::setMaxPlayer(int num)
{
#ifdef _SCC_DEBUG
	if((num<1)||(num>MAX_PLAYER))
	{
		debug.toLog(0,"DEBUG: (MAP::setMaxPlayer): Value [%i] out of range.",num);
		return(0);
	}
#endif
	maxPlayer=num;
	return(1);
};

int MAP::adjustSupply()
{
	//initialized?
	for(int k=0;k<MAX_PLAYER;k++)
	{
		startPlayer[k].setSupply(0);
		startPlayer[k].setMaxSupply(0);
		for(int i=0;i<MAX_LOCATIONS;i++)
			for(int j=UNIT_TYPE_COUNT;j--;)
			{
				if(i>0)
				{
					if(stats[startPlayer[k].getRace()][j].supply<0)
					{
						if(startPlayer[k].getMaxSupply()-stats[startPlayer[k].getRace()][j].supply*location[i].getForce(k,j)>MAX_SUPPLY)
						{
							if(startPlayer[k].getMaxSupply()<MAX_SUPPLY)
							{
								startPlayer[k].setSupply(startPlayer[k].getSupply()+(MAX_SUPPLY-startPlayer[k].getMaxSupply()));
								startPlayer[k].setMaxSupply(MAX_SUPPLY);
							}
						}
						else
						{
							startPlayer[k].setSupply(startPlayer[k].getSupply()-stats[startPlayer[k].getRace()][j].supply*location[i].getForce(k,j));
							startPlayer[k].setMaxSupply(startPlayer[k].getMaxSupply()-stats[startPlayer[k].getRace()][j].supply*location[i].getForce(k,j));
						}
					} else
					startPlayer[k].setSupply(startPlayer[k].getSupply()-stats[startPlayer[k].getRace()][j].supply*location[i].getForce(k,j));
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

int MAP::adjustResearches()
{
	for(int k=0;k<getMaxPlayer();k++)
		switch(startPlayer[k].getRace())
		{
			case TERRA:
				for(int j=R_STIM_PACKS;j<=R_CHARON_BOOSTER;j++)
				{
					setLocationForce(0,k,j,1-getLocationForce(0,k,j+STIM_PACKS-R_STIM_PACKS));
					setLocationAvailible(0,k,j,1-getLocationForce(0,k,j+STIM_PACKS-R_STIM_PACKS));
				}
				for(int j=R_INFANTRY_ARMOR;j<=R_SHIP_WEAPONS;j++)
				{
					setLocationForce(0,k,j,3-getLocationForce(0,k,j+STIM_PACKS-R_STIM_PACKS));
					setLocationAvailible(0,k,j,1-getLocationForce(0,k,j+STIM_PACKS-R_STIM_PACKS));
					//temporary researches and upgrades
				};break;
			case PROTOSS:
				for(int j=R_PSIONIC_STORM;j<=R_ARGUS_TALISMAN;j++)
				{
					setLocationForce(0,k,j,1-getLocationForce(0,k,j+PSIONIC_STORM-R_PSIONIC_STORM));
					setLocationAvailible(0,k,j,1-getLocationForce(0,k,j+PSIONIC_STORM-R_PSIONIC_STORM));
				}
				for(int j=R_ARMOR;j<=R_PLASMA_SHIELDS;j++)
				{
					setLocationForce(0,k,j,3-getLocationForce(0,k,j+PSIONIC_STORM-R_PSIONIC_STORM));
					setLocationAvailible(0,k,j,1-getLocationForce(0,k,j+PSIONIC_STORM-R_PSIONIC_STORM));
					//temporary researches and upgrades
				};break;
			case ZERG:
				for(int j=R_VENTRAL_SACKS;j<R_LURKER_ASPECT;j++)
				{
					 setLocationForce(0,k,j,1-getLocationForce(0,k,j+VENTRAL_SACKS-R_VENTRAL_SACKS));
					 setLocationAvailible(0,k,j,1-getLocationForce(0,k,j+VENTRAL_SACKS-R_VENTRAL_SACKS));
				}
				for(int j=R_CARAPACE;j<R_FLYER_ATTACKS;j++)
				{
					setLocationForce(0,k,j,3-getLocationForce(0,k,j+VENTRAL_SACKS-R_VENTRAL_SACKS));
					setLocationAvailible(0,k,j,1-getLocationForce(0,k,j+VENTRAL_SACKS-R_VENTRAL_SACKS));
					       //temporary researches and upgrades
				};break;
			default:break;//error ?
		}
	return(0);
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

