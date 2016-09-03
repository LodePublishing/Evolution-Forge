#include "prerace.h"
#include "debug.h"

MAP* PRERACE::getMap()
{
	return(pMap);
};

int EXPORT PRERACE::setMins(int num)
{
	if((num<0)||(num>MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMins): Value [%i] out of range.",num);
		return(0);
	}
	mins=num;
	return(1);
};

int EXPORT PRERACE::setGas(int num)
{
	if((num<0)||(num>MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setGas): Value [%i] out of range.",num);
		return(0);
	}
	gas=num;
	return(1);
};

int EXPORT PRERACE::setTimer(int num)
{
	if((num<0)||(num>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setTimer): Value [%i] out of range.",num);
		return(0);
	}
	timer=num;
	return(1);
};

int EXPORT PRERACE::setSupply(int num)
{
	if((num>MAX_SUPPLY)||(num<0))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setSupply): Value [%i] out of range.",num);
		return(0);
	}
	supply=num;
	return(1);
};

int EXPORT PRERACE::setMaxSupply(int num)
{
	if((num>MAX_SUPPLY)||(num<0))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMaxSupply): Value [%i] out of range.",num);
		return(0);
	}
	maxSupply=num;
	return(1);
};

int EXPORT PRERACE::setMap(MAP* map)
{
	if(!map)
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMap): Map not initialized.");
		return(0);
	}
	pMap=map;
	mapInitialized=1;
	return(1);
};


int EXPORT PRERACE::getPlayerNum()
{
	if((playerNum<0)||(playerNum>=MAX_PLAYER))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getPlayer): Variable not initialized [%i].",playerNum);
		return(0);
	}
	return(playerNum);
};

int EXPORT PRERACE::getSupply()
{
	if((supply<0)||(supply>200))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getSupply): Variable not initialized [%i].",supply);
		return(0);
	}
	return(supply);
};
int EXPORT PRERACE::getMaxSupply()
{
	if((maxSupply<0)||(maxSupply>200))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMaxSupply): Variable not initialized [%i].",maxSupply);
		return(0);
	}
	return(maxSupply);
};
int EXPORT PRERACE::getMins()
{
	if((mins<0)||(mins>MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMins): Variable not initialized [%i].",mins);
		return(0);
	}
	return(mins);
};
int EXPORT PRERACE::getGas()
{
	if((gas<0)||(gas>MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getGas): Variable not initialized [%i].",gas);
		return(0);
	}
	return(gas);
};
int EXPORT PRERACE::getTimer()
{
	if((timer<0)||(timer>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getTimer): Variable not initialized [%i].",timer);
		return(0);
	}
	return(timer);
};


int EXPORT PRERACE::adjustMineralHarvest(int num)
{
	if((num<0)||(num>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::adjustMineralHarvest): Value [%i] out of range.",num);
		return(0);
	}
	int i,j;
	if((num==0)||((!location[num].force[COMMAND_CENTER])&&(!loc[0][num].force[MINERALS])))
	{
		for(j=45;j--;)
			mineralHarvestPerSecond[num][j]=0;
	}
	/*else if((!pMap->location[num].force[playerNum][COMMAND_CENTER])&&(pMap->location[num].force[0][MINERALS]))
	{
	//nach naehestem command_center suchen
	}
	else if((pMap->location[num].force[playerNum][COMMAND_CENTER])&&(!pMap->location[num].force[0][MINERALS]))
	{
	//nach naehesten Mineralien suchen
	}*/

	//TODO: Wenn 2 SPieler an einem sammeln, beide einberechnen!
	else if(player->getBasicMineralHarvestPerSecond(1)>0) //???
	{
//		int k;
		for(i=45;i--;)
		{
//			k=0;
//			for(j=0;j<45;j++)
//				if(i*8<=j*pMap->location[num].force[0][MINERALS]) 
//				{ k=j;j=45;};
			mineralHarvestPerSecond[num][i]=player->getBasicMineralHarvestPerSecond(i/*k*/);//*pMap->location[num].force[0][MINERALS])/8;
		}	//ab hier funzt alles... nur scheint player->getBasic... nicht richtig initialisiert zu sein...
	
	}
	return(1);
};

