#include "settings.hpp"

void toLog(const string& msg)
{
	ofstream pFile("error.log", ios_base::app);
	pFile << msg.c_str() << endl;
}


SETTINGS::SETTINGS()
{
	srand(time(NULL));
	initDefaults();
	speed=2;
}

SETTINGS::~SETTINGS()
{
}

SETTINGS settings;

const int SETTINGS::getSpeed() const
{
	return(speed);
}

void SETTINGS::setSpeed(const int speed)
{
	this->speed=speed;
}


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
}

// -------------------------------
// ------ CONTROL FUNCTIONS ------
// -------------------------------

void EXPORT SETTINGS::assignRunParametersToSoup()
{
//	soup.initializeMap(start.getMap()); //???? TODO
	// set GA and START on prerace and soup
	soup.setParameters(&ga, &start);
	// allocate memory for players ~~
}

ANARACE** EXPORT SETTINGS::newGeneration(ANARACE* oldAnarace[MAX_PLAYER])
{
	return(soup.newGeneration(oldAnarace));
}

void EXPORT SETTINGS::calculateAnaplayer()
{
	soup.calculateAnaplayer();
}

void EXPORT SETTINGS::fillGroups()
{
	start.fillGroups();
}

void EXPORT SETTINGS::checkForChange() const
{
	soup.checkForChange();	
}

// ------------------------------------------
// ------ PARSING TOOLS AND ERROR LOGS ------
// ------------------------------------------


void parse_line(string& text, list<string>& words)
{
	size_t n = text.length();
	size_t stop;
	bool inParantheses=false;
	// " gefunden? ignoriere alle Sonderzeichen bis naechstes "
	size_t start = text.find_first_not_of("\t ");
	while (start!=string::npos) 
	{
		if(text[start]=='\"')
		{
			inParantheses=true;
			start++;
		}
	
		if(inParantheses)
			stop = text.find_first_of("\"", start);
		else 
			stop = text.find_first_of(",\t\" =", start);
		inParantheses=false;
		if(stop==string::npos)
			stop = n;
		string bla=text.substr(start, stop - start);
		words.push_back(bla);
	   
		start = text.find_first_not_of(",\t =", stop+1);
	}
}

void parse_block(ifstream& stream, map<string, list<string> >& block)
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
	}
}

void parse_2nd_block(ifstream& stream, map<string, map<string, list<string> > >& block)
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
	}
}

// -----------------------------------------------
// ------  END PARSING TOOLS AND ERROR LOGS ------
// -----------------------------------------------



// ---------------------------
// ------- FILE LOADING ------
// ---------------------------

void SETTINGS::loadGoalFile(const string& goalFile)
{
	ifstream pFile(goalFile.c_str());
	if(!pFile.is_open())
	{
		toLog("ERROR: (loadGoalFile): File not found.");
		return;
	}
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
		if(stop==string::npos) stop=text.size();
		string index=text.substr(start, stop);
		map<string, list<string> >::iterator i;
		if(index=="@GOAL")
		{
				map<string, list<string> > block;
				parse_block(pFile, block);
				if((i=block.find("Name"))!=block.end()){
					i->second.pop_front();
					loadedGoal[getGoalCount()].setName(i->second.front());
				}
				if((i=block.find("Race"))!=block.end()) 
				{
					eRace race=TERRA;
					i->second.pop_front();
					string estr=i->second.front();
					if(i->second.front()=="Terra") race=TERRA;
					else if(i->second.front()=="Protoss") race=PROTOSS;
					else if(i->second.front()=="Zerg") race=ZERG;
#ifdef _SCC_DEBUG
					else {
						toLog("ERROR: (loadSettingsFile): Wrong race entry.");return;
					}
#endif
					loadedGoal[getGoalCount()].setRace(race);
				}
				map<string, list<string> >::iterator k;
				for(int unit=UNIT_TYPE_COUNT;unit--;)
				{
					if((k=block.find(stats[loadedGoal[getGoalCount()].getRace()][unit].name))!=block.end())
					{
						list<string>::iterator l=k->second.begin();
						if(l->size()>=3)
						{
							l++;int count=atoi(l->c_str());
							l++;int location=atoi(l->c_str());
							l++;int time=atoi(l->c_str());
							loadedGoal[getGoalCount()].addGoal(unit, count, 60*time, location);
						}
					}
				}
		}

	}
