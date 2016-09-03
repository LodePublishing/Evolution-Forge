#include "map.h"
#include "string.h"
#include "debug.h"

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

void MAP::setStartPlayerGoal(int player, GOAL_ENTRY* goal)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=MAX_PLAYER))
	{
		   debug.toLog(0,"DEBUG: (MAP::setStartPlayerGoal): Value player [%i] out of range.",player);
		return;
	};	   
	if(!goal)
	{
		   debug.toLog(0,"DEBUG: (MAP::setStartPlayerGoal): Variable goal not initialized [%i].",goal);
		   return;
	};
#endif
	startPlayer[player].setGoal(goal);
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

int MAP::getStartPlayerPosition(int player)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=MAX_PLAYER))
	{
		   debug.toLog(0,"DEBUG: (MAP::getStartPlayerPosition): Value player [%i] out of range.", player);
		   return(0);
	};
#endif
	return(startPlayer[player].getPosition());
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
	return(startPlayer[player].setTimer(time));
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

const char* MAP::getLocationName(int loc)
{
#ifdef _SCC_DEBUG
		if((loc<0)||(loc>=MAX_LOCATIONS))
		{
				debug.toLog(0,"DEBUG: (MAP::getLocationName): Value loc [%i] out of range.",loc);
				return(0);
		}
		if(location[loc].getName()==NULL)
		{
				debug.toLog(0,"DEBUG: (MAP::getLocationName): Value location[%i] not initialized. [%i]",loc,location[loc].getName());
				return(0);
		}
#endif
	return(location[loc].getName());
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
#ifdef _SCC_DEBUG
    if(!name)
    {
        debug.toLog(0,"DEBUG: (MAP::gatName): Variable name not initialized [%i].",name);
        return(0);
    }
#endif
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

void MAP::adjustSupply()
{
	//initialized?
	for(int k=0;k<MAX_PLAYER;k++)
	{
		int supply=0;
		int maxSupply=0;
		for(int i=0;i<MAX_LOCATIONS;i++)
			location[i].adjustSupply(k,startPlayer[k].getRace(),supply,maxSupply);
		startPlayer[k].setSupply(supply);
		startPlayer[k].setMaxSupply(maxSupply);
	}
};

void MAP::adjustDistanceList()
{
	//initialized?
	for(int i=1;i<MAX_LOCATIONS;i++)
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
};

void MAP::adjustResearches()
{
	for(int k=1;k<getMaxPlayer();k++)
		location[0].adjustResearches(k,startPlayer[k].getRace());
};

MAP_LOCATION* EXPORT MAP::getLocation(int loc)
{
#ifdef _SCC_DEBUG
    if((loc<0)||(loc>=MAX_LOCATIONS))
    {
        debug.toLog(0,"DEBUG: (MAP::getLocation): Value loc [%i] out of range.",loc);
        return(0);
    }
#endif
	return(&location[loc]);
};

void MAP::resetForce()
{
	for(int i=0;i<MAX_LOCATIONS;i++)
		location[i].resetData();
};

void MAP::adjustGoals(int player)
{
#ifdef _SCC_DEBUG
    if((player<0)||(player>=getMaxPlayer()))
    {
        debug.toLog(0,"DEBUG: (MAP::adjustGoals): Value player [%i] out of range.",player);
        return;
    }
#endif
	startPlayer[player].adjustGoals(1, location[0].getUnits(player));
};

void MAP::copyBasic(MAP* map)
{
#ifdef _SCC_DEBUG
    if(!map)
    {
        debug.toLog(0,"DEBUG: (MAP::copyBasic): Variable map not initialized [%i].",map);
        return;
    }
#endif
    setName(map->getName());
    setMaxPlayer(map->getMaxPlayer());
    setMaxLocations(map->getMaxLocations());
    for(int i=0;i<MAX_PLAYER;i++)
    //  startPlayer[i].copyBasic(map->getStartPlayer(i));
        startPlayer[i].setPosition(map->getStartPlayerPosition(i));
    for(int i=0;i<MAX_LOCATIONS;i++)
	{
		location[i].resetForce(); //TODO: optimization: player 0 needs not be resetted.
	 	location[i].copyBasic(map->getLocation(i)); // copy only the locations, not the units
		location[i].copy(0, map->getLocation(i)->getUnits(0)); // copy player 0
	}
	// the remaining units will be written by default
    for(int i=0;i<MAX_LOCATIONS;i++) //~~ TODO
        for(int j=0;j<MAX_LOCATIONS;j++)
            locationList[i][j]=map->locationList[i][j];
};



void MAP::copy(MAP* map)
{
#ifdef _SCC_DEBUG
    if(!map)
    {
        debug.toLog(0,"DEBUG: (MAP::copy): Variable map not initialized [%i].",map);
        return;
    }
#endif
	setName(map->getName());
	setMaxPlayer(map->getMaxPlayer());
	setMaxLocations(map->getMaxLocations());
	for(int i=0;i<MAX_PLAYER;i++)
	{
		startPlayer[i].copy(map->getStartPlayer(i));
		startPlayer[i].setPosition(map->getStartPlayerPosition(i));
	}
	for(int i=0;i<MAX_LOCATIONS;i++)
		location[i].copy(map->getLocation(i));
	for(int i=0;i<MAX_LOCATIONS;i++) //~~ TODO
		for(int j=0;j<MAX_LOCATIONS;j++)
			locationList[i][j]=map->locationList[i][j];
};

void MAP::copy(DEFAULT* defaults)
{
#ifdef _SCC_DEBUG
    if(!defaults)
    {
        debug.toLog(0,"DEBUG: (MAP::copy): Variable defaults not initialized [%i].",defaults);
        return;
    }
#endif
	for(int i=1;i<getMaxPlayer();i++)
	{
		location[startPlayer[i].getPosition()].copy(i, defaults->getUnits(startPlayer[i].getRace()));
		location[0].copy(i, defaults->getUnits(startPlayer[i].getRace()));
		startPlayer[i].copy(defaults->getPlayer(startPlayer[i].getRace()));
	}	
};

void MAP::resetData()
{
	for(int i=0;i<MAX_LOCATIONS;i++)
		for(int j=0;j<MAX_LOCATIONS;j++)
			locationList[i][j]=0;
	for(int i=0;i<MAX_PLAYER;i++)
		startPlayer[i].resetData();
	//todo mit zeigern? brauch ja nicht immer maxplayer..
	strcpy(name,"Error!");
	maxLocations=0;
	maxPlayer=0;
	resetForce();
};


/*void MAP::switchTVBRace(int player, int race)
{
	//TODO evtl in ne externe Datei schreiben
	setLocationForce(0,player,SCV,4);			
	setLocationForce(getStartPlayerPosition(player),i,SCV,4);
	setStartPlayerRace(player, race);
	switch(race)
	{
		case TERRA:
			{
				setLocationForce(0,player,COMMAND_CENTER,1);
				setLocationForce(getStartPlayerPosition(player),player,COMMAND_CENTER,1);
			};break;
		case PROTOSS:
			{
				setLocationForce(0,player,NEXUS,1);
				setLocationForce(getStartPlayerPosition(player),player,NEXUS,1);
			};break;						
		case ZERG:
			{
			   setLocationForce(0,player,HATCHERY,1);
			   setLocationForce(getStartPlayerPosition(player),player,HATCHERY,1);
			   setLocationForce(0,player,OVERLORD,1);
			   setLocationForce(getStartPlayerPosition(player),player,OVERLORD,1);
			   setLocationForce(0,player,LARVA,3);
			   setLocationForce(getStartPlayerPosition(player),player,LARVA,3);
			};break;					
		}
	}
}*/

	
MAP::MAP()
{
	name=new char[64];
	resetData();
};

MAP::~MAP()
{
	delete[] name;
};

