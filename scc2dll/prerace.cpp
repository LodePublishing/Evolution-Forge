#include "prerace.h"
#include "debug.h"

void EXPORT PRERACE::createSpecial()
{
	if(getPlayer()->goal->getRace()==ZERG)
		for(int i=0;i<larvacounternumber;i++)
			if(!--larva[i].counter)
			{
				larva[i].counter=20;
				if(larva[i].larvacount<3)
				{
					larva[i].larvacount++;
					addLocationForce(larva[i].location,LARVA,1);
					addLocationAvailible(larva[i].location,LARVA,1);					
				}
			}
	//TODO energy, regeneration life/shield etc.
};

int EXPORT PRERACE::getMapLocationAvailible(int player, int loc, int type)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=pMap->getMaxPlayer()))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMapLocationAvailible): Value player [%i] out of range.",player);
		return(0);
	}

	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMapLocationAvailible): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMapLocationAvailible): Value type [%i] out of range.",type);
		return(0);
	}
	if((this->loc[player][loc].availible[type]<0)||(this->loc[player][loc].availible[type]>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMapLocationAvailible): Variable loc[%i][%i].availible[%i] not initialized [%i].",player,loc,type,this->loc[player][loc].availible[type]);
		return(0);
	}
#endif
	return(this->loc[player][loc].availible[type]);
};

int EXPORT PRERACE::getMapLocationForce(int player, int loc, int type)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=pMap->getMaxPlayer()))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMapLocationForce): Value player [%i] out of range.",player);
		return(0);
	}
																			    
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMapLocationForce): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMapLocationForce): Value type [%i] out of range.",type);
		return(0);
	}
	if((this->loc[player][loc].force[type]<0)||(this->loc[player][loc].force[type]>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMapLocationForce): Variable loc[%i][%i].force[%i] not initialized [%i].",player,loc,type,this->loc[player][loc].force[type]);
		return(0);
	}
#endif
	return(this->loc[player][loc].force[type]);
};

//TODO Static machen
int EXPORT PRERACE::setMapLocationAvailible(int player, int loc, int type, int num)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=pMap->getMaxPlayer()))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMapLocationAvailible): Value player [%i] out of range.",player);
		return(0);
	}
																			    
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMapLocationAvailible): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMapLocationAvailible): Value type [%i] out of range.",type);
		return(0);
	}
#endif
	this->loc[player][loc].availible[type]=num;
	return(1);
};
																			    
int EXPORT PRERACE::setMapLocationForce(int player, int loc, int type, int num)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=pMap->getMaxPlayer()))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMapLocationForce): Value player [%i] out of range.",player);
		return(0);
	}
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMapLocationForce): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMapLocationForce): Value type [%i] out of range.",type);
		return(0);
	}
	if((num<0)||(num>=MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMapLocationForce): Value num [%i] out of range.",num);
		return(0);
	}
#endif
	this->loc[player][loc].force[type]=num;
	return(1);
};

int EXPORT PRERACE::addMapLocationAvailible(int player, int loc, int type, int num)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=pMap->getMaxPlayer()))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addMapLocationAvailible): Value player [%i] out of range.",player);
		return(0);
	}
																			    
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addMapLocationAvailible): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addMapLocationAvailible): Value type [%i] out of range.",type);
		return(0);
	}
	if((this->loc[player][loc].availible[type]<0)||(this->loc[player][loc].availible[type]>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addMapLocationAvailible): Variable loc[%i][%i].availible[%i] not initialized [%i].",player,loc,type,this->loc[player][loc].availible[type]);
		return(0);
	}

	if((this->loc[player][loc].availible[type]+num<0)||(this->loc[player][loc].availible[type]+num>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addMapLocationAvailible): Value num [%i] out of range.",num);
		return(0);
	}
#endif
	this->loc[player][loc].availible[type]+=num;
	return(1);
};