//  loadedGoal[getGoalCount()].adjustGoals(ga.allowGoalAdaption);
	setGoalCount(getGoalCount()+1);
} // schoen :)

void SETTINGS::loadSettingsFile(const string& settingsFile)
{
	ifstream pFile(settingsFile.c_str());
	if(!pFile.is_open())
	{
		toLog("ERROR: (loadSettingsFile): File not found.");
		return;
	}
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
		if(stop==string::npos) stop=text.size();
		string index=text.substr(start, stop);
		string value;
		map<string, list<string> >::iterator i;
		if(index=="@SETTINGS")
		{
				map<string, list<string> > block;
				parse_block(pFile, block);
				if((i=block.find("Allow goal adaption"))!=block.end()){
					i->second.pop_front();
				   	setAllowGoalAdaption(atoi(i->second.front().c_str()));
				}
				if((i=block.find("Max Time"))!=block.end()){
					i->second.pop_front();
				   	setMaxTime(atoi(i->second.front().c_str()));
				}
				if((i=block.find("Max Timeout"))!=block.end()){
					i->second.pop_front();
				   	setMaxTimeOut(atoi(i->second.front().c_str()));
				}
				if((i=block.find("Max Length"))!=block.end()){
					i->second.pop_front();
				   	setMaxLength(atoi(i->second.front().c_str()));
				}
				if((i=block.find("Max Runs"))!=block.end()){
					i->second.pop_front();
				   	setMaxRuns(atoi(i->second.front().c_str()));
				}
				if((i=block.find("Preprocess Buildorder"))!=block.end()){
					i->second.pop_front();
				   	setPreprocessBuildOrder(atoi(i->second.front().c_str()));
				}
				if((i=block.find("Mode"))!=block.end()){
					i->second.pop_front();
				   	setMode(atoi(i->second.front().c_str()));
				}
				if((i=block.find("Breed Factor"))!=block.end()){
					i->second.pop_front();
				   	setBreedFactor(atoi(i->second.front().c_str()));
				}
				if((i=block.find("Crossing Over"))!=block.end()){ 
					i->second.pop_front();
					setCrossOver(atoi(i->second.front().c_str()));
				}
				if((i=block.find("Max unchanged Generations"))!=block.end()){
					i->second.pop_front();
				   	setMaxGenerations(atoi(i->second.front().c_str()));
				}
		}
	}// END while
} // schoen :)