int EXPORT PRERACE::adjustGasHarvest(int num)
{
	if((num<0)||(num>=MAX_LOCATIONS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::adjustGasHarvest): Value [%i] out of range.",num);
		return(0);
	}
	int i,j;
	if((num==0)||((!location[num].force[COMMAND_CENTER])&&(!location[num].force[REFINERY])))
	{
		for(j=5;j--;)
			gasHarvestPerSecond[num][j]=0;
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
		for(i=5;i--;)
		{
			k=0;
//			for(j=0;j<5;j++)
//				if(i<=j*pMap->location[num].force[playerNum][REFINERY]) { k=j;j=5;}
					gasHarvestPerSecond[num][i]=(player->getBasicGasHarvestPerSecond(i/*k*/)*location[num].force[REFINERY]);

//					hier liegts problem wegen gas
//						gasharvestps ist zu gross, evtl wegen zu vieler refineries!
		}
	}
	return(1);
};


int EXPORT PRERACE::setPlayerNum(int num)
{
	if((num<0)||(num>=MAX_PLAYER))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setPlayerNum): Value [%i] out of range.",num);
		return(0);
	}
	playerNum=num; //~```~  player[0]?
	location=(loc[num]);
	return(1);
};

PLAYER* EXPORT PRERACE::getPlayer()
{
	if(!player->getInitialized())
	{
		debug.toLog(0,"DEBUG: (PRERACE::getPlayer): Variable not initialized.");
		return(0);
	}
	return(player);
};

int EXPORT PRERACE::setPlayer(PLAYER* player)
{
	if(!player->getInitialized())
	{
		debug.toLog(0,"DEBUG: (PRERACE::setPlayer): Variable not initialized.");
		return(0);
	}
	this->player=player;
	return(1);
};

int EXPORT PRERACE::setpStats(const UNIT_STATISTICS* pStats)
{
	if(pStats[0].mins!=0)//~~
	{
		debug.toLog(0,"DEBUG: (PRERACE::setpStats): Variable not initialized.");
		return(0);
	}
	this->pStats=pStats;
	return(1);
};


int EXPORT PRERACE::loadPlayer(int num)
{
	setPlayerNum(num);
	setPlayer(&pMap->player[num]);
	setpStats(player->goal->getpStats());
	setMins(player->getMins());
	setGas(player->getGas());
	setTimer(player->getTimer());
	return(1);
};

int EXPORT PRERACE::adjustHarvest()
{
	if(!mapInitialized)
	{
		debug.toLog(0,"DEBUG: (PRERACE::adjustHarvest): Map was not initialized.");
		return(0);
	}
	int i;
	for(i=0;i<MAX_LOCATIONS;i++)
	{
		adjustMineralHarvest(i);
		adjustGasHarvest(i);
	}
	return(1);
}

void EXPORT PRERACE::harvestResources()
{
	int i,s;
//	int t=(rand()%10)-5;
	for(i=1;i<MAX_LOCATIONS;i++)//~~
	if(s=location[i].availible[SCV])
	{
		//availible is 0, harvestSpeed ist ok!
		if(s<56) //~~
		{
			mins+=mineralHarvestPerSecond[i][s];
		       	//~~~ neu berechnen / Tabelle anlegen! sind ja nicht bei jeder Exe gleichviel Mineralien...
			harvestedMins+=mineralHarvestPerSecond[i][s]; //~~~
		}
		else
		{
			mins+=mineralHarvestPerSecond[i][56];
			harvestedMins+=mineralHarvestPerSecond[i][56];
		}
		s=location[i].availible[GAS_SCV];
		if(s<4)
		{
			setGas(getGas()+gasHarvestPerSecond[i][s]);
			harvestedGas+=gasHarvestPerSecond[i][s];
		}
		else
		{
			setGas(getGas()+gasHarvestPerSecond[i][4]);
			harvestedGas+=gasHarvestPerSecond[i][4];
		}
	}
};

