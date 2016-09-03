#include "settings.h"
#include "debug.h"
#include <stdio.h> 
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <fstream>
#include <iostream>
#include <string>
#include <list>

void parse_line(string& text, list<string>& words)
{
    int n = text.length();
    int start, stop;
    start = text.find_first_not_of(",\9\"\ =");
    while ((start >= 0) && (start < n)) 
	{
       stop = text.find_first_of(",\9\0\n\"=", start);
       if ((stop < 0) || (stop > n)) stop = n;
       words.push_back(text.substr(start, stop - start));
       start = text.find_first_not_of(",\9\n\"\ =", stop+1);
    }
}


void parse_block(ifstream stream, map<string, list<string>>& block)
{
	char line[1024];
	string text;
    while(stream.getline(line, sizeof line))
    {
        if(pFile.fail())
            stream.clear(pFile.rdstate() & ~ios::failbit);
		text=line;
		if(text.find("@END")!=npos) return;
		int start=text.find_first_not_of("\9 ");
		if ((start >= 0) && (start <= n))
			if((text[0]=='#')||(text[0]=='\0')) continue; // ignore line
		list<string> words;
        parse_line(text, words);
		block.Insert(text.substr(start), words);
	};
};

void parse_2nd_block(ifstream stream, map<string, map<string, list<string>>>& block)
{
    char line[1024];
    string text;
    while(stream.getline(line, sizeof line))
    {
        if(pFile.fail())
            stream.clear(pFile.rdstate() & ~ios::failbit);
        text=line;
		if(text.find("@END")!=npos) return;
        int start=text.find_first_not_of("\9 ");
        if ((start >= 0) && (start <= n))
            if((text[0]=='#')||(text[0]=='\0')) continue; // ignore line
        map<string, list<string>> words;
        parse_block(stream, words);
        block.Insert(text.substr(start), words);
    };
};


void parse_file()
{
    char line[1024];
    string text;
    while(stream.getline(line, sizeof line))
    {
        if(pFile.fail())
            stream.clear(pFile.rdstate() & ~ios::failbit);
        text=line;
        int start=text.find_first_not_of("\9 ");
        if ((start >= 0) && (start <= n))
            if((text[0]=='#')||(text[0]=='\0')) continue; // ignore line
		string index=text.substr(start);
		switch(index)
		{
			case "@STRINGS":
				map<string, map<string, list<string>>> block;
				parse_2nd_block(stream, block);
				for(const_iterator ... begin ... end)
					for(const iterator ... begin ... end)
						for(const iterator ... begin ... end)
							stringList[i][j][k]=block[][][].string;
				break;
			case ...

		}
    };
};


SETTINGS::SETTINGS()
{
	initDefaults();
	srand(time(NULL));
};

SETTINGS::~SETTINGS()
{
};

void EXPORT SETTINGS::initDefaults()
{
	currentMap=-1;UMS=0;
	setAllowGoalAdaption(1);
	setMaxTime(MAX_TIME);
	setMaxTimeOut(MAX_TIMEOUT);
	setMaxLength(MAX_LENGTH);
	setMaxRuns(MAX_RUNS);
	setMaxGenerations(MAX_GENERATIONS);
	setPreprocessBuildOrder(0);
	setCrossOver(MIN_CROSSOVER);
	setBreedFactor(20);
	setMode(MIN_MODE);
};

void EXPORT SETTINGS::calculateAnaplayer()
{
	soup.calculateAnaplayer();
};

MAP* EXPORT SETTINGS::getMap(int num)
{
#ifdef _SCC_DEBUG
	if((num>getMapCount())||(num<0))
	{
		debug.toLog(0,"WARNING: (SETTINGS::getMap): Value [%i] out of range.",num);
		return(0);
	}
#endif
	return(&loadedMap[num]);
};

int EXPORT SETTINGS::getMAXBreedFactor()
{
	return(MAX_BREED_FACTOR);
};

int EXPORT SETTINGS::getMAXMode()
{
	return(MAX_MODE);
};

int EXPORT SETTINGS::getMAXCrossOver()
{
	return(MAX_CROSSOVER);
};

int EXPORT SETTINGS::getMAXMaxTime()
{
	return(MAX_TIME);
};

int EXPORT SETTINGS::getMAXMaxTimeOut()
{
	return(MAX_TIMEOUT);
};

int EXPORT SETTINGS::getMAXMaxLength()
{
	return(MAX_LENGTH);
};

int EXPORT SETTINGS::getMAXMaxRuns()
{
	return(MAX_RUNS);
};