void SETTINGS::loadHarvestFile(const string& harvestFile)
{
	ifstream pFile(harvestFile.c_str());
	if(!pFile.is_open())
	{
		toLog("ERROR: (loadHarvestFile): File not found.");
		return;
	}
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
		if(stop==string::npos) stop=text.size();
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
					// erstes Element falsch? TODO
					if((item=value->second.find("Mineral Harvest"))!=value->second.end())
					{
						int j=0;
						item->second.pop_front(); // the expression 'mineral harvest' itself
						for(list<string>::const_iterator i=item->second.begin();i!=item->second.end();++i)
							loadedHarvestSpeed[TERRA].setHarvestMineralSpeed(j++,atoi(i->c_str()));
					}
					if((item=value->second.find("Gas Harvest"))!=value->second.end())
					{
						int j=0;
						item->second.pop_front(); // the expression 'gas harvest' itself
						for(list<string>::const_iterator i=item->second.begin();i!=item->second.end();++i)
							loadedHarvestSpeed[TERRA].setHarvestGasSpeed(j++,atoi(i->c_str()));
					}
				}
				if((value=block.find("@PROTOSS"))!=block.end())
				{
					if((item=value->second.find("Mineral Harvest"))!=value->second.end())
					{
						int j=0;
						item->second.pop_front();
						for(list<string>::const_iterator i=item->second.begin();i!=item->second.end();++i)
							loadedHarvestSpeed[PROTOSS].setHarvestMineralSpeed(j++, atoi(i->c_str()));
					}
					if((item=value->second.find("Gas Harvest"))!=value->second.end())
					{
						int j=0;
						item->second.pop_front(); 
						for(list<string>::const_iterator i=item->second.begin();i!=item->second.end();++i)
							loadedHarvestSpeed[PROTOSS].setHarvestGasSpeed(j++,atoi(i->c_str()));
					}
				}
				if((value=block.find("@ZERG"))!=block.end())
				{
					if((item=value->second.find("Mineral Harvest"))!=value->second.end())
					{
						int j=0;
						item->second.pop_front(); 
						for(list<string>::const_iterator i=item->second.begin();i!=item->second.end();++i)
							loadedHarvestSpeed[ZERG].setHarvestMineralSpeed(j++,atoi(i->c_str()));
					}
					if((item=value->second.find("Gas Harvest"))!=value->second.end())
					{
						int j=0;
						item->second.pop_front(); 
						for(list<string>::const_iterator i=item->second.begin();i!=item->second.end();++i)
							loadedHarvestSpeed[ZERG].setHarvestGasSpeed(j++,atoi(i->c_str()));
					}
				}
		}
	}// END while
} // schoen :)

void EXPORT SETTINGS::loadMapFile(const string& mapFile)
{
	ifstream pFile(mapFile.c_str());
	if(!pFile.is_open())
	{
		toLog("ERROR: (loadMapFile): File not found.");
		return;
	}
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
				if((i=block.find("Name"))!=block.end()){
					i->second.pop_front();
				   	loadedMap[getMapCount()].setName(i->second.front());
				}
				if((i=block.find("Max Locations"))!=block.end()){
					i->second.pop_front();
				   	loadedMap[getMapCount()].setMaxLocations(atoi(i->second.front().c_str()));
				}
				if((i=block.find("Max Player"))!=block.end()){
					i->second.pop_front();
				   	loadedMap[getMapCount()].setMaxPlayer(atoi(i->second.front().c_str()));
				}
		}
		else if(index=="@LOCATION")
		{
			if(j==words.end())
			{
				toLog("ERROR: (loadMapFile): Every @LOCATION entry needs a number.");
				return;
			}
			int location = atoi(j->c_str());
			map<string, list<string> > block;
			parse_block(pFile, block);
			if((i=block.find("Name"))!=block.end()) 
			{
				i->second.pop_front();
				loadedMap[getMapCount()].setLocationName(location-1, i->second.front().c_str());
			}
			if((i=block.find("Mineral Distance"))!=block.end()) 
			{
				i->second.pop_front();
				loadedMap[getMapCount()].setLocationMineralDistance(location-1, atoi(i->second.front().c_str()));
			}
			if((i=block.find("Distances"))!=block.end()) 
			{
				i->second.pop_front();
				loadedMap[getMapCount()].setLocationDistance(location-1, i->second);
			}
			if((i=block.find("Minerals"))!=block.end())
			{
				i->second.pop_front();
				loadedMap[getMapCount()].setLocationMineralPatches(location-1, atoi(i->second.front().c_str()));
			}
			if((i=block.find("Geysirs"))!=block.end())
			{
				i->second.pop_front();
				loadedMap[getMapCount()].setLocationVespeneGeysirs(location-1, atoi(i->second.front().c_str()));
			}
			
		}
	}// END while
	loadedMap[getMapCount()].calculateLocationsDistances();
	setMapCount(getMapCount()+1);
} // schoen :)