int EXPORT PRERACE::addMapLocationForce(int player, int loc, int type, int num)
{
#ifdef _SCC_DEBUG
	if((player<0)||(player>=pMap->getMaxPlayer()))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addMapLocationForce): Value player [%i] out of range.",player);
		return(0);
	}
																			    
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addMapLocationForce): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addMapLocationForce): Value type [%i] out of range.",type);
		return(0);
	}
	if((this->loc[player][loc].force[type]<0)||(this->loc[player][loc].force[type]>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addMapLocationForce): Variable loc[%i][%i].force[%i] not initialized [%i].",player,loc,type,this->loc[player][loc].force[type]);
		return(0);
	}

	if((this->loc[player][loc].force[type]+num<0)||(this->loc[player][loc].force[type]+num>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addMapLocationForce): Value num [%i] out of range.",num);
		return(0);
	}
#endif
	this->loc[player][loc].force[type]+=num;
	return(1);
};
																			    

//----

int EXPORT PRERACE::getLocationAvailible(int loc, int type)
{
#ifdef _SCC_DEBUG
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getLocationAvailible): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getLocationAvailible): Value type [%i] out of range.",type);
		return(0);
	}
	if((location[loc].availible[type]<0)||(location[loc].availible[type]>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getLocationAvailible): Variable location[%i].availible[%i] not initialized [%i].",loc,type,location[loc].availible[type]);
		return(0);
	}
#endif
	return(location[loc].availible[type]);
};

int EXPORT PRERACE::getLocationForce(int loc, int type)
{
#ifdef _SCC_DEBUG
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getLocationForce): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getLocationForce): Value type [%i] out of range.",type);
		return(0);
	}
	if((location[loc].force[type]<0)||(location[loc].force[type]>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getLocationForce): Variable location[%i].force[%i] not initialized [%i].",loc,type,location[loc].force[type]);
		return(0);
	}
#endif
	return(location[loc].force[type]);
};


int EXPORT PRERACE::setLocationAvailible(int loc, int type, int num)
{
#ifdef _SCC_DEBUG
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setLocationAvailible): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setLocationAvailible): Value type [%i] out of range.",type);
		return(0);
	}
	if((num<0)||(num>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setLocationAvailible): Value num [%i] out of range.",num);
		return(0);
	}
#endif
	location[loc].availible[type]=num;
	return(1);
};

int EXPORT PRERACE::setLocationForce(int loc, int type, int num)
{
#ifdef _SCC_DEBUG
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setLocationForce): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setLocationForce): Value type [%i] out of range.",type);
		return(0);
	}
	if((num<0)||(num>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setLocationForce): Value num [%i] out of range.",num);
		return(0);
	}
#endif
	location[loc].force[type]=num;
	return(1);
};


int EXPORT PRERACE::addLocationAvailible(int loc, int type, int num)
{
#ifdef _SCC_DEBUG
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addLocationAvailible): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addLocationAvailible): Value type [%i] out of range.",type);
		return(0);
	}
	if((location[loc].availible[type]<0)||(location[loc].availible[type]>MAX_SUPPLY)) //location[0] auch pruefen?
	{
		debug.toLog(0,"DEBUG: (PRERACE::addLocationAvailible): Variable location[%i].availible[%i] not initialized [%i].",loc,type,location[loc].availible[type]);
		return(0);
	}
	if((location[loc].availible[type]+num<0)||(location[loc].availible[type]+num>MAX_SUPPLY)) //location[0] auch pruefen?
	{
		debug.toLog(0,"DEBUG: (PRERACE::addLocationAvailible): Value num [%i] out of range.",num);
		return(0);
	}
#endif	
	location[loc].availible[type]+=num;
	if(loc>0) //sonst waers ja doppelt...
		location[0].availible[type]+=num;
	return(1);
};
																			    
