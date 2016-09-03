#include "settings.hpp"
#include "../ui/object.hpp"

// TODO in theme rein oder so

SETTINGS::SETTINGS():
	loadedMap(),
	ga(),
	soup(),
	start(),
	currentMap(0),
	speed(2)
{
	srand(time(NULL));
	initDefaults();
}

SETTINGS::~SETTINGS()
{ }

SETTINGS settings;

const unsigned int SETTINGS::getSpeed() const
{
	return(speed);
}

void SETTINGS::setSpeed(const unsigned int set_speed)
{
	speed=set_speed;
}


void SETTINGS::initDefaults()
{
	currentMap=0;
	setAllowGoalAdaption(true);
	setMaxTime(MAX_TIME-1);
	setMaxTimeOut(MAX_TIMEOUT);
	setMaxLength(MAX_LENGTH);
	setMaxRuns(MAX_RUNS);
	setMaxGenerations(MAX_GENERATIONS);
	setPreprocessBuildOrder(false);
	setCrossOver(MIN_CROSSOVER);
	setBreedFactor(20);
}

// -------------------------------
// ------ CONTROL FUNCTIONS ------
// -------------------------------

void SETTINGS::assignRunParametersToSoup()
{
//	soup.initializeMap(start.getMap()); //???? TODO
	// set GA and START on prerace and soup
	soup.setParameters(&ga, &start);
	// allocate memory for players ~~
}

ANARACE** SETTINGS::newGeneration(ANARACE* oldAnarace[MAX_PLAYER])
{
	return(soup.newGeneration(oldAnarace));
}

void SETTINGS::calculateAnaplayer()
{
	soup.calculateAnaplayer();
}

void SETTINGS::fillGroups()
{
	start.fillGroups();
}

void SETTINGS::checkForChange() const
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
	if((goalFile.compare(goalFile.size()-4, goalFile.size(), ".gol")==1))
		return;

//	ostringstream os;
//	os << "Loading " << goalFile << " ...";
//	toLog(os.str());

	ifstream pFile(goalFile.c_str());
	if(!pFile.is_open())
	{
		ostringstream os;
		os.str("");
		os << "ERROR: (loadGoalFile): File " << goalFile << " not found.";
		toLog(os.str());
		return;
	}
	char line[1024];
	string text;
	GOAL_ENTRY goal;		
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
				goal.setName(i->second.front());
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
				goal.setRace(race);
			}
			map<string, list<string> >::iterator k;
			for(int unit=UNIT_TYPE_COUNT;unit--;)
			{
				if((k=block.find(stats[goal.getRace()][unit].name))!=block.end())
				{
					list<string>::iterator l=k->second.begin();
					if(l->size()>=3)
					{
						l++;int count=atoi(l->c_str());
						l++;int location=atoi(l->c_str());
						l++;int time=atoi(l->c_str());
						goal.addGoal(unit, count, time, location);
					}
				}
			}
		} // end index == GOAL
	}

	loadedGoal[goal.getRace()].push_back(goal);
	
//  loadedGoal[getGoalCount()].adjustGoals(ga.allowGoalAdaption);
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

void SETTINGS::loadMapFile(const string& mapFile)
{
	ifstream pFile(mapFile.c_str());
	if(!pFile.is_open())
	{
		toLog("ERROR: (loadMapFile): File not found.");
		return;
	}
	char line[1024];
	string text;
	BASIC_MAP basicmap;
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
				   	basicmap.setName(i->second.front());
				}
				if((i=block.find("Max Locations"))!=block.end()){
					i->second.pop_front();
				   	basicmap.setMaxLocations(atoi(i->second.front().c_str()));
				}
				if((i=block.find("Max Player"))!=block.end()){
					i->second.pop_front();
				   	basicmap.setMaxPlayer(atoi(i->second.front().c_str()));
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
				basicmap.setLocationName(location-1, i->second.front().c_str());
			}
			if((i=block.find("Mineral Distance"))!=block.end()) 
			{
				i->second.pop_front();
				basicmap.setLocationMineralDistance(location-1, atoi(i->second.front().c_str()));
			}
			if((i=block.find("Distances"))!=block.end()) 
			{
				i->second.pop_front();
				basicmap.setLocationDistance(location-1, i->second);
			}
			if((i=block.find("Minerals"))!=block.end())
			{
				i->second.pop_front();
				basicmap.setLocationMineralPatches(location-1, atoi(i->second.front().c_str()));
			}
			if((i=block.find("Geysirs"))!=block.end())
			{
				i->second.pop_front();
				basicmap.setLocationVespeneGeysirs(location-1, atoi(i->second.front().c_str()));
			}
			
		}
	}// END while
	basicmap.calculateLocationsDistances();
	loadedMap.push_back(basicmap);
} // schoen :)