void EXPORT SETTINGS::loadStartconditionFile(const string& startconditionFile)
{
	ifstream pFile(startconditionFile.c_str());
	if(!pFile.is_open())
	{
		toLog("ERROR: (loadStartconditionFile): File not found.");
		return;
	}
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
			}

			if(*j=="Terra") race=TERRA;
			else if(*j=="Protoss") race=PROTOSS;
			else if(*j=="Zerg") race=ZERG;

				map<string, list<string> > block;
				parse_block(pFile, block);

				if((i=block.find("Name"))!=block.end()) 
				{
					i->second.pop_front();
					loadedStartcondition[getStartconditionCount()].setName(i->second.front());
				}
				if((i=block.find("Minerals"))!=block.end()) 
				{
					i->second.pop_front();
					loadedStartcondition[getStartconditionCount()].setMinerals(100*atoi(i->second.front().c_str()));
				}
				if((i=block.find("Gas"))!=block.end()) 
				{
					i->second.pop_front();
					loadedStartcondition[getStartconditionCount()].setGas(100*atoi(i->second.front().c_str()));
				}
				if((i=block.find("Time"))!=block.end()) 
				{
					i->second.pop_front();
					loadedStartcondition[getStartconditionCount()].setStartTime(atoi(i->second.front().c_str()));
				}
		}
		else if(index=="@LOCATION")
		{
			if(j==words.end())
			{
				toLog("ERROR: (loadMapFile): Every @LOCATION entry needs a number.");
				return;
			}
			
			int location = atoi(j->c_str());
// TODO pruefen
			map<string, list<string> > block;
			parse_block(pFile, block);
			for(int k=UNIT_TYPE_COUNT;k--;)
			{
					string unit=stats[race][k].name;
					if((i=block.find(unit))!=block.end())
					{
						i->second.pop_front();int count=atoi(i->second.front().c_str());
							//TODO: values checken!
						loadedStartcondition[getStartconditionCount()].setLocationTotal(location-1, k, count);
						loadedStartcondition[getStartconditionCount()].setLocationAvailible(location-1, k, count);
					}
			}
		} // end if == LOCATION
	}// END while
	loadedStartcondition[getStartconditionCount()].assignRace(race);
	loadedStartcondition[getStartconditionCount()].adjustResearches();
	loadedStartcondition[getStartconditionCount()].adjustSupply();
	setStartconditionCount(getStartconditionCount()+1);
} // schoen :)

// ---------------------------------
// ------ END OF FILE LOADING ------
// ---------------------------------


// -------------------------------
// ------ GET/SET FUNCTIONS ------
// -------------------------------

const BASIC_MAP* EXPORT SETTINGS::getMap(const int mapNumber) const
{
#ifdef _SCC_DEBUG
	if((mapNumber>getMapCount())||(mapNumber<0)) {
		toLog("WARNING: (SETTINGS::getMap): Value out of range.");return(0);
	}
#endif
	return(&loadedMap[mapNumber]);
}

void EXPORT SETTINGS::setAllowGoalAdaption(const bool allowGoalAdaption) // allow the program to change goals (for example ignore command center when command center [NS] is already a goal
{
	ga.allowGoalAdaption=allowGoalAdaption;
}

void EXPORT SETTINGS::setMaxTime(const int maxTime) //maximum time of build order in seconds
{
#ifdef _SCC_DEBUG
	if((maxTime<MIN_TIME)||(maxTime>MAX_TIME)) {
		toLog("WARNING: (SETTINGS::setMaxTime): Value out of range.");return;
	}
#endif
	ga.maxTime=maxTime;
}

void EXPORT SETTINGS::setMaxTimeOut(const int maxTimeOut) //timeout for building
{
#ifdef _SCC_DEBUG
	if((maxTimeOut<MIN_TIMEOUT)||(maxTimeOut>MAX_TIMEOUT)) {
		toLog("WARNING: (SETTINGS::setMaxTimeOut): Value out of range.");return;
	}
#endif
	ga.maxTimeOut=maxTimeOut;
}