int EXPORT PRERACE::addLocationForce(int loc, int type, int num)
{
#ifdef _SCC_DEBUG
	if((loc<0)||(loc>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addLocationForce): Value loc [%i] out of range.",loc);
		return(0);
	}
	if((type<0)||(type>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addLocationForce): Value type [%i] out of range.",type);
		return(0);
	}
	if((location[loc].force[type]<0)||(location[loc].force[type]>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addLocationForce): Variable location[%i].force[%i] not initialized [%i].",loc,type,location[loc].force[type]);
		return(0);
	}
	if((location[loc].force[type]+num<0)||(location[loc].force[type]+num>MAX_SUPPLY))
	{
		debug.toLog(0,"DEBUG: (PRERACE::addLocationForce): Variable num not initialized [%i].",num);
		return(0);
	}
#endif
	location[0].force[type]+=num;
	if(loc>0)
		location[loc].force[type]+=num;
	return(1);
};

MAP* PRERACE::getMap()
{
//TODO Fehlerbehandlung
	return(pMap);
};

int EXPORT PRERACE::setMins(int mins)
{
#ifdef _SCC_DEBUG
	if((mins<0)||(mins>MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMins): Value mins [%i] out of range.",mins);
		return(0);
	}
#endif
	this->mins=mins;
	return(1);
};

int EXPORT PRERACE::setGas(int gas)
{
#ifdef _SCC_DEBUG
	if((gas<0)||(gas>MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setGas): Value gas [%i] out of range.",gas);
		return(0);
	}
#endif
	this->gas=gas;
	return(1);
};

int EXPORT PRERACE::setTimer(int time)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setTimer): Value time [%i] out of range.",time);
		return(0);
	}
#endif
	timer=time;
	return(1);
};

int EXPORT PRERACE::setSupply(int supply)
{
#ifdef _SCC_DEBUG
	if((supply>MAX_SUPPLY)||(supply<0))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setSupply): Value supply [%i] out of range.",supply);
		return(0);
	}
#endif
	this->supply=supply;
	return(1);
};

int EXPORT PRERACE::setMaxSupply(int supply)
{
#ifdef _SCC_DEBUG
	if((supply>MAX_SUPPLY)||(supply<0))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMaxSupply): Value supply [%i] out of range.",supply);
		return(0);
	}
#endif
	maxSupply=supply;
	return(1);
};

int EXPORT PRERACE::setMap(MAP* map)
{
#ifdef _SCC_DEBUG
	if(!map)
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMap): Map not initialized.");
		return(0);
	}
#endif
	pMap=map;
	mapInitialized=1;
	return(1);
};


int EXPORT PRERACE::getPlayerNum()
{
#ifdef _SCC_DEBUG
	if((playerNum<0)||(playerNum>=MAX_PLAYER))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getPlayer): Variable not initialized [%i].",playerNum);
		return(0);
	}
#endif
	return(playerNum);
};

int EXPORT PRERACE::getSupply()
{
#ifdef _SCC_DEBUG
	if((supply<0)||(supply>200))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getSupply): Variable not initialized [%i].",supply);
		return(0);
	}
#endif
	return(supply);
};
int EXPORT PRERACE::getMaxSupply()
{
#ifdef _SCC_DEBUG
	if((maxSupply<0)||(maxSupply>200))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMaxSupply): Variable not initialized [%i].",maxSupply);
		return(0);
	}
#endif
	return(maxSupply);
};
int EXPORT PRERACE::getMins()
{
#ifdef _SCC_DEBUG
	if((mins<0)||(mins>MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMins): Variable mins not initialized [%i].",mins);
		return(0);
	}
#endif
	return(mins);
};
int EXPORT PRERACE::getGas()
{
#ifdef _SCC_DEBUG
	if((gas<0)||(gas>MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getGas): Variable gas not initialized [%i].",gas);
		return(0);
	}
#endif
	return(gas);
};
int EXPORT PRERACE::getTimer()
{
#ifdef _SCC_DEBUG
	if((timer<0)||(timer>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getTimer): Variable timer not initialized [%i].",timer);
		return(0);
	}
#endif
	return(timer);
};


int EXPORT PRERACE::adjustMineralHarvest(int location)
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::adjustMineralHarvest): Value location [%i] out of range.",location);
		return(0);
	}
#endif
	//TODO Zerg hatchery,lair etc.
	if((location==0)||((!getLocationForce(location,COMMAND_CENTER))&&(!getMapLocationForce(0,location,MINERALS))))
	{
		for(int i=45;i--;)
			setMineralHarvestPerSecond(location,i,0);
	}