void SETTINGS::loadStartconditionFile(const string& startconditionFile)
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
	START_CONDITION startcondition;
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
					startcondition.setName(i->second.front());
				}
				if((i=block.find("Minerals"))!=block.end()) 
				{
					i->second.pop_front();
					startcondition.setMinerals(100*atoi(i->second.front().c_str()));
				}
				if((i=block.find("Gas"))!=block.end()) 
				{
					i->second.pop_front();
					startcondition.setGas(100*atoi(i->second.front().c_str()));
				}
				if((i=block.find("Time"))!=block.end()) 
				{
					i->second.pop_front();
					startcondition.setStartTime(atoi(i->second.front().c_str()));
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
						startcondition.setLocationTotal(location-1, k, count);
						startcondition.setLocationAvailible(location-1, k, count);
					}
			}
		} // end if == LOCATION
	}// END while
	startcondition.assignRace(race);
	startcondition.adjustResearches();
	startcondition.adjustSupply();
	loadedStartcondition[race].push_back(startcondition);
} // schoen :)

// ---------------------------------
// ------ END OF FILE LOADING ------
// ---------------------------------

// FILE SAVING


void SETTINGS::saveBuildOrder(const ANARACE* anarace) const
{
    ostringstream os;
    os << "output/bos/";
    os << raceString[anarace->getRace()] << "/bo" << rand()%100 << ".html";// TODO!
    ofstream pFile(os.str().c_str(), ios_base::out | ios_base::trunc);
    if(!pFile.is_open())
    {
        toLog("ERROR: Could not create file (write protection? disk space?)");
        return;
    }
pFile << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">" << std::endl;
pFile << "<html>" << std::endl;
pFile << "<head>" << std::endl;
pFile << "  <meta content=\"text/html; charset=ISO-8859-1\"" << std::endl;
pFile << " http-equiv=\"content-type\">" << std::endl;
pFile << "  <title>Build order list</title>" << std::endl;
pFile << "</head>" << std::endl;
pFile << "<body alink=\"#000099\" vlink=\"#990099\" link=\"#000099\" style=\"color: rgb("<< (int)UI_Object::theme.lookUpColor(BRIGHT_TEXT_COLOR)->r() << ", " << (int)UI_Object::theme.lookUpColor(BRIGHT_TEXT_COLOR)->g() << ", " << (int)UI_Object::theme.lookUpColor(BRIGHT_TEXT_COLOR)->b() << "); background-color: rgb(" << (int)UI_Object::theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH)->GetColor()->r() << ", " << (int)UI_Object::theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH)->GetColor()->g() << ", " << (int)UI_Object::theme.lookUpBrush(WINDOW_BACKGROUND_BRUSH)->GetColor()->b() << ");\">" << std::endl;
pFile << "<div style=\"text-align: center;\"><big style=\"font-weight: bold;\"><big>Evolution Forge BETA Demo 1.59</big></big><br><br>" << std::endl;
pFile << "<big>Buildorder list</big><br>" << std::endl;
pFile << "</div>" << std::endl;
pFile << "<br>" << std::endl;
pFile << "<table style=\"background-color: rgb(" << (int)UI_Object::theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH)->GetColor()->r() << ", " << (int)UI_Object::theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH)->GetColor()->g() << ", " << (int)UI_Object::theme.lookUpBrush(WINDOW_FOREGROUND_BRUSH)->GetColor()->b() << "); text-align: center; vertical-align: middle; width: 600px; margin-left: auto; margin-right: auto;\""<< std::endl;
pFile << " border=\"1\" cellspacing=\"0\" cellpadding=\"1\">" << std::endl;
pFile << "  <tbody>" << std::endl;
pFile << "    <tr>" << std::endl;
pFile << "      <td style=\"text-align: center; vertical-align: middle; width: 200px;\">Unitname<br>" << std::endl;
pFile << "      </td>" << std::endl;
pFile << "      <td style=\"text-align: center; vertical-align: middle; width: 75px;\">Supply</td>" << std::endl;
pFile << "      <td style=\"text-align: center; vertical-align: middle; width: 75px;\">Minerals<br>" << std::endl;
pFile << "      </td>" << std::endl;
pFile << "      <td style=\"text-align: center; vertical-align: middle; width: 75px;\">Gas<br>" << std::endl;
pFile << "      </td>" << std::endl;
pFile << "      <td style=\"text-align: center; vertical-align: middle; width: 100px;\">Location<br>" << std::endl;
pFile << "      </td>" << std::endl;
pFile << "      <td style=\"text-align: center; vertical-align: middle; width: 75px;\">Time<br>" << std::endl;
pFile << "      </td>" << std::endl;
pFile << "    </tr>" << std::endl;