int EXPORT SETTINGS::getMAXMaxGenerations()
{
	return(MAX_GENERATIONS);
};

int EXPORT SETTINGS::getMAXPreprocessBuildOrder()
{
	return(MAX_PREPROCESS_BUILDORDER);
};

int EXPORT SETTINGS::getMAXCurrentMap()
{
	return(MAX_MAPS);
};

int EXPORT SETTINGS::getMINBreedFactor()
{
	return(MIN_BREED_FACTOR);
};

int EXPORT SETTINGS::getMINMode()
{
	return(MIN_MODE);
};

int EXPORT SETTINGS::getMINCrossOver()
{
	return(MIN_CROSSOVER);
};

int EXPORT SETTINGS::getMINMaxTime()
{
	return(MIN_TIME);
};

int EXPORT SETTINGS::getMINMaxTimeOut()
{
	return(MIN_TIMEOUT);
};

int EXPORT SETTINGS::getMINMaxLength()
{
	return(MIN_LENGTH);
};

int EXPORT SETTINGS::getMINMaxRuns()
{
	return(MIN_RUNS);
};

int EXPORT SETTINGS::getMINMaxGenerations()
{
	return(MIN_GENERATIONS);
};

int EXPORT SETTINGS::getMINPreprocessBuildOrder()
{
	return(MIN_PREPROCESS_BUILDORDER);
};

int EXPORT SETTINGS::getMINCurrentMap()
{
	return(MIN_MAPS);
};

										                                                                                                                    
void EXPORT SETTINGS::setAllowGoalAdaption(int num) // allow the program to change goals (for example ignore command center when command center [NS] is already a goal
{
#ifdef _SCC_DEBUG
	if((num<0)||(num>1))
	{
		debug.toLog(0,"WARNING: (SETTINGS::setAllowGoalAdaption): Value [%i] out of range.",num);
		return;
	}
#endif
	ga.allowGoalAdaption=num;
	return(1);
};


void EXPORT SETTINGS::setMaxTime(int num) //maximum time of build order in seconds
{
#ifdef _SCC_DEBUG
	if((num<MIN_TIME)||(num>MAX_TIME)) 
	{
		debug.toLog(0,"WARNING: (SETTINGS::setMaxTime): Value [%i] out of range.",num);
		return;
	}
#endif
	ga.maxTime=num;
	return;
};

void EXPORT SETTINGS::setMaxTimeOut(int num) //timeout for building
{
#ifdef _SCC_DEBUG
	if((num<MIN_TIMEOUT)||(num>MAX_TIMEOUT)) 
	{
		debug.toLog(0,"WARNING: (SETTINGS::setMaxTimeOut): Value [%i] out of range.",num);
		return;
	}
#endif
	ga.maxTimeOut=num;
	return;
};

void EXPORT SETTINGS::setMaxLength(int num)
{
#ifdef _SCC_DEBUG
	if((num<MIN_LENGTH)||(num>MAX_LENGTH)) 
	{
		debug.toLog(0,"WARNING: (SETTINGS::setMaxLength): Value [%i] out of range.",num);
		return;
	}
#endif
	ga.maxLength=num;
	return(1);
};

void EXPORT SETTINGS::setMaxRuns(int num)
{
#ifdef _SCC_DEBUG
	if((num<MIN_RUNS)||(num>MAX_RUNS))
	{
		debug.toLog(0,"WARNING: (SETTINGS::setMaxRuns): Value [%i] out of range.",num);
		return;
	}
#endif
	ga.maxRuns=num;
	return(1);
};

void EXPORT SETTINGS::setMaxGenerations(int num)
{
#ifdef _SCC_DEBUG
	if((num<MIN_GENERATIONS)||(num>MAX_GENERATIONS))
	{
		debug.toLog(0,"WARNING: (SETTINGS::setMaxGenerations): Value [%i] out of range.",num);
		return;
	}
#endif
	ga.maxGenerations=num;
	return(1);
};

int SETTINGS::setMapCount(int num)
{
#ifdef _SCC_DEBUG
	if((num<MIN_MAPS)||(num>=MAX_MAPS))
	{
		debug.toLog(0,"WARNING: (SETTINGS::setMapCount): Value [%i] out of range.",num);
		return;
	}
#endif
	ga.mapCount=num;
	return(1);
};