//	else if((!pMap->location[num].force[playerNum][COMMAND_CENTER])&&(pMap->location[num].force[0][MINERALS]))
//	{
	//nach naehestem command_center suchen
//	}
//	else if((pMap->location[num].force[playerNum][COMMAND_CENTER])&&(!pMap->location[num].force[0][MINERALS]))
//	{
	//nach naehesten Mineralien suchen
//	}

	//TODO: Wenn 2 SPieler an einem sammeln, beide einberechnen!
	else if(player->getBasicMineralHarvestPerSecond(1)>0) //???
	{
//		int k;
		for(int i=45;i--;)
		{
//			k=0;
//			for(j=0;j<45;j++)
//				if(i*8<=j*pMap->location[num].force[0][MINERALS]) 
//				{ k=j;j=45;};
			setMineralHarvestPerSecond(location,i,player->getBasicMineralHarvestPerSecond(i/*k*/));//*pMap->location[num].force[0][MINERALS])/8;
		}	//ab hier funzt alles... nur scheint player->getBasic... nicht richtig initialisiert zu sein...
	
	}
	return(1);
};

int EXPORT PRERACE::adjustGasHarvest(int location)
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::adjustGasHarvest): Value location [%i] out of range.",location);
		return(0);
	}
#endif
	if((location==0)||((!getLocationForce(location,COMMAND_CENTER))&&(!getLocationForce(location,REFINERY))))
	{
		for(int i=5;i--;)
			setGasHarvestPerSecond(location,i,0);
	}
/*	else if((!pMap->location[num].force[playerNum][COMMAND_CENTER])&&(pMap->location[num].force[playerNum][REFINERY]))
	{
	//nach naehestem command_center suchen
	}
	else if((pMap->location[num].force[playerNum][COMMAND_CENTER])&&(!pMap->location[num].force[playerNum][REFINERY]))
	{
	//nach naehesten Mineralien suchen
	}*/
	else
	{
		int k;
		for(int i=5;i--;)
		{
			k=0;
//			for(j=0;j<5;j++)
//				if(i<=j*pMap->location[num].force[playerNum][REFINERY]) { k=j;j=5;}
					setGasHarvestPerSecond(location,i,player->getBasicGasHarvestPerSecond(i/*k*/)*getLocationForce(location,REFINERY));

//					hier liegts problem wegen gas
//						gasharvestps ist zu gross, evtl wegen zu vieler refineries!
		}
	}
	return(1);
};


int EXPORT PRERACE::setPlayerNum(int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>=MAX_PLAYER))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setPlayerNum): Value [%i] out of range.",num);
		return(0);
	}
#endif
	playerNum=num; //~```~  player[0]?
	location=(loc[num]);
	return(1);
};

PLAYER* EXPORT PRERACE::getPlayer()
{
#ifdef _SCC_DEBUG
	if(!player->getInitialized())
	{
		debug.toLog(0,"DEBUG: (PRERACE::getPlayer): Variable not initialized.");
		return(0);
	}
#endif
	return(player);
};

int EXPORT PRERACE::setPlayer(PLAYER* player)
{
#ifdef _SCC_DEBUG
	if(!player->getInitialized())
	{
		debug.toLog(0,"DEBUG: (PRERACE::setPlayer): Variable not initialized.");
		return(0);
	}
#endif
	this->player=player;
	return(1);
};

int EXPORT PRERACE::setpStats(const UNIT_STATISTICS* pStats)
{
#ifdef _SCC_DEBUG
	if(pStats[0].mins!=0)//~~
	{
		debug.toLog(0,"DEBUG: (PRERACE::setpStats): Variable not initialized.");
		return(0);
	}
#endif
	this->pStats=pStats;
	return(1);
};


const UNIT_STATISTICS* EXPORT PRERACE::getpStats()
{
#ifdef _SCC_DEBUG
	if(!pStats)
	{
		debug.toLog(0,"DEBUG: (PRERACE::getpStats): Variable not initialized.");
		return(0);
	}
#endif
	return(pStats);
};