for(int i = MAX_LENGTH;i--;)
	if(anarace->getProgramIsBuilt(i))
	{
pFile << "    <tr style=\"text-align: center; vertical-align: middle; background-color: rgb(" << (int)UI_Object::theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[anarace->getRace()][anarace->getPhaenoCode(i)].unitType))->GetColor()->r() << ", " << (int)UI_Object::theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[anarace->getRace()][anarace->getPhaenoCode(i)].unitType))->GetColor()->g() << ", " << (int)UI_Object::theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[anarace->getRace()][anarace->getPhaenoCode(i)].unitType))->GetColor()->b() << ");\">" << std::endl;

pFile << "      <td style=\"\">" << stats[anarace->getRace()][anarace->getPhaenoCode(i)].name << "<br>" << std::endl;
pFile << "      </td>" << std::endl;

pFile << "      <td style=\"\">" << anarace->getStatisticsNeedSupply(i) << "/" << anarace->getStatisticsHaveSupply(i) << "<br>" << std::endl;
pFile << "      </td>" << std::endl;

pFile << "      <td style=\"\">" << anarace->getStatisticsHaveMinerals(i)/100 << "<br>" << std::endl;
pFile << "      </td>" << std::endl;

pFile << "      <td style=\"\">" << anarace->getStatisticsHaveGas(i)/100 << "<br>" << std::endl;
pFile << "      </td>" << std::endl;

pFile << "      <td style=\"\">" << (*anarace->getMap())->getLocation(anarace->getProgramLocation(i))->getName() << "<br>" << std::endl;
pFile << "      </td>" << std::endl;

if(anarace->getRealProgramTime(i)%60>9)
{
pFile << "      <td style=\"\">" << anarace->getRealProgramTime(i)/60 << ":" << anarace->getRealProgramTime(i)%60 << "<br>" << std::endl;
pFile << "      </td>" << std::endl;
} else
{
pFile << "      <td style=\"\">" << anarace->getRealProgramTime(i)/60 << ":0" << anarace->getRealProgramTime(i)%60 << "<br>" << std::endl;
pFile << "      </td>" << std::endl;
}
                                                                                                   
pFile << "    </tr>" << std::endl;
}

pFile << "  </tbody>" << std::endl;
pFile << "</table>" << std::endl;
pFile << "<br>" << std::endl;
pFile << "</body>" << std::endl;
pFile << "</html>" << std::endl;
}

void SETTINGS::saveGoal(const GOAL_ENTRY* goalentry)
{
	ostringstream os;
	os << "settings/goals/";
	os << raceString[goalentry->getRace()] << "/goal" << rand()%100 << ".gol";// TODO!
    ofstream pFile(os.str().c_str(), ios_base::out | ios_base::trunc);
    if(!pFile.is_open())
    {
        toLog("ERROR: Could not create file (write protection? disk space?)");
        return;
	}

	pFile << "@GOAL" << std::endl;
	pFile << "        \"Name\" \"" << os.str() << "\"" << std::endl; // TODO
	pFile << "        \"Race\" \"" << raceString[goalentry->getRace()] << "\"" << std::endl;

    for(std::list<GOAL>::const_iterator i = goalentry->goal.begin(); i!=goalentry->goal.end(); i++)
//    bool first=true;
  //  while(goalentry->getNextGoal(i, first))
    {
//        first=false;
		pFile << "        \"" << stats[goalentry->getRace()][i->getUnit()].name << "\" \"" << i->getCount() << "\" \"" << i->getLocation() << "\" \"" << i->getTime() << "\"" << std::endl;		
	}
	
	pFile << "@END" << std::endl;
}