int SETTINGS::setGoalCount(int num)
{
#ifdef _SCC_DEBUG
	if((num<MIN_GOAL_ENTRIES)||(num>=MAX_GOAL_ENTRIES))
	{
		debug.toLog(0,"WARNING: (SETTINGS::setGoalCount): Value [%i] out of range.",num);
		return;
	}
#endif
	ga.goalCount=num;
	return(1);
};


void EXPORT SETTINGS::setPreprocessBuildOrder(int num)
{
	if(num==1)
		ga.preprocessBuildOrder=1;
	else
	if(num==0) 
		ga.preprocessBuildOrder=0;
#ifdef _SCC_DEBUG
	else 
	{
		debug.toLog(0,"WARNING: (SETTINGS::setPreprocessBuildOrder): Value [%i] out of range.",num);
		return;
	}
#endif
	return(1);
};

/*void EXPORT SETTINGS::setCurrentMap(int num)
{
#ifdef _SCC_DEBUG
	if((num<MIN_MAPS)||(num>getMapCount()))
	{
		debug.toLog(0,"WARNING: (SETTINGS::setCurrentMap): Value [%i] out of range.",num);
		return;
	}
#endif
	ga.currentMap=num;
	soup.setMap(&loadedMap[num]);

	return(1);
};*/

void EXPORT SETTINGS::adjustGoals(int player)
{
	start.adjustGoals(player);
};

void EXPORT SETTINGS::setStartRace(int player, int race)
{
	start.setPlayerRace(player, race);
};

int EXPORT SETTINGS::getUMS()
{
	return(UMS);
};

void EXPORT SETTINGS::setMap(int UMS, int num)
{
	if(num==-1)
		num=currentMap;
	else currentMap=num;
	if(UMS)
		start.assignMap(&(loadedMap[num]));
	else
		start.assignMap(&(loadedMap[num]),&defaults);
	soup.initializeMap(start.getMap());
	this->UMS=UMS;
};


void EXPORT SETTINGS::setGoal(int goal, int player)
{
#ifdef _SCC_DEBUG
	if((goal<MIN_GOAL_ENTRIES)||(goal>=getGoalCount())||(player<MIN_PLAYER)||(player>=MAX_PLAYER/*loadedMap[getCurrentMap()].getMaxPlayer()*/))
	{
		debug.toLog(0,"WARNING: (SETTINGS::setGoal): Value [%i/%i] out of range.",goal,player);
		return;
	}
#endif
	soup.setGoal(&loadedGoal[goal], player);
};

void EXPORT SETTINGS::checkForChange()
{
	soup.checkForChange();	
};

void EXPORT SETTINGS::setBreedFactor(int num)
{
#ifdef _SCC_DEBUG
	if((num<MIN_BREED_FACTOR)||(num>MAX_BREED_FACTOR))
	{
		debug.toLog(0,"WARNING: (SETTINGS::setBreedFactor): Value [%i] out of range.",num);
		return;
	}
#endif
	ga.setBreedFactor(num);
	return;
};

void EXPORT SETTINGS::setMode(int num)
{
#ifdef _SCC_DEBUG
	if((num<MIN_MODE)||(num>MAX_MODE))
	{
		debug.toLog(0,"WARNING: (SETTINGS::setMode): Value [%i] out of range.",num);
		return;
	}
#endif
	ga.mode=num;
	return;
};

void EXPORT SETTINGS::setCrossOver(int num)
{
#ifdef _SCC_DEBUG
	if((num<MIN_CROSSOVER)||(num>MAX_CROSSOVER))
	{
		debug.toLog(0,"WARNING: (SETTINGS::setCrossOver): Value [%i] out of range.",num);
		return;
	}
#endif
	ga.setCrossOver(num);
	return;
};

int EXPORT SETTINGS::getBreedFactor()
{
	return(ga.getBreedFactor());
};

int EXPORT SETTINGS::getMode()
{
	return(ga.mode);
};

int EXPORT SETTINGS::getCrossOver()
{
	return(ga.getCrossOver());
};

int EXPORT SETTINGS::getMaxTime()
{
	return(ga.maxTime);
};

int EXPORT SETTINGS::getMaxTimeOut()
{
	return(ga.maxTimeOut);
};

int EXPORT SETTINGS::getMaxLength()
{
	return(ga.maxLength);
};

int EXPORT SETTINGS::getMaxRuns()
{
	return(ga.maxRuns);
};

int EXPORT SETTINGS::getMaxGenerations()
{
	return(ga.maxGenerations);
};

int EXPORT SETTINGS::getPreprocessBuildOrder()
{
	return(ga.preprocessBuildOrder);
};