void EXPORT SETTINGS::setMaxLength(const int maxLength)
{
#ifdef _SCC_DEBUG
	if((maxLength<MIN_LENGTH)||(maxLength>MAX_LENGTH)) {
		toLog("WARNING: (SETTINGS::setMaxLength): Value out of range.");return;
	}
#endif
	ga.maxLength=maxLength;
}

void EXPORT SETTINGS::setMaxRuns(const int maxRuns)
{
#ifdef _SCC_DEBUG
	if((maxRuns<MIN_RUNS)||(maxRuns>MAX_RUNS)) {
		toLog("WARNING: (SETTINGS::setMaxRuns): Value out of range.");return;
	}
#endif
	ga.maxRuns=maxRuns;
}

void EXPORT SETTINGS::setMaxGenerations(const int maxGenerations)
{
#ifdef _SCC_DEBUG
	if((maxGenerations<MIN_GENERATIONS)||(maxGenerations>MAX_GENERATIONS)) {
		toLog("WARNING: (SETTINGS::setMaxGenerations): Value out of range.");return;
	}
#endif
	ga.maxGenerations=maxGenerations;
}

void SETTINGS::setMapCount(const int mapCount)
{
#ifdef _SCC_DEBUG
	if((mapCount<MIN_MAPS)||(mapCount>=MAX_MAPS)) {
		toLog("WARNING: (SETTINGS::setMapCount): Value out of range.");return;
	}
#endif
	ga.mapCount=mapCount;
}

void SETTINGS::setStartconditionCount(const int startconditionCount)
{
#ifdef _SCC_DEBUG
	if((startconditionCount<MIN_GOAL_ENTRIES)||(startconditionCount>=MAX_GOAL_ENTRIES)) {
		toLog("WARNING: (SETTINGS::setStartconditionCount): Value out of range.");return;
	}
#endif
	ga.startconditionCount=startconditionCount;
}


void SETTINGS::setGoalCount(const int goalCount)
{
#ifdef _SCC_DEBUG
	if((goalCount<MIN_GOAL_ENTRIES)||(goalCount>=MAX_GOAL_ENTRIES)) {
		toLog("WARNING: (SETTINGS::setGoalCount): Value out of range.");return;
	}
#endif
	ga.goalCount=goalCount;
}

void EXPORT SETTINGS::setPreprocessBuildOrder(const bool preprocess)
{
	ga.preprocessBuildOrder=preprocess;
}

void EXPORT SETTINGS::assignMap(const int mapNumber)
{
#ifdef _SCC_DEBUG
	if((mapNumber<0)||(mapNumber>=MAX_MAPS)) {
		toLog("WARNING: (SETTINGS::setMap): Value out of range.");return;
	}
#endif
	start.assignMap(&(loadedMap[mapNumber]));
//	soup.initializeMap(&(loadedMap[mapNumber])); //?
}

/// ==> aendern, dass es nicht auf ga zeigt... oder? mmmh... muss ja auch fitnessfunktion zugreifen
void EXPORT SETTINGS::setMode(const int mode)
{
#ifdef _SCC_DEBUG
	if((mode<MIN_MODE)||(mode>MAX_MODE)) {
		toLog("WARNING: (SETTINGS::setMode): Value out of range.");return;
	}
#endif
	ga.mode=mode;
}

void EXPORT SETTINGS::setStartRace(const int player, const eRace race)
{
	start.setPlayerRace(player, race);
}

void EXPORT SETTINGS::setStartcondition(const int player, const int startcondition)
{
	// TODO Fehlerpruefung
	start.setStartcondition(player, &(loadedStartcondition[startcondition]));
}