int EXPORT PRERACE::loadPlayer(int num)
{
	setPlayerNum(num);
	setPlayer(pMap->getStartPlayer(num));
	setpStats(player->goal->getpStats());
	setMins(player->getMins());
	setGas(player->getGas());
	setTimer(player->getTimer());
	return(1);
};

int EXPORT PRERACE::adjustHarvest()
{
#ifdef _SCC_DEBUG
	if(!mapInitialized)
	{
		debug.toLog(0,"DEBUG: (PRERACE::adjustHarvest): Map was not initialized.");
		return(0);
	}
#endif
	for(int i=0;i<MAX_LOCATIONS;i++)
	{
		adjustMineralHarvest(i);
		adjustGasHarvest(i);
	}
	return(1);
}


int EXPORT PRERACE::harvestMinerals()
{
	int sum=0;
//      int t=(rand()%10)-5;
	for(int i=1;i<MAX_LOCATIONS;i++)//~~
	{
		int s=location[i].availible[SCV];
		if(s)
		{
			//availible is 0, harvestSpeed ist ok!
			if(s<44) //~~ war 56
				sum+=getMineralHarvestPerSecond(i,s);
			else
				sum+=getMineralHarvestPerSecond(i,44);
/*		{
			setMins(getMins()+getMineralHarvestPerSecond(i,s));
			//~~~ neu berechnen / Tabelle anlegen! sind ja nicht bei jeder Exe gleichviel Mineralien... <- das sollte sich mit adjustHarvest erledigt haben
			harvestedMins+=getMineralHarvestPerSecond(i,s); //~~~
		}
		else
		{
			setMins(getMins()+getMineralHarvestPerSecond(i,44)); //war 56
			harvestedMins+=getMineralHarvestPerSecond(i,44);
		}*/
		}
	}
	return(sum);
}


int EXPORT PRERACE::harvestGas()
{
	int sum=0;
//	int t=(rand()%10)-5;
	for(int i=1;i<MAX_LOCATIONS;i++)//~~
	{
		int s=location[i].availible[GAS_SCV];
		if(s)
		{
			if(s<4)
				sum+=getGasHarvestPerSecond(i,s);
			else
				sum+=getGasHarvestPerSecond(i,4); //TODO
	/*
				harvestedGas+=getGasHarvestPerSecond(i,s);
			}
			else
			{
				setGas(getGas()+getGasHarvestPerSecond(i,4));
				harvestedGas+=getGasHarvestPerSecond(i,4);
			}*/
		}
	}
	return(sum);
};

int EXPORT PRERACE::getCalculated()
{
#ifdef _SCC_DEBUG
	if((calculated<0)||(calculated>1))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getCalculated): Variable is not initialized [%i].",calculated);
		return(0);
	}
#endif
	return(calculated);
};

int EXPORT PRERACE::setCalculated(int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>1))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setCalculated): Value [%i] out of range.",num);
		return(0);
	}
#endif
	calculated=num;
	return(1);
};

int EXPORT PRERACE::resetSupply()
{
#ifdef _SCC_DEBUG
	if(!mapInitialized)
	{
		debug.toLog(0,"DEBUG: (PRERACE::resetSupply): Map was not initialized.");
		return(0);
	}
#endif
	setMaxSupply(0);
	int temp=0;
	for(int i=1;i<MAX_LOCATIONS;i++)
		for(int j=UNIT_TYPE_COUNT;j--;)
		{
			if(pStats[j].supply<0) // -> supply building like depot, cc etc.
			{
				if(getMaxSupply()-pStats[j].supply*location[i].force[j]>MAX_SUPPLY)
				{
					if(getMaxSupply()<MAX_SUPPLY)
					{
	//TODO!!				
						temp=getMaxSupply()+(MAX_SUPPLY-getMaxSupply());
						setMaxSupply(MAX_SUPPLY);
					}
				}
				else
				{
	//TODO handle negative supply at beginning
					temp-=pStats[j].supply*location[i].force[j];
					setMaxSupply(getMaxSupply()-pStats[j].supply*location[i].force[j]);
				}
			} else //unit or something
				temp-=pStats[j].supply*location[i].force[j];
		}
	setSupply(temp);
	return(1);
};