//int EXPORT SETTINGS::getCurrentMap()
//{
//	return(ga.currentMap);
//};

int EXPORT SETTINGS::getGoalCount()
{
	return(ga.goalCount);
};

int EXPORT SETTINGS::getMapCount()
{
	return(ga.mapCount);
};

int EXPORT SETTINGS::getHarvestMineralsSpeed(int race, int workers)
{
#ifdef _SCC_DEBUG
	if((race<0)||(race>=MAX_RACES))
	{
		debug.toLog(0,"WARNING: (SETTINGS::getHarvestMineralSpeed): Value race [%i] out of range.",race);
		return(0);
	}
#endif
	return(harvestSpeed[race].getHarvestMineralSpeed(workers));
};

int EXPORT SETTINGS::getHarvestGasSpeed(int race, int workers)
{
#ifdef _SCC_DEBUG
    if((race<0)||(race>=MAX_RACES))
    {
        debug.toLog(0,"WARNING: (SETTINGS::getHarvestGasSpeed): Value race [%i] out of range.",race);
        return(0);
    }
#endif
	return(harvestSpeed[race].getHarvestGasSpeed(workers));
};


/*int EXPORT SETTINGS::getDistance(int l1,int l2)
{
	if((l1<MIN_LOCATIONS)||(l1>=MAX_LOCATIONS))
		return(0);
	return(loadedMap[ga.currentMap].getDistance(l1,l2));
};*/

GOAL_ENTRY* EXPORT SETTINGS::getGoal(int num)
{
#ifdef _SCC_DEBUG
    if((num<MIN_GOAL_ENTRIES)||(num>=getGoalCount()))
    {
        debug.toLog(0,"WARNING: (SETTINGS::getGoal): Value num [%i] out of range.",num);
        return(0);
    }
#endif
	return(&loadedGoal[num]);
};

void EXPORT SETTINGS::loadDefaultsFile(string& defaultFile)
{
	defaults.loadFromFile(defaultFile, harvestSpeed);
};


//=> in race schreiben! :-) Bzw. in settings nur die Datei an sich laden, in ein array speichern und dann zur Auswahl geben
// zu dem player[4] Problem... evtl pointer auf 4 Klassen machen...
void EXPORT SETTINGS::loadGoalFile(string& goalFile) //~~~
{
	char line[1024];
	string<list> parameter;
	int<list>value;
	int ln=0;
	int mode=0;
    ifstream pFile(goalFile.c_str());
    if(!pFile.is_open())
    {
        debug.toLog(0,"ERROR: (UI_Theme::loadGoalFile) %s: Could not open file!",goalFile.c_str());
        return;
    }
	//"unit name" "count" "time" "location"
    while(pFile.getline(line, sizeof line))
    {
        if(pFile.fail())
        {
            debug.toLog(0,"WARNING: (UI_Theme::loadGoalFiles) %s: Long line!",goalFile.c_str());
            pFile.clear(pFile.rdstate() & ~ios::failbit);
        };
		++ln;
		if((line[0]=='#')||(line[0]=='\0')) continue;

		parse(line, parameter, value); // evtl immer 2 zwischenraeume... naj aerstmal testen..

//		if((value1<0)||(value2<0)||(value3<0))
//		{
//			debug.toLog(0,"WARNING: (SETTINGS::loadGoalFile) %s: Line %d [%s]: Value below zero.",goalFile,ln,old);
//			continue;
//		}
		
		if(mode==0)
		{
			if(parameter[0]=="@GOAL")
				mode=1;
			//else ...
		}
		else if(mode==1)
		{
			switch(parameter[0])
			{
			if(parameter[0]=="Name")
				loadedGoal[getGoalCount()].setName(param);
			else if(parameter[0]=="@RACE"))
			{
				mode=2;
				if(!strcmp(param1,"Terra")) loadedGoal[getGoalCount()].setRace(TERRA);
				else if(!strcmp(param1,"Protoss")) loadedGoal[getGoalCount()].setRace(PROTOSS);
				else if(!strcmp(param1,"Zerg")) loadedGoal[getGoalCount()].setRace(ZERG);
				else
				{
					debug.toLog(0,"ERROR: (SETTINGS::loadGoalFile) %s: Line %d [%s]: Invalid race.",goalFile,ln,old);
					return;
				}
			}
			else if(parameter[0]=="@END"))
				mode=0;
			//else...
			
		}
		else if(mode==2)
		{
		/*	if(!loadedGoal[getGoalCount()].isRaceInitialized())
			{
				debug.toLog(0,"ERROR: (SETTINGS::loadGoalFile) %s: Line %d [%s]: @RACE must be the first line.",goalFile,ln,old);
				return(0);
			}
		else
			{*/
			if(parameter[0]=="@END"))
				mode=1;
			else
			{
//Aufbau der goal datei: "Einheitname" LEER "Anzahl" LEER "Zeit" LEER "Ort"
				int i;
				for(i=0;i<REFINERY;i++) 
					if((strstr(stats[loadedGoal[getGoalCount()].getRace()][i].name,item)!=NULL)&&(value2<=getMaxTime())&&(value3<=MAX_LOCATIONS))
					{
						//TODO: values checken!
			 			//TODO: evtl statt Ortsnummer einfach den Ortsnamen nehmen
						if(!loadedGoal[getGoalCount()].addGoal(i,value1,60*value2,value3))
							debug.toLog(0,"WARNING: (SETTINGS::loadGoalFile) %s: Line %d [%s]: Problems with adding goal.",goalFile,ln,old);
						i=REFINERY+1;
						break;
					}
				if(i!=REFINERY+1)
					debug.toLog(0,"WARNING: (SETTINGS::loadGoalFile) %s: Line %d [%s]: No unit name matched this goal.",goalFile,ln,old);
			}
		}
	} //end while
