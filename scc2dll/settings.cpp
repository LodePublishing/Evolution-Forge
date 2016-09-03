#include "settings.h"
#include "debug.h"
#include <time.h>
#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <deque>

using namespace std;
typedef map<string, list<string> > stblock;
typedef map<string, map<string, list<string> > > ndblock;


void parse_line(string& text, list<string>& words)
{
    size_t n = text.length();
    size_t stop;
    size_t start = text.find_first_not_of(",\t\" =");
    while (start!=string::npos) 
	{
       stop = text.find_first_of(",\t\n\" =\0", start);
	   if(stop==string::npos)
		   stop = n;
	   string bla=text.substr(start, stop - start);
       words.push_back(bla);
	   start = text.find_first_not_of(",\t\n\" =", stop+1);
    }
};


void parse_block(ifstream& stream, stblock& block)
{
	char line[1024];
	string text;
    while(stream.getline(line, sizeof line))
    {
        if(stream.fail())
            stream.clear(stream.rdstate() & ~ios::failbit);
		text=line;
		if(text.find("@END")!=string::npos) return;
		size_t start=text.find_first_not_of("\t\" ");
		if((start==string::npos)||(text[0]=='#')||(text[0]=='\0')) 
			continue; // ignore line
		
		list<string> words;
        parse_line(text, words);
		string bla=words.front();
//		string bla=text.substr(start);
		block.insert(pair<string, list<string> >(bla, words));
	};
};