int EXPORT PRERACE::getCalculated()
{
	if((calculated<0)||(calculated>1))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getCalculated): Variable is not initialized [%i].",calculated);
		return(0);
	}
	return(calculated);
};

int EXPORT PRERACE::setCalculated(int num)
{
	if((num<0)||(num>1))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setCalculated): Value [%i] out of range.",num);
		return(0);
	}
	calculated=num;
	return(1);
};

int EXPORT PRERACE::resetSupply()
{
	if(!mapInitialized)
	{
		debug.toLog(0,"DEBUG: (PRERACE::resetSupply): Map was not initialized.");
		return(0);
	}
	setSupply(0);
	setMaxSupply(0);
	int i,j;
	for(i=1;i<MAX_LOCATIONS;i++)
		for(j=UNIT_TYPE_COUNT;j--;)
		{
			if(pStats[j].supply<0)
			{
				if(getMaxSupply()-pStats[j].supply*location[i].force[j]>MAX_SUPPLY)
				{
					if(getMaxSupply()<MAX_SUPPLY)
					{
						setSupply(getMaxSupply()+(MAX_SUPPLY-getMaxSupply()));
						setMaxSupply(MAX_SUPPLY);
					}
				}
				else
				{
					setSupply(getSupply()-pStats[j].supply*location[i].force[j]);
					setMaxSupply(getMaxSupply()-pStats[j].supply*location[i].force[j]);
				}
			} else
				setSupply(getSupply()-pStats[j].supply*location[i].force[j]);
		}
	return(1);
};


int EXPORT PRERACE::getIP()
{
	if((IP<0)||(IP>MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getIP): Variable not initialized [%i].",IP);
		return(0);
	}
	return(IP);
}

int EXPORT PRERACE::getMineralHarvestPerSecond(int tloc,int worker)
{
	if((tloc<0)||(tloc>=MAX_LOCATIONS)||(worker<0)||(worker>=45))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMineralHarvestPerSecond): Value [%i/%i] out of range.",tloc,worker);
		return(0);
	}

	if((mineralHarvestPerSecond[tloc][worker]<0)||(mineralHarvestPerSecond[tloc][worker]>MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getMineralHarvestPerSecond): Variable not initialized [%i].",mineralHarvestPerSecond[tloc][worker]);
		return(0);
	}
	return(mineralHarvestPerSecond[tloc][worker]);
}

int EXPORT PRERACE::getGasHarvestPerSecond(int tloc,int worker)
{
	if((tloc<0)||(tloc>=MAX_LOCATIONS)||(worker<0)||(worker>=5))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getGasHarvestPerSecond): Value [%i/%i] out of range.",tloc,worker);
		return(0);
	}

	if((gasHarvestPerSecond[tloc][worker]<0)||(gasHarvestPerSecond[tloc][worker]>MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getGasHarvestPerSecond): Variable not initialized [%i].",gasHarvestPerSecond[tloc][worker]);
		return(0);
	}
	return(gasHarvestPerSecond[tloc][worker]);
}

int EXPORT PRERACE::getHarvestedMins()
{
	if((harvestedMins<0)||(harvestedMins>MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getHarvestedMins): Variable not initialized [%i].",harvestedMins);
		return(0);
	}
	return(harvestedMins);
};

int EXPORT PRERACE::getHarvestedGas()
{
	if((harvestedGas<0)||(harvestedGas>MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getHarvestedGas): Variable not initialized [%i].",harvestedGas);
		return(0);
	}
	return(harvestedGas);
};

int EXPORT PRERACE::getFinalTime(int goal)
{
	if((goal<0)||(goal>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getFinalTime): Value [%i] out of range.",goal);
		return(0);
	}
	if((ftime[goal]<0)||(ftime[goal]>MAX_TIME))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getFinalTime): Variable not initialized [%i].",ftime[goal]);
		return(0);
	}
	return(ftime[goal]);
};