void EXPORT SETTINGS::setHarvestSpeed(const eRace race, const int harvest)
{
	// todo: checken ob harvest dazupasst
	start.setHarvestSpeed(race, &(loadedHarvestSpeed[harvest]));
}

void EXPORT SETTINGS::setStartPosition(const int player, const int startPosition)
{
	start.setStartPosition(player, startPosition);
}

void EXPORT SETTINGS::setGoal(const int player, const int goal)
{
#ifdef _SCC_DEBUG
	if((goal<MIN_GOAL_ENTRIES)||(goal>=getGoalCount())||(player<MIN_PLAYER)||(player>=MAX_PLAYER/*loadedMap[getCurrentMap()].getMaxPlayer()*/)) {
		toLog("WARNING: (SETTINGS::setGoal): Value out of range.");return;
	}
#endif
	start.setGoal(player, &loadedGoal[goal]);
}



void EXPORT SETTINGS::setBreedFactor(const int breedFactor)
{
#ifdef _SCC_DEBUG
	if((breedFactor<MIN_BREED_FACTOR)||(breedFactor>MAX_BREED_FACTOR)) {
		toLog("WARNING: (SETTINGS::setBreedFactor): Value out of range.");return;
	}
#endif
	ga.setBreedFactor(breedFactor);
}

void EXPORT SETTINGS::setCrossOver(const int crossOver)
{
#ifdef _SCC_DEBUG
	if((crossOver<MIN_CROSSOVER)||(crossOver>MAX_CROSSOVER)) {
		toLog("WARNING: (SETTINGS::setCrossOver): Value out of range.");return;
	}
#endif
	ga.setCrossOver(crossOver);
}

const int EXPORT SETTINGS::getBreedFactor() const
{
	return(ga.getBreedFactor());
}

const int EXPORT SETTINGS::getMode() const
{
	return(ga.mode);
}

const int EXPORT SETTINGS::getCrossOver() const
{
	return(ga.getCrossOver());
}

const int EXPORT SETTINGS::getMaxTime() const
{
	return(ga.maxTime);
}

const int EXPORT SETTINGS::getMaxTimeOut() const
{
	return(ga.maxTimeOut);
}

const int EXPORT SETTINGS::getMaxLength() const
{
	return(ga.maxLength);
}

const int EXPORT SETTINGS::getMaxRuns() const
{
	return(ga.maxRuns);
}

const int EXPORT SETTINGS::getMaxGenerations() const
{
	return(ga.maxGenerations);
}

const bool EXPORT SETTINGS::getPreprocessBuildOrder() const
{
	return(ga.preprocessBuildOrder);
}

const int EXPORT SETTINGS::getStartconditionCount() const
{
	return(ga.startconditionCount);
}

const int EXPORT SETTINGS::getGoalCount() const
{
	return(ga.goalCount);
}

const int EXPORT SETTINGS::getMapCount() const
{
	return(ga.mapCount);
}

const GOAL_ENTRY* EXPORT SETTINGS::getGoal(const int goalNumber) const
{
#ifdef _SCC_DEBUG
	if((goalNumber<MIN_GOAL_ENTRIES)||(goalNumber>=getGoalCount())) {
		toLog("WARNING: (SETTINGS::getGoal): Value out of range.");return(0);
	}
#endif
	return(&loadedGoal[goalNumber]);
}

const START_CONDITION* EXPORT SETTINGS::getStartcondition(const int startconditionNumber) const
{
#ifdef _SCC_DEBUG
	if((startconditionNumber<0)||(startconditionNumber>=getStartconditionCount())) {
		toLog("WARNING: (SETTINGS::getStartcondition): Value out of range.");return(0);
	}
#endif
	return(&loadedStartcondition[startconditionNumber]);
}
const GA* EXPORT SETTINGS::getGa() const
{
	return(&ga);
}
// --------------------------------------
// ------ END OF GET/SET FUNCTIONS ------
// --------------------------------------

