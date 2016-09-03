#include "default.h"
#include "debug.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

DEFAULT::DEFAULT()
{
};

DEFAULT::~DEFAULT()
{
};

UNITS* EXPORT DEFAULT::getUnits(int race)
{
#ifdef _SCC_DEBUG
    if((race<MIN_RACE)||(race>MAX_RACE))     
	{
        debug.toLog(0,"DEBUG: (UNITS::getUnits): Value race [%i] out of range.",race);
        return(0);
    }
#endif 	
	return(&units[race]);
};

PLAYER* EXPORT DEFAULT::getPlayer(int race)
{
#ifdef _SCC_DEBUG
    if((race<MIN_RACE)||(race>MAX_RACE))
    {
        debug.toLog(0,"DEBUG: (UNITS::getPlayer): Value race [%i] out of range.",race);
        return(0);
    }
#endif
	return(&player[race]);
};

void DEFAULT::loadFromFile(const char* defaultFile, HARVEST_SPEED* harvestSpeed)
{
// TODO uebergebenen buffer von file-lese routine aufrufen und dann hierher uebergeben... aber wie kann ich den buffer zeilenweise auslesen?
	char line[1024],old[1024];
	FILE* pFile;
	char* buffer;
	char item[1024],param1[1024],param2[1024],param3[1024];
	int ln=0;
	int value1=0,value2=0,value3=0;
	int mode=0;
	if((pFile = fopen (defaultFile,"r"))==NULL)
	{
		debug.toLog(0,"ERROR: (DEFAULT::loadDefaults) %s: Could not open file!",defaultFile);
		return;
	}
	item[0]='\0';param1[0]='\0';param2[0]='\0';param3[0]='\0';
	while(fgets(line,sizeof(line),pFile)!=NULL)
	{
		++ln;
		line[strlen(line)-1]='\0';
		if((line[0]=='#')||(line[0]=='\0')) continue;
		strcpy(old,line);
		strcpy(item,"");
		strcpy(param1,"");strcpy(param2,"");strcpy(param3,"");
		value1=0;value2=0;value3=0;
		if((buffer=strtok(line,"\""))!=NULL) strcpy(item,buffer);strtok(NULL,"\"");
		if((buffer=strtok(NULL,"\""))!=NULL) strcpy(param1,buffer);strtok(NULL,"\"");
		if((buffer=strtok(NULL,"\""))!=NULL) strcpy(param2,buffer);strtok(NULL,"\"");
		if((buffer=strtok(NULL,"\""))!=NULL) strcpy(param3,buffer);strtok(NULL,"\"");
		if((buffer=strtok(NULL,"\""))!=NULL)
		{
			debug.toLog(0,"WARNING: (DEFAULT::loadDefaults) %s: Line %d [%s]: Too many parameters.",defaultFile,ln,old);
			continue;
		}
		if(((value1=atoi(param1))<0)||((value2=atoi(param2))<0)||((value3=atoi(param3))<0))
		{
			debug.toLog(0,"WARNING: (DEFAULT::loadDefaults) %s: Line %d [%s]: Value below zero.",defaultFile,ln,old);
			continue;
		}

		if(mode==0) //Out of any block
		{
			if(!strcmp(item,"@TERRA"))
				mode=1;
			else if(!strcmp(item,"@PROTOSS"))
				mode=2;
			else if(!strcmp(item,"@ZERG"))
				mode=3;
			else if(!strcmp(item,"@END"))
				debug.toLog(0,"WARNING: (DEFAULT::loadDefaults) %s: Line %d [%s]: Lonely @END.",defaultFile,ln,old);
			else
				debug.toLog(0,"WARNING: (DEFAULT::loadDefaults) %s: Line %d [%s]: Line is outside a block but is not marked as comment.",defaultFile,ln,old);
		}
		else if(mode>0)
		{
			if(!strcmp(item,"Starting Minerals"))
			{
				if(!player[mode-1].setMins(value1*100))
					debug.toLog(0,"WARNING: (DEFAULT::loadDefaults) %s: Line %d [%s]: Starting Minerals out of range.",defaultFile,ln,old);
			}
			else if(!strcmp(item,"Starting Gas"))
			{
				if(!player[mode-1].setGas(value1*100))
					debug.toLog(0,"WARNING: (DEFAULT::loadDefaults) %s: Line %d [%s]: Starting Gas out of range.",defaultFile,ln,old);
			}
			else if(!strcmp(item,"@END"))
				mode=0;
			else
			{
				int i;
				for(i=0;i<UNIT_TYPE_COUNT;i++)  // TODO reicht eigentlich nur bis GAS SCV... aber dann auch unten die abbruchbedingung anguggn
					if(strstr(stats[mode-1][i].name,item)!=NULL)
					{
						units[mode-1].setTotal(i, value1);
						units[mode-1].setAvailible(i, value1); //TODO evtl
						i=UNIT_TYPE_COUNT;
						break;
					};
				if(i!=UNIT_TYPE_COUNT) debug.toLog(0,"WARNING: (DEFAULT::loadDefaults) %s: Line %d [%s]: No matching unit name.",defaultFile,ln,item);
			}
		}
	}// END while
	// TODO check ob auch wirklich alle 3 rassen in der datei standen

	player[TERRA].setRace(TERRA);
	player[PROTOSS].setRace(PROTOSS);
	player[ZERG].setRace(ZERG);
	player[TERRA].setHarvest(&harvestSpeed[TERRA].minerals[0],&harvestSpeed[TERRA].gas[0]);
	player[PROTOSS].setHarvest(&harvestSpeed[PROTOSS].minerals[0],&harvestSpeed[PROTOSS].gas[0]);
	player[ZERG].setHarvest(&harvestSpeed[ZERG].minerals[0],&harvestSpeed[ZERG].gas[0]);
	
	adjustSupply();
	adjustResearches();		
	adjustGoals();
};

void DEFAULT::adjustResearches()
{
	for(int i=0;i<=MAX_RACE;i++)
		units[i].adjustResearches(i);
};

void DEFAULT::adjustGoals()
{
	for(int i=0;i<=MAX_RACE;i++)
	{
		player[i].getGoal()->setRace(i);
		player[i].adjustGoals(1, &units[i]);	
	}
};


void DEFAULT::adjustSupply()
{
	for(int i=0;i<=MAX_RACE;i++)
	{
		int supply=0;
		int maxSupply=0;
		units[i].adjustSupply(i,supply,maxSupply);
		player[i].setSupply(supply);
		player[i].setMaxSupply(maxSupply);
	}
}
		