// -------------------------------
// ------ GET/SET FUNCTIONS ------
// -------------------------------

const bool SETTINGS::getIsNewRun()
{
	return(soup.getIsNewRun());
}

const BASIC_MAP* SETTINGS::getMap(const unsigned int mapNumber) const
{
#ifdef _SCC_DEBUG
	if(mapNumber>loadedMap.size()) {
		toLog("WARNING: (SETTINGS::getMap): Value out of range.");return(0);
	}
#endif
	return(&loadedMap[mapNumber]);
}

void SETTINGS::setAllowGoalAdaption(const bool allowGoalAdaption) // allow the program to change goals (for example ignore command center when command center [NS] is already a goal
{
	ga.allowGoalAdaption=allowGoalAdaption;
}

void SETTINGS::setMaxTime(const unsigned int maxTime) //maximum time of build order in seconds
{
#ifdef _SCC_DEBUG
	if((maxTime<MIN_TIME)||(maxTime>=MAX_TIME)) {
		toLog("WARNING: (SETTINGS::setMaxTime): Value out of range.");return;
	}
#endif
	ga.maxTime=maxTime;
}

void SETTINGS::setMaxTimeOut(const unsigned int maxTimeOut) //timeout for building
{
#ifdef _SCC_DEBUG
	if((maxTimeOut<MIN_TIMEOUT)||(maxTimeOut>MAX_TIMEOUT)) {
		toLog("WARNING: (SETTINGS::setMaxTimeOut): Value out of range.");return;
	}
#endif
	ga.maxTimeOut=maxTimeOut;
}

void SETTINGS::setMaxLength(const unsigned int maxLength)
{
#ifdef _SCC_DEBUG
	if((maxLength<MIN_LENGTH)||(maxLength>MAX_LENGTH)) {
		toLog("WARNING: (SETTINGS::setMaxLength): Value out of range.");return;
	}
#endif
	ga.maxLength=maxLength;
}

void SETTINGS::setMaxRuns(const unsigned int maxRuns)
{
#ifdef _SCC_DEBUG
	if((maxRuns<MIN_RUNS)||(maxRuns>MAX_RUNS)) {
		toLog("WARNING: (SETTINGS::setMaxRuns): Value out of range.");return;
	}
#endif
	ga.maxRuns=maxRuns;
}

void SETTINGS::setMaxGenerations(const unsigned int maxGenerations)
{
#ifdef _SCC_DEBUG
	if((maxGenerations<MIN_GENERATIONS)||(maxGenerations>MAX_GENERATIONS)) {
		toLog("WARNING: (SETTINGS::setMaxGenerations): Value out of range.");return;
	}
#endif
	ga.maxGenerations=maxGenerations;
}

GOAL_ENTRY* SETTINGS::getCurrentGoal(const unsigned int player)
{
	return(*(start.getCurrentGoal(player)));
}

void SETTINGS::setPreprocessBuildOrder(const bool preprocess)
{
	ga.preprocessBuildOrder=preprocess;
}

void SETTINGS::assignMap(const unsigned int mapNumber)
{
#ifdef _SCC_DEBUG
	if(mapNumber>=loadedMap.size()) {
		toLog("WARNING: (SETTINGS::assignMap): Value out of range.");return;
	}
#endif
	start.assignMap(&(loadedMap[mapNumber]));
//	soup.initializeMap(&(loadedMap[mapNumber])); //?
}

void SETTINGS::assignStartRace(const unsigned int player, const eRace race)
{
	start.setPlayerRace(player, race);
}