//missing @END ??
	loadedGoal[getGoalCount()].adjustGoals(ga.allowGoalAdaption);
	setGoalCount(getGoalCount()+1);
	return(1);
};


void SETTINGS::loadSettingsFile(string& settingsFile)
{
    ifstream pFile(settingsFile.c_str());
    char line[1024];
    string text;
    while(pFile.getline(line, sizeof line))
    {
        if(pFile.fail())
            pFile.clear(pFile.rdstate() & ~ios::failbit);
        text=line;
        int start=text.find_first_not_of("\9 ");
        if ((start >= 0) && (start <= n))
            if((text[0]=='#')||(text[0]=='\0')) continue; // ignore line
        string index=text.substr(start);
		string value;
		if(index=="@STRINGS")
		{
                map<string, list<string>> block;
                parse_block(pFile, block);
				if(value=block["Allow goal adaption"].front()) setAllowGoalAdaption(atoi(value.c_str()));
				if(value=block["Max Time"].front()) setMaxTime(atoi(value.c_str()));
				if(value=block["Max Timeout"].front()) setMaxTimeOut(atoi(value.c_str()));
				if(value=block["Max Length"].front()) setMaxLength(atoi(value.c_str()));
				if(value=block["Max Runs"].front()) setMaxRuns(atoi(value.c_str()));
				if(value=block["Preprocess Buildorder"].front()) setPreprocessBuildOrder(atoi(value.c_str()));
				if(value=block["Mode"].front()) setMode(atoi(value.c_str()));
				if(value=block["Breed Factor"].front()) setBreedFactor(atoi(value.c_str()));
				if(value=block["Crossing Over"].front()) setCrossOver(atoi(value.c_str()));
				if(value=block["Max unchanged Generations"].front()) setMaxGenerations(atoi(value.c_str()));
		};
	}// END while
}; // schoen :)

void SETTINGS::loadSettingsFile(string& harvestFile)
{
    ifstream pFile(harvestFile.c_str());
    char line[1024];
    string text;
    while(pFile.getline(line, sizeof line))
    {
        if(pFile.fail())
            pFile.clear(pFile.rdstate() & ~ios::failbit);
        text=line;
        int start=text.find_first_not_of("\9 ");
        if ((start >= 0) && (start <= n))
            if((text[0]=='#')||(text[0]=='\0')) continue; // ignore line
        string index=text.substr(start);
        list<string> item;
        if(index=="@HARVESTDATA")
        {
                map<string, map<string, list<string>>> block;
                parse_2nd_block(pFile, block);
				map<string, list<string>> player;
				if(player=block["@TERRA"])
				{
					if(item=player["Mineral Harvest"])
					{
						int j=0;
						for(list<string>::const_iterator i=item.begin();i!=item.end();++i)
	                        harvestSpeed[0].setHarvestMineralSpeed(j++,atoi(*i));
					}
					if(item=player["Gas Harvest"])
					{
						int j=0;
						for(list<string>::const_iterator i=item.begin();i!=item.end();++i)
	                        harvestSpeed[0].setHarvestGasSpeed(j++,atoi(*i));
					}
				};
				if(player=block["@PROTOSS"])
				{
					if(item=player["Mineral Harvest"])
					{
						int j=0;
						for(list<string>::const_iterator i=item.begin();i!=item.end();++i)
	                        harvestSpeed[1].setHarvestMineralSpeed(j++,atoi(*i));
					}
					if(item=player["Gas Harvest"])
					{
						int j=0;
						for(list<string>::const_iterator i=item.begin();i!=item.end();++i)
	                        harvestSpeed[1].setHarvestGasSpeed(j++,atoi(*i));
					}
				};
				if(player=block["@ZERG"])
				{
					if(item=player["Mineral Harvest"])
					{
						int j=0;
						for(list<string>::const_iterator i=item.begin();i!=item.end();++i)
	                        harvestSpeed[2].setHarvestMineralSpeed(j++,atoi(*i));
					}
					if(item=player["Gas Harvest"])
					{
						int j=0;
						for(list<string>::const_iterator i=item.begin();i!=item.end();++i)
	                        harvestSpeed[2].setHarvestGasSpeed(j++,atoi(*i));
					}
				};
        };
    }// END while
}; // schoen :)