int EXPORT PRERACE::getIP()
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getIP): Variable IP not initialized [%i].",IP);
		return(0);
	}
#endif
	return(IP);
}

int EXPORT PRERACE::getMineralHarvestPerSecond(int location, int worker)
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMineralHarvestPerSecond): Value location [%i] out of range.",location);
		return(0);
	}
	if((worker<0)||(worker>=45))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMineralHarvestPerSecond): Value worker [%i] out of range.",worker);
		return(0);
	}

	if((mineralHarvestPerSecond[location][worker]<0)||(mineralHarvestPerSecond[location][worker]>MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMineralHarvestPerSecond): Variable mineralHarvestPerSecond[%i][%i] not initialized [%i].",location,worker,mineralHarvestPerSecond[location][worker]);
		return(0);
	}
#endif
	return(mineralHarvestPerSecond[location][worker]);
}

int EXPORT PRERACE::getGasHarvestPerSecond(int location,int worker)
{
#ifdef _SCC_DEBUG
	if((location<0)||(location>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getGasHarvestPerSecond): Value location [%i] out of range.",location);
		return(0);
	}
	if((worker<0)||(worker>=5))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getGasHarvestPerSecond): Value worker [%i] out of range.",worker);
		return(0);
	}
	if((gasHarvestPerSecond[location][worker]<0)||(gasHarvestPerSecond[location][worker]>MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getGasHarvestPerSecond): Variable gasHarvestPerSecond[%i][%i] not initialized [%i].",location,worker,gasHarvestPerSecond[location][worker]);
		return(0);
	}
#endif
	return(gasHarvestPerSecond[location][worker]);
}

int EXPORT PRERACE::getHarvestedMins()
{
#ifdef _SCC_DEBUG
	if((harvestedMins<0)||(harvestedMins>MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getHarvestedMins): Variable harvestedMins not initialized [%i].",harvestedMins);
		return(0);
	}
#endif
	return(harvestedMins);
};

int EXPORT PRERACE::getHarvestedGas()
{
#ifdef _SCC_DEBUG
	if((harvestedGas<0)||(harvestedGas>MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getHarvestedGas): Variable harvestedGas not initialized [%i].",harvestedGas);
		return(0);
	}
#endif
	return(harvestedGas);
};

int EXPORT PRERACE::getFinalTime(int goal)
{
#ifdef _SCC_DEBUG
	if((goal<0)||(goal>=MAX_GOALS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getFinalTime): Value goal [%i] out of range.",goal);
		return(0);
	}
	if((ftime[goal]<0)||(ftime[goal]>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getFinalTime): Variable ftime[%i] not initialized [%i].",goal,ftime[goal]);
		return(0);
	}
#endif
	return(ftime[goal]);
};

int EXPORT PRERACE::getLength()
{
#ifdef _SCC_DEBUG
	if((length<0)||(length>MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getLength): Variable length not initialized [%i].",length);
		return(0);
	}
#endif
	return(length);
};

int EXPORT PRERACE::setIP(int IP)
{
#ifdef _SCC_DEBUG
	if((IP<0)||(IP>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setIP): Value IP [%i] out of range.",IP);
		return(0);
	}
#endif
	this->IP=IP;
	return(1);
};

int EXPORT PRERACE::setMineralHarvestPerSecond(int location,int worker,int mins)
{
#ifdef _SCC_DEBUG
	if((mins<0)||(mins>=MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMineralHarvestPerSecond): Value mins [%i] out of range.",mins);
		return(0);
	}
	if((location<0)||(location>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMineralHarvestPerSecond): Value location [%i] out of range.",location);
		return(0);
	}
	if((worker<0)||(worker>=45))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMineralHarvestPerSecond): Value worker [%i] out of range.",worker);
		return(0);
	}