int EXPORT PRERACE::getLength()
{
	if((length<0)||(length>MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getLength): Variable not initialized [%i].",length);
		return(0);
	}
	return(length);
};

int EXPORT PRERACE::setIP(int num)
{
	if((num<0)||(num>=MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setIP): Value [%i] out of range.",num);
		return(0);
	}
	IP=num;
	return(1);
};

int EXPORT PRERACE::setMineralHarvestPerSecond(int tloc,int worker,int num)
{
	if((num<0)||(num>=MAX_MINS)||(tloc<0)||(tloc>=MAX_LOCATIONS)||(worker<0)||(worker>=45))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setMineralHarvestPerSecond): Value [%i/%i/%i] out of range.",tloc,worker,num);
		return(0);
	}
	mineralHarvestPerSecond[tloc][worker]=num;
	return(1);
};

int EXPORT PRERACE::setGasHarvestPerSecond(int tloc,int worker,int num)
{
	if((num<0)||(num>=MAX_GAS)||(tloc<0)||(tloc>=MAX_LOCATIONS)||(worker<0)||(worker>=5))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setGasHarvestPerSecond): Value [%i/%i/%i] out of range.",tloc,worker,num);
		return(0);
	}
	gasHarvestPerSecond[tloc][worker]=num;
	return(1);
};

int EXPORT PRERACE::setHarvestedMins(int num)
{
	if((num<0)||(num>=MAX_MINS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setHarvestedMins): Value [%i] out of range.",num);
		return(0);
	}
	harvestedMins=num;
	return(1);
};

int EXPORT PRERACE::setHarvestedGas(int num)
{
	if((num<0)||(num>=MAX_GAS))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setHarvestedGas): Value [%i] out of range.",num);
		return(0);
	}
	harvestedGas=num;
	return(1);
};

int EXPORT PRERACE::setFinalTime(int goal, int num)
{
	if((num<0)||(num>MAX_TIME)||(goal<0)||(goal>=UNIT_TYPE_COUNT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setFinalTime): Value [%i/%i] out of range.",goal,num);
		return(0);
	}
	ftime[goal]=num;
	return(1);
};

int EXPORT PRERACE::setLength(int num)
{
	if((num<0)||(num>MAX_LENGTH))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setLength): Value [%i] out of range.",num);
		return(0);
	}
	length=num;
	return(1);
};


int EXPORT PRERACE::setTimeOut(int num)
{
	if((num<0)||(num>MAX_TIMEOUT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::setTimeOut): Value [%i] out of range.",num);
		return(0);
	}
	timeout=num;
	return(1);
};

int EXPORT PRERACE::getTimeOut()
{
	if((timeout<0)||(timeout>MAX_TIMEOUT))
	{
		debug.toLog(0,"DEBUG: (PRERACE::getTimeOut): Variable not initialized [%i].",timeout);
		return(0);
	}
	return(timeout);
};

void EXPORT PRERACE::resetMapInitialized()
{
	mapInitialized=0;	
};

PRERACE::PRERACE()
{
	int i,j;
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
	length=0;
	//todo: mehr auf 0 setzen!
	for(i=0;i<MAX_LOCATIONS;i++)
	{
		for(j=0;j<45;j++)
			mineralHarvestPerSecond[i][j]=0;
		for(j=0;j<5;j++)
			gasHarvestPerSecond[i][j]=0;
	};
	for(i=0;i<MAX_LENGTH;i++)
	{
		Code[0][i]=0;
		Code[1][i]=0;
		last[i].unit=0;
		last[i].location=0;
		last[i].count=0;
	};
};

PRERACE::~PRERACE()
{
};


// void PRERACE::resetMapInitialized();
//int PRERACE::setMap(MAP* map);
//MAP* PRERACE::getMap();
MAP* PRERACE::pMap;
GA* PRERACE::ga;
int PRERACE::mapInitialized;
MAP_LOCATION PRERACE::loc[MAX_PLAYER][MAX_LOCATIONS];