int EXPORT SETTINGS::loadMapFile(string& mapFile)
{
	char line[1024],old[1024];
	FILE* pFile;
	char* buffer;
	char item[1024],param1[1024],param2[1024],param3[1024];
	int ln=0;
	int value1=0,value2=0,value3=0;
	int mode=0,modeLocation=0,modePlayer=-1;
	int playerCount=0;
//	sprintf(buffer,"maps/%s.map",mapFile);
	if((pFile = fopen ("maps/LostTemple.map","r"))==NULL)
	{
		debug.toLog(0,"ERROR: (SETTINGS::loadMapFile) %s: Could not open file!",mapFile);
		return; 
	}
//todo: maps in ne linked list oder so...
//allgemeine linked list schreiben ... etc. etc.
	loadedMap[getMapCount()].resetForce();
	item[0]='\0';param1[0]='\0';param2[0]='\0';param3[0]='\0';

	int distanceCount=0;
	
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
			debug.toLog(0,"WARNING: (SETTINGS::loadMapFile) %s: Line %d [%s]: Too many parameters.",mapFile,ln,old);
			continue;
		}

		if(((value1=atoi(param1))<0)||((value2=atoi(param2))<0)||((value3=atoi(param3))<0))
		{
			debug.toLog(0,"WARNING: (SETTINGS::loadMapFile) %s: Line %d [%s]: Value below zero.",mapFile,ln,old);
			continue;
		}

		if(mode==0) //Out of any block
		{
			if(parameter[0]=="@MAP")) 
				mode=1;
			else if(parameter[0]=="@LOCATION"))
			{
				if((value1>0)&&(value1<=MAX_LOCATIONS)) 
				{
					mode=2;
					modeLocation=value1;
				}
				else debug.toLog(0,"WARNING: (SETTINGS::loadMapFile) %s: Line %d [%s]: Location out of range.",mapFile,ln,old);
			}
			else if(parameter[0]=="@PLAYER"))
			{
				if((value1>=0)&&(value1<=loadedMap[getMapCount()].getMaxPlayer()))
				{
					mode=3;
					modePlayer=value1;
				}
				else debug.toLog(0,"WARNING: (SETTINGS::loadMapFile) %s: Line %d [%s]: Player is out of range.",mapFile,ln,old);
			}
			else if(parameter[0]=="@END"))
				debug.toLog(0,"WARNING: (SETTINGS::loadMapFile) %s: Line %d [%s]: Lonely @END.",mapFile,ln,old);
			else 
				debug.toLog(0,"WARNING: (SETTINGS::loadMapFile) %s: Line %d [%s]: Line is outside a block but is not marked as comment.",mapFile,ln,old);
		
		}
		else if(mode>0)		
		{
			if(mode==1) // MAP Block
			{
				if(parameter[0]=="Name"))
				{
					if(!loadedMap[getMapCount()].setName(param1))
						debug.toLog(0,"ERROR: (SETTINGS::loadMapFile) %s: Line %d [%s]: Name not set!",mapFile,ln,old);
				}
				else if(parameter[0]=="Max Locations"))
				{
					if(!loadedMap[getMapCount()].setMaxLocations(value1))
						debug.toLog(0,"WARNING: (SETTINGS::loadMapFile) %s: Line %d [%s]: Max Locations out of range.",mapFile,ln,old);
				}
				else if(parameter[0]=="Max Players"))
				{
					if(!loadedMap[getMapCount()].setMaxPlayer(value1))
						debug.toLog(0,"WARNING: (SETTINGS::loadMapFile) %s: Line %d [%s]: Max Player out of range.",mapFile,ln,old);
				}
				else if(parameter[0]=="@END"))
					mode=0;
				else 
					debug.toLog(0,"WARNING: (SETTINGS::loadMapFile) %s: Line %d [%s]: Unknown entry in MAP Block.",mapFile,ln,old);
			}
			else if(mode==2) // LOCATION Block
			{
				if(parameter[0]=="Name"))
				{
					if(!loadedMap[getMapCount()].setLocationName(modeLocation-1,param1))
						debug.toLog(0,"WARNING: (SETTINGS::loadMapFile) %s: Line %d [%s]: Name invalid.",mapFile,ln,old);
				}
				else if(parameter[0]=="Mineral Distance"))
				{
					if(!loadedMap[getMapCount()].setMineralDistance(modeLocation-1,value1))
						debug.toLog(0,"WARNING: (SETTINGS::loadMapFile) %s: Line %d [%s]: Mineral Distance out of range.",mapFile,ln,old);
				}
				else if(parameter[0]=="Distances"))
				{
					//~~ wenn maxLocations noch nicht initialisiert...
					distanceCount=0;
					buffer=strtok(param1," ");
					while((buffer!=NULL)&&(distanceCount<loadedMap[getMapCount()].getMaxLocations()))
					{
						if(!loadedMap[getMapCount()].setDistance(modeLocation-1,distanceCount++,(value1=atoi(buffer))) )
						{
							debug.toLog(0,"WARNING: (SETTINGS::loadMapFile) %s: Line %d [%s]: Values out of range.",mapFile,ln,old);
							break;
						}
						buffer=strtok(NULL," ");
					}
			
					if(distanceCount<loadedMap[getMapCount()].getMaxLocations())
					{
						debug.toLog(0,"WARNING: (SETTINGS::loadMapFile) %s: Line %d [%s]: Missing entries",mapFile,ln,old);
						for(;distanceCount<loadedMap[getMapCount()].getMaxLocations();distanceCount++)
							loadedMap[getMapCount()].setDistance(modeLocation-1,distanceCount,0);
					}
				}
				else if(parameter[0]=="@PLAYER"))
				{
					if((value1>=0)&&(value1<=loadedMap[getMapCount()].getMaxPlayer()))
					{
						mode=4;
						modePlayer=value1;
					}
					else 
						debug.toLog(0,"WARNING: (SETTINGS::loadMapFile) %s: Line %d [%s]: Value out of range.",mapFile,ln,old);
				}
				else if(parameter[0]=="@END"))
					mode=0;
				else 
					debug.toLog(0,"WARNING: (SETTINGS::loadMapFile) %s: Line %d [%s]: Unknown entry in LOCATION Block.",mapFile,ln,old);
			} //end LOCATION Block
			else if(mode==3) //PLAYER Block
			{
				if(parameter[0]=="Race"))
				{
					if(!strcmp(param1,"Terra")) 
					{
						loadedMap[getMapCount()].setStartPlayerRace(modePlayer,TERRA);
						loadedMap[getMapCount()].setStartPlayerHarvestSpeed(modePlayer,&harvestSpeed[TERRA].minerals[0],&harvestSpeed[TERRA].gas[0]);
					}
					else if(!strcmp(param1,"Protoss")) 
					{
						loadedMap[getMapCount()].setStartPlayerRace(modePlayer,PROTOSS);
						loadedMap[getMapCount()].setStartPlayerHarvestSpeed(modePlayer,&harvestSpeed[PROTOSS].minerals[0],&harvestSpeed[PROTOSS].gas[0]);
					}
					else if(!strcmp(param1,"Zerg")) 
					{
						loadedMap[getMapCount()].setStartPlayerRace(modePlayer,ZERG);
						loadedMap[getMapCount()].setStartPlayerHarvestSpeed(modePlayer,&harvestSpeed[ZERG].minerals[0],&harvestSpeed[ZERG].gas[0]);
					}
					else 
						debug.toLog(0,"WARNING: (SETTINGS::loadMapFile) %s: Line %d [%s]: Invalid race.",mapFile,ln,old);
				}
				else if(parameter[0]=="Starting Point"))
				{
					if(!loadedMap[getMapCount()].setStartPlayerPosition(modePlayer,value1-1))
						debug.toLog(0,"WARNING: (SETTINGS::loadMapFile) %s: Line %d [%s]: Starting Point out of range.",mapFile,ln,old);
				}
				else if(parameter[0]=="Starting Minerals"))
				{
					if(!loadedMap[getMapCount()].setStartPlayerMins(modePlayer,value1*100))
						debug.toLog(0,"WARNING: (SETTINGS::loadMapFile) %s: Line %d [%s]: Starting Minerals out of range.",mapFile,ln,old);
				}
				else if(parameter[0]=="Starting Gas"))
				{
					if(!loadedMap[getMapCount()].setStartPlayerGas(modePlayer,value1*100))
						debug.toLog(0,"WARNING: (SETTINGS::loadMapFile) %s: Line %d [%s]: Starting Gas out of range.",mapFile,ln,old);
				}
				else if(parameter[0]=="Starttime"))
				{
					if(!loadedMap[getMapCount()].setStartPlayerTime(modePlayer,value1))
						debug.toLog(0,"WARNING: (SETTINGS::loadMapFile) %s: Line %d [%s]: Start time out of range.",mapFile,ln,old);
				}
				else if(parameter[0]=="@END"))
				{
					playerCount++;
					mode=0;
				}
				else
					debug.toLog(0,"WARNING: (SETTINGS::loadMapFile) %s: Line %d [%s]: Unknown entry in PLAYER Block.",mapFile,ln,old);
			}
			else if(mode==4) // PLAYER Block in LOCATION Block
			{
				int i=0;
				if(parameter[0]=="@END"))
					mode=2;
				else 
				{
					for(i=0;i<UNIT_TYPE_COUNT;i++)  // TODO reicht eigentlich nur bis GAS SCV... aber dann auch unten die abbruchbedingung anguggn
						if(strstr(stats[loadedMap[getMapCount()].getStartPlayerRace(modePlayer)][i].name,item)!=NULL)
						{
							loadedMap[getMapCount()].setLocationForce(0,modePlayer,i,value1);
							loadedMap[getMapCount()].setLocationForce(modeLocation-1,modePlayer,i,value1);
							loadedMap[getMapCount()].setLocationAvailible(0,modePlayer,i,value1);
							loadedMap[getMapCount()].setLocationAvailible(modeLocation-1,modePlayer,i,value1);
							i=UNIT_TYPE_COUNT;
							break;
						}
					if(i!=UNIT_TYPE_COUNT) debug.toLog(0,"WARNING: (SETTINGS::loadMapFile) %s: Line %d [%s]: No matching unit name.",mapFile,ln,item);
				}
			}
			else
				debug.toLog(0,"WARNING: (SETTINGS::loadMapFile) %s: Line %d [%s]: Unknown entry in PLAYER Block in LOCATION Block.",mapFile,ln,item);

		}// END if(mode>0)
	}// END while
	if(loadedMap[getMapCount()].getMaxPlayer()>playerCount+1)
	{
		debug.toLog(0,"ERROR: (SETTINGS::loadMapFile) %s: 'Max players' declared as %i but only %i (incl. neutral player) players found.",mapFile,loadedMap[getMapCount()].getMaxPlayer(),playerCount+1);
		return;
	}