#endif
	mineralHarvestPerSecond[location][worker]=mins;
	return(1);
};

int EXPORT PRERACE::setGasHarvestPerSecond(int tloc,int worker,int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>=MAX_GAS)||(tloc<0)||(tloc>=MAX_LOCATIONS)||(worker<0)||(worker>=5))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setGasHarvestPerSecond): Value [%i/%i/%i] out of range.",tloc,worker,num);
		return(0);
	}
#endif
	gasHarvestPerSecond[tloc][worker]=num;
	return(1);
};

int EXPORT PRERACE::setHarvestedMins(int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>=MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setHarvestedMins): Value [%i] out of range.",num);
		return(0);
	}
#endif
	harvestedMins=num;
	return(1);
};

int EXPORT PRERACE::setHarvestedGas(int num)
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>=MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setHarvestedGas): Value [%i] out of range.",num);
		return(0);
	}
#endif
	harvestedGas=num;
	return(1);
};

int EXPORT PRERACE::setFinalTime(int goal, int time)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setFinalTime): Value time [%i] out of range.",time);
		return(0);
	}
        if((goal<0)||(goal>=MAX_GOALS))
        {
                debug.toLog(0,"DEBUG: (PRERACE::setFinalTime): Value goal [%i] out of range.",goal);
                return(0);
        }

#endif
	ftime[goal]=time;
	return(1);
};

int EXPORT PRERACE::setLength(int length)
{
#ifdef _SCC_DEBUG
	if((length<0)||(length>MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setLength): Value length [%i] out of range.",length);
		return(0);
	}
#endif
	this->length=length;
	return(1);
};


int EXPORT PRERACE::setTimeOut(int time)
{
#ifdef _SCC_DEBUG
	if((time<0)||(time>MAX_TIMEOUT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setTimeOut): Value time [%i] out of range.",time);
		return(0);
	}
#endif
	timeout=time;
	return(1);
};

int EXPORT PRERACE::getTimeOut()
{
#ifdef _SCC_DEBUG
	if((timeout<0)||(timeout>MAX_TIMEOUT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getTimeOut): Variable timeout not initialized [%i].",timeout);
		return(0);
	}
#endif
	return(timeout);
};

void EXPORT PRERACE::resetMapInitialized()
{
	mapInitialized=0;	
};

void EXPORT PRERACE::resetSpecial()
{
	for(int i=0;i<20;i++)
	{
		larva[i].counter=0;
		larva[i].location=0;
		larva[i].larvacount=0;
	}
	larvacounternumber=0;
	if(getPlayer()->goal->getRace()==ZERG)
	{
		for(int i=1;i<MAX_LOCATIONS;i++)
		{
			int j=(getLocationForce(i,HATCHERY)+getLocationForce(i,LAIR)+getLocationForce(i,HIVE));
			if(j)
			{
				for(int k=0;k<j;k++)
				{
					larva[larvacounternumber].counter=20;
					larva[larvacounternumber].location=i;
					larva[larvacounternumber].larvacount=1;
					larvacounternumber++;
					addLocationForce(i,LARVA,1);
					addLocationAvailible(i,LARVA,1);	
				}
			}
		}
	}
}