void SETTINGS::assignStartcondition(const unsigned int player, const unsigned int startcondition)
{
#ifdef _SCC_DEBUG
    if(startcondition>=loadedStartcondition[start.getPlayerRace(player)].size()) {
        toLog("WARNING: (SETTINGS::setStartcondition): Value out of range.");return;
    }
#endif
	start.assignStartcondition(player, &(loadedStartcondition[start.getPlayerRace(player)][startcondition]));
}

void SETTINGS::setHarvestSpeed(const eRace race, const unsigned int harvest)
{
	// todo: checken ob harvest dazupasst
	start.setHarvestSpeed(race, &(loadedHarvestSpeed[harvest]));
}

void SETTINGS::setStartPosition(const unsigned int player, const unsigned int startPosition)
{
	start.setStartPosition(player, startPosition);
}

void SETTINGS::assignGoal(const unsigned int player, const unsigned int goal)
{
#ifdef _SCC_DEBUG
	if(goal>=loadedGoal[start.getPlayerRace(player)].size()) {
		toLog("WARNING: (SETTINGS::assignGoal): Value out of range.");return;
	}
#endif
	start.assignGoal(player, &loadedGoal[start.getPlayerRace(player)][goal]);
}

void SETTINGS::setBreedFactor(const unsigned int breedFactor)
{
#ifdef _SCC_DEBUG
	if((breedFactor<MIN_BREED_FACTOR)||(breedFactor>MAX_BREED_FACTOR)) {
		toLog("WARNING: (SETTINGS::setBreedFactor): Value out of range.");return;
	}
#endif
	ga.setBreedFactor(breedFactor);
}

void SETTINGS::setCrossOver(const unsigned int crossOver)
{
#ifdef _SCC_DEBUG
	if((crossOver<MIN_CROSSOVER)||(crossOver>MAX_CROSSOVER)) {
		toLog("WARNING: (SETTINGS::setCrossOver): Value out of range.");return;
	}
#endif
	ga.setCrossOver(crossOver);
}

const unsigned int SETTINGS::getBreedFactor() const
{
	return(ga.getBreedFactor());
}

const unsigned int SETTINGS::getCrossOver() const
{
	return(ga.getCrossOver());
}

const unsigned int SETTINGS::getMaxTime() const
{
	return(ga.maxTime);
}

const unsigned int SETTINGS::getMaxTimeOut() const
{
	return(ga.maxTimeOut);
}

const unsigned int SETTINGS::getMaxLength() const
{
	return(ga.maxLength);
}

const unsigned int SETTINGS::getMaxRuns() const
{
	return(ga.maxRuns);
}

const unsigned int SETTINGS::getMaxGenerations() const
{
	return(ga.maxGenerations);
}

const bool SETTINGS::getPreprocessBuildOrder() const
{
	return(ga.preprocessBuildOrder);
}

const unsigned int SETTINGS::getStartconditionCount(const unsigned int player) const
{
	return(loadedStartcondition[start.getPlayerRace(player)].size());
}

const unsigned int SETTINGS::getGoalCount(const unsigned int player) const
{
	return(loadedGoal[start.getPlayerRace(player)].size());
}

const unsigned int SETTINGS::getMapCount() const
{
	return(loadedMap.size());
}



const GOAL_ENTRY* SETTINGS::getGoal(const unsigned int player, const unsigned int goal) const

{
#ifdef _SCC_DEBUG
	if(goal>=loadedGoal[start.getPlayerRace(player)].size()) {
		toLog("WARNING: (SETTINGS::getGoal): Value out of range.");return(0);
	}
#endif
	return(&loadedGoal[start.getPlayerRace(player)][goal]);
}

const START_CONDITION* SETTINGS::getStartcondition(const unsigned int player, const unsigned int startconditionNumber) const
{
#ifdef _SCC_DEBUG
	if(startconditionNumber>=loadedStartcondition[start.getPlayerRace(player)].size()) {
		toLog("WARNING: (SETTINGS::getStartcondition): Value out of range.");return(0);
	}
#endif
	return(&loadedStartcondition[start.getPlayerRace(player)][startconditionNumber]);
}

const GA* SETTINGS::getGa() const
{
	return(&ga);
}
// --------------------------------------
// ------ END OF GET/SET FUNCTIONS ------
// --------------------------------------