void parse_2nd_block(ifstream& stream, ndblock& block)
{
    char line[1024];
    string text;
    while(stream.getline(line, sizeof line))
    {
        if(stream.fail())
            stream.clear(stream.rdstate() & ~ios::failbit);
        text=line;
		if(text.find("@END")!=string::npos) return;
        size_t start=text.find_first_not_of("\t ");
		if((start==string::npos)||(text[0]=='#')||(text[0]=='\0')) 
			continue; // ignore line
        map<string, list<string> > words;
        parse_block(stream, words);
        block.insert(pair<string, map<string, list<string> > > (text.substr(start), words));
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
	currentMap=-1;
	setAllowGoalAdaption(true);
	setMaxTime(MAX_TIME);
	setMaxTimeOut(MAX_TIMEOUT);
	setMaxLength(MAX_LENGTH);
	setMaxRuns(MAX_RUNS);
	setMaxGenerations(MAX_GENERATIONS);
	setPreprocessBuildOrder(false);
	setCrossOver(MIN_CROSSOVER);
	setBreedFactor(20);
	setMode(MIN_MODE);
};

void EXPORT SETTINGS::calculateAnaplayer()
{
	soup.calculateAnaplayer();
};

const BASIC_MAP* EXPORT SETTINGS::getMap(const int mapNumber) const
{
#ifdef _SCC_DEBUG
	if((mapNumber>getMapCount())||(mapNumber<0))
	{
		toLog("WARNING: (SETTINGS::getMap): Value out of range.");
		return(0);
	}
#endif
	return(&loadedMap[mapNumber]);
};

const int EXPORT SETTINGS::getMAXBreedFactor() const
{
	return(MAX_BREED_FACTOR);
};

const int EXPORT SETTINGS::getMAXMode() const
{
	return(MAX_MODE);
};

const int EXPORT SETTINGS::getMAXCrossOver() const
{
	return(MAX_CROSSOVER);
};

const int EXPORT SETTINGS::getMAXMaxTime() const
{
	return(MAX_TIME);
};

const int EXPORT SETTINGS::getMAXMaxTimeOut() const
{
	return(MAX_TIMEOUT);
};

const int EXPORT SETTINGS::getMAXMaxLength() const
{
	return(MAX_LENGTH);
};

const int EXPORT SETTINGS::getMAXMaxRuns() const
{
	return(MAX_RUNS);
};

const int EXPORT SETTINGS::getMAXMaxGenerations() const
{
	return(MAX_GENERATIONS);
};

const int EXPORT SETTINGS::getMAXCurrentMap() const
{
	return(MAX_MAPS);
};

const int EXPORT SETTINGS::getMINBreedFactor() const
{
	return(MIN_BREED_FACTOR);
};

const int EXPORT SETTINGS::getMINMode() const
{
	return(MIN_MODE);
};

const int EXPORT SETTINGS::getMINCrossOver() const
{
	return(MIN_CROSSOVER);
};

const int EXPORT SETTINGS::getMINMaxTime() const
{
	return(MIN_TIME);
};

const int EXPORT SETTINGS::getMINMaxTimeOut() const
{
	return(MIN_TIMEOUT);
};

const int EXPORT SETTINGS::getMINMaxLength() const
{
	return(MIN_LENGTH);
};

const int EXPORT SETTINGS::getMINMaxRuns() const
{
	return(MIN_RUNS);
};

const int EXPORT SETTINGS::getMINMaxGenerations() const
{
	return(MIN_GENERATIONS);
};

const int EXPORT SETTINGS::getMINCurrentMap() const
{
	return(MIN_MAPS);
};

										                                                                                                                    
void EXPORT SETTINGS::setAllowGoalAdaption(const bool allowGoalAdaption) // allow the program to change goals (for example ignore command center when command center [NS] is already a goal
{
	ga.allowGoalAdaption=allowGoalAdaption;
};


void EXPORT SETTINGS::setMaxTime(const int maxTime) //maximum time of build order in seconds
{
#ifdef _SCC_DEBUG
	if((maxTime<MIN_TIME)||(maxTime>MAX_TIME)) 
	{
		toLog("WARNING: (SETTINGS::setMaxTime): Value out of range.");
		return;
	}
#endif
	ga.maxTime=maxTime;
};

void EXPORT SETTINGS::setMaxTimeOut(const int maxTimeOut) //timeout for building
{
#ifdef _SCC_DEBUG
	if((maxTimeOut<MIN_TIMEOUT)||(maxTimeOut>MAX_TIMEOUT)) 
	{
		toLog("WARNING: (SETTINGS::setMaxTimeOut): Value out of range.");
		return;
	}
#endif
	ga.maxTimeOut=maxTimeOut;
};

void EXPORT SETTINGS::setMaxLength(const int maxLength)
{
#ifdef _SCC_DEBUG
	if((maxLength<MIN_LENGTH)||(maxLength>MAX_LENGTH)) 
	{
		toLog("WARNING: (SETTINGS::setMaxLength): Value out of range.");
		return;
	}
#endif
	ga.maxLength=maxLength;
};

void EXPORT SETTINGS::setMaxRuns(const int maxRuns)
{
#ifdef _SCC_DEBUG
	if((maxRuns<MIN_RUNS)||(maxRuns>MAX_RUNS))
	{
		toLog("WARNING: (SETTINGS::setMaxRuns): Value out of range.");
		return;
	}
#endif
	ga.maxRuns=maxRuns;
};

void EXPORT SETTINGS::setMaxGenerations(const int maxGenerations)
{
#ifdef _SCC_DEBUG
	if((maxGenerations<MIN_GENERATIONS)||(maxGenerations>MAX_GENERATIONS))
	{
		toLog("WARNING: (SETTINGS::setMaxGenerations): Value out of range.");
		return;
	}
#endif
	ga.maxGenerations=maxGenerations;
};

void SETTINGS::setMapCount(const int mapCount)
{
#ifdef _SCC_DEBUG
	if((mapCount<MIN_MAPS)||(mapCount>=MAX_MAPS))
	{
		toLog("WARNING: (SETTINGS::setMapCount): Value out of range.");
		return;
	}
#endif
	ga.mapCount=mapCount;
};

void SETTINGS::setStartconditionCount(const int startconditionCount)
{
#ifdef _SCC_DEBUG
	if((startconditionCount<MIN_GOAL_ENTRIES)||(startconditionCount>=MAX_GOAL_ENTRIES))
	{
		toLog("WARNING: (SETTINGS::setStartconditionCount): Value out of range.");
		return;
	}
#endif
	ga.startconditionCount=startconditionCount;
};

void SETTINGS::setGoalCount(const int goalCount)
{
#ifdef _SCC_DEBUG
	if((goalCount<MIN_GOAL_ENTRIES)||(goalCount>=MAX_GOAL_ENTRIES))
	{
		toLog("WARNING: (SETTINGS::setGoalCount): Value out of range.");
		return;
	}
#endif
	ga.goalCount=goalCount;
};


void EXPORT SETTINGS::setPreprocessBuildOrder(const bool preprocess)
{
	ga.preprocessBuildOrder=preprocess;
};

/*void EXPORT SETTINGS::setCurrentMap(int num)
{
#ifdef _SCC_DEBUG
	if((num<MIN_MAPS)||(num>getMapCount()))
	{
		toLog("WARNING: (SETTINGS::setCurrentMap): Value [%i] out of range.",num);
		return;
	}
#endif
	ga.currentMap=num;
	soup.setMap(&loadedMap[num]);

	return(1);
};*/

//void EXPORT SETTINGS::setStartRace(const int player, const eRace race)
//{
//	start.setPlayerRace(player, race);
//};

void EXPORT SETTINGS::setMap(const int mapNumber)
{
#ifdef _SCC_DEBUG
    if((mapNumber<0)||(mapNumber>=MAX_MAPS))
    {
        toLog("WARNING: (SETTINGS::setMap): Value out of range.");
        return;
    }
#endif
	start.assignMap(&(loadedMap[mapNumber]));
//	soup.initializeMap(&(loadedMap[mapNumber])); //?
};


void EXPORT SETTINGS::setGoal(const int goal, const int player)
{
#ifdef _SCC_DEBUG
	if((goal<MIN_GOAL_ENTRIES)||(goal>=getGoalCount())||(player<MIN_PLAYER)||(player>=MAX_PLAYER/*loadedMap[getCurrentMap()].getMaxPlayer()*/))
	{
		toLog("WARNING: (SETTINGS::setGoal): Value out of range.");
		return;
	}
#endif
	soup.setGoal(&loadedGoal[goal], player);
};

void EXPORT SETTINGS::checkForChange() const
{
	soup.checkForChange();	
};

void EXPORT SETTINGS::setBreedFactor(const int breedFactor)
{
#ifdef _SCC_DEBUG
	if((breedFactor<MIN_BREED_FACTOR)||(breedFactor>MAX_BREED_FACTOR))
	{
		toLog("WARNING: (SETTINGS::setBreedFactor): Value out of range.");
		return;
	}
#endif
	ga.setBreedFactor(breedFactor);
};

void EXPORT SETTINGS::setMode(const int mode)
{
#ifdef _SCC_DEBUG
	if((mode<MIN_MODE)||(mode>MAX_MODE))
	{
		toLog("WARNING: (SETTINGS::setMode): Value out of range.");
		return;
	}
#endif
	ga.mode=mode;
};

void EXPORT SETTINGS::setCrossOver(const int crossOver)
{
#ifdef _SCC_DEBUG
	if((crossOver<MIN_CROSSOVER)||(crossOver>MAX_CROSSOVER))
	{
		toLog("WARNING: (SETTINGS::setCrossOver): Value out of range.");
		return;
	}
#endif
	ga.setCrossOver(crossOver);
};

const int EXPORT SETTINGS::getBreedFactor() const
{
	return(ga.getBreedFactor());
};

const int EXPORT SETTINGS::getMode() const
{
	return(ga.mode);
};

const int EXPORT SETTINGS::getCrossOver() const
{
	return(ga.getCrossOver());
};

const int EXPORT SETTINGS::getMaxTime() const
{
	return(ga.maxTime);
};

const int EXPORT SETTINGS::getMaxTimeOut() const
{
	return(ga.maxTimeOut);
};

const int EXPORT SETTINGS::getMaxLength() const
{
	return(ga.maxLength);
};

const int EXPORT SETTINGS::getMaxRuns() const
{
	return(ga.maxRuns);
};

const int EXPORT SETTINGS::getMaxGenerations() const
{
	return(ga.maxGenerations);
};

const bool EXPORT SETTINGS::getPreprocessBuildOrder() const
{
	return(ga.preprocessBuildOrder);
};

//int EXPORT SETTINGS::getCurrentMap()
//{
//	return(ga.currentMap);
//};
const int EXPORT SETTINGS::getStartconditionCount() const
{
	return(ga.startconditionCount);
};

const int EXPORT SETTINGS::getGoalCount() const
{
	return(ga.goalCount);
};

const int EXPORT SETTINGS::getMapCount() const
{
	return(ga.mapCount);
};

const int EXPORT SETTINGS::getHarvestMineralsSpeed(const eRace race, const int workers) const
{
	return(harvestSpeed[race].getHarvestMineralSpeed(workers));
};

const int EXPORT SETTINGS::getHarvestGasSpeed(const eRace race, const int workers) const
{
	return(harvestSpeed[race].getHarvestGasSpeed(workers));
};


/*int EXPORT SETTINGS::getDistance(int l1,int l2)
{
	if((l1<MIN_LOCATIONS)||(l1>=MAX_LOCATIONS))
		return(0);
	return(loadedMap[ga.currentMap].getDistance(l1,l2));
};*/

const GOAL_ENTRY* EXPORT SETTINGS::getGoal(const int goalNumber) const
{
#ifdef _SCC_DEBUG
    if((goalNumber<MIN_GOAL_ENTRIES)||(goalNumber>=getGoalCount()))
    {
        toLog("WARNING: (SETTINGS::getGoal): Value out of range.");
        return(0);
    }
#endif
	return(&loadedGoal[goalNumber]);
};

//void EXPORT SETTINGS::loadDefaultsFile(const string& defaultFile)
//{
//	defaults.loadFromFile(defaultFile, harvestSpeed);
//};

void SETTINGS::loadGoalFile(const string& goalFile)
{
    ifstream pFile(goalFile.c_str());
    char line[1024];
    string text;
    while(pFile.getline(line, sizeof line))
    {
        if(pFile.fail())
            pFile.clear(pFile.rdstate() & ~ios::failbit);
        text=line;
        size_t start=text.find_first_not_of("\t ");
        if((start==string::npos)||(text[0]=='#')||(text[0]=='\0'))
	            continue; // ignore line
        size_t stop=text.find_first_of("\t ",start);
        if(stop==string::npos) stop=0;
        string index=text.substr(start, stop);
		map<string, list<string> >::iterator i;
        if(index=="@GOAL")
        {
                map<string, list<string> > block;
                parse_block(pFile, block);
                if((i=block.find("Name"))!=block.end()) 
					loadedGoal[getGoalCount()].setName(i->second.front());
                if((i=block.find("Race"))!=block.end()) 
				{
					eRace race;
					i->second.pop_front();
					string estr=i->second.front();
	                if(i->second.front()=="Terra") race=TERRA;
    	            else if(i->second.front()=="Protoss") race=PROTOSS;
        	        else if(i->second.front()=="Zerg") race=ZERG;
					loadedGoal[getGoalCount()].setRace(race);
				}
                int j;
				map<string, list<string> >::iterator k;
                for(j=0;j<UNIT_TYPE_COUNT;j++)
				{
					string unit=stats[loadedGoal[getGoalCount()].getRace()][j].name;
					if((k=block.find(unit))!=block.end())
                    {
						list<string>::iterator l=k->second.begin();
						if(l->size()<3) break;
						l++;int time=atoi(l->c_str());
						l++;int loc=atoi(l->c_str());
						l++;int count=atoi(l->c_str());
						if((time<=getMaxTime())&&(loc<=MAX_LOCATIONS))
						{
	                        //TODO: values checken!
    	                    //TODO: evtl statt Ortsnummer einfach den Ortsnamen nehmen
        	                loadedGoal[getGoalCount()].addGoal(j, loc, 60*time, count);
                	        j=UNIT_TYPE_COUNT+1;
                    	    break;
	                    }
					}
				}
   			    if(j!=UNIT_TYPE_COUNT+1)
                    toLog("WARNING: (SETTINGS::loadGoalFile) No unit name matched this goal.");
		}

    };
    loadedGoal[getGoalCount()].adjustGoals(ga.allowGoalAdaption);
    setGoalCount(getGoalCount()+1);
}; // schoen :)

void SETTINGS::loadSettingsFile(const string& settingsFile)
{
    ifstream pFile(settingsFile.c_str());
    char line[1024];
    string text;
    while(pFile.getline(line, sizeof line))
    {
        if(pFile.fail())
            pFile.clear(pFile.rdstate() & ~ios::failbit);
        text=line;
        size_t start=text.find_first_not_of("\t ");
        if((start==string::npos)||(text[0]=='#')||(text[0]=='\0'))
	            continue; // ignore line
        size_t stop=text.find_first_of("\t ", start);
        if(stop==string::npos) stop=0;
        string index=text.substr(start, stop);
		string value;
	    map<string, list<string> >::iterator i;
		if(index=="@STRINGS")
		{
                map<string, list<string> > block;
                parse_block(pFile, block);
				if((i=block.find("Allow goal adaption"))!=block.end()) setAllowGoalAdaption(atoi(i->second.front().c_str()));
				if((i=block.find("Max Time"))!=block.end()) setMaxTime(atoi(i->second.front().c_str()));
				if((i=block.find("Max Timeout"))!=block.end()) setMaxTimeOut(atoi(i->second.front().c_str()));
				if((i=block.find("Max Length"))!=block.end()) setMaxLength(atoi(i->second.front().c_str()));
				if((i=block.find("Max Runs"))!=block.end()) setMaxRuns(atoi(i->second.front().c_str()));
				if((i=block.find("Preprocess Buildorder"))!=block.end()) setPreprocessBuildOrder(atoi(i->second.front().c_str()));
				if((i=block.find("Mode"))!=block.end()) setMode(atoi(i->second.front().c_str()));
				if((i=block.find("Breed Factor"))!=block.end()) setBreedFactor(atoi(i->second.front().c_str()));
				if((i=block.find("Crossing Over"))!=block.end()) setCrossOver(atoi(i->second.front().c_str()));
				if((i=block.find("Max unchanged Generations"))!=block.end()) setMaxGenerations(atoi(i->second.front().c_str()));
		};
	}// END while
}; // schoen :)

void SETTINGS::loadHarvestFile(const string& harvestFile)
{
    ifstream pFile(harvestFile.c_str());
    char line[1024];
    string text;
    while(pFile.getline(line, sizeof line))
    {
        if(pFile.fail())
            pFile.clear(pFile.rdstate() & ~ios::failbit);
        text=line;
        size_t start=text.find_first_not_of("\t ");
        if((start==string::npos)||(text[0]=='#')||(text[0]=='\0'))
	            continue; // ignore line
        size_t stop=text.find_first_of("\t ", start);
        if(stop==string::npos) stop=0;
        string index=text.substr(start, stop);
        map<string, map<string, list<string> > >::iterator value;
        map<string, list<string> >::iterator item;
        if(index=="@HARVESTDATA")
        {
                map<string, map<string, list<string> > > block;
                parse_2nd_block(pFile, block);
				map<string, list<string> > player;
				if((value=block.find("@TERRA"))!=block.end())
				{
					if((item=value->second.find("Mineral Harvest"))!=value->second.end())
					{
						int j=0;
						for(list<string>::const_iterator i=item->second.begin();i!=item->second.end();++i)
	                        harvestSpeed[0].setHarvestMineralSpeed(j++,atoi(i->c_str()));
					}
					if((item=value->second.find("Gas Harvest"))!=value->second.end())
					{
						int j=0;
						for(list<string>::const_iterator i=item->second.begin();i!=item->second.end();++i)
	                        harvestSpeed[0].setHarvestGasSpeed(j++,atoi(i->c_str()));
					}
				};
				if((value=block.find("@PROTOSS"))!=block.end())
				{
					if((item=value->second.find("Mineral Harvest"))!=value->second.end())
					{
						int j=0;
						for(list<string>::const_iterator i=item->second.begin();i!=item->second.end();++i)
	                        harvestSpeed[1].setHarvestMineralSpeed(j++,atoi(i->c_str()));
					}
					if((item=value->second.find("Gas Harvest"))!=value->second.end())
					{
						int j=0;
						for(list<string>::const_iterator i=item->second.begin();i!=item->second.end();++i)
	                        harvestSpeed[1].setHarvestGasSpeed(j++,atoi(i->c_str()));
					}
				};
				if((value=block.find("@ZERG"))!=block.end())
				{
					if((item=value->second.find("Mineral Harvest"))!=value->second.end())
					{
						int j=0;
						for(list<string>::const_iterator i=item->second.begin();i!=item->second.end();++i)
	                        harvestSpeed[2].setHarvestMineralSpeed(j++,atoi(i->c_str()));
					}
					if((item=value->second.find("Gas Harvest"))!=value->second.end())
					{
						int j=0;
						for(list<string>::const_iterator i=item->second.begin();i!=item->second.end();++i)
	                        harvestSpeed[2].setHarvestGasSpeed(j++,atoi(i->c_str()));
					}
				};
        };
    }// END while
}; // schoen :)

void EXPORT SETTINGS::loadMapFile(const string& mapFile)
{
    ifstream pFile(mapFile.c_str());
    char line[1024];
    string text;
    while(pFile.getline(line, sizeof line))
    {
        if(pFile.fail())
            pFile.clear(pFile.rdstate() & ~ios::failbit);
        text=line;
        size_t start=text.find_first_not_of("\t ");
        if((start==string::npos)||(text[0]=='#')||(text[0]=='\0'))
	            continue; // ignore line
		list<string> words;
        parse_line(text, words);
		if(words.empty()) continue;
		list<string>::iterator j=words.begin();
        string index=*j;j++;
        map<string, list<string> >::iterator i;
        if(index=="@MAP")
        {
                map<string, list<string> > block;
                parse_block(pFile, block);
                if((i=block.find("Name"))!=block.end()) loadedMap[getMapCount()].setName(i->second.front());
                if((i=block.find("Max Locations"))!=block.end()) loadedMap[getMapCount()].setMaxLocations(atoi(i->second.front().c_str()));
                if((i=block.find("Max Player"))!=block.end()) loadedMap[getMapCount()].setMaxPlayer(atoi(i->second.front().c_str()));
        }
		else if(index=="@LOCATION")
		{
			if(j==words.end())
			{
				toLog("ERROR: (loadMapFile): Every @LOCATION entry needs a number.");
				return;
			};
			int location = atoi(j->c_str());
            map<string, list<string> > block;
            parse_block(pFile, block);
            if((i=block.find("Name"))!=block.end()) loadedMap[getMapCount()].setLocationName(location, i->second.front().c_str());
            if((i=block.find("Mineral Distance"))!=block.end()) loadedMap[getMapCount()].setLocationMineralDistance(location, atoi(i->second.front().c_str()));
            if((i=block.find("Distances"))!=block.end()) 
			{
				i->second.pop_front();
				loadedMap[getMapCount()].setLocationDistance(location, i->second);
			}
		}
    }// END while
	loadedMap[getMapCount()].calculateLocationsDistances();
	setMapCount(getMapCount()+1);
}; // schoen :)



void EXPORT SETTINGS::loadStartconditionFile(const string& startconditionFile)
{
    ifstream pFile(startconditionFile.c_str());
    char line[1024];
    string text;
	eRace race=TERRA; 
    while(pFile.getline(line, sizeof line))
    {
        if(pFile.fail())
            pFile.clear(pFile.rdstate() & ~ios::failbit);
        text=line;
        size_t start=text.find_first_not_of("\t ");
        if((start==string::npos)||(text[0]=='#')||(text[0]=='\0'))
	            continue; // ignore line
        list<string> words;
        parse_line(text, words);

		list<string>::iterator j=words.begin();
        string index=*j;j++;
        map<string, list<string> >::iterator i;
        if(index=="@STARTCONDITIONS")
        {
			if(j==words.end())
			{
				toLog("ERROR: (loadMapFile): Every @LOCATION entry needs a number.");
				return;
			};

			if(*j=="Terra") race=TERRA;
			else if(*j=="Protoss") race=PROTOSS;
			else if(*j=="Zerg") race=ZERG;

                map<string, list<string> > block;
                parse_block(pFile, block);

                if((i=block.find("Name"))!=block.end()) loadedStartcondition[getStartconditionCount()].setName(i->second.front());
                if((i=block.find("Minerals"))!=block.end()) loadedStartcondition[getStartconditionCount()].setMinerals(atoi(i->second.front().c_str()));
                if((i=block.find("Gas"))!=block.end()) loadedStartcondition[getStartconditionCount()].setGas(atoi(i->second.front().c_str()));
                if((i=block.find("Time"))!=block.end()) loadedStartcondition[getStartconditionCount()].setStartTime(atoi(i->second.front().c_str()));
        }
        else if(index=="@LOCATION")
        {
            if(j==words.end())
            {
                toLog("ERROR: (loadMapFile): Every @LOCATION entry needs a number.");
                return;
            };
			
            int location = atoi(j->c_str());
// TODO pruefen
            map<string, list<string> > block;
            parse_block(pFile, block);
            int j;
            for(j=0;j<UNIT_TYPE_COUNT;j++)
            {
                    string unit=stats[race][j].name;
                    if((i=block.find(unit))!=block.end())
                    {
                        i->second.pop_front();int count=atoi(i->second.front().c_str());
                            //TODO: values checken!
						loadedStartcondition[getStartconditionCount()].setLocationTotal(location, j, count);
						j=UNIT_TYPE_COUNT+1;
                        break;
                    }
                if(j!=REFINERY+1)
 	               toLog("WARNING: (SETTINGS::loadGoalFile) No unit name matched this goal.");
            }
        } // end if == LOCATION
    }// END while
//	loadedStartcondition[getStartconditionCount()].adjustAvailibility(); ?
    loadedStartcondition[getStartconditionCount()].adjustResearches();
    loadedStartcondition[getStartconditionCount()].adjustSupply();
    setStartconditionCount(getStartconditionCount()+1);
}; // schoen :)

const GA* EXPORT SETTINGS::getGa() const
{
	return(&ga);
};

/*void EXPORT SETTINGS::initializeMap()
{
	soup.initializeMap(start.getMap());
};*/

void EXPORT SETTINGS::initSoup()
{
	soup.initializeMap(start.getMap());
	// set GA and START on prerace and soup
	soup.setParameters(&ga, &start);
	// allocate memory for players ~~
}


ANARACE** EXPORT SETTINGS::newGeneration(ANARACE* oldAnarace[MAX_PLAYER])
{
	return(soup.newGeneration(oldAnarace));
};


SETTINGS settings;