void PRERACE::adjustAvailibility(int loc,int fac,const UNIT_STATISTICS* stat)
{
	switch(stat->facility_type)
	{
		case IS_LOST:
			if(stat->facility[fac]>0)
			{
				addLocationAvailible(loc,stat->facility[fac],-1);
				setSupply(getSupply()+pStats[stat->facility[fac]].supply);
				if((getPlayer()->goal->getRace()==ZERG)&&(stat->facility[fac]==LARVA))
				{
					int bestPlace=0;//unschoen hier :/
					int bestCounter=0;
					for(int i=0;i<larvacounternumber;i++)
						if((larva[i].location==loc)&&(larva[i].larvacount>bestCounter))
							{bestCounter=larva[i].larvacount;bestPlace=i;}
					larva[bestPlace].larvacount--;
				}
			}
			if(stat->facility2>0)
			{
				addLocationAvailible(loc,stat->facility2,-1);
				setSupply(getSupply()+pStats[stat->facility2].supply);
			}
			break;
		case NEEDED_ONCE:
			break;
		case NEEDED_UNTIL_COMPLETE:
			if(stat->facility[fac]>0)
				addLocationAvailible(loc,stat->facility[fac],-1);
			if(stat->facility2>0)
				addLocationAvailible(loc,stat->facility2,-1);
				//kommt glaub nicht vor...
			break;
		case NEEDED_ONCE_IS_LOST:
			if(stat->facility2>0)
			{
				addLocationAvailible(loc,stat->facility2,-1);
				setSupply(getSupply()+pStats[stat->facility2].supply); // <- nicht noetig :/ eigentlich schon... bei gas scv z.B. :/
			}
			break;
		case NEEDED_UNTIL_COMPLETE_IS_LOST:
			if(stat->facility2>0)
			{
				addLocationAvailible(0/*location*/ ,stat->facility2,-1); // primarily for temporary R_researches, have to be location 0
// TODO				 setSupply(getSupply()+pStats[stat->facility2].supply); // <- nicht noetig :/
			}
			if(stat->facility[fac]>0)
				addLocationAvailible(loc,stat->facility[fac],-1);
			break;
		case NEEDED_UNTIL_COMPLETE_IS_LOST_BUT_AVAILIBLE:
			if(stat->facility2>0)
				addLocationAvailible(0/*loc*/,stat->facility2,-1);
			// no supply gain as the item is recovered... well or not? mmmh... ~~~~
			if(stat->facility[fac]>0)
				addLocationAvailible(loc,stat->facility[fac],-1);
			break;
		case NEEDED_ALWAYS:
			if(stat->facility[fac]>0)
				addLocationAvailible(loc,stat->facility[fac],-1);
				// supply?
			if(stat->facility2>0)
				addLocationAvailible(loc,stat->facility2,-1);
			break;
		default:debug.toLog(0,"DEBUG: (ANARACE::buildGene) default case reached!");break;
	}
}


int PRERACE::calculateReady()
{
	int ready=1;
	for(int i=MAX_GOALS;(i--)&&(ready);)
		if(getPlayer()->goal->goal[i].count)
			ready&=((getPlayer()->goal->goal[i].count<=getLocationForce(getPlayer()->goal->goal[i].location,getPlayer()->goal->goal[i].unit))&&((getPlayer()->goal->goal[i].time>=getFinalTime(i))||(getPlayer()->goal->goal[i].time==0)));
	return(ready);
}


PRERACE::PRERACE()
{
	calculated=0;
	player=0;
	mins=0;
	gas=0;
	lastcounter=0;
	lastunit=0;
	timer=MAX_TIME;
	setSupply(0);
	setMaxSupply(0);
	harvestedGas=0;
	harvestedMins=0;
	setLength(0);
	//todo: mehr auf 0 setzen!
	for(int i=0;i<MAX_LOCATIONS;i++)
	{
		for(int j=0;j<45;j++)
			setMineralHarvestPerSecond(i,j,0);
		for(int j=0;j<5;j++)
			setGasHarvestPerSecond(i,j,0);
	};
	for(int i=0;i<MAX_LENGTH;i++)
	{
		Code[0][i]=0;
		Code[1][i]=0;
		last[i].unit=0;
		last[i].location=0;
		last[i].count=0;
	};
	for(int i=0;i<20;i++)
	{
		larva[i].counter=0;
		larva[i].location=0;
		larva[i].larvacount=0;
	}
	larvacounternumber=0;
};

PRERACE::~PRERACE()
{
};

//void PRERACE::resetMapInitialized();
//int PRERACE::setMap(MAP* map);
//MAP* PRERACE::getMap();
int PRERACE::markerCounter;
MAP* PRERACE::pMap;
GA* PRERACE::ga;
int PRERACE::mapInitialized;
int PRERACE::noise[MAX_TIME];
MAP_LOCATION PRERACE::loc[MAX_PLAYER][MAX_LOCATIONS];