//	if(UMS==0)
//	{
//		for(int i=1;i<loadedMap[getMapCount()].getMaxPlayer();i++)
//		loadedMap[getMapCount()].resetForTVBRace(i,loadedMap[getPlayerCount()].getRace(i));
//	}
//TODO	
	loadedMap[getMapCount()].adjustSupply();
	loadedMap[getMapCount()].adjustResearches(); // temporary units, 3 for upgrades, 1 for researchs etc.
	loadedMap[getMapCount()].adjustDistanceList();
	//TODO goals
	setMapCount(getMapCount()+1);
	return(1);
};

GA* EXPORT SETTINGS::getGa()
{
	return(&ga);
};

void EXPORT SETTINGS::initializeMap()
{
	soup.initializeMap(start.getMap());
};

int EXPORT SETTINGS::initSoup()
{
	int tmp=0;
	// set GA and START on prerace and soup
	if((tmp=soup.setParameters(&ga, &start))!=1) 
		debug.toLog(0,"error1");//return(100-tmp);
	// allocate memory for players ~~
//	if((tmp=soup.initSoup())!=1) 
//		debug.toLog(0,"error2");//return(200-tmp);
	return(1);
}


ANARACE** EXPORT SETTINGS::newGeneration(ANARACE* oldAnarace[MAX_PLAYER])
{
	return(soup.newGeneration(oldAnarace));
};


SETTINGS settings;

